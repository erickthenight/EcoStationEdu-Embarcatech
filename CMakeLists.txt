cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

project(EcoStation_Edu C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# --- VITAL: Diz ao FreeRTOS onde encontrar o seu FreeRTOSConfig.h ---
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(FREERTOS_PORT GCC_RP2040 CACHE STRING "")
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE src)
target_link_libraries(freertos_config INTERFACE pico_stdlib pico_multicore) 

add_subdirectory(lib/FreeRTOS-Kernel)

add_executable(EcoStation_Edu
    src/main.cpp
    src/Aht10.cpp
    src/BH1750.cpp
    src/ssd1306.c
)

target_include_directories(EcoStation_Edu PRIVATE src)

target_link_libraries(EcoStation_Edu
    pico_stdlib
    pico_cyw43_arch_lwip_sys_freertos
    freertos_kernel
    hardware_i2c
    hardware_adc
)

# Blindagem contra erros de conversão do Windows
target_compile_options(freertos_kernel PRIVATE -Wno-error -Wno-conversion)
target_compile_options(freertos_kernel_port PRIVATE -Wno-error -Wno-conversion)
target_compile_options(EcoStation_Edu PRIVATE -Wno-error)

pico_enable_stdio_usb(EcoStation_Edu 1)
pico_add_extra_outputs(EcoStation_Edu)