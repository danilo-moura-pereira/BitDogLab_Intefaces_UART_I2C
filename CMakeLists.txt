# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.0.0)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(UART_I2C C CXX ASM)

pico_sdk_init()

add_executable(UART_I2C
    UART_I2C.c
    lib/numeros/numeros.c
    lib/ssd1306.c
)

# Adiciona diretórios de inclusão
include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib/numeros
)

# Adiciona o arquivo PIO
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
pico_generate_pio_header(UART_I2C ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)


pico_enable_stdio_usb(UART_I2C 1)
pico_enable_stdio_uart(UART_I2C 0)


target_link_libraries(UART_I2C
    pico_stdlib
    hardware_pio
    hardware_clocks
	hardware_adc
    hardware_i2c
    hardware_uart
)

pico_enable_stdio_usb(UART_I2C 1)
pico_enable_stdio_uart(UART_I2C 1)

pico_add_extra_outputs(UART_I2C)