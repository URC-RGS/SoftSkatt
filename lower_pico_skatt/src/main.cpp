// подключаем библиотеки 
#include <Arduino.h>
#include <Servo.h>
#include <GParser.h>
#include <AsyncStream.h>
#include <ServoSmooth.h>
#include <Config.h>
#include <GyverFilters.h>
#include "DFRobot_BNO055.h"
#include "MS5837.h"
#include <Wire.h> 
#include <Adafruit_ADS1X15.h>


ServoSmooth servos[8];
AsyncStream<100> serialCom(&Serial1, '\n');
uint32_t turnTimer;
int ledState = LOW;

DFRobot_BNO055 mpu;
MS5837 sensor;
Adafruit_ADS1115 ads;

int16_t adc0;

// поправки 
float yaw_defolt = 0.0;
float depth_defolt = 0.13;

// переменные телеметрии 
uint16_t yaw = 0;
uint16_t temp = 0;
uint16_t depth = 0;
uint16_t volt = 0;


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  // подключение отладочного сериала 
  Serial.begin(BITRATE);
  // подключение сериала для общения с постом управления 
  Serial1.setRX(UART_RX);
  Serial1.setTX(UART_TX);
  pinMode(UART_COM, OUTPUT);
  digitalWrite(UART_COM, LOW);
  Serial1.begin(BITRATE);

  // подключаем моторы 
  servos[0].attach(PIN_MOTOR_0, 1000, 2000);
  servos[0].setSpeed(SPEED_MOTORS);
  servos[0].setAccel(ACCEL_MOTORS);
  servos[0].setAutoDetach(false);

  servos[1].attach(PIN_MOTOR_1, 1000, 2000);
  servos[1].setSpeed(SPEED_MOTORS);
  servos[1].setAccel(ACCEL_MOTORS);
  servos[1].setAutoDetach(false);

  servos[2].attach(PIN_MOTOR_2, 1000, 2000);
  servos[2].setSpeed(SPEED_MOTORS);
  servos[2].setAccel(ACCEL_MOTORS);
  servos[2].setAutoDetach(false);

  servos[3].attach(PIN_MOTOR_3, 1000, 2000);
  servos[3].setSpeed(SPEED_MOTORS);
  servos[3].setAccel(ACCEL_MOTORS);
  servos[3].setAutoDetach(false);

  servos[4].attach(PIN_MOTOR_4, 1000, 2000);
  servos[4].setSpeed(SPEED_MOTORS);
  servos[4].setAccel(ACCEL_MOTORS);
  servos[4].setAutoDetach(false);

  servos[5].attach(PIN_MOTOR_5, 1000, 2000);
  servos[5].setSpeed(SPEED_MOTORS);
  servos[5].setAccel(ACCEL_MOTORS);
  servos[5].setAutoDetach(false);

  servos[6].attach(PIN_SERVO_CAM, 1000, 2000);
  servos[6].setSpeed(SPEED_SERVO);
  servos[6].setAccel(ACCELERATE_SERVO);
  servos[6].writeMicroseconds(1500);
  servos[6].setAutoDetach(false);

  servos[7].attach(PIN_SERVO_ARM, 1000, 2000);
  servos[7].setSpeed(SPEED_SERVO);
  servos[7].setAccel(ACCELERATE_SERVO);
  servos[7].writeMicroseconds(1500);
  servos[7].setAutoDetach(false);

  servos[0].writeMicroseconds(2000);
  servos[1].writeMicroseconds(2000);
  servos[2].writeMicroseconds(2000);
  servos[3].writeMicroseconds(2000);
  servos[4].writeMicroseconds(2000);
  servos[5].writeMicroseconds(2000);
  delay(5000);

  servos[0].writeMicroseconds(1500);
  servos[1].writeMicroseconds(1500);
  servos[2].writeMicroseconds(1500);
  servos[3].writeMicroseconds(1500);
  servos[4].writeMicroseconds(1500);
  servos[5].writeMicroseconds(1500);
  delay(5000);

  digitalWrite(LED_BUILTIN, LOW);

}

// цикл для первого ядра 
void loop() {
  // тикалка PWM выходов  
  if (millis()- turnTimer >= 15){
    turnTimer = millis();
    servos[0].tick();
    servos[1].tick();
    servos[2].tick();
    servos[3].tick();
    servos[4].tick();
    servos[5].tick();
    servos[6].tick();
    servos[7].tick();

    // мигалка для индикации работы
    if (ledState == LOW) ledState = HIGH;
    else ledState = LOW;

    digitalWrite(LED_BUILTIN, ledState);
  }
    
    
  // если данные получены
  if (serialCom.available()) {

    // парсим данные по резделителю возвращает список интов 
    GParser data = GParser(serialCom.buf, ' ');
    
    // вывод принимаемых пакетов с поста управления для отладки 
    Serial.println(serialCom.buf);

    // проверка на корректность пакета (проверяется колличетво интов после разбития строки)
    if (data.amount() == 11){

      int data_input[data.amount()];
      int am = data.parseInts(data_input);

      // отправляем значения движители 
      servos[0].writeMicroseconds(data_input[0]);
      servos[1].writeMicroseconds(3000 - data_input[1]);
      servos[2].writeMicroseconds(data_input[2]);
      servos[3].writeMicroseconds(3000 - data_input[3]);
      servos[4].writeMicroseconds(3000 - data_input[4]);
      servos[5].writeMicroseconds(data_input[5]);

      // отправляем значения на сервопривод камеры и манипулятора 
      servos[6].writeMicroseconds(data_input[8]);
      servos[7].writeMicroseconds(data_input[9]);

      // ответ посту 
      if (FEEDBEAK){

        digitalWrite(UART_COM, HIGH);
        delay(50);

        // ответ на пост управления, в перспективе отправка данных с датчика оринтеции 
        Serial1.print(yaw);
        Serial1.print(" ");
        Serial1.print(temp);
        Serial1.print(" ");
        Serial1.print(depth);
        Serial1.print(" ");
        Serial1.println(volt);

        delay(50);
        digitalWrite(UART_COM, LOW);
      }
    }
  }  
}

// цикл для второго ядра 
void loop1(){
  Wire.setSDA(20);
	Wire.setSCL(21);
  Wire.begin();

  sensor.init();
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // плотность воды 
  delay(1000);

  mpu.init();
  mpu.setMode(mpu.eNORMAL_POWER_MODE, mpu.eFASTEST_MODE);
  delay(1000);

  ads.setGain(GAIN_ONE);  
  ads.begin();
  delay(100);

  while (true) {
    // опрос датчиков ориентации и глубины 
    mpu.readEuler(); 
    delay(20);
    sensor.read();
    delay(20);
    adc0 = ads.readADC_SingleEnded(0);
    delay(20);

    // преобразуем float в int (умножаем на 100)
    yaw = ceil((mpu.EulerAngles.x - yaw_defolt) * 100);
    temp = ceil(sensor.temperature() * 100);
    depth = ceil((sensor.depth() - depth_defolt) * 100);
    volt =  ceil((adc0 * 0.00240) * 100);

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

    delay(100);
  }
}