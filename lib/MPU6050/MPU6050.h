#pragma once
#include<Arduino.h>
#include<Wire.h>

class MPU6050
{
    public:

        struct  Vector3
        {
            float X;
            float Y;
            float Z;
        };

        bool begin();
        bool update();

        Vector3 getAccelerometer() const;
        Vector3 getGyro()  const;

    private:
        static constexpr uint8_t imuAddress = 0x68;
        static constexpr size_t MAX_READ_LENGTH = 14;
        
        static constexpr uint8_t WHO_AM_I = 0x68;
        static constexpr uint8_t REG_WHO_AM_I = 0x75;
        static constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
        static constexpr uint8_t REG_SENSOR_DATA = 0x3B;


        Vector3 accelerometerData{};
        Vector3 gyroData{};

        bool readRegister(uint8_t reg, uint8_t* buffer, size_t length);
        bool writeRegister(uint8_t reg, uint8_t value);

};  