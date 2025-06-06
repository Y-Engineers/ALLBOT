from machine import Pin, DAC
import network
import espnow

# Pin definitions
Dtoggle = Pin(19, Pin.OUT)

# 4-bit command output pins
DC1 = Pin(12, Pin.OUT)  # Bit 0 (LSB)
DC2 = Pin(14, Pin.OUT)  # Bit 1
DC3 = Pin(27, Pin.OUT)  # Bit 2
DC4 = Pin(16, Pin.OUT)  # Bit 3 (MSB)

# DAC output pins
# Aspeed = DAC(Pin(36))   # GPIO25 = DAC1
# Atimes = DAC(Pin(34))   # GPIO26 = DAC2

def setup_wifi_and_espnow():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    esp = espnow.ESPNow()
    esp.active(True)
    return esp

def handle_message(msg: bytes):
    text = msg.decode().strip()

    # Handle simple toggle commands
    if text == "0":
        Dtoggle.off()
        print("Toggled OFF")
        return
    elif text == "1":
        Dtoggle.on()
        print("Toggled ON")
        return

    # Handle formatted command: code,times,speed
    parts = text.split(",")
    if len(parts) != 3:
        print("Invalid command format.")
        return

    try:
        command_code = int(parts[0])
        times = int(parts[1])
        speed = int(parts[2])
    except ValueError:
        print("Non-integer command arguments.")
        return

    # Clamp DAC values to 0–255
    # times = max(0, min(255, times))
    # speed = max(0, min(255, speed))

    # Set DAC values
    # Aspeed.write(speed)
    # Atimes.write(times)

    # Set 4-bit command on digital pins
    for i, pin in enumerate((DC1, DC2, DC3, DC4)):
        pin.value((command_code >> i) & 1)

    print(f"Command: {command_code:04b}, Times: {times}, Speed: {speed}")

def main():
    esp = setup_wifi_and_espnow()
    print("Waiting for messages...")

    while True:
        host, msg = esp.recv()
        if msg:
            print("Received from", host, ":", msg)
            try:
                handle_message(msg)
            except Exception as e:
                print("Error handling message:", e)

# Run main
if __name__ == "__main__":
    main()

