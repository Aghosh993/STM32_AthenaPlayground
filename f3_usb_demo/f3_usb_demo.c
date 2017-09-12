/*
	This is the main user software application
	In this case, we have below the code necessary to toggle a set of LEDs on a	STM32 F3 Discovery PCB at 20 Hz.

	(c) Abhimanyu Ghosh, 2017
 */

#include "cpu_hal_interface.h"
#include "board_led.h"
#include "usbd_cdc_if.h"

#include "foo.h"

volatile int data_available;
volatile int lastRecvLen;
extern uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];
extern uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USB Device Core handle declaration */
extern USBD_HandleTypeDef hUsbDeviceFS;

int main()
{
  data_available = 0;
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();
  /*
    Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
   */
  board_led_init();

  board_usb_init();

  int i = 0;

  /*
    In an infinite loop, keep toggling the LEDs in an alternating pattern:
   */
  uint32_t recvBytes = 0;
  uint8_t rc[100U];
  while(1)
  {
    if(data_available)
    {
      data_available = 0;
      if(lastRecvLen < 64)
      {
        CDC_Transmit_FS(UserRxBufferFS, (uint16_t)lastRecvLen);
        USBD_CDC_ReceivePacket(&hUsbDeviceFS);
      }
      else
      {
        CDC_Transmit_FS(UserRxBufferFS, 64U);
        USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
        while(hcdc->TxState);
        CDC_Transmit_FS(UserRxBufferFS, 0U);
        USBD_CDC_ReceivePacket(&hUsbDeviceFS);
      }
    }
  }
  return 0;
}
