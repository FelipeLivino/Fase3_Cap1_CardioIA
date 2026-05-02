# Relatório: Parte 2 - Transmissão para Nuvem e Visualização (Fog/Cloud Computing)

## 1. Introdução

Esta segunda etapa do projeto **CardioIA** descreve a camada de comunicação, processamento e visualização dos dados coletados na borda. Depois que o ESP32 simulado no Wokwi realiza a leitura dos sensores, a telemetria é transmitida via MQTT para um broker público, consumida pelo Node-RED, exposta como métricas para o Prometheus e visualizada em painéis no Grafana.

O objetivo arquitetural é separar responsabilidades: o dispositivo de borda coleta e publica dados, a camada MQTT desacopla produtores e consumidores, o Node-RED atua como camada de integração e transformação, o Prometheus armazena séries temporais e o Grafana fornece a visão analítica para acompanhamento dos sinais vitais.

## 2. Justificativas das Tecnologias de Cloud/Fog

As tecnologias da camada cloud/fog foram escolhidas para formar uma arquitetura modular, observável e reproduzível.

| Tecnologia | Justificativa |
|------------|---------------|
| **MQTT** | Protocolo leve e adequado para IoT, reduzindo tráfego de rede e permitindo comunicação assíncrona entre o dispositivo e os consumidores. |
| **EMQX** | Broker MQTT público estável para testes, eliminando a necessidade de provisionar um broker local durante a avaliação. |
| **Node-RED** | Camada de integração visual que facilita ingestão MQTT, transformação de mensagens, regras de alerta e exposição de métricas sem criar uma aplicação intermediária do zero. |
| **Prometheus** | Banco de séries temporais apropriado para métricas operacionais, com scraping periódico e consultas PromQL simples para cada sinal vital. |
| **Grafana** | Ferramenta de visualização analítica que se integra diretamente ao Prometheus e permite dashboards provisionados por arquivo, mantendo a observabilidade versionada no repositório. |
| **Docker Compose** | Orquestra os serviços locais com configuração padronizada, reduzindo divergências entre ambientes de execução. |

## 3. Fluxo de Comunicação MQTT

O protocolo **MQTT (Message Queuing Telemetry Transport)** foi adotado por ser leve, simples e adequado para cenários de IoT. Seu modelo de **publish/subscribe** evita acoplamento direto entre o ESP32 e os serviços de visualização, permitindo que múltiplos consumidores recebam a telemetria sem alterar o firmware.

### 3.1 Publicador: ESP32 CardioIA

O ESP32 coleta os sinais a cada 5 segundos e publica um payload JSON no broker MQTT. A versão atual transmite temperatura, batimentos cardíacos, postura/inclinação e a origem da leitura.

```json
{
  "temperatura": 36.5,
  "bpm": 82,
  "postura": 1.24,
  "tipo": "REALTIME"
}
```

O campo `tipo` diferencia leituras enviadas em tempo real (`REALTIME`) de leituras sincronizadas após uma queda de conexão (`CACHE`). Isso permite que a camada de visualização indique quando um dado veio da fila offline.

### 3.2 Broker MQTT: EMQX

O broker usado é o **EMQX público**, disponível em:

```text
broker.emqx.io:1883
```

A escolha pelo EMQX se justifica pela estabilidade do endpoint público e pela compatibilidade direta com clientes MQTT comuns, como a biblioteca `PubSubClient` usada no ESP32 e o nó MQTT do Node-RED.

### 3.3 Tópico de Publicação

A árvore de tópicos adotada foi:

```text
fiap/cardioia/felipe/paciente_01
```

Essa nomenclatura organiza domínio, projeto, responsável e paciente monitorado. Em uma expansão real, o padrão poderia evoluir para múltiplos pacientes, por exemplo `fiap/cardioia/<unidade>/<paciente_id>`.

### 3.4 Subscriber: Node-RED

O Node-RED assina o tópico MQTT e recebe todas as mensagens publicadas pelo dispositivo. Ele funciona como camada **Fog**, pois fica entre o dispositivo de borda e os serviços analíticos, realizando parse, transformação, regras de alerta e exposição de métricas.

## 4. Processamento no Node-RED

O fluxo do Node-RED foi atualizado para suportar os novos parâmetros do payload. Ele está salvo em dois pontos do projeto:

- `node_red_flow.json`: exportação para importação manual.
- `nodered_data/flows.json`: fluxo carregado automaticamente pelo container Docker.

### 4.1 Pipeline do Fluxo

O processamento segue as etapas abaixo:

1. **Entrada MQTT**: recebe mensagens do tópico `fiap/cardioia/felipe/paciente_01`.
2. **Parse JSON**: converte o payload recebido para objeto JavaScript.
3. **Extração de temperatura**: envia `temperatura` para o gráfico, gauge e métrica Prometheus.
4. **Extração de BPM**: envia `bpm` para o gráfico e métrica Prometheus.
5. **Extração de postura**: envia `postura` para o gráfico, gauge e métrica Prometheus.
6. **Extração do tipo de envio**: exibe `REALTIME` ou `CACHE` no dashboard.
7. **Análise de risco**: cruza temperatura, BPM e postura para indicar condição normal ou crítica.

### 4.2 Dashboard Local do Node-RED

O dashboard local em `http://localhost:1880/ui` apresenta:

- gráfico de evolução com as séries `Temperatura`, `BPM` e `Postura`;
- gauge de temperatura;
- gauge de postura/inclinação;
- texto com a origem da leitura (`REALTIME` ou `CACHE`);
- status do paciente com alerta visual.

### 4.3 Regras de Alerta

O nó de análise de risco considera o paciente em estado crítico quando qualquer uma das condições abaixo é verdadeira:

| Variável | Critério |
|----------|----------|
| BPM | `bpm > 120` |
| Temperatura | `temperatura > 38.0` |
| Postura | `abs(postura) > 7` |

Quando a mensagem possui `tipo: "CACHE"`, o alerta também informa que a leitura foi sincronizada do cache. Isso é importante para distinguir dados atuais de dados recuperados após indisponibilidade de rede.

## 5. Prometheus como Camada de Métricas

O Prometheus foi utilizado como banco de séries temporais da solução. O Node-RED expõe métricas no endpoint:

```text
http://nodered:1880/metrics
```

No ambiente Docker, o Prometheus acessa esse endpoint pela rede interna dos containers, conforme definido no `prometheus.yml`.

### 5.1 Métricas Exportadas

O fluxo Node-RED exporta três métricas independentes:

| Métrica | Origem | Finalidade |
|---------|--------|------------|
| `cardioia_temperatura` | Campo `temperatura` | Acompanhar variação térmica do paciente |
| `cardioia_bpm` | Campo `bpm` | Acompanhar ritmo cardíaco simulado |
| `cardioia_postura` | Campo `postura` | Acompanhar inclinação/postura pelo MPU6050 |

A decisão de separar as métricas evita problemas de tipagem, facilita consultas PromQL e simplifica a criação de painéis independentes no Grafana.

## 6. Grafana para Visualização Analítica

O Grafana foi utilizado como camada de visualização por integrar-se nativamente ao Prometheus e permitir provisionamento de dashboards por arquivo. O dashboard do projeto está versionado em:

```text
grafana/provisioning/dashboards/cardioia.json
```

Com isso, ao subir a stack Docker, o painel é carregado automaticamente, sem necessidade de configuração manual.

### 6.1 Painéis Configurados

O dashboard **CardioIA Sinais Vitais em Tempo Real** contém:

1. **Variação de Temperatura**: consulta `cardioia_temperatura`.
2. **Variação de Ritmo Cardíaco (BPM)**: consulta `cardioia_bpm`.
3. **Variação de Postura / Inclinação**: consulta `cardioia_postura`.

O painel de postura trabalha com faixa visual de `-10` a `10`, compatível com a leitura do acelerômetro no eixo X, e utiliza thresholds em `> 7` e `< -7` para sinalizar inclinações críticas.

### 6.2 Datasource

O datasource Prometheus é provisionado em:

```text
grafana/provisioning/datasources/datasource.yml
```

Essa estratégia reduz passos manuais e torna a entrega reproduzível: qualquer avaliador pode subir a stack e obter o mesmo dashboard.

## 7. Execução da Stack Cloud/Fog

A infraestrutura local é orquestrada por Docker Compose:

```bash
docker compose up -d
```

Serviços disponíveis:

| Serviço | URL | Responsabilidade |
|---------|-----|------------------|
| Node-RED | `http://localhost:1880` | Edição e execução do fluxo |
| Node-RED Dashboard | `http://localhost:1880/ui` | Visualização operacional em tempo real |
| Prometheus | `http://localhost:9090` | Consulta e coleta de métricas |
| Grafana | `http://localhost:3000` | Dashboards analíticos |

## 8. Conclusão

A camada Cloud/Fog do CardioIA completa o ciclo de monitoramento em saúde digital. O ESP32 publica telemetria de forma leve via MQTT, o Node-RED processa e enriquece os dados, o Prometheus armazena métricas temporais e o Grafana apresenta uma visão analítica dos sinais vitais.

Com a inclusão dos novos campos `postura` e `tipo`, a solução deixou de monitorar apenas temperatura e BPM e passou a representar também inclinação/postura e origem da leitura. Isso torna a arquitetura mais robusta para cenários de instabilidade, pois diferencia dados em tempo real de dados sincronizados do cache e amplia a capacidade de análise clínica simulada.
