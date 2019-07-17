/*
 * UART.h
 *
 * Created: 2019-06-28 오후 10:52:00
 *  Author: kijun
 */ 


#ifndef USART_H_
#define USART_H_

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define F_CPU 16000000UL

#define USART_INTERRUPT_ENABLE ;

#define USART_UBRRH UBRR1H
#define USART_UBRRL UBRR1L

#define USART_UCSRA UCSR1A
#define USART_UCSRB UCSR1B
#define USART_UCSRC UCSR1C

#define USART_RXEN RXEN1
#define USART_TXEN TXEN1
#define USART_USBS USBS1
#define USART_UDRE UDRE1

#define USART_UDR UDR1

#define USART_RXC RXC1

/* interrupt related section */
#ifdef USART_INTERRUPT_ENABLE
#define USART_RXCIE RXCIE1
#define USART_RX_VECT USART1_RX_vect

#define USART_INTERRUPT_PORT DDRD
#define USART_INTERRUPT_RX_PIN 2
#define USARt_INTERRUPT_TX_PIN 3
#endif

/************************************************************************/
/* in case of the atmeaga128                                            */
/* use this type like below                                             */
/* struct usart_conf_t uart_conf = {                                    */
/*          .fosc = 1843200,                                            */
/*          .baud = 9600                                                */
/* };                                                                   */
/* This configuration is very reasonable for your programming           */
/************************************************************************/
struct UsartConfigure_t {
    unsigned long fosc;
    unsigned long baud;
};

void          vUsartInit             (struct UsartConfigure_t);
void          vUsartTransmit         (unsigned char);
void          vUsartTransmitString   (const char *);
unsigned char vUsartReceive          (void);
void          vUsartReceiveString    (char *, size_t, bool);
//@TODO: make a read the string and write the string.

#ifdef USART_INTERRUPT_ENABLE
#include <avr/interrupt.h>
#include "FreeRTOS.h" // THIS IS NECESSARY!! DO NOT FORGET THIS!
#include "queue.h"
#include "limits.h"
#endif


#endif /* UART_H_ */