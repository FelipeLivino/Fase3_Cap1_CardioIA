# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
  <a href="https://www.fiap.com.br/">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/logo-fiap.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="40%" height="40%">
  </a>
</p>

<br>

# CardioIA

## Fase 3: IoT e Visualização de Dados para a Saúde Digital

O **CardioIA** é uma prova de conceito de arquitetura IoT para monitoramento de sinais vitais em saúde digital. A solução simula um dispositivo de borda com ESP32 no Wokwi, publica telemetria via MQTT, processa os dados no Node-RED, expõe métricas para o Prometheus e apresenta indicadores operacionais no Grafana.

O foco técnico desta fase é demonstrar uma malha de aquisição, transmissão, resiliência offline e observabilidade em tempo real, com separação clara entre borda, mensageria, processamento e visualização.

## Grupo

**Rumo ao NEXT**

## Integrantes

- Felipe Livino dos Santos (RM 563187)
- Daniel Veiga Rodrigues de Faria (RM 561410)
- Tomas Haru Sakugawa Becker (RM 564147)
- Daniel Tavares de Lima Freitas (RM 562625)
- Gabriel Konno Carrozza (RM 564468)

## Professores

**Tutor:** Caique Nonato da Silva Bezerra

**Coordenador:** André Godoi Chiovato

## Visão Arquitetural

```text
┌─────────────────────────┐
│ Edge / Wokwi             │
│ ESP32 + sensores         │
│ DHT22 + BPM + MPU6050    │
│ Cache SD Card -> RAM     │
└───────────┬─────────────┘
            │ MQTT
            ▼
┌─────────────────────────┐
│ Broker EMQX público      │
│ broker.emqx.io:1883      │
└───────────┬─────────────┘
            │ Subscribe
            ▼
┌─────────────────────────┐
│ Node-RED                 │
│ Parse, validação, UI     │
│ Exporter Prometheus      │
└───────────┬─────────────┘
            │ /metrics
            ▼
┌─────────────────────────┐
│ Prometheus               │
│ Coleta temporal          │
└───────────┬─────────────┘
            │ Datasource
            ▼
┌─────────────────────────┐
│ Grafana                  │
│ Dashboard provisionado   │
└─────────────────────────┘
```

## Componentes

| Camada | Tecnologia | Responsabilidade |
|--------|------------|------------------|
| Edge | Wokwi + ESP32 | Coleta de temperatura, BPM e postura; cache offline; publicação MQTT |
| Sensores | DHT22, potenciômetro, MPU6050 | Simulação de temperatura corpórea, batimentos e inclinação/postura |
| Mensageria | EMQX público | Broker MQTT para desacoplamento entre edge e processamento |
| Processamento | Node-RED | Recepção MQTT, extração de campos, alerta, dashboard simples e exportação Prometheus |
| Séries temporais | Prometheus | Coleta das métricas publicadas pelo Node-RED |
| Observabilidade | Grafana | Painéis para temperatura, BPM e postura/inclinação |

## Justificativas Técnicas

As tecnologias foram escolhidas para manter a solução modular, reproduzível e aderente a uma arquitetura IoT realista, sem criar dependência entre coleta, processamento e visualização.

| Tecnologia | Por que foi usada |
|------------|-------------------|
| **ESP32** | É um microcontrolador com Wi-Fi integrado, baixo custo e ampla adoção em IoT. Permite simular um dispositivo de borda capaz de coletar dados, tomar decisões locais e publicar telemetria sem depender de um computador intermediário. |
| **Wokwi** | Permite validar o circuito e o firmware diretamente no navegador, sem exigir hardware físico. Isso facilita testes acadêmicos, reprodução do projeto e demonstração do comportamento offline/online. |
| **DHT22** | Foi usado para representar temperatura corpórea de forma simples e didática. Embora seja um sensor ambiental, atende bem ao objetivo da prova de conceito de telemetria térmica. |
| **Potenciômetro** | Simula o BPM de forma controlável durante a execução. Essa escolha facilita testar cenários normais e críticos sem depender de um sensor cardíaco real. |
| **MPU6050** | Adiciona uma dimensão de postura/inclinação ao monitoramento. Em um contexto de saúde, essa leitura pode representar movimentação anormal, queda, mudança brusca de posição ou risco postural. |
| **MicroSD** | Representa armazenamento local persistente em hardware real. Ele é importante para demonstrar resiliência quando a rede falha e os dados precisam ser preservados na borda. |
| **Buffer em RAM** | Funciona como fallback quando o SD Card não está disponível no simulador. Garante que a lógica de cache offline continue demonstrável mesmo com limitações do Wokwi. |
| **MQTT** | É um protocolo leve, baseado em publish/subscribe, muito usado em IoT. Ele desacopla o ESP32 dos consumidores dos dados, permitindo que Node-RED, monitoramento e outros serviços recebam a telemetria sem alterar o firmware. |
| **EMQX público** | Foi escolhido como broker MQTT por oferecer endpoint público estável para testes e integração rápida, sem exigir a criação de um broker local na máquina do avaliador. |
| **Node-RED** | É adequado para integração IoT porque permite montar fluxos de ingestão, transformação, alerta e exposição de métricas com baixo atrito. Também facilita visualizar rapidamente se os dados estão chegando. |
| **Prometheus** | Foi usado para armazenar e consultar métricas temporais. Ele é o padrão de facto em observabilidade para coleta periódica e combina bem com exporters expostos por serviços como o Node-RED. |
| **Grafana** | Foi escolhido para a camada analítica e visual. Ele consome o Prometheus como datasource e permite criar dashboards provisionados por arquivo, garantindo que a visualização seja versionada junto com o projeto. |
| **Docker Compose** | Padroniza a execução local dos serviços. Com um único comando, Node-RED, Prometheus e Grafana sobem com portas, volumes e configurações consistentes. |

Essa combinação separa responsabilidades: o ESP32 coleta e decide localmente, o MQTT transporta mensagens, o Node-RED processa eventos, o Prometheus guarda séries temporais e o Grafana apresenta os indicadores.

## Fluxo de Dados

1. O ESP32 lê os sensores a cada 5 segundos.
2. O payload é montado em JSON e classificado como `REALTIME` ou `CACHE`.
3. Se a rede estiver disponível, o payload é publicado no tópico MQTT.
4. Se a rede estiver indisponível, a leitura é armazenada em cache local.
5. Ao reconectar, o ESP32 sincroniza as leituras pendentes.
6. O Node-RED assina o tópico MQTT, extrai os campos e alimenta a UI e o exporter.
7. O Prometheus coleta as métricas.
8. O Grafana apresenta os painéis provisionados automaticamente.

## Payload MQTT

**Tópico:**

```text
fiap/cardioia/felipe/paciente_01
```

**Exemplo de mensagem:**

```json
{
  "temperatura": 36.5,
  "bpm": 82,
  "postura": 1.24,
  "tipo": "REALTIME"
}
```

| Campo | Tipo | Descrição |
|-------|------|-----------|
| `temperatura` | number | Temperatura corpórea simulada em graus Celsius |
| `bpm` | number | Batimentos por minuto simulados pelo potenciômetro |
| `postura` | number | Aceleração no eixo X do MPU6050, usada como indicador de inclinação |
| `tipo` | string | Origem da leitura: `REALTIME` para envio imediato ou `CACHE` para dado sincronizado |

## Edge Computing e Resiliência Offline

O firmware implementa uma estratégia de cache em duas camadas:

| Camada | Tipo | Persistência | Uso esperado |
|--------|------|--------------|--------------|
| SD Card | Primária | Persistente | Hardware real |
| RAM Buffer | Fallback | Volátil | Simulador Wokwi |

No `setup()`, o ESP32 tenta inicializar o cartão SD. Se o SD estiver disponível, as leituras offline são gravadas em `/data.csv`. Se o SD não estiver disponível, o sistema usa um buffer em RAM com capacidade para 50 leituras.

Essa abordagem evita que a coleta seja interrompida por falha de rede ou limitação do simulador. Quando a conectividade volta, o dispositivo processa a fila de cache e publica os dados pendentes no mesmo tópico MQTT, marcando essas mensagens com `tipo: "CACHE"`.

## Observabilidade

### Node-RED

O fluxo está disponível em dois arquivos:

- `node_red_flow.json`: exportação para importação manual no Node-RED.
- `nodered_data/flows.json`: fluxo carregado pelo container via volume Docker.

O fluxo atual:

- recebe mensagens MQTT do EMQX;
- converte o payload para JSON;
- extrai `temperatura`, `bpm`, `postura` e `tipo`;
- exibe valores em dashboard local;
- aplica regra de alerta para sinais vitais e postura;
- exporta métricas para Prometheus.

### Métricas Prometheus

| Métrica | Descrição |
|---------|-----------|
| `cardioia_temperatura` | Temperatura corpórea simulada |
| `cardioia_bpm` | Batimentos por minuto simulados |
| `cardioia_postura` | Inclinação/postura no eixo X do MPU6050 |

### Grafana

O dashboard provisionado em `grafana/provisioning/dashboards/cardioia.json` contém:

- Variação de temperatura.
- Variação de ritmo cardíaco.
- Variação de postura/inclinação.

## Estrutura do Projeto

```text
├── asset/                          # Pasta com ativos visuais
│   └── logo-fiap.png               # Logo da FIAP
├── grafana/                        # Configurações para Grafana
│   └── provisioning/               # Pasta de provisionamento
│       ├── dashboards/             # Dashboards provisionados
│       │   ├── dashboard.yml       # Arquivo de configuração do dashboard
│       │   └── cardioia.json       # Definição do dashboard CardioIA
│       └── datasources/            # Fontes de dados
│           └── datasource.yml      # Configuração da fonte de dados Prometheus
├── nodered_data/                   # Dados para Node-RED
│   ├── flows.json                  # Fluxo do Node-RED
│   ├── package.json                # Dependências do Node-RED
│   ├── package-lock.json           # Lockfile das dependências
│   └── settings.js                 # Configurações do Node-RED
├── wokwi/                          # Arquivos para simulação no Wokwi
│   ├── diagram.json                # Diagrama do circuito
│   ├── libraries.txt               # Bibliotecas necessárias
│   ├── sketch.ino                  # Código do firmware ESP32
│   └── wokwi-project.txt           # Configuração do projeto Wokwi
├── docker-compose.yml              # Arquivo para subir os serviços com Docker Compose
├── node_red_flow.json              # Exportação do fluxo Node-RED para importação manual
├── prometheus.yml                  # Configuração do Prometheus
├── relatorio_parte1_edge.md        # Relatório da parte 1 (edge)
└── relatorio_parte2_cloud.md       # Relatório da parte 2 (cloud)
```

## Pré-Requisitos

- Docker Engine com Docker Compose.
- Navegador para executar a simulação no Wokwi.
- Acesso de rede ao broker MQTT `broker.emqx.io` na porta `1883`.
- Portas locais disponíveis:

| Serviço | Porta | URL |
|---------|-------|-----|
| Node-RED | 1880 | `http://localhost:1880` |
| Node-RED Dashboard | 1880 | `http://localhost:1880/ui` |
| Prometheus | 9090 | `http://localhost:9090` |
| Grafana | 3000 | `http://localhost:3000` |

## Como Executar

1. Clone o repositório e acesse a pasta do projeto.

```bash
git clone <url-do-repositorio>
cd ex_fase3_cap1
```

2. Suba a stack local.

```bash
docker compose up -d
```

3. Acesse o Node-RED.

```text
http://localhost:1880
```

Se necessário, importe manualmente o arquivo `node_red_flow.json` e faça o deploy.

4. Execute a simulação no Wokwi usando os arquivos da pasta `wokwi/`, ou pelo link <a href="https://wokwi.com/projects/461851222553172993">https://wokwi.com/projects/461851222553172993</a>.

5. No Wokwi, deixe a chave de rede ligada para envio em tempo real. Desligue a chave para simular queda de conectividade e validar o cache offline.

6. Acompanhe a UI do Node-RED.

```text
http://localhost:1880/ui
```

7. Acesse o Grafana.

```text
http://localhost:3000
```

Credenciais padrão:

```text
Usuário: admin
Senha: admin
```

## Dependências do Wokwi

As bibliotecas usadas no firmware estão declaradas em `wokwi/libraries.txt`:

- `DHT sensor library`
- `PubSubClient`
- `ArduinoJson`
- `Adafruit MPU6050`

## Critérios de Alerta

O fluxo do Node-RED considera o paciente em alerta quando qualquer uma das condições abaixo ocorre:

| Condição | Limite |
|----------|--------|
| BPM elevado | `bpm > 120` |
| Temperatura elevada | `temperatura > 38.0` |
| Inclinação crítica | `abs(postura) > 7` |

Quando o dado vem de sincronização offline, a mensagem do dashboard também indica que a leitura foi recuperada do cache.

## Limitações Conhecidas

Durante o desenvolvimento foi observada uma limitação do componente `wokwi-microsd-card` com ESP32 no simulador Wokwi. A inicialização SPI do cartão pode falhar no ambiente virtual, mesmo com pinos e bibliotecas configurados.

Por esse motivo, o projeto usa fallback automático para RAM no simulador. Em hardware real, o SD Card permanece como estratégia primária de armazenamento persistente.

Também não fazem parte deste repositório bases externas pesadas, como datasets clínicos, imagens ou arquivos de treinamento de modelos. Esses ativos devem ser mantidos em armazenamento externo e consumidos sob demanda.

## Relatórios

- `relatorio_parte1_edge.md`: detalha a implementação de borda, sensores, cache local e resiliência offline.
- `relatorio_parte2_cloud.md`: detalha a arquitetura de nuvem, processamento e visualização.


## Demonstrações de fluxos

- Funcionamento wokwi
<p align="center">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/wokwi.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="60%">
</p>

- Fluxo Node-RED
<p align="center">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/node-red-fluxo.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0"width="60%">
</p>

- Node-RED Dashboards

<p align="center">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/node-red.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="60%">
</p>

- Node-RED Dashboards AVISOS
<p align="center">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/node-red-aviso.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="60%">
</p>

- Dashboards Grafana
<p align="center">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/grafana.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="60%">
</p>


## Histórico de Lançamentos

### 0.3.0 - 09/05/2026

- Atualização do fluxo Node-RED para suportar `postura` e `tipo`.
- Inclusão da métrica Prometheus `cardioia_postura`.
- Atualização do dashboard Grafana com painel de postura/inclinação.
- Reorganização arquitetural da documentação do projeto.

### 0.2.0 - 01/05/2026

- Implementação da arquitetura de cache dual-mode: SD Card com fallback em RAM.
- Adição do sensor MPU6050 para monitoramento de postura/inclinação.
- Documentação da limitação do simulador Wokwi com o componente `wokwi-microsd-card`.

### 0.1.0 - 20/04/2026

- Refatoração da infraestrutura Node-RED e Prometheus.
- Alteração do broker MQTT padrão de HiveMQ para EMQX.
- Finalização da base da Fase 3 e adequação acadêmica.

## Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1">
<p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms">
  <a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por
  <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">FIAP</a>
  está licenciado sobre
  <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.
</p>
