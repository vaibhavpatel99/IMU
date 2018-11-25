#include<Wire.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
float AcX,AcY,AcZ,GyX,GyY,GyZ;
int x = 0;
int row = 0;
void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 128000 bps
  Serial.println("CLEARDATA");
  Serial.println("LABEL,Time,AcX,AcY,AcZ,GyX,GyY,GyZ");
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true); 
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("DATA,TIME,"); Serial.print(AcX/16384); Serial.print(","); Serial.print(AcY/16384); Serial.print(","); Serial.print(AcZ/16384);Serial.print(","); Serial.print(GyX/131);Serial.print(","); Serial.print(GyY/131);Serial.print(","); Serial.println(GyZ/131); 
  row++;
  x++;
  if (row > 360) 
   {
    row=0;
    Serial.println("ROW,SET,2");
   }
  //delay(100);
}
