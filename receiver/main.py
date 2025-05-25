# receiver

import network
import espnow

# Set up WLAN in station mode
w0 = network.WLAN(network.STA_IF)
w0.active(True)

# Initialize ESP-NOW
e = espnow.ESPNow()
e.init()

print("Waiting for messages...")

while True:
    host, msg = e.recv()
    if msg:
        print("Received from", host, ":", msg)

