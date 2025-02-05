#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "uart_usb.h"
#include "DisplayC.h"

typedef void (*FuncaoPrincipal)();

void executar_funcao(FuncaoPrincipal funcao) {
    if (funcao) {
        funcao();
    }
}

int main() {
    stdio_init_all();
    
    printf("Iniciando os sistemas...\n");

    executar_funcao(iniciar_oled);
    executar_funcao(controlar_leds);
    
    return 0;
}
