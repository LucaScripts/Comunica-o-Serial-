/*
 * Por: Wilton Lacerda Silva
 *
 * Utiliza a UART para controlar LEDs RGB.
 *
 * O teste foi feito utilizando a extensão do Wokwi Simulator:
 *
 * O funcionamento é simples, o código lê um caractere da UART e acende
 * um LED RGB de acordo com o caractere recebido. Por exemplo, se o caractere
 * 'r' for recebido, o LED vermelho é acionado. Se o caractere 'g' for recebido,
 * o LED verde é acionado. Se o caractere 'b' for recebido, o LED azul é acionado.
 *
 * Obs. Veja o consumo de CPU do seu PC na simulação.
 * No loop não há delay, então o código está sempre lendo a UART.
 */

#include <stdio.h>
#include "pico/stdlib.h"

#define led_pin_g 11
#define led_pin_b 12
#define led_pin_r 13

// Inicializa a comunicação e configura os pinos dos LEDs
void inicializar_pinos_led() {
    gpio_init(led_pin_r);
    gpio_set_dir(led_pin_r, GPIO_OUT);
    gpio_put(led_pin_r, 0); // Inicialmente desligado

    gpio_init(led_pin_g);
    gpio_set_dir(led_pin_g, GPIO_OUT);
    gpio_put(led_pin_g, 0); // Inicialmente desligado

    gpio_init(led_pin_b);
    gpio_set_dir(led_pin_b, GPIO_OUT);
    gpio_put(led_pin_b, 0); // Inicialmente desligado
}

// Alterna o LED com base no caractere recebido
void alternar_led(char c) {
    switch (c) {
        case 'r':
            gpio_put(led_pin_r, !gpio_get(led_pin_r));
            printf("LED vermelho alternado!\n");
            break;
        case 'g':
            gpio_put(led_pin_g, !gpio_get(led_pin_g));
            printf("LED verde alternado!\n");
            break;
        case 'b':
            gpio_put(led_pin_b, !gpio_get(led_pin_b));
            printf("LED azul alternado!\n");
            break;
        default:
            printf("Comando inválido: '%c'\n", c);
    }
}

// Função principal removida, lógica mantida em funções auxiliares
