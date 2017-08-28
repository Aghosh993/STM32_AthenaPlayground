target extended-remote /dev/ttyACM0
monitor swdp_scan
attach 1
load f3_usb_demo.elf
kill