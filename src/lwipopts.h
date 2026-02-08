#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// ---------------------------------------------------------------------------------------
// CONFIGURAÇÕES BÁSICAS E CORREÇÕES (PICO W + FreeRTOS)
// ---------------------------------------------------------------------------------------
// Resolve o erro de redefinição de 'struct timeval'
#define LWIP_TIMEVAL_PRIVATE        0 

// NO_SYS = 0 indica que estamos usando um Sistema Operacional (FreeRTOS)
#define NO_SYS                      0
#define LWIP_SOCKET                 1
#define LWIP_NETCONN                1

// ---------------------------------------------------------------------------------------
// FUNCIONALIDADES DE REDE
// ---------------------------------------------------------------------------------------
#define LWIP_IGMP                   1
#define LWIP_ICMP                   1
#define LWIP_DNS                    1
#define LWIP_UDP                    1
#define LWIP_TCP                    1

// Resolve o aviso 'implicit declaration of netif_set_hostname' do seu log
#define LWIP_NETIF_HOSTNAME         1
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

// ---------------------------------------------------------------------------------------
// GERENCIAMENTO DE MEMÓRIA (Otimizado para a EcoStation)
// ---------------------------------------------------------------------------------------
#define MEM_SIZE                    16000
#define MEMP_NUM_TCP_SEG            32
#define PBUF_POOL_SIZE              24

// ---------------------------------------------------------------------------------------
// CONFIGURAÇÕES DE THREADS (Sincronia com FreeRTOS)
// ---------------------------------------------------------------------------------------
#define TCPIP_MBOX_SIZE             8
#define TCPIP_THREAD_STACKSIZE      1024
#define TCPIP_THREAD_PRIO           3
#define DEFAULT_THREAD_STACKSIZE    1024
#define DEFAULT_RAW_MBOX_SIZE       8
#define SYS_LIGHTWEIGHT_PROT        1

#endif