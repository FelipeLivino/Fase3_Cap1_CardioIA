# Classificação de Batimentos Cardíacos: Regressão Logística vs Rede Neuromórfica (LIF)

## Objetivo

Aplicar técnicas de IA para análise de séries temporais de sinais vitais (batimentos cardíacos), comparando um classificador tradicional (Regressão Logística) com uma rede neuromórfica simples (modelo Leaky Integrate-and-Fire - LIF).

## Datasets

- **MIT-BIH Arrhythmia Dataset**: 109.446 amostras, 5 classes de arritmia, 125Hz
- **PTB Diagnostic ECG Database**: 14.552 amostras, classificação binária (normal/anormal), 125Hz

Fonte: [Kaggle - ECG Heartbeat Categorization Dataset](https://www.kaggle.com/datasets/shayanfazeli/heartbeat)

Referência: Kachuee, M., Fazeli, S., & Sarrafzadeh, M. (2018). "ECG Heartbeat Classification: A Deep Transferable Representation." arXiv:1805.00794.

## Estrutura do Projeto

```
├── ecg_classification.ipynb   # Notebook Jupyter com todo o pipeline
├── relatorio_comparativo.md   # Relatório comparativo (2 páginas)
├── README.md                  # Este arquivo
├── mitbih_train.csv           # Dados MIT-BIH (treino)
├── mitbih_test.csv            # Dados MIT-BIH (teste)
├── ptbdb_normal.csv           # Dados PTB (normal)
├── ptbdb_abnormal.csv         # Dados PTB (anormal)
└── contexto_dataset.txt       # Descrição do dataset
```

## Como Executar

### Requisitos

```bash
pip install numpy pandas scikit-learn matplotlib jupyter
```

### Execução

```bash
jupyter notebook ecg_classification.ipynb
```

Ou abrir diretamente no VS Code / JupyterLab.

## Modelos Implementados

### 1. Regressão Logística (Baseline)
- Classificador linear tradicional
- Aplicado diretamente nas 187 features temporais do sinal ECG
- Solver LBFGS com regularização L2

### 2. Rede Neuromórfica LIF (Leaky Integrate-and-Fire)
- População de 10 neurônios LIF com parâmetros variados
- Codificação temporal: sinal ECG → padrões de spikes
- Features extraídas: contagem de spikes, taxa de disparo, ISI médio, variância ISI
- Camada de readout (Regressão Logística) para classificação final

## Resultados Esperados

| Dataset | Modelo | Acurácia | F1-Score |
|---------|--------|----------|----------|
| PTB | Reg. Logística | ~0.92 | ~0.92 |
| PTB | LIF + Readout | ~0.82 | ~0.82 |
| MIT-BIH | Reg. Logística | ~0.87 | ~0.85 |
| MIT-BIH | LIF + Readout | ~0.75 | ~0.70 |

## Autor

Projeto acadêmico - Análise de Séries Temporais com IA

## Licença

MIT
