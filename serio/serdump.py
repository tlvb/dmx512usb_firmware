#!/usr/bin/env python2

import serial;
import sys;
import time;

s = serial.Serial('/dev/ttyUSB0', 1250000)
while True:
	if s.inWaiting() > 0:
		sys.stdout.write(s.read(s.inWaiting()))
	else:
		time.sleep(0.0625);

