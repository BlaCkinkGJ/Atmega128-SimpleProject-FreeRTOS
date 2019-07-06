/*
 * UART.c
 *
 * Created: 2019-06-28 오후 10:55:09
 * Author: kijun
 *
 * Notice: If you want to change the USART number
 *         then change each 0 to another number.
 *         For example, If I want to change the USART1 to USART0.
 *         Change the value of UBRR1H to UBRR0H.
 */ 
#include "usart.h"

static unsigned char usart_get_ubrr(struct usart_conf_t conf) {
	return conf.fosc/16/conf.baud - 1;
}

void usart_init(struct usart_conf_t conf) {
	unsigned char ubrr = usart_get_ubrr(conf);

    /* Set baud rate */
    USART_UBRRH = (unsigned char) (ubrr >> 8);
    USART_UBRRL = (unsigned char) ubrr;

    /* Enable receiver and transmitter */
    USART_UCSRB = (1 << USART_RXEN) | (1 << USART_TXEN)
#ifdef USART_INTERRUPT_ENABLE
           | (1 << USART_RXCIE);
    cbi(USART_INTERRUPT_PORT, USART_INTERRUPT_RX_PIN);
    sbi(USART_INTERRUPT_PORT, USARt_INTERRUPT_TX_PIN);
    //queue_init(&usart_queue)
#endif
    ;

    /* Set frame format: 8 data, 1 stop bits*/
    // DO NOT CHANGE THE UCSZ0 value
    USART_UCSRC = (1 << USART_USBS) | (3 << UCSZ0);
}

void usart_transmit(unsigned char data) {
    /* Wait for empty transmit buffer */
    while (!(USART_UCSRA & (1 << USART_UDRE)))
        ;
    /* Put data into buffer, sends the data */
    USART_UDR = data;
}

unsigned char usart_receive(void) {
    /* Wait for data to be received */
    while(!(USART_UCSRA & (1 << USART_RXC)))
        ;

    /* Get and return received data from buffer */
    return USART_UDR;
}

#ifdef USART_INTERRUPT_ENABLE
static char g_buffer;
unsigned char usart_get_buffer (void){
    int ret = g_buffer;
    g_buffer = '\0';
    return ret;
    //char ret = queue_get(&usart_queue);
    //return ret == -1 ? '\0' : ret;
}

ISR(USART_RX_VECT){
    /* Get and return received data from buffer */
    //queue_put(&usart_queue, UDR0);
    g_buffer = USART_UDR;
}

/************************************************************************/
/* usart flush function returns dummy data.                             */
/* This makes programmer can know which data is dummy                   */
/************************************************************************/
void usart_flush(void) {
    unsigned char dummy = '\0';
    while (USART_UCSRA & (1 << USART_RXC))
        dummy = USART_UDR;
}

#endif