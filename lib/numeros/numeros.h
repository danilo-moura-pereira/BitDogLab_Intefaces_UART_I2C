#ifndef NUMEROS_H
#define NUMEROS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// Função que define a cor de cada led
uint32_t matrix_rgb2(double b, double r, double g);

// Função para inicializar a matrix de LEDs
void numeros_pio(double *numero, uint32_t iRgb_led, PIO pio, uint sm, double r, double g, double b);

// Função que exibe os números no painel de LEDs 5x5
void gera_numeros(char tecla, PIO pio, uint sm);

#endif // NUMEROS_H
