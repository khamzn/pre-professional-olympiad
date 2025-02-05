import serial
import time
# for Raspberry pi use 'dev/ttyUSB0'


def initialization(port):
    ser = serial.Serial(port, 9600, timeout=1)
    ser.flush()
    return ser


def post(data, ser):
    ser.flush()
    ser.write((f"{data}\n").encode('utf-8'))
    print(f'отправлено: {data}')


def get(ser):
    ser.flush()
    line = ser.readline().decode('utf-8').rstrip()
    return line
