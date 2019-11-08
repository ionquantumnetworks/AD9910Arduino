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
  byte CFR1InstructionByte = 0x00; //Instruction Byte to tell DDS what the following data is
  byte CFR1Byte1 = 0x00;
  byte CFR1Byte2 = 0x40; //0x40 to enable inverse sinc filter
  byte CFR1Byte3 = 0x00; 
  byte CFR1Byte4 = 0x02; //0x00
//CFR2 Bytes
  byte CFR2InstructionByte = 0x01; //Instruction Byte to tell DDS what the following data is
  byte CFR2Byte1 = 0x00;
  byte CFR2Byte2 = B00001100;//0x48
  byte CFR2Byte3 = 0x08; 
  byte CFR2Byte4 = 0x20; 
//CFR3 Bytes
  byte CFR3InstructionByte = 0x02; //Instruction Byte to tell DDS what the following data is
  byte CFR3Byte1 = 0x1F;
  byte CFR3Byte2 = 0x3F; 
  byte CFR3Byte3 = 0x40; //0x40
  byte CFR3Byte4 = 0x00; 
//Digital Ramp Limit
  byte DRLInstructionByte = 0x0B;
  byte DRLByte1 = 0x33; //Upper limit Frequency
  byte DRLByte2 = 0x33; //Upper limit Frequency
  byte DRLByte3 = 0x33; //Upper limit Frequency
  byte DRLByte4 = 0x33; //Upper limit Frequency
  byte DRLByte5 = 0x05; //Lower limit Frequency
  byte DRLByte6 = 0x1E; //Lower limit Frequency
  byte DRLByte7 = 0xB8; //Lower limit Frequency
  byte DRLByte8 = 0x52; //Lower limit Frequency
//Digital Ramp Step
  byte DRSInstructionByte = 0x0C;
  byte DRSByte1 = 0x00; //Decrement Step
  byte DRSByte2 = 0x0D; //Decrement Step
  byte DRSByte3 = 0x1B; //Decrement Step
  byte DRSByte4 = 0x71; //Decrement Step
  byte DRSByte5 = 0x00; //Increment Step
  byte DRSByte6 = 0x0D; //Increment Step
  byte DRSByte7 = 0x1B; //Increment Step
  byte DRSByte8 = 0x71; //Increment Step
//Digital Ramp Rate
  byte DRRInstructionByte = 0x0D;
  byte DRRByte1 = 0xF4; //-ve slope rate
  byte DRRByte2 = 0x24; //-ve slope rate
  byte DRRByte3 = 0xF4; //+ve slope rate
  byte DRRByte4 = 0x24; //+ve slope rate
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
//Digital Ramp Limit data transfer
  digitalWrite(IOreset, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(IOreset, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(DRLInstructionByte);
  SPI.transfer(DRLByte1);
  SPI.transfer(DRLByte2);
  SPI.transfer(DRLByte3);
  SPI.transfer(DRLByte4);
  SPI.transfer(DRLByte5);
  SPI.transfer(DRLByte6);
  SPI.transfer(DRLByte7);
  SPI.transfer(DRLByte8);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(IOupdate, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(IOupdate, LOW);//Send the above data to DDS from register
//Digital Ramp Step data transfer
  digitalWrite(IOreset, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(IOreset, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(DRSInstructionByte);
  SPI.transfer(DRSByte1);
  SPI.transfer(DRSByte2);
  SPI.transfer(DRSByte3);
  SPI.transfer(DRSByte4);
  SPI.transfer(DRSByte5);
  SPI.transfer(DRSByte6);
  SPI.transfer(DRSByte7);
  SPI.transfer(DRSByte8);
  delay(5);
  digitalWrite(SS,HIGH);//set the write line to HIGH so that the DDS stops reading
  delay(5);
  digitalWrite(IOupdate, HIGH);//Send the above data to DDS from register
  delay(5);
  digitalWrite(IOupdate, LOW);//Send the above data to DDS from register
//Digital Ramp Rate data transfer
  digitalWrite(IOreset, HIGH);//Reset the I/O communication
  delay(5);
  digitalWrite(IOreset, LOW);//Reset the I/O communication
  digitalWrite(SS,LOW);//set the write line to LOW so that the DDS reads the following data
  delay(5);
  SPI.transfer(DRRInstructionByte);
  SPI.transfer(DRRByte1);
  SPI.transfer(DRRByte2);
  SPI.transfer(DRRByte3);
  SPI.transfer(DRRByte4);
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
    digitalWrite(DRCTL,HIGH);
    //delay(1);
    digitalWrite(DRCTL,LOW);
    delay(5000);
  }
}
