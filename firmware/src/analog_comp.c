#include "analog_comp.h"
#include "pwm.h"
#include "usart.h"

void analog_comp_init(void)
{
    // PD6 (AIN0) e PD7 (AIN1) como entrada (high-Z)
    DDRD  &= ~((1 << PD6) | (1 << PD7));
    PORTD &= ~((1 << PD6) | (1 << PD7));  // sem pull-up
    
    // Desabilita buffer digital nos pinos AIN0/AIN1
    // para reduzir ruído e consumo
    DIDR1 = (1 << AIN0D) | (1 << AIN1D);
    
    // Configura o comparador analógico:
    // - ACBG = 0: usa AIN0 externo (não a referência interna de 1.1V)
    // - ACIS1:0 = 11: dispara na borda de SUBIDA 
    //   (AIN0 cruza AIN1 de baixo para cima = sobretensão)
    // Desabilita interrupção antes de mudar os bits de configuração
    ACSR = (0 << ACIE) | (1 << ACIS1) | (1 << ACIS0);
    
    // Limpa a flag de interrupção (escrevendo 1 em ACI) para evitar disparos falsos
    ACSR |= (1 << ACI);
    
    // Habilita a interrupção do comparador
    ACSR |= (1 << ACIE);
    
    // Garante que o comparador NÃO está desabilitado
    ACSR &= ~(1 << ACD);
    
    // Desconecta o multiplexador do ADC do comparador
    // (queremos usar AIN1 externo, não um canal ADC)
    ADCSRB &= ~(1 << ACME);
}

/**
 * @brief ISR do Comparador Analógico — PROTEÇÃO DE SOBRETENSÃO
 * 
 * Executada em ~3-5 µs após Vbatt ultrapassar o limiar de hardware.
 * Desabilita imediatamente o driver de gate e zera o PWM.
 */
ISR(ANALOG_COMP_vect)
{
    // 1. DESLIGA O DRIVER IMEDIATAMENTE (PB3 = LOW)
    clr_bit(ENABLE_DRIVER_PORT, ENABLE_DRIVER);
    
    // 2. ZERA O PWM (OCR1A = 0)
    set_pwm_off();
    
    // 3. Sinaliza erro de HARDWARE para a FSM
    //    Usa flag dedicada que o software nunca limpa automaticamente
#ifdef MACHINE_ON
    error_flags.overvoltage_hw = 1;
    state_machine = STATE_ERROR;
#endif
}
