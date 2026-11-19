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

        Vector3 accelerometerData;
        Vector3 gyroData;

        bool readRegister(uint8_t reg, uint8_t* buffer, size_t length);
        bool writeRegister(uint8_t reg, uint8_t value);

};  