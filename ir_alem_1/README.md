# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
  <a href="https://www.fiap.com.br/">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/logo-fiap.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="40%" height="40%">
  </a>
</p>

<br>

# CardioIA

## IR Alem 1 - Comunicacao automatizada com REST e e-mail

Sistema academico de monitoramento de sinais vitais, implementado em Python, com
comunicacao REST, interface web, validacao de risco clinico e automacao de alerta por
e-mail simulado.

O projeto foi desenhado para demonstrar, em pequena escala, um fluxo comum de saude
digital: uma aplicacao recebe dados de pacientes, aplica regras de negocio sobre os
sinais vitais e aciona uma notificacao automatizada quando existe risco operacional ou
clinico.

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

## Sumario executivo

A solucao expoe uma API REST responsavel por receber leituras de sinais vitais,
armazenar os registros em memoria, calcular alertas e disponibilizar os dados para
consulta. Sobre essa API existe uma tela web simples, servida pela propria aplicacao,
com duas areas principais: a lista de dados recebidos e o formulario de envio de novas
leituras.

O envio de e-mail foi implementado como simulacao controlada. Em vez de depender de
credenciais reais de SMTP, o sistema grava a mensagem de alerta em `outbox/alertas.log`.
Essa decisao mantem a entrega reproduzivel, segura e adequada ao contexto academico,
sem comprometer a demonstracao do conceito de automacao.

## Visao de arquitetura

```text
Usuario / Operador
      |
      | acessa tela web
      v
Interface Web em /              Cliente REST demo.py
      |                                  |
      | POST /vitals                     | POST /vitals
      v                                  v
API REST Python -------------------> Motor de regras
      |                                  |
      | GET /vitals                      | detecta risco
      v                                  v
Registros em memoria              Servico de e-mail simulado
                                             |
                                             v
                                      outbox/alertas.log
```

### Responsabilidades por componente

| Componente | Responsabilidade |
| --- | --- |
| `api_server.py` | Servir a API REST, validar payloads, armazenar leituras, servir arquivos estaticos e coordenar alertas. |
| `risk_checker.py` | Centralizar as regras de deteccao de risco para manter a logica clinica isolada. |
| `email_service.py` | Simular o envio automatizado de e-mail gravando mensagens em arquivo local. |
| `rest_client.py` | Demonstrar consumo programatico da API usando HTTP. |
| `demo.py` | Executar um cenario completo de envio e consulta de leituras. |
| `static/` | Entregar a tela web em React usada para visualizar registros e enviar novas leituras. |
| `Dockerfile` | Empacotar a aplicacao Python em uma imagem executavel. |
| `docker-compose.yml` | Orquestrar a API e, opcionalmente, o fluxo de demonstracao. |

## Estrutura do projeto

```text
.
|-- api_server.py              # Servir a API REST, validar payloads, armazenar leituras, servir arquivos estáticos e coordenar alertas
|-- rest_client.py              # Demonstrar consumo programático da API usando HTTP
|-- risk_checker.py             # Centralizar as regras de detecção de risco para manter a lógica clínica isolada
|-- email_service.py            # Simular o envio automatizado de e-mail gravando mensagens em arquivo local
|-- demo.py                     # Executar um cenário completo de envio e consulta de leituras
|-- static/                     # Entregar a tela web usada para visualizar registros e enviar novas leituras
|   |-- index.html              # Página HTML com o ponto de montagem do React
|   |-- styles.css              # Estilos CSS para a interface web
|   `-- app.js                  # Componentes React e chamadas à API
|-- Dockerfile                  # Empacotar a aplicação Python em uma imagem executável
|-- docker-compose.yml          # Orquestrar a API e, opcionalmente, o fluxo de demonstração
|-- relatorio.md                # Relatório do projeto
|-- .dockerignore               # Arquivo para ignorar arquivos no build do Docker
|-- .gitignore                  # Arquivo para ignorar arquivos no Git
`-- outbox/                     # Criado automaticamente quando alertas forem gerados
```

## Fluxo funcional

1. O usuario acessa `http://localhost:8000`.
2. A interface web consulta `GET /vitals` para carregar as leituras recebidas.
3. O usuario preenche os campos do formulario e envia uma nova leitura.
4. A tela chama `POST /vitals` com um payload JSON.
5. A API valida os dados obrigatorios e os limites basicos de entrada.
6. A leitura e armazenada em memoria com `id` e `received_at`.
7. O modulo `risk_checker.py` avalia as regras de risco.
8. Se houver alerta, `email_service.py` registra a mensagem em `outbox/alertas.log`.
9. A tela atualiza a listagem com o novo registro e o status do alerta.

## Regras de risco

| Risco | Condicao aplicada |
| --- | --- |
| Taquicardia | `heart_rate > 120` |
| Febre | `temperature >= 38.0` |
| Ausencia de movimento | `movement_detected == false` |

Uma mesma leitura pode gerar mais de um alerta. Por exemplo, uma leitura com
frequencia cardiaca de `130`, temperatura de `38.4` e ausencia de movimento gera tres
alertas independentes.

## Como executar com Docker Compose

### Subir a API e a tela web

```bash
docker compose up --build api
```

Depois acesse:

```text
http://localhost:8000
```

### Executar o fluxo automatico de demonstracao

```bash
docker compose --profile demo up --build
```

Esse comando sobe a API e executa o container `demo`, que envia leituras simuladas para
o servico. Quando houver risco, o arquivo `outbox/alertas.log` sera criado no diretorio
do projeto.

### Encerrar os containers

```bash
docker compose down
```

## Como executar localmente

Use dois terminais.

No primeiro terminal, inicie a API:

```bash
python api_server.py
```

Acesse a tela web:

```text
http://127.0.0.1:8000
```

No segundo terminal, execute a demonstracao:

```bash
python demo.py
```

Tambem e possivel testar apenas o cliente REST:

```bash
python rest_client.py
```

## Variaveis de ambiente

| Variavel | Padrao | Uso |
| --- | --- | --- |
| `API_HOST` | `127.0.0.1` | Endereco em que a API escuta. No Docker usa `0.0.0.0`. |
| `API_PORT` | `8000` | Porta HTTP da API. |
| `VITALS_API_BASE_URL` | `http://127.0.0.1:8000` | URL consumida pelo cliente REST e pelo `demo.py`. |
| `ALERT_RECIPIENT` | `equipe.monitoramento@hospital.example` | Destinatario usado na simulacao de e-mail. |

## Endpoints REST

### `GET /`

Entrega a interface web do sistema.

### `GET /health`

Verifica se a API esta disponivel.

Resposta:

```json
{
  "status": "ok",
  "service": "vitals-api"
}
```

### `POST /vitals`

Recebe uma leitura de sinais vitais.

Requisicao:

```json
{
  "patient_id": "PAC001",
  "patient_name": "Ana Souza",
  "heart_rate": 128,
  "temperature": 38.5,
  "movement_detected": true
}
```

Resposta com alerta:

```json
{
  "id": 1,
  "received_at": "2026-05-08T23:58:21.510345+00:00",
  "patient_id": "PAC001",
  "patient_name": "Ana Souza",
  "heart_rate": 128,
  "temperature": 38.5,
  "movement_detected": true,
  "alerts": [
    {
      "type": "TAQUICARDIA",
      "message": "Frequencia cardiaca elevada: 128 bpm"
    },
    {
      "type": "FEBRE",
      "message": "Temperatura elevada: 38.5 C"
    }
  ]
}
```

### `GET /vitals`

Lista todas as leituras recebidas desde a inicializacao da API.

Resposta:

```json
{
  "count": 1,
  "items": [
    {
      "id": 1,
      "received_at": "2026-05-08T23:58:21.510345+00:00",
      "patient_id": "PAC001",
      "patient_name": "Ana Souza",
      "heart_rate": 128,
      "temperature": 38.5,
      "movement_detected": true,
      "alerts": []
    }
  ]
}
```

## Validacoes de entrada

A API rejeita leituras incompletas ou malformadas com status `422`.

Campos obrigatorios:

- `patient_id`: texto
- `patient_name`: texto
- `heart_rate`: numero
- `temperature`: numero
- `movement_detected`: booleano

Validacoes adicionais:

- `heart_rate` deve ser maior que zero.
- `temperature` deve estar entre `30` e `45` graus Celsius.
- O corpo da requisicao deve ser JSON valido.

## Decisoes de arquitetura

- A API usa apenas biblioteca padrao do Python para reduzir dependencias e facilitar a
  execucao em qualquer ambiente.
- As regras de risco ficam em modulo separado, evitando acoplamento entre transporte
  HTTP e logica de negocio.
- O disparo de alerta acontece dentro da API, logo todo cliente que chama `POST /vitals`
  aciona o mesmo fluxo de verificacao.
- A persistencia e em memoria por simplicidade academica; ao reiniciar o container ou
  processo, os registros sao reiniciados.
- A simulacao de e-mail por arquivo evita exposicao de credenciais e torna o resultado
  auditavel durante a correcao.
- O Docker Compose padroniza a execucao e elimina diferencas entre ambiente local e
  ambiente de avaliacao.

## Observabilidade da simulacao

Os alertas gerados sao registrados em:

```text
outbox/alertas.log
```

Cada entrada contem:

- data e hora do alerta;
- destinatario simulado;
- assunto;
- paciente;
- leitura recebida;
- lista de riscos detectados.

## Possiveis evolucoes

- Persistir leituras em banco de dados, como SQLite ou PostgreSQL.
- Adicionar autenticacao e autorizacao nos endpoints.
- Implementar envio real de e-mail via SMTP ou servico transacional.
- Criar historico por paciente e filtros por periodo.
- Adicionar testes automatizados para regras de risco e contratos REST.
- Publicar metricas operacionais, como quantidade de alertas por tipo.

## Status da entrega

Entregaveis atendidos:

- cliente REST para envio e recebimento de dados;
- API REST funcional em Python;
- logica de verificacao de riscos;
- automacao de e-mail simulada;
- tela web em React para operacao visual;
- execucao via Docker Compose;
- relatorio curto em `relatorio.md`;
- estrutura organizada e documentada.


## Histórico de Lançamentos

### 1.0.0 08/05/2026
- Implementação da API REST para recebimento de sinais vitais.
- Interface web simples para visualização e envio de dados.
- Validação de risco clínico baseada em regras de negócio.
- Simulação de alertas por e-mail gravados em arquivo de log.

## Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1">
<p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms">
  <a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por
  <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">FIAP</a>
  está licenciado sobre
  <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.
</p>
