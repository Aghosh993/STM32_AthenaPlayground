/*
	(c) Abhimanyu Ghosh, 2017
	License: LGPL-3.0

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "timekeeper_hal.h"
// #include "robot_pwm.h"

static volatile TIM_HandleTypeDef tim3_handle;

void timekeeper_hal_setup(void)
{
	tim3_handle.Instance = TIM3;
	
	tim3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim3_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	tim3_handle.Init.Prescaler = 71;
	tim3_handle.Init.Period = 0xFFFF;
	tim3_handle.Init.AutoReloadPreload = 0;

	TIM_ClockConfigTypeDef tim_clk_init;
	tim_clk_init.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	tim_clk_init.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;

	HAL_TIM_Base_Init(&tim3_handle);
	HAL_TIM_Base_Start(&tim3_handle);
	HAL_InitTick(TICK_INT_PRIORITY);
}

void timekeeper_hal_callback(void)
{
	// Do anything here that is terribly hardware-specific...
}

uint32_t timekeeper_hal_get_micros(void)
{
	return (uint32_t)(__HAL_TIM_GET_COUNTER(&tim3_handle));
}

void timekeeper_hal_reset_micros(void)
{
	__HAL_TIM_SET_COUNTER(&tim3_handle, 0);
}