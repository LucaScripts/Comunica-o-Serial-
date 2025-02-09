/*
 * Autor: Lucas Dias
 * Descrição: Este código é responsável por controlar uma matriz de LEDs RGB, um display OLED e botões em um microcontrolador
 * utilizando a biblioteca Pico SDK. Ele inicializa as GPIOs, a comunicação I2C, a PIO para controle dos LEDs e configura
 * interrupções para os botões. O display OLED é utilizado para mostrar mensagens e o estado dos LEDs. A matriz de LEDs
 * exibe padrões numéricos de acordo com a entrada do usuário via comunicação serial.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "ws2812b.pio.h"

// Function prototype for num
void num(uint8_t value, PIO pio, uint sm);

// Definição dos pinos para conexão com os LEDs RGB
#define LED_G_PIN 11
#define LED_B_PIN 12

// Definição dos pinos para a conexão com os Botões
#define BTN_A_PIN 5
#define BTN_B_PIN 6

// Definições para uso dos LEDs na Matriz 5x5
#define LED_MTX_COUNT 25
#define LED_MTX_LEVEL 20 // A intensidade está baixa para não causar incômodo (0-255, caso deseje alterar)
#define LED_MTX_PIN 7

// Definições para o uso da comunicação serial I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ADDRESS 0x3C

#define DEBOUNCE_DELAY_MS 200
volatile uint32_t last_interrupt_time = 0;

// Pix GRB
typedef struct pixel_t {
    uint8_t G, R, B;
} led_t;

led_t led_matrix[LED_MTX_COUNT]; // Buffer de pixels que compõem a matriz

/*
 * Inicialização das GPIOs
 */
void init_gpio() {
    uint pins[] = {LED_G_PIN, LED_B_PIN, BTN_A_PIN, BTN_B_PIN};
    for (int i = 0; i < 4; i++) {
        gpio_init(pins[i]);
    }

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    gpio_set_dir(LED_B_PIN, GPIO_OUT);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);

    gpio_pull_up(BTN_A_PIN);
    gpio_pull_up(BTN_B_PIN);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

// Variáveis para uso da PIO
PIO pio;
uint sm;

/*
 * Inicialização da PIO
 */
void init_pio(uint pin) {
    uint offset = pio_add_program(pio0, &ws2812b_program);
    pio = pio0;

    sm = pio_claim_unused_sm(pio, false);

    ws2812b_program_init(pio, sm, offset, pin);
}

ssd1306_t ssd; // Inicializa a estrutura do display
void init_display() {
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADDRESS, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);  
    
    // Valores iniciais
    ssd1306_draw_string(&ssd, "Digite algo", 8, 10);
    ssd1306_draw_string(&ssd, "G OFF", 8, 48);
    ssd1306_draw_string(&ssd, "B OFF", 80, 48);
    ssd1306_send_data(&ssd);  
}

/*
 * Atribuição de cor a um LED
 */
void set_led(const uint id, const uint8_t R, const uint8_t G, const uint8_t B) {
    led_matrix[id].R = R;
    led_matrix[id].G = G;
    led_matrix[id].B = B;
}

/*
 * Limpeza do buffer de LEDs
 */
void clear_leds() {
    for (uint i = 0; i < LED_MTX_COUNT; i++) {
        led_matrix[i].R = 0;
        led_matrix[i].G = 0;
        led_matrix[i].B = 0;
    }
}

// Formatação do valor GRB
uint32_t rgb_value(uint8_t B, uint8_t R, uint8_t G){
  return (G << 24) | (R << 16) | (B << 8);
}

/*
 * Transferência dos valores do buffer para a matriz de LEDs
 */
void write_leds() {
    uint32_t value;
    for (uint i = 0; i < LED_MTX_COUNT; ++i) {
        value = rgb_value(led_matrix[i].B, led_matrix[i].R, led_matrix[i].G);
        pio_sm_put_blocking(pio, sm, value);
    }
}

/*
 * Decodificação do padrão binário para LEDs da matriz
 */

volatile bool green_led_on = false;
volatile bool blue_led_on = false;
volatile int number_id = -1;

/*
 * Limpa uma linha
 */
void clear_line(uint y) {
    for(int x = 8; x <= 112; x += 8) {
      ssd1306_draw_char(&ssd, ' ', x, y);
    }
}

void button_callback(uint gpio, uint32_t events) {
    uint32_t now = to_ms_since_boot(get_absolute_time()); // Obtém o tempo atual em ms

    if(now - last_interrupt_time > DEBOUNCE_DELAY_MS) { // Verifica se o botão foi pressionado após o tempo de debounce
        last_interrupt_time = now; // Atualiza o tempo da última interrupção

        // Alterna o estado do LED verde se o botão A for pressionado
        if(gpio == BTN_A_PIN) {
            green_led_on = !green_led_on;
            gpio_put(LED_G_PIN, green_led_on);
            ssd1306_draw_string(&ssd, green_led_on ? "G ON " : "G OFF", 8, 48); // Atualiza o display
            printf(green_led_on ? "LED Verde ON\n" : "LED Verde OFF\n");
        } 
        // Alterna o estado do LED azul se o botão B for pressionado
        else if(gpio == BTN_B_PIN) {
            blue_led_on = !blue_led_on;
            gpio_put(LED_B_PIN, blue_led_on);
            ssd1306_draw_string(&ssd, blue_led_on ? "B ON " : "B OFF", 80, 48); // Atualiza o display
            printf(blue_led_on ? "LED Azul ON\n" : "LED Azul OFF\n");
        }

        // Atualiza os LEDs da matriz se um número válido estiver selecionado
        if(number_id >= 0 && number_id <= 9) {
            num(number_id, pio, sm); // Chama a função num para exibir o número na matriz
        }

        ssd1306_send_data(&ssd); // Envia os dados atualizados para o display
    }
}

// Rotina para exibir números na matriz WS2812
void num(uint8_t value, PIO pio, uint sm) {
    static const uint8_t segmentos[10][15] = {
        {23, 22, 21, 16, 13, 6, 3, 2, 1, 8, 11, 18, 0},     // 0
        {22, 16, 17, 12, 7, 3, 2, 1},                       // 1
        {1, 2, 3, 6, 11 ,12, 13, 18, 21, 22, 23, 0},        // 2
        {1, 2, 3, 8, 11, 12, 13, 18, 21, 22, 23, 0},        // 3
        {1, 8, 11, 12, 13, 16, 23, 21, 18, 0},              // 4
        {21, 22, 23, 16, 13, 12, 11, 8, 1, 2, 3, 0},        // 5
        {21, 22, 23, 16, 13, 12, 11, 8, 1, 2, 3, 6, 0},     // 6
        {24, 23, 22, 2, 17, 13, 12, 11, 7},                 // 7
        {23, 22, 21, 16, 13, 6, 3, 2, 1, 8, 11, 18, 12, 0}, // 8
        {23, 22, 21, 16, 13, 1, 8, 11, 18, 12, 0}           // 9
    };

    clear_leds();
    for (int i = 0; segmentos[value][i] != 0; i++) {
        uint8_t R = LED_MTX_LEVEL, G = 0, B = 0; // Define a cor vermelha com intensidade reduzida
        if (green_led_on && blue_led_on) {
            R = LED_MTX_LEVEL; G = LED_MTX_LEVEL; B = LED_MTX_LEVEL; // Define a cor branca
        } else if (green_led_on) {
            R = 0; G = LED_MTX_LEVEL; B = 0; // Define a cor verde com intensidade reduzida
        } else if (blue_led_on) {
            R = 0; G = 0; B = LED_MTX_LEVEL; // Define a cor azul com intensidade reduzida
        }
        set_led(segmentos[value][i], R, G, B);
    }
    write_leds();
}

int main() {
    stdio_init_all();
    sleep_ms(500);

    init_pio(LED_MTX_PIN); // Configura o PIO para controlar a matriz de LEDs
    clear_leds();
    write_leds();
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o barramento I2C com frequência de 400 kHz
    init_gpio();
    init_display();

    // Configurando as interrupções para o pressionamento dos botões
    gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    gpio_set_irq_enabled_with_callback(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    char c; 
    int x = 8, y = 10; // Posição inicial do caractere
    while (true) {
        sleep_ms(100);
        scanf("%c", &c);

        if (c == 127) { // Verifica se é o caractere de backspace (ASCII 127)
            if (x > 8) {
                x -= 8; // Move o cursor para a posição anterior
            } else if (y > 10) {
                y -= 9; // Move para a linha anterior
                x = 112; // Define a posição x para o final da linha anterior
            }
            ssd1306_draw_char(&ssd, ' ', x, y); // Apaga o caractere atual
        } else if (c == '\n' || c == '\r') { // Verifica se é o caractere de enter
            x = 8; // Volta para o início da linha
            y += 9; // Move para a próxima linha
            if (y > 28) y = 10; // Verifica se já é a última linha e retorna para a primeira caso seja
        } else {
            if (x == 8) clear_line(y); // Limpar uma nova linha
            ssd1306_draw_char(&ssd, c, x, y); // Escreve o caractere

            x += 8; // Ajusta o valor de x para a coordenada do próximo caractere
            if (x > 112) { // Verifica se já passou do limite da linha
                x = 8; // Volta para o início
                y += 9; // Redireciona para a próxima linha
                if (y > 28) y = 10; // Verifica se já é a última linha e retorna para a primeira caso seja
            }

            if(c >= '0' && c <= '9') {
                number_id = c - '0';
                num(number_id, pio, sm); // Chama a função num para exibir o número na matriz
            }
        }
        ssd1306_send_data(&ssd); // Atualiza o display
    }
}
