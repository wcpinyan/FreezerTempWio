import time
import board
import digitalio

# Configure the RGB LED
red_pin = board.GP2
green_pin = board.GP4
blue_pin = board.GP3
print("hello")
red_led = digitalio.DigitalInOut(red_pin)
red_led.direction = digitalio.Direction.OUTPUT

green_led = digitalio.DigitalInOut(green_pin)
green_led.direction = digitalio.Direction.OUTPUT

blue_led = digitalio.DigitalInOut(blue_pin)
blue_led.direction = digitalio.Direction.OUTPUT

def set_color(red, green, blue):
    red_led.value = red
    green_led.value =False
    blue_led.value = blue

# Main loop
# while True:
#     # Set RGB colors
#     set_color(False, True, True)  # Red
#     time.sleep(1)
#     
#     set_color(True, False, True)  # Green
#     time.sleep(1)
#     
#     set_color(True, True, False)  # Blue
#     time.sleep(1)
    
    