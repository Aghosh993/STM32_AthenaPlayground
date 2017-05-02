/*
  BSD 3-Clause License

  Copyright (c) 2017, Abhimanyu Ghosh
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the name of the copyright holder nor the names of its
    contributors may be used to endorse or promote products derived from
    this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	This is the main user software application
  Several "Apps" are defined here with a standard calling interface to be compatible with a bash-like 
  shell environment. These apps can be "installed" into the shell by calling the necessary API registration 
  function in system_shell.h and system_shell.c
 */

#include <stdint.h>
#include <stdio.h>

#include "cpu_hal_interface.h"
#include "board_led.h"
#include "button.h"

#include "timekeeper.h"
#include "complementary_filter.h"
#include "system_shell.h"

void HAL_IncTick(void)
{
  timekeeper_callback();
  flag_scheduler_callback();
}

/*
  Apps defined for the various commands:
 */

void shell_clear(int argc, char** argv)
{
  clear_buffer();
}

void ledcontrol(int argc, char** argv)
{
  led l = LED1;

  if(argc < 2)
  {
    printf("Usage: led_ctl [LED1/LED2] [on/off]\r\n");
  }
  else
  {
    if(strncmp(argv[0], "led1", 4) == 0)
    {
      l = LED1;
    }
    else
    {
      if(strncmp(argv[0], "led2", 4) == 0)
      {
        l = LED2;
      }
      else
      {
        printf("INVALID LED selected\r\n");
      }
    }
    if(strncmp(argv[1], "on", 3) == 0)
    {
      board_led_on(l);
    }
    if(strncmp(argv[1], "off", 3) == 0)
    {
      board_led_off(l);
    }
  }
}

void watch(int argc, char** argv)
{
  time_val_t timenow = timekeeper_get_time();
  printf("\r\nTime now is: %d sec, %d microseconds\r\n", timenow.t_sec, timenow.t_us);
}

void run_comp_filter(int argc, char** argv)
{
  printf("\r\nThis app will run the second-order complementary filter until the USER button is pressed.\r\n");
  printf("Press p to proceed or e to exit\r\n");
  char c = ' ';
  while(c != 'p' && c != 'e')
  {
    scanf("%c", &c);
    if(c == 'e')
    {
      return;
    }
    if(c == 'p')
    {
      printf("Proceeding...\r\n");
      break;
    }
  }

  complementary_filter_struct s;
  init_complementary_filter(&s, SCALE_2G, SCALE_250_DPS, SCALE_1POINT9_GAUSS,
                              0.010f, 0.8f, 1.0f);

  uint8_t flag_10ms = create_flag(9U);
  uint8_t flag_100ms = create_flag(99U);

  /*
    Initialize the GPIOA Pin 0 (PA0) USER button as an input:
   */
  board_button_init();

  while(1)
  {
    if(board_button_pressed(USER_BUTTON))
    {
      int i = 0;
      for(i=0; i<100; ++i)
      {
        if(!board_button_pressed(USER_BUTTON))
        {
          break;
        }
      }
      printf("\r\nUSER button pressed, exiting...\r\n");
      return;
    }
    if(get_flag_state(flag_10ms) == STATE_PENDING)
    {
      reset_flag(flag_10ms);
      update_complementary_filter(&s);
    }

    if(get_flag_state(flag_100ms) == STATE_PENDING)
    {
      reset_flag(flag_100ms);
      printf("Roll: %f Pitch: %f\r\n", radians_to_degrees(s.state_vector.roll), radians_to_degrees(s.state_vector.pitch));
      // printf("Roll: %f Pitch: %f\r\n", radians_to_degrees(s.state_vector.roll_rate), radians_to_degrees(s.state_vector.pitch_rate));
    }
  }
}

void run_madgwick(int argc, char** argv)
{
  printf("\r\nThis app will run the Madgwick AHRS filter until the USER button is pressed.\r\n");
  printf("Press p to proceed or e to exit\r\n");
  char c = ' ';
  while(c != 'p' && c != 'e')
  {
    scanf("%c", &c);
    if(c == 'e')
    {
      return;
    }
    if(c == 'p')
    {
      printf("Proceeding...\r\n");
      break;
    }
  }
  init_madgwick(SCALE_2G, SCALE_250_DPS, SCALE_1POINT9_GAUSS, 0.010f, 0.10f);

  uint8_t flag_10ms = create_flag(9U);
  uint8_t flag_100ms = create_flag(99U);

  /*
    Initialize the GPIOA Pin 0 (PA0) USER button as an input:
   */
  board_button_init();

  while(1)
  {
    if(board_button_pressed(USER_BUTTON))
    {
      int i = 0;
      for(i=0; i<100; ++i)
      {
        if(!board_button_pressed(USER_BUTTON))
        {
          break;
        }
      }
      printf("\r\nUSER button pressed, exiting...\r\n");
      return;
    }

    if(get_flag_state(flag_10ms) == STATE_PENDING)
    {
      reset_flag(flag_10ms);
      update_madgwick();
    }

    if(get_flag_state(flag_100ms) == STATE_PENDING)
    {
      reset_flag(flag_100ms);
      float att[3];
      madgwick_get_attitude(att);
      printf("Roll: %f Pitch: %f Yaw: %f\r\n", radians_to_degrees(att[0]), radians_to_degrees(att[1]), radians_to_degrees(att[2]));
    }
  }
}

void imu_display(int argc, char** argv)
{
  printf("\r\nThis app will indefinitely poll and display IMU data.\r\n");
  printf("Press p to proceed or e to exit\r\n");
  char c = ' ';
  while(c != 'p' && c != 'e')
  {
    scanf("%c", &c);
    if(c == 'e')
    {
      return;
    }
    if(c == 'p')
    {
      printf("Proceeding...\r\n");
      break;
    }
  }

  imu_scaled_data_struct imu_data;
  uint8_t flag_100ms = create_flag(99U);
  initialize_imu(SCALE_2G, SCALE_250_DPS, SCALE_1POINT9_GAUSS, &imu_data);

  /*
    Initialize the GPIOA Pin 0 (PA0) USER button as an input:
   */
  board_button_init();

  while(1)
  {

  }
}

int main()
{
  /*
    Initialize the PLL, clock tree to all peripherals, flash and Systick 1 ms time reference:
   */
  cpu_init();
  /*
    Initialize the GPIO (General-Purpose I/O) subsystem pins that are connected to the LEDs on the board:
   */
  board_led_init();

  /*
    Setup timekeeping library and reset system time:
   */

  timekeeper_setup();
  timekeeper_reset_time();

  /*
    Install the desired commands, and then indefinitely run the shell:
   */

  setup_system_shell();

  printf("Initialized system shell...\r\n now installing apps...\r\n");

  if(install_cmd("led_ctl", ledcontrol) < 0)
  {
    printf("Install error in command led_ctl\r\n");
  }

  if(install_cmd("time", watch) < 0)
  {
    printf("Install error in command time\r\n");
  }

  if(install_cmd("compfilter", run_comp_filter) < 0)
  {
    printf("Install error in command compfilter\r\n");
  }

  if(install_cmd("madgwick", run_madgwick) < 0)
  {
    printf("Install error in command madgwick\r\n");
  }

  if(install_cmd("clear", shell_clear) < 0)
  {
    printf("Install error in command clear\r\n");
  }

  printf("...Done\r\n");

  watch(0, NULL);

  while(1)
  {
    shell_run();
  }
  return 0;
}
