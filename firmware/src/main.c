#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "main.h"

int main(void)
{
    #ifdef USART_ON
    usart_init(UBRR_VALUE,1,1);                         // inicializa a usart
    usart_send_string("\n\n\nUSART... OK!\n");
    #endif

    _delay_ms(1000);

    #ifdef PWM_ON
    usart_send_string("PWM...");
    pwm_init();
    set_EN_driver();
    usart_send_string(" OK!\n");
    #endif

    #ifdef ADC_ON
    usart_send_string("ADC...");
    adc_init();
    usart_send_string(" OK!\n");
    #else
    usart_send_string("ADC... OFF!\n");
    #endif

    #ifdef WATCHDOG_ON
    usart_send_string("WATCHDOG...");
    wdt_init();
    usart_send_string(" OK!\n");
    #else
    usart_send_string("WATCHDOG... OFF!\n");
    #endif

    #ifdef MACHINE_ON
    usart_send_string("MACHINE...");
    machine_init();
    state_machine = STATE_INITIALIZING;
    usart_send_string(" OK!\n");
    #else
    usart_send_string("MACHINE... OFF!\n");
    #endif

    sei();
    while(1){
       // #ifdef WATCHDOG_ON
          //  wdt_reset();
		//#endif

        #ifdef MACHINE_ON
            machine_run();
        #endif

    }
    return 0;
}