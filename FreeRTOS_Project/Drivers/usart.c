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

static QueueHandle_t usart_interrupt_queue;

static unsigned char usartGetUbrr(struct UsartConfigure_t conf) {
	return conf.fosc/16/conf.baud - 1;
}

void vUsartInit(struct UsartConfigure_t conf) {
	unsigned char ubrr = usartGetUbrr(conf);

    /* Set baud rate */
    USART_UBRRH = (unsigned char) (ubrr >> 8);
    USART_UBRRL = (unsigned char) ubrr;

    /* Enable receiver and transmitter */
    USART_UCSRB = (1 << USART_RXEN) | (1 << USART_TXEN)
#ifdef USART_INTERRUPT_ENABLE
           | (1 << USART_RXCIE);
    USART_INTERRUPT_PORT &= ~(1 << USART_INTERRUPT_RX_PIN);
    USART_INTERRUPT_PORT |= (1 << USARt_INTERRUPT_TX_PIN);
    usart_interrupt_queue = xQueueCreate(10, sizeof(char));
#endif
    ;

    /* Set frame format: 8 data, 1 stop bits*/
    // DO NOT CHANGE THE UCSZ0 value
    USART_UCSRC = (1 << USART_USBS) | (3 << UCSZ0);
}

void vUsartTransmit(unsigned char data) {
    /* Wait for empty transmit buffer */
    while (!(USART_UCSRA & (1 << USART_UDRE)))
        ;
    /* Put data into buffer, sends the data */
    USART_UDR = data;
}

unsigned char vUsartReceive(void) {
#ifdef USART_INTERRUPT_ENABLE
    int ret = '\0';
    if (!xQueueReceive(usart_interrupt_queue, &(ret), (TickType_t)portMAX_DELAY))
        return ret;
    return ret;
#else
    /* Wait for data to be received */
    while(!(USART_UCSRA & (1 << USART_RXC)))
        ;

    /* Get and return received data from buffer */
    return USART_UDR;
#endif
}

#ifdef USART_INTERRUPT_ENABLE
ISR(USART_RX_VECT){
    BaseType_t xHigherPriorityTaskWoken = pdTRUE;
    xQueueSendFromISR(usart_interrupt_queue,
                      (const void *) &USART_UDR,
                      &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken)
        taskYIELD();
}

#endif