#include <Arduino.h>
#include<Wire.h>
#include<MPU6050.h>

MPU6050 imu;

unsigned long lastRetryTime = 0;
const unsigned long retryInterval = 2000;
const unsigned long dataDelayTime = 500;
unsigned long initialDelay = 0;

enum class Status
{
  Initializing,
  Running,
  Error,
  Recovery
};
Status currentStatus = Status::Initializing;
void setup()
{
  Serial.begin(115200);
}

void handleStateMachine()
{
  switch(currentStatus)
  {
    case Status::Initializing:
      Serial.println("Trying to restablish the communication with the sensor");
      if(!imu.begin())
      {
      currentStatus = Status::Error;
      }
      else
      {
        currentStatus = Status::Running;
      }
      break;

    case Status::Running:

      if((millis() - initialDelay) >= dataDelayTime)
      {
        if(!imu.update())
        {
          currentStatus = Status::Error;
          break;
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

        initialDelay = millis();
      }
      break;


    case Status::Error:
        Serial.println("ERROR: imu error.");
        lastRetryTime = millis();
        currentStatus = Status::Recovery;
        break;

    case Status::Recovery:
        
        if((millis() - lastRetryTime) >= retryInterval)
        {
          if(!imu.begin())
          {
            Serial.println("In recovery mode");
            currentStatus = Status::Recovery;
            lastRetryTime = millis();
          }
          else{
            initialDelay = millis();
            Serial.println("Communication recovered successfully");
            currentStatus = Status::Running;
          }
        }
        break;  
    }   
}

void loop()
{
  handleStateMachine(); 
}