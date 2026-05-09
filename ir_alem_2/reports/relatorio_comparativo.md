# Relatorio comparativo - IA em series temporais de saude

## 1. Contexto e metodologia

Este projeto avalia duas abordagens de inteligencia artificial aplicadas a series temporais de sinais vitais, usando janelas simuladas de batimentos cardiacos. A base foi construida de forma sintetica para representar tres padroes clinicos simplificados: ritmo normal, taquicardia e ritmo irregular. Cada amostra corresponde a uma janela temporal com variacoes naturais, tendencia, ruido e, no caso irregular, perturbacoes mais acentuadas no intervalo entre batimentos.

O primeiro grupo de metodos e composto por classificadores tradicionais. Como esses modelos nao trabalham diretamente com a sequencia bruta, foram extraidos atributos estatisticos e temporais: media, desvio padrao, minimo, maximo, amplitude, RMSSD, inclinacao da serie, proporcao de variacoes acima de um limiar, autocorrelacao e energia espectral em faixas baixas e altas. Foram avaliados Regressao Logistica, SVM, Random Forest, KNN, Arvore de Decisao, Gradient Boosting e Naive Bayes Gaussiano. Essa escolha cria representacoes simples, comparaveis e computacionalmente baratas.

O segundo grupo usa modelos neuromorficos simples baseados em neuronios LIF, do ingles *Leaky Integrate-and-Fire*. Nesse modelo, a serie temporal e convertida em correntes de entrada para um conjunto de neuronios com diferentes limiares e ganhos. A membrana acumula potencial ao longo do tempo, sofre vazamento e dispara quando ultrapassa um limiar. Apos o disparo, o potencial e reiniciado. Alem do LIF base, foram adicionadas variacoes com normalizacao definida pelo treino, atributos de disparo mais ricos, multiplas constantes de tempo e uma camada de leitura nao linear.

## 2. Comparacao dos modelos

Os classificadores tradicionais apresentam a vantagem de formar varias linhas de base sobre os mesmos atributos. A regressao logistica favorece interpretabilidade, o SVM testa fronteiras nao lineares, o KNN compara amostras por proximidade, os modelos baseados em arvores capturam interacoes entre atributos e o Naive Bayes oferece uma referencia probabilistica simples. Como os atributos sao explicitos, e possivel discutir por que determinada classe foi favorecida: uma media de batimentos mais alta tende a indicar taquicardia, enquanto maior variabilidade e RMSSD podem indicar irregularidade.

O modelo LIF oferece uma perspectiva diferente. Em vez de resumir a serie diretamente com estatisticas fixas, ele transforma a dinamica temporal em eventos de disparo. Isso se aproxima de uma ideia neuromorfica: representar informacao por atividade esparsa ao longo do tempo. As variacoes testadas tornam essa representacao mais forte: a escala do sinal passa a ser aprendida no treino, os disparos geram mais atributos temporais e a versao multi-escala combina neuronios rapidos, intermediarios e lentos. Em cenarios com hardware neuromorfico, tambem haveria potencial de eficiencia energetica, ja que o processamento baseado em eventos evita computacao constante quando nao ha disparos relevantes.

A comparacao no notebook foi ampliada para incluir acuracia, acuracia balanceada, precision macro, recall macro, F1 macro, F1 ponderado, quantidade de atributos e tempo medio de predicao no conjunto de teste. Alem das matrizes de confusao, foram adicionados graficos comparativos de metricas gerais, ranking por F1 macro, custo de inferencia e F1-score por classe. Isso facilita enxergar nao apenas quem acertou mais, mas tambem se algum modelo ficou pior em uma classe especifica ou se exige mais custo para predizer.

Nos testes do notebook, espera-se que os modelos atinjam desempenho razoavel, pois as classes foram simuladas com padroes distintos. Alguns classificadores tradicionais podem apresentar desempenho alto por receberem atributos informativos e desenhados para o problema. O LIF, por outro lado, pode se aproximar do desempenho tradicional, mas sua qualidade depende muito de parametros como limiar, constante de tempo, ganho de corrente e quantidade de neuronios. Isso mostra uma limitacao importante: modelos neuromorficos simples podem exigir calibracao cuidadosa antes de entregar vantagem pratica.

## 3. Vantagens e limitacoes

Os modelos tradicionais sao indicados quando se deseja uma solucao transparente, rapida e facil de comparar. Eles tambem facilitam a depuracao do pipeline, pois erros podem ser associados aos atributos extraidos. Sua limitacao e depender fortemente da engenharia manual de atributos. Caso a serie possua padroes complexos, nao lineares ou dependencias temporais longas, atributos simples podem perder informacao importante.

O LIF e mais adequado para explorar processamento temporal e codificacao por eventos. Ele pode capturar diferencas no comportamento dinamico da serie sem reduzir tudo imediatamente a estatisticas globais. Entretanto, nesta implementacao, o LIF nao e treinado de ponta a ponta; ele atua como extrator fixo de caracteristicas. Assim, sua capacidade depende da escolha manual dos parametros dos neuronios. As versoes melhorada e multi-escala reduzem parte dessa fragilidade, mas aumentam a quantidade de atributos e ainda exigem calibracao.

## 4. Conclusao

A comparacao indica que nao existe uma resposta unica sobre qual modelo e melhor. Para uma primeira analise de sinais vitais, o conjunto de modelos tradicionais oferece linhas de base fortes e variadas. As variacoes LIF demonstram valor como experimento neuromorfico por representar a serie temporal em disparos, testar diferentes escalas temporais e permitir discutir eficiencia, dinamica temporal e inspiracao biologica.

Em uma aplicacao real de saude, seria necessario substituir a base sintetica por dados clinicos validados, realizar validacao cruzada por paciente, avaliar desbalanceamento de classes, medir sensibilidade e especificidade por classe e discutir riscos de falso negativo. Portanto, os resultados deste projeto devem ser entendidos como uma demonstracao tecnica e critica, nao como ferramenta diagnostica.
