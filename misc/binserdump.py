#!/usr/bin/env python2

import serial;
import sys;

s = serial.Serial('/dev/ttyUSB0', 1250000)
while True:
	raw = s.read()
	sys.stdout.write(raw)
	sys.stdout.flush()

