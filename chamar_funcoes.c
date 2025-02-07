#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/irq.h"
#include "ws2812.pio.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc/ws2812.h" // Inclua o cabeçalho ws2812.h

// Definições de pinos
#define PINO_LED_VERMELHO 13
#define PINO_LED_VERDE 11
#define PINO_LED_AZUL 12
#define PINO_WS2812 7
#define PINO_BOTAO_A 5
#define PINO_BOTAO_B 6
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_SSD1306 0x3C

// Variáveis globais
volatile int numero_exibido = 0;
ssd1306_t ssd;
PIO pio = pio0;
uint sm = 0;

// Protótipos de funções
void configurar_perifericos();
void atualizar_display_ws2812(PIO pio, uint sm);
void manipulador_irq_gpio(uint gpio, uint32_t eventos);
void processar_uart();

int main() {
    stdio_init_all();
    configurar_perifericos();

    while (true) {
        processar_uart();
        sleep_ms(100);
    }

    return 0;
}

void configurar_perifericos() {
    // Inicializar GPIOs
    gpio_init(PINO_LED_VERMELHO);
    gpio_set_dir(PINO_LED_VERMELHO, GPIO_OUT);
    gpio_init(PINO_LED_VERDE);
    gpio_set_dir(PINO_LED_VERDE, GPIO_OUT);
    gpio_init(PINO_LED_AZUL);
    gpio_set_dir(PINO_LED_AZUL, GPIO_OUT);

    // Inicializar I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializar display SSD1306
    ssd1306_init(&ssd, 128, 64, false, ENDERECO_SSD1306, I2C_PORT);
    ssd1306_clear(&ssd);
    ssd1306_draw_string(&ssd, "Inicializado", 0, 0);
    ssd1306_show(&ssd);

    // Inicializar matriz WS2812
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, PINO_WS2812, 800000, false);

    // Configurar botões com interrupções
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &manipulador_irq_gpio);
    gpio_set_irq_enabled_with_callback(PINO_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &manipulador_irq_gpio);
}

void manipulador_irq_gpio(uint gpio, uint32_t eventos) {
    if (gpio == PINO_BOTAO_A) {
        numero_exibido = (numero_exibido + 1) % 10;
        printf("Incrementado: %d\n", numero_exibido);
    } else if (gpio == PINO_BOTAO_B) {
        numero_exibido = (numero_exibido - 1 + 10) % 10;
        printf("Decrementado: %d\n", numero_exibido);
    }
    atualizar_display_ws2812(pio, sm);
}

void processar_uart() {
    if (stdio_usb_connected()) {
        char c;
        if (scanf("%c", &c) == 1) {
            switch (c) {
                case 'r': gpio_put(PINO_LED_VERMELHO, !gpio_get(PINO_LED_VERMELHO)); break;
                case 'g': gpio_put(PINO_LED_VERDE, !gpio_get(PINO_LED_VERDE)); break;
                case 'b': gpio_put(PINO_LED_AZUL, !gpio_get(PINO_LED_AZUL)); break;
                default: printf("Comando inválido: %c\n", c);
            }
        }
    }
}

void atualizar_display_ws2812(PIO pio, uint sm) {
    for (int i = 0; i < 25; i++) {
        colocar_pixel(pio, sm, PINO_WS2812, 0x00400000); // Exemplo de cor fixa
    }
}
