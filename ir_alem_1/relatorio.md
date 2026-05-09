# Relatorio - Comunicacao automatizada com REST e e-mail

## Visao geral

O projeto implementa uma simulacao de monitoramento de sinais vitais usando Python,
comunicacao HTTP por meio de uma API REST e automacao de e-mail em caso de alerta.
O objetivo e representar, de forma simples, um fluxo comum em saude digital: sensores
ou sistemas externos enviam dados clinicos, uma aplicacao central recebe e armazena as
leituras, regras de risco sao avaliadas e a equipe responsavel e notificada quando uma
situacao exige atencao.

## Fluxo implementado

O arquivo `api_server.py` disponibiliza uma API REST local no endereco
`http://127.0.0.1:8000`. A API possui um endpoint `POST /vitals`, responsavel por
receber leituras de sinais vitais em JSON, e um endpoint `GET /vitals`, usado para
consultar as leituras registradas. Tambem existe o endpoint `GET /health`, que confirma
se o servico esta em execucao.

O arquivo `rest_client.py` atua como cliente REST. Ele usa a biblioteca padrao
`urllib` para enviar requisicoes HTTP para a API, demonstrando tanto o envio de dados
quanto o consumo das informacoes cadastradas. O arquivo `demo.py` integra o fluxo
completo: valida a conexao com a API, envia leituras simuladas de pacientes, verifica
os riscos e consulta o total de registros armazenados.

Tambem foi criada uma tela web servida pela propria API no endereco
`http://127.0.0.1:8000`. A interface apresenta, de um lado, as leituras recebidas pela
API e, do outro, um formulario para cadastrar novas leituras. Ao enviar o formulario,
a tela usa o endpoint REST `POST /vitals` e atualiza a listagem consultando
`GET /vitals`.

## Logica de verificacao de risco

A verificacao esta centralizada em `risk_checker.py`, mantendo as regras separadas da
comunicacao HTTP. Foram implementadas tres condicoes de alerta:

- taquicardia, quando a frequencia cardiaca esta acima de 120 bpm;
- febre, quando a temperatura e igual ou superior a 38.0 C;
- ausencia de movimento, quando o campo `movement_detected` e falso.

Cada leitura pode gerar nenhum, um ou varios alertas. Por exemplo, um paciente com
temperatura de 38.6 C e sem movimento detectado gera alertas de febre e ausencia de
movimento ao mesmo tempo.

## Automacao de e-mail

O envio de e-mail foi simulado no arquivo `email_service.py`. Quando uma leitura
apresenta risco, a propria API monta uma mensagem com destinatario, assunto, dados do
paciente e detalhes dos alertas. Essa mensagem e gravada em `outbox/alertas.log`, o
que representa a acao automatizada de envio sem exigir credenciais reais de SMTP.

Essa abordagem e adequada para ambiente academico porque permite verificar o disparo
da automacao com seguranca, sem depender de um provedor externo. Em um ambiente real,
o mesmo ponto do codigo poderia ser adaptado para usar `smtplib`, uma API de e-mail
transacional ou uma ferramenta de RPA integrada ao fluxo hospitalar.

## Conclusao

A solucao atende aos criterios propostos ao implementar consumo e envio de dados via
REST, regras claras de deteccao de risco e uma automacao funcional de alerta por
e-mail simulado. A estrutura dos arquivos separa responsabilidades, facilita a leitura
do codigo e permite evolucoes futuras, como persistencia em banco de dados, autenticacao
da API, historico por paciente e envio real de notificacoes.
