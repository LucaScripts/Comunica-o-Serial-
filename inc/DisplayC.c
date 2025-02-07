/*
 * Por: Wilton Lacerda Silva
 *    Comunicação serial com I2C
 *  
 * Uso da interface I2C para comunicação com o Display OLED
 * 
 * Estudo da biblioteca ssd1306 com PicoW na Placa BitDogLab.
 *  
 * Este programa escreve uma mensagem no display OLED.
 * 
 * 
*/


#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Função para inicializar a comunicação I2C e o display OLED
void inicializar_display(ssd1306_t *ssd) {
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA); // Pull up the data line
  gpio_pull_up(I2C_SCL); // Pull up the clock line
  
  ssd1306_init(ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(ssd); // Configura o display
  ssd1306_send_data(ssd); // Envia os dados para o display
  
  ssd1306_fill(ssd, false); // Limpa o display
  ssd1306_send_data(ssd);
}

// Função para atualizar o conteúdo do display com animações
void atualizar_display(ssd1306_t *ssd) {
  static bool cor = true;
  
  cor = !cor; // Alterna a cor
  ssd1306_fill(ssd, !cor); // Limpa o display
  ssd1306_rect(ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
  ssd1306_draw_string(ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
  ssd1306_draw_string(ssd, "EMBARCATECH", 20, 30); // Desenha uma string
  ssd1306_draw_string(ssd, "PROF WILTON", 15, 48); // Desenha uma string
  ssd1306_send_data(ssd); // Atualiza o display
  
  sleep_ms(1000); // Atraso para animação
}
