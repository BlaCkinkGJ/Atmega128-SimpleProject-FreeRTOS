#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
// Output Port pin LED_O
#define ledtestPORT_LED_O      PORTD
#define ledtestDDR_LED_O       DDRD
#define ledtestBIT_LED_O       0

void vLEDTestInitialise( void )
{
    /* Set port B direction to outputs.  Start with all output off. */
    ledtestDDR_LED_O |= (1<<ledtestBIT_LED_O);
    ledtestPORT_LED_O &= ~(1<<ledtestBIT_LED_O);
}
/*-----------------------------------------------------------*/

void vLEDTestSetLED( signed portBASE_TYPE xValue )
{

    vTaskSuspendAll();
    {
        if( xValue == pdTRUE )
        {
            ledtestPORT_LED_O &= ~(1<<ledtestBIT_LED_O);
        }
        else
        {
            ledtestPORT_LED_O |= (1<<ledtestBIT_LED_O);
        }
    }
    xTaskResumeAll();

}
void vLEDTestToggleLED( void )
{
    vTaskSuspendAll();
    {
        ledtestPORT_LED_O ^= (1<<ledtestBIT_LED_O);
    }
    xTaskResumeAll();

}
