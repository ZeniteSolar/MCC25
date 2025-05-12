// MODULES ACTIVATION
#define USART_ON
//#define CAN_ON
//#define CAN_DEPENDENT
#define ADC_ON
#define MACHINE_ON
#define PWM_ON
#define WATCHDOG_ON

#define set_bit(reg, bit) ((reg) |= (1 << (bit))) // Declaração
#define clr_bit(REG, BIT) (REG &= ~(1 << BIT))  // Limpa o bit