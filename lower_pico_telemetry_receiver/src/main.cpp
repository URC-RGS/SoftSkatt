// подключаем библиотеки 
#include <Arduino.h>
#include <GParser.h>
#include <AsyncStream.h>
#include <Config.h>
#include <GyverFilters.h>
#include <LiquidCrystal_I2C.h>

AsyncStream<100> serial(&Serial, '\n');
AsyncStream<100> serial1(&Serial1, '\n');
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

uint32_t turnTimer;
uint32_t workTimer;
int ledState = LOW;

float yaw = 0;
float temp = 0;
float depth = 0;
float volt = 0;

void setup() {

  Wire.setSDA(20);
	Wire.setSCL(21);
  Wire.begin();
  delay(100);

  Serial.begin(BITRATE);

  // подключение сериала для общения с роботом 
  Serial1.setRX(UART_0_RX);
  Serial1.setTX(UART_0_TX);
  Serial1.begin(BITRATE);
  pinMode(UART_COM, OUTPUT);
  digitalWrite(UART_COM, LOW);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(19, OUTPUT);

  lcd.init();
  lcd.backlight();

  delay(100);
}

void loop() {
  // мониторим порт общения с роботом 
  if (serial1.available()) {

    if (DEBUG) Serial.println(serial1.buf);
    
    GParser data = GParser(serial1.buf, ' ');

    if (data.amount() == 4) {
      int data_input[data.amount()];
      int am = data.parseInts(data_input);
      float data0 = data_input[0];
      float data1 = data_input[1];
      float data2 = data_input[2];
      float data3 = data_input[3];
      yaw = data0 / 100;
      temp = data1 / 100;
      depth = data2 / 100;
      volt = data3 / 100;

    }
  }

  // мигалка для индикации работы 
  if (millis()- turnTimer >= 200){
    turnTimer = millis();

    if (ledState == LOW) ledState = HIGH;
    else ledState = LOW;

    digitalWrite(LED_BUILTIN, ledState);

  }
}


void loop1() {
  if (millis() - workTimer >= 350){
    workTimer = millis();

    Serial.print("yaw: "); 
    Serial.print(yaw); 
    Serial.print("  "); 

    Serial.print("temp: ");
    Serial.print(temp);
    Serial.print("  ");

    Serial.print("depth: ");
    Serial.print(depth); 
    Serial.print("  ");

    Serial.print("volt: ");
    Serial.print(volt); 
    Serial.println("  ");

    lcd.setCursor(0,0);
    lcd.print("yaw: ");
    lcd.print(yaw);
    lcd.setCursor(0,1);
    lcd.print("temp: ");
    lcd.print(temp);
    lcd.setCursor(0,2);
    lcd.print("depth: ");
    lcd.print(depth);

    lcd.setCursor(0,3);
    lcd.print("volt: ");
    lcd.print(volt);

  }  
}