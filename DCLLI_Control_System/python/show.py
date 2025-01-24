import serial
import tkinter as tk

def send_reference_value(ser, reference_value):
    message = f"Reference_Lux = {reference_value}, ILLuminance = 0.0, PWM = 0\r"
    ser.write(message.encode())
    print(f"Sent: {message}")

def set_reference_value():
    reference_value = float(entry.get())
    send_reference_value(ser, reference_value)

# Replace 'COM1' with your actual serial port
serial_port = 'COM24'
baud_rate = 115200

ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Set up the GUI
root = tk.Tk()
root.title("Reference Value Setter")

# Entry for user to input reference value
entry_label = tk.Label(root, text="Enter Reference Value:")
entry_label.pack()
entry = tk.Entry(root)
entry.pack()

# Button to set reference value
set_button = tk.Button(root, text="Set Reference Value", command=set_reference_value)
set_button.pack()

# Close the serial connection when the GUI is closed
root.protocol("WM_DELETE_WINDOW", lambda: [ser.close(), root.destroy()])

root.mainloop()
