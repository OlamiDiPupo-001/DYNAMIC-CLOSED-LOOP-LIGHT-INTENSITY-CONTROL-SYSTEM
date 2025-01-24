import serial
import numpy as np
import time

def send_duty(ser, duty_cycle):
    message = f"PWM = {duty_cycle}\r"
    ser.write(message.encode())
    print(f"Sent: {message}")

def read_sensor_data(ser):
    message = ser.readline().decode().strip()
    print(f"Read: {message}")

    # Extracting values from the serial message using regular expressions
    import re
    match = re.match(r"Reference_Lux = (\d+),\s+ILLuminance = (\d+\.\d+),\s+PWM = (\d+)", message)
    if match:
        illuminance, pwm = map(float, match.groups()[1:])  # Extract only illuminance and PWM
        return illuminance, pwm
    else:
        print("Error: Invalid serial data format")
        return None

def save_data_to_numpy(timestamps, illuminance, pwm, file_path):
    np.savez(file_path, timestamps=timestamps, illuminance=illuminance, pwm=pwm)

def main():
    # Replace 'COM1' with your actual serial port
    serial_port = 'COM24'
    baud_rate = 115200

    ser = serial.Serial(serial_port, baud_rate, timeout=1)
    send_duty(ser, 100)
    
    illuminance_readout = []
    pwm_readout = []
    timestamps = []
    t = 0
    ts = 1 
    file_path = "sensor_data.npz"  # Define file_path here

    try:
        while True:
            sensor_data = read_sensor_data(ser)
            if sensor_data is not None:
                illuminance, pwm = sensor_data
                illuminance_readout.append(illuminance)
                pwm_readout.append(pwm)
                timestamps.append(t)
                t += ts
            time.sleep(ts)  # Add a delay to control the sampling rate

    except KeyboardInterrupt:
        pass
    finally:
        ser.close()

    print("Done.")
    
    save_data_to_numpy(timestamps, illuminance_readout, pwm_readout, file_path)
    print(f"Data saved to {file_path}")

if __name__ == "__main__":
    main()
