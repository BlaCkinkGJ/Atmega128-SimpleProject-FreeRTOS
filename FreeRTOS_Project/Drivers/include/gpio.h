#ifndef GPIO_H
#define GPIO_H

#include <inttypes.h>

void vGPIOinit( volatile uint8_t *ddr, uint8_t bit );
void vGPIOset( volatile uint8_t *port, uint8_t bit, signed portBASE_TYPE xValue );
void vGPIOtoggle( volatile uint8_t *port, uint8_t bit );

#endif
