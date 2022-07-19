


/** @file AS5247.cpp
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

#include "AS5247.h"
#define bitframe16

AS5247::AS5247(uint8_t chipSelectPin) : spi(chipSelectPin) {
}

ReadDataFrame16 AS5247::readRegister16(uint16_t registerAddress) {
	CommandFrame16 command;
	command.values.rw = READ;
	command.values.commandFrame = registerAddress;

	CommandFrame16 nopCommand;
	nopCommand.values.rw = READ;
	nopCommand.values.commandFrame = NOP_REG;

	ReadDataFrame16 receivedFrame;
	receivedFrame.raw = spi.readData16(command.raw, nopCommand.raw);
	return receivedFrame;
}

ReadDataFrame AS5247::readRegister(uint16_t registerAddress) {
	
	CommandFramePacket_t commandFramePacket;  // create an instance 
	commandFramePacket.commandFrame.rw = READ;
	commandFramePacket.commandFrame.address = registerAddress;

	
	// Serial.print("value from union before crc: "); 
	// Serial.println(commandFramePacket.value, HEX); 

	byte bytesCommandRegister[2]; 
    for (int i = 2; i >= 1; i--){
		// Serial.print("byte sample: "); 
		// Serial.println(commandFramePacket.CommandFramePacket[i], HEX); 
        bytesCommandRegister[2-i] = commandFramePacket.CommandFramePacket[i]; 
    }

	commandFramePacket.commandFrame.crc = CRC8(bytesCommandRegister, 2);	

	CommandFramePacket_t nopCommandFramePacket; 
	nopCommandFramePacket.commandFrame.rw = READ;
	nopCommandFramePacket.commandFrame.address = NOP_REG;

	byte bytesNopCommandRegister[2]; 
    for (int i = 1; i < 3; i++){
        bytesNopCommandRegister[i - 1] = nopCommandFramePacket.CommandFramePacket[i]; 
    }
	nopCommandFramePacket.commandFrame.crc = CRC8(bytesNopCommandRegister, 2); 

	// CommandFrame16 nopCommand;
	// nopCommand.values.rw = READ;
	// nopCommand.values.commandFrame = NOP_REG;

	// ReadDataFrame16 receivedFrame;

	ReadDataFrame receivedFrame;
	// receivedFrame.raw = spi.readData(commandFramePacket.CommandFramePacket, nopCommandFramePacket.CommandFramePacket);
	receivedFrame.raw = spi.readData(commandFramePacket.value, nopCommandFramePacket.value);

	return receivedFrame;
}


void AS5247::writeRegister(uint16_t registerAddress, uint16_t registerValue) {
	CommandFramePacket_t commandFramePacket;
	commandFramePacket.commandFrame.rw = WRITE;
	commandFramePacket.commandFrame.address = registerAddress;

	byte bytesCommandRegister[2]; 

    for (int i = 2; i >= 1; i--){
		// Serial.print("byte sample: "); 
		// Serial.println(commandFramePacket.CommandFramePacket[i], HEX); 
        bytesCommandRegister[2-i] = commandFramePacket.CommandFramePacket[i]; 
    }

	commandFramePacket.commandFrame.crc = CRC8(bytesCommandRegister, 2);	

	WriteDataPacket_t writeDataPacket;
	writeDataPacket.writeData.data = registerValue;

	byte bytesWriteRegister[2]; 
    for (int i = 3; i >= 2; i--){
		// Serial.print("byte sample: "); 
		// Serial.println(writeDataPacket.WriteDataPacket[i], HEX); 
        bytesWriteRegister[3-i] = writeDataPacket.WriteDataPacket[i]; 
    }

	writeDataPacket.writeData.crc = CRC8(bytesWriteRegister, 2);
	spi.writeData(commandFramePacket.value, writeDataPacket.value);

}


void AS5247::printDebugString() {

	#ifdef bitframe16
		ReadDataFrame16 readDataFrame;
		readDataFrame = readRegister16(ERRFL_REG);
	#else
		ReadDataFrame readDataFrame;
		readDataFrame = readRegister(ERRFL_REG);
	#endif 
	Errfl errfl;
	errfl.raw = readDataFrame.values.data;
	Serial.println("======== AS5X47 Debug ========");
	Serial.println("------- ERRFL Register: ");
	
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   AGC_WARNING: ");
	Serial.println(errfl.values.agc_warning);
	Serial.print("|   MAGHALF: ");
	Serial.println(errfl.values.maghalf);
	Serial.print("|   P2RAM_WARNING: ");
	Serial.println(errfl.values.p2ram_warning);
	Serial.print("|   P2RAM_ERROR: ");
	Serial.println(errfl.values.p2ram_error);
	Serial.print("|   FRAMING_ERROR: ");
	Serial.println(errfl.values.framing_error);
	Serial.print("|   COMMAND_ERROR: ");
	Serial.println(errfl.values.command_error);
	Serial.print("|   CRC_ERROR: ");
	Serial.println(errfl.values.crc_error);
	Serial.print("|   WDTST: ");
	Serial.println(errfl.values.wdtst);
	Serial.print("|   BRKHALL: ");
	Serial.println(errfl.values.brkhall);
	Serial.print("|   OFF_COMP_NOT_FINISHED: ");
	Serial.println(errfl.values.off_comp_not_finished);
	Serial.print("|   CORDIC_OVERFLOW: ");
	Serial.println(errfl.values.cordic_overflow);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(PROG_REG);
	#else
		readDataFrame = readRegister(PROG_REG);
	#endif 
	Prog prog;
	prog.raw = readDataFrame.values.data;
	Serial.println("------- PROG Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   PROGEN: ");
	Serial.println(prog.values.progen);
	Serial.print("|   OTPREF: ");
	Serial.println(prog.values.otpref);
	Serial.print("|   PROGOTP: ");
	Serial.println(prog.values.progotp);
	Serial.print("|   PROVER: ");
	Serial.println(prog.values.progver);
	Serial.println("|");


	#ifdef bitframe16
		readDataFrame = readRegister16(DIA_REG);
	#else
		readDataFrame = readRegister(DIA_REG);
	#endif 
	Dia dia;
	dia.raw = readDataFrame.values.data;
	Serial.println("------- DIA Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   VDDMODE: ");
	Serial.println(dia.values.vdd_mode);
	Serial.print("|   LOOPSFINISHED: ");
	Serial.println(dia.values.loops_finished);
	Serial.print("|   CORDIC_OVERFLOW: ");
	Serial.println(dia.values.cordic_overflow);
	Serial.print("|   COML: ");
	Serial.println(dia.values.comp_l);
	Serial.print("|   COMH: ");
	Serial.println(dia.values.comp_h);
	Serial.print("|   MAGHALF_FLAG: ");
	Serial.println(dia.values.maghalf_flag);
	Serial.print("|   COSOFF_FIN: ");
	Serial.println(dia.values.cosoff_fin);
	Serial.print("|   SINOFF_FIN: ");
	Serial.println(dia.values.sinoff_fin);
	Serial.print("|   FINISHED: ");
	Serial.println(dia.values.finished);
	Serial.print("|   AGC_FINISHED: ");
	Serial.println(dia.values.agc_finished);
	Serial.print("|   FUSA_ERROR: ");
	Serial.println(dia.values.fusa_error);
	Serial.print("|   SPI_CNT: ");
	Serial.println(dia.values.spi_cnt);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(AGC_REG);
	#else
		readDataFrame = readRegister(AGC_REG);
	#endif 
	Agc agc;
	agc.raw = readDataFrame.values.data;
	Serial.println("------- AGC Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   AGC: ");
	Serial.println(agc.values.agc);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(SIN_DATA_REG);
	#else
		readDataFrame = readRegister(SIN_DATA_REG);
	#endif 
	Sin_data sin_data;
	sin_data.raw = readDataFrame.values.data;
	Serial.println("------- SIN DATA Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   SIN DATA: ");
	Serial.println(sin_data.values.sin_data);
	Serial.println("|");


	#ifdef bitframe16
		readDataFrame = readRegister16(COS_DATA_REG);
	#else
		readDataFrame = readRegister(COS_DATA_REG);
	#endif 
	Cos_data cos_data;
	cos_data.raw = readDataFrame.values.data;
	Serial.println("------- COS DATA Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   COS DATA: ");
	Serial.println(cos_data.values.cos_data);
	Serial.println("|");


	#ifdef bitframe16
		readDataFrame = readRegister16(VEL_REG);
	#else
		readDataFrame = readRegister(VEL_REG);
	#endif 
	Velocity velocity;
	velocity.raw = readDataFrame.values.data;
	Serial.println("------- VELOCITY Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   VELOCITY: ");
	Serial.println(velocity.values.velocity);
	Serial.println("|");


	#ifdef bitframe16
		readDataFrame = readRegister16(MAG_REG);
	#else
		readDataFrame = readRegister(MAG_REG);
	#endif 
	Mag mag;
	mag.raw = readDataFrame.values.data;
	Serial.println("|------- MAG Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   CMAG: ");
	Serial.println(mag.values.cmag);
	Serial.println("|");


	#ifdef bitframe16
		readDataFrame = readRegister16(ANGLEUNC_REG);
	#else
		readDataFrame = readRegister(ANGLEUNC_REG);
	#endif 
	Angleunc angleunc;
	angleunc.raw = readDataFrame.values.data;
	Serial.println("|------- ANGLEUNC Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   ANGLEUNC: ");
	Serial.println(angleunc.values.angleunc);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(ANGLECOM_REG);
	#else
		readDataFrame = readRegister(ANGLECOM_REG);
	#endif
	Anglecom anglecom;
	anglecom.raw = readDataFrame.values.data;
	Serial.println("|------- ANGLEUNC Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   ANGLECOM: ");
	Serial.println(anglecom.values.anglecom);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(ECC_CHECKSUM_REG);
	#else
		readDataFrame = readRegister(ECC_CHECKSUM_REG);
	#endif
	Ecc_checksum ecc_checksum;
	ecc_checksum.raw = readDataFrame.values.data;
	Serial.println("|------- ECC CHECKSUM Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   CHECKSUM: ");
	Serial.println(ecc_checksum.values.eec_s);
	Serial.println("|");


	#ifdef bitframe16
		readDataFrame = readRegister16(DISABLE_REG);
	#else
		readDataFrame = readRegister(DISABLE_REG);
	#endif
	Disable disable;
	disable.raw = readDataFrame.values.data;
	Serial.println("|------- DISABLE Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   UVW_OFF: ");
	Serial.println(disable.values.uvw_off);
	Serial.print("|   FILTER_DISABLE: ");
	Serial.println(disable.values.filter_disable);
	Serial.print("|   BRKHALL_SET: ");
	Serial.println(disable.values.brkhall_set);
	Serial.print("|   ABI_OFF: ");
	Serial.println(disable.values.abi_off);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(ZPOSM_REG);
	#else
		readDataFrame = readRegister(ZPOSM_REG);
	#endif
	Zposm zposm;
	zposm.raw = readDataFrame.values.data;
	Serial.println("|------- ZPOSM Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   ZPOSM: ");
	Serial.println(zposm.values.zposm);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(ZPOSL_REG);
	#else
		readDataFrame = readRegister(ZPOSL_REG);
	#endif
	Zposl zposl;
	zposl.raw = readDataFrame.values.data;
	Serial.println("|------- ZPOSL Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   ZPOSL: ");
	Serial.println(zposl.values.zposl);
	Serial.print("|   DIA1_EN: ");
	Serial.println(zposl.values.dia1_en);
	Serial.print("|   DIA2_EN: ");
	Serial.println(zposl.values.dia2_en);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(SETTINGS1_REG);
	#else
		readDataFrame = readRegister(SETTINGS1_REG);
	#endif
	Settings1 settings1;
	settings1.raw = readDataFrame.values.data;
	Serial.println("|------- SETTINGS2 Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   K_MAX: ");
	Serial.println(settings1.values.k_max);
	Serial.print("|   K_MIN: ");
	Serial.println(settings1.values.k_min);
	Serial.print("|   DIA3_EN: ");
	Serial.println(settings1.values.dia3_en);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(SETTINGS2_REG);
	#else
		readDataFrame = readRegister(SETTINGS2_REG);
	#endif
	Settings2 settings2;
	settings2.raw = readDataFrame.values.data;
	Serial.println("|------- SETTINGS2 Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   IWIDTH: ");
	Serial.println(settings2.values.iwidth);
	Serial.print("|   NOISESET: ");
	Serial.println(settings2.values.noiseset);
	Serial.print("|   DIR: ");
	Serial.println(settings2.values.dir);
	Serial.print("|   UVW_ABI: ");
	Serial.println(settings2.values.uvw_abi);
	Serial.print("|   DAECDIS: ");
	Serial.println(settings2.values.daecdis);
	Serial.print("|   ABI_DEC: ");
	Serial.println(settings2.values.abi_dec);
	Serial.print("|   DATASELECT: ");
	Serial.println(settings2.values.dataselect);
	Serial.print("|   PWMON: ");
	Serial.println(settings2.values.pwmon);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(SETTINGS3_REG);
	#else
		readDataFrame = readRegister(SETTINGS3_REG);
	#endif
	Settings3 settings3;
	settings3.raw = readDataFrame.values.data;
	Serial.println("|------- SETTINGS3 Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   UVWPP: ");
	Serial.println(settings3.values.uvwpp);
	Serial.print("|   HYS: ");
	Serial.println(settings3.values.hys);
	Serial.print("|   ABIRES: ");
	Serial.println(settings3.values.abires);
	Serial.println("|");

	#ifdef bitframe16
		readDataFrame = readRegister16(ECC_REG);
	#else
		readDataFrame = readRegister(ECC_REG);
	#endif
	Ecc ecc;
	ecc.raw = readDataFrame.values.data;
	Serial.println("|------- ECC Register: ");
	Serial.print("|   Warning: ");
	Serial.println(readDataFrame.values.warning);
	Serial.print("|   Reading Error: ");
	Serial.println(readDataFrame.values.err);

	#ifndef bitframe16
	Serial.print("|   CRC: ");
	Serial.println(readDataFrame.values.crc, HEX);
	#endif

	Serial.print("|   ECC_CHSUM: ");
	Serial.println(ecc.values.ecc_chsum);
	Serial.print("|   ECC_EN: ");
	Serial.println(ecc.values.ecc_en);
	Serial.println("|");
	Serial.println("==============================");

}


uint8_t AS5247::CRC8(byte data [], int size){
	

	// Polynomial 
    const byte polynomial = 0x1D;
    // initial value 
    byte crc = 0xC4;  

    // get the size of byte array 
    // size_t size = sizeof(data)/(sizeof(data[0])); 
    // Serial.print("size: "); 
    // Serial.println(size); 

    // calculate crc 
    for(int i = 0; i < size; i++){
        crc ^= data[i]; 
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

	Serial.print("CRC Value: "); 
	Serial.println(crc, HEX); 
    return crc; 

}

bool AS5247::isEven(uint16_t data) {
	int count=0;
	unsigned int b = 1;
	for (unsigned int i=0; i<15; i++) {
		if (data & (b << i)) {
			count++;
		}
	}
	
	if (count%2==0) {
		return false;

	} else {
		return true;
	}
}
