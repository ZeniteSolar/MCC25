#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "adc.h"
#include "usart.h"
#include "pwm.h"

#define set_bit(reg, bit) ((reg) |= (1 << (bit))) // Declaração
volatile uint16_t control_VP = 0;
volatile uint16_t control_IP = 0;
volatile uint16_t control_VB = 0;
volatile uint16_t potenciaInit = 0;
volatile uint16_t potenciaGeral = 0;
volatile uint16_t control_D = 100;
int main(void)
{
    sei();

    usart_init(UBRR_VALUE,1,1);                         // inicializa a usart
    usart_send_string("\n\n\nUSART... OK!\n\n\n\r");
    
    pwm_init();
    usart_send_string("\n\n\nTimer1... OK!\n\n\n\r");

    OCR1A = control_D;


    set_bit(DDRB,PB3);
    set_bit(PORTB,PB3);
        
    set_bit(DDRC,PC5);
    set_bit(PORTC,PC5);

    usart_send_string("\n\n\nADC... OK!\n\n\n\r");
    adc_init();                                         // inicializa ADC
    
    
    while(1){ 
        control_IP = ma_adc0();
        control_VP = ma_adc1();
        control_VB = ma_adc2();

        usart_send_string("Ipanel: ");
        usart_send_float(IpanelToFloat(control_IP),4);
        _delay_ms(200);

        usart_send_string(" Vpanel: ");
        usart_send_float(VpanelToFloat(control_VP),4);
        _delay_ms(200);

        usart_send_string(" Vbatt: ");
        usart_send_float(VbattToFloat(control_VB),4);
        _delay_ms(200);

        usart_send_string("\n");
        

    }

    return 0;
}