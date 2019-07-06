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

#define mainLED_TASK_PRIORITY tskIDLE_PRIORITY

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

portSHORT main(void){
    xTaskCreate(vLEDFlashTask1,
                (const char *)"LED1",
                configMINIMAL_STACK_SIZE,
                NULL,
                mainLED_TASK_PRIORITY,
                NULL);
    xTaskCreate(vLEDFlashTask2,
                (const char *)"LED2",
                configMINIMAL_STACK_SIZE,
                NULL,
                mainLED_TASK_PRIORITY,
                NULL);
    vTaskStartScheduler();
    
    while (1) { }
    return 0;
}

