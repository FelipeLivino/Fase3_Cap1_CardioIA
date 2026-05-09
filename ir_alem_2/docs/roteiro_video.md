# Roteiro para video - ate 4 minutos

## 0:00 - 0:30 | Apresentacao do problema

Apresente o objetivo do projeto: aplicar IA em series temporais de saude, usando sinais vitais simulados de batimentos cardiacos. Explique as tres classes analisadas: normal, taquicardia e irregular.

## 0:30 - 1:10 | Base de dados e visualizacao

Mostre no notebook os graficos das tres janelas de exemplo. Destaque que a base e sintetica e reprodutivel, criada para permitir comparacao controlada entre os modelos.

## 1:10 - 1:50 | Modelo tradicional

Explique que a regressao logistica nao recebe a serie bruta, mas atributos como media, desvio padrao, amplitude, RMSSD, autocorrelacao e energia espectral. Mostre o relatorio de classificacao e comente a interpretabilidade.

## 1:50 - 2:40 | Modelo neuromorfico LIF

Mostre a funcao `lif_encode`. Explique de forma simples: cada neuronio acumula corrente, perde carga com o tempo e dispara quando ultrapassa um limiar. As taxas e tempos de disparo viram atributos para uma camada linear.

## 2:40 - 3:30 | Comparacao dos resultados

Mostre a tabela final e as matrizes de confusao. Comente que, neste experimento, a regressao logistica foi superior porque os atributos estatisticos separam muito bem as classes sinteticas. O LIF foi util para explorar codificacao temporal por eventos, mas depende de parametros manuais.

## 3:30 - 4:00 | Analise critica

Finalize dizendo que o projeto e uma demonstracao tecnica, nao uma ferramenta clinica. Para uso real, seria necessario usar dados de pacientes, validacao por paciente, metricas clinicas como sensibilidade e especificidade e avaliacao cuidadosa de falso negativo.
