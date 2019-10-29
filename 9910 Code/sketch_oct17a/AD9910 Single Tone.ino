#include <SPI.h>

void setup() {
  // initialize SPI:
  SPISettings settingsA(20000000, MSBFIRST, SPI_MODE1);
  SPI.begin();
  digitalWrite(SS,HIGH); //the AD9910 reads data when this is low. So set to high to begin
  SPI.setBitOrder(MSBFIRST); //set the data mode to most significant bit. The AD9910 has this as default
  pinMode(2,OUTPUT); //set pin 2 to be an output pin
  pinMode(3,OUTPUT); //set pin 3 to be an output pin
  pinMode(4,OUTPUT); //set pin 4 to be an output pin
  digitalWrite(2, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
   //CFR1 Bytes
  byte CFR1InstructionByte = B00000000;
  byte CFR1Byte1 = B00000000;
  byte CFR1Byte2 = B01000000; 
  byte CFR1Byte3 = B00000000; 
  byte CFR1Byte4 = B00000010; //0x00
//CFR2 Bytes
  byte CFR2InstructionByte = B00000001;
  byte CFR2Byte1 = B00000000; //0x01
  byte CFR2Byte2 = B01000000; 
  byte CFR2Byte3 = B00001000; 
  byte CFR2Byte4 = B00100000; 
//CFR3 Bytes
  byte CFR3InstructionByte = B00000010;
  byte CFR3Byte1 = B00011111;
  byte CFR3Byte2 = B00111111; 
  byte CFR3Byte3 = B11000000; //0x40
  byte CFR3Byte4 = B00000000; 
//Aux DAC control
  byte ADCInstructionByte = B00000011;
  byte ADCByte1 = B00000000;
  byte ADCByte2 = B00000000; 
  byte ADCByte3 = B01111111; 
  byte ADCByte4 = B01111111; 
  //I/O update Rate
  byte IOInstructionByte = B00000100;
  byte IOByte1 = B11111111;
  byte IOByte2 = B11111111; 
  byte IOByte3 = B11111111; 
  byte IOByte4 = B11111111; 
//Single Tone Data
  byte InstructionByte = B00001110;; //Byte to tell DDS what the following data is
  byte DataByte1 = B00001000; //Amplitude scale factor
  byte DataByte2 = B10110101; //Amplitude scale factor
  byte DataByte3 = B00000000; //Phase Offset
  byte DataByte4 = B00000000; //Phase Offset
  byte DataByte5 = 0x14; //Frequency
  byte DataByte6 = 0x7A; //Frequency
  byte DataByte7 = 0xE1; //Frequency
  byte DataByte8 = 0x48; //Frequency
  digitalWrite(4, HIGH);//Reset the board
  delay(5);
  digitalWrite(4, LOW);//Reset the board
  digitalWrite(3, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(3, LOW);//Reset the I/O communication
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
  delay(5);
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
  delay(5);
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
  delay(5);
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(ADCInstructionByte);
  SPI.transfer(ADCByte1);
  SPI.transfer(ADCByte2);
  SPI.transfer(ADCByte3);
  SPI.transfer(ADCByte4);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(2, LOW);//Send the above data to DDS from register
  delay(5);
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(IOInstructionByte);
  SPI.transfer(IOByte1);
  SPI.transfer(IOByte2);
  SPI.transfer(IOByte3);
  SPI.transfer(IOByte4);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(2, LOW);//Send the above data to DDS from register
  delay(5);
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
 
}
