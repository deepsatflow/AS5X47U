

all: upload monitor

compile: 
	arduino-cli compile -b arduino:avr:mega 

upload: 
	arduino-cli compile -b arduino:avr:mega -p /dev/cu.usbmodem11201 -u -t

monitor: 
	arduino-cli monitor -p /dev/cu.usbmodem11201 -c baudrate=115200 

