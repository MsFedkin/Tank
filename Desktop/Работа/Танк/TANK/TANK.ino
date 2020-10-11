#include <SPI.h>                                          // Подключаем библиотеку  для работы с шиной SPI
#include <nRF24L01.h>                                     // Подключаем файл настроек из библиотеки RF24
#include <RF24.h>                                         // Подключаем библиотеку  для работы с nRF24L01+

#define DBG
RF24  radio(9, 10); 

enum
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  ARRAY_SIZE
};

enum
{
  MOTOR_PIN1 = 7, // 2 пин на L293D
  MOTOR_PIN2 = 6, // 3 пин на L293D
  MOTOR_PIN3 = 5, // 6 пин на L293D
  MOTOR_PIN4 = 4, // 7 пин на L293D
  PINS_NUM
};

uint8_t radioInput [ARRAY_SIZE];
uint8_t *motor =  new uint8_t[PINS_NUM] {MOTOR_PIN1,MOTOR_PIN2,MOTOR_PIN3,MOTOR_PIN4};
void setup() 
{
  #ifdef DBG
    Serial.begin(9600);
  #endif
  radio.begin();                                        // Инициируем работу nRF24L01+
  radio.setChannel(0x76);                               // Указываем канал приёма данных (от 0 до 127)
  radio.setDataRate     (RF24_1MBPS);                   // Указываем скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
  radio.setPALevel      (RF24_PA_HIGH);                 // Указываем мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)

  radio.openReadingPipe (0,0xF0F0F0F0E1LL);            // Открываем i трубу с идентификатором F0F0F0F0E1LL для приема данных (на ожном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
  radio.startListening();  

  for(uint8_t i = 0; i < PINS_NUM;i++)
  {
    pinMode(*(motor+i),OUTPUT);
  }
  delete [] motor;
}

void loop() 
{
  delay(100);
  radio.read(&radioInput,ARRAY_SIZE);

  #ifdef DBG
      for(uint8_t i = 0; i < ARRAY_SIZE; i++)
      {
        Serial.print(radioInput[i]);
      }
      Serial.println();
  #endif

  if(radioInput[FORWARD])
  {
    digitalWrite(MOTOR_PIN1,HIGH);
    digitalWrite(MOTOR_PIN2,LOW);
    digitalWrite(MOTOR_PIN3,HIGH);
    digitalWrite(MOTOR_PIN4,LOW);
  }
  else if (radioInput[BACKWARD])
  {
    digitalWrite(MOTOR_PIN1,LOW);
    digitalWrite(MOTOR_PIN2,HIGH);
    digitalWrite(MOTOR_PIN3,LOW);
    digitalWrite(MOTOR_PIN4,HIGH);
  }
  else if (radioInput[LEFT])
  {
    digitalWrite(MOTOR_PIN1,HIGH);
    digitalWrite(MOTOR_PIN2,LOW);
    digitalWrite(MOTOR_PIN3,LOW);
    digitalWrite(MOTOR_PIN4,HIGH);
  }
  else if (radioInput[RIGHT])
  {
    digitalWrite(MOTOR_PIN1,LOW);
    digitalWrite(MOTOR_PIN2,HIGH);
    digitalWrite(MOTOR_PIN3,HIGH);
    digitalWrite(MOTOR_PIN4,LOW);
  }
  else
  {
    digitalWrite(MOTOR_PIN1,LOW);
    digitalWrite(MOTOR_PIN2,LOW);
    digitalWrite(MOTOR_PIN3,LOW);
    digitalWrite(MOTOR_PIN4,LOW);
  }
  
  

}
