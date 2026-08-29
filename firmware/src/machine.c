#include "machine.h"
#include "usart.h"
#include "pwm.h"

// Global variable definitions
volatile uint8_t machine_clk;
uint8_t total_errors;   // Contagem de ERROS

volatile control_t control;
volatile error_flags_t error_flags;
volatile state_machine_t state_machine;

volatile uint32_t tick = 0;
volatile uint32_t set_tick = 0;

void machine_init(void)
{
    TCCR2A  =   (1 << WGM21) | (0 << WGM20)         // Timer 2 in Mode 2 = CTC (clear on compar  e)
            | (0 << COM2A1) | (0 << COM2A0)         // do nothing with OC2A
            | (0 << COM2B1) | (0 << COM2B0);        // do nothing with OC2B
    TCCR2B  =   (0 << WGM22)                        // Timer 0 in Mode 2 = CTC (clear on compar  e)
            | (0 << FOC0A) | (0 << FOC0B)           // dont force outputs
            | (1 << CS22)                           // clock enabled, prescaller = 1024
            | (1 << CS21)
            | (1 << CS20);
    OCR2A   =   154;                              // Valor para igualdade de comparacao A para frequencia de ~100 Hz
    TIMSK2 |=   (1 << OCIE2A);                      // Ativa a interrupcao na igualdade de comp  aração do TC2 com OCR2A
	//TODO revisar isso
} 

void check_buffers(void)
{
    //VERBOSE_MSG_MACHINE(usart_send_string("Checking buffers..."));
    while(!CBUF_IsFull(cbuf_adc0));
    while(!CBUF_IsFull(cbuf_adc1));
    while(!CBUF_IsFull(cbuf_adc2));
    //VERBOSE_MSG_ERROR(usart_send_string("<No buffers to check>"));
    //VERBOSE_MSG_MACHINE(usart_send_string(" \t\tdone.\n")); 
}

void check_panel_voltage(void)
{
    uint8_t over = 0;
    uint8_t under = 0;
    
    if(control.v_panel[0] >= MAXIMUM_PANEL_VOLTAGE){
        over = 1;
    }
    else if(control.v_panel[0] <= MINIMUM_PANEL_VOLTAGE){
        under = 1;
    }
    
    cli();
    error_flags.overvolt_panel = over;
    error_flags.undervolt_panel = under;
    sei();
}

void check_panel_current(void)
{
    uint8_t over = 0;
    
    if(control.i_panel[0] >= MAXIMUM_PANEL_CURRENT)
    {
        over = 1;
    }
    
    cli();
    error_flags.overcurrent = over;
    sei();
}
void check_batt_voltage(void){
    if(control.v_batt[0] >= MAXIMUM_BATT_VOLTAGE){
        error_flags.overvoltage = 1;
    //} else if(control.v_batt[0] <= MINIMUM_BATT_VOLTAGE){
        //error_flags.undervoltage = 1;
    } else{
        // Protege contra race condition com a ISR do comparador analógico:
        // só limpa overvoltage se NÃO foi detectada pelo hardware
        cli();
        if (!error_flags.overvoltage_hw) {
            error_flags.overvoltage = 0;
        }
        //error_flags.undervoltage = 0;
        sei();
    }
}
void read_and_check_adcs(void){
#ifdef ADC_ON
    control.i_panel[0] = ipanel_to_float(ma_adc0());
    control.v_panel[0] = vpanel_to_float(ma_adc1());
    control.v_batt[0] = vbat_to_float(ma_adc2());

    //control.vbatt = vbaterry_to_float(ma_adc2); //TODO terminar
    switch(state_machine){
        case STATE_INITIALIZING:
            check_panel_voltage();
            check_panel_current();
            check_batt_voltage();

            break;
        case STATE_RUNNING:
            check_panel_voltage();
            check_panel_current();
            check_batt_voltage();

            break;      
        default:
            break;
    }
#endif
}

void set_LED(void){
    set_bit(LED_DDR, LED);
    set_bit(LED_PORT, LED);
}

void set_EN_driver(void){
    set_bit(ENABLE_DRIVER_DDR, ENABLE_DRIVER);
    set_bit(ENABLE_DRIVER_PORT, ENABLE_DRIVER);
}

void clr_EN_driver(void){
    clr_bit(ENABLE_DRIVER_PORT, ENABLE_DRIVER);
}

void task_initializing(void){
    //control.i_panel[0] = ipanel_to_float(ma_adc0());
    //control.v_panel[0] = vpanel_to_float(ma_adc1());
    set_LED();
    control.sweep_done = 0;
    //sweep(); --> Fazer uma varredura para encontrar a melhor faixa de operação

    //Verificando os limites elétricos
    //check_buffers();
    check_panel_voltage(); 
    check_panel_current();
    check_batt_voltage();
    //set_EN_driver();

    if(!error_flags.all){
        usart_send_string("Inicializando o sistema!\n");
        set_EN_driver(); // Liga o driver de forma segura antes de operar
        state_machine = STATE_RUNNING;
    }else{
        usart_send_string("Não foi possível inicializar!\n");
        state_machine = STATE_ERROR;
    }
}
void task_running(void){
    if(error_flags.all){
        state_machine = STATE_ERROR;
        return;
    }

    #ifdef PWM_ON
        if ((tick - set_tick) >= 500){
            set_tick = tick;
            pwm_compute();
        }
    #endif


    // TODO add usart msgs

    //chamar o check limites eletricos 
    //TODO logica de primeira corrida
        //sweep duty inicial 
    //P&O
    //COnsider duty limit
        // set duty  
    

}
void task_error(void){
    #ifdef PWM_ON
        pwm_reset();
        clr_EN_driver();
        clr_bit(LED_PORT,LED);
    #endif

    // Incrementa total_errors apenas na transição para estado de erro
    static state_machine_t prev_state = STATE_INITIALIZING;
    if (prev_state != STATE_ERROR) {
        total_errors++;
        prev_state = STATE_ERROR;
    }

    usart_send_string("The error code is: ");
    usart_send_uint16(error_flags.all);
    usart_send_char('\n');

    if(error_flags.overcurrent)
        usart_send_string("\t - Panel over-current!\n");
    if(error_flags.overvoltage)
        usart_send_string("\t - Battery over-voltage!\n");
    if(error_flags.undervolt_panel)
        usart_send_string("\t - Panel under-voltage!\n");
    //if(error_flags.undervoltage)
        //usart_send_string("\t - Baterry under-voltage!\n");
    if(error_flags.overvoltage_hw)
        usart_send_string("\t - HARDWARE Battery over-voltage!\n");
    if(!error_flags.all)
        usart_send_string("\t - Oh no, it was some unknown error.\n");

    usart_send_string("The error level is: ");
    usart_send_uint16(total_errors);
    usart_send_char('\n');

    if(total_errors < 2){
        usart_send_string("I will reset the machine state.\n");
    }
    if(total_errors >= 5){
        usart_send_string("The watchdog will reset the whole system.\n");
        for(;;);    // waits the watchdog to reset.
    }
 
    // Limpa a flag de HW para que o software (check_batt_voltage) 
    // decida se a tensão já baixou na próxima inicialização
    cli();
    error_flags.overvoltage_hw = 0;
    sei();

    state_machine = STATE_INITIALIZING;
    prev_state = STATE_INITIALIZING;
}

void machine_run(void){
    if(machine_clk){
        machine_clk = 0;
        if(adc_data_ready){
            adc_data_ready = 0;
            read_and_check_adcs();
        } 

        switch(state_machine){
            case STATE_INITIALIZING:
                task_initializing();

                break;
            case STATE_RUNNING:
                task_running();

                break;
            case STATE_ERROR:
                task_error();
                break;
            default:
                break;
        }
    
    }
/*     usart_send_string("Ipanel: ");
    usart_send_float(control.i_panel[0], 4);
    usart_send_string("Vpanel: ");
    usart_send_float(control.v_panel[0],4);
    usart_send_string("Vbatt: ");
    usart_send_uint16(ma_adc2());
    usart_send_char('\n');*/

}
ISR(TIMER2_COMPA_vect)
{
    //if(machine_clk) usart_send_string("\nERROR: CLOCK CONFLICT!!!\n");
	machine_clk = 1;
    tick +=10;
}