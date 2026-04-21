# Relatório: Parte 2 - Transmissão para Nuvem e Visualização (Fog/Cloud Computing)

## 1. Introdução
Esta segunda etapa do trabalho tangibiliza a comunicação de dados, subindo do plano físico (Borda) para a abstração na Nuvem (Cloud/Fog Computing). O objetivo principal é receber os sinais vitais, transportá-los de maneira leve e eficaz sob redes instáveis usando MQTT, e exibi-los em tempo real de forma amigável e crítica (com alertas automatizados) em um Dashboard centralizado. 

## 2. Fluxo de Comunicação MQTT
O protocolo **MQTT (Message Queuing Telemetry Transport)** foi adotado como o padrão de telemetria devido ao seu bajíssimo overhead de rede baseado no modelo de Publish/Subscribe.
1. **Publicador (ESP32 da CardioIA)**: Coleta a cada 5 segundos os dados (Temperatura em °C e BPM do pulso) e estrutura-os em JSON da seguinte forma:
   ```json
   {
      "temperatura": 36.5,
      "bpm": 80,
      "tipo_dado": "tempo_real" 
   }
   ```
2. **Broker MQTT Cloud (EMQX)**: Na prática, para fugir da instabilidade extrema dos servidores HiveMQ gratuitos, adotamos o cluster público robusto do EMQX (`broker.emqx.io`), porta 1883.
3. **Topic Tree (Tópicos)**: A árvore de publicação obedeceu à padronização lógica `fiap/cardioia/felipe/paciente_01`.
4. **Subscriber (Node-RED)**: Na nuvem ou numa rede interna (Fog), instanciou-se o Node-RED, que fica à escuta (Subscribe) do mesmo tópico para interceptar todos os JSONs em tempo real.

## 3. Configuração do Dashboard no Node-RED
O fluxo do Node-RED gerado contém a pipeline visual do projeto:
- **Parse Inteligente**: O nó de MQTT injeta os dados num Parse de JSON, convertendo a string de volta para objeto de máquina JavaScript.
- **Divisão Lógica**: Funções separadas interceptam e formatam especificamente `msg.payload.temperatura` ou `msg.payload.bpm`.
- **Evolução Contínua (Chart)**: Um gráfico de linhas em tempo real é atualizado mesclando a pulsação junto da temperatura na mesma tela.
- **Termômetro Analógico (Gauge)**: Para fornecer rápida legibilidade humana, usou-se o layout de velocímetro para a temperatura. Cores predefinidas configuram o estado do mostrador visualmente (verde até 35, amarelo até 38, vermelho após 38).
- **Indicador de Risco / Alertas Automáticos**: O core lógico da aplicação fica centralizado em um nó de função específico. Ele cruza as duas variáveis sob premissas programadas. 
  - Se o BPM estiver acima de 120 e/ou Temperatura acima de 38°C, a string disparada para a camada de visualização é alterada para `"ALERTA: Sinais Vitais Críticos!"`, colorindo automaticamente a fonte em vermelho, alertando clinicamente enfermeiros sobre o risco do paciente. Do contrário exibe "Estável" em verde.

## 4. Integração Analítica: Grafana Cloud
Visando expandir a prova de conceito com ferramentas modernas de Observabilidade Empresarial, efetuou-se a integração com o **Grafana Cloud**. 
Diferente da dashboard local do Node-RED e seu painel restrito, o Grafana permite uma agregação complexa e armazenamento em séries temporais (TSDB) resiliente.

### Passo a passo executado para esta integração:
1. **Conexão via Banco de Séries Temporais ou Plugin Opcional**: Como o Grafana, por natureza construtiva, depende de uma base de dados externa (Datasource), podemos direcionar as requisições de duas formas. A abordagem utilizada envolve interceptar as requisições no estágio do Node-RED.
2. **Exportação via Prometheus Node**: No fluxo do Node-RED do projeto, conectamos um nó Prometheus (`prometheus-out`) no barramento pós-Parse. Esse nó expõe uma métrica no formato `<HOST>:1880/metrics` rotulada como `cardioia_sinais_vitais`.
3. **Scraping e Configuração do Dashboard no Grafana Cloud**:
    - Acessou-se o ecossistema Grafana Cloud.
    - Na aba *Connections/Data Sources*, adicionou-se a conectividade com o banco/endpoint configurado via Prometheus.
    - Criou-se um novo *Panel Dashboard*. Na consulta avançada (PromQL), puxando a métrica contínua do coração agrupada num intervalo de tempos longos (ex: das últimas 24h).
    - Essa integração viabilizou cruzamentos complexos e disparos de Webhook diretamente a aplicações com Slack ou chamadas aos médicos, ratificando a maturidade corporativa da stack escolhida para as entregas de Healthtech.

_Isso encerra o ciclo completo de monitoramento em saúde digital proposto pelo problema, integrando coleta em borda, transmissão assíncrona leve e interface gráfica de consumo unificado._
