cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

# --- FORÇA A PLACA SER A PICO W (ADICIONE ESTA LINHA) ---
set(PICO_BOARD pico_w)
# --------------------------------------------------------

project(EcoStation_Start C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Cria o executável juntando o main e o driver do display
add_executable(EcoStation_Start
    main.c
    inc/ssd1306.c
)

# Avisa onde estão os headers (.h)
target_include_directories(EcoStation_Start PRIVATE inc)

# Habilita as funções da placa
target_link_libraries(EcoStation_Start
    pico_stdlib
    hardware_adc
    hardware_pwm
    hardware_i2c
    pico_cyw43_arch_lwip_threadsafe_background  # <--- A LINHA MÁGICA DO WI-FI
)

# Gera o arquivo .uf2
pico_add_extra_outputs(EcoStation_Start)