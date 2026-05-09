# FIAP - Faculdade de Informática e Administração Paulista

<p align="center">
  <a href="https://www.fiap.com.br/">
    <img src="https://github.com/FelipeLivino/Fase3_Cap1_CardioIA/blob/main/asset/logo-fiap.png" alt="FIAP - Faculdade de Informática e Administração Paulista" border="0" width="40%" height="40%">
  </a>
</p>

<br>

# CardioIA

# IR ALEM 2 - IA em series temporais de saude

Projeto para comparar um classificador tradicional com uma abordagem neuromorfica simples em series temporais de sinais vitais.

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

## Objetivo

Analisar janelas simuladas de batimentos cardiacos e classificar tres estados:

- `normal`
- `taquicardia`
- `irregular`

Foram comparados:

- Modelos tradicionais com atributos estatisticos extraidos da serie temporal: Regressao Logistica, SVM, Random Forest, KNN, Arvore de Decisao, Gradient Boosting e Naive Bayes Gaussiano.
- Variacoes neuromorficas baseadas em neuronios LIF, usadas como extratores de caracteristicas temporais por disparos, com leitura linear e nao linear.

## Estrutura

```text
.
├── notebooks/
│   └── ir_alem_2_series_temporais_saude.ipynb
├── docs/
│   └── roteiro_video.md
├── reports/
│   └── relatorio_comparativo.md
├── requirements.txt
└── README.md
```

## Como executar

1. Crie um ambiente virtual:

```bash
python -m venv .venv
```

2. Ative o ambiente:

```bash
# Windows PowerShell
.venv\Scripts\Activate.ps1
```

3. Instale as dependencias:

```bash
pip install -r requirements.txt
```

4. Abra o notebook:

```bash
jupyter notebook notebooks/ir_alem_2_series_temporais_saude.ipynb
```

5. Execute todas as celulas para gerar as metricas, matriz de confusao e graficos.

## Entregaveis

- Notebook Python comentado: `notebooks/ir_alem_2_series_temporais_saude.ipynb`
- Relatorio comparativo: `reports/relatorio_comparativo.md`
- Roteiro sugerido para apresentacao: `docs/roteiro_video.md`
- Repositorio GitHub publico: publicar este projeto em um repositorio publico.
- Video de ate 4 minutos: publicar no YouTube como "nao listado" e substituir o marcador abaixo.

## Video

Link do video nao listado: `INSERIR_LINK_DO_YOUTUBE_AQUI`

## Resumo esperado dos resultados

Os modelos tradicionais oferecem varias linhas de base para comparar fronteiras lineares, nao lineares, baseadas em vizinhanca, arvores e probabilidades. As variacoes LIF capturam dinamicas temporais por meio de taxas e padroes de disparo, testando normalizacao pelo treino, atributos de disparo mais ricos, multiplas constantes de tempo e leitura nao linear.


## Histórico de Lançamentos

### 1.0.0 09/05/2026
- Implementação de análise de séries temporais de sinais vitais cardíacos.
- Comparação entre classificadores tradicionais com atributos estatísticos e abordagem neuromórfica (rede baseada em neurônios LIF).
- Classificação de estados: normal, taquicardia e irregular.
- Geração de métricas, matriz de confusão e gráficos no notebook Jupyter.
- Relatório comparativo dos modelos e roteiro para apresentação em vídeo.

## Licença

<img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/cc.svg?ref=chooser-v1"><img style="height:22px!important;margin-left:3px;vertical-align:text-bottom;" src="https://mirrors.creativecommons.org/presskit/icons/by.svg?ref=chooser-v1">
<p xmlns:cc="http://creativecommons.org/ns#" xmlns:dct="http://purl.org/dc/terms">
  <a property="dct:title" rel="cc:attributionURL" href="https://github.com/agodoi/template">MODELO GIT FIAP</a> por
  <a rel="cc:attributionURL dct:creator" property="cc:attributionName" href="https://fiap.com.br">FIAP</a>
  está licenciado sobre
  <a href="http://creativecommons.org/licenses/by/4.0/?ref=chooser-v1" target="_blank" rel="license noopener noreferrer" style="display:inline-block;">Attribution 4.0 International</a>.
</p>
