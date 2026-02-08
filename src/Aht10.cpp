#include "Aht10.h"

// Se o CMake habilitou o FreeRTOS, usamos as bibliotecas de tasks
#ifdef HABILITAR_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

// Construtor: Mapeia os pinos e o endereço do sensor
Aht10::Aht10(i2c_inst_t *controlador_i2c, uint8_t pino_sda, uint8_t pino_scl, uint8_t endereco)
    : controladorI2c(controlador_i2c),
      pinoSda(pino_sda),
      pinoScl(pino_scl),
      enderecoI2c(endereco),
      leituraDisponivel(false),
      amostragemTemperatura(0),
      amostragemUmidade(0)
{
}

// Inicializa o sensor enviando o comando de calibração
void Aht10::inicializar()
{
    leituraDisponivel = false;
    amostragemTemperatura = 0;
    amostragemUmidade = 0;
    
    resetar();
    pausarExecucaoPorMilissegundos(20);
    calibrar();
}

// Cálculo matemático conforme o Datasheet (Retorna Celsius)
float Aht10::obterTemperaturaCelsius() const
{
    return ((amostragemTemperatura * 200.0f) / (1 << 20)) - 50.0f;
}

// Cálculo matemático conforme o Datasheet (Retorna % de Umidade)
float Aht10::obterUmidadeRelativa() const
{
    return (amostragemUmidade * 100.0f) / (1 << 20);
}

// Solicita uma nova medição ao hardware
void Aht10::enviarComandoMedicao()
{
    uint8_t cmd[3] = {
        AHT10_CMD_MEASURE,
        AHT10_CMD_MEASURE_ARG,
        AHT10_CMD_MEASURE_ARG2
    };
    enviarComandos(cmd, sizeof(cmd));
}

// Lê os 6 bytes brutos do barramento I2C
void Aht10::lerDados(uint8_t *buf)
{
    i2c_read_blocking(controladorI2c, enderecoI2c, buf, 6, false);
}

// Transforma os bytes lidos em valores numéricos (Temperatura e Umidade)
void Aht10::processarDados(uint8_t *buf)
{
    amostragemUmidade = ((uint32_t)buf[1] << 12) | ((uint32_t)buf[2] << 4) | ((buf[3] >> 4) & 0x0F);
    amostragemTemperatura = (((uint32_t)(buf[3] & 0x0F)) << 16) | ((uint32_t)buf[4] << 8) | buf[5];
}

// Função principal de verificação: Dispara a leitura e aguarda o tempo de resposta
bool Aht10::disponivel()
{
    leituraDisponivel = false;

    enviarComandoMedicao();
    
    // O AHT10 precisa de ~75ms para processar a leitura. 
    // No FreeRTOS, isso libera a CPU para o Display rodar enquanto o sensor trabalha.
    pausarExecucaoPorMilissegundos(80); 
    
    uint8_t buf[6];
    lerDados(buf);
    processarDados(buf);
    
    leituraDisponivel = true;
    return leituraDisponivel;
}

void Aht10::calibrar()
{
    uint8_t cmd[3] = {
        AHT10_CMD_CALIBRATION,
        AHT10_CMD_CALIBRATION_ARG,
        AHT10_CMD_CALIBRATION_ARG2
    };
    enviarComandos(cmd, sizeof(cmd));
}

void Aht10::resetar()
{
    uint8_t cmd[3] = {
        AHT10_CMD_SOFT_RESET,
        AHT10_CMD_SOFT_RESET_ARG,
        AHT10_CMD_SOFT_RESET_ARG2
    };
    enviarComandos(cmd, sizeof(cmd));   
    pausarExecucaoPorMilissegundos(20);
}

void Aht10::enviarComandos(const uint8_t *comandos, size_t tamanho)
{
    i2c_write_blocking(controladorI2c, enderecoI2c, comandos, tamanho, false);
}

// Função inteligente de pausa: Diferencia se estamos no RTOS ou no Bare Metal (SDK puro)
void Aht10::pausarExecucaoPorMilissegundos(uint32_t tempo_milissegundos)
{
#ifdef HABILITAR_FREERTOS
    vTaskDelay(pdMS_TO_TICKS(tempo_milissegundos));
#else
    sleep_ms(tempo_milissegundos);
#endif
}
