from machine import Pin, PWM
from time import sleep

servoPin = PWM(Pin(18))
servoPin.freq(50)

def servo(degrees):
    if degrees > 180:
        degrees=180
    if degrees < 0:
        degrees=0
    
    maxDuty=8000
    minDuty=1725
    
    newDuty=minDuty+(maxDuty-minDuty)*(degrees/180)
    servoPin.duty_u16(int(newDuty))
    
while True:
    servo(180)
    print("increasing -- 100")
    sleep(2)
    servo(0)
    print("increasing -- 40")
    sleep(2)
    servo(90)
    print("increasing -- 40")
    sleep(2)


# протестированно 