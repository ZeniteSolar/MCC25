#include "adc.h"
#include "usart.h"
#include <util/delay.h>
volatile uint8_t ADC_CHANNEL = 0;
volatile float ADC_CONVERTER = 0;

void init_buffers(void)
{
    CBUF_Init(cbuf_adc0);
    CBUF_Init(cbuf_adc1);
    CBUF_Init(cbuf_adc2);
    
}

uint16_t ma_adc0(void)
{   
    uint16_t sum = 0;
    for(uint16_t i = cbuf_adc0_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc0, i);
    }
    avg_adc0 = sum >> cbuf_adc0_SIZE_2;
    return avg_adc0;
}

uint16_t ma_adc1(void)
{   
    uint16_t sum = 0;
    for(uint16_t i = cbuf_adc1_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc1, i);
    }
    avg_adc1 = sum >> cbuf_adc1_SIZE_2;
    return avg_adc1;
}

uint16_t ma_adc2(void)
{   
    uint16_t sum = 0;
    for(uint16_t i = cbuf_adc2_SIZE; i; i--){
        sum += CBUF_Get(cbuf_adc2, i);
    }
    avg_adc2 = sum >> cbuf_adc2_SIZE_2;
    return avg_adc2;
}

float IpanelToFloat(int adcValue) {
    // Calcula a parte inteira (em volts)
    int inteiro = (adcValue * 5) / 1023;
    // Calcula o resto
    int resto = (adcValue * 5) % 1023;
    // Combina a parte inteira com a parte decimal
    float resultado = inteiro + (float)resto / 1023;

    return resultado;
}

float VpanelToFloat(int adcValue) {
    // Calcula a parte inteira (em volts)
    int inteiro = (adcValue * 21) / 1023;
    // Calcula o resto
    int resto = (adcValue * 21) % 1023;
    // Combina a parte inteira com a parte decimal
    float resultado = inteiro + (float)resto / 1023;

    return resultado;
}

float VbattToFloat(int adcValue) {
    // Calcula a parte inteira (em volts)
    int inteiro = (adcValue * 60) / 1023;
    // Calcula o resto
    int resto = (adcValue * 60) % 1023;
    // Combina a parte inteira com a parte decimal
    float resultado = inteiro + (float)resto / 1023;

    return resultado;
}

void adc_init(void) {
    
    // Configuração do PORTC para pull-up
    PORTC = 0x00;  // Desativa pull-ups
    DDRC = 0x00;   // Configura todos os pinos de PORTC como entrada
    DIDR0 = 0xFF;  // Desabilita os pinos digitais dos ADCs (reduz ruído)
    
        ADMUX   =   (0 << REFS1)                    // AVcc alimentação interna
            | (1 << REFS0)
            | (0 << ADLAR);                         // ADC ordem de posicionamento do registrador

        ADCSRB  =   (0 << ADTS2)                    // Auto-trigger source: timer0 Compare Match A
            | (1 << ADTS1)
            | (1 << ADTS0);
        
        ADMUX = (ADMUX & 0xF8) | 0;                       // Atribuindo canal

        ADCSRA  =   (1 << ADATE)                        // ADC Auto Trigger Enable
                | (1 << ADIE)                           // ADC Interrupt Enable
                | (1 << ADEN)                           // ADC Enable
                | (1 << ADSC)                           // Start na primeira conversão
                | (1 << ADPS2)                          // ADC Prescaller = 128;
                | (1 << ADPS1)
                | (1 << ADPS0);

        // configuracao do Timer TC0 --> TIMER DO ADC
        TCCR0A  =   (1 << WGM01) | (0 << WGM00)         // Timer 0 in Mode 2 = CTC (clear on compare)
                | (0 << COM0A1) | (0 << COM0A0)         // Normal port operation
                | (0 << COM0B1) | (0 << COM0B0);        // do nothing with OC0B
        TCCR0B  =   (0 << WGM02)                        // Timer 0 in Mode 2 = CTC (clear on compare)
                | (0 << FOC0A) | (0 << FOC0B)           // dont force outputs
                | (1 << CS02)                           // clock enabled, prescaller = 1024
                | (0 << CS01)
                | (1 << CS00);

        OCR0A  =    20;                                 // Valor para igualdade de comparacao A para frequencia de ~1500 Hz
        TIMSK0 |=   (1 << OCIE0A);                      // Ativa a interrupcao na igualdade de comparação do TC0 com OCR0A
        
        init_buffers();
        
}

ISR(ADC_vect){    
    switch(ADC_CHANNEL){
        case 0:
            CBUF_Push(cbuf_adc0, ADC); 
            //usart_send_string("\nIpanel:\r");
            //_delay_ms(200);
            ADC_CHANNEL = 1;
            break;
        case 1:
            CBUF_Push(cbuf_adc1, ADC); 
            //usart_send_string("\nVpanel:\r");
            //_delay_ms(200);
            ADC_CHANNEL = 2;
            break;
        case 2:
            CBUF_Push(cbuf_adc2, ADC);
           // usart_send_string("\nVbatt:\r");
            //_delay_ms(200);
            ADC_CHANNEL = 0;
            break;
        default:
            ADC_CHANNEL = 0; // Se algo der errado, reseta para 0
            break;
    }

    // Agora sim, atualiza o canal do ADC no ADMUX
    ADMUX = (ADMUX & 0xF8) | ADC_CHANNEL;
    //ADC_CONVERTER = adcToFloat(ADC);
    //usart_send_float(ADC_CONVERTER, 4);
    //usart_send_string("\r");
}
EMPTY_INTERRUPT(TIMER0_COMPA_vect);