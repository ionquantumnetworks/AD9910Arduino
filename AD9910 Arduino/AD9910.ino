//Run File for AD9910 - See AD9910.h for function definitions


# include "AD9910.h"
# include <SPI.h>

# define cs 10
# define rst 3
# define update 2
# define sdio 11
# define sclk 13

# define CLOCKSPEED  15000000//Clockspeed used for SPI serial communication between arduino and AD9910... setting to 15 MHz... James had it at 25 MHz - should check

AD9910 DDS(cs, rst, update, sdio, sclk);
double freq = 40000000.0; //set frequency in Hz
double amp = 100.0; //set amplitude (out of 100?)

void setup() {
  // Output pin intended to be connected to LED to show that the board is running... commenting out for now
  //pinMode(25,OUTPUT);

  //Serial
  Serial.begin(115200); //Baud rate of USB.. need to check how fast we can make this. 9600 is standard when speed isn't critical. 115200 is another common option. 
  //SPI
  SPI.begin();
  //DDS
  DDS.begin();
}

void loop() {
  //LED showing code working
  //digitalWrite(25, HIGH);

  //serial message saying stuff is working
  Serial.println("Working...");

  DDS.set_freq(freq);
  DDS.set_Amp(amp);
}
