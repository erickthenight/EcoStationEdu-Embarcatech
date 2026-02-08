#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

typedef struct {
    uint8_t width, height, pages, address;
    i2c_inst_t *i2c_port;
    uint8_t buffer[1025]; // Buffer fixo para evitar erro de memória
} ssd1306_t;

void ssd1306_init(ssd1306_t *p, uint8_t width, uint8_t height, uint8_t address, i2c_inst_t *i2c);
void ssd1306_fill(ssd1306_t *p, bool color);
void ssd1306_draw_string(ssd1306_t *p, const char *str, uint8_t x, uint8_t y);
void ssd1306_show(ssd1306_t *p);

#endif