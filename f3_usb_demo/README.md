# STM32 F3 Discovery USB CDC Loopback

This demo performs a loopback test using the STM32 F3's USB Device peripheral. The USB USER port on the board is configured as a USB CDC device endpoint, and a simple loopback implemented to echo back user inputs.

This example has been tested to implement the ZLP (Zero Length Packet) portion of the USB CDC specification. I.e. when perfect multiples of 64 bytes are sent, a zero-length packet is send afterward to demarcate the transfer.

## Loopback tester

The included "test_loopback.py" script can be used with a user-specified COM port and data buffer length to benchmark this demo. The script sends a random string of the specified size and then verifies the contents of the response. On my Linux laptop running Kernel 4.12.3 using the cdc_acm kernel module, I am seeing around 280 kB/second round-trip bandwidth according to the script, using a chunk size of 4096 bytes. As always, YMMV depending on the host operating system and drivers used.

On my Windows 7 x86 virtual machine, I see a dismal 30 kBytes/second for a 4096-byte block size, though that may be correlated to the fact that I'm convolving Linux Kernel USB performance with that of VirtualBox USB passthru, and the Windows CDC ACM stack which is known to have substantially higher overhead for CDC ACM transactions compared to Mac OS X and Linux.

I do not presently own or have access to an OS X machine to benchmark this code on, but comments/feedback from those that do would be welcome :)

One can invoke the loopback test through the Makefile as well by running:

```bash
make -j5 && make bmp_looptest
```

### Note on loaders

The "bmp_looptest" target uses the Black Magic probe target but once can run the above command and replace bmp_looptest with stlink_looptest to use OpenOCD+STLink as the loader. Please note that the COMPORT variable in the Makefile might also have to be changed to match what your host enumerates the COM port as. Also, note that with the STLink target, the board will not auto-reset and this must be done manually by power-cycling the board or pushing the Reset button on the Discovery.

Note that a prompt will appear once the build and firmware flash is finished, asking for the USER USB port to be unplugged and plugged back in again. The Makefile provides 7 seconds for this to occur, before the loopback test proceeds and attempts to open the serial port.

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