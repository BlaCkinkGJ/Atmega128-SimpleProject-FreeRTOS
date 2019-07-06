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

void vLEDFlashTask(void *pvParameters) {
    vLEDTestInitialise();
    portTickType xLastWakeTime;
    const portTickType xFrequency = 1000;
    xLastWakeTime = xTaskGetTickCount();
    while(1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        vLEDTestToggleLED();
    }
}


portSHORT main(void)
{
    xTaskCreate(vLEDFlashTask,
                (const char *)"LED",
                configMINIMAL_STACK_SIZE,
                NULL,
                mainLED_TASK_PRIORITY,
                NULL);
    vTaskStartScheduler();
    while (1) 
    {
    }
    return 0;
}

