#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку  для работы с nRF24L01+
#include <SimpleModbusSlave.h> //библиотке для MODBUS

RF24  radio(9, 10);                         // Создаём объект radio   для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)

enum 
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  HOLDING_REGS_SIZE
};

uint16_t holdingRegsForScada[HOLDING_REGS_SIZE]; // массив с нашими перемеными для скада
uint8_t holdingRegsForNRF [HOLDING_REGS_SIZE]; // массив с нашими перемеными для радио

void setup() 
{
  modbus_configure(&Serial, 9600, SERIAL_8N2, 1, 2, HOLDING_REGS_SIZE, holdingRegsForScada);
  modbus_update_comms(9600, SERIAL_8N2, 1);

  radio.begin();                                     // Инициируем работу nRF24L01+
  radio.setChannel(0x76);                            // Указываем канал приёма данных (от 0 до 127)
  radio.setDataRate     (RF24_1MBPS);                // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);              // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
  radio.openWritingPipe (0xF0F0F0F0E1LL);            // Открываем i трубу с идентификатором F0F0F0F0E1LL для приема данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
}

void loop() 
{
  modbus_update(); // обмен даными по ModBus

  for(uint8_t i = 0; i < sizeof(holdingRegsForNRF);i++)
  {
    holdingRegsForNRF[i] = holdingRegsForScada[i];
  }

  radio.write(&holdingRegsForNRF,sizeof(holdingRegsForNRF));
}
