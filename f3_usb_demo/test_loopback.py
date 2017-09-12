#!/usr/bin/env python

# A simple Python script to test round-trip bandwidth of any UART interface.
# (c) Abhimanyu Ghosh, 2017
# License: BSD 3-Clause

import os
import time
import serial
import argparse
import random
import string

# A simple script to perform a loopback and bandwidth test on a serial interface
# (c) Abhimanyu Ghosh, 2017

def main():
	parser = argparse.ArgumentParser(description='Test loopback of a UART interface')
	parser.add_argument('-p', '--port', metavar='port', nargs=1, help='Path to UART port to open')
	parser.add_argument('-b', '--baud', metavar='baud', nargs=1, help='Baudrate in bits/sec (Ignored for USB CDC-ACM ports!!)')
	parser.add_argument('-n', '--chunklen', metavar='chunklen', nargs=1, help='Length of data buffer to send in loopback test. Default=32')

	args = parser.parse_args()

	if not args.port:
		serial_port = "/dev/ttyUSB0"
	else:
		serial_port = args.port[0]

	print("Opening port "+repr(serial_port))

	if not args.baud:
		baudrate = 115200
	else:
		baudrate = args.baud[0]

	try:
		com = serial.Serial(serial_port, baudrate, timeout=1)
	except IOError:
		print("Error opening UART, perhaps a permissions/parameter issue? Bye!")
		exit(-1)

	if not args.chunklen:
		bufSize = 32
	else:
		bufSize = int(args.chunklen[0])

	# Generate random string:
	testCharSeq = (''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(bufSize))).encode()

	# Mark the time of transmission:
	startTime = time.perf_counter()
	
	# Write data buffer to COM port:
	com.write(testCharSeq)

	# Now read back the data and verify it:
	inData = com.read(len(testCharSeq))
	# Error, verification failed!!
	if inData != testCharSeq:
		print("ERROR, only got:")
		print(inData)
		print("Length of response:")
		print(len(inData))
	else:
		# If verify went swimmingly, calculate RTT (round-trip time)
		# and bandwidth based on RTT:
		bandwidth = len(testCharSeq)/(time.perf_counter() - startTime)
		print(repr(len(testCharSeq))+" bytes echoed, round-trip bandwidth = "+repr(bandwidth)+" bytes/sec")

if __name__ == '__main__':
	main()