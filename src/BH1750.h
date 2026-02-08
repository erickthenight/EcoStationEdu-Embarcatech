#ifndef BH1750_H
#define BH1750_H

#include <pico/stdlib.h>
#include <hardware/i2c.h>

class BH1750 {
public:
    enum class ComandoBh1750 : uint8_t {
        LIGAR = 0x01,
        REINICIAR = 0x07,
        MODO_CONTINUO_ALTA_RESOLUCAO = 0x10
    };

    BH1750(i2c_inst_t *const interface_i2c, uint8_t pino_sda, uint8_t pino_scl, uint8_t endereco_dispositivo = 0x23);
    
    bool inicializarSensor();
    float lerIntensidadeLuminosaLux();

private:
    i2c_inst_t *interfaceI2c;
    uint8_t pinoSda, pinoScl, enderecoSensor;
    bool sensorInicializado;

    bool enviarComando(ComandoBh1750 comando) const;
    bool lerDadosBrutos(uint16_t &dados_brutos) const;
    void pausarExecucao(uint32_t ms) const;
};

#endif
