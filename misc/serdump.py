#!/usr/bin/env python2

import serial;
import sys;
import time;

s = serial.Serial('/dev/ttyUSB0', 1250000, rtscts=True)
h = 44
while True:
	if s.inWaiting() > 0:
		raw = s.read(s.inWaiting());
		for c in raw:
			o = ord(c);
			if o >= 32 and o < 127:
				sys.stdout.write("%s"%c)
			elif o == 10:
				sys.stdout.write("\x1b[34m0a\x1b[0m\n")
			else:
				h = [43,47][h==43];
				sys.stdout.write("\x1b[%dm%02x\x1b[0m"%(h,o))
	else:
		sys.stdout.flush();
		time.sleep(0.0625);

