# receiver

from machine import Pin, PWM

# communication Pins
Ptoggle = Pin(1, Pin.OUT) # off->no user control
PC1 = Pin(2, Pin.OUT) # 1
PC2 = Pin(3, Pin.OUT) # 2
PC3 = Pin(4, Pin.OUT) # 4
PC4 = Pin(5, Pin.OUT) # 8
Pspeed = PWM(Pin(6), freq=1000)
Ptimes = PWM(Pin(7), freq=1000)


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
    host, msg = e.recv()
    if msg:
        print("Received from", host, ":", msg)

