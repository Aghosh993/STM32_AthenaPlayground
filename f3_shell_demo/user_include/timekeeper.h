/*
	(c) Abhimanyu Ghosh, 2017
	License: GPL-3.0

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

#ifndef TIMEKEEPER_H_
#define TIMEKEEPER_H_	1

#include "timekeeper_hal.h"
#include <stdint.h>

#define MAX_NUM_FLAGS				30U

typedef struct {
	uint32_t t_sec;
	uint32_t t_us;
} time_val_t;

typedef enum {
	STATE_NOT_SETUP,
	STATE_PENDING,
	STATE_PROCESSED,
	ERR_FLAG_NOT_FOUND,
	FLAG_SET_SUCCESS
} flag_state;

typedef struct {
	flag_state state;
	uint16_t millis_to_go;
	uint16_t flag_interval_millis;
} flag_data_struct;

typedef enum {
	FLAG_SET,
	FLAG_RESET
} timekeeper_task_status;

typedef struct {
	timekeeper_task_status s;
	uint32_t task_id;
} asynch_task;

void timekeeper_setup(void);
void timekeeper_callback(void);
void flag_scheduler_callback(void);

/*
	Task scheduling functions:
 */
uint8_t create_flag(uint16_t interval_ms);
flag_state get_flag_state(uint8_t flag_id);
flag_state reset_flag(uint8_t flag_id);

time_val_t timekeeper_get_time(void);
uint64_t timekeeper_get_abstime_micros(void);
void timekeeper_reset_time(void);
void timekeeper_set_time(time_val_t t);

void timekeeper_delay_blocking(time_val_t delay_time);

time_val_t timekeeper_add_times(time_val_t t1, time_val_t t2);
// asynch_task* timekeeper_sched_delay_asynch(time_val_t delay_time);
// timekeeper_task_status timekeeper_get_task_flag(asynch_task t);

#endif