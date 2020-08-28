

#include <dht11.h>
// определение режима соединения и подключение библиотеки RemoteXY 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT
#include <RemoteXY.h>

// настройки соединения 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377
// настройка пинов
#define RELAY 13
#define DHT_PIN 5 


// конфигурация интерфейса  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,1,0,22,0,119,0,10,5,1,
  2,0,14,20,34,15,2,26,31,31,
  208,146,208,154,208,155,0,208,146,208,
  171,208,154,208,155,0,129,0,24,10,
  18,6,17,208,160,208,181,208,187,208,
  181,0,129,0,15,40,33,6,17,208,
  146,208,187,208,176,208,182,208,189,208,
  190,209,129,209,130,209,140,0,67,4,
  19,49,24,7,2,26,11,129,0,13,
  62,38,6,17,208,162,208,181,208,188,
  208,191,208,181,209,128,208,176,209,130,
  209,131,209,128,208,176,0,67,4,19,
  71,24,7,2,26,11 };
  
// структура определяет все переменные и события вашего интерфейса управления 
struct {

    // input variables
  uint8_t switch_relay; // =1 если переключатель включен и =0 если отключен 

    // output variables
  char text_temperature[11];  // =строка UTF8 оканчивающаяся нулем 
  char text_humidity[11];  // =строка UTF8 оканчивающаяся нулем 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

dht11 dht;


void setup() 
{
  RemoteXY_Init ();  
  #ifdef DBG
    Serial.begin(9600);
  #endif
  pinMode(RELAY,OUTPUT);
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  dht.read(DHT_PIN);
  
  digitalWrite(RELAY,RemoteXY.switch_relay);

  dtostrf(dht.temperature,0,1,RemoteXY.text_temperature);
  dtostrf(dht.humidity,0,1,RemoteXY.text_humidity);
#ifdef DBG
  //Serial.print(RemoteXY.text_humidity + '\t');
  Serial.println(dht.humidity);

  //Serial.print(RemoteXY.text_temperature + '\t');
  Serial.println(dht.temperature);
#endif

}
