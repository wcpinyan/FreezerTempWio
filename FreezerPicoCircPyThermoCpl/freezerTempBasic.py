"""
Taken loosely from https://www.youtube.com/watch?v=07vG-_CcDG0&t=1809s
from DroneBot and AdaFruit max31855 learn page.

"""
import board
import busio

import digitalio
import adafruit_max31855
from time import sleep
import adafruit_dht

MISO=board.GP12 #phys pin 16
clk=board.GP14 #phys pin 19
#cs=board.GP13  #phys pin 17 

spi = busio.SPI(clk,MISO=MISO)
cs = digitalio.DigitalInOut(board.GP13)

max31855 = adafruit_max31855.MAX31855(spi, cs)
dht = adafruit_dht.DHT22(board.GP15)

class Temp:
    def __init__(self):
        print("Initiating Temp Class")
        self.tempC = max31855.temperature
        self.tempF = self.tempC * 9 / 5 + 32
    

    # count=0
    # led = digitalio.DigitalInOut(board.LED)
    # led.direction=digitalio.Direction.OUTPUT
    # 
    # while True:
    #     led.value=True
    #     
    #     print(count)
    #     count=count+1
    #     sleep(0.5)
    #     led.value=False
    #     sleep(0.5)
   
    
def getTemp():
    return Temp()
temp=getTemp()   
while True:
    tempC=dht.temperature
#     print(temp.tempF)
#     tempC = max31855.temperature
    tempF = tempC * 9 / 5 + 32
    print("Temperature: {} C {} F ".format(tempC, tempF))
    sleep(1)
    if tempF>80:
        print("Ouch")