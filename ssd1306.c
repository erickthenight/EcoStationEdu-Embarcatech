#include "ssd1306.h"
#include <string.h>
#include <stdlib.h>
#include "hardware/i2c.h"

#define SSD1306_SET_MEM_MODE 0x20
#define SSD1306_SET_COL_ADDR 0x21
#define SSD1306_SET_PAGE_ADDR 0x22
#define SSD1306_SET_DISP_START_LINE 0x40
#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_SET_SEG_REMAP 0xA0
#define SSD1306_SET_DISP 0xAE
#define SSD1306_SET_COM_OUT_DIR 0xC0
#define SSD1306_SET_DISP_OFFSET 0xD3
#define SSD1306_SET_COM_PIN_CFG 0xDA
#define SSD1306_SET_DISP_CLK_DIV 0xD5
#define SSD1306_SET_PRECHARGE 0xD9
#define SSD1306_SET_VCOM_DESEL 0xDB
#define SSD1306_SET_CHARGE_PUMP 0x8D

const uint8_t font_basic[][5] = {
    {0,0,0,0,0}, {0,0,95,0,0}, {0,7,0,7,0}, {20,127,20,127,20}, {36,42,127,42,18}, 
    {35,19,8,100,98}, {54,73,85,34,5}, {0,5,3,0,0}, {0,28,34,65,0}, {0,65,34,28,0}, 
    {20,8,62,8,20}, {8,8,62,8,8}, {0,80,48,0,0}, {8,8,8,8,8}, {0,96,96,0,0}, {32,16,8,4,2}, 
    {62,81,73,69,62}, {0,66,127,64,0}, {66,97,81,73,70}, {33,65,69,75,49}, {24,20,18,127,16}, 
    {39,69,69,69,57}, {60,74,73,73,48}, {1,113,9,5,3}, {54,73,73,73,54}, {6,73,73,41,30}, 
    {0,54,54,0,0}, {0,86,54,0,0}, {8,20,34,65,0}, {20,20,20,20,20}, {0,65,34,20,8}, {2,1,81,9,6}, 
    {50,73,121,65,62}, {126,17,17,17,126}, {127,73,73,73,54}, {62,65,65,65,34}, {127,65,65,34,28}, 
    {127,73,73,65,65}, {127,9,9,9,1}, {62,65,73,73,122}, {127,8,8,8,127}, {0,65,127,65,0}, 
    {32,64,65,63,1}, {127,8,20,34,65}, {127,64,64,64,64}, {127,2,12,2,127}, {127,4,8,16,127}, 
    {62,65,65,65,62}, {127,9,9,9,6}, {62,65,81,33,94}, {127,9,25,41,70}, {70,73,73,73,49}, 
    {1,1,127,1,1}, {63,64,64,64,63}, {31,32,64,32,31}, {63,64,56,64,63}, {99,20,8,20,99}, 
    {7,8,112,8,7}, {97,81,73,69,67} 
};

void ssd1306_write_cmd(ssd1306_t *p, uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(p->i2c_port, p->address, buf, 2, false);
}

void ssd1306_init(ssd1306_t *p, uint16_t width, uint16_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    p->width = width; p->height = height; p->pages = height / 8;
    p->address = address; p->i2c_port = i2c; p->bufsize = p->pages * p->width + 1;
    p->buffer = malloc(p->bufsize);
    p->buffer[0] = 0x40;
    uint8_t cmds[] = {
        SSD1306_SET_DISP | 0x00, SSD1306_SET_MEM_MODE, 0x00, SSD1306_SET_DISP_START_LINE | 0x00, SSD1306_SET_SEG_REMAP | 0x01,
        SSD1306_SET_COM_OUT_DIR | 0x08, SSD1306_SET_COM_PIN_CFG, 0x12, SSD1306_SET_CONTRAST, 0xCF, SSD1306_SET_PRECHARGE, 0xF1,
        SSD1306_SET_VCOM_DESEL, 0x40, SSD1306_SET_DISP_CLK_DIV, 0x80, SSD1306_SET_CHARGE_PUMP, 0x14, SSD1306_SET_DISP | 0x01
    };
    for(int i=0; i<sizeof(cmds); i++) ssd1306_write_cmd(p, cmds[i]);
    ssd1306_fill(p, 0); ssd1306_send_data(p);
}

void ssd1306_fill(ssd1306_t *p, uint8_t val) { memset(p->buffer + 1, val, p->bufsize - 1); }

void ssd1306_draw_char(ssd1306_t *p, char c, uint8_t x, uint8_t y) {
    if (c < 32 || c > 90) c = 32;
    uint16_t index = c - 32;
    for (uint8_t i = 0; i < 5; i++) {
        uint8_t line = font_basic[index][i];
        for (uint8_t j = 0; j < 8; j++) {
            if (line & (1 << j)) p->buffer[1 + x + i + ((y / 8) * p->width)] |= (1 << (y % 8));
        }
    }
}

void ssd1306_draw_string(ssd1306_t *p, const char *str, uint8_t x, uint8_t y) {
    while (*str) { ssd1306_draw_char(p, *str++, x, y); x += 6; if (x + 5 >= p->width) { x = 0; y += 8; } }
}

void ssd1306_send_data(ssd1306_t *p) {
    ssd1306_write_cmd(p, SSD1306_SET_COL_ADDR); ssd1306_write_cmd(p, 0); ssd1306_write_cmd(p, p->width - 1);
    ssd1306_write_cmd(p, SSD1306_SET_PAGE_ADDR); ssd1306_write_cmd(p, 0); ssd1306_write_cmd(p, p->pages - 1);
    i2c_write_blocking(p->i2c_port, p->address, p->buffer, p->bufsize, false);
}