#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// --- CONFIGURAÇÃO "WEATHER LITE" (ESTÁVEL) ---

#define NO_SYS                      1
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0
#define LWIP_IGMP                   0
#define LWIP_ICMP                   1 

// Memória ajustada para pacotes segmentados
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    6000  // 6KB é suficiente e seguro
#define MEMP_NUM_TCP_SEG            32
#define MEMP_NUM_ARP_QUEUE          10
#define PBUF_POOL_SIZE              32    // Buffer médio

#define LWIP_ARP                    1
#define LWIP_ETHERNET               1
#define LWIP_IPV4                   1
#define LWIP_TCP                    1
#define LWIP_UDP                    1
#define LWIP_DNS                    1
#define LWIP_RAW                    1

#define LWIP_NETIF_HOSTNAME         1
#define LWIP_DHCP                   1
#define LWIP_HTTPD_MAX_TAG_NAME_LEN 20

// Tamanho máximo do segmento TCP (Evita fragmentação)
#define TCP_MSS                     1460
#define TCP_WND                     (2 * TCP_MSS)

#define CHECKSUM_GEN_UDP            1
#define CHECKSUM_GEN_TCP            1
#define CHECKSUM_CHECK_IP           1
#define CHECKSUM_CHECK_UDP          1
#define CHECKSUM_CHECK_TCP          1

#define MEM_LIBC_MALLOC             0
#define MEMP_MEM_MALLOC             0

#endif