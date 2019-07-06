#ifndef LEDTEST_H
#define LEDTEST_H

#include <inttypes.h>

void vLEDinit( volatile uint8_t *ddr, uint8_t bit );
void vLEDset( volatile uint8_t *port, uint8_t bit, signed portBASE_TYPE xValue );
void vLEDtoggle( volatile uint8_t *port, uint8_t bit );

#endif
