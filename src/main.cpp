#include <Arduino.h>
#include<Wire.h>
#include<MPU6050.h>

MPU6050 imu;

unsigned long lastRetryTime = 0;
const unsigned long retryInterval = 2000;

const unsigned long sensorDataDelayTime = 100;
unsigned long sensorInitialDelay = 0;

unsigned long commDelayTime = 500;
unsigned long commInitialDelay = 0;

MPU6050::Vector3 accel{};
MPU6050::Vector3 gyro{};

enum class Status
{
  Initializing,
  Running,
  Error,
  Recovery
};
Status currentStatus = Status::Initializing;

struct SensorData
{
  float accelX;
  float accelY;
  float accelZ;

  float gyroX;
  float gyroY;
  float gyroZ;
};


void handleStateMachine()
{
  switch(currentStatus)
  {
    case Status::Initializing:
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

      if((millis() - sensorInitialDelay) >= sensorDataDelayTime)
      {
        if(!imu.update())
        {
          currentStatus = Status::Error;
          break;
        }
        accel = imu.getAccelerometer();
        gyro = imu.getGyro();

        sensorInitialDelay = millis();
      }
      break;


    case Status::Error:
        lastRetryTime = millis();
        currentStatus = Status::Recovery;
        break;

    case Status::Recovery:
        
        if((millis() - lastRetryTime) >= retryInterval)
        {
          if(!imu.begin())
          {
            
            currentStatus = Status::Recovery;
            lastRetryTime = millis();
          }
          else{
            sensorInitialDelay = millis();
            commInitialDelay = millis();
            currentStatus = Status::Running;
          }
        }
        break;  
    }   
}
void communicationHandler()
{
  if (currentStatus == Status::Initializing)
  {
    Serial.println("Trying to restablish the communication with the sensor");
  }
  if (currentStatus == Status::Error)
  {
    Serial.println("ERROR: imu error.");
  }
  if (currentStatus == Status::Recovery)
  {
    Serial.println("In recovery mode");
  }

  if(((millis() - commInitialDelay) >= commDelayTime) && currentStatus == Status::Running)
      {
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
        commInitialDelay = millis();
      }
}


void sensorTask(void *pvParameters)
{
  while(true)
  {
    handleStateMachine();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void communicationTask(void *pvParameters)
{
  while(true)
  {
    communicationHandler();
    vTaskDelay(pdMS_TO_TICKS(500));
  }

}
void setup()
{
  Serial.begin(115200);
  BaseType_t sensor_result = xTaskCreate(
    sensorTask,
    "handle_state_machine",
    4096,
    nullptr,
    2,
    nullptr 
  );

  BaseType_t comm_result = xTaskCreate(
    communicationTask,
    "communication_handler",
    4096,
    nullptr,
    1,
    nullptr
  );

  if(sensor_result != pdPASS)
  {
    Serial.println("Failed to create sensor acquisition task");
  }
  if(comm_result != pdPASS)
  {
    Serial.println("Failed to create serial communication task");
  }
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}   