# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
<a href= "https://www.fiap.com.br/"><img src="https://github.com/FelipeLivino/Fase2_Cap1_CardioIA/blob/main/asset/logo-fiap.png" alt="FIAP - Faculdade de Informática e Admnistração Paulista" border="0" width=40% height=40%></a>
</p>

<br>

# Nome do projeto

CardioIA - Fase 3: Diagnóstico e Triagem com Inteligência Artificial

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

Este projeto materializa a Fase 2 do PBL (Problem Project Learning) do CardioIA, com o desafio prático de simular a automatização de diagnósticos clínicos com o auxílio da Inteligência Artificial. Nossa missão como equipe foi desenvolver módulos inteligentes capazes de analisar dados e narrativas médicas, reconhecer sintomas, propor diagnósticos assistidos e classificar os riscos inerentes a cada queixa — posicionando nossos algoritmos como autênticos "estetoscópios digitais".

Em profundo alinhamento com a jornada teórica e garantindo a devida reflexão sobre governança em dados e justiça da IA, o projeto estruturou as soluções em torno de duas frentes (Entregáveis):

1. **Extração de Informações via Sistemas de Base de Regras**:
   Na primeira etapa, consolidamos uma ontologia estruturando uma associação explícita entre expressões que simulam relatos de pacientes (ex: "dor no peito") a doenças reais, mapeadas no arquivo `mapa_conhecimento.csv`. O script de integração `diagnostico.py` lê o arquivo de log `sintomas.txt` (que contém os 10 cenários e frases distintas relatando dores e fadigas completas da nossa simulação narrativa) e processa essas sentenças buscando padronizações, visando sugerir automaticamente um possível diagnóstico de saúde.

2. **Classificador de Triagem (Text Classification & TF-IDF)**:
   A segunda etapa abraça a reprodução funcional de um sistema de triagem clínica em recepções de hospitais. Uma base própria contendo frases pre-diagnosticadas e seus níveis atrelados ("alto risco" e "baixo risco") foi construída no `base_risco.csv`. O notebook de Ciência de Dados `classificador_risco.ipynb` consome tais textos e os traduz matematicamente através do popular método TF-IDF (_Term Frequency - Inverse Document Frequency_). A partir disso, em vez de depender apenas de um modelo, o sistema testa e treina as predições usando um pool dos mais avançados modelos do Scikit-Learn (Logistic Regression, Decision Trees/Random Forests, Support Vector Machines e Naive Bayes), selecionando instantaneamente o classificador de maior acurácia para simular na automação final os graus de emergência da triagem do doente.

Todos esses passos combinam criatividade orgânica e dados bem-organizados e ilustram de fato nossa maturação rumo ao desenvolvimento de IA responsável: construir a pedra fundamental de plataformas de saúde reais, com o foco em priorizar filas e otimizar tempo valioso para potencialmente salvar vidas.

### 🎥 Demonstração de Funcionamento Completo (Vídeo)

> O funcionamento final do protótipo contendo a explicação da triagem pela solução do modelo pode ser conferido em: **[https://youtu.be/8in6x4p6Z3A]**

## 📁 Estrutura de pastas

De modo a facilitar o versionamento e sanar eventuais dúvidas na navegação da estrutura do GitHub, **esta listagem reflete única e exclusivamente o conteúdo contido dentro da raiz interna deste git**, delineando a organização do que é mantido como código:

Dentre os arquivos e pastas presentes na raiz do projeto, definem-se:

- <b>ir_alem_1/</b>: Pasta contendo o módulo e código fonte do painel de Frontend em React/Vite (dashboard da aplicação CardioIA Web).
- <b>ir_alem_2/</b>: Pasta dedicada ao módulo de processamento de imagens, incluindo o classificador das ondas cardíacas lidas via MLP (Visão em Eletrocardiograma).
- <b>base_risco.csv</b>: Dataset simulado focal com frases categorizadas ("alto risco" / "baixo risco") para uso nos pipelines matemáticos de classificação NLP na triagem de enfermagem.
- <b>classificador_risco.ipynb</b>: Jupyter Notebook de Machine Learning contendo o ciclo completo (do TF-IDF e métricas de confusão até o treinamento concorrente com algoritmos de ML como Random Forest e SVC).
- <b>diagnostico.py</b>: Script lógico (Sistemas Baseados em Regras) responsável por aplicar processamentos e interconectar os pacientes em txt baseados nas regras em csv, sugerindo um laudo textual.
- <b>mapa_conhecimento.csv</b>: Um documento tabular funcionando como a "tabela da verdade" ou ontologia que vincula o peso/perigo de um agrupamento de sintomas a uma enfermidade listada.
- <b>sintomas.txt</b>: Arquivo com simulações de inputs (textos puros como se transcritos pelos pacientes).
- <b>README.md</b>: arquivo principal que serve como guia geral sobre o escopo, arquitetura e execução do projeto (o mesmo que você está lendo agora).

> **Aviso de Dados Externos (Storage Cloud):** Devido às boas práticas computacionais e aos limites nativos do Git, bases numéricas robustas (do Kaggle/Physionet ou outros bancos) e os densos arquivos que contêm pesadas matrizes de imagens utilizadas no treinamento de Redes Neurais para extração de views **NÃO FAZEM PARTE** desta estrutura interna. Este volume enorme de mídia externa está hospedado em plataformas de Nuvem/Kaggle/Google Drive, sendo instanciados ou operados apenas pontualmente via scripts ou download sob demanda pelo consumidor do projeto.

## 🔧 Como executar o código

**Pré-requisitos do ambiente**

- Python 3 devidamente configurado nas variáveis de ambiente.
- Interpretador interativo Jupyter para leitura do Notebook.

**Execução e Replicação:**

1. Realize o download ou `git clone` deste repositório para o diretório raiz na sua máquina com o VS Code.
2. Dependendo da sua instalação local, recomendamos criar um ambiente virtual (VENV) exclusivo do projeto.
3. Instale os requerimentos e bibliotecas operacionais da simulação através do gerenciador de pacotes:
   ```bash
   pip install pandas scikit-learn numpy matplotlib seaborn
   ```
4. Para acompanhar de perto o modelo de base de Regras e Lógica de grafos diretos, no terminal digite:
   ```bash
   python diagnostico.py
   ```
5. Para mergulhar nos cenários de predição via IA e análise text-to-risk (ML e NLP), abra tranquilamente o diretório `classificador_risco.ipynb` no seu editor, verifique se selecionou o Kernel que instalou o PIP anterior, e dispare suas células em sequência (Run All). O relatório da competição analítica será gerado visualmente por meio dos recursos do pyplot e seaborn no formato nativo interativo.

## 🗃 Histórico de lançamentos

- 1.0.0 - 11/04/2026
  - Refatoração na arquitetura de visualizações de treinamento do classificador NLP com inserção das métricas Matplotlib/Seaborn. Atualização semântica integral do repositório conforme diretrizes FIAP.
- 0.2.0 - 07/04/2026
  - Modularização de diretórios contendo desenvolvimento e implementação visual do modelo neural artificial (MLP) de classificação `ir_alem_2`, bem como acréscimo de painel Front `ir_alem_1`.
- 0.1.0 - 05/04/2026
  - Commit Base e Lançamento incial contendo pipelines de base e lógicas predeterminadas CSV-to-Python e a concepção vetorial e de Logistic Regression no notebook original.

## 📋 Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1"><p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms/"><a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">Fiap</a> está licenciado sobre <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.</p>
