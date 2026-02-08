#include "BH1750.h"
#include "FreeRTOS.h"
#include "task.h"

BH1750::BH1750(i2c_inst_t *const interface_i2c, uint8_t pino_sda, uint8_t pino_scl, uint8_t endereco_dispositivo)
    : interfaceI2c(interface_i2c), pinoSda(pino_sda), pinoScl(pino_scl), 
      enderecoSensor(endereco_dispositivo), sensorInicializado(false) {}

bool BH1750::inicializarSensor() {
    if (sensorInicializado) return true;
    
    pausarExecucao(10);
    if (!enviarComando(ComandoBh1750::LIGAR)) return false;
    pausarExecucao(10);
    if (!enviarComando(ComandoBh1750::MODO_CONTINUO_ALTA_RESOLUCAO)) return false;

    sensorInicializado = true;
    return true;
}

float BH1750::lerIntensidadeLuminosaLux() {
    if (!sensorInicializado) return -1.0f;

    uint16_t dados_brutos = 0;
    if (!lerDadosBrutos(dados_brutos)) return -1.0f;

    return static_cast<float>(dados_brutos) / 1.2f;
}

bool BH1750::enviarComando(ComandoBh1750 comando) const {
    uint8_t cmd = static_cast<uint8_t>(comando);
    return i2c_write_blocking(interfaceI2c, enderecoSensor, &cmd, 1, false) == 1;
}

bool BH1750::lerDadosBrutos(uint16_t &dados_brutos) const {
    uint8_t buffer[2] = {0};
    // O sensor precisa de tempo para converter a luz em bits
    pausarExecucao(180); 
    
    if (i2c_read_blocking(interfaceI2c, enderecoSensor, buffer, 2, false) != 2) return false;
    
    dados_brutos = (static_cast<uint16_t>(buffer[0]) << 8) | buffer[1];
    return true;
}

void BH1750::pausarExecucao(uint32_t ms) const {
    vTaskDelay(pdMS_TO_TICKS(ms));
}