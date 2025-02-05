import csv
import cv2
import con_arduino

cap = cv2.VideoCapture(0)
detector = cv2.QRCodeDetector()
ser = con_arduino.initialization('COM8')
wait_car = False
ceff = 2600


def load_table():
    table = {}
    with open('spaces.csv', 'r', newline='') as csvfile: # открываем файл с таблицей для чтения
        file = csv.reader(csvfile, delimiter=';', quotechar='|')
        for i in file:
            slot_num, auto_num = map(str, i)
            table[int(slot_num)] = auto_num # преобразуем в ввиде словаря
    return table


def save(table): # функция для записи словаря в файл с таблицей
    with open('spaces.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile, dialect='excel', delimiter=';')
        for i in table.items():
            writer.writerow(i)


def scan_qr_code():
    global wait_car
    while True:
        _, img = cap.read()
        data, box, _ = detector.detectAndDecode(img)

        while wait_car:
            _, img = cap.read()
            data, box, _ = detector.detectAndDecode(img)

            if box is None:
                wait_car = False

        if data:
            return data

        cv2.imshow("Camera", img)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


def process_qr_code(data, table):
    global wait_car
    if data in table.values():
        right_key = next(key for key, value in table.items() if value == data)
        
        con_arduino.post('unloading', ser)
        
        if ' ' in table.values():
            min_key = min(key for key, value in table.items() if value == ' ')
        else:
            min_key = 6

        con_arduino.post(ceff*(right_key - min_key), ser)

        while True:
            if con_arduino.get(ser) == "unloading end":
                break
            print(con_arduino.get(ser))

        table[right_key] = ' '

        con_arduino.post('green',ser)

        wait_car = True

    elif not(wait_car):
        

        if ' ' in table.values():
            con_arduino.post('loading', ser)
            min_key1 = min(key for key, value in table.items() if value == ' ')
            print(f'min slot : {min_key1}')
            table[min_key1] = data
            if ' ' in table.values():
                min_key2 = min(key for key, value in table.items() if value == ' ')
                con_arduino.post(ceff * (min_key2 - min_key1), ser)
            else:
                con_arduino.post(0,ser)
                con_arduino.post("red",ser)


            while True:
                if con_arduino.get(ser) == "loading end":
                    break

            con_arduino.post("green", ser)

        else:
            print('нет свободных мест')
            con_arduino.post('red', ser)

    save(table)


table = load_table()

while True:
    data = scan_qr_code()
    process_qr_code(data, table)
