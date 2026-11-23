import serial
import time
from sensor_parser import parse_sensor_data
from csv_logger import log_sensor_data


def serial_comm(port, baudrate):
    arduino_serial = None
    try:
        arduino_serial = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)

        while True:
            try:
                serial_receive = arduino_serial.readline().decode(
                    'ascii', errors="replace").strip()

                imu_data = parse_sensor_data(serial_receive)
                if imu_data is not None:
                    # print(imu_data)
                    log_sensor_data("sensor_data.csv", imu_data)

            except serial.SerialException as err:
                print(f"Serial communication error: {err}")
                return

    except (PermissionError, serial.SerialException) as err:
        print(
            f"could not open serial connection: {err}")
    finally:
        arduino_serial.close()


if __name__ == "__main__":
    serial_comm("/dev/ttyUSB0", 115200)
