//This is the header file for the Arduino Code to control an AD9910 DDS chip. 
//It contains, or should be edited to contain all functions needed to operate the AD9910 using an Arduino Uno.
//This should be accompanied by the AD9910.cpp file, which serves as the library
//Based on code written by A-suozhang AD9910-Booster-Arduino - can be found on github "https://github.com/A-suozhang/AD9910-Booster-Arduino"

#pragma once //Makes sure this file is included only once in  a single compilation.

# define uchar unsigned char //specifying shorthand for unsigned charater
# define CLOCKSPEED  25000000//Clockspeed used for SPI serial communication between arduino and AD9910... setting to 15 MHz... James had it at 25 MHz - should check
# define FTWConst 8.589934592 // constant needed to calculate frequency tuning word using 1 GHz clock "fref" divided by 2 into 500 MHz "fsysclock"
# define TTWConst 125000000 // constant used to calculate time step. equal to fsysclock/4 or 500 MHz/ 4 = 125 MHz.
# define DDSCLOCK 500000000


# include <SPI.h> //SPI interface that arduino and other microcontrollers use to communicate with slave devices
# include <stdint.h>

class AD9910
{
public:
	int _cs, _rst, _update, _sdio, _sclk, _mrst, _sTrig; //slave selection pin (pin 10, should be low for slave you are writing to), reset, update, serial data input output , system clock , master reset, sweep trigger// can probably be private....
	////Control and general settings registers
	//extern uint8_t cfr1[4];
	//extern uint8_t cfr2[4];
	//extern uint8_t cfr3[4];
	//extern uint8_t DAC_config[4];
	//extern uint8_t Profile0[8];
	////Sweep registers
	//extern uint8_t SweepLimits[8]; //First 4 bytes make upper limit FTW, last 4 lower limit FTW. 
	//extern uint8_t FreqStepSize[8]; //First 4 bytes make decrement size FTW, last 4 increment size FTW.
	//extern uint8_t TimeStepSize[4]; // First 2 bytes are decrement time step tuning word, last 2 increment time step tuning word.

	////Amplitude Scale Factor Register 0x09
	//extern uint8_t ASF[4];

//Constructor
	AD9910(int cs, int rst, int update, int sdio, int sclk, int mrst, int sTrig); //this defines which pins are which via integer corresponding to a pin on the arduino board

	//Init IO

	void begin();

	//WE SHOULD PLAY WITH REMOVING THE DELAYS AT SOME POINT

	//Define IO reset function
	//Takes reset pin high then low - using delay inbetween that is shorter than what James used

	void master_reset();

	void reset();

	//Define update function.
	//High on update pin transfers the contents of the I/O buffers in the 9910 to the proper internal registers

	void update();

	//Define a standard SPI write function

	void SPI_Write_Reg(uint8_t addr, uint8_t bytes[], uint8_t num_bytes); //addr corresponds to the serial addresses or register names given in the AD9910 manual, just use the hex to designate what register you want to change
	//Function to initialize the AD9910 with standard settings

	void initialize();

	//Function to set frequency for a profile in single frequency mode

	void set_freq(unsigned long freq, uint8_t profile = 0); //default profile set to 0. freq as a double allows for 64 bit precision // put frequency in Hz

	//A function we may need, that preps a profile to be rewritten, but does not send an update command.  

	void prep_freq(unsigned long freq, uint8_t profile = 0);

	//Function to set amplitude for a profile in single frequency mode

	void set_Amp(double amp, uint8_t profile = 0);

	//Function to swap to singleFreqMode - it may be that we want to do a separate update if we want to prep this before firing

	void singleFreqMode();

	//Function to swap to a frequency sweep - will assume parameters are already set
	//mode 0 = default mode
	//mode 1 = no dwell high
	//mode 2 = no dwell low
	//mode 3 = oscillatory

	void freqSweepMode(int mode);

	//Function to swap to an amplitude sweep

	void ampSweepMode();

	//Function to swap to a phase sweep

	void phaseSweepMode();

	//Allow for frequency sweep paramters to be changed independently of turning the sweep on?
	//Do global amplitude/phase settings apply here?

	void freqSweepParameters(unsigned long ULim, unsigned long LLim, unsigned long stepsizeDown, unsigned long stepsizeUp, unsigned long timeStepDown, unsigned long timeStepUp);

	void rampReset();

	//OSK enable function
	//mode 0 = manual
	//mode 1 = auto
	//in either case, OSK pin is enabled. To disable it, one needs to turn off bit 23 in CFR1. We want it on for everything in the lab so far.
	//I believe that it will always be on for Automatic mode.
	void OSKenable(int mode = 0);

	void OSKdisable();

	void setAmpScaleFactor(double amplitude); //Still needs to be tested

	void setOSKRampRate(); //only needed for automatic mode
};
