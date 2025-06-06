from machine import Pin
import network
import espnow

# Pin definitions
Dtoggle = Pin(19, Pin.OUT)

# 4-bit command output pins
DC1 = Pin(12, Pin.OUT)  # Bit 0 (LSB)
DC2 = Pin(14, Pin.OUT)  # Bit 1
DC3 = Pin(27, Pin.OUT)  # Bit 2
DC4 = Pin(16, Pin.OUT)  # Bit 3 (MSB)

def setup_wifi_and_espnow():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    esp = espnow.ESPNow()
    esp.active(True)
    return esp

def handle_message(msg: bytes):
    text = msg.decode().strip()

    # Handle simple toggle commands
    if text == "on":
        Dtoggle.off()
        print("Toggled ON")
        return
    elif text == "off":
        Dtoggle.on()
        print("Toggled OFF")
        return

    # Handle formatted command: code only
    try:
        command_code = int(text)
    except ValueError:
        print("Invalid command. Must be an integer or '0'/'1'.")
        return

    # Set 4-bit command on digital pins
    for i, pin in enumerate((DC1, DC2, DC3, DC4)):
        pin.value((command_code >> i) & 1)

    print(f"Command: {command_code:04b}")

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

