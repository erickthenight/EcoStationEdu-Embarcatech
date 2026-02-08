#ifndef AHT10_H
#define AHT10_H

#include <stddef.h>
#include <cstdint>
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

class Aht10 {
public:
    static constexpr uint8_t ENDERECO_PADRAO = 0x38;
    
    // Construtores
    Aht10(i2c_inst_t *controlador_i2c, uint8_t pino_sda, uint8_t pino_scl, uint8_t endereco = ENDERECO_PADRAO);
    
    // Métodos públicos
    void inicializar();
    bool disponivel();
    float obterTemperaturaCelsius() const;
    float obterUmidadeRelativa() const;
    void resetar();

private:
    // Constantes de comandos
    static constexpr uint8_t AHT10_CMD_CALIBRATION = 0xE1;
    static constexpr uint8_t AHT10_CMD_CALIBRATION_ARG = 0x08;
    static constexpr uint8_t AHT10_CMD_CALIBRATION_ARG2 = 0x00;
    static constexpr uint8_t AHT10_CMD_MEASURE = 0xAC;
    static constexpr uint8_t AHT10_CMD_MEASURE_ARG = 0x33;
    static constexpr uint8_t AHT10_CMD_MEASURE_ARG2 = 0x00;
    static constexpr uint8_t AHT10_CMD_SOFT_RESET = 0xBA;
    static constexpr uint8_t AHT10_CMD_SOFT_RESET_ARG = 0x00;
    static constexpr uint8_t AHT10_CMD_SOFT_RESET_ARG2 = 0x00;
    
    // Métodos privados
    void enviarComandos(const uint8_t *comandos, size_t tamanho);
    void calibrar();
    void enviarComandoMedicao();
    void lerDados(uint8_t *buf);
    void processarDados(uint8_t *buf);
    void pausarExecucaoPorMilissegundos(uint32_t tempo_milissegundos);
    
    // Variáveis de instância
    i2c_inst_t *controladorI2c;
    uint8_t pinoSda;
    uint8_t pinoScl;
    uint8_t enderecoI2c;
    bool leituraDisponivel;
    uint32_t amostragemTemperatura;
    uint32_t amostragemUmidade;
};

#endif
