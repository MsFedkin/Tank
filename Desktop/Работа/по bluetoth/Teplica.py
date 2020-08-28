
import RPi.GPIO as pin
import serial
pin.setmode(pin.BCM)

motor = 26 
relay = 17
led = 27
ldr = 22
servo = 20

for i in motor,relay,led,ldr,servo:
    pin.setup(i,pin.OUT)

#Объявление сервоприрада
servoPWM = pin.PWM(servo, 50)     # создаем ШИМ-объект для пина pinPWM с частотой 100 Гц
servoPWM.start(0)                  # Запускаем ШИМ на пине со скважностью 0% (0-100%)

#настройка порта
port = "/dev/ttyACM0" # имя USB порта
baud = 9600 # Бод
s = serial.Serial(port) #Обявления порта
s.baudrate = baud #устанавливаем количество бод
s.parity = serial.PARITY_NONE
s.databits = serial.EIGHTBITS
s.stopbits = serial.STOPBITS_ONE

i = 0 # индекс массива
n = 0 # номер цикла

while True:
    data = s.readline()
    try: 
        i = i % 5 # обнуление индекса
        data = data.decode(('utf-8;')).rstrip() #преобразованеи полученной инфы
        #для обработки дискртных сигналов
        if i == 0: 
            pin.output(relay,bool(int(data)))
        if i == 1: 
            pin.output(led,bool(int(data)))
        if i == 2: 
            pin.output(motor,bool(int(data)))
        if i == 3: # сервопривод
            dutyCycle = 0.9 * float(data)/ 18. + 3. # поворот серво от 0 до  90 градусов
            servoPWM.ChangeDutyCycle(dutyCycle)
        if i == 4: #Первый аналоговый сигнал
            pin.output(ldr,bool(int(data)))
            
        print('-',i,' ',data)
        i +=1
        if i == 5:
            n +=1
            print(n,end = "")
            print("---------------")
            
    except IndexError:
        print("Переполнение индекса" , i )
        print("обнуление индекса")
        i = 0 
