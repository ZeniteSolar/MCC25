#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#ifndef PWM_DDR
#define PWM_DDR DDRB
#endif
#ifndef PWM
#define PWM PB1
#endif

#ifndef INITIAL_D
#define INITIAL_D       100
#endif

#ifndef PWM_PRESCALE
#define PWM_PRESCALE    1
#endif

#define set_bit(reg, bit) ((reg) |= (1 << (bit))) // Declaração
#define clr_bit(REG, BIT) (REG &= ~(1 << BIT))  // Limpa o bit

// Equations for mode 14 (Fast PWM with TOP in ICR1)
// Note the resolution. For example.. at 100Khz, ICR1 = PWM_TOP = 159, so it has 7.3 bits of resolution.
#define PWM_TOP 159
#define PWM_RESOLUTION log10(PWM_TOP +1)/log10(2)

void pwm_init(void);

#endif /* ifndef PWM_H */