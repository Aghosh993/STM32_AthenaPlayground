target extended-remote /dev/ttyACM0
monitor swdp_scan
attach 1
load FreeRTOS_f3_demo.elf
kill