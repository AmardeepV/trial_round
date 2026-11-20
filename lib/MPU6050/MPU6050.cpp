#include<MPU6050.h>

bool MPU6050::begin()
{
    if(!Wire.begin())
    {
        return false;
    }
    uint8_t identity = 0;
    if(!readRegister(REG_WHO_AM_I, &identity, 1))
    {
        return false;
    }
    if(identity != WHO_AM_I)
    {
        return false;
    }
    if(!writeRegister(REG_PWR_MGMT_1, 0x00))
    {
        return false;
    }
    uint8_t woke_up_check =0;
    if(!readRegister(REG_PWR_MGMT_1, &woke_up_check, 1))
    {
        return false;
    }

    return ((woke_up_check & (1<< 6)) == 0);
}

bool MPU6050::update()
{
    uint8_t sensorData[14]={};

    if(!readRegister(REG_SENSOR_DATA, sensorData, 14))
    {
        return false;
    }

    // read accelerometer
    uint16_t rawAccelX = (static_cast<uint16_t>(sensorData[0]) << 8) | sensorData[1];
    int16_t accelX = static_cast<int16_t>(rawAccelX);
    uint16_t rawAccelY = (static_cast<uint16_t>(sensorData[2]) << 8) | sensorData[3];
    int16_t accelY = static_cast<int16_t>(rawAccelY);
    uint16_t rawAccelZ = (static_cast<uint16_t>(sensorData[4]) << 8) | sensorData[5];
    int16_t accelZ = static_cast<int16_t>(rawAccelZ);

    // Convert to physical units (default ±2g -> 16384 LSB/g, ±250°/s -> 131 LSB/(°/s))
    float ax = accelX / 16384.0;
    float ay = accelY / 16384.0;
    float az = accelZ / 16384.0;
    
    // read gyro
    uint16_t rawGyroX = (static_cast<uint16_t>(sensorData[8]) << 8) | sensorData[9];
    int16_t gyroX = static_cast<int16_t>(rawGyroX);
    uint16_t rawGyroY = (static_cast<uint16_t>(sensorData[10]) << 8) | sensorData[11];
    int16_t gyroY = static_cast<int16_t>(rawGyroY);
    uint16_t rawGyroZ = (static_cast<uint16_t>(sensorData[12]) << 8) | sensorData[13];
    int16_t gyroZ = static_cast<int16_t>(rawGyroZ);

    // Convert to physical units (default ±2g -> 16384 LSB/g, ±250°/s -> 131 LSB/(°/s))
    float gx = gyroX /  131.0;
    float gy = gyroY /  131.0;
    float gz = gyroZ /  131.0;

    accelerometerData.X = ax;
    accelerometerData.Y = ay;
    accelerometerData.Z = az;
    gyroData.X = gx;
    gyroData.Y = gy;
    gyroData.Z = gz;

    return true;
}

MPU6050::Vector3 MPU6050::getAccelerometer() const
{
    return accelerometerData;
}

MPU6050::Vector3 MPU6050::getGyro()  const
{
    return gyroData;
}

bool MPU6050::readRegister(uint8_t reg, uint8_t* buffer, size_t length)
{
    uint8_t tempBuffer[MAX_READ_LENGTH];
    if(buffer == nullptr || length ==0 || length > MAX_READ_LENGTH)
    {
        return false;
    }

    Wire.beginTransmission(imuAddress);
    if(Wire.write(reg)!= 1)
    {
        Wire.endTransmission(true);
        return false;
    }
    if(Wire.endTransmission(false) != 0)
    {
        return false;
    }

    size_t received_length = Wire.requestFrom(imuAddress, length, true);
    
    if(received_length != length)
    {
        return false;
    }
    for(size_t i=0;i<received_length;i++)
    {
        int recived_bytes = Wire.read();
        if(recived_bytes == -1)
        {
            return false;
        }
        tempBuffer[i] = static_cast<uint8_t>(recived_bytes);
    }
    for(size_t i =0; i< received_length; i++)
    {
        buffer[i] = tempBuffer[i];
    }
    
    
    return true;
}

bool MPU6050::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(imuAddress);
    if(Wire.write(reg) != 1)
    {
        Wire.endTransmission(true);
        return false;
    }
    if(Wire.write(value) != 1)
    {
        Wire.endTransmission(true);
        return false;
    }
    uint8_t status = Wire.endTransmission(true);
    return (status == 0);
}