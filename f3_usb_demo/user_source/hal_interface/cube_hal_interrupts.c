#include "hal_common_includes.h"
#include "board_usb.h"

extern PCD_HandleTypeDef hpcd_USB_FS;

void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

void USB_LP_CAN_RX0_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
  while(1);
}

void SVC_Handler(void)
{

}

void PendSV_Handler(void)
{
  
}