/*
   -- Теплица --
*/


// определение режима соединения и подключение библиотеки RemoteXY 
#define REMOTEXY_MODE__SOFTSERIAL
#include <SoftwareSerial.h>

#include <RemoteXY.h>

// настройки соединения 
#define REMOTEXY_SERIAL_RX 2
#define REMOTEXY_SERIAL_TX 3
#define REMOTEXY_SERIAL_SPEED 9600


// конфигурация интерфейса  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,3,0,1,0,213,0,10,13,1,
  2,0,18,11,27,12,2,26,31,31,
  208,146,208,154,208,155,0,208,146,208,
  171,208,154,208,155,0,2,0,18,33,
  27,12,2,26,31,31,208,146,208,154,
  208,155,0,208,146,208,171,208,154,208,
  155,0,2,0,19,55,27,12,2,26,
  31,31,208,146,208,154,208,155,0,208,
  146,208,171,208,154,208,155,0,129,0,
  25,3,14,6,17,208,160,208,181,208,
  187,208,181,0,129,0,15,26,33,6,
  17,208,161,208,178,208,181,209,130,208,
  190,208,180,208,184,208,190,208,180,0,
  65,4,27,86,9,9,129,0,17,49,
  31,6,17,208,148,208,178,208,184,208,
  179,208,176,209,130,208,181,208,187,209,
  140,0,129,0,15,70,32,6,17,208,
  152,208,189,208,180,208,184,208,186,208,
  176,209,130,208,190,209,128,0,129,0,
  10,77,43,6,17,208,190,209,129,208,
  178,208,181,209,137,208,181,208,189,208,
  189,208,190,209,129,209,130,208,184,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t relay; // =1 if switch ON and =0 if OFF 
  uint8_t led; // =1 if switch ON and =0 if OFF 
  uint8_t motor; // =1 if switch ON and =0 if OFF 

    // output variables
  uint8_t lightFlag; // =0..255 LED Red brightness 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

#define POT A0  
#define LDR A1

uint8_t message[5];

void setup() 
{
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  Serial.begin(9600);
  RemoteXY_Init (); 
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  // считываем дискретные сигналы
  message [0] = RemoteXY.relay;
  message [1] = RemoteXY.led;
  message [2] = RemoteXY.motor;
  // преобразуем значение потенциометра для сервопривода
  message [3] = analogRead(POT)/10.24;

  // сигнал с фоторезистора преобразуется в дискретный
  if (analogRead(LDR) > 400) 
  {
     RemoteXY.lightFlag = 255; // индикатор на телефоне горит
     message [4] = 1;
  }
  else 
  {
    RemoteXY.lightFlag = 0; // индикатор на не телефоне горит
    message [4] = 0;
  }

  // передаем данные 
  for ( uint8_t i = 0; i < sizeof(message);i++)
  {
    Serial.println(*(message + i));
  }

  delay(300);


}
