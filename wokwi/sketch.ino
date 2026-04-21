#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// --- CONFIGURAÇÕES DE REDE E MQTT ---
const char* WIFI_SSID = "Wokwi-GUEST"; 
const char* WIFI_PASSWORD = ""; 
const char* MQTT_SERVER = "broker.emqx.io";
const int MQTT_PORT = 1883;
const char* MQTT_TOPIC = "fiap/cardioia/felipe/paciente_01";

// --- MAPEAMENTO DE PINOS ---
#define DHT_PINO 19
#define DHT_TIPO DHT22
#define POT_PINO 35
#define SWITCH_REDE 12 // Chave para simular queda de rede (Edge Computing)

// --- CONFIGURAÇÕES GERAIS ---
#define RECONNECT_INTERVAL_MS 5000
#define INTERVALO_COLETA_MS 5000

// Instâncias
DHT dht(DHT_PINO, DHT_TIPO);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
TimerHandle_t wifiReconnectTimer;

// --- ESTRUTURA DE DADOS (EDGE COMPUTING) ---
// Struct para armazenar dados localmente quando offline
struct SensorDataPayload {
  float temperatura;
  int bpm;
};

const int MAX_BUFFER = 20; // Limite de leituras offline
SensorDataPayload edgeBuffer[MAX_BUFFER];
int bufferIndex = 0; // Controla quantas leituras estão pendentes

unsigned long ultimoTempoColeta = 0;

// Declaração de funções
void conectarWiFi();
void WiFiEvent(WiFiEvent_t event);
void conectarMQTT();
void enviarDado(float temp, int bpm, bool isHistorico);

void setup() {
  Serial.begin(115200);
  
  pinMode(POT_PINO, INPUT);
  pinMode(SWITCH_REDE, INPUT_PULLUP);
  dht.begin();
  
  // Timer do FreeRTOS para tentar reconectar o Wi-Fi sem travar o loop
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(RECONNECT_INTERVAL_MS), pdFALSE, (void*)0, [](TimerHandle_t xTimer){ conectarWiFi(); });

  WiFi.onEvent(WiFiEvent);
  conectarWiFi();

  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  // Leitura da chave física (Simula se o ambiente está com internet ou não)
  bool redeDisponivelSimulada = (digitalRead(SWITCH_REDE) == HIGH);

  // Mantém o MQTT rodando e conectado se houver rede
  if (redeDisponivelSimulada && WiFi.status() == WL_CONNECTED) {
    if (!mqttClient.connected()) {
      conectarMQTT();
    }
    mqttClient.loop(); // Essencial para manter a conexão MQTT viva
  }

  // --- LÓGICA DE COLETA (Non-blocking) ---
  if (millis() - ultimoTempoColeta >= INTERVALO_COLETA_MS) {
    ultimoTempoColeta = millis();
    
    float t = dht.readTemperature();
    int rawPot = analogRead(POT_PINO);

    if (isnan(t)) {
      Serial.println("[ERRO] Falha crítica: Sensor DHT não respondeu!");
      return;
    }

    // Mapeia o potenciômetro para simular Batimentos Cardíacos (50 a 160 BPM)
    int bpm = map(rawPot, 0, 4095, 50, 300); 

    Serial.printf("\n[LEITURA] Temp: %.1f °C | Batimento: %d BPM\n", t, bpm);

    // --- LÓGICA DE EDGE COMPUTING E TRANSMISSÃO ---
    if (redeDisponivelSimulada && mqttClient.connected()) {
      
      // 1. Despejo do Buffer (Resiliência)
      while (bufferIndex > 0) {
        bufferIndex--;
        Serial.printf("[EDGE SYNC] Sincronizando dado retido %d/%d...\n", bufferIndex, MAX_BUFFER);
        enviarDado(edgeBuffer[bufferIndex].temperatura, edgeBuffer[bufferIndex].bpm, true);
        delay(200); // Previne banimento por flood no HiveMQ Público
      }

      // 2. Envio em Tempo Real
      enviarDado(t, bpm, false);

    } else {
      // 3. Sistema Offline - Salva localmente
      if (bufferIndex < MAX_BUFFER) {
        edgeBuffer[bufferIndex].temperatura = t;
        edgeBuffer[bufferIndex].bpm = bpm;
        bufferIndex++;
        Serial.printf("[EDGE OFFLINE] Sem conexão! Dado salvo no buffer local. Ocupação: %d/%d\n", bufferIndex, MAX_BUFFER);
      } else {
        Serial.println("[EDGE ALERTA] Buffer cheio! Sobrescrevendo dados mais antigos não suportado nesta versão.");
      }
    }
  }
}

// --- FUNÇÕES AUXILIARES ---

void enviarDado(float temp, int bpm, bool isHistorico) {
  // Uso do ArduinoJson v7 (Sem necessidade de Static/Dynamic sizing)
  JsonDocument doc;
  
  doc["temperatura"] = temp;
  doc["bpm"] = bpm;
  doc["tipo_dado"] = isHistorico ? "historico" : "tempo_real";

  String jsonString;
  serializeJson(doc, jsonString);

  if(mqttClient.publish(MQTT_TOPIC, jsonString.c_str())) {
    Serial.printf("[MQTT] Payload enviado: %s\n", jsonString.c_str());
  } else {
    Serial.println("[MQTT] Falha ao enviar payload.");
  }
}

void conectarWiFi() {
  Serial.println("[WIFI] Iniciando conexão...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("[WIFI] Conectado! IP: ");
      Serial.println(WiFi.localIP());
      xTimerStop(wifiReconnectTimer, 0);
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("[WIFI] Caiu! Agendando reconexão via FreeRTOS...");
      xTimerStart(wifiReconnectTimer, 0);
      break;
    default: break;
  }
}

void conectarMQTT() {
  Serial.print("[MQTT] Tentando conectar ao broker...");
  String clientId = "ESP32-CardioIA-" + String(random(0xffff), HEX);
  
  if (mqttClient.connect(clientId.c_str())) {
    Serial.println(" Conectado!");
  } else {
    Serial.printf(" Falha, rc=%d. Tentará novamente no próximo ciclo.\n", mqttClient.state());
  }
}