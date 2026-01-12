#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"     // Necessário para achar o ThingSpeak
#include "hardware/i2c.h"
#include "hardware/adc.h"

// --- SUAS CONFIGURAÇÕES (EDITAR AQUI) ---
#define WIFI_SSID "Apto203b"
#define WIFI_PASS "29062009bi"
#define THINGSPEAK_API_KEY "WPXY9J0WYGJOG11K" // <--- COLE SUA CHAVE AQUI
// ----------------------------------------

#define LED_PIN 12 
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_OLED 0x3C

// --- DRIVERS OLED (Mantidos) ---
static const uint8_t font[] = { 0x00,0x00,0x00,0x00,0x00, 0x00,0x00,0x5F,0x00,0x00, 0x00,0x07,0x00,0x07,0x00, 0x14,0x7F,0x14,0x7F,0x14, 0x24,0x2A,0x7F,0x2A,0x12, 0x23,0x13,0x08,0x64,0x62, 0x36,0x49,0x56,0x20,0x50, 0x00,0x08,0x07,0x03,0x00, 0x00,0x1C,0x22,0x41,0x00, 0x00,0x41,0x22,0x1C,0x00, 0x2A,0x1C,0x7F,0x1C,0x2A, 0x08,0x08,0x3E,0x08,0x08, 0x00,0x00,0x50,0x30,0x00, 0x08,0x08,0x08,0x08,0x08, 0x00,0x00,0x60,0x60,0x00, 0x20,0x10,0x08,0x04,0x02, 0x3E,0x51,0x49,0x45,0x3E, 0x00,0x42,0x7F,0x40,0x00, 0x72,0x49,0x49,0x49,0x46, 0x21,0x41,0x49,0x4D,0x33, 0x18,0x14,0x12,0x7F,0x10, 0x27,0x45,0x45,0x45,0x39, 0x3C,0x4A,0x49,0x49,0x31, 0x41,0x21,0x11,0x09,0x07, 0x36,0x49,0x49,0x49,0x36, 0x46,0x49,0x49,0x29,0x1E, 0x00,0x00,0x36,0x36,0x00, 0x00,0x00,0x56,0x36,0x00, 0x00,0x08,0x14,0x22,0x41, 0x14,0x14,0x14,0x14,0x14, 0x41,0x22,0x14,0x08,0x00, 0x02,0x01,0x51,0x09,0x06, 0x32,0x49,0x79,0x41,0x3E, 0x7E,0x11,0x11,0x11,0x7E, 0x7F,0x49,0x49,0x49,0x36, 0x3E,0x41,0x41,0x41,0x22, 0x7F,0x41,0x41,0x22,0x1C, 0x7F,0x49,0x49,0x49,0x41, 0x7F,0x09,0x09,0x01,0x01, 0x3E,0x41,0x41,0x51,0x32, 0x7F,0x08,0x08,0x08,0x7F, 0x00,0x41,0x7F,0x41,0x00, 0x20,0x40,0x41,0x3F,0x01, 0x7F,0x08,0x14,0x22,0x41, 0x7F,0x40,0x40,0x40,0x40, 0x7F,0x02,0x04,0x02,0x7F, 0x7F,0x04,0x08,0x10,0x7F, 0x3E,0x41,0x41,0x41,0x3E, 0x7F,0x09,0x09,0x09,0x06, 0x3E,0x41,0x51,0x21,0x5E, 0x7F,0x09,0x19,0x29,0x46, 0x46,0x49,0x49,0x49,0x31, 0x01,0x01,0x7F,0x01,0x01, 0x3F,0x40,0x40,0x40,0x3F, 0x1F,0x20,0x40,0x20,0x1F, 0x7F,0x20,0x18,0x20,0x7F, 0x63,0x14,0x08,0x14,0x63, 0x03,0x04,0x78,0x04,0x03, 0x61,0x51,0x49,0x45,0x43, 0x00,0x00,0x7F,0x41,0x00, 0x02,0x04,0x08,0x10,0x20, 0x00,0x41,0x7F,0x00,0x00, 0x04,0x02,0x01,0x02,0x04, 0x40,0x40,0x40,0x40,0x40, 0x00,0x01,0x02,0x04,0x00, 0x20,0x54,0x54,0x54,0x78, 0x7F,0x48,0x44,0x44,0x38, 0x38,0x44,0x44,0x44,0x20, 0x38,0x44,0x44,0x48,0x7F, 0x38,0x54,0x54,0x54,0x18, 0x08,0x7E,0x09,0x01,0x02, 0x08,0x14,0x54,0x54,0x3C, 0x7F,0x08,0x04,0x04,0x78, 0x00,0x44,0x7D,0x40,0x00, 0x20,0x40,0x44,0x3D,0x00, 0x00,0x7F,0x10,0x28,0x44, 0x00,0x41,0x7F,0x40,0x00, 0x7C,0x04,0x18,0x04,0x78, 0x7C,0x08,0x04,0x04,0x78, 0x38,0x44,0x44,0x44,0x38, 0x7C,0x14,0x14,0x14,0x08, 0x08,0x14,0x14,0x18,0x7C, 0x7C,0x08,0x04,0x04,0x08, 0x48,0x54,0x54,0x54,0x20, 0x04,0x3F,0x44,0x40,0x20, 0x3C,0x40,0x40,0x20,0x7C, 0x1C,0x20,0x40,0x20,0x1C, 0x3C,0x40,0x30,0x40,0x3C, 0x44,0x28,0x10,0x28,0x44, 0x0C,0x50,0x50,0x50,0x3C, 0x44,0x64,0x54,0x4C,0x44, 0x00,0x08,0x36,0x41,0x00, 0x00,0x00,0x7F,0x00,0x00, 0x00,0x41,0x36,0x08,0x00, 0x08,0x08,0x2A,0x1C,0x08 };
static uint8_t buffer[8][128];
bool led_state = false;
static char site_buffer[2048];

void oled_command(uint8_t command) { uint8_t data[2] = {0x00, command}; i2c_write_blocking(I2C_PORT, ENDERECO_OLED, data, 2, false); }
void oled_update() { for (uint8_t page = 0; page < 8; page++) { oled_command(0xB0 + page); oled_command(0x00); oled_command(0x10); uint8_t payload[129]; payload[0] = 0x40; for (int i = 0; i < 128; i++) payload[1 + i] = buffer[page][i]; i2c_write_blocking(I2C_PORT, ENDERECO_OLED, payload, 129, false); } }
void oled_clear() { memset(buffer, 0, sizeof(buffer)); }
void draw_text(const char *str, uint8_t x, uint8_t line) { while (*str) { uint8_t c = *str; if (c < 32 || c > 126) c = 32; int idx = (c - 32) * 5; for (int i = 0; i < 5; i++) { if (x < 128) buffer[line][x] = font[idx + i]; x++; } x += 2; str++; } }
void oled_init_seguro() { oled_command(0xAE); oled_command(0xD5); oled_command(0x80); oled_command(0xA8); oled_command(0x3F); oled_command(0xD3); oled_command(0x00); oled_command(0x40); oled_command(0x8D); oled_command(0x14); oled_command(0x20); oled_command(0x02); oled_command(0xA1); oled_command(0xC8); oled_command(0xDA); oled_command(0x12); oled_command(0x81); oled_command(0xCF); oled_command(0xD9); oled_command(0xF1); oled_command(0xDB); oled_command(0x40); oled_command(0xA4); oled_command(0xA6); oled_command(0xAF); }

float read_temp() { adc_select_input(4); return 27.0f - (adc_read() * 3.3f / 4095.0f - 0.706f) / 0.001721f; }

// --- 1. SERVIDOR WEB LOCAL (MANTIDO) ---
err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (!p) { tcp_close(tpcb); return ERR_OK; }
    tcp_recved(tpcb, p->tot_len);
    char *req = (char *)p->payload;
    if (strstr(req, "GET /favicon.ico")) { tcp_write(tpcb, "HTTP/1.1 404\r\n\r\n", 14, 0); tcp_close(tpcb); pbuf_free(p); return ERR_OK; }
    if (strstr(req, "GET /alert/on")) { led_state = true; gpio_put(LED_PIN, 1); }
    else if (strstr(req, "GET /alert/off")) { led_state = false; gpio_put(LED_PIN, 0); }
    float t = read_temp();
    snprintf(site_buffer, sizeof(site_buffer),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"
        "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1'><title>EcoStation</title><style>"
        "body{font-family:Arial,sans-serif;background:#f0f2f5;text-align:center;padding:20px}.card{background:#fff;border-radius:20px;padding:30px;box-shadow:0 10px 25px rgba(0,0,0,0.1);max-width:320px;margin:auto}.temp{font-size:4em;font-weight:bold;color:#333}.status{padding:8px 15px;border-radius:50px;display:inline-block;font-weight:bold}.safe{background:#d4edda;color:#155724}.danger{background:#f8d7da;color:#721c24}.btn{display:block;width:100%%;padding:15px;margin-top:25px;border:none;border-radius:12px;color:white;text-decoration:none}.btn-alert{background:#dc3545}.btn-norm{background:#28a745}</style></head><body>"
        "<div class='card'><h3>MANAUS - AM</h3><div class='temp'>%.1f&deg;C</div><div class='status %s'>%s</div><a href='/alert/%s' class='btn %s'>%s</a></div></body></html>",
        t, led_state?"danger":"safe", led_state?"FUMA&Ccedil;A":"NORMAL", led_state?"off":"on", led_state?"btn-norm":"btn-alert", led_state?"NORMALIZAR":"SIMULAR ALERTA");
    tcp_write(tpcb, site_buffer, strlen(site_buffer), TCP_WRITE_FLAG_COPY); tcp_close(tpcb); pbuf_free(p); return ERR_OK;
}
err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) { tcp_recv(newpcb, http_callback); return ERR_OK; }
void start_server() { struct tcp_pcb *pcb = tcp_new(); tcp_bind(pcb, IP_ADDR_ANY, 80); pcb = tcp_listen(pcb); tcp_accept(pcb, connection_callback); }

// --- 2. CLIENTE CLOUD (THINGSPEAK) ---
err_t cloud_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len) { tcp_close(tpcb); return ERR_OK; }
err_t cloud_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) { tcp_close(tpcb); return err; }
    char request[256];
    float t = read_temp();
    // Monta o pacote para o ThingSpeak
    snprintf(request, sizeof(request),
        "GET /update?api_key=%s&field1=%.2f&field2=%d HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n",
        THINGSPEAK_API_KEY, t, led_state ? 1 : 0);
    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_sent(tpcb, cloud_sent_callback); // Fecha depois de enviar
    return ERR_OK;
}
void dns_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr) {
        struct tcp_pcb *pcb = tcp_new();
        tcp_connect(pcb, ipaddr, 80, cloud_connected_callback);
    }
}
void send_to_cloud() {
    ip_addr_t ip;
    // Tenta achar o IP do ThingSpeak
    err_t err = dns_gethostbyname("api.thingspeak.com", &ip, dns_callback, NULL);
    if (err == ERR_OK) {
        struct tcp_pcb *pcb = tcp_new();
        tcp_connect(pcb, &ip, 80, cloud_connected_callback);
    }
}

int main() {
    stdio_init_all(); adc_init(); adc_set_temp_sensor_enabled(true);
    gpio_init(LED_PIN); gpio_set_dir(LED_PIN, GPIO_OUT);
    i2c_init(I2C_PORT, 400000); gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); gpio_pull_up(I2C_SDA); gpio_pull_up(I2C_SCL);
    oled_init_seguro(); oled_clear();

    if (cyw43_arch_init_with_country(CYW43_COUNTRY_BRAZIL)) return 1;
    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM); // Força alta performance

    oled_clear(); draw_text("WIFI...", 40, 3); oled_update();
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_MIXED_PSK, 30000)) {
        oled_clear(); draw_text("FALHA REDE", 20, 3); oled_update(); while(1);
    }

    int wait = 0;
    while(cyw43_state.netif[0].ip_addr.addr == 0 && wait < 30) {
        oled_clear(); draw_text("IP...", 40, 3); oled_update(); sleep_ms(1000);
    }

    start_server(); 

    uint32_t last_cloud_send = 0;

    while (true) {
        oled_clear();
        draw_text("ECO STATION", 20, 0);
        uint32_t ip = cyw43_state.netif[0].ip_addr.addr;
        char buf[32];
        if (ip == 0) sprintf(buf, "IP: 0.0.0.0");
        else sprintf(buf, "IP: %d.%d.%d.%d", ip & 0xFF, (ip>>8)&0xFF, (ip>>16)&0xFF, ip>>24);
        draw_text(buf, 0, 6);
        
        if(led_state) draw_text("! ALERTA !", 25, 3);
        else { sprintf(buf, "%.1f C", read_temp()); draw_text(buf, 40, 3); }
        
        // --- LÓGICA DE ENVIO NUVEM (A CADA 30 SEGUNDOS) ---
        uint32_t now = to_ms_since_boot(get_absolute_time());
        if (now - last_cloud_send > 30000) { // 30000ms = 30s
            draw_text("NUVEM...", 40, 2); // Avisa na tela que tá enviando
            oled_update();
            send_to_cloud();
            last_cloud_send = now;
            sleep_ms(1000); // Pausa visual
        } else {
            oled_update();
        }
        
        sleep_ms(200);
    }
}