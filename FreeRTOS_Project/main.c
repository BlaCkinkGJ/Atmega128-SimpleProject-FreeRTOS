/*
 * FreeRTOS_Project.c
 *
 * Created: 2019-07-06 오후 2:55:28
 * Author : kijun
 */ 
#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "limits.h"
#include "led.h"
#include "usart.h"

#define LED_TASK_PRIO 1
#define USART_TASK_PRIO tskIDLE_PRIORITY 

QueueHandle_t ledQueue[2];

void vLEDFlashTask1(void *pvParameters) {
    vLEDinit(&DDRD, 0);
    while(1) {
        char recv = 0;
        if (!xQueueReceive(ledQueue[0], &(recv), (TickType_t)portMAX_DELAY))
            continue;
        vLEDtoggle(&PORTD, 0);
    }
}

void vLEDFlashTask2(void *pvParameters) {
    vLEDinit(&DDRD, 1);
    while(1) {
        char recv = 0;
        if (!xQueueReceive(ledQueue[1], &(recv), (TickType_t)portMAX_DELAY))
            continue;
        vLEDtoggle(&PORTD, 1);
    }
}

void vUSARTCommunicationTask(void *pvParameters) {
    struct UsartConfigure_t conf = {
        .fosc = F_CPU,
        .baud = 9600
    };
    vUsartInit(conf);

    while (1) {
        unsigned char buffer = '\0';
        buffer = vUsartReceive();
        if (buffer != '\0')
            vUsartTransmit(buffer);
        switch (buffer) {
        case '0':
            xQueueSend(ledQueue[0], &(buffer), (TickType_t)portMAX_DELAY);
            break;
        case '1':
            xQueueSend(ledQueue[1], &(buffer), (TickType_t)portMAX_DELAY);
            break;
        }
    }
}

portSHORT main(void){
    for (int i = 0 ; i < 2; i++)
        ledQueue[i] = xQueueCreate(5, sizeof(char));
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

