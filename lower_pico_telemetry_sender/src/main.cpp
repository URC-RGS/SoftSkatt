// подключаем библиотеки 
#include <Arduino.h>
#include <GParser.h>
#include <AsyncStream.h>
#include <Config.h>
#include "DFRobot_BNO055.h"
#include "MS5837.h"
#include <Wire.h> 
// #include <Adafruit_ADS1X15.h>


AsyncStream<100> serialCom(&Serial1, '\n');

uint32_t turnTimer;
uint32_t workTimer;

int ledState = LOW;

DFRobot_BNO055 mpu;
MS5837 sensor;
// Adafruit_ADS1115 ads;

int16_t adc0;

// поправки 
float yaw_defolt = 0.0;
float depth_defolt = 0.17;

// переменные телеметрии 
uint16_t yaw = 0;
uint16_t temp = 0;
uint16_t depth = 0;
uint16_t volt = 0;


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  // подключение отладочного сериала 
  Serial.begin(BITRATE);
  // подключение сериала для общения с постом управления 
  Serial1.setRX(UART_RX);
  Serial1.setTX(UART_TX);
  pinMode(UART_COM, OUTPUT);
  digitalWrite(UART_COM, HIGH);
  Serial1.begin(BITRATE);

  Wire.setSDA(20);
	Wire.setSCL(21);
  Wire.begin();
  delay(100);

  sensor.init();
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // плотность воды 
  delay(100);

  mpu.init();
  mpu.setMode(mpu.eNORMAL_POWER_MODE, mpu.eFASTEST_MODE);
  delay(100);

  // ads.setGain(GAIN_ONE);  
  // ads.begin();
  // delay(100);

}

// цикл для первого ядра 
void loop() {

  if (millis()- turnTimer >= 50){
    turnTimer = millis();

    // мигалка для индикации работы
    if (ledState == LOW) ledState = HIGH;
    else ledState = LOW;

    digitalWrite(LED_BUILTIN, ledState);
  }

  if (millis() - workTimer >= 200){
    workTimer = millis();

    // опрос датчиков ориентации и глубины 
    mpu.readEuler(); 
    sensor.read();
    // adc0 = ads.readADC_SingleEnded(0);

    // преобразуем float в int (умножаем на 100)
    yaw = ceil((mpu.EulerAngles.x - yaw_defolt) * 100);
    temp = ceil(sensor.temperature() * 100);
    depth = ceil((sensor.depth() - depth_defolt) * 100);
    // volt =  ceil((adc0 * 0.00240) * 100);

    // для отладки можем выводить показания в консоль 
    if (DEBUG){
      Serial.print(yaw);
      Serial.print(" ");
      Serial.print(temp);
      Serial.print(" ");
      Serial.print(depth);
      Serial.print(" ");
      Serial.println(volt);
    }

    Serial1.print(yaw);
    Serial1.print(" ");
    Serial1.print(temp);
    Serial1.print(" ");
    Serial1.print(depth);
    Serial1.print(" ");
    Serial1.println(volt);

  }
}

