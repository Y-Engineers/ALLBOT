# receiver

from machine import Pin, PWM

# communication Pins
Dtoggle = Pin(14, Pin.OUT) # off->no user control
DC1 = Pin(15, Pin.OUT) # 1
DC2 = Pin(16, Pin.OUT) # 2
DC3 = Pin(17, Pin.OUT) # 4
DC4 = Pin(18, Pin.OUT) # 8
Aspeed = PWM(Pin(12), freq=1000)
Atimes = PWM(Pin(13), freq=1000)


import network
import espnow

# Set up WLAN in station mode
wlan = network.WLAN(network.STA_IF)
wlan.active(True)

# init ESP-NOW
esp = espnow.ESPNow()
esp.active(True)   # esp.init()

print("Waiting for messages...")

while True:
    host, msg = esp.recv()
    if msg:
        print("Received from", host, ":", msg)

