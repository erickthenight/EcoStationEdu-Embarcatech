#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// =============================================================
//  CONFIGURAÇÕES DO SISTEMA OPERACIONAL (FreeRTOS)
// =============================================================
// O define mais importante: 0 significa que TEMOS um RTOS
#define NO_SYS                      0

// Proteção crítica para ambientes Multicore (SMP)
#define SYS_LIGHTWEIGHT_PROT        1
#define LWIP_ALLOW_MEM_FREE_FROM_OTHER_CONTEXT 1

// Configuração da Task interna do TCP/IP (Obrigatório quando NO_SYS = 0)
#define TCPIP_THREAD_NAME           "TCP/IP"
#define TCPIP_THREAD_STACKSIZE      1024
#define TCPIP_THREAD_PRIO           3
#define TCPIP_MBOX_SIZE             8
#define DEFAULT_TCP_RECVMBOX_SIZE   8
#define DEFAULT_UDP_RECVMBOX_SIZE   8
#define DEFAULT_RAW_RECVMBOX_SIZE   8
#define DEFAULT_ACCEPTMBOX_SIZE     8

// =============================================================
//  GERENCIAMENTO DE MEMÓRIA (RP2040)
// =============================================================
// Aumentei o MEM_SIZE para evitar "Out of Memory" durante handshake TLS/HTTPS
#define MEM_Alignment               4
#define MEM_SIZE                    16000  // Era 4000. 16KB é mais seguro.
#define MEM_LIBC_MALLOC             0      // Usa o heap do lwIP, não do C padrão

// Pools de buffers (Pausam o tráfego se encherem, mas não crasham)
#define MEMP_NUM_TCP_SEG            32
#define MEMP_NUM_ARP_QUEUE          10
#define PBUF_POOL_SIZE              24

// =============================================================
//  FUNCIONALIDADES DE REDE
// =============================================================
#define LWIP_ARP                    1
#define LWIP_ETHERNET               1
#define LWIP_ICMP                   1
#define LWIP_RAW                    1
#define LWIP_IPV4                   1
#define LWIP_TCP                    1
#define LWIP_UDP                    1
#define LWIP_DNS                    1
#define LWIP_DHCP                   1
#define LWIP_IGMP                   0 // Desativado para economizar, geralmente não usado em IoT simples

// Configurações de Hostname
#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

// =============================================================
//  APIs (SOCKETS)
// =============================================================
// Manter desligado se usar apenas a API raw do SDK (economiza flash)
// Se precisar de sockets BSD no futuro, mude para 1
#define LWIP_SOCKET                 0 
#define LWIP_NETCONN                0

// =============================================================
//  AJUSTES DE PERFORMANCE TCP
// =============================================================
#define TCP_MSS                     1460
#define TCP_WND                     (8 * TCP_MSS)
#define TCP_SND_BUF                 (8 * TCP_MSS)
#define TCP_SND_QUEUELEN            16

// =============================================================
//  INTEGRAÇÃO COM DRIVER PICO W (CYW43)
// =============================================================
// Deixa o hardware/driver decidir o melhor método de checksum
#define LWIP_CHECKSUM_CTRL_PER_NETIF 1
#define ETH_PAD_SIZE                0

// Debug (Opcional - Desligado para produção)
#define LWIP_DEBUG                  0
#define LWIP_STATS                  0

#endif // _LWIPOPTS_H