import board
import busio, displayio
import adafruit_max31855
import time
import digitalio
import pwmio
import myRGBLed as rgb
# for 7 segment display
#from adafruit_ht16k33 import segments
import adafruit_displayio_ssd1306
from adafruit_display_text import label
import adafruit_bitmap_font
from adafruit_bitmap_font import bitmap_font
# from Jonh Gallaugher youtube Control a Pico W from Web Dashboard
import os,ssl,socketpool,wifi
import adafruit_minimqtt.adafruit_minimqtt as MQTT

print(f"Connecting to WIFI")
wifi.radio.connect(os.getenv("WIFI_SSID"), os.getenv("WIFI_PASSWORD"))
print("Connected!!!!")

pool=socketpool.SocketPool(wifi.radio)
# high temp threshhold
treshold=77
tempF=0.0

#Adafruit IO connection
aio_username=os.getenv('AIO_USERNAME')
aio_key=os.getenv('AIO_KEY')
mqtt_broker = "io.adafruit.com"
port=os.getenv('PORT')

on=False
off=True
red=off
blue=off
green=on

font_file = "fonts/terminal.bdf"
font = bitmap_font.load_font(font_file)
temperature_string = f"{tempF}°F"
    # Draw the temperature value label
temperature_value_label = label.Label(font, text = temperature_string, color = 0xFFFFFF)

# setup ssd1306 128 X 32 Display
def setupLCD():
    displayio.release_displays()
    sda, scl = board.GP6, board.GP7 
    i2c = busio.I2C(scl, sda)
    display_bus = displayio.I2CDisplay(i2c, device_address=0x3C)
    display = adafruit_displayio_ssd1306.SSD1306(display_bus, width=128, height=32)
        # Load a font, we need it for the degrees symbol
    # Make the display context
    splash = displayio.Group()
    display.show(splash)

    color_bitmap = displayio.Bitmap(128, 32, 1)
    color_palette = displayio.Palette(1)
    color_palette[0] = 0xFFFFFF  # White

    bg_sprite = displayio.TileGrid(color_bitmap, pixel_shader=color_palette, x=0, y=0)
    splash.append(bg_sprite)
    board_type=os.uname().machine
    # Draw a smaller inner rectangle
    inner_bitmap = displayio.Bitmap(126, 30, 1)
    inner_palette = displayio.Palette(1)
    inner_palette[0] = 0x000000  # Black
    inner_sprite = displayio.TileGrid(inner_bitmap, pixel_shader=inner_palette, x=1, y=1)
    splash.append(inner_sprite)

    
    temperature_value_label.anchor_point = (0.5, 0.5) # Change anchor point to center
    temperature_value_label.anchored_position = (64, 15)
    temperature_value_label.scale = 2
    splash.append(temperature_value_label)
def setColor(red,green,blue): # odd hook for rgb, but pins mixed on board
    rgb.set_color(red,green,blue)
freezer_temp_feed=aio_username+"/feeds/FreezerTemp"
def activateBuzzer():
    buzzer.duty_cycle = 2 ** 15
    time.sleep(0.5)        
    buzzer.duty_cycle = 0
    time.sleep(0.5)
def connected(client,userdata,flags,rc):
    print("Connected to Adafruit and listening for changes in temp")
    client.subscribe(freezer_temp_feed)
    
def disconnected(client, userdata,rc):
    print("Disconnected from Adafruit MQTT server")
    
def publish_temp(client,temp):
    client.publish(freezer_temp_feed,temp)
    
mqtt=MQTT.MQTT(
    broker=mqtt_broker,
    port=port,
    username=aio_username,
    password=aio_key,
    socket_pool=pool,
    ssl_context=ssl.create_default_context(),
          
          )
mqtt.connect()

setupLCD()
# create an SPI connection for Themocouple
spi = busio.SPI(board.GP10,None, board.GP12,)
 
cs = digitalio.DigitalInOut(board.GP13)

# create a MAX31855 object
max31855 = adafruit_max31855.MAX31855(spi, cs)

# setup buzzer
buzzer = pwmio.PWMOut(board.GP14, frequency=660, duty_cycle=0, variable_frequency=True)

count=0 # used to throttle msg to adafruit
while True:
    count=count+1
    if count>5000: #keeps the count for blowing the limit on var size??
        count=0
    mqtt.loop()
    setColor(red,green,blue)
    # read temperature data from the MAX31855 sensor
    #internal_temp = max31855.temperature
    thermocouple_temp = max31855.temperature
    tempF=thermocouple_temp*(9/5)+32.0
    publish_temp(mqtt,tempF)
    if tempF>treshold:
        red=on
        green=off
        activateBuzzer()
        time.sleep(.5)
    else:
        red=off
        green=on
        
    # print the temperature readings
    #print('Internal Temperature: {0:.2f}°C'.format(internal_temp))
    #print('Thermocouple Temperature: {0:.2f}°C'.format(thermocouple_temp))
    print('Temp: {0:.2f}°F'.format(tempF))
    temperature =  round(tempF,1)
    temperature_string = f"{temperature}°F"
    temperature_value_label.text = temperature_string
    # delay for 3 sec before reading again
    time.sleep(3)