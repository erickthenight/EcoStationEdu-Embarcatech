# 🌱 EcoStation Edu - IoT Environmental Monitor

![Status](https://img.shields.io/badge/Status-Validation_Phase-yellow) ![Hardware](https://img.shields.io/badge/Hardware-BitDogLab%20RP2040-blue) ![Language](https://img.shields.io/badge/Language-C%20%2F%20Pico%20SDK-green)

Projeto da **Unidade 7** da residência tecnológica **EmbarcaTech**. Uma estação de monitoramento microclimático baseada em IoT para gestão de crises ambientais em escolas públicas.

## 📋 Sobre o Projeto

O **EcoStation Edu** é um dispositivo IoT desenhado para fornecer dados granulares de temperatura e qualidade do ar em escolas. Nesta fase de validação (RED), o foco foi estabelecer a conectividade estável e a integração de sensores.

### Funcionalidades Atuais (V13.0)
- **Web Server Embarcado:** Interface local para visualização de dados via navegador (Wi-Fi Station Mode).
- **Integração Nuvem (ThingSpeak):** Envio periódico de telemetria.
- **Alerta Visual:** Feedback imediato via Display OLED e LEDs RGB.
- **Modo Alta Performance:** Otimização do driver Wi-Fi (`CYW43_PERFORMANCE_PM`) para baixa latência.

## 🛠️ Hardware Necessário

- Placa de Desenvolvimento **BitDogLab** (Raspberry Pi Pico W).
- Cabo USB de Dados.
- Rede Wi-Fi 2.4GHz disponível.

## 🚀 Arquitetura e Roadmap

O firmware atual utiliza uma arquitetura de **Multitarefa Cooperativa** (Bare Metal) para validação de drivers.

### Próximos Passos (Entrega Final)
Conforme requisitos do projeto final, a próxima versão (V14.0) implementará o **FreeRTOS** para segregar as funções críticas:
- [ ] Migração para FreeRTOS (Tasks independentes).
- [ ] Implementação de Segurança Avançada na API.
- [ ] Integração com sensores industriais.

## 📂 Estrutura de Arquivos

- `main.c`: Lógica principal e Loops de controle.
- `CMakeLists.txt`: Configurações de compilação.
- `lwipopts.h`: Configurações da pilha TCP/IP.

## 📜 Licença

Desenvolvido por **Erick Mattos** para fins educacionais no âmbito do programa EmbarcaTech (2025/2026).
