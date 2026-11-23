import csv
from datetime import datetime
import os


def log_sensor_data(filename, sensor_data):

    header = [
        "timestamp",
        "accel_x", "accel_y", "accel_z",
        "gyro_x", "gyro_y", "gyro_z"
    ]

    # check if the file already exist and contains data
    file_exists = os.path.exists(filename)
    file_has_data = file_exists and os.path.getsize(filename) > 0

    with open(filename, mode='a', newline="") as file:
        writer = csv.writer(file)

        # Write the header only when creating an empty file
        if not file_has_data:
            writer.writerow(header)

        # Generate timestamp
        timestamp = datetime.now().isoformat(timespec="milliseconds")

        # Extract sensor values
        row = [
            timestamp,
            sensor_data["accel_x"],
            sensor_data["accel_y"],
            sensor_data["accel_z"],
            sensor_data["gyro_x"],
            sensor_data["gyro_y"],
            sensor_data["gyro_z"]
        ]

        writer.writerow(row)
