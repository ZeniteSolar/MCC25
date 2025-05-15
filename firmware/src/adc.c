#include "adc.h"

#ifdef USART_ON
    #include "usart.h"
#endif

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

float vpanel_to_float(uint16_t vpanel_adc)
{
  return 0.020717284 * (float)vpanel_adc - 0.057896889;
}
float vbat_to_float(uint16_t vbat_adc)
{
    return 0.0664796828951478 * (float)vbat_adc - 0.156185640842708;
}

float ipanel_to_float(uint16_t ipanel_adc)
{
    return 0.032923627 * (float)ipanel_adc - 16.62932491;
}

void adc_init(void) {
    adc_data_ready = 0;
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
                | (0 << CS02)                           // clock enabled, prescaller = 64
                | (1 << CS01)
                | (0 << CS00);

        OCR0A  =    199;                                 // Valor para igualdade de comparacao A para frequencia de ~5000 Hz
        TIMSK0 |=   (1 << OCIE0A);                      // Ativa a interrupcao na igualdade de comparação do TC0 com OCR0A

        init_buffers();
       
}

ISR(ADC_vect){    
    switch(ADC_CHANNEL){
        case 0:
            CBUF_Push(cbuf_adc0, ADC); 
            ADC_CHANNEL = 1;
            break;
        case 1:
            CBUF_Push(cbuf_adc1, ADC); 
            ADC_CHANNEL = 2;
            break;
        case 2:
            CBUF_Push(cbuf_adc2, ADC);
            adc_data_ready = 1;
            ADC_CHANNEL = 0;
            break;
        default:
            adc_data_ready = 1;
            ADC_CHANNEL = 0; 
            break;
    }

    
    ADMUX = (ADMUX & 0xF8) | ADC_CHANNEL;

}
EMPTY_INTERRUPT(TIMER0_COMPA_vect);