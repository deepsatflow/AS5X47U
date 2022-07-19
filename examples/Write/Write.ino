


/*

    The encoder is connected as follows:
    AS5X47         Arduino Board

    5V <-------------> 5V
    GND <-------------> GND
    MOSI <-------------> MOSI (Pin 51 for Arduino Mega)
    MISO <-------------> MISO (Pin 50 for Arduino Mega)
    SCK <-------------> SCK  (Pin 52 for Arduino Mega)
    CSN <-------------> SS   Arbitrary, Pin 10 in this example.
    See Arduino SPI Reference to see how to connect with other boards.
    https://www.arduino.cc/en/reference/SPI

*/

// Include the library
#include <AS5247.h>

// Define where the CSN Pin in connected. 
int slaveSelectPin = 10;

// Programming status 
bool status = false;

// Start connection to the sensor.
AS5247 as5247u(slaveSelectPin);

ReadDataFrame16 readDataFrame16; 
ReadDataFrame readDataFrame; 

//===================== SETUP FUNCTION ================================= 
void setup() {

    // Setup serial 
    Serial.begin(115200);
    delay(1000);
    Serial.println("Setup complete!"); 
    delay(1000); 

    Serial.println("Writing register setting");
    as5247u.writeRegister(SETTINGS3_REG, 0x03); 
    as5247u.writeRegister(SETTINGS2_REG, 0xFF); 
    delay(500); 

    as5247u.printDebugString(); 
    as5247u.printDebugString();
    delay(10000); 
}

// ================ LOOP FUNCTION ====================
void loop() { 
    
}
