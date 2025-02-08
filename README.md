### Link do vídeo explicativo
https://youtu.be/MDhtyVqm0Lo

# UART_I2C
Embarcatech - Tarefa 1 - Aula Síncrona 03/02

### Funcionamento 📋
O programa executa as seguintes operações utilizando a placa BitDogLab:
1. Modificação da Biblioteca font.h
- Foram adicionados os caracteres minúsculos à biblioteca font.h.
2. Entrada de caracteres via PC
- A entrada de caracteres é feita através do Serial Monitor do VS Code.
- Cada caractere digitado no Serial Monitor é exibido no display SSD1306.
- Quando um número entre 0 e 9 é digitado, um símbolo correspondente ao número é exibido, também, na matriz 5x5 WS2812.
3. Interação com o Botão A
- Ao pressionar o botão A o estado do LED RGB Verde é alternado (ligado/desligado).
- A operação é registrada de duas formas:
. Uma mensagem informativa sobre o estado do LED é exibida no display SSD1306.
. Um texto descritivo sobre a operação é enviado ao Serial Monitor.
4. Interação com o Botão B
- Ao pressionar o botão A deve o estado do LED RGB Azul é alternado (ligado/desligado).
- A operação é registrada de duas formas:
. Uma mensagem informativa sobre o estado do LED é exibida no display SSD1306.
. Um texto descritivo sobre a operação é enviado ao Serial Monitor.

### Requisitos cumpridos pelo projeto
1. Uso de interrupções: Todas as funcionalidades relacionadas aos botões foram implementadas utilizando rotinas de interrupção (IRQ).
2. Debouncing: foi implementado o tratamento do bouncing dos botões via software.
3. Controle de LEDs: foi incluído o uso de LEDs comuns e LEDs WS2812, demonstrando o
domínio de diferentes tipos de controle.
4. Utilização do Display 128 x 64: foram utilidas fontes maiúsculas e minúsculas demonstrando o domínio do uso de bibliotecas, o entendimento do princípio de funcionamento do display, bem como, a utilização do protocolo I2C.
5. Envio de informação pela UART.
6. Organização do código: O código foi bem estruturado e comentado para facilitar o entendimento.

### Link do vídeo explicativo
https://youtu.be/MDhtyVqm0Lo