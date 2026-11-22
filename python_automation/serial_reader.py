import serial
import time


def serial_comm(port, baudrate):
    arduino_serial = serial.Serial(port, baudrate, timeout=1)
    time.sleep(2)

    while True:
        serial_receive = arduino_serial.readline().decode(
            'ascii', errors="replace").strip()

        data = serial_receive.split(",")
        parse_sensor_data(data)


def parse_sensor_data(serial_line):
    if serial_line[0] == 'DATA' and len(serial_line) == 7:
        if not -2.0 <= accel_x <= 2.0:
            print("Accelerometer X out of expected range")

        accel_x = float(serial_line[1])
        accel_y = float(serial_line[2])
        accel_z = float(serial_line[3])

        gyro_x = float(serial_line[4])
        gyro_y = float(serial_line[5])
        gyro_z = float(serial_line[6])

        # print("Accelerometer (g):")
        # print(f"X: {accel_x}")
        # print(f"Y: {accel_y}")
        # print(f"Z: {accel_z}")

        # print("Gyroscope (degrees/s):")
        # print(f"X: {gyro_x}")
        # print(f"Y: {gyro_y}")
        # print(f"Z: {gyro_z}")
        imu_data = {
            "accel_x": accel_x,
            "accel_y": accel_y,
            "accel_z": accel_z,
            "gyro_x":  gyro_x,
            "gyro_y":  gyro_y,
            "gyro_z":  gyro_z
        }

        return imu_data


if __name__ == "__main__":
    serial_comm("/dev/ttyUSB0", 115200)
