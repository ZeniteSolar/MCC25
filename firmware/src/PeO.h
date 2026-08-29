#ifndef PeO_H_
#define PeO_H_

#include "conf.h"
#include "pwm.h"

// ─── Configuração do Perturb & Observe ─────────────────────────────
// Step fixo em unidades de OCR1A (inteiro).
// Com PWM_TOP=159, cada unidade = 0.625% de duty.
// Step=1 dá a menor perturbação possível (~0.625%).
// Step=2 dá ~1.25% — mais rápido para convergir, mais oscilação.
#define PEO_STEP                    1

// Tensão mínima do painel para considerar válido durante sweep
#define MINIMUM_PANEL_VOLTAGE_MAX_POWER  9.0f

// ─── Variáveis externas do sweep ───────────────────────────────────
extern volatile float max_power;
extern volatile float max_power_duty_cycle;

// ─── Funções ───────────────────────────────────────────────────────
void perturb_and_observe(void);
void sweep_duty(void);

#endif /* PeO_H_ */