#include "usart.h"

void usart_init(uint16_t ubrr, uint8_t rx, uint8_t tx)
{
    // set BAUDRATE
    UBRR0H = (uint8_t)(ubrr >>8);
    UBRR0L = (uint8_t)ubrr;
    
    // Enable RX and TX
    UCSR0B = ((rx&1)<<RXEN0) | ((tx&1)<<TXEN0);
}
void usart_send_char(char data)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}
void usart_send_string(const char *s)
{
    uint8_t i = 0;  
    while(s[i] != '\0') usart_send_char(s[i++]);
}
void usart_send_uint16(uint16_t num)
{
    #define LEN      6              // 20gth of the string w/ null terminator
    #define BASE    10              // string as a decimal base
    #define FILL    '0'             // character to fill non-used algarisms.
    
    uint8_t i = LEN -1;             // index for each char of the string
    char str[LEN] = {FILL};         // ascii zero filled array
    str[i] = '\0';                  // adds string null terminator
    while(i--){
        str[i] = FILL + (num % BASE);// gets each algarism}
        num /= BASE;                // prepare the next
    }
    usart_send_string(str);       // sends the string
    
    #undef LEN
    #undef BASE
    #undef FILL
}
void usart_send_uint32(uint32_t num)
{
    #define LEN     11              // 20gth of the string w/ null terminator
    #define BASE    10              // string as a decimal base
    #define FILL    '0'             // character to fill non-used algarisms.
    
    uint8_t i = LEN -1;             // index for each char of the string
    char str[LEN] = {FILL};         // ascii zero filled array
    str[i] = '\0';                  // adds string null terminator
    while(i--){
        str[i] = FILL + (num % BASE);// gets each algarism}
        num /= BASE;                // prepare the next
    }
    usart_send_string(str);       // sends the string
    
    #undef LEN
    #undef BASE
    #undef FILL
}
// VERSAO DA ESTER
// void usart_send_float(float num, uint8_t decimal_places) {
//     char buffer[20]; // Buffer para armazenar a string
//     dtostrf(num, 1, decimal_places, buffer); // Converte o float para string
//     usart_send_string(buffer); // Envia a string pela UART
// }

// VERSAO DO MDE22

/**
 * @brief sends a float number in ascii trough serial.
 * @note Be aware that big numbers could cause a buffer overflow and override all memory after the buffer
 */
 void usart_send_float(float num, uint8_t decimal_places)
 {
     char str[20];
 
     dtostrf(num, 1, decimal_places,str);
 
     usart_send_string(str);
 }
 
 