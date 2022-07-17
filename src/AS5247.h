


/** @file AS5247.h
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



#ifndef AS5247_h
#define AS5247_h

#include "AS5247Spi/AS5247Spi.h"

// Volatile Registers Addresses
#define NOP_REG			    0x0000
#define ERRFL_REG 		    0x0001
#define PROG_REG		    0x0003
#define DIA_REG             0x3FF5
#define AGC_REG             0x3FF9
#define SIN_DATA_REG        0x3FFA 
#define COS_DATA_REG        0x3FFB
#define VEL_REG             0x3FFC
#define MAG_REG             0x3FFD
#define ANGLEUNC_REG        0x3FFE 
#define ANGLECOM_REG        0x3FFF
#define ECC_CHECKSUM_REG    0x00D1 


// Non-Volatile Registers Addresses
#define DISABLE_REG         0x0015 
#define ZPOSM_REG           0x0016 
#define ZPOSL_REG           0x0017 
#define SETTINGS1_REG       0x0018 
#define SETTINGS2_REG 	    0x0019
#define SETTINGS3_REG       0x001A
#define ECC_REG             0x001B 


#define WRITE			0
#define READ			1


// ERRFL Register Definition
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t agc_warning:1; 
        uint16_t maghalf:1; 
        uint16_t p2ram_warning:1; 
        uint16_t p2ram_error:1; 
        uint16_t framing_error:1; 
        uint16_t command_error:1; 
        uint16_t crc_error:1;
        uint16_t wdtst:1; 
        uint16_t brkhall:1;
        uint16_t off_comp_not_finished:1;
        uint16_t cordic_overflow:1;
        uint16_t unused:5; 
    } values;
} Errfl;

// PROG Register Definition
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t progen:1;
        uint16_t unused:1;
        uint16_t otpref:1;
        uint16_t progotp:1;
        uint16_t unused1:2;
        uint16_t progver:1;
        uint16_t unused2:9;
    } values;
} Prog;

// DIA Register Definition
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t vdd_mode:1;
        uint16_t loops_finished:1;
        uint16_t cordic_overflow:1;
        uint16_t comp_l:1;
        uint16_t comp_h:1;
        uint16_t maghalf_flag:1;
        uint16_t cosoff_fin:1;
        uint16_t sinoff_fin:1;
        uint16_t finished:1;
        uint16_t agc_finished:1;
        uint16_t fusa_error:1;
        uint16_t spi_cnt:2;
        uint16_t unused:3; 
    } values;
} Dia;


// AGC Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t agc;
    } values;
} Agc;


// SIN DATA Register Defination 
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t sin_data:14;
        uint16_t unused:2;
    } values;
} Sin_data;

// COS DATA Register Defination 
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t cos_data:14;
        uint16_t unused:2;
    } values;
} Cos_data;

// VELOCITY Register Defination 
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t velocity:14;
        uint16_t unused:2;
    } values;
} Velocity;

// MAG Register Definition
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t cmag:14;
        uint16_t unused:2;
    } values;
} Mag;

// ANGLEUNC Register Definition
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t angleunc:14;
        uint16_t unused:2;
    } values;
} Angleunc;

// ANGLECOM Register Definition
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t anglecom:14;
        uint16_t unused:2;
    } values;
} Anglecom;

// ECC CHECKSUM Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t eec_s:7;
        uint8_t unused:1;
    } values;
} Ecc_checksum;


// DISABLE Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t uvw_off:1;
        uint8_t filter_disable:1;
        uint8_t brkhall_set:4;
        uint8_t abi_off:1;
    } values;
} Disable;

// ZPOSM Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t zposm;
    } values;
} Zposm;

// ZPOSL Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t zposl:6;
        uint8_t dia1_en:1;
        uint8_t dia2_en:1;
    } values;
} Zposl;


// SETTINGS1 Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t k_max:3;
        uint8_t k_min:3;
        uint8_t dia3_en:1;
    } values;
} Settings1;



// SETTINGS2 Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t iwidth:1;
        uint8_t noiseset:1;
        uint8_t dir:1;
        uint8_t uvw_abi:1;
        uint8_t daecdis:1;
        uint8_t abi_dec:1;
        uint8_t dataselect:1;
        uint8_t pwmon:1;
    } values;
} Settings2;


// SETTINGS3 Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t uvwpp:3;
        uint8_t hys:2;
        uint8_t abires:3;
    } values;
} Settings3;


// ECC Register Definition
typedef union {
    uint8_t raw;
    struct __attribute__ ((packed)) {
        uint8_t ecc_chsum:7;
        uint8_t ecc_en:1;
    } values;
} Ecc;


// Command Frame (16 bit)  Definition
// Only read operation 
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t commandFrame:14;
        uint16_t rw:1;
        uint16_t dnc:1;
    } values;
} CommandFrame16;

// ReadData Frame (16 bit)  Definition
// Only read operation 
typedef union {
    uint16_t raw;
    struct __attribute__ ((packed)) {
        uint16_t data:14;
        uint16_t err:1; 
        uint16_t warning:1; 
    } values;
} ReadDataFrame16;




// attribute is set to packed so that there is no padding 
typedef struct __attribute__((packed)) { 
    uint32_t crc:8; 
    uint32_t commandFrame:14;
    uint32_t rw:1;
    uint32_t dnc:1;
    uint32_t pad:8; 
} commandFrame_t ; 

// ============== CONVERT REQUEST DATA TO BYTE ARRAY =========================== 
typedef union {
    commandFrame_t commandFrame;
    uint32_t value; 
    byte CommandFramePacket[sizeof(commandFrame_t)];
} CommandFramePacket_t;


typedef union ArrayToInteger {
    byte array[4];
    uint32_t integer;
};


// // Command Frame  Definition
// typedef union {
//     uint32_t raw;
//     struct __attribute__ ((packed)) {
//         uint32_t crc:8; 
//         uint32_t commandFrame:14;
//         uint32_t rw:1;
//         uint32_t dnc:1;
//         uint32_t pad:8; 
//     } values;
// } CommandFrame;

// ReadData Frame  Definition
typedef union {
    uint32_t raw;
    struct __attribute__ ((packed)) {
        uint32_t pad:8;
        uint32_t crc:8; 
        uint32_t data:14;
        uint32_t err:1; 
        uint32_t warning:1; 
    } values;
} ReadDataFrame;

// // WriteData Frame  Definition
// typedef union {
//     uint32_t raw;
//     struct __attribute__ ((packed)) {
//         uint32_t pad:8;
//         uint32_t crc:8; 
//         uint32_t data:14;
//         uint32_t er:2; 
//     } values;
// } WriteDataFrame;

class AS5247 {
    public:
        AS5247(uint8_t chipSelectPin);

        ReadDataFrame16 readRegister16(uint16_t registerAddress);
        ReadDataFrame readRegister(uint16_t registerAddress);
        // void writeRegister(uint16_t registerAddress, uint16_t registerValue);
        void printDebugString();

    private:
        bool isEven(uint16_t data);

        // added by me 
        uint8_t CRC8(byte data[], int size);
        
        AS5247Spi spi;

};

#endif // #AS5247_h

