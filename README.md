# EcoStation Edu - Monitoramento Ambiental com FreeRTOS

**Residente:** Erick de Araujo Mattos  
**Programa:** Residência em Sistemas Embarcados (EmbarcaTech)  
**Instituição:** IFCE / IREDE AM  
**Local:** Manaus - AM  

## 🚀 Visão Geral
O **EcoStation Edu** é um hub de monitoramento climático desenvolvido sobre a arquitetura do microcontrolador **RP2040 (Raspberry Pi Pico W)**. O projeto utiliza o sistema operacional de tempo real **FreeRTOS** para garantir o determinismo e a estabilidade na coleta de dados ambientais em tempo real.

## 🛠️ Tecnologias e Ferramentas
- **Linguagem:** C++ / C
- **Kernel:** FreeRTOS (Escalonamento Preemptivo)
- **SDK:** Raspberry Pi Pico SDK v1.5.1
- **Hardware:** Placa BitDogLab (RP2040)
- **Interface Visual:** Display OLED SSD1306 (I2C)
- **Sensores:** - MQ135 (Qualidade do Ar/Gases - ADC)
  - BH1750 (Luminosidade - I2C)
  - AHT10 (Umidade e Temperatura - I2C)

## 🏗️ Arquitetura de Software
O diferencial deste firmware é a gestão multitarefa. O sistema opera através de tarefas independentes (tasks), garantindo que a atualização da IHM (Interface Homem-Máquina) não interfira na precisão das leituras dos sensores.

### Pontos de Destaque Técnico:
- **Resolução de Conflitos de Linkagem:** Tratamento de dependências multicore do SDK 1.5.1.
- **Robustez de Barramento:** Implementação de timeouts I2C para resiliência de hardware.
- **Heartbeat System:** Monitoramento visual da integridade do Kernel via LED (GP12).

## 📊 Como executar o projeto
1. Clone o repositório.
2. Certifique-se de ter o **Pico SDK v1.5.1** configurado.
3. Compile utilizando a extensão CMake do VS Code.
4. Carregue o arquivo `EcoStation_Edu.uf2` na placa.

## 📈 Próximos Passos
- Integração de protocolo **MQTT** para telemetria em nuvem.
- Implementação de log de dados em **Micro SD Card**.

---
*Projeto desenvolvido como requisito para a Unidade 7 da Residência EmbarcaTech.*