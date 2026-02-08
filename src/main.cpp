#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "FreeRTOS.h"
#include "task.h"

extern "C" {
    #include "ssd1306.h"
}

// --- CONFIGURAÇÃO DE HARDWARE ---
#define I2C_PORT i2c1
#define PINO_SDA 14
#define PINO_SCL 15
#define OLED_ADDR 0x3C
#define LED_PICO 12
#define PINO_MQ135 26
#define BH1750_ADDR 0x23
#define AHT10_ADDR  0x38

ssd1306_t display_global;
volatile int g_ar_ppm = 0;
volatile float g_lux = 0.0f, g_temp = 0.0f, g_umi = 0.0f;

// Linker Fix para FreeRTOS
extern "C" {
    void vEventGroupSetBitsCallback(void *xEventGroup, const uint32_t ulBitsToSet) {
        (void)xEventGroup; (void)ulBitsToSet;
    }
}

// Task Principal de Monitoramento
void task_principal(void *pvParameters) {
    char buf[32];
    adc_init(); adc_gpio_init(PINO_MQ135);
    gpio_init(LED_PICO); gpio_set_dir(LED_PICO, GPIO_OUT);

    while (true) {
        // Leitura AR (MQ135) - Funcionando!
        adc_select_input(0);
        g_ar_ppm = adc_read();

        // Lógica de Atualização do Visor
        ssd1306_fill(&display_global, false);
        ssd1306_draw_string(&display_global, "EMBARCATECH AM", 10, 2);
        ssd1306_draw_string(&display_global, "RESIDENTE: ERICK", 10, 12);
        
        // Espaçamento Profissional entre Blocos
        sprintf(buf, "AR:  %d PPM", g_ar_ppm);
        ssd1306_draw_string(&display_global, buf, 5, 28);
        
        sprintf(buf, "LUZ: %.1f LX", g_lux);
        ssd1306_draw_string(&display_global, buf, 5, 42);
        
        sprintf(buf, "UMI: %.1f %%", g_umi);
        ssd1306_draw_string(&display_global, buf, 5, 54);
        
        ssd1306_show(&display_global);

        // Heartbeat (Prova de vida do Kernel)
        gpio_put(LED_PICO, 1);
        vTaskDelay(pdMS_TO_TICKS(150));
        gpio_put(LED_PICO, 0);
        vTaskDelay(pdMS_TO_TICKS(1350));
    }
}

int main() {
    stdio_init_all();
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA); gpio_pull_up(PINO_SCL);

    sleep_ms(1000);
    ssd1306_init(&display_global, 128, 64, OLED_ADDR, I2C_PORT);
    
    // Task Final para EmbarcaTech
    xTaskCreate(task_principal, "MONITOR", 2048, NULL, 1, NULL);
    vTaskStartScheduler();
    while(1);
}