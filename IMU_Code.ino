#include <SPI.h>
#include <mcp_can.h>

const long acl_period_ms = 5;
const long gys_period_ms = 20;

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

unsigned char acl_len = 6;
unsigned char gys_len = 6;
unsigned long acl_id = 0x3FF;
unsigned long gys_id = 0x400;
unsigned char acl_data[6] = {0, 0, 0, 0, 0, 0};
unsigned char gys_data[6] = {0, 0, 0, 0, 0, 0};
unsigned long acl_time = 0;
unsigned long gys_time = 0;
 
#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
float AcX, AcY, AcZ, GyX, GyY, GyZ;


char CANSignalConvert(float data, int range, int shift){
  int sig = (data + range) * 0xFFFF / (range*2);
  int mask = 0xFF << shift;
  return (sig & mask) >> shift;
}
  
void setup(){
    Serial.begin(115200);
    CAN.init_Filt(0, 0, acl_id);
      while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN initialization retrying.");}
    
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // PWR_MGMT_1 register
    Wire.write(0);     // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}
void loop(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("AcX = "); Serial.print(AcX/16384);
  Serial.print(" | AcY = "); Serial.print(AcY/16384);
  Serial.print(" | AcZ = "); Serial.print(AcZ/16384);
  Serial.print(" | GyX = "); Serial.print(GyX/131);
  Serial.print(" | GyY = "); Serial.print(GyY/131);
  Serial.print(" | GyZ = "); Serial.println(GyZ/131);


   if ((millis() - acl_time) > acl_period_ms){
      //reset last transmit time
      acl_time = millis();
  
//      put accelerometer data into can_data 8 byte array
      acl_data[0] = CANSignalConvert(AcX/16384, 2, 8);
      acl_data[1] = CANSignalConvert(AcX/16384, 2, 0);
      acl_data[2] = CANSignalConvert(AcY/16384, 2, 8);
      acl_data[3] = CANSignalConvert(AcY/16384, 2, 0);
      acl_data[4] = CANSignalConvert(AcZ/16384, 2, 8);
      acl_data[5] = CANSignalConvert(AcZ/16384, 2, 0);
      
      //send CAN message onto bus
      CAN.sendMsgBuf(acl_id, 0, acl_len, acl_data);
    }
     
     if ((millis() - gys_time) > gys_period_ms){
      //reset last transmit time
      gys_time = millis();
  
      //put accelerometer data into can_data 8 byte array
      gys_data[0] = CANSignalConvert(GyX/131, 250, 8);
      gys_data[1] = CANSignalConvert(GyX/131, 250, 0);
      gys_data[2] = CANSignalConvert(GyY/131, 250, 8);
      gys_data[3] = CANSignalConvert(GyY/131, 250, 0);
      gys_data[4] = CANSignalConvert(GyZ/131, 250, 8);
      gys_data[5] = CANSignalConvert(GyZ/131, 250, 0);
  
  
      //send CAN message onto bus
      CAN.sendMsgBuf(gys_id, 0, gys_len, gys_data); 
    }
}




