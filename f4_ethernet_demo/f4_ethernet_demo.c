/*
  This is the main user software application
  In this case, we have below the code necessary to toggle a set of LEDs on a STM32 F4 Discovery PCB at 20 Hz.

  (c) Abhimanyu Ghosh, 2017
 */

#include "cpu_hal_interface.h"
#include "board_led.h"
#include "lwip_demo.h"

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

  MX_LWIP_Init();

  /*
    In an infinite loop, keep toggling the LEDs in an alternating pattern:
   */
  while(1)
  {
    MX_LWIP_Process();
    // cpu_sw_delay(1); // Need to figure out how to avoid polling, probably DMA or interrupts
  }

  return 0;
}