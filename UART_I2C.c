#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"

#include "lib/ssd1306.h"
#include "lib/font.h"
#include "lib/numeros/numeros.h"
#include "ws2812.pio.h"

#define GPIO_BOTAO_A    5
#define GPIO_BOTAO_B    6
#define GPIO_LED_R      13
#define GPIO_LED_G      11
#define GPIO_LED_B      12
#define GPIO_WS2812     7   
#define NUM_LEDS        25
#define I2C_PORT        i2c1
#define GPIO_I2C_SDA    14     
#define GPIO_I2C_SLC    15
#define ADDRESS         0x3C

/*
 * Cria variáveis globais
 * static: variável permanece na memória durante toda a execução do programa 
 * volatile: a variável pode ser alterada por eventos externos
 */ 
static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile PIO pio;
static volatile uint sm = 0;
// Variável de inicialização do display
ssd1306_t ssd; 
// Variável string para linhas do Display (máximo 12 caracteres por linha)
char linhaTextoDisplay[12];


// Inicializa vetor para apagar todos os leds
static double numeros_apaga[NUM_LEDS] = {0.0, 0.0, 0.0, 0.0, 0.0,
                                         0.0, 0.0, 0.0, 0.0, 0.0, 
                                         0.0, 0.0, 0.0, 0.0, 0.0,
                                         0.0, 0.0, 0.0, 0.0, 0.0,
                                         0.0, 0.0, 0.0, 0.0, 0.0};

// Função para acender o LED por um tempo específico
void controlaLed(uint gpio, bool operacao) {
    gpio_put(gpio, operacao); // Liga/Desliga o LED indicado no parâmetro gpio
}

// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se passou tempo suficiente desde o último evento (200 ms de debouncing)
    if (current_time - last_time > 200000) {
        last_time = current_time; // Atualiza o tempo do último evento

        // Exibe número na Matriz de LEDs 5x5
        if (gpio == GPIO_BOTAO_A) {
            controlaLed(GPIO_LED_G, !gpio_get(GPIO_LED_G));
            // Envia o texto para o display
            sprintf(linhaTextoDisplay, "%s%s", "RGB VERDE: ", (gpio_get(GPIO_LED_G) == 1) ? "ON " : "OFF"); 
            ssd1306_draw_string(&ssd, linhaTextoDisplay, 8, 30); 
            // Atualiza o display
            ssd1306_send_data(&ssd); 
            // Envia texto para o Serial Monitor
            printf("%s\n", linhaTextoDisplay);
        } 
        else if (gpio == GPIO_BOTAO_B) {
            controlaLed(GPIO_LED_B, !gpio_get(GPIO_LED_B));
            // Envia o texto para o display
            sprintf(linhaTextoDisplay, "%s%s", "RGB AZUL: ", (gpio_get(GPIO_LED_B) == 1) ? "ON " : "OFF"); 
            ssd1306_draw_string(&ssd, linhaTextoDisplay, 8, 48); 
            // Atualiza o display
            ssd1306_send_data(&ssd); 
            // Envia texto para o Serial Monitor
            printf("%s\n", linhaTextoDisplay);
        }
    }
}

// Função que configura PIO da Matriz de LEDs 5x5
uint pio_config(PIO pio) {
    bool activateClock;
    activateClock = set_sys_clock_khz(128000, false);

    // Inicializa PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    uint sm = pio_claim_unused_sm(pio, true);
    ws2812_program_init(pio, sm, offset, GPIO_WS2812);

    return sm;
}

// Inicializa LEDs RGB
void init_leds() {
    gpio_init(GPIO_LED_R);
    gpio_set_dir(GPIO_LED_R, GPIO_OUT);

    gpio_init(GPIO_LED_G);
    gpio_set_dir(GPIO_LED_G, GPIO_OUT);

    gpio_init(GPIO_LED_B);
    gpio_set_dir(GPIO_LED_B, GPIO_OUT);
}

// Inicializa botões A e B
void init_botoes() {
    gpio_init(GPIO_BOTAO_A);
    gpio_set_dir(GPIO_BOTAO_A, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(GPIO_BOTAO_A); // Habilita o pull-up interno

    gpio_init(GPIO_BOTAO_B);
    gpio_set_dir(GPIO_BOTAO_B, GPIO_IN); // Configura o pino como entrada
    gpio_pull_up(GPIO_BOTAO_B); // Habilita o pull-up interno
}

// Inicializa Display SSD1306
void init_I2C() {
    // Inicializa Diplay através da interface I2C usando a frequência de 400 Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    // Define a linha SDA na GPIO 14 (GPIO_I2C_SDA)
    gpio_set_function(GPIO_I2C_SDA, GPIO_FUNC_I2C);

    // Define a linha SLC na GPIO 15 (GPIO_I2C_SLC) 
    gpio_set_function(GPIO_I2C_SLC, GPIO_FUNC_I2C); 

    // Coloca o pino SDA em PULL UP
    gpio_pull_up(GPIO_I2C_SDA); 

    // Coloca o pino SLC em PULL UP
    gpio_pull_up(GPIO_I2C_SLC); 

    // Inicializa o display LCD
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDRESS, I2C_PORT); 

    // Passa os parâmetros de configuração do display
    ssd1306_config(&ssd); 

    // Envia os dados para o display
    ssd1306_send_data(&ssd); 

    // Inicia o display com todos os pixels apagados
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Rotina principal
int main() {
    // Inicializa comunicação USB CDC para monitor serial
    stdio_init_all();

    init_leds();
    init_botoes();
    
    init_I2C();

    pio = pio0; 
    sm = pio_config(pio);

    // Apaga LEDs
    numeros_pio(numeros_apaga, 0, pio, sm, 0, 0, 0);

    // Configuração da interrupção com callback ao acionar os botões A ou B
    gpio_set_irq_enabled_with_callback(GPIO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(GPIO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Variável para receber o caractere digitado pelo usuário via Serial Monitor
    char caractere;
    // Variável para conversão chat -> int
    int numero = 0;
    // Variável para verificar se houve conexão via USB para entrada/saída
    bool bUsbConnected = false;
    
    // Laço principal do programa 
    while (true) {
        bUsbConnected = stdio_usb_connected();
        // Verifica se o USB está conectado
        if (stdio_usb_connected()) {
            // Verifica se o usuário digitou algum caractere
            if (scanf("%c", &caractere) == 1) { 
                printf("Recebido: '%c'\n", caractere);
                // Converte de char para string
                char str1[2] = {caractere , '\0'};
                char str2[2] = "";
                strcpy(str2, str1);

                // Envia o caractere digitado para o display
                sprintf(linhaTextoDisplay, "%s%s", "CARACTERE  ", str2);
                ssd1306_draw_string(&ssd, linhaTextoDisplay, 8, 10); 
                // Atualiza o display
                ssd1306_send_data(&ssd); 
                // Verifica se o caractere digitado foi um número e acende a Matriz de LEDs 5x5
                switch (caractere) {
                    case '0':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '1':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '2':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '3':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '4':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '5':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '6':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '7':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '8':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                    case '9':
                        numero = caractere - '0';
                        printf("Numero = %d\n", numero);
                        gera_numeros(numero, pio, sm);
                        break;
                }
            }
        }
        else {
            printf("Conexão USB para I/O não estabelecida!\n\n");
        }
        sleep_ms(40);
    }
    return 0;
}