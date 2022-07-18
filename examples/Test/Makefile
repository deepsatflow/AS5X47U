

all: upload monitor

compile: 
	arduino-cli compile -b arduino:avr:mega -e

upload: 
	arduino-cli compile -b arduino:avr:mega -p /dev/cu.usbmodem11201 -u -e 

monitor: 
	arduino-cli monitor -p /dev/cu.usbmodem11201 -c baudrate=115200 

