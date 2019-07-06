#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"

void vLEDinit( volatile uint8_t *ddr, uint8_t bit ) {
    *ddr |= (1 << bit);
}

void vLEDset( volatile uint8_t *port, uint8_t bit, signed portBASE_TYPE xValue ) {
    if (xValue == pdTRUE) {
        *port &= ~(1 << bit);
    } else {
        *port |= (1 << bit);
    }
}

void vLEDtoggle( volatile uint8_t *port, uint8_t bit) {
    *port ^= (1 << bit);
}
