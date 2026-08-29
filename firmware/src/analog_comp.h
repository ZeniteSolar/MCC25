#ifndef ANALOG_COMP_H
#define ANALOG_COMP_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "conf.h"

#ifdef MACHINE_ON
#include "machine.h"
#endif

/**
 * @brief Inicializa o comparador analógico para proteção de sobretensão.
 * 
 * AIN0 (PD6, pino 12): Sinal de Vbatt sensoriado (via LM324C)
 * AIN1 (PD7, pino 13): Referência fixa (~3,205V = 43.3V na bateria)
 * 
 * Quando AIN0 > AIN1 (Vbatt > 43.3V), dispara ISR que desliga o driver.
 */
void analog_comp_init(void);

#endif /* ANALOG_COMP_H */
