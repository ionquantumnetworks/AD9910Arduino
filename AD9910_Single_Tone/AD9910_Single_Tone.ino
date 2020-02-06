#include <SPI.h>
//Arduino setup: Remove jumpers: W3, W5, W6. Set jumpers W1, W2 and W4 to disable. Place jumper on EXT_PWR_DWN

void setup() {
  // initialize SPI:
  SPI.beginTransaction(SPISettings(25000000, MSBFIRST, SPI_MODE2)); //SCK is PIN 13 MOSI is PIN 11
  SPI.begin();
  digitalWrite(SS,HIGH); //PIN 10 the AD9910 reads data when this is low. So set to high to begin
  //outout pin setups
  int MasterReset = 4;
  int IOupdate = 2;
  int IOreset = 3;
  int DRCTL = 5;
  int P0 = 6;
  int P1 = 7;
  int P2 = 8;
  pinMode(P0,OUTPUT);
  pinMode(P1,OUTPUT);
  pinMode(P2,OUTPUT);
  pinMode(IOupdate,OUTPUT); //set pin 2 to be an output pin
  pinMode(IOreset,OUTPUT); //set pin 3 to be an output pin
  pinMode(MasterReset,OUTPUT); //set pin 4 to be an output pin
  pinMode(DRCTL, OUTPUT);
  digitalWrite(IOupdate, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(IOreset, LOW); //Set the IOreset line to LOW as a HIGH value resets IO communication
  digitalWrite(MasterReset, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
  digitalWrite(P0,LOW);
  digitalWrite(P1,LOW);
  digitalWrite(P2,LOW);
  //CFR1 Bytes
  byte CFR1InstructionByte = 0x00;
  byte CFR1Byte1 = 0x00;
  byte CFR1Byte2 = 0x40; //0x40 tp enable inverse sinc filter
  byte CFR1Byte3 = B00100000; 
  byte CFR1Byte4 = 0x02; //0x00
//CFR2 Bytes
  byte CFR2InstructionByte = 0x01;
  byte CFR2Byte1 = 0x01; //0x01 to enable single tone amplitude data
  byte CFR2Byte2 = 0x00; //0x40 to enable SYNC_CLK output
  byte CFR2Byte3 = 0x08; 
  byte CFR2Byte4 = 0x20; 
//CFR3 Bytes
  byte CFR3InstructionByte = 0x02;
  byte CFR3Byte1 = 0x1F;
  byte CFR3Byte2 = 0x3F; 
  byte CFR3Byte3 = 0x40; //0x40
  byte CFR3Byte4 = 0x00; 
//Single Tone Data Profile 0
  byte InstructionByteP0 = 0x0E;; //Byte to tell DDS what the following data is
  byte DataByte1P0 = 0x08; //Amplitude scale factor
  byte DataByte2P0 = 0xB5; //Amplitude scale factor
  byte DataByte3P0 = 0x00; //Phase Offset
  byte DataByte4P0 = 0x00; //Phase Offset
  byte DataByte5P0 = 0x14; //Frequency NOTE: fsysclk is 500 MHz with a 1 GHz referernce clock
  byte DataByte6P0 = 0x7A; //Frequency
  byte DataByte7P0 = 0xE1; //Frequency
  byte DataByte8P0 = 0x48; //Frequency
//Single Tone Data Profile 1
  byte InstructionByteP1 = 0x0F;; //Byte to tell DDS what the following data is
  byte DataByte1P1 = 0x00; //Amplitude scale factor
  byte DataByte2P1 = 0x00; //Amplitude scale factor
  byte DataByte3P1 = 0x00; //Phase Offset
  byte DataByte4P1 = 0x00; //Phase Offset
  byte DataByte5P1 = 0x00; //Frequency 
  byte DataByte6P1 = 0x00; //Frequency
  byte DataByte7P1 = 0x00; //Frequency
  byte DataByte8P1 = 0x00; //Frequency
//RESETS
  digitalWrite(MasterReset, HIGH);//Reset the board
  delay(5);
  digitalWrite(MasterReset, LOW);//Reset the board
  digitalWrite(IOreset, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(IOreset, LOW);//Reset the I/O communication
//CFR1 data transfer
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(CFR1InstructionByte);
  SPI.transfer(CFR1Byte1);
  SPI.transfer(CFR1Byte2);
  SPI.transfer(CFR1Byte3);
  SPI.transfer(CFR1Byte4);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(IOupdate, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(IOupdate, LOW);//Send the above data to DDS from register
//CFR2 data transfer
  digitalWrite(IOreset, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(IOreset, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(CFR2InstructionByte);
  SPI.transfer(CFR2Byte1);
  SPI.transfer(CFR2Byte2);
  SPI.transfer(CFR2Byte3);
  SPI.transfer(CFR2Byte4);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(IOupdate, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(IOupdate, LOW);//Send the above data to DDS from register
//CFR3 data transfer
  digitalWrite(IOreset, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(IOreset, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(CFR3InstructionByte);
  SPI.transfer(CFR3Byte1);
  SPI.transfer(CFR3Byte2);
  SPI.transfer(CFR3Byte3);
  SPI.transfer(CFR3Byte4);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(IOupdate, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(IOupdate, LOW);//Send the above data to DDS from register
//Single Tone data transfers
  digitalWrite(3, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(3, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(InstructionByteP0);
  SPI.transfer(DataByte1P0);
  SPI.transfer(DataByte2P0);
  SPI.transfer(DataByte3P0);
  SPI.transfer(DataByte4P0);
  SPI.transfer(DataByte5P0);
  SPI.transfer(DataByte6P0);
  SPI.transfer(DataByte7P0);
  SPI.transfer(DataByte8P0);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(2, LOW);//Send the above data to DDS from register
    digitalWrite(3, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(3, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(InstructionByteP1);
  SPI.transfer(DataByte1P1);
  SPI.transfer(DataByte2P1);
  SPI.transfer(DataByte3P1);
  SPI.transfer(DataByte4P1);
  SPI.transfer(DataByte5P1);
  SPI.transfer(DataByte6P1);
  SPI.transfer(DataByte7P1);
  SPI.transfer(DataByte8P1);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(2, LOW);//Send the above data to DDS from register
}

void loop() {
  int MasterReset = 4;
  int IOupdate = 2;
  int IOreset = 3;
  int DRCTL = 5;
  int DRHOLD = 6;
  int P0 = 6;
  int P1 = 7;
  int P2 = 8;
  int i = 0;
  digitalWrite(P0,LOW);
  digitalWrite(P1,LOW);
  digitalWrite(P2,LOW);
  digitalWrite(IOupdate, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(IOreset, LOW); //Set the IOReset line to LOW as a HIGH value resets IO communication
  digitalWrite(MasterReset, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  while(i < 100){
    digitalWrite(P0,LOW);
  }
}
