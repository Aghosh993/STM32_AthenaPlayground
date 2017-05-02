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

#include "timekeeper.h"

static volatile flag_data_struct flag_queue[MAX_NUM_FLAGS];
static volatile uint8_t flag_queue_depth;

static volatile time_val_t system_epoch;
static volatile time_val_t t_target;

static volatile uint8_t system_epoch_mutex;
static volatile uint8_t delay_flag;

void timekeeper_increment_timebase(void)
{
	system_epoch.t_us += 1000;
	if(system_epoch.t_us >= 1000000)
	{
		system_epoch.t_sec += 1;
		system_epoch.t_us %= 1000000;
	}
}

static uint64_t timekeeper_to_usec(time_val_t t)
{
	return (uint64_t)(t.t_sec * (uint32_t)1000000) + (uint64_t)(t.t_us);
}

time_val_t timekeeper_add_times(time_val_t t1, time_val_t t2)
{
	time_val_t sum;

	sum.t_sec = t1.t_sec + t2.t_sec;
	sum.t_us = t1.t_us + t2.t_us;

	sum.t_sec += sum.t_us / 1000000;
	sum.t_us %= 1000000;

	return sum;
}

void timekeeper_setup(void)
{
	timekeeper_hal_setup();

	uint8_t i = 0U;
	for(i=0U; i<MAX_NUM_FLAGS; ++i)
	{
		flag_queue[i].flag_interval_millis = 0xFFFFU;
		flag_queue[i].millis_to_go = 0xFFFFU;
		flag_queue[i].state = STATE_NOT_SETUP;
	}

	system_epoch_mutex = 0;
		system_epoch.t_us = 0;
		system_epoch.t_sec = 0;
		delay_flag = 0;
	system_epoch_mutex = 1;

	flag_queue_depth = 0U;
}

void flag_scheduler_callback(void)
{
	uint8_t i = 0U;
	for(i=0U; i<flag_queue_depth; ++i)
	{
		if(flag_queue[i].state != STATE_NOT_SETUP && flag_queue[i].state != STATE_PENDING)
		{
			if(flag_queue[i].millis_to_go > 0U)
			{
				--flag_queue[i].millis_to_go;
			}
			else
			{
				flag_queue[i].state = STATE_PENDING;
				flag_queue[i].millis_to_go = flag_queue[i].flag_interval_millis;
			}
		}
	}
}

uint8_t create_flag(uint16_t interval_ms)
{
	_disable_interrupts();
		if(flag_queue_depth < MAX_NUM_FLAGS)
		{
			flag_queue[flag_queue_depth].state = STATE_PROCESSED;
			flag_queue[flag_queue_depth].millis_to_go = interval_ms;
			flag_queue[flag_queue_depth].flag_interval_millis = interval_ms;
		}
		++flag_queue_depth;
	_enable_interrupts();
	return flag_queue_depth-1;
}

flag_state get_flag_state(uint8_t flag_id)
{
	if(flag_id < flag_queue_depth)
	{
		return flag_queue[flag_id].state;
	}
	return ERR_FLAG_NOT_FOUND;
}

flag_state reset_flag(uint8_t flag_id)
{
	if(flag_id < flag_queue_depth)
	{
		flag_queue[flag_id].state = STATE_PROCESSED;
		return FLAG_SET_SUCCESS;
	}
	return ERR_FLAG_NOT_FOUND;
}

void timekeeper_callback(void)
{
	if(system_epoch_mutex)
	{
		timekeeper_hal_reset_micros();
		timekeeper_increment_timebase();
	}

	if(delay_flag)
	{
		if(system_epoch.t_sec == t_target.t_sec && system_epoch.t_us >= t_target.t_us)
		{
			delay_flag = 0;
		}
		if(system_epoch.t_sec > t_target.t_sec)
		{
			delay_flag = 0;			
		}
	}
	timekeeper_hal_callback();
}

time_val_t timekeeper_get_time(void)
{
	system_epoch_mutex = 0;
		time_val_t ret = system_epoch;
	system_epoch_mutex = 1;

	ret.t_us += timekeeper_hal_get_micros();

	ret.t_sec += system_epoch.t_us/1000000;
	ret.t_us %= 1000000;

	return ret;
}

uint64_t timekeeper_get_abstime_micros(void)
{
	return timekeeper_to_usec(timekeeper_get_time());
}

void timekeeper_reset_time(void)
{
	system_epoch.t_us = 0;
	system_epoch.t_sec = 0;
}

void timekeeper_set_time(time_val_t t)
{
	system_epoch.t_us = t.t_us;
	system_epoch.t_sec = t.t_sec;
}

void timekeeper_delay_blocking(time_val_t delay_time)
{
	system_epoch_mutex = 0;
		time_val_t t0 = timekeeper_get_time();
	system_epoch_mutex = 1;

	t_target = timekeeper_add_times(t0, delay_time);
	
	delay_flag = 1;
	
	while(delay_flag);
}

// asynch_task* timekeeper_sched_delay_asynch(time_val_t delay_time)
// {

// }

// timekeeper_task_status timekeeper_get_task_flag(asynch_task t)
// {

// }