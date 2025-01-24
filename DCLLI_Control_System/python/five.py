import serial
import time

# Set the serial port and baud rate
serial_port = 'COM24'  # Replace 'COM1' with the appropriate serial port on your system
baud_rate = 115200

# Create a serial object
ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Open a file for writing
file_path = 'sensor_values.log'
with open(file_path, 'w') as file:
    file.write("Reference_Lux,ILLuminance,PWM\n")

    try:
        while True:
            # Read the line from the serial port
            line = ser.readline().decode('utf-8').strip()

            # Check if the line is not empty
            if line:
                # Split the line into values
                values = line.split(',')

                # Write the values to the file
                file.write(','.join(values) + '\n')

                # Print the values to the console (optional)
                print("Reference_Lux:", values[0], "ILLuminance:", values[1], "PWM:", values[2])

                # Add a delay if needed to avoid overwhelming the serial port
                time.sleep(1)

    except KeyboardInterrupt:
        # Close the serial port and the file when the script is interrupted
        ser.close()
        print("Serial port closed.")
