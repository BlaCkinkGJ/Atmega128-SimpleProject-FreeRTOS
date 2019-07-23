/*
 * FreeRTOS_Project.c
 *
 * Created: 2019-07-06 오후 2:55:28
 * Author : kijun
 */ 
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "limits.h"
#include "gpio.h"
#include "usart.h"

#define DATA_TASK_PRIO 2
#define CLOCK_TASK_PRIO 3
#define USART_TASK_PRIO tskIDLE_PRIORITY 

#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)

EventGroupHandle_t xEventGroup;

void useVoltageSupply(volatile uint8_t *ddr, volatile uint8_t *port, uint8_t pin) {
   vGPIOinit(ddr, pin);
   vGPIOset(port, pin, 1);
}

void vSetDataPathValue(void *pvParameters) {
    EventBits_t uxBits;

    vGPIOinit(&DDRD, 0);
    vGPIOinit(&DDRD, 1);

    while(1) {
        uxBits = xEventGroupWaitBits(xEventGroup,
                                     BIT_0 | BIT_1,
                                     pdTRUE,
                                     pdFALSE,
                                     portMAX_DELAY);
        if ((uxBits & (BIT_0 | BIT_1)) == (BIT_0 | BIT_1)) {
            vGPIOset(&PORTD, 0, 1);
            vGPIOset(&PORTD, 1, 1);
        } else if ((uxBits & BIT_0) != 0) {
            vGPIOset(&PORTD, 0, 1);
            vGPIOset(&PORTD, 1, 0);
        } else if ((uxBits & BIT_1) != 0) {
            vGPIOset(&PORTD, 0, 0);
            vGPIOset(&PORTD, 1, 1);
        } else { /* When time-out occurred */
            vGPIOset(&PORTD, 0, 0);
            vGPIOset(&PORTD, 1, 0);
        }
    }
}

void vD_FlipFlopClock(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 5000;

    vGPIOinit(&DDRD, 4);
    xLastWakeTime = xTaskGetTickCount();
    while(4) {
        vGPIOset(&PORTD, 4, 1);
        vGPIOset(&PORTD, 4, 0);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void vUSARTCommunicationTask(void *pvParameters) {
    struct UsartConfigure_t conf = {
        .fosc = F_CPU,
        .baud = 9600
    };
    vUsartInit(conf);

    while (1) {
        char bitseq = vUsartReceive() - '0';

        xEventGroupSetBits(xEventGroup,
                           bitseq);
        vUsartTransmit(((bitseq & 0x02) >> 1)+'0');
        vUsartTransmit((bitseq & 0x01)+'0');
        vUsartTransmit('\r');
        vUsartTransmit('\n');
    }
}

portSHORT main(void){
    useVoltageSupply(&DDRD, &PORTD, 5);
    xEventGroup = xEventGroupCreate();

    if (xEventGroup == NULL)
        return -1;
    xTaskCreate(vSetDataPathValue,
                (const char *)"SetData",
                configMINIMAL_STACK_SIZE,
                NULL,
                DATA_TASK_PRIO,
                NULL);
    xTaskCreate(vD_FlipFlopClock,
                (const char *)"Clock",
                configMINIMAL_STACK_SIZE,
                NULL,
                CLOCK_TASK_PRIO,
                NULL);
    xTaskCreate(vUSARTCommunicationTask,
                (const char *)"USART",
                configMINIMAL_STACK_SIZE,
                NULL,
                USART_TASK_PRIO,
                NULL);
    vTaskStartScheduler();
    
    while (1) { }
    return 0;
}

