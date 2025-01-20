import cv2
import csv
import con_arduino

cap = cv2.VideoCapture(0)
detector = cv2.QRCodeDetector()
table = {}
unload = False
wait_car = False
exit_car = False
right_key = None
ser = con_arduino.initialization('COM4')

with open('spaces.csv', 'r', newline='') as csvfile: # открываем файл с таблицей для чтения
    file = csv.reader(csvfile, delimiter=';', quotechar='|')
    for i in file:
        slot_num, auto_num = map(str, i)
        table.update([(int(slot_num), auto_num)]) # преобразуем в ввиде словаря


def save(table): # функция для записи словаря в файл с таблицей
    with open('spaces.csv', 'w', newline='') as csvfile:
        writer = csv.writer(csvfile, dialect='excel', delimiter=';')
        for i in table.items():
            writer.writerow(i)


def exiting(data):
    global unload
    global right_key
    global wait_car
    global exit_car

    if data.startswith('1'):
        right_key = next(key for key, value in table.items() if value == data[1:])

    if data == table.get(right_key):  # Проверка  выехала ли машина

        print('Выгрузка завершена, заберите машину')

        wait_car = True

    if exit_car:
        print('Успешно')
        unload = False
        wait_car = False
        exit_car = False
        table.update([(right_key, None)])


def loading(data):
    min_key = min(key for key, value in table.items() if value == '')  # Нахождение минимальной свободной ячейки

    print(f'Номер машины: {data}, Нажмите кнопку для продолжения')
    con_arduino.post('loading', ser)

    while True: # Ожидание нажатия кнопки

        # if con_arduino.get(ser) == 'end_loading':
        #     break

        if cv2.waitKey(1) == ord("q"):
            break

    table.update([(min_key, data)])


def space_check(data):
    global unload

    if not ('' in table.values()) and not data.startswith('1'):  # проверка на наличие в словаре значений None, т.е свободных мест
        print('Нет свободных мест!')

    else:
        if (data.startswith('1') and data[1:] in table.values()) or unload:
            # Если qr код у владельца машины начинается на цифру 1, сл. происходит проверка на qr код владельца и наличие его машины в парковке

            if not(unload):
                print(f'Номер машины: {data[1:]}, Процесс выгрузки')
                con_arduino.post('unloading', ser)

                # вычисление нужной ячейки с учёиом qr кода владельца

                unload = True

            else:
                exiting(data)

        if not (data in table.values()) and not(data.startswith('1')) and not (unload):
            loading(data)


while True:
    _, img = cap.read()
    data, box, _ = detector.detectAndDecode(img)
    if box is not None:
        if data:
            space_check(data)
            save(table)
    elif wait_car:
        if box is None:
            exit_car = True
            space_check(data)
            save(table)
    cv2.imshow('img', img)
    if cv2.waitKey(1) == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()
