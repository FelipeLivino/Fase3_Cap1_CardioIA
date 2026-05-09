# Relatorio comparativo - IA em series temporais de saude

## 1. Contexto e metodologia

Este projeto avalia duas abordagens de inteligencia artificial aplicadas a series temporais de sinais vitais, usando janelas simuladas de batimentos cardiacos. A base foi construida de forma sintetica para representar tres padroes clinicos simplificados: ritmo normal, taquicardia e ritmo irregular. Cada amostra corresponde a uma janela temporal com variacoes naturais, tendencia, ruido e, no caso irregular, perturbacoes mais acentuadas no intervalo entre batimentos.

O primeiro metodo e uma regressao logistica multinomial. Como esse modelo nao trabalha diretamente com a sequencia bruta, foram extraidos atributos estatisticos e temporais: media, desvio padrao, minimo, maximo, amplitude, RMSSD, inclinacao da serie, proporcao de variacoes acima de um limiar, autocorrelacao e energia espectral em faixas baixas e altas. Essa escolha cria uma representacao simples, interpretavel e computacionalmente barata.

O segundo metodo usa uma rede neuromorfica simples baseada em neuronios LIF, do ingles *Leaky Integrate-and-Fire*. Nesse modelo, a serie temporal e convertida em correntes de entrada para um conjunto de neuronios com diferentes limiares e ganhos. A membrana acumula potencial ao longo do tempo, sofre vazamento e dispara quando ultrapassa um limiar. Apos o disparo, o potencial e reiniciado. As caracteristicas finais usadas para classificacao sao taxas de disparo, tempo do primeiro disparo e variabilidade dos intervalos entre disparos. Uma regressao logistica atua como camada de leitura sobre essas caracteristicas neuromorficas.

## 2. Comparacao dos modelos

A regressao logistica apresenta como principal vantagem a interpretabilidade. Como os atributos sao explicitos, e possivel discutir por que determinada classe foi favorecida: uma media de batimentos mais alta tende a indicar taquicardia, enquanto maior variabilidade e RMSSD podem indicar irregularidade. Tambem e um modelo leve, facil de treinar e com poucos hiperparametros. Para um problema educacional e uma base sintetica, essa simplicidade e uma vantagem importante, pois reduz o risco de conclusoes artificiais causadas por excesso de complexidade.

O modelo LIF oferece uma perspectiva diferente. Em vez de resumir a serie diretamente com estatisticas fixas, ele transforma a dinamica temporal em eventos de disparo. Isso se aproxima de uma ideia neuromorfica: representar informacao por atividade esparsa ao longo do tempo. Essa abordagem pode ser interessante para sinais fisiologicos porque preserva a ordem temporal durante a simulacao do potencial de membrana. Em cenarios com hardware neuromorfico, tambem haveria potencial de eficiencia energetica, ja que o processamento baseado em eventos evita computacao constante quando nao ha disparos relevantes.

Nos testes do notebook, espera-se que os dois modelos atinjam desempenho razoavel, pois as classes foram simuladas com padroes distintos. A regressao logistica pode apresentar desempenho igual ou superior por receber atributos altamente informativos e desenhados para o problema. O LIF, por outro lado, pode se aproximar do desempenho tradicional, mas sua qualidade depende muito de parametros como limiar, constante de tempo, ganho de corrente e quantidade de neuronios. Isso mostra uma limitacao importante: modelos neuromorficos simples podem exigir calibracao cuidadosa antes de entregar vantagem pratica.

## 3. Vantagens e limitacoes

A regressao logistica e indicada quando se deseja uma solucao transparente, rapida e facil de justificar. Ela tambem facilita a depuracao do pipeline, pois erros podem ser associados aos atributos extraidos. Sua limitacao e depender fortemente da engenharia manual de atributos. Caso a serie possua padroes complexos, nao lineares ou dependencias temporais longas, atributos simples podem perder informacao importante.

O LIF e mais adequado para explorar processamento temporal e codificacao por eventos. Ele pode capturar diferencas no comportamento dinamico da serie sem reduzir tudo imediatamente a estatisticas globais. Entretanto, nesta implementacao, o LIF nao e treinado de ponta a ponta; ele atua como extrator fixo de caracteristicas. Assim, sua capacidade depende da escolha manual dos parametros dos neuronios. Outra limitacao e que a explicacao dos resultados e menos direta do que na regressao logistica, embora ainda seja mais interpretavel do que redes profundas complexas.

## 4. Conclusao

A comparacao indica que nao existe uma resposta unica sobre qual modelo e melhor. Para uma primeira analise de sinais vitais, a regressao logistica e uma excelente linha de base: simples, robusta e interpretavel. A rede LIF demonstra valor como experimento neuromorfico por representar a serie temporal em disparos e permitir discutir eficiencia, dinamica temporal e inspiracao biologica.

Em uma aplicacao real de saude, seria necessario substituir a base sintetica por dados clinicos validados, realizar validacao cruzada por paciente, avaliar desbalanceamento de classes, medir sensibilidade e especificidade por classe e discutir riscos de falso negativo. Portanto, os resultados deste projeto devem ser entendidos como uma demonstracao tecnica e critica, nao como ferramenta diagnostica.
