# Relatório: Parte 1 - Armazenamento e Processamento Local (Edge Computing)

## 1. Introdução e Contexto
Nesta fase do projeto **CardioIA**, o principal desafio é a construção de um sistema embarcado confiável para o monitoramento contínuo de sinais vitais de pacientes. Tratando-se da área da saúde, não basta apenas coletar os dados; é fundamental garantir sua retenção local mesmo em cenários de instabilidade na conexão, configurando um caso clássico de **Edge Computing** (Computação de Borda).

O protótipo atualizado no Wokwi simula um nó de borda com coleta periódica de temperatura, batimentos cardíacos e postura/inclinação. Quando a conectividade está indisponível, o ESP32 armazena as leituras localmente em cache e as sincroniza com a nuvem assim que a conexão MQTT volta a estar operacional.

## 2. Arquitetura de Hardware Simulado
O protótipo foi elaborado no simulador Wokwi estruturado com um **ESP32**, embarcando os seguintes periféricos:

1. **Sensor DHT22**: Responsável por simular a temperatura corpórea. O barramento de dados está conectado ao GPIO 4.
2. **Potenciômetro Analógico**: Atua como sensor simulado de batimentos cardíacos. Ele se conecta ao GPIO 35 e é convertido no código com `map(analogRead(POT_PINO), 0, 4095, 50, 200)`, emulando uma faixa de 50 a 200 BPM.
3. **Acelerômetro MPU6050**: Adicionado para monitoramento de postura/inclinação do paciente. A comunicação I2C utiliza SDA no GPIO 21 e SCL no GPIO 22.
4. **Slide Switch (Chave seletora)**: Conectada ao GPIO 25 com `INPUT_PULLUP`, funciona como interruptor físico para simular rede disponível ou indisponível.
5. **MicroSD Card**: Adicionado ao circuito para representar armazenamento persistente em hardware real. No barramento SPI, utiliza CS no GPIO 5, MOSI no GPIO 23, SCK no GPIO 18 e MISO no GPIO 19.

## 3. Fluxo de Funcionamento e Resiliência (Offline/Online)
O objetivo de aplicar Edge Computing aqui é garantir a **resiliência offline**: o sistema não deve interromper a coleta nem descartar imediatamente dados críticos se a nuvem estiver inacessível. A versão atual implementa uma arquitetura de cache em duas camadas:

1. **Cache primário em SD Card**: quando o cartão é inicializado com sucesso, as leituras são gravadas em `/data.csv`.
2. **Fallback em RAM**: quando o SD não está disponível, o sistema armazena até 50 leituras em um vetor local (`cache`).

Essa abordagem mantém compatibilidade com hardware real, onde o SD Card pode operar como armazenamento persistente, e também garante funcionamento no simulador Wokwi, onde o cartão virtual pode apresentar limitação de inicialização SPI.

### 3.1 Captura de Dados (Edge)
O laço principal (`loop()`) roda sem bloquear a CPU, usando `millis()` para disparar a coleta a cada 5 segundos. Em cada ciclo são obtidos:

- `temperatura`: leitura do DHT22.
- `bpm`: valor convertido a partir do potenciômetro.
- `postura`: eixo X de aceleração do MPU6050, usado como indicador de inclinação.

Antes da transmissão, os dados são empacotados em JSON com os campos `temperatura`, `bpm`, `postura` e `tipo`. O campo `tipo` identifica se o envio é `REALTIME` ou `CACHE`.

### 3.2 O Algoritmo de Resiliência
- **Inicialização**: No `setup()`, o ESP32 inicializa DHT22, MPU6050, Wi-Fi, MQTT e tenta iniciar o SD Card com `SD.begin(SD_CS_PIN)`. Se o SD falhar, `sdOk` permanece falso e o sistema passa a usar RAM.
- **Estado Online**: A chave física está ligada e existe status `WL_CONNECTED`. O ESP32 mantém a conexão MQTT com o broker `broker.emqx.io` e publica os dados no tópico `fiap/cardioia/felipe/paciente_01`.
- **Queda de Conexão (Estado Offline)**: A chave física é desativada ou o MQTT não está conectado. Ao invés de descartar a leitura, o microcontrolador chama `salvarCache()`. A função tenta gravar no SD Card primeiro; se não conseguir, grava no vetor em RAM até o limite de 50 leituras.
- **Sincronização (Reconexão)**: Ao restabelecer a rede, `processarFilaCache()` envia primeiro as linhas pendentes do arquivo `/data.csv`, remove o arquivo após a leitura e depois envia as leituras acumuladas na RAM. Os envios de cache são marcados com `tipo: "CACHE"` no payload JSON.

### 3.3 Limitação Observada no Wokwi
Durante a simulação, foi considerada a limitação do componente `wokwi-microsd-card` com o ESP32 no Wokwi. A tentativa de inicialização do SD pode falhar no ambiente virtual, mesmo com o barramento SPI configurado. Por isso, o projeto não depende exclusivamente do cartão SD para demonstrar resiliência offline: a RAM assume automaticamente o papel de cache secundário.

## 4. Conclusão da Parte 1
Essa etapa demonstrou na prática o papel vital do Edge Computing em aplicações médicas e IoT aplicadas à saúde. Diferente de sensores simples que podem ignorar leituras perdidas, o CardioIA mantém uma estratégia local de retenção e sincronização posterior, reduzindo a perda de telemetria em momentos de instabilidade de rede.

A atualização do Wokwi também amplia o escopo clínico da simulação: além de temperatura e BPM, o sistema passa a observar postura/inclinação por meio do MPU6050 e documenta uma estratégia realista de armazenamento local com SD Card e fallback em RAM.
