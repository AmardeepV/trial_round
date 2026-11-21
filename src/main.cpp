#include <Arduino.h>
#include<Wire.h>
#include<MPU6050.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Serial.print("I2C scanner");
  while(!Serial)
  {
    Serial.print(".");
  }
}

void loop()
{
  int nDevice = 0;
  int counter =0;
  uint8_t error, addresss;
  if( counter == 0)
  {
    Serial.println("I2C Scanning.");
    for(int address=1; address<127;address++)
    {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      if(error == 0)
      {
        Serial.print("I2c device found at the address of 0x");
        if(address < 16)
        {
          Serial.print("0");
        }
        Serial.print(address,HEX);
        nDevice++;
      }
      //Serial.println("");
      if(nDevice ==0)
      {
        Serial.println("No i2c device found");  
      }
      else
      {
        Serial.println("Done");
      }
    }
    counter++;
  }
  delay(5000);
}