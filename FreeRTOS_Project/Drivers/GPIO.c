#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"

void vGPIOinit( volatile uint8_t *ddr, uint8_t bit ) {
    *ddr |= (1 << bit);
}

void vGPIOset( volatile uint8_t *port, uint8_t bit, signed portBASE_TYPE xValue ) {
    if (xValue == pdFALSE) {
        *port &= ~(1 << bit);
    } else {
        *port |= (1 << bit);
    }
}

void vGPIOtoggle( volatile uint8_t *port, uint8_t bit) {
    *port ^= (1 << bit);
}
