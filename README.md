# Matriz_5x5_IRQ
Embarcatech - Tarefa 1 - Aula Síncrona 27/01

### Funcionamento 📋
O programa executa as seguintes operações utilizando a placa BitDogLab:
- O LED vermelho do LED RGB pisca continuamente 5 vezes por segundo (se mantém aceso por conta do intervalo de tempo muito pequeno).
- Ao pressionar o botão "A" incrementa o número exibido na matriz de LEDs cada vez que é pressionado.
- Ao pressionar o botão "B" decrementa o número exibido na matriz de LEDs cada vez que é pressionado.

### Requisitos cumpridos pelo projeto
1. Uso de interrupções: todas as funcionalidades relacionadas aos botões foram implementadas utilizando rotinas de interrupção (IRQ).
2. Debouncing: foi implementado o tratamento do bouncing dos botões via software.
3. Controle de LEDs: o projeto inclui o uso de LEDs comuns e LEDs WS2812, demonstrando o
domínio de diferentes tipos de controle.
4. Organização do código: o código está bem estruturado e comentado para facilitar o
entendimento.
