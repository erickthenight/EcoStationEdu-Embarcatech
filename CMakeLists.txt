cmake_minimum_required(VERSION 3.13)

# 1. Configuração da Placa (CRÍTICO: Tem que ser pico_w)
set(PICO_BOARD pico_w)

# Importa o SDK da Raspberry Pi
include(pico_sdk_import.cmake)

project(EcoStation_Edu C CXX ASM)

# Inicializa o ambiente do SDK
pico_sdk_init()

# ==============================================================================
# 2. Configuração do FreeRTOS (SMP / Dual Core)
# ==============================================================================
set(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel")
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# ==============================================================================
# 3. Definição do Executável e Fontes
# ==============================================================================
# ATENÇÃO: Adicionei o 'src/ssd1306.c'. Sem isso, o Linker falha ao procurar as funções do display.
add_executable(EcoStation_Edu
    src/main.cpp
    src/ssd1306.c
)

# ==============================================================================
# 4. Diretórios de Inclusão (Onde estão os .h)
# ==============================================================================
target_include_directories(EcoStation_Edu PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/lib
)

# ==============================================================================
# 5. Linkagem de Bibliotecas (O "Pulo do Gato")
# ==============================================================================
target_link_libraries(EcoStation_Edu
    # Bibliotecas Padrão
    pico_stdlib
    
    # A LIB MÁGICA: Habilita Wi-Fi (cyw43) + TCP/IP (lwip) + FreeRTOS (sys)
    # Isso conecta o driver de rede às Tasks do FreeRTOS automaticamente
    pico_cyw43_arch_lwip_sys_freertos
    
    # O Kernel do Sistema Operacional
    FreeRTOS-Kernel
    
    # Periféricos de Hardware
    hardware_adc
    hardware_i2c
    hardware_dma  # Necessário internamente pelo driver de Wi-Fi
    hardware_pwm  # Preparado caso use PWM no futuro
)

# ==============================================================================
# 6. Configurações de Saída (Serial e Arquivos)
# ==============================================================================
# Habilita USB (Serial) e desabilita UART (Pinos 0 e 1) para evitar lixo no boot
pico_enable_stdio_usb(EcoStation_Edu 1)
pico_enable_stdio_uart(EcoStation_Edu 0)

# Gera os arquivos finais (.uf2, .hex, .bin, .map)
pico_add_extra_outputs(EcoStation_Edu)

# Força o padrão C++17 para evitar erros de compatibilidade no main.cpp
target_compile_features(EcoStation_Edu PRIVATE cxx_std_17)