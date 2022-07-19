/** @file AS5247Spi.cpp
 *
 * @brief A library for Arduino boards that reads angles from AS5047 and AS5147 sensors.
 * 		  Also support configuration of the sensor parameters.
 *
 * @par
 * COPYRIGHT NOTICE: MIT License
 *
 * 	Copyright (c) 2020 Adrien Legrand <contact@adrien-legrand.com>
 *
 * 	Permission is hereby granted, free of charge, to any person obtaining a copy
 * 	of this software and associated documentation files (the "Software"), to deal
 * 	in the Software without restriction, including without limitation the rights
 * 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * 	copies of the Software, and to permit persons to whom the Software is
 * 	furnished to do so, subject to the following conditions:
 *
 * 	The above copyright notice and this permission notice shall be included in all
 * 	copies or substantial portions of the Software.
 *
 * 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * 	SOFTWARE.
 *
*/

#include "AS5247Spi.h"

AS5247Spi::AS5247Spi(uint8_t _chipSelectPin) {
	// Initialize SPI Communication
	chipSelectPin = _chipSelectPin;
	pinMode(chipSelectPin, OUTPUT);
	digitalWrite(chipSelectPin, HIGH);
	SPI.begin();
}


void AS5247Spi::writeData(uint32_t command, uint32_t value) {
	// @todo Expose the SPI Maximum Frequency in library interface.
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
	
	
	// Send command
	digitalWrite(chipSelectPin, LOW);

	Serial.println("");
	Serial.print("command: "); 
	Serial.println(command, HEX); 
	Serial.print("value: "); 
	Serial.println(value, HEX); 
	Serial.println("");

	uint8_t byteSend1 = command >> 16;
	uint8_t byteSend2 = command >> 8;
	uint8_t byteSend3 = command;

	Serial.print("byteSend1: "); 
	Serial.println(byteSend1, HEX);
	Serial.print("byteSend2: "); 
	Serial.println(byteSend2, HEX);
	Serial.print("byteSend3: "); 
	Serial.println(byteSend3, HEX);


	Serial.println(); 

	// send three times one byte at a time
	SPI.transfer(byteSend1);
	SPI.transfer(byteSend2);
	// SPI.transfer(byteSend3);

	Serial.println("command send: "); 
	byte byte_command[3]; 
	byte_command[0] = byteSend1;
	byte_command[1] = byteSend2; 
	byte_command[2] = byteSend3;

	for (int i = 0; i < sizeof(byte_command); i++){
		for(int j=7; j >= 0; j--){
            Serial.print(bitRead(byte_command[i], j)); 
        }
        Serial.print(" ");
    } 
	Serial.println(" "); 

	// SPI.transfer16(command);

	digitalWrite(chipSelectPin, HIGH);
	delayMicroseconds(1);
	
	// Read data
	digitalWrite(chipSelectPin, LOW);

	uint8_t byteSendValue1 = value >> 24; 
	uint8_t byteSendValue2 = value >> 16;
	uint8_t byteSendValue3 = value >> 8;

	Serial.print("byteSendValue1: "); 
	Serial.println(byteSendValue1, HEX);
	Serial.print("byteSendValue2: "); 
	Serial.println(byteSendValue2, HEX);
	Serial.print("byteSendValue3: "); 
	Serial.println(byteSendValue3, HEX);

	// send four times one bit at a time
	SPI.transfer(byteSendValue1);
	SPI.transfer(byteSendValue2);
	SPI.transfer(byteSendValue3);

	byte bytes_value[3]; 
	bytes_value[0] = byteSendValue1;
	bytes_value[1] = byteSendValue2; 
	bytes_value[2] = byteSendValue3;

	Serial.println("values send: ");
	for (int i = 0; i < sizeof(byte_command); i++){
		for(int j=7; j >= 0; j--){
            Serial.print(bitRead(byte_command[i], j)); 
        }
        Serial.print(" ");
    } 
	Serial.println(" "); 

	digitalWrite(chipSelectPin, HIGH);
	SPI.endTransaction();
	delayMicroseconds(1);

}

uint32_t AS5247Spi::readData(uint32_t command, uint32_t nopCommand) {
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));

	// Send Read Command
	digitalWrite(chipSelectPin, LOW);

	// send 16 bits data two times 
	// SPI.transfer16(command >> 16);
	// SPI.transfer16(command); 


	Serial.println("");
	Serial.print("command: "); 
	Serial.println(command, HEX); 
	Serial.println("");

	uint8_t byteSend1 = command >> 16;
	uint8_t byteSend2 = command >> 8;
	uint8_t byteSend3 = command;

	Serial.print("byteSend1: "); 
	Serial.println(byteSend1, HEX);
	Serial.print("byteSend2: "); 
	Serial.println(byteSend2, HEX);
	Serial.print("byteSend3: "); 
	Serial.println(byteSend3, HEX);
	Serial.println(); 


	// send three times one byte at a time
	SPI.transfer(byteSend1);
	SPI.transfer(byteSend2);
	SPI.transfer(byteSend3);

	Serial.println("command send: "); 
	byte byte_command[3]; 
	byte_command[0] = byteSend1;
	byte_command[1] = byteSend2; 
	byte_command[2] = byteSend3;

	for (int i = 0; i < sizeof(byte_command); i++){
		for(int j=7; j >= 0; j--){
            Serial.print(bitRead(byte_command[i], j)); 
        }
        Serial.print(" ");
    } 
	Serial.println(" "); 

	digitalWrite(chipSelectPin, HIGH);
	delayMicroseconds(1);

	// Send Nop Command while receiving data
	digitalWrite(chipSelectPin, LOW);


	uint32_t byteReceived1 = SPI.transfer(0x0000);
	uint32_t byteReceived2 = SPI.transfer(0x0000);
	uint32_t byteReceived3 = SPI.transfer(0x0000); 

	Serial.print("byteReceived1: "); 
	Serial.println(byteReceived1, BIN); 
	Serial.print("byteReceived2: "); 
	Serial.println(byteReceived2, BIN); 
	Serial.print("byteReceived3: "); 
	Serial.println(byteReceived3, BIN); 

	// merge data for two times 
	uint32_t receivedData = (byteReceived1 << 16) + (byteReceived2 << 8) + (byteReceived3 << 0); 

	digitalWrite(chipSelectPin, HIGH);
	SPI.endTransaction();
	delayMicroseconds(1);

	byte bytes_received[3]; 
	bytes_received[0] = (receivedData >> 16)  & 0xFF;
	bytes_received[1] = (receivedData >> 8)  & 0xFF;
	bytes_received[2] = (receivedData >> 0) & 0xFF;
	
	Serial.println("values received: "); 
	for (int i = 0; i < sizeof(bytes_received); i++){
		for(int j=7; j >= 0; j--){
            Serial.print(bitRead(bytes_received[i], j)); 
        }
        Serial.print(" ");
    } 
    Serial.println(); 

	return receivedData;
}

uint16_t AS5247Spi::readData16(uint16_t command, uint16_t nopCommand) {
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));

	// for testing only 
	// command = 0x7FF5; 

	// Send Read Command
	digitalWrite(chipSelectPin, LOW);
	// SPI.transfer16(command);

	uint8_t a = command >> 8; 
	uint8_t b = command;

	Serial.print("a: "); 
	Serial.println(a, HEX);
	Serial.print("b: "); 
	Serial.println(b, HEX);

	byte byte_command[2]; 
	byte_command[0] = b;
	byte_command[1] = a; 

	for (int i = sizeof(byte_command)-1; i >= 0; i--){
		for(int j=7; j >= 0; j--){
            Serial.print(bitRead(byte_command[i], j)); 
        }
        Serial.print(" ");
    } 
	Serial.println(" "); 

	SPI.transfer(a);
	SPI.transfer(b);

	Serial.print("command inside: "); 
	Serial.println(command, HEX);

	digitalWrite(chipSelectPin, HIGH);
	delayMicroseconds(1);
	// Send Nop Command while receiving data
	digitalWrite(chipSelectPin, LOW);


	uint16_t receivedData = SPI.transfer16(0x00);

	// uint16_t receivedData1 = SPI.transfer16(0x0000);
	// uint16_t receivedData2 = SPI.transfer16(0x0000);

	// uint16_t receivedData1 = SPI.transfer(0x00);
	// uint16_t receivedData2 = SPI.transfer(0x00);

	// uint16_t receivedData = (receivedData1 << 8)  + receivedData2; 

	// Serial.print("receivedData1: "); 
	// Serial.println(receivedData1, BIN); 
	// Serial.print("receivedData2: "); 
	// Serial.println(receivedData2, BIN); 

	// uint16_t receivedData = (receivedData2 << 8)  + receivedData1; 

	digitalWrite(chipSelectPin, HIGH);
	SPI.endTransaction();
	delayMicroseconds(1);

	byte bytes[2]; 
	bytes[0] = (receivedData >> 0)  & 0xFF;
	bytes[1] = (receivedData >> 8)  & 0xFF;

	for (int i = sizeof(bytes)-1; i >= 0; i--){
		for(int j=7; j >= 0; j--){
            Serial.print(bitRead(bytes[i], j)); 
        }
        Serial.print(" ");
    } 
    Serial.println(); 

	return receivedData;
}

uint8_t AS5247Spi::CRC8(byte byteArray[], int size){


    for(int i = 0; i < size; i++){
        Serial.println(byteArray[i], HEX); 
    }

    // Polynomial 
    const byte polynomial = 0x1D;
    // initial value 
    byte crc = 0xC4;  
    
    // calculate crc 
    for(int i = 0; i < size; i++){
        crc ^= byteArray[i]; 
        for(int j = 0; j < 8; j++){
            if((crc & 0x80) != 0){
                crc = (byte)((crc << 1) ^ polynomial); 
            } else {
                crc <<= 1; 
            }
        }
    }
    // final XOR  
    crc &= 0xFF;
    crc ^= 0xFF; 
    return crc; 

}