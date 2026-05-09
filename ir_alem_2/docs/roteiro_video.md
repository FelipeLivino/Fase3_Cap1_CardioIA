# Roteiro para video - ate 4 minutos

## 0:00 - 0:30 | Apresentacao do problema

Apresente o objetivo do projeto: aplicar IA em series temporais de saude, usando sinais vitais simulados de batimentos cardiacos. Explique as tres classes analisadas: normal, taquicardia e irregular.

## 0:30 - 1:10 | Base de dados e visualizacao

Mostre no notebook os graficos das tres janelas de exemplo. Destaque que a base e sintetica e reprodutivel, criada para permitir comparacao controlada entre os modelos.

## 1:10 - 1:50 | Modelos tradicionais

Explique que os modelos tradicionais nao recebem a serie bruta, mas atributos como media, desvio padrao, amplitude, RMSSD, autocorrelacao e energia espectral. Cite que foram comparados Regressao Logistica, SVM, Random Forest, KNN, Arvore de Decisao, Gradient Boosting e Naive Bayes Gaussiano.

## 1:50 - 2:40 | Modelo neuromorfico LIF

Mostre a funcao `lif_encode`. Explique de forma simples: cada neuronio acumula corrente, perde carga com o tempo e dispara quando ultrapassa um limiar. Depois mostre as secoes 3.1, 3.2 e 3.3: LIF melhorado com normalizacao pelo treino, LIF multi-escala temporal e LIF multi-escala com leitura nao linear.

## 2:40 - 3:30 | Comparacao dos resultados

Mostre a tabela final e as matrizes de confusao. Comente que, neste experimento, os atributos estatisticos podem favorecer os classificadores tradicionais porque separam bem as classes sinteticas. As variacoes LIF tornam a comparacao mais justa porque testam melhorias na codificacao por disparos e na camada de leitura.

## 3:30 - 4:00 | Analise critica

Finalize dizendo que o projeto e uma demonstracao tecnica, nao uma ferramenta clinica. Para uso real, seria necessario usar dados de pacientes, validacao por paciente, metricas clinicas como sensibilidade e especificidade e avaliacao cuidadosa de falso negativo.
