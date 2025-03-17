#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include"conf.h"

#ifdef MACHINE_ON
#include "machine.h"
#endif



// // pwm macros
#define set_pwm_duty_cycle(d)       OCR1A = d      //!< apply duty cycle 'd'
#define set_pwm_off()               OCR1A = 0;     //!< d = 0

#ifndef PWM_DDR
#define PWM_DDR DDRB
#endif
#ifndef PWM
#define PWM PB1
#endif


#ifndef INITIAL_D
#define INITIAL_D      0
#endif

#ifndef PWM_PRESCALE
#define PWM_PRESCALE    1
#endif  
#define PWM_CLK_DIVIDER_VALUE           20 // TODO

// Equations for mode 14 (Fast PWM with TOP in ICR1)
// Note the resolution. For example.. at 100Khz, ICR1 = PWM_TOP = 159, so it has 7.3 bits of resolution.
#define PWM_TOP 159
#define PWM_RESOLUTION log10(PWM_TOP +1)/log10(2)
#define PWM_D_MAX   0.85*PWM_TOP
#define PWM_D_MIN   0.25*PWM_TOP
void pwm_init(void);
void pwm_compute(void);
void pwm_reset(void);
#endif /* ifndef PWM_H */