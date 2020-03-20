//Run File for AD9910 - See AD9910.h for function definitions


# include "AD9910.h"
# include <SPI.h>
# include <stdint.h>

# define cs 10
# define rst 3 //3
# define update 2
# define sdio 11
# define sclk 13
# define mrst 4
# define sTrig 5

# define CLOCKSPEED 25000000//Clockspeed used for SPI serial communication between arduino and AD9910... setting to 15 MHz... James had it at 25 MHz - should check

AD9910 DDS(cs, rst, update, sdio, sclk, mrst, sTrig);
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
  
  int P0 = 6;
  int P1 = 7;
  int P2 = 8;
  pinMode(P0,OUTPUT);
  pinMode(P1,OUTPUT);
  pinMode(P2,OUTPUT);
  digitalWrite(P0,LOW);
  digitalWrite(P1,LOW);
  digitalWrite(P2,LOW);
  DDS.set_freq(freq,0);

}

void loop() {
  //LED showing code working
  //digitalWrite(25, HIGH);
  int P0 = 6;
  int P1 = 7;
  int P2 = 8;
  int counter = 0;
  while (counter < 10000){
  digitalWrite(P0,LOW);
  digitalWrite(P1,LOW);
  digitalWrite(P2,LOW);
  //Serial.println("Working...");
  counter++;
    } 
  Serial.println("Working...");
  counter = 0;
  //DDS.set_freq(freq,0);
  //DDS.set_Amp(amp);
}
