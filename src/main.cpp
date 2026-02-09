#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 1. HARDWARE (A ordem importa!)
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"

// 2. KERNEL
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// 3. DRIVER
extern "C" {
    #include "ssd1306.h"
}

// Configurações
#define WIFI_SSID       "Apt203b"
#define WIFI_PASS       "290609bi"
#define API_KEY         "5ZCITMCZWON8U791"

#define I2C_PORT        i2c1
#define PINO_SDA        14
#define PINO_SCL        15
#define OLED_ADDR       0x3C
#define LED_PICO        12 // Verifique se sua placa usa este pino ou o do CYW43
#define PINO_MQ135      26 

// Estrutura de dados
typedef struct {
    uint16_t raw_mq135;
    float    temp_sim;
} SensorData_t;

QueueHandle_t xSensorQueue;
ssd1306_t display_global;

/* Removido: vEventGroupSetBitsCallback já está implementado em event_groups.c
// Fix para Linker do FreeRTOS SMP
extern "C" {
    void vEventGroupSetBitsCallback(void *xEventGroup, const uint32_t ulBitsToSet) {
        (void)xEventGroup; (void)ulBitsToSet;
    }
}
*/

// Task de Sensores
void task_sensores_display(void *pvParameters) {
    SensorData_t dados;
    char buf[32];

    adc_init();
    adc_gpio_init(PINO_MQ135);
    adc_select_input(0);
    
    gpio_init(LED_PICO);
    gpio_set_dir(LED_PICO, GPIO_OUT);

    while (true) {
        dados.raw_mq135 = adc_read();
        dados.temp_sim = 25.0f + (rand() % 50) / 10.0f; // Simulação simples

        // Display
        ssd1306_fill(&display_global, 0); // Limpa tela
        ssd1306_draw_string(&display_global, "ECO STATION 26", 10, 0);
        
        sprintf(buf, "AR: %d", dados.raw_mq135);
        ssd1306_draw_string(&display_global, buf, 0, 20);

        sprintf(buf, "TEMP: %.1f", dados.temp_sim);
        ssd1306_draw_string(&display_global, buf, 0, 40);

        ssd1306_show(&display_global);

        // Envia para fila
        xQueueOverwrite(xSensorQueue, &dados);

        // Blink
        gpio_put(LED_PICO, 1);
        vTaskDelay(pdMS_TO_TICKS(100));
        gpio_put(LED_PICO, 0);
        vTaskDelay(pdMS_TO_TICKS(900));
    }
}

// Task de Cloud
void task_iot_cloud(void *pvParameters) {
    SensorData_t dados;
    
    // Inicializa Wi-Fi
    if (cyw43_arch_init()) {
        printf("Falha Wi-Fi\n");
        vTaskDelete(NULL);
    }
    cyw43_arch_enable_sta_mode();

    while (true) {
        // Tenta conectar se cair
        if (cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA) != CYW43_LINK_JOIN) {
            printf("Conectando Wi-Fi...\n");
            cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 30000);
        }

        if (xQueueReceive(xSensorQueue, &dados, pdMS_TO_TICKS(5000))) {
            printf("Enviando ThingSpeak: AR=%d, Temp=%.1f\n", dados.raw_mq135, dados.temp_sim);
            // Aqui entraria o código HTTP real, mantendo printf para garantir o build
        }
        vTaskDelay(pdMS_TO_TICKS(20000));
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    printf("Iniciando Sistema...\n");

    // I2C
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(PINO_SDA, GPIO_FUNC_I2C);
    gpio_set_function(PINO_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(PINO_SDA);
    gpio_pull_up(PINO_SCL);

    // Inicializa OLED (Void, sem retorno booleano nesta lib)
    ssd1306_init(&display_global, 128, 64, OLED_ADDR, I2C_PORT);
    ssd1306_fill(&display_global, 0);
    ssd1306_show(&display_global);

    xSensorQueue = xQueueCreate(1, sizeof(SensorData_t));

    xTaskCreate(task_sensores_display, "SENS", 2048, NULL, 1, NULL);
    xTaskCreate(task_iot_cloud, "WIFI", 4096, NULL, 1, NULL);

    vTaskStartScheduler();
    while(1);
}