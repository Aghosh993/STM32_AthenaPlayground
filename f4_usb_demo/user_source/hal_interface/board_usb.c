#include "board_usb.h"

#include "board_led.h"

/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;
volatile int conn_init;

static void usb_vbus_exti_init(void)
{
	/*
		PA0 (OTG_VBUS) Init:
	 */
	/*
		GPIO data struct to hold settings:
	 */
	static GPIO_InitTypeDef  GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*
		Set up interrupt prioroties and enable the interrupts:
	 */
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 1);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/*
	User code section of the EXTI IRQ, called from HAL_GPIO_EXTI_IRQHandler
	after interrupt flags are cleared:
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_9)
	{
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == GPIO_PIN_RESET)
		{
			conn_init = 0;
			board_led_off(LED1);
		}
		else
		{
			conn_init = 1;
			board_led_on(LED1);
		}

	}
}

/*
	EXTI IRQ Handlers. These call the generic EXTI IRQ handler from the Cube
	HAL, which in turn call the user-defined generic EXTI Callback function 
	defined further below:
 */

void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

/* init function */
void board_usb_init(void)
{
  conn_init=0;

  usb_vbus_exti_init();

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
		if(conn_init)
		{
			if(CDC_Transmit_FS(pbuf, (uint16_t)len) == USBD_OK)
			{
				return len;
			}
			return -1;			
		}
		return -1;
	}
	return -1;
}