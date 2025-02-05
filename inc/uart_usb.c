/*
 * Por: Lucas Dias da Silva
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
#include "uart_usb.h"

#define led_pin_g 11
#define led_pin_b 12
#define led_pin_r 13

void controlar_leds() {
    stdio_init_all();  // Inicializa a comunicação serial

    // Inicializa os pinos dos LEDs
    gpio_init(led_pin_r);
    gpio_set_dir(led_pin_r, GPIO_OUT);
    gpio_put(led_pin_r, 0);

    gpio_init(led_pin_g);
    gpio_set_dir(led_pin_g, GPIO_OUT);
    gpio_put(led_pin_g, 0);

    gpio_init(led_pin_b);
    gpio_set_dir(led_pin_b, GPIO_OUT);
    gpio_put(led_pin_b, 0);

    printf("Controle de LEDs iniciado. Envie 'r', 'g' ou 'b' para alternar.\n");

    while (true) {
        if (stdio_usb_connected()) {  // Verifica se a comunicação USB está conectada
            if (uart_is_readable(uart0)) {  // Verifica se há dados disponíveis na UART
                char c = uart_getc(uart0);  // Lê o caractere da UART
                printf("Recebido: '%c'\n", c);
                
                switch (c) {
                    case 'r':
                        gpio_put(led_pin_r, !gpio_get(led_pin_r));  // Alterna o LED vermelho
                        printf("LED vermelho alternado!\n");
                        break;
                    case 'g':
                        gpio_put(led_pin_g, !gpio_get(led_pin_g));  // Alterna o LED verde
                        printf("LED verde alternado!\n");
                        break;
                    case 'b':
                        gpio_put(led_pin_b, !gpio_get(led_pin_b));  // Alterna o LED azul
                        printf("LED azul alternado!\n");
                        break;
                    default:
                        printf("Comando inválido: '%c'\n", c);
                }
            }
        }
        sleep_ms(40);  // Ajuste o tempo de espera conforme necessário
    }
}
