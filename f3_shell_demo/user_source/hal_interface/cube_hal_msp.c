#include "hal_common_includes.h"

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	if(huart->Instance == USART1)
	{
		#if defined FTDI_SERIAL
			__HAL_RCC_GPIOA_CLK_ENABLE();
			
			/* UART TX GPIO pin configuration  */
			GPIO_InitStruct.Pin       = GPIO_PIN_9;
			GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
			GPIO_InitStruct.Pull      = GPIO_PULLUP;
			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
			GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			/* UART RX GPIO pin configuration  */
			GPIO_InitStruct.Pin       = GPIO_PIN_10;
			GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
			GPIO_InitStruct.Pull      = GPIO_NOPULL;
			GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
			GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

			__HAL_RCC_USART1_CLK_ENABLE();
		#else
			#if defined BLACKMAGIC_PROBE_SERIAL
				__HAL_RCC_GPIOC_CLK_ENABLE();
				
				/* UART TX GPIO pin configuration  */
				GPIO_InitStruct.Pin       = GPIO_PIN_4;
				GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
				GPIO_InitStruct.Pull      = GPIO_PULLUP;
				GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

				HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

				/* UART RX GPIO pin configuration  */
				GPIO_InitStruct.Pin       = GPIO_PIN_5;
				GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
				GPIO_InitStruct.Pull      = GPIO_NOPULL;
				GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
				GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

				HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

				__HAL_RCC_USART1_CLK_ENABLE();
				#else
					#error "ERROR: Must select between BLACKMAGIC_PROBE_SERIAL or FTDI_SERIAL"
				#endif
		#endif
	}
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(htim_pwm->Instance == TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
		
		__HAL_RCC_GPIOD_CLK_ENABLE();
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;

		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		__HAL_RCC_I2C1_CLK_ENABLE();
	}
}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(hspi->Instance == SPI1)
	{
		/*
			For SPI1 Signal lines (MOSI, MISO, CLK)
		 */
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/*
			For GPIO allocated to L3GD20 CS line:
		 */
		__HAL_RCC_GPIOE_CLK_ENABLE();
		
		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET); // Initialize CS pin to HIGH to deselect L3GD20 on the board

		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		__HAL_RCC_SPI1_CLK_ENABLE();
	}
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance == TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
	}
}