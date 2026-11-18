#pragma once
#include<Arduino.h>

class MPU6050
{
    public:

        struct IMUData
        {
            float accX;
            float accY;
            float accZ;

            float gyroX;
            float gyroY;
            float gyroZ;
        };

        enum class SystemState
        {
            INITILIZING,
            NORMAL,
            WARNING,
            ERROR   
        };

        bool begin();
        bool read();


    private:
        uint8_t const imu_address = 0x68;
        void readSensor();

};  