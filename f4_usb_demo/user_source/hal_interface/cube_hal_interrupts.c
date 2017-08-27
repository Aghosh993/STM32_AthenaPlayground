#include "hal_common_includes.h"

extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

/**
* @brief This function handles USB On The Go FS global interrupt.
*/
void OTG_FS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd_USB_OTG_FS);
}
void NMI_Handler(void)
{
	while(1);
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