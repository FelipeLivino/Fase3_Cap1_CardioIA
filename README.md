# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>

# Nome do projeto

CardioIA - Fase 3: IoT e Visualização de Dados para a Saúde Digital

## Nome do grupo

Rumo ao NEXT

## 👨‍🎓 Integrantes:

- <a href="#">Felipe Livino dos Santos (RM 563187)</a>
- <a href="#">Daniel Veiga Rodrigues de Faria (RM 561410)</a>
- <a href="#">Tomas Haru Sakugawa Becker (RM 564147)</a>
- <a href="#">Daniel Tavares de Lima Freitas (RM 562625)</a>
- <a href="#">Gabriel Konno Carrozza (RM 564468)</a>

## 👩‍🏫 Professores:

### Tutor(a)

- <a href="#">Caique Nonato da Silva Bezerra</a>

### Coordenador(a)

- <a href="https://www.linkedin.com/company/inova-fusca">ANDRÉ GODOI CHIOVATO</a>

## 📜 Descrição

O projeto CardioIA (Fase 3) apresenta a implementação arquitetural completa de uma malha IoT escalável voltada à saúde e aquisição digital de sinais vitais de pacientes. O sistema é englobado por uma prova de conceito composta por:
1. **Edge/Sensores (Wokwi):** Simulação de um microcontrolador ESP32 equipado com sensores DHT22, chave seletora e atuadores, programado em C++ para captar dados e suportar "buffer offline" em caso de perca de rede.
2. **Cloud/Fog/Messaging:** Adoção do cluster robusto MQTT da EMQX (`broker.emqx.io`) para orquestramento de telemetria baseada em fluxos de publicação e escuta em tempo real.
3. **Data Core & Analytics (Docker):** Contêineres provendo serviço de processamento via **Node-RED** (com nós cirúrgicos de extração para evitar tipagens corrompidas de strings), coleta temporal via **Prometheus** (com separação pontual de métricas para burlar bloqueios de Labels), culminando numa experiência visual e hospitalar imersiva utilizando painéis auto-instanciados no **Grafana**.

## 📁 Estrutura de pastas

\`\`\`text
├── grafana/                          # Diretórios de Provisionamento Dinâmico (Dashboards .json e Databases)
├── nodered_data/                     # Volumes montados do contêiner armazenando a árvore lógica e métricas correntes
├── wokwi/                            # Arquivos de código embarcado C++ (sketch.ino) simulados no Wokwi
├── docker-compose.yml                # Receita do orquestrador para provisionamento instantâneo do Node-RED, Prometheus e Grafana
├── node_red_flow.json                # Exportação literal do fluxo lógico processual finalizado da Fase 3
├── relatorio_parte1_edge.md          # Relatório Acadêmico (Parte 1 - Wokwi/Edge)
└── relatorio_parte2_cloud.md         # Relatório Acadêmico (Parte 2 - Nuvem e Arquitetura)
\`\`\`

> **Aviso de Dados Externos (Storage Cloud):** Devido às boas práticas computacionais e aos limites nativos do Git, bases numéricas robustas (do Kaggle/Physionet ou outros bancos) e os densos arquivos que contêm pesadas matrizes de imagens utilizadas no treinamento de Redes Neurais para extração de views **NÃO FAZEM PARTE** desta estrutura interna. Este volume enorme de mídia externa está hospedado em plataformas de Nuvem/Kaggle/Google Drive, sendo instanciados ou operados apenas pontualmente via scripts ou download sob demanda pelo consumidor do projeto.

## 🔧 Como executar o código

**Pré-requisitos do ambiente**
* Docker Engine e Docker-compose integrados na máquina/WSL.
* Conexão ativa liberada para a porta interna 1883 TCP.

**Execução e Replicação:**
1. Clone este repositório e navegue até a pasta pelo terminal.
2. Execute o serviço dos contêineres: `docker-compose up -d`. Este comando inicializa as vias de Analytics silenciosamente nos bastidores.
3. Acesse o **Node-RED** no endereço `http://localhost:1880/`. No menu Injetar do canto, importe o arquivo local `node_red_flow.json`. Certifique-se de realizar o **Deploy**.
4. Dispare a simulação do embarcado Edge em seu navegador abrindo o portal Wokwi utilizando a lógica do `sketch.ino`. Ative a chaveta de "Rede Online".
5. Visualize o fluxo instantâneo em caráter cru através do `http://localhost:1880/ui`.
6. Para observação analítica minuciosa e histórica, acesse o painel unificado em `http://localhost:3000` (Login: admin/admin) na página CardioIA Sinais Vitais do **Grafana**.

## 🗃 Histórico de lançamentos

* **0.1.0** - 20/04/2026
    * Refatoração massiva na infraestrutura Node-RED/Prometheus.
    * Alteração do broker MQTT padrão de HiveMQ para EMQX por razões de estabilidade do free-tier.
    * Finalização integral da Fase 3 e adequação FIAP Acadêmica (Merge de Documentos e Código).

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">Fiap</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>
