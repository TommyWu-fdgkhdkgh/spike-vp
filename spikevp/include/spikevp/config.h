#ifndef SPIKE_VP_CONFIG_H
#define SPIKE_VP_CONFIG_H

#define SPIKEVP_VERSION_STRING  "1"
#define SPIKEVP_CPU_DEFCLK (100 * vcml::MHz)

//#define SPIKEVP_RESETVEC (0x80000000)
#define SPIKEVP_RESETVEC (0x23000004)
#define SPIKEVP_FDT_ADDR (0xbfe00000)

/* Interrupt causes */
#define SPIKEVP_IRQ_U_SOFT                         0
#define SPIKEVP_IRQ_S_SOFT                         1
#define SPIKEVP_IRQ_VS_SOFT                        2
#define SPIKEVP_IRQ_M_SOFT                         3
#define SPIKEVP_IRQ_U_TIMER                        4
#define SPIKEVP_IRQ_S_TIMER                        5
#define SPIKEVP_IRQ_VS_TIMER                       6
#define SPIKEVP_IRQ_M_TIMER                        7
#define SPIKEVP_IRQ_U_EXT                          8
#define SPIKEVP_IRQ_S_EXT                          9
#define SPIKEVP_IRQ_VS_EXT                         10
#define SPIKEVP_IRQ_M_EXT                          11

/* IRQ num for interrupt controller */
#define SPIKEVP_UART_IRQ                           1

/* Memory map */
#define SPIKEVP_MEM_ADDR (0x80000000)
#define SPIKEVP_MEM_SIZE (0x1000000000) // 4 GB
#define SPIKEVP_MEM_END  (SPIKEVP_MEM_ADDR + SPIKEVP_MEM_SIZE - 1)

#define SPIKEVP_MROM_ADDR (0x23000000)
#define SPIKEVP_MROM_SIZE (0x4000)
#define SPIKEVP_MROM_END (SPIKEVP_MROM_ADDR + SPIKEVP_MROM_SIZE - 1)

#define SPIKEVP_PLIC_ADDR (0xc000000)
#define SPIKEVP_PLIC_SIZE (0x6000000)
#define SPIKEVP_PLIC_END (SPIKEVP_PLIC_ADDR + SPIKEVP_PLIC_SIZE - 1)

#define SPIKEVP_CLINT_ADDR (0x2000000)
#define SPIKEVP_CLINT_SIZE (0x10000)
#define SPIKEVP_CLINT_END (SPIKEVP_CLINT_ADDR + SPIKEVP_CLINT_SIZE - 1)

#define SPIKEVP_UART_ADDR (0x20000000)
#define SPIKEVP_UART_SIZE (0x1000)
#define SPIKEVP_UART_END (SPIKEVP_UART_ADDR + SPIKEVP_UART_SIZE - 1)

#endif
