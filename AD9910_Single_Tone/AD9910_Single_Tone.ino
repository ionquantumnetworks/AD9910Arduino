#include <SPI.h>

void setup() {
  // initialize SPI:
  SPI.beginTransaction(SPISettings(25000000, MSBFIRST, SPI_MODE2));
  SPI.begin();
  digitalWrite(SS,HIGH); //the AD9910 reads data when this is low. So set to high to begin
  //SPI.setBitOrder(MSBFIRST); //set the data mode to most significant bit. The AD9910 has this as default
  pinMode(2,OUTPUT); //set pin 2 to be an output pin
  pinMode(3,OUTPUT); //set pin 3 to be an output pin
  pinMode(4,OUTPUT); //set pin 4 to be an output pin
  digitalWrite(2, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(3, LOW); //Set the IOReset line to LOW as a HIGH value resets IO communication
  digitalWrite(4, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
//CFR1 Bytes
  byte CFR1InstructionByte = 0x00;
  byte CFR1Byte1 = 0x00;
  byte CFR1Byte2 = 0x40; //0x40 tp enable inverse sinc filter
  byte CFR1Byte3 = 0x00; 
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
  byte DataByte5 = 0x14; //Frequency
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
  digitalWrite(2, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
  digitalWrite(3, LOW); //Set the IOReset line to LOW as a HIGH value resets IO communication
  digitalWrite(4, LOW); //Set the Master Reset line to LOW as a HIGH value resets chip to default values
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
}
