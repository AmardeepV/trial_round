#include<MPU6050.h>

bool MPU6050::begin()
{
    return Wire.begin();
}

bool MPU6050::update()
{

}

MPU6050::Vector3 MPU6050::getAccelerometer() const
{

}

MPU6050::Vector3 MPU6050::getGyro()  const
{

}

bool MPU6050::readRegister(uint8_t reg, uint8_t* buffer, size_t length)
{
    if(buffer == nullptr || length ==0)
    {
        return false;
    }

    Wire.beginTransmission(imuAddress);
    Wire.write(reg);
    uint8_t error = Wire.endTransmission(false);
        
    if (error !=0)
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
        buffer[i] = recived_bytes;
    }

    return true;
}

bool MPU6050::writeRegister(uint8_t reg, uint8_t value)
{

}