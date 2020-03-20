//This is the header file for the Arduino Code to control an AD9910 DDS chip. 
//It contains, or should be edited to contain all functions needed to operate the AD9910 using an Arduino Uno.
//This code is heavily based on code written by A-suozhang
#pragma once //Makes sure this file is included only once in  a single compilation.

# define uchar unsigned char //specifying shorthand for unsigned charater
# define CLOCKSPEED  25000000//Clockspeed used for SPI serial communication between arduino and AD9910... setting to 15 MHz... James had it at 25 MHz - should check

# include <SPI.h> //SPI interface that arduino and other microcontrollers use to communicate with slave devices
# include <stdint.h>

class AD9910
{
	public:
		int _cs, _rst, _update, _sdio, _sclk, _mrst; //choose slave? I believe this is equivalent to SS that james was using, pin 10, should be low for slave you are writing to, reset, update, serial data input output , system clock
		uint8_t cfr1[4] = { 0x00, 0x40, 0x20, 0x00 }; //Define Control Function Register 1 default values as originally figured out by James - see page 49/64 and 54/64 of AD9910 manual
		uint8_t cfr2[4] = { 0x01, 0x00, 0x08, 0x20 }; //Define Control Function Register 2 default values as originally figured out by James
		uint8_t cfr3[4] = { 0x1F, 0x3F, 0x40, 0x00 }; //Define Control Function Register 3 default values as originally figured out by James
		uint8_t DAC_config[4] = { 0x00, 0x00, 0x00, 0x7F }; //Onboard Auxillary DAC control register, default is 0x7F
		uint8_t Profile0[8] = { 0x08, 0xB5, 0x00, 0x00, 0x14, 0x7A, 0xE1, 0x48 }; //Single Frequency or RAM profile, depending on control register settings. Bytes organized as :{amplitude, amplitude, phase, phase, freq, freq, freq ,freq}
	//Constructor
		AD9910(int cs, int rst, int update, int sdio, int sclk, int mrst) //In main program, I believe this defines which pins are which... I will check this
		{
			_cs = cs;
			_rst = rst;
			_update = update;
			_sdio = sdio; //Check if we want this unidirectional or bidirectional
			_sclk = sclk;
      _mrst = mrst;
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
			
			//Set pins High or Low
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
        digitalWrite(_mrst,LOW);
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
				return;
			}
			unsigned long temp; //temp variable for frequency calculations
			if (freq > 200000000) {//protection against too big of a frequency - setting to 200 MHz for now - AOM designed for 40 MHz anyway
				freq = 200000000;
			}
			temp = freq * 8.589934592;//4.294967296; //uses our clock frequency of 1 GHz with a divider of 2, and includes 2^32

			Profile0[7] = (uchar)temp; //uchar will only ever take the last byte of a number's binary representation. We then need to take our frequency tuning word, temp, in byte sized steps before sending to AD9910 via SPI
			Profile0[6] = (uchar)(temp >> 8); //shifts binary representation 8 bits to the right, or one byte, and we then take the last byte to send to AD9910
			Profile0[5] = (uchar)(temp >> 16);
			Profile0[4] = (uchar)(temp >> 24);

			SPI_Write_Reg(0x0E + profile, Profile0, 8);
			update();
		}

		//Function to set amplitude for a profile in single frequency mode

		void set_Amp(double amp, uint8_t profile = 0)
		{
			unsigned long temp;
			temp = (unsigned long)amp * 25.20615385; //amplitude tuning word.. need to check math
			if (temp > 0x3FFF) {
				temp = 0x3FFF;
			}
			temp &= 0x3FFF;
			Profile0[0] = (uchar)(temp >> 8); //see set_freq for similar syntax
			Profile0[1] = (uchar)(temp);

			SPI_Write_Reg(0x0E + profile, Profile0, 8);
			update();
		}
};
