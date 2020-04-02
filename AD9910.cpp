//Library file for AD9910 Arduino
//Based on code written by A-suozhang AD9910-Booster-Arduino - can be found on github "https://github.com/A-suozhang/AD9910-Booster-Arduino"

#include <SPI.h>
#include <stdint.h>
#include "AD9910.h"




//Defining Control Register Default Values and Initilizing size
//Control and general settings registers
uint8_t cfr1[4] = { 0x00, 0x40, 0x20, 0x00 }; //what may be causing unwated jumps in signal is the 0x20 which makes the phase accumlator reset on updates. Define Control Function Register 1 default values as originally figured out by James - see page 49/64 and 54/64 of AD9910 manual
uint8_t cfr2[4] = { 0x01, 0x00, 0x08, 0x20 }; //Define Control Function Register 2 default values as originally figured out by James
uint8_t cfr3[4] = { 0x1F, 0x3F, 0x40, 0x00 }; //Define Control Function Register 3 default values as originally figured out by James
uint8_t DAC_config[4] = { 0x00, 0x00, 0x00, 0x7F }; //Onboard Auxillary DAC control register, default is 0x7F
//Single Frequency Register
uint8_t Profile0[8] = { 0x08, 0xB5, 0x00, 0x00, 0x14, 0x7A, 0xE1, 0x48 }; //Single Frequency or RAM profile, depending on control register settings. Bytes organized as :{amplitude, amplitude, phase, phase, freq, freq, freq ,freq}
//Sweep registers
uint8_t SweepLimits[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //First 4 bytes make upper limit FTW, last 4 lower limit FTW. 
uint8_t FreqStepSize[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; //First 4 bytes make decrement size FTW, last 4 increment size FTW.
uint8_t TimeStepSize[4] = { 0x00, 0x00, 0x00, 0x00 }; // First 2 bytes are decrement time step tuning word, last 2 increment time step tuning word.


AD9910::AD9910(int cs, int rst, int update, int sdio, int sclk, int mrst, int sTrig) //this defines which pins are which via integer corresponding to pin number on the arduino board
{
	_cs = cs;
	_rst = rst;
	_update = update;
	_sdio = sdio; //Check if we want this unidirectional or bidirectional
	_sclk = sclk;
	_mrst = mrst;
	_sTrig = sTrig;
}

void AD9910::begin()
{
	//Set up the pins that will be used for commmunication between the arduino and the AD9910
	pinMode(_cs, OUTPUT);
	pinMode(_rst, OUTPUT);
	pinMode(_update, OUTPUT);
	pinMode(_sdio, OUTPUT);
	pinMode(_sclk, OUTPUT);
	pinMode(_mrst, OUTPUT);
	pinMode(_sTrig, OUTPUT);

	//Set pins High or Low
	digitalWrite(_sTrig, LOW);
	digitalWrite(_cs, HIGH);
	digitalWrite(_rst, LOW);
	digitalWrite(_update, LOW);
	digitalWrite(_mrst, LOW);
	delay(5);
	master_reset();
	delay(5);
	reset();
	delay(5);
	initialize();
	delay(5);
	//WE SHOULD PLAY WITH REMOVING THE DELAYS AT SOME POINT
}

void AD9910::master_reset()
{
	digitalWrite(_mrst, HIGH);
	delay(5);
	digitalWrite(_mrst, LOW);
	delay(5);
}

void AD9910::reset()
{
	digitalWrite(_rst, HIGH);
	delay(5);
	digitalWrite(_rst, LOW);
}

void AD9910::update()
{
	//High on update pin transfers the contents of the I/O buffers in the 9910 to the proper internal registers
	digitalWrite(_update, HIGH);
	delay(5);
	digitalWrite(_update, LOW);
}

void AD9910::SPI_Write_Reg(uint8_t addr, uint8_t bytes[], uint8_t num_bytes) //addr corresponds to the serial addresses or register names given in the AD9910 manual, just use the hex to designate what register you want to change
{
	SPI.beginTransaction(SPISettings(CLOCKSPEED, MSBFIRST, SPI_MODE0)); //code from xxx uses mode0, James used MODE 2 - should check 
	digitalWrite(_cs, LOW);
	SPI.transfer(addr); //First byte of transfer selects which register we are writing to
	delay(5); //potentially optional delay
	for (int i = 0; i < num_bytes; i++) {//for loop that sends the bytes from the byte array bytes[] to the SPI device
		SPI.transfer(bytes[i]);
	}
	digitalWrite(_cs, HIGH); //Deselects the AD9910 as the active slave
	SPI.endTransaction(); //Closes SPI transaction. This is important to do if any other libraries want to use SPI. 
}

void AD9910::initialize()
{
	//Load standard control function registers - this will need to potentially change when we want to do frequency sweeps
	SPI_Write_Reg(0x00, cfr1, 4); //0x00 corresponds to cfr1 address, similar notation used in the next few lines
	delay(5);
	SPI_Write_Reg(0x01, cfr2, 4);
	delay(5);
	SPI_Write_Reg(0x02, cfr3, 4);
	delay(5);
	SPI_Write_Reg(0x03, DAC_config, 4); //James doesn't have this in his code.. not sure how necessary
	delay(5);
	update(); //transfer from buffer to internal register
	delay(5);
}

void AD9910::set_freq(double freq, uint8_t profile) //default profile set to 0. freq as a double allows for 64 bit precision // put frequency in Hz
{
	if (profile > 7) { //protection against a impossible profile number
		Serial.println("Invalid Profile Number.");
		return;
	}
	unsigned long temp; //temp variable for frequency calculations
	if (freq > 200000000) {//protection against too big of a frequency - setting to 200 MHz for now - AOM designed for 40 MHz anyway
		freq = 200000000;
	}
	temp = freq * FTWConst;//8.589934592;//4.294967296; //uses our clock frequency of 1 GHz with a divider of 2, and includes 2^32

	Profile0[7] = (uchar)temp; //uchar will only ever take the last byte of a number's binary representation. We then need to take our frequency tuning word, temp, in byte sized steps before sending to AD9910 via SPI
	Profile0[6] = (uchar)(temp >> 8); //shifts binary representation 8 bits to the right, or one byte, and we then take the last byte to send to AD9910
	Profile0[5] = (uchar)(temp >> 16);
	Profile0[4] = (uchar)(temp >> 24);

	SPI_Write_Reg(0x0E + profile, Profile0, 8);
	update();
}

void AD9910::prep_freq(double freq, uint8_t profile)
{
	if (profile > 7) { //protection against a impossible profile number.. need to make this work for negative or invalid data types as well..
		Serial.println("Invalid Profile Number.");
		return;
	}
	unsigned long temp; //temp variable for frequency calculations
	if (freq > 200000000) {//protection against too big of a frequency - setting to 200 MHz for now - AOM designed for 40 MHz anyway
		freq = 200000000;
	}
	temp = freq * FTWConst;//4.294967296; //uses our clock frequency of 1 GHz with a divider of 2, and includes 2^32

	Profile0[7] = (uchar)temp; //uchar will only ever take the last byte of a number's binary representation. We then need to take our frequency tuning word, temp, in byte sized steps before sending to AD9910 via SPI
	Profile0[6] = (uchar)(temp >> 8); //shifts binary representation 8 bits to the right, or one byte, and we then take the last byte to send to AD9910
	Profile0[5] = (uchar)(temp >> 16);
	Profile0[4] = (uchar)(temp >> 24);

	SPI_Write_Reg(0x0E + profile, Profile0, 8);
}

void AD9910::set_Amp(double amp, uint8_t profile) //Function to set amplitude for a profile in single frequency mode
{
	unsigned long temp;
	temp = (unsigned long)amp * 25.20615385; //amplitude tuning word.. need to check math and need to make this a global variable
	if (temp > 0x3FFF) {
		temp = 0x3FFF;
	}
	temp &= 0x3FFF;
	Profile0[0] = (uchar)(temp >> 8); //see set_freq for similar syntax
	Profile0[1] = (uchar)(temp);

	SPI_Write_Reg(0x0E + profile, Profile0, 8);
	update();
}

void AD9910::freqSweepMode(int mode)
{	//mode 0 = default mode
	//mode 1 = no dwell high
	//mode 2 = no dwell low
	//mode 3 = oscillatory
	if (mode == 0)
	{
		cfr2[1] = cfr2[1] | 0x08; //turn on bit 3
		cfr2[1] = cfr2[1] & 0xF9; //turn off bits 2 and 1
		//cfr2[1] = 0x08;//old way of rewriting entire register.
	}
	else if (mode == 1)
	{
		cfr2[1] = cfr2[1] | 0x0C; // turn on bits 3 and 2
		cfr2[1] = cfr2[1] & 0xFD; // turn off bit 1
		//cfr2[1] = 0x0C;//old way of rewriting entire register
	}
	else if (mode == 2)
	{
		cfr2[1] = cfr2[1] | 0x0A; //turn on bits 3 and 1
		cfr2[1] = cfr2[1] & 0xFB; //turn off bit 2
		//cfr2[1] = 0x0A;//old way of rewriting entire register
	}
	else if (mode == 3)
	{
		cfr2[1] = cfr2[1] | 0x0E; //turn on bits 3 2 and 1
		//cfr2[1] = 0x0E;// old way of rewriting entire register
	}
	else
	{
		Serial.println("Invalid mode selection, will go to default sweep mode.");
		cfr2[1] = cfr2[1] | 0x08; //turn on bit 3
		cfr2[1] = cfr2[1] & 0xF9; //turn off bits 2 and 1
		//cfr2[1] = 0x08;//old way of rewriting entire register.
	}

	SPI_Write_Reg(0x01, cfr2, 4);
	update();

}

void AD9910::freqSweepParameters(double ULim, double LLim, double stepsizeDown, double stepsizeUp, double timeStepDown, double timeStepUp)
{
	//Keep values within bounds defined by our reference clock divided by 2 (known as SYSCLOCK)
	if (ULim > 200000000 || ULim < 0)
	{
		ULim = 200000000;
		Serial.println("Upper Limit out of bounds, setting to 200 MHz");
	}
	if (LLim > 200000000 || LLim < 0)
	{
		LLim = 0;
		Serial.println("Lower Limit out of bounds, setting to 0 MHz");
	}
	if (stepsizeDown > 200000000)
	{
		stepsizeDown = 200000000;
		Serial.println("Step size down too large, setting to 200 MHz");
	}
	if (stepsizeUp > 200000000)
	{
		stepsizeUp = 200000000;
		Serial.println("Step size up too large, setting to 200 MHz");
	}
	if (stepsizeDown < 0.11641532182)
	{
		stepsizeDown = 0.11641532182;
		Serial.println("Step size down too small, setting to 0.11641532182 Hz");
	}
	if (stepsizeUp < 0.11641532182)
	{
		stepsizeUp = 0.11641532182;
		Serial.println("Step size up too small, setting to 0.11641532182 Hz");
	}
	if (timeStepDown > 0.000524288)
	{
		timeStepDown = 0.000524288;
		Serial.println("Decrement time step too small, setting to 524.288 us");
	}
	if (timeStepUp > 0.000524288)
	{
		timeStepUp = 0.000524288;
		Serial.println("Increment time step too small, setting to 524.288 us");
	}
	if (timeStepDown < 0.000000008)
	{
		timeStepDown = 0.000000008;
		Serial.println("Decrement time step too small, setting to 8 ns");
	}
	if (timeStepUp < 0.000000008)
	{
		timeStepUp = 0.000000008;
		Serial.println("Increment time step too small, setting to 8 ns");
	}
	if (LLim > ULim)
	{
		Serial.println("Lower limit is larger than upper limit... expect weird stuff.");
	}
	//calculation of tuning words using temp variables... separate variables for diagnostics.. could just redefine a single variable after each calculation
	//UL - upper limit, LL - lower limit, SD - stepsize down, SU - stepsize up, TD - timestep down, TU - timestep up
	unsigned long tempUL;
	unsigned long tempLL;
	unsigned long tempSD;
	unsigned long tempSU;
	unsigned long tempTD;
	unsigned long tempTU;
	//calcuation of tuning words
	//frequency
	tempUL = ULim * FTWConst;
	tempLL = LLim * FTWConst;
	tempSD = stepsizeDown * FTWConst;
	tempSU = stepsizeUp * FTWConst;
	//time
	tempTD = timeStepDown * TTWConst;
	tempTU = timeStepUp * TTWConst;
	/////
	//loading values into DDS
	SweepLimits[7] = (uchar)tempLL; //uchar will only ever take the last byte of a number's binary representation. We then need to take our frequency tuning word, temp, in byte sized steps before sending to AD9910 via SPI
	SweepLimits[6] = (uchar)(tempLL >> 8); //shifts binary representation 8 bits to the right, or one byte, and we then take the last byte to send to AD9910
	SweepLimits[5] = (uchar)(tempLL >> 16);
	SweepLimits[4] = (uchar)(tempLL >> 24);
	SweepLimits[3] = (uchar)tempUL;
	SweepLimits[2] = (uchar)(tempUL >> 8);
	SweepLimits[1] = (uchar)(tempUL >> 16);
	SweepLimits[0] = (uchar)(tempUL >> 24);

	FreqStepSize[7] = (uchar)tempSU;
	FreqStepSize[6] = (uchar)(tempSU >> 8);
	FreqStepSize[5] = (uchar)(tempSU >> 16);
	FreqStepSize[4] = (uchar)(tempSU >> 24);
	FreqStepSize[3] = (uchar)tempSD;
	FreqStepSize[2] = (uchar)(tempSD >> 8);
	FreqStepSize[1] = (uchar)(tempSD >> 16);
	FreqStepSize[0] = (uchar)(tempSD >> 24);

	TimeStepSize[3] = (uchar)tempTU;
	TimeStepSize[2] = (uchar)(tempTU >> 8);
	TimeStepSize[1] = (uchar)(tempTU >> 16);
	TimeStepSize[0] = (uchar)(tempTU >> 24);

	SPI_Write_Reg(0x0B, SweepLimits, 8);
	SPI_Write_Reg(0x0C, FreqStepSize, 8);
	SPI_Write_Reg(0x0D, TimeStepSize, 8);
	//update() may go here in the future
	update();
}

void AD9910::rampReset()
{
	cfr2[2] = cfr2[2] | 0x10;
	//cfr1[2] = 0x30;//Old way of rewriting entire register. Replace with turning on bit 4 only and leaving all else the same.
	SPI_Write_Reg(0x00, cfr1, 4);
	update();
	cfr2[2] = cfr2[2] & 0xEF;
	//cfr1[2] = 0x20;//Old way of rewriting register. Replace with turning off only bit 4 and leaving all else the same.
	SPI_Write_Reg(0x00, cfr1, 4);
	update();
} //Reset ramp using digiatl ramp accumluator reset set bit 12 of CFR1 to 1 then do I/O update, then set back to 0 and I/O update

void AD9910::OSKenable(int mode)
{	//OSK enable function
	//mode 0 = manual
	//mode 1 = auto
	if (mode == 0)
	{	//Turn on control register 1 bits 23 and 9, turn off bit 8
		cfr1[1] = cfr1[1] | 0x80;
		cfr1[2] = cfr1[2] | 0x02;
		cfr1[2] = cfr1[2] & 0xFE;
		SPI_Write_Reg(0x00, cfr1, 4);
		//for testing purposes only
		//Serial.print("Control Register 1[1] value: ");
		//Serial.println(cfr1[1], BIN);
		//Serial.print("Control Register 1[2] value: ");
		//Serial.println(cfr1[2], BIN);
	}
	else if (mode == 1)
	{	//turn on control register bits 9 and 8
		cfr1[2] = cfr1[2] | 0x03;
		SPI_Write_Reg(0x00, cfr1, 4);
		//for testing purposes only
		//Serial.print("Control Register 1[1] value: ");
		//Serial.println(cfr1[1], BIN);
		//Serial.print("Control Register 1[2] value: ");
		//Serial.println(cfr1[2], BIN);
	}
	else
	{
		Serial.println("Invalid Mode");
		return;
	}
	update();
}

void AD9910::OSKdisable()
{	//turn off control register 1 bit 9
	cfr1[2] = cfr1[2] & 0xFD;
	//for testing purposes only
	//Serial.print("Control Register 1[2] value: ");
	//Serial.println(cfr1[2],BIN);
	update();
}