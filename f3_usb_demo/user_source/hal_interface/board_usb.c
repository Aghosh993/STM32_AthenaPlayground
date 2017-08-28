#include "board_usb.h"

/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;

void board_usb_init(void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

  USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

  USBD_Start(&hUsbDeviceFS);

}

int board_usb_sendString(uint8_t *pbuf, int len)
{
	if(len > 0)
	{
		if(CDC_Transmit_FS(pbuf, (uint16_t)len) == USBD_OK)
		{
			return len;
		}
		return -1;
	}
	return -1;
}