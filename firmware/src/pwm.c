#include "pwm.h"
#include "PeO.h"
#include "usart.h"

void pwm_init(void)
{
    clr_bit(PRR, PRTIM1);                          // Activates clock
    
    // MODE 14 -> Fast PWM with TOP on ICR1
    TCCR1A  =    (1 << WGM11) | (0 << WGM10)        // mode 14
            | (0 << COM1B1) | (0 << COM1B0)         // do nothing
            | (1 << COM1A1) | (0 << COM1A0);        // pwm
    
        TCCR1B =
            #if PWM_PRESCALE ==     1
                        (0 << CS12) | (0 << CS11) | (1 << CS10) // Prescaler N=1
            #elif PWM_PRESCALE ==   8
                        (0 << CS12) | (0 << CS11) | (1 << CS10) // Prescaler N=8
            #elif PWM_PRESCALE ==   64
                        (0 << CS12) | (1 << CS11) | (0 << CS10) // Prescaler N=64
            #elif PWM_PRESCALE ==   256
                        (0 << CS12) | (1 << CS11) | (1 << CS10) // Prescaler N=256
            #elif PWM_PRESCALE ==   1024
                        (1 << CS12) | (0 << CS11) | (1 << CS10) // Prescaler N=1024
            #else 
                        0
            #endif
            | (1 << WGM13) | (1 << WGM12);          // mode 14
    
    ICR1 = PWM_TOP;                                 // ICR1 = TOP = fcpu/(N*f) -1
    OCR1A = INITIAL_D;                              // set initial Duty Cycle
    set_bit(PWM_DDR, PWM);                          // PWM as output

}
void pwm_reset(void)
{
    set_pwm_off();
#ifdef MACHINE_ON
    control.D = 0;
#endif
    usart_send_string("PWM turned off!\n");
}
void pwm_compute(void){
#ifdef MACHINE_ON
    
    if(!control.sweep_done){
        sweep_duty();
    }else{
        perturb_and_observe();
    }
    
   // perturb_and_observe();

    // apply some threshhold saturation limits
    if(control.D > PWM_D_MAX)        control.D = PWM_D_MAX;
    else if(control.D < PWM_D_MIN)   control.D = PWM_D_MIN;



    // apply dutycycle
    OCR1A = control.D;

 

    static uint8_t pwm_clk_divider = 0;
    if(pwm_clk_divider++ == PWM_CLK_DIVIDER_VALUE){
        pwm_clk_divider = 0;
        usart_send_string("PWM computed as: ");
        usart_send_uint16((control.D));
        //usart_send_char('\n');

        usart_send_string(" Vpanel: ");
        usart_send_float(control.v_panel[0], 4);
        usart_send_string(" Ipanel: ");
        usart_send_float(control.i_panel[0],4);
        usart_send_string(" Vbatt: ");
        usart_send_uint16(ma_adc2());
        usart_send_char('\n');
    }
#endif
    
}