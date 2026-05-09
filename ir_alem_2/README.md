# IR ALEM 2 - IA em series temporais de saude

Projeto para comparar um classificador tradicional com uma abordagem neuromorfica simples em series temporais de sinais vitais.

## Objetivo

Analisar janelas simuladas de batimentos cardiacos e classificar tres estados:

- `normal`
- `taquicardia`
- `irregular`

Foram comparados:

- Regressao Logistica com atributos estatisticos extraidos da serie temporal.
- Rede neuromorfica simples baseada em neuronios LIF, usada como extrator de caracteristicas temporais por disparos, seguida por uma camada de leitura linear.

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

A regressao logistica tende a ser mais simples, rapida e interpretavel. O modelo LIF, mesmo simples, captura dinamicas temporais por meio de taxas e padroes de disparo, mas exige mais escolhas de parametros e nao necessariamente supera o classificador tradicional em uma base pequena ou sintetica.
