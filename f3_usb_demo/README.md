# STM32 F3 Discovery USB CDC Loopback

This demo performs a loopback test using the STM32 F3's USB Device peripheral. The USB USER port on the board is configured as a USB CDC device endpoint, and a simple loopback implemented to echo back user inputs. In addition, the red and blue LEDs on the Discovery board should blink at about 1 Hz.

## Operating System compatibility

Linux users may need to run:

```bash
sudo modprobe cdc_acm
```

to load the necessary kernel module, prior to plugging into the USER USB port.

Windows users should see the port come up in device manager, but it is possible that on older Windows boxes, one might need to install a special INF file from ST to make Windows properly recognize the device as a USB CDC endpoint.

## Testing this demo

On a Linux system, the firmware can be built/loaded with:

```bash
make -j5 && make load
```

On Windows, MSYS can be used to run the "make -j5" portion of te above, while the STLink Utility needs to be downloaded (or a compatible JTAG probe used) to flash the resulting build artifacts (ELF/BIN files).

Once the firmware is flashed, reset. You should see the USB enumeration in Device Manager or by running "dmesg" on Linux. Now, open the resulting serial port (typically /dev/ttyACM0 on Linux and COMx on Windows) on PuTTy, GNU Screen or Teraterm. You should be able to type text and see it echoed back.

(c) Abhimanyu Ghosh, 2017