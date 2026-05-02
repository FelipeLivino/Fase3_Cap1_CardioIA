#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// --- CONFIGURAÇÕES DE REDE E MQTT ---
const char* WIFI_SSID = "Wokwi-GUEST"; 
const char* WIFI_PASSWORD = ""; 
const char* MQTT_SERVER = "broker.emqx.io";
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC = "fiap/cardioia/felipe/paciente_01";

// --- MAPEAMENTO DE PINOS (ESTÁVEL) ---
#define DHT_PINO 4      
#define DHT_TIPO DHT22
#define POT_PINO 35
#define SWITCH_REDE 25   // Pino seguro para o switch
#define SD_CS_PIN 5

// --- CONFIGURAÇÕES GERAIS ---
#define RECONNECT_INTERVAL_MS 5000
#define INTERVALO_COLETA_MS 5000

// --- BUFFER EM MEMÓRIA (Cache offline robusto) ---
#define CACHE_MAX 50  // Máximo de leituras armazenadas
struct Leitura {
  float temp;
  int bpm;
  float inclinacao;
};
Leitura cache[CACHE_MAX];
int cacheCount = 0;

// Instâncias
DHT dht(DHT_PINO, DHT_TIPO);
Adafruit_MPU6050 mpu;
WiFiClient espClient;
PubSubClient mqttClient(espClient);
TimerHandle_t wifiReconnectTimer;

unsigned long ultimoTempoColeta = 0;
bool sdOk = false;

// Protótipos
void conectarWiFi();
void WiFiEvent(WiFiEvent_t event);
void conectarMQTT();
void enviarDado(float temp, int bpm, float inclinacao, bool isHistorico);
void salvarCache(float temp, int bpm, float inclinacao);
void processarFilaCache();

void setup() {
  Serial.begin(115200);
  delay(500);
  
  pinMode(POT_PINO, INPUT);
  pinMode(SWITCH_REDE, INPUT_PULLUP);
  
  dht.begin();

  // Inicialização MPU6050
  if (!mpu.begin()) {
    Serial.println("[AVISO] MPU6050 não detectado.");
  }

  // --- TENTATIVA DE INICIALIZAÇÃO SD CARD ---
  Serial.println("[SD] Tentando iniciar cartão SD...");
  if (SD.begin(SD_CS_PIN)) {
    sdOk = true;
    Serial.println("[SD] Cartão SD inicializado com sucesso!");
  } else {
    Serial.println("[SD] Cartão SD indisponível. Usando cache em memória (RAM).");
  }
  
  Serial.printf("[INFO] Sistema pronto. Cache: %s (máx %d leituras)\n", 
                sdOk ? "SD Card" : "Memória RAM", CACHE_MAX);
  
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(RECONNECT_INTERVAL_MS), pdFALSE, (void*)0, [](TimerHandle_t xTimer){ conectarWiFi(); });
  WiFi.onEvent(WiFiEvent);
  conectarWiFi();
  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  bool redeDisponivel = (digitalRead(SWITCH_REDE) == HIGH);

  if (redeDisponivel && WiFi.status() == WL_CONNECTED) {
    if (!mqttClient.connected()) {
      conectarMQTT();
    }
    mqttClient.loop();
    if (mqttClient.connected()) {
      processarFilaCache();
    }
  }

  if (millis() - ultimoTempoColeta >= INTERVALO_COLETA_MS) {
    ultimoTempoColeta = millis();
    
    float t = dht.readTemperature();
    int bpm = map(analogRead(POT_PINO), 0, 4095, 50, 200);

    sensors_event_t a, g, temp_mpu;
    mpu.getEvent(&a, &g, &temp_mpu);
    float inclinacaoX = a.acceleration.x; 

    if (isnan(t)) return;

    Serial.printf("\n[STATUS] Rede: %s | Temp: %.1f | BPM: %d | Inc: %.2f\n", 
                  redeDisponivel ? "ON" : "OFF", t, bpm, inclinacaoX);

    if (redeDisponivel && mqttClient.connected()) {
      enviarDado(t, bpm, inclinacaoX, false);
    } else {
      salvarCache(t, bpm, inclinacaoX);
    }
  }
}

// --- FUNÇÕES DE CACHE (RAM com fallback SD) ---

void salvarCache(float temp, int bpm, float inclinacao) {
  // Tenta SD primeiro (se disponível)
  if (sdOk) {
    File dataFile = SD.open("/data.csv", FILE_APPEND);
    if (dataFile) {
      dataFile.printf("%.1f,%d,%.2f\n", temp, bpm, inclinacao);
      dataFile.close();
      Serial.println("[CACHE] Dado salvo no SD Card.");
      return;
    }
  }
  
  // Fallback: buffer em memória RAM
  if (cacheCount < CACHE_MAX) {
    cache[cacheCount].temp = temp;
    cache[cacheCount].bpm = bpm;
    cache[cacheCount].inclinacao = inclinacao;
    cacheCount++;
    Serial.printf("[CACHE] Dado salvo na RAM (%d/%d).\n", cacheCount, CACHE_MAX);
  } else {
    Serial.println("[CACHE] Buffer cheio! Dado descartado.");
  }
}

void processarFilaCache() {
  // Processa cache do SD (se disponível)
  if (sdOk && SD.exists("/data.csv")) {
    File dataFile = SD.open("/data.csv", FILE_READ);
    if (dataFile && dataFile.size() > 15) {
      Serial.println("[SYNC] Enviando cache SD → Cloud...");
      while (dataFile.available()) {
        String linha = dataFile.readStringUntil('\n');
        if (linha.length() > 5 && !linha.startsWith("temp")) {
          int v1 = linha.indexOf(',');
          int v2 = linha.lastIndexOf(',');
          float t = linha.substring(0, v1).toFloat();
          int b = linha.substring(v1 + 1, v2).toInt();
          float inc = linha.substring(v2 + 1).toFloat();
          enviarDado(t, b, inc, true);
          delay(100);
        }
      }
      dataFile.close();
      SD.remove("/data.csv");
    } else if (dataFile) {
      dataFile.close();
    }
  }
  
  // Processa cache da RAM
  if (cacheCount > 0) {
    Serial.printf("[SYNC] Enviando %d leituras da RAM → Cloud...\n", cacheCount);
    for (int i = 0; i < cacheCount; i++) {
      enviarDado(cache[i].temp, cache[i].bpm, cache[i].inclinacao, true);
      delay(100);
    }
    cacheCount = 0; // Limpa o buffer após envio
    Serial.println("[SYNC] Cache RAM sincronizado com sucesso!");
  }
}

void enviarDado(float temp, int bpm, float inclinacao, bool isHistorico) {
  JsonDocument doc;
  doc["temperatura"] = temp;
  doc["bpm"] = bpm;
  doc["postura"] = inclinacao;
  doc["tipo"] = isHistorico ? "CACHE" : "REALTIME";

  String jsonString;
  serializeJson(doc, jsonString);
  mqttClient.publish(MQTT_TOPIC, jsonString.c_str());
  Serial.printf("[MQTT] %s enviado.\n", isHistorico ? "Cache" : "Realtime");
}

void conectarWiFi() { WiFi.begin(WIFI_SSID, WIFI_PASSWORD); }
void WiFiEvent(WiFiEvent_t event) {
  if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) xTimerStop(wifiReconnectTimer, 0);
  else if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) xTimerStart(wifiReconnectTimer, 0);
}
void conectarMQTT() {
  String clientId = "ESP32-CardioIA-" + String(random(0xffff), HEX);
  mqttClient.connect(clientId.c_str());
}