/*
  This is the main user software application
  In this case, we have below the code necessary to set up a USB CDC interface on the 
  F4 Discovery and then blink the red LED at about 1 Hz. The green LED is turned on/off 
  in response to the user plugging or unplugging the board from the USER USB (OTG) interface.
  This is done in response to the VBUS_OTG net going high when the USB interface comes up.
  
  To try out this demo, load it and then (with the OTG USB port unplugged) 
  reset the Discovery board. The red LED should be blinking. Now, plug in a microUSB cable 
  into the OTG port. The green LED should come on to indicate the software has detected 
  the USB port coming up. Now, open a terminal and point it to the CDC ACM interface that 
  should be created. You should be able to type text and see it being looped back over 
  the USB CDC ACM interface.
  
  (c) Abhimanyu Ghosh, 2017
 */

#include "cpu_hal_interface.h"
#include "board_led.h"
#include "board_usb.h"

#include "foo.h"

int main()
{
  /*
    Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
   */
  board_led_init();

  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();

  board_usb_init();

  int i = 0;

  /*
    In an infinite loop, keep toggling the LEDs in an alternating pattern:
   */
  while(1)
  {
    /*
      Carry out a simple unit test of foo() declared in foo.h:
     */
    if(TEST_FOO(i, i+1) < 0)
    {
      /*
        If the above fails there is either a hardware, code or other undefined error.
        Now we're in an undefined state regarding processor core behavior...
       */
      while(1); // We probably have had a radiation hit or a major malfunction on the ALU of the processor...
    }
    else
    {
      board_led_on(LED2);

      cpu_sw_delay(50U);  // Invoke a simple software busy-wait routine to delay approximately 50 milliseconds

      board_led_off(LED2);

      cpu_sw_delay(50U);

      ++i; // Increment i for the next test iteration...
    }
  }

  return 0;
}