#include <SPI.h>
//Arduino setup: Remove jumpers: W3, W5, W6. Set jumpers W1, W2 and W4 to disable. Place jumpers on P_0, P_1, P_2 and EXT_PWR_DWN

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
  int DROVER = 6;
  int DRHOLD = 7;
  pinMode(IOupdate,OUTPUT); //set pin 2 to be an output pin
  pinMode(IOreset,OUTPUT); //set pin 3 to be an output pin
  pinMode(MasterReset,OUTPUT); //set pin 4 to be an output pin
  pinMode(DRCTL, OUTPUT);
  pinMode(DROVER, OUTPUT);
  pinMode(DRHOLD, OUTPUT);
  digitalWrite(IOupdate, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(IOreset, LOW); //Set the IOreset line to LOW as a HIGH value resets IO communication
  digitalWrite(MasterReset, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
//CFR1 Bytes
  byte CFR1InstructionByte = 0x00; //Instruction Byte to tell DDS what the following data is
  byte CFR1Byte1 = 0x00;
  byte CFR1Byte2 = 0x00; //0x40 to enable inverse sinc filter
  byte CFR1Byte3 = 0x00; 
  byte CFR1Byte4 = 0x02; //0x00
//CFR2 Bytes
  byte CFR2InstructionByte = 0x01; //Instruction Byte to tell DDS what the following data is
  byte CFR2Byte1 = 0x01; //0x01 to enable single tone amplitude data
  byte CFR2Byte2 = 0x00; //0x40 to enable SYNC_CLK output
  byte CFR2Byte3 = 0x08; 
  byte CFR2Byte4 = 0x20; 
//CFR3 Bytes
  byte CFR3InstructionByte = 0x02; //Instruction Byte to tell DDS what the following data is
  byte CFR3Byte1 = 0x1F;
  byte CFR3Byte2 = 0x3F; 
  byte CFR3Byte3 = 0x40; //0x40
  byte CFR3Byte4 = 0x00; 
//Single Tone Data
  byte InstructionByte = 0x0E;; //Instruction Byte to tell DDS what the following data is
  byte DataByte1 = 0x3F; //Amplitude scale factor
  byte DataByte2 = 0xFF; //Amplitude scale factor
  byte DataByte3 = 0x00; //Phase Offset
  byte DataByte4 = 0x00; //Phase Offset
  byte DataByte5 = 0x14; //Frequency
  byte DataByte6 = 0x7A; //Frequency
  byte DataByte7 = 0xE1; //Frequency
  byte DataByte8 = 0x48; //Frequency
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
//Single Tone data transfer
  digitalWrite(IOreset, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(IOreset, LOW);//Reset the I/O communication
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
  digitalWrite(IOupdate, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(IOupdate, LOW);//Send the above data to DDS from register
}


void loop() {
  int MasterReset = 4;
  int IOupdate = 2;
  int IOreset = 3;
  digitalWrite(IOupdate, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(IOreset, LOW); //Set the IOReset line to LOW as a HIGH value resets IO communication
  digitalWrite(MasterReset, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
}
