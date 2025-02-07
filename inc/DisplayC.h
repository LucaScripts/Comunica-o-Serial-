#ifndef DISPLAY_H
#define DISPLAY_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_SSD1306 0x3C

void display_init(ssd1306_t *ssd);
void display_atualizar_texto(ssd1306_t *ssd, const char *texto);

#endif
