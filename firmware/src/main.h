#ifndef MAIN_H
#define MAIN_H 

#ifndef F_CPU
#define F_CPU 16000000UL    ///< Frequency of CPU
#endif /* ifndef F_CPU */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// MODULOS DO SISTEMA
#include "conf.h"

#ifdef USART_ON
#include "usart.h"
#pragma message "USART: ON!"
#else
#pragma message "USART: OFF!"
#endif /*ifdef USART_ON*/

#ifdef ADC_ON 
#include "adc.h"
#pragma message "ADC: ON!"
#else
#pragma message "ADC: OFF!"
#endif  /*ifdef ADC_ON*/

#ifdef MACHINE_ON
#include "machine.h"
#pragma message "MACHINE: ON!"
#else
#pragma message "MACHINE: OFF!"
#endif /*ifdef MACHINE_ON*/

#ifdef PWM_ON
#include "pwm.h"
#pragma message "PWM: ON!"
#else
#pragma message "PWM: OFF!"
#endif /*ifdef PWM_ON*/


#ifdef WATCHDOG_ON
#include "watchdog.h"
#pragma message "WATCHDOG: ON!"
#else
#pragma message "WATCHDOG: OFF!"
#endif /*ifdef WATCHDOG_ON*/

#endif /* ifndef MAIN_H */