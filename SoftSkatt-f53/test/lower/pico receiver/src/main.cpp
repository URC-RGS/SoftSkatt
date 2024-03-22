// подключаем библиотеки 
#include <Arduino.h>
#include <AsyncStream.h>
#include <Config.h>


AsyncStream<100> serial1(&Serial1, '\n');
// TODO подобрать параметры измерения вольтажа

void setup() {
  // подключение отладочного сериала 
  Serial.begin(57600);

  Serial1.setRX(UART_RX);
  Serial1.setTX(UART_TX);
  
  Serial1.begin(9600);
}

void loop() {
  // если данные получены
  if (serial1.available()) {

    Serial.println(serial1.buf);
  }  
}
