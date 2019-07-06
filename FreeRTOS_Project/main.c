/*
 * FreeRTOS_Project.c
 *
 * Created: 2019-07-06 오후 2:55:28
 * Author : kijun
 */ 
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "usart.h"

#define LED_TASK_PRIO 1
#define USART_TASK_PRIO tskIDLE_PRIORITY 

void vLEDFlashTask1(void *pvParameters) {
    portTickType xLastWakeTime;
    const portTickType xFrequency = 500;
    vLEDinit(&DDRD, 0);
    xLastWakeTime = xTaskGetTickCount();
    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        vLEDtoggle(&PORTD, 0);
    }
}

void vLEDFlashTask2(void *pvParameters) {
    portTickType xLastWakeTime;
    const portTickType xFrequency = 1000;
    xLastWakeTime = xTaskGetTickCount();
    vLEDinit(&DDRD, 1);
    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        vLEDtoggle(&PORTD, 1);
    }
}

void vUSARTCommunicationTask(void *pvParameters) {
    portTickType xLastWakeTime;
    const portTickType xFrequency = 10;
    struct usart_conf_t conf = {
        .fosc = F_CPU,
        .baud = 9600
    };
    usart_init(conf);
    xLastWakeTime = xTaskGetTickCount();

    while (1) {
        unsigned char buffer = '\0';
        buffer = usart_receive();
        if (buffer != '\0') usart_transmit(buffer);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

portSHORT main(void){
    xTaskCreate(vLEDFlashTask1,
                (const char *)"LED1",
                configMINIMAL_STACK_SIZE,
                NULL,
                LED_TASK_PRIO,
                NULL);
    xTaskCreate(vLEDFlashTask2,
                (const char *)"LED2",
                configMINIMAL_STACK_SIZE,
                NULL,
                LED_TASK_PRIO,
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

