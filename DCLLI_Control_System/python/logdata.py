import serial
import csv
import time

# Set the serial port and baud rate
serial_port = 'COM24'  # Replace 'COM1' with the appropriate serial port on your system
baud_rate = 115200

# Create a serial object
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Open a CSV file for writing sensor values
csv_sensor_file_path = 'sensor_values.csv'
with open(csv_sensor_file_path, mode='w', newline='') as sensor_file:
    sensor_writer = csv.writer(sensor_file)
    sensor_writer.writerow(['Reference_Lux', 'ILLuminance', 'PWM'])  # Write header to CSV file

    # Open a CSV file for writing 'ILLuminance', 'Reference_Lux', and 'PWM' data
    csv_data_file_path = 'data.csv'
    
    with open(csv_data_file_path, mode='w', newline='') as data_file:
        data_writer = csv.writer(data_file)
        data_writer.writerow(['Reference_Lux', 'ILLuminance', 'PWM'])  # Write header to CSV file

        try:
            while True:
                # Read the line from the serial port
                line = ser.readline().decode('utf-8').strip()

                # Check if the line is not empty
                if line:
                    # Split the line into values
                    values = line.split(',')

                    # Write the sensor values to the sensor CSV file
                    sensor_writer.writerow(values)
                    
                    # Print the sensor values to the console (optional)
                    print("Reference_Lux:", values[0], "ILLuminance:", values[1], "PWM:", values[2])

                    # Extract 'Reference_Lux', 'ILLuminance', and 'PWM' values
                    if line.startswith("REF:"):
                        parts = line.split(":")
                        reference_lux = int(parts[1].split()[0])
                        illuminance = float(parts[3].split()[0])
                        pwm = int(parts[5].split()[0])

                        # Print the values to the console (optional)
                        print("Reference_Lux:", reference_lux, "ILLuminance:", illuminance, "PWM:", pwm)

                        # Write the values to the data CSV file
                        data_writer.writerow([reference_lux, illuminance, pwm])

                    # Flush the CSV files to ensure data is written
                    sensor_file.flush()
                    data_file.flush()

                    # Add a delay if needed to avoid overwhelming the serial port
                    time.sleep(1)

        except KeyboardInterrupt:
            # Close the serial port and the CSV files when the script is interrupted
            ser.close()
            sensor_file.close()
            data_file.close()
            print("Serial port closed.")
