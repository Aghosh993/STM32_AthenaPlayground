# Basic demo of a shell on the STM32 F3 Discovery

## Overview

This project shows how to invoke a simple shell API I have created (in 
the user_source/system_shell.c and corresponding header files) to allow 
the the registration of any number of applications created by the user 
at compile-time. All apps must obey the C standard argc/argv-style calling 
convention so that the shell can accept, tokenize and pass on user commands.

## Interface

The shell performs standard I/O over the STM32 F3's USART1 peripheral, 
at 115200 baud, 8n1. Any standard terminal emulator (screen, miniterm, 
etc.) can be used to interface with this shell. At startup, the terminal 
will be cleared, and a simple welcome message should be printed. The 
PC4/PC5 pins are used for the Black Magic Probe COM port mode, while PA9 
and PA10 are used if the user is utilizing an external FTDI adapter.

Those using the "Black Magic Probe" firmware on the Discovery board's 
ST-Link should use the "bmp" Make target to load the firmware. The 
USART/UART interface should be present on the /dev/ttyACM1 interface on 
most Unix-like operating systems, or on the lower-numbered COMx port on 
Windows. On Windows, "Teraterm" is a most excellent terminal emulator to 
use the shell over UART. The same baud and parity settings apply as 
mentioned above.

Those using the ST-Link Virtual COM port functionality (with ST's 
default STLink firmware) should load the firmware using the "load" 
target. Please note: You will probably be unable to see the ST-Link's 
VCOM port on Linux/Mac as to my knowledge, that functionality only works 
on Windows with ST's drivers. Please use a TTL UART-USB converter (3.3V 
levels) such as an FT230XS-based FTDI chip or FT232R. Breakout boards 
are available throughout the web, on sites like 
[Sparkfun](https://www.sparkfun.com/products/9873) or 
[Adafruit](https://www.adafruit.com/product/284).

# Command set

Some default commands that are bundled:

- help: Shows available commands/apps installed

- led_ctl: Controls one of two LEDs on the Discovery board

- time: Shows the system elapsed time in second,microsecond format

- compfilter: Runs a second-order complementary filter using the onboard 
IMU, and displays the roll and pitch of the board

- clear: Clears the terminal buffer

(c) Abhimanyu Ghosh, 2017
