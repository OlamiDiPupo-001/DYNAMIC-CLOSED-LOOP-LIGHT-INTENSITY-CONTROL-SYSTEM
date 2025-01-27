import serial
import csv
import time
import re

# Set the serial port and baud rate
serial_port = 'COM24'  # Replace 'COM1' with the appropriate serial port on your system
baud_rate = 115200

# Create a serial object
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Open a CSV file for writing sensor and data values
csv_file_path = 'sensor_and_data_values.csv'
with open(csv_file_path, mode='w', newline='') as csv_file:
    csv_writer = csv.writer(csv_file)
    csv_writer.writerow(['Reference_Lux', 'ILLuminance', 'PWM'])  # Write header to CSV file

    try:
        while True:
            # Read the reference value from the user in the terminal
            reference_value = float(input("Enter Reference_Lux value: "))

            # Print the reference value
            print(f"Reference_Lux set to: {reference_value}")

            # Send the updated reference value to the serial port
            ser.write(f"REF:{reference_value}\n".encode('utf-8'))

            # Loop to continuously read and log sensor values
            while True:
                # Read the line from the serial port
                line = ser.readline().decode('utf-8').strip()

                # Check if the line is not empty
                if line:
                    # Extract numerical values using regular expressions
                    values = [float(val) for val in re.findall(r"[-+]?\d*\.\d+|\d+", line)]

                    # Update the Reference_Lux value
                    values[0] = reference_value

                    # Print the values to the console
                    print(f"Reference_Lux: {values[0]} ILLuminance: {values[1]} PWM: {values[2]}")

                    # Write the values to the CSV file with commas and a newline
                    csv_file.write(f"{values[0]}, {values[1]}, {values[2]},\n")

                    # Add a delay if needed to avoid overwhelming the serial port
                    time.sleep(1)

    except KeyboardInterrupt:
        # Close the serial port and the CSV file when the script is interrupted
        ser.close()
        print("Serial port closed.")
