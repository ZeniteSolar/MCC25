#include "PeO.h"
#ifdef MACHINE_ON
    #include "machine.h"
#endif
#include "usart.h"

volatile float max_power;
volatile float max_power_duty_cycle;
static uint8_t done;
static uint8_t callSweep;
volatile uint8_t count;

void perturb_and_observe(void){
    static uint8_t d_step = PWM_D_STEP;

    // Computes power input
    control.pi[0] = control.v_panel[0] * control.i_panel[0];

    //Derivate power
    float dpi = (control.pi[0]) -(control.pi[1]);
    float dvi = (control.v_panel[0]) -(control.v_panel[1]);
    float dii = (control.i_panel[0]) - (control.i_panel[1]);
    float coef_di = (dii/dvi);
    float coef_def = ((-control.i_panel[0]/control.v_panel[0]));

    if(!callSweep){
        if(dpi > 0){
            usart_send_string("Increasing: ");
            usart_send_float(dpi, 4);
            usart_send_char('\n');
            control.D += d_step;
        }else if (dpi==0){
            control.D = control.D;
        }
        else{
            usart_send_string(" Decreasing: ");
            usart_send_float(dpi, 4);
            usart_send_char('\n');
            control.D -= d_step;
        }
         
        if((control.pi[0] < (max_power*0.6f) || (control.D == 39))){
            usart_send_string("potencia atual: ");
            usart_send_float(control.pi[0], 4);
            usart_send_string(" Maxima potencia: ");
            usart_send_float(max_power, 4);
            usart_send_char('\n');
            max_power = 0;
            callSweep = 1;
            done = 0;
        }
        
    }
    else{
        usart_send_string("SWEEP\n");
        control.sweep_done = 0;
    }


	// recycles
    control.pi[1] = control.pi[0];
    control.v_panel[1] = control.v_panel[0];
    control.i_panel[1] = control.i_panel[0];
    
}
void sweep_duty(void) {
    static uint8_t d_step = 1;
    control.pi[0] = control.v_panel[0] * control.i_panel[0];

    if(control.pi[0] > max_power){
        max_power = control.pi[0];
        max_power_duty_cycle = control.D;
    }
    if(!done){
        control.D +=d_step;
    }
    else{
        if(control.D > max_power_duty_cycle){
            control.D -= d_step;
        }
        else{
            usart_send_string("Maxima potencia\n");
            callSweep = 0;
            control.sweep_done = 1;
            return;
        }
    }
    if(control.D > PWM_D_MAX){
        control.D = PWM_D_MAX;
        done = 1;
    }

    if (control.v_panel[0] <= MINIMUM_PANEL_VOLTAGE_MAX_POWER) 
    {
        control.D = max_power_duty_cycle;
        done = 1;
    }

}