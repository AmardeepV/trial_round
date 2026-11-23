import serial
import time
from sensor_parser import parse_sensor_data


def hil_testing():
    serial_connection = None
    total_data = 0
    total_invalid_data = 0
    port = "/dev/ttyUSB0"
    test_duration = 10

    print("HIL TEST: MPU6050 telemetry")

    try:
        print(f"Connecting to {port}")
        serial_connection = serial.Serial(port, 115200, timeout=1)
        time.sleep(2)
        print("Connection successful.")

        start_time = time.monotonic()
        print(f"Collecting telemetry for {test_duration} seconds...")

        while time.monotonic() - start_time < test_duration:
            serial_receive = serial_connection.readline().decode(
                'ascii', errors="replace").strip()

            # Ignore empty serial reads (timeout)
            if not serial_receive:
                continue

            sensor_data = parse_sensor_data(serial_receive)

            if sensor_data is not None:
                total_data += 1
            else:
                total_invalid_data += 1

        elapsed_time = time.monotonic() - start_time
        measurement_rate = total_data / elapsed_time if elapsed_time > 0 else 0

        print("\n========== HIL TEST REPORT ==========")
        print(f"Valid measurements:   {total_data}")
        print(f"Invalid measurements: {total_invalid_data}")
        print(f"Elapsed time:          {elapsed_time:.2f} seconds")
        print(f"Measurement rate:      {measurement_rate:.2f} Hz")

        if total_data == 0:
            print("HIL TEST FAILED")
        else:
            print("HIL TEST PASSED")

    except (PermissionError, serial.SerialException, OSError) as err:
        print(f"Serial communication error: {err}")

    finally:
        if serial_connection is not None and serial_connection.is_open:
            serial_connection.close()
            print("Serial connection closed.")


if __name__ == "__main__":
    hil_testing()
