#include <SPI.h>

void setup() {
  // initialize SPI:
  SPI.begin();
  digitalWrite(SS,HIGH); //the AD9910 reads data when this is low. So set to high to begin
  SPI.setBitOrder(MSBFIRST); //set the data mode to most significant bit. The AD9910 has this as default
  pinMode(2,OUTPUT); //set pin 2 to be an output pin
  digitalWrite(2, LOW); //Set the IOupdate line to LOW as a HIGH value sends data to DDS from register
//  int data = 110;
//  while(data < 100 ){
//    digitalWrite(SS, LOW);
//    SPI.transfer(data);
//    SPI.transfer(data);
//    digitalWrite(SS,HIGH);
//    data++;
//    delay(500);}
//CFR1 Bytes
  byte CFR1InstructionByte = B0110000;
  byte CFR1Byte1 = 0x00;
  byte CFR1Byte2 = 0x00; 
  byte CFR1Byte3 = 0x00; 
  byte CFR1Byte4 = 0x00; 
//CFR2 Bytes
  byte CFR2InstructionByte = B0110001;
  byte CFR2Byte1 = 0x01;
  byte CFR2Byte2 = 0x40; 
  byte CFR2Byte3 = 0x08; 
  byte CFR2Byte4 = 0x20; 
//CFR3 Bytes
  byte CFR3InstructionByte = B0110010;
  byte CFR3Byte1 = 0x1F;
  byte CFR3Byte2 = 0x3F; 
  byte CFR3Byte3 = 0x40; 
  byte CFR3Byte4 = 0x00; 
//Aux DAC control
  byte ADCInstructionByte = B0110011;;
  byte ADCByte1 = 0x00;
  byte ADCByte2 = 0x00; 
  byte ADCByte3 = 0x7F; 
  byte ADCByte4 = 0x7F; 
//Single Tone Data
  byte InstructionByte = B0111110;; //Byte to tell DDS what the following data is
  byte DataByte1 = 0x20; //Amplitude scale factor
  byte DataByte2 = 0x00; //Amplitude scale factor
  byte DataByte3 = 0x00; //Phase Offset
  byte DataByte4 = 0x00; //Phase Offset
  byte DataByte5 = 0x14; //Frequency
  byte DataByte6 = 0x7A; //Frequency
  byte DataByte7 = 0xE1; //Frequency
  byte DataByte8 = 0x48; //Frequency
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(500);
  SPI.transfer(CFR1InstructionByte);
  SPI.transfer(CFR1Byte1);
  SPI.transfer(CFR1Byte2);
  SPI.transfer(CFR1Byte3);
  SPI.transfer(CFR1Byte4);
  delay(500);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(500);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(500);
  digitalWrite(2, LOW);//Send the above data to DDS from register
  delay(500);
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  SPI.transfer(CFR2InstructionByte);
  SPI.transfer(CFR2Byte1);
  SPI.transfer(CFR2Byte2);
  SPI.transfer(CFR2Byte3);
  SPI.transfer(CFR2Byte4);
  delay(500);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(500);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(500);
  digitalWrite(2, LOW);//Send the above data to DDS from register
  delay(500);
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  SPI.transfer(CFR3InstructionByte);
  SPI.transfer(CFR3Byte1);
  SPI.transfer(CFR3Byte2);
  SPI.transfer(CFR3Byte3);
  SPI.transfer(CFR3Byte4);
  delay(500);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(500);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(500);
  digitalWrite(2, LOW);//Send the above data to DDS from register
  delay(500);
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  SPI.transfer(ADCInstructionByte);
  SPI.transfer(ADCByte1);
  SPI.transfer(ADCByte2);
  SPI.transfer(ADCByte3);
  SPI.transfer(ADCByte4);
  delay(500);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(500);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(500);
  digitalWrite(2, LOW);//Send the above data to DDS from register
  delay(500);
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  SPI.transfer(0x04);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  SPI.transfer(0xFF);
  delay(500);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(500);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(500);
  digitalWrite(2, LOW);//Send the above data to DDS from register
  delay(500);
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  SPI.transfer(InstructionByte);
  SPI.transfer(DataByte1);
  SPI.transfer(DataByte2);
  SPI.transfer(DataByte3);
  SPI.transfer(DataByte4);
  SPI.transfer(DataByte5);
  SPI.transfer(DataByte6);
  SPI.transfer(DataByte7);
  SPI.transfer(DataByte8);
  delay(500);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(500);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(500);
  digitalWrite(2, LOW);//Send the above data to DDS from register
}

void loop() {

}
