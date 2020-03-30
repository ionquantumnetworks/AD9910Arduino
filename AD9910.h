//This is the header file for the Arduino Code to control an AD9910 DDS chip. 
//It contains, or should be edited to contain all functions needed to operate the AD9910 using an Arduino Uno.
//This code is heavily based on code written by A-suozhang
#pragma once //Makes sure this file is included only once in  a single compilation.

# define uchar unsigned char //specifying shorthand for unsigned charater
# define CLOCKSPEED  25000000//Clockspeed used for SPI serial communication between arduino and AD9910... setting to 15 MHz... James had it at 25 MHz - should check
# define FTWConst 8.589934592 // constant needed to calculate frequency tuning word using 1 GHz clock "fref" divided by 2 into 500 MHz "fsysclock"
# define TTWConst 125000000 // constant used to calculate time step. equal to fsysclock/4 or 500 MHz/ 4 = 125 MHz.

# include <SPI.h> //SPI interface that arduino and other microcontrollers use to communicate with slave devices
# include <stdint.h>

class AD9910
{
public:
	int _cs, _rst, _update, _sdio, _sclk, _mrst, _sTrig; //slave selection pin (pin 10, should be low for slave you are writing to), reset, update, serial data input output , system clock , master reset, sweep trigger
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
//Constructor
	AD9910(int cs, int rst, int update, int sdio, int sclk, int mrst, int sTrig) //this defines which pins are which via integer corresponding to a pin on the arduino board
	{
		_cs = cs;
		_rst = rst;
		_update = update;
		_sdio = sdio; //Check if we want this unidirectional or bidirectional
		_sclk = sclk;
		_mrst = mrst;
		_sTrig = sTrig;
	}

	//Init IO

	void begin()
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
	}

	//WE SHOULD PLAY WITH REMOVING THE DELAYS AT SOME POINT

	//Define IO reset function
	//Takes reset pin high then low - using delay inbetween that is shorter than what James used

	void master_reset()
	{
		digitalWrite(_mrst, HIGH);
		delay(5);
		digitalWrite(_mrst, LOW);
		delay(5);
	}

	void reset()
	{
		digitalWrite(_rst, HIGH);
		delay(5);
		digitalWrite(_rst, LOW);
	}

	//Define update function.
	//High on update pin transfers the contents of the I/O buffers in the 9910 to the proper internal registers

	void update()
	{
		digitalWrite(_update, HIGH);
		delay(5);
		digitalWrite(_update, LOW);
	}

	//Define a standard SPI write function

	void SPI_Write_Reg(uint8_t addr, uint8_t bytes[], uint8_t num_bytes) //addr corresponds to the serial addresses or register names given in the AD9910 manual, just use the hex to designate what register you want to change
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

	//Function to initialize the AD9910 with standard settings

	void initialize()
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

	//Function to set frequency for a profile in single frequency mode

	void set_freq(double freq, uint8_t profile = 0) //default profile set to 0. freq as a double allows for 64 bit precision // put frequency in Hz
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

	//A function we may need, that preps a profile to be rewritten, but does not send an update command.  

	void prep_freq(double freq, uint8_t profile = 0)
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

	//Function to set amplitude for a profile in single frequency mode

	void set_Amp(double amp, uint8_t profile = 0)
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

	//Function to swap to singleFreqMode - it may be that we want to do a separate update if we want to prep this before firing

	void singleFreqMode(){}

	//Function to swap to a frequency sweep - will assume parameters are already set
	//mode 0 = default mode
	//mode 1 = no dwell high
	//mode 2 = no dwell low
	//mode 3 = oscillatory

	void freqSweepMode(int mode = 0) 
	{
		if (mode = 0) 
		{
			cfr2[2] = 0x08;
		}
		else if (mode = 1)
		{
			cfr2[2] = 0x0C;
		}
		else if (mode = 2)
		{
			cfr2[2] = 0x0A;
		}
		else if (mode = 3)
		{
			cfr2[2] = 0x0E;
		}
		else
		{
			Serial.println("Invalid mode selection, will go to default sweep mode.");
			cfr2[2] = 0x08;
		}

		SPI_Write_Reg(0x01, cfr2, 4);
		update();

	}

	//Function to swap to an amplitude sweep

	void ampSweepMode(){}

	//Function to swap to a phase sweep

	void phaseSweepMode(){}

	//Allow for frequency sweep paramters to be changed independently of turning the sweep on?
	//Do global amplitude/phase settings apply here?

	void freqSweepParameters(double ULim, double LLim, double stepsizeDown, double stepsizeUp, double timeStepDown, double timeStepUp)
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
	}

	void rampReset()
	{
		cfr1[3] = 0x30;
		SPI_Write_Reg(0x00, cfr1, 4);
		update();
		cfr1[3] = 0x20;
		SPI_Write_Reg(0x00, cfr1, 4);
		update();
	} //Reset ramp using digiatl ramp accumluator reset set bit 12 of CFR1 to 1 then do I/O update, then set back to 0 and I/O update
};

//For Frequency Sweep, functions that need to change: 
// -initialize
// -optional trigger pin (5) -- done in header
// -potentially set_freq and set_amp need to be edited to do nothing if sweep mode is enabled. It is possible you can still set profiles even while in sweep mode... I will check via trial and error
// hmm maybe I should make functions to turn on/off sweep mode or single frequency mode. such as singleFreqMode() and sweepMode(). sweepMode() can contain variables having to do with certain registers such as OSK.. I think that is tomorrow's job
// We may want the arduino to connect to the RF switches when applicable