import serial
import time

ser = serial.Serial('/dev/ttyS0',9600);

print(ser.is_open)

def write(text):
    ser.write(text + "/n".encode())

def read():
    msg = []
    while ser.in_waiting == False:
        pass
    
    while ser.in_waiting:
        data = ser.read()
        value = data.decode()
        message.append(value)
        time.sleep(.1)
    return msg

ser.write("all work and no play makes dennis a dull boy\n".encode())

    #   <message>
ser.close