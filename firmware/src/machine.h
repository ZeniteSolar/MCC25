#ifndef __AVR_ATmega328P__
    #define __AVR_ATmega328P__
#endif

#ifndef MACHINE_H
#define MACHINE_H 

#include <avr/io.h>
#include <avr/wdt.h>
#include"conf.h"

// #ifdef USART_ON
// #include "usart.h"
// #endif
#ifdef ADC_ON
#include "adc.h"
#endif

//DEFINIÇÕES DE MEDIDAS
#define MAXIMUM_PANEL_CURRENT 11.0f //11A
#define MAXIMUM_PANEL_VOLTAGE 24.0f // 21V
#define MAXIMUM_BATT_VOLTAGE 60.0f // 60V
#define MINIMUM_BATT_VOLTAGE 33.0f //33V
#define MINIMUM_PANEL_VOLTAGE 9.0f //9V

//DEFINIÇÕES GERAIS
#define LED PC5
#define LED_DDR DDRC
#define LED_PORT PORTC
#define ENABLE_DRIVER PB3
#define ENABLE_DRIVER_DDR DDRB
#define ENABLE_DRIVER_PORT PORTB

volatile uint8_t machine_clk;
uint8_t total_errors;   // Contagem de ERROS

typedef enum state_machine{
    STATE_INITIALIZING,
    STATE_RUNNING,
    STATE_ERROR,
} state_machine_t;

extern volatile state_machine_t state_machine;

typedef struct control{
    uint8_t    D;              // value converted from 0 to TOP
    uint8_t  sweep_done;
    float    i_panel[2];              // value of panel current in AMPS
    float    v_panel[2];              // value of panel voltage in VOLTS
	float	v_batt[2];			// value of battery voltage in VOLTS
	float	pi[2];			// value of power in WATTS

}control_t;

extern volatile control_t control;

typedef union error_flags{
    struct{
        uint8_t     overcurrent :1;
        uint8_t     overvoltage :1;
		uint8_t		undervoltage:1;
		uint8_t		overvolt_panel:1;
		uint8_t		undervolt_panel:1;
    };
    uint8_t   all;
}error_flags_t;
extern volatile error_flags_t error_flags;

void machine_init(void);
void check_buffers(void);
void check_panel_voltage(void);
void check_panel_current(void);
void check_batt_voltage(void);
void read_and_check_adcs(void);
void clr_EN_driver(void);
void set_EN_driver(void);
void set_LED(void);
void task_initializing(void);
void task_running(void);
void task_error(void);
void machine_run(void);

//control.i_panel = ma_adc0(void);
//control.v_panel = ma_adc1(void);
//control.v_batt = ma_adc2(void);

#endif /* ifndef MACHINE_H */