#include <SPI.h>

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
  byte CFR1Byte2 = 0x41; //0x41 to enable inverse sinc filter and sine output
  byte CFR1Byte3 = 0x20; 
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
//Single Tone Data
  byte InstructionByte = 0x0E;; //Byte to tell DDS what the following data is
  byte DataByte1 = 0x3F; //Amplitude scale factor
  byte DataByte2 = 0xFF; //Amplitude scale factor
  byte DataByte3 = 0x00; //Phase Offset
  byte DataByte4 = 0x00; //Phase Offset
  byte DataByte5 = 0x05; //Frequency
  byte DataByte6 = 0x7A; //Frequency
  byte DataByte7 = 0xE1; //Frequency
  byte DataByte8 = 0x48; //Frequency
//RESETS
  digitalWrite(4, HIGH);//Reset the board
  delay(5);
  digitalWrite(4, LOW);//Reset the board
  digitalWrite(3, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(3, LOW);//Reset the I/O communication
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
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(2, LOW);//Send the above data to DDS from register
//CFR2 data transfer
  digitalWrite(3, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(3, LOW);//Reset the I/O communication
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
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(2, LOW);//Send the above data to DDS from register
//CFR3 data transfer
  digitalWrite(3, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(3, LOW);//Reset the I/O communication
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
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(2, LOW);//Send the above data to DDS from register
//Single Tone data transfer
  digitalWrite(3, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(3, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(InstructionByte);
  SPI.transfer(DataByte1);
  SPI.transfer(DataByte2);
  SPI.transfer(DataByte3);
  SPI.transfer(DataByte4);
  SPI.transfer(DataByte5);
  SPI.transfer(DataByte6);
  SPI.transfer(DataByte7);
  SPI.transfer(DataByte8);
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
  int P0 = 6;
  int P1 = 7;
  int P2 = 8;
  int i = 0;
  int ExtTrig = 9;
  int dum = 0;
  digitalWrite(P0,LOW); //The high-low toggle for this signal on the DDS board will be provided from an externl source
  digitalWrite(P1,LOW);
  digitalWrite(P2,LOW);
  digitalWrite(IOupdate, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(IOreset, LOW); //Set the IOReset line to LOW as a HIGH value resets IO communication
  digitalWrite(MasterReset, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  pinMode(ExtTrig, INPUT);
  digitalWrite(2, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(3, LOW); //Set the IOReset line to LOW as a HIGH value resets IO communication
  digitalWrite(4, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
}
