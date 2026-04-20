# Relatório: Parte 1 - Armazenamento e Processamento Local (Edge Computing)

## 1. Introdução e Contexto
Nesta fase do projeto **CardioIA**, o principal desafio é a construção de um sistema embarcado confiável para o monitoramento contínuo de sinais vitais de pacientes. Tratando-se da área da saúde, não basta apenas coletar os dados; é fundamental garantir sua persistência mesmo em cenários de instabilidade na conexão, configurando um caso clássico de **Edge Computing** (Computação de Borda).

## 2. Arquitetura de Hardware Simulado
O protótipo foi elaborado no simulador Wokwi estruturado com um **ESP32**, embarcando os seguintes periféricos:
1. **Sensor DHT22**: Responsável por simular e aferir a temperatura corpórea e a umidade. Adota o pino digital 19 para transmissão do barramento de dados.
2. **Potenciômetro Analógico**: Atua como o segundo sensor escolhido pela equipe. Ele se conecta ao pino analógico 35 e foi programado via código (`map(rawPot, 0, 4095, 50, 300)`) para emular o batimento cardíaco do paciente, variando de 50 a 300 BPM (Batimentos Por Minuto), viabilizando testes de estresse cardíaco.
3. **Slide Switch (Chave seletora)**: Conectada ao pino 12, funciona como o interruptor físico que desativa artificialmente a conectividade do dispositivo na camada de rede. Isso nos permite verificar com precisão o comportamento do algoritmo de resiliência do sistema quando a internet cai.

## 3. Fluxo de Funcionamento e Resiliência (Offline/Online)
O objetivo de aplicar Edge Computing aqui é garantir a **resiliência offline**: o sistema não pode falhar ou descartar dados críticos se a nuvem estiver inacessível. Devido à volatilidade de hardware real simulado (e para suprir a impossibilidade de gravar arquivos em um chip virtual via SPIFFS persistentemente após o reset do navegador), a estratégia implementada reside no armazenamento em memória estática via Buffer.

### 3.1 Captura de Dados (Edge)
O laço principal (`loop()`) roda sem bloquear a CPU, usando a função `millis()` para disparar a leitura sensoriada a cada 5 segundos. Os dados processados localmente são estruturados em um pacote (Struct C++ chamado `SensorDataPayload`) contendo Temperatura e BPM.

### 3.2 O Algoritmo de Resiliência
- **Estado Online**: A chave física está ligada e existe status `WL_CONNECTED`. O ESP32 realiza um push dos dados ao Broker em tempo real através do payload JSON.
- **Queda de Conexão (Estado Offline)**: A chave física é desativada. As requisições MQTT são congeladas. Ao invés de descartar o pacote lido, o microcontrolador o salva ordenadamente em um vetor `edgeBuffer` limitado a um limite predefinido (neste caso, 20 leituras por conta das restrições de memória e para respeitar cotas de Brokers públicos). O Monitor Serial registra log indicando: `[EDGE OFFLINE] Sem conexão! Dado salvo no buffer local...`.
- **Sincronização (Reconexão)**: Ao simularmos o restabelecimento da rede ligando o interruptor novamente, a lógica avalia a quantidade de itens retidos na variável `bufferIndex`. O sistema inicia o despejo dos dados ("sync") do buffer de forma inversa, esvaziando a carga para a nuvem. Adicionou-se  delay preventivo de 200ms para evitar interrupções de antispam do broker ("flood block").

## 4. Conclusão da Parte 1
Essa etapa demonstrou na prática o papel vital do Edge Computing em aplicações médicas e IoT aplicadas à saúde. Diferente de um sensor comum de temperatura residencial que pode simplesmente ignorar que perdeu a leitura, o projeto CardioIA garante que os rastros críticos do coração de um paciente fiquem retidos localmente e só sejam apagados assim que for enviada uma notificação confirmada à nuvem para processamento avançado.
