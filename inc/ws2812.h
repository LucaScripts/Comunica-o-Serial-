#ifndef WS2812_H
#define WS2812_H

#include "hardware/pio.h"

void colocar_pixel(PIO pio, uint sm, uint pin, uint32_t color);
void atualizar_display_ws2812(PIO pio, uint sm);
void manipulador_irq_gpio(uint gpio, uint32_t eventos);

#endif // WS2812_H