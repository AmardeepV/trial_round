import math


def parse_sensor_data(serial_receive):
    data = serial_receive.split(",")

    if data[0] != 'DATA' and len(data) != 7:
        return None

    if data[0] == 'DATA' and len(data) == 7:
        try:
            accel_x = float(data[1])
            accel_y = float(data[2])
            accel_z = float(data[3])

            gyro_x = float(data[4])
            gyro_y = float(data[5])
            gyro_z = float(data[6])

            sensorData = [accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z]

            if not all(math.isfinite(value) for value in sensorData):
                return None

            if not all(-2.0 <= value <= 2.0 for value in sensorData[0:3]):
                print("Accelerometer data is out of expected range")
                return None

            if not all(-250.0 <= value <= 250.0 for value in sensorData[3:6]):
                print("Gyro data is out of range")
                return None

            imu_data = {
                "accel_x": accel_x,
                "accel_y": accel_y,
                "accel_z": accel_z,
                "gyro_x":  gyro_x,
                "gyro_y":  gyro_y,
                "gyro_z":  gyro_z
            }

            return imu_data

        except ValueError:
            print("Sensor data contain non numeric value")
            return None
