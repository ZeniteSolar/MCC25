#ifndef PeO_H_
#define PeO_H_

#include"conf.h"
#include "pwm.h"
#define PWM_D_STEP 					        1

#define MINIMUM_PANEL_VOLTAGE_MAX_POWER  9.0f
extern volatile float max_power;
extern volatile float max_power_duty_cycle;

typedef enum state_PeO{
    increase,
    decrement,
} state_PeO_t;
state_PeO_t state_PeO;

void perturb_and_observe(void);
void sweep_duty(void);
#endif /* PeO_H_ */