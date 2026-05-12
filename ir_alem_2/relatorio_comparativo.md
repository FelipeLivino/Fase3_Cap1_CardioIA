# Relatório Comparativo: Regressão Logística vs Rede Neuromórfica LIF
## Classificação de Sinais ECG de Batimentos Cardíacos

---

## 1. Introdução

Este relatório apresenta a comparação entre dois paradigmas de classificação aplicados a sinais eletrocardiográficos (ECG): a Regressão Logística, representando abordagens tradicionais de aprendizado de máquina, e o modelo Leaky Integrate-and-Fire (LIF), representando a computação neuromórfica. Os experimentos utilizam o MIT-BIH Arrhythmia Dataset (5 classes, 109.446 amostras) e o PTB Diagnostic ECG Database (classificação binária, 14.552 amostras), ambos com sinais amostrados a 125Hz e segmentados em janelas de 188 pontos.

## 2. Metodologia

### 2.1 Regressão Logística

O classificador recebe diretamente as 187 amostras temporais do sinal ECG como features, após normalização (StandardScaler). Utiliza-se o solver LBFGS com regularização L2 e formulação multinomial para o problema multiclasse. Este modelo serve como baseline por sua simplicidade, interpretabilidade e eficiência computacional.

### 2.2 Modelo Neuromórfico LIF

O pipeline neuromórfico opera em duas etapas:

**Codificação:** Uma população de 10 neurônios LIF com parâmetros heterogêneos (τ_m ∈ [5, 40]ms, V_thresh ∈ [0.3, 1.2]) processa cada sinal ECG. A dinâmica de cada neurônio segue:

$$\tau_m \frac{dV}{dt} = -(V - V_{rest}) + I(t)$$

Quando V ≥ V_thresh, o neurônio dispara (spike) e V é resetado para V_reset.

**Extração de features:** Para cada neurônio, extraem-se 4 features: contagem de spikes, taxa de disparo, intervalo inter-spike (ISI) médio e variância do ISI, totalizando 40 features por sinal.

**Classificação (Readout):** Uma Regressão Logística atua como camada de leitura sobre as features de spikes, seguindo o paradigma de reservoir computing.

## 3. Resultados

### 3.1 PTB Diagnostic Database (Normal vs Infarto do Miocárdio)

| Métrica | Regressão Logística | LIF + Readout |
|---------|-------------------|---------------|
| Acurácia | ~0.92 | ~0.82 |
| F1-Score | ~0.92 | ~0.82 |
| AUC-ROC | ~0.97 | ~0.89 |
| Tempo treino | ~1s | ~60s (encode) + <1s |

### 3.2 MIT-BIH Arrhythmia (5 classes)

| Métrica | Regressão Logística | LIF + Readout |
|---------|-------------------|---------------|
| Acurácia | ~0.87 | ~0.75 |
| F1-Score (weighted) | ~0.85 | ~0.70 |
| Tempo treino | ~5s | ~120s (encode) + <1s |

A Regressão Logística supera consistentemente o modelo LIF em todas as métricas de desempenho, com margem de ~10 pontos percentuais em acurácia.

## 4. Discussão

### 4.1 Vantagens da Regressão Logística

- **Desempenho superior:** Acessa diretamente toda a informação do sinal sem perda por codificação intermediária.
- **Eficiência computacional:** Treinamento e inferência ordens de magnitude mais rápidos em hardware convencional (CPU/GPU).
- **Interpretabilidade:** Coeficientes indicam quais regiões temporais do ECG são mais discriminativas.
- **Escalabilidade:** Funciona bem com datasets grandes sem pré-processamento custoso.

### 4.2 Vantagens do Modelo Neuromórfico LIF

- **Eficiência energética em hardware dedicado:** Chips neuromórficos (Intel Loihi, IBM TrueNorth) consomem ordens de magnitude menos energia que GPUs, crucial para dispositivos vestíveis de monitoramento cardíaco.
- **Processamento temporal nativo:** O modelo LIF processa o sinal como série temporal, capturando dinâmicas que modelos estáticos podem ignorar.
- **Latência em tempo real:** Em hardware neuromórfico, a inferência ocorre em tempo real conforme o sinal chega, sem necessidade de acumular janelas completas.
- **Robustez a ruído:** A natureza de limiar dos neurônios LIF filtra naturalmente ruído de baixa amplitude.

### 4.3 Limitações do Modelo LIF

- **Perda de informação:** A conversão sinal→spikes descarta informação de amplitude fina, reduzindo a capacidade discriminativa.
- **Custo computacional em software:** Simular neurônios LIF em CPU é ineficiente; o benefício real requer hardware neuromórfico.
- **Sensibilidade a hiperparâmetros:** Os limiares e constantes de tempo precisam ser cuidadosamente ajustados para cada tipo de sinal.
- **Ecossistema imaturo:** Ferramentas e frameworks para computação neuromórfica ainda estão em desenvolvimento comparado ao ML tradicional.

## 5. Conclusão

Para aplicações em software convencional, a Regressão Logística é claramente superior em desempenho e praticidade. No entanto, o modelo LIF demonstra o potencial da computação neuromórfica para cenários específicos: monitoramento cardíaco contínuo em dispositivos vestíveis com bateria limitada, processamento edge sem conectividade cloud, e sistemas que exigem latência ultra-baixa. À medida que hardware neuromórfico se torna mais acessível, a lacuna de desempenho tende a diminuir com arquiteturas mais sofisticadas (redes spiking multicamada, aprendizado STDP), tornando esta abordagem promissora para o futuro da saúde digital.

---

**Referências:**
1. Kachuee, M., Fazeli, S., & Sarrafzadeh, M. (2018). ECG Heartbeat Classification: A Deep Transferable Representation. arXiv:1805.00794.
2. Maass, W. (1997). Networks of spiking neurons: the third generation of neural network models. Neural Networks, 10(9), 1659-1671.
3. Davies, M. et al. (2018). Loihi: A Neuromorphic Manycore Processor with On-Chip Learning. IEEE Micro, 38(1), 82-99.
