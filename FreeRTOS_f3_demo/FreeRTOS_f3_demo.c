/*
  This is the main user software application
  In this case, we have below the code necessary to use the FreeRTOS
  kernel to toggle a set of LEDs on a STM32 F0 Discovery board.

  (c) Abhimanyu Ghosh, 2017
 */

#include "cpu_hal_interface.h"
#include "board_led.h"
#include "board_button.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

xQueueHandle pbq;

void SysTick_Handler(void) {
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
    HAL_IncTick();
}

void buttonReadTask(void *pvParameters)
{
  int on = 0;
  BaseType_t status;

  while(1)
  {
    if(board_button_read(BTN_USER))
    {
      on = 1;
    }
    else
    {
      on = 0;
    }
    status = xQueueSendToBack(pbq, &on, 0);
    if(status != pdPASS)
    {
      board_led_on(LED2);
    }
    else
    {
      board_led_off(LED2);
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void ledControlTask(void *pvParameters)
{
  int stateVal = 0;
  BaseType_t status;
  
  while(1)
  {
    status = xQueueReceive(pbq, &stateVal, 100);
    if(status == pdPASS)
    {
      if(stateVal)
      {
        board_led_on(LED1);
      }
      else
      {
        board_led_off(LED1);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

int main()
{
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();
  /*
    Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
   */
  board_led_init();

  /*
    Initialize the buttons on the board:
   */
  board_button_init();

  int i = 0;

  pbq = xQueueCreate(10, sizeof(int));
  
  if (pbq == 0) {
    board_led_on(LED1);
    while(1); /* fatal error */
  }

  xTaskCreate(
      buttonReadTask,                 /* Function pointer */
      "Task0",                          /* Task name - for debugging only*/
      configMINIMAL_STACK_SIZE,         /* Stack depth in words */
      (void*) NULL,                     /* Pointer to tasks arguments (parameter) */
      tskIDLE_PRIORITY + 2UL,           /* Task priority*/
      NULL                              /* Task handle */
  );

  xTaskCreate(
      ledControlTask,                 /* Function pointer */
      "Task1",                          /* Task name - for debugging only*/
      configMINIMAL_STACK_SIZE,         /* Stack depth in words */
      (void*) NULL,                     /* Pointer to tasks arguments (parameter) */
      tskIDLE_PRIORITY + 2UL,           /* Task priority*/
      NULL                              /* Task handle */
  );
  
  vTaskStartScheduler();

  /*
    Do nothing here... the FreeRTOS kernel should be running and executing all scheduled tasks...
   */
  while(1);

  return 0;
}
