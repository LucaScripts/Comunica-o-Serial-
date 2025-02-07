#ifndef UART_USB_H
#define UART_USB_H

#include "pico/stdlib.h"

#define PINO_LED_VERMELHO 13
#define PINO_LED_VERDE 11
#define PINO_LED_AZUL 12

void uart_init();
void processar_uart();

#endif
