[![Build 
Status](https://travis-ci.org/Aghosh993/STM32_AthenaPlayground.svg?branch=master)](https://travis-ci.org/Aghosh993/STM32_AthenaPlayground)

# STM32_AthenaPlayground
A repo of various C/C++ projects using the Athena build system applied to the STM32 F3 and F4 discovery boards

# Projects

## F3 Discovery Shell Demo

This project (within f3_shell_demo) implements a simple single-process 
command shell on the STM32F3 Discovery containing the STM32F303VCT6 MCU. 
Newlib is utilized and a simple getline() is implemented to dynamically 
parse variable-length commands.

## F3 Discovery USB Demo

This project (within f3_usb_demo) implements a loopback over the USB interface, and makes the STM32's USB USER port on the F3 Discovery emulate as a CDC ACM interface when plugged into a computer.

## F4 Discovery USB Demo

This project (within f4_usb_demo) implements a loopback over the USB interface, and makes the STM32's USB OTG port on the F4 Discovery emulate as a CDC ACM interface when plugged into a computer. It also shows a way to detect hotplugging events, by using an EXTI (External Interrupt) configured on both edges to detect when the OTG (microUSB) port has a cable connected or not.

## F3 Discovery FreeRTOS Queue Demo

This project (within FreeRTOS_f3_demo) shows how to use the FreeRTOS Queue API to send data between processes. It implements a simple demo where the user pushing the USER button on the Discovery board will cause the blue LED to come on, with the LED being turned off once the button is released. Both devices are controlled at 100 Hz.

## F4 Discovery Ethernet Demo

This project is a first-cut demo of the LwIP (Lightweight IP) library on an STM32F4 Discovery board connected to a Waveshare DP83848 breakout through the on-chip EMAC via an RMII interface. The board is initialized and configures itself to a static IP, and can be pinged from any suitably-configured machine.