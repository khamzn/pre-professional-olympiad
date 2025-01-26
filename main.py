import csv
import cv2

cap = cv2.VideoCapture(0)
detector = cv2.QRCodeDetector()
wait_car = False

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


def procces_qr_code(data, table):
    global wait_car
    if data in table.values():
        print('unloading')

        right_key = next(key for key, value in table.items() if value == data)
        table[right_key] = ' '
        print('led is green')

        while True:

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        wait_car = True

    elif not(wait_car):
        print('loading')

        if ' ' in table.values():
            min_key = min(key for key, value in table.items() if value == ' ')
            print(f'min slot : {min_key}')
            table[min_key] = data
            while True:

                if cv2.waitKey(1) & 0xFF == ord('q'):
                    break
        else:
            print(table)
            print('нет свободных мест')
            print('led is red')

    save(table)


table = load_table()

while True:
    data = scan_qr_code()
    procces_qr_code(data, table)
