#include "PeO.h"
#ifdef MACHINE_ON
    #include "machine.h"
#endif
#include "usart.h"

// ─── Variáveis globais ─────────────────────────────────────────────
volatile float max_power;
volatile float max_power_duty_cycle;

// ─── Variáveis internas do sweep ───────────────────────────────────
static uint8_t sweep_finished_ascending;

// ═══════════════════════════════════════════════════════════════════
// Perturb & Observe — Algoritmo clássico com step fixo
// ═══════════════════════════════════════════════════════════════════
//
// Lógica:
//   1. Calcula a potência atual P[0] = V_panel × I_panel
//   2. Compara com a potência da iteração anterior P[-1]
//   3. Se P caiu (dP < 0) → inverte a direção da perturbação
//      Se P subiu ou ficou igual (dP >= 0) → mantém a direção
//   4. Aplica D = D + direction × PEO_STEP
//   5. Satura D nos limites [PWM_D_MIN, PWM_D_MAX]
//
// Isso garante:
//   - Step constante (sem acúmulo → sem oscilação crescente)
//   - Convergência suave para o MPP
//   - Comportamento previsível e depurável
//
void perturb_and_observe(void) {
    static int8_t direction = 1;

    // 1. Calcula potência de entrada atual
    control.pi[0] = control.v_panel[0] * control.i_panel[0];

    // 2. Derivada de potência
    float dpi = control.pi[0] - control.pi[1];

    // 3. P&O clássico: se potência caiu, inverte direção
    if (dpi < 0.0f) {
        direction = -direction;
    }
    // Se dpi >= 0, mantém a direção atual (está melhorando ou estável)

    // 4. Aplica perturbação com step fixo inteiro, já com clamp
    int16_t new_D = (int16_t)control.D + (int16_t)(direction * PEO_STEP);

    if (new_D > (int16_t)PWM_D_MAX)  new_D = (int16_t)PWM_D_MAX;
    if (new_D < (int16_t)PWM_D_MIN)  new_D = (int16_t)PWM_D_MIN;

    control.D = (uint16_t)new_D;

    // 5. Salva valores para próxima iteração
    control.pi[1]      = control.pi[0];
    control.v_panel[1]  = control.v_panel[0];
    control.i_panel[1]  = control.i_panel[0];
}

// ═══════════════════════════════════════════════════════════════════
// Sweep Inicial — Varredura para encontrar ponto de máxima potência
// ═══════════════════════════════════════════════════════════════════
//
// Chamado repetidamente por pwm_compute() até sweep_done = 1.
// Varre D de PWM_D_MIN até PWM_D_MAX (ou até a tensão do painel
// cair demais), registra o D que deu mais potência, e depois
// retorna para esse ponto.
//
void sweep_duty(void) {
    control.pi[0] = control.v_panel[0] * control.i_panel[0];

    // Registra máximo encontrado
    if (control.pi[0] > max_power) {
        max_power = control.pi[0];
        max_power_duty_cycle = control.D;
    }

    if (!sweep_finished_ascending) {
        // Fase 1: subindo o duty
        control.D += 1;

        // Atingiu limite superior → começa a descer
        if (control.D > (uint16_t)PWM_D_MAX) {
            control.D = (uint16_t)PWM_D_MAX;
            sweep_finished_ascending = 1;
        }

        // Tensão do painel caiu demais → para e volta
        if (control.v_panel[0] <= MINIMUM_PANEL_VOLTAGE_MAX_POWER) {
            control.D = (uint16_t)max_power_duty_cycle;
            sweep_finished_ascending = 1;
        }
    } else {
        // Fase 2: descendo até o ponto de máxima potência
        if (control.D > (uint16_t)max_power_duty_cycle) {
            control.D -= 1;
        } else {
            // Chegou no ponto ótimo — sweep concluído
            usart_send_string("Sweep OK! D_max=");
            usart_send_uint16((uint16_t)max_power_duty_cycle);
            usart_send_string(" P_max=");
            usart_send_float(max_power, 4);
            usart_send_string("\n");

            // Inicializa P[-1] para o P&O começar com referência válida
            control.pi[1] = control.pi[0];

            control.sweep_done = 1;
            sweep_finished_ascending = 0;  // reseta para eventual re-sweep
        }
    }
}