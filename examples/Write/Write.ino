


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

    // Serial.println("Writing register setting");
    as5247u.writeRegister(SETTINGS3_REG, 0xFC); 
    delay(500); 

    // readDataFrame16 = as5247u.readRegister16(SETTINGS3_REG); 

    // uint16_t settings3_value_1 = readDataFrame16.values.data; 
    // uint8_t a = settings3_value_1; 
	// uint8_t b = settings3_value_1 >> 8;

	// Serial.print("a: "); 
	// Serial.println(a, HEX);
	// Serial.print("b: "); 
	// Serial.println(b, HEX);

    // byte bytes[2]; 
	// bytes[0] = (settings3_value_1 >> 0)  & 0xFF;
	// bytes[1] = (settings3_value_1 >> 8)  & 0xFF;

	// for (int i = sizeof(bytes)-1; i >= 0; i--){
	// 	for(int j=7; j >= 0; j--){
    //         Serial.print(bitRead(bytes[i], j)); 
    //     }
    //     Serial.print(" ");
    // } 

    // Serial.println("\nend of 16 bit\n"); 

    // readDataFrame = as5247u.readRegister(SETTINGS3_REG); 
    // uint32_t settings3_value_2 = readDataFrame.values.data; 


	// Serial.println("values send: ");
	// for (int i = sizeof(bytes_value)-1; i >= 0; i--){
	// 	for(int j=7; j >= 0; j--){
    //         Serial.print(bitRead(bytes_value[i], j)); 
    //     }
    //     Serial.print(" ");
    // } 
	// Serial.println(" "); 


    as5247u.printDebugString(); 
    delay(10000); 
}

// ================ LOOP FUNCTION ====================
void loop() { 
    
}
