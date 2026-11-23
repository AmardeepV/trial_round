from sensor_parser import parse_sensor_data


def test_valid_sensor_data():
    message = "DATA,1.01,-0.01,-0.13,-2.85,1.31,-0.04"
    error_message = "ERROR,1.01,-0.01,-0.13,-2.85,1.31,-0.04"
    missing_field_message = "DATA,1.01,-0.01,-0.13,-2.85,1.31"
    non_numeric_message = "DATA,hello,-0.01,-0.13,-2.85,1.31,-0.04"
    nan_message = "DATA,nan,-0.01,-0.13,-2.85,1.31,-0.04"
    accel_range_message = "DATA,2.5,-0.01,-0.13,-2.85,1.31,-0.04"
    gyro_range_message = "DATA,1.01,-0.01,-0.13,300,1.31,-0.04"

    output = parse_sensor_data(message)

    assert output is not None
    assert output["accel_x"] == 1.01
    assert output["gyro_x"] == -2.85
