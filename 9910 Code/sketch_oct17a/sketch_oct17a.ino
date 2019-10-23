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
  int CFR1InstructionByte = 0x00;
  int CFR1Byte1 = 0x00;
  int CFR1Byte2 = 0x00; 
  int CFR1Byte3 = 0x00; 
  int CFR1Byte4 = 0x00; 
//CFR2 Bytes
  int CFR2InstructionByte = 0x01;
  int CFR2Byte1 = 0x00;
  int CFR2Byte2 = 0x00; 
  int CFR2Byte3 = 0x00; 
  int CFR2Byte4 = 0x00; 
//CFR3 Bytes
  int CFR3InstructionByte = 0x02;
  int CFR3Byte1 = 0x00;
  int CFR3Byte2 = 0x00; 
  int CFR3Byte3 = 0x00; 
  int CFR3Byte4 = 0x00; 
//Single Tone Data
  int InstructionByte = 0x0E; //Byte to tell DDS what the following data is
  int DataByte1 = 0x08; //Amplitude scale factor
  int DataByte2 = 0xB5; //Amplitude scale factor
  int DataByte3 = 0x00; //Phase Offset
  int DataByte4 = 0x00; //Phase Offset
  int DataByte5 = 0x0A; //Frequency
  int DataByte6 = 0x3D; //Frequency
  int DataByte7 = 0x70; //Frequency
  int DataByte8 = 0xA4; //Frequency
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  SPI.transfer(CFR1InstructionByte);
  SPI.transfer(CFR1Byte1);
  SPI.transfer(CFR1Byte2);
  SPI.transfer(CFR1Byte3);
  SPI.transfer(CFR1Byte4);
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
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(500);
  digitalWrite(2, HIGH);//Send the above data to DDS from register
  delay(500);
  digitalWrite(2, LOW);//Send the above data to DDS from register
}

void loop() {

}
//40 MHz using a 1GHz clock shoud be: 00001010 00111101 01110000 10100100
//                                      0x0A    0x3D     0x70     0xA4
