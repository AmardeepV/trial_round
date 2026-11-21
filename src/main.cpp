#include <Arduino.h>
#include<Wire.h>
#include<MPU6050.h>

MPU6050 imu;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initialization MPU6050");

  if(!imu.begin())
  {
    Serial.println("ERROR: imu initialization failed.");
    while(true)
    {
      delay(1000);
    }
  }
  Serial.println("MPU6050 initialized successfully.");
}

void loop()
{
  if(!imu.update())
  {
    Serial.println("ERROR: Failed to read sensor data.");
    delay(500);
    return;
  }
  MPU6050::Vector3 accel = imu.getAccelerometer();
  MPU6050::Vector3 gyro = imu.getGyro();

  Serial.println("Accelerometer (g):");
    Serial.print("X: ");
    Serial.print(accel.X);
    Serial.print(" | Y: ");
    Serial.print(accel.Y);
    Serial.print(" | Z: ");
    Serial.println(accel.Z);

    Serial.println("Gyroscope (degrees/s):");
    Serial.print("X: ");
    Serial.print(gyro.X);
    Serial.print(" | Y: ");
    Serial.print(gyro.Y);
    Serial.print(" | Z: ");
    Serial.println(gyro.Z);

    Serial.println("---------------------------");

    delay(500);
}