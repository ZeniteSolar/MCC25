#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef _ADC_H_
#define _ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include"conf.h"
#include "../lib/cbuf.h"

#ifndef F_CPU
    #define F_CPU 16000000UL  
#endif

uint8_t adc_data_ready;
/* 
typedef volatile enum adc_channels{ 
    ADC0, ADC1 ,ADC2, ADC3, ADC4, ADC5  
} adc_channels_t;                           < the adc_channel type 
static adc_channels_t ADC_CHANNEL = ADC0;   < current chosen adc channel
*/

//uint8_t adc_select_channel(ADC_CHANNEL);

void adc_init(void);

void init_buffers(void);

float IpanelToFloat(int adcValue);

float VpanelToFloat(int adcValue);

float VbattToFloat(int adcValue);

#define cbuf_adc0_SIZE         32        // size of buffers
#define cbuf_adc0_SIZE_2        5         // size of buffers in 2^n
volatile struct cbuf_adc0{
    uint16_t     m_getIdx;
    uint16_t     m_putIdx;
    uint16_t     m_entry[cbuf_adc0_SIZE];
} cbuf_adc0;
uint16_t ma_adc0(void);
uint16_t avg_adc0;

#define cbuf_adc1_SIZE          32       // size of buffers
#define cbuf_adc1_SIZE_2        5         // size of buffers in 2^n
volatile struct cbuf_adc1{
    uint16_t     m_getIdx;
    uint16_t     m_putIdx;
    uint16_t     m_entry[cbuf_adc1_SIZE];
} cbuf_adc1; 
uint16_t ma_adc1(void);
uint16_t avg_adc1;

#define cbuf_adc2_SIZE          32        // size of buffers
#define cbuf_adc2_SIZE_2        5        // size of buffers in 2^n
volatile struct cbuf_adc2{
    uint16_t     m_getIdx;
    uint16_t     m_putIdx;
    uint16_t     m_entry[cbuf_adc2_SIZE];
} cbuf_adc2;  
uint16_t ma_adc2(void);
uint16_t avg_adc2;

float vpanel_to_float(uint16_t vpanel_adc);
float ipanel_to_float(uint16_t ipanel_adc);
float vbat_to_float(uint16_t vbat_adc);

#endif /* ifndef _ADC_H_ */