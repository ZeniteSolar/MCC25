#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef F_CPU
    #define F_CPU 16000000UL  
#endif

#ifndef BAUD
    #define BAUD 9600         
#endif

#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

void usart_init(uint16_t ubrr, uint8_t rx, uint8_t tx);
void usart_send_char(char data);
void usart_send_string(const char *s);
void usart_send_uint16(uint16_t num);
void usart_send_uint32(uint32_t num);
void usart_send_float(float num, uint8_t decimal_places);