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

#ifndef TIMEKEEPER_HAL_H_
#define TIMEKEEPER_HAL_H_	1

#include "hal_common_includes.h"
#include "interrupt_utils.h"

void timekeeper_hal_setup(void);
void timekeeper_hal_callback(void);
uint32_t timekeeper_hal_get_micros(void);
void timekeeper_hal_reset_micros(void);

#endif