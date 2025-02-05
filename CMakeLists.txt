# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == Configuração do Raspberry Pi Pico SDK ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico CACHE STRING "Board type")

# Importar o SDK do Raspberry Pi Pico
include(pico_sdk_import.cmake)

# Nome do projeto
project(ComunicaoSerial C CXX ASM)

# Inicializar o SDK do Pico
pico_sdk_init()

# Adicionar o executável principal
add_executable(ComunicaoSerial 
    chamar_funcoes.c 
    inc/uart_usb.c 
    inc/DisplayC.c 
    inc/ssd1306.c
)

# Configurar nome e versão do programa
pico_set_program_name(ComunicaoSerial "ComunicaoSerial")
pico_set_program_version(ComunicaoSerial "0.1")

# Habilitar saída via USB e desativar via UART
pico_enable_stdio_uart(ComunicaoSerial 0)
pico_enable_stdio_usb(ComunicaoSerial 1)

# Adicionar bibliotecas necessárias
target_link_libraries(ComunicaoSerial
    pico_stdlib
    hardware_i2c
)

# Incluir diretórios de cabeçalhos
target_include_directories(ComunicaoSerial PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

# Gerar arquivos extras para depuração e programação
pico_add_extra_outputs(ComunicaoSerial)
