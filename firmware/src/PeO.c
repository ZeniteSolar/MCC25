#include "PeO.h"
#ifdef MACHINE_ON
    #include "machine.h"
#endif
#include "usart.h"

#define PERTURB_AND_OBSERVE_STEP 1.0f
#define PERTURB_AND_OBSERVE_INITIAL_DIRECTION 1.0f
/* This avoid to stop in a maximum local because the direction will only change 
when the step reduce the power in a significant way */
#define PERTURB_AND_OBSERVE_DERIVATIVE_THRESHOLD -0.1f

// Global variable definitions
state_PeO_t state_PeO;
volatile float max_power;
volatile float max_power_duty_cycle;

static uint8_t done;
static uint8_t callSweep;

void perturb_and_observe(void){
    static float step = PERTURB_AND_OBSERVE_STEP;
    static float direction = PERTURB_AND_OBSERVE_INITIAL_DIRECTION;

    // Computes power input
    control.pi[0] = control.v_panel[0] * control.i_panel[0];

    //Derivate power
    float dpi = (control.pi[0]) -(control.pi[1]);

    if(dpi <= PERTURB_AND_OBSERVE_DERIVATIVE_THRESHOLD){
        step = PERTURB_AND_OBSERVE_STEP;
        direction = -direction;
    }

    step += 0.3f;
    if (step > 3.0f)
    {
        step = 3.0f;
    }

    control.D = (uint16_t)(control.D + direction * step);

    /* Save values for next iteration */
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

    // limite superior
    if(control.D > PWM_D_MAX){
        control.D = PWM_D_MAX;
        done = 1;
    }

    // tensão minima do painel
    if (control.v_panel[0] <= MINIMUM_PANEL_VOLTAGE_MAX_POWER) 
    {
        control.D = max_power_duty_cycle;
        done = 1;
    }

}