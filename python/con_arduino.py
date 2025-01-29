import serial
# for Raspberry pi use 'dev/ttyUSB0'


def initialization(port):
    ser = serial.Serial(port, 9600, timeout=1)
    ser.flush()
    return ser


def post(data, ser):
    ser.write((f"{data}\n").encode())
    print(f'отправлено: {data}')


def get(ser):
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(line)

