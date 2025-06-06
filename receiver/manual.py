from machine import Pin, DAC

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

def main():
    while True:
        Dtoggle.on()
        DC1.value(1)
        DC2.value(1)
        DC3.value(1)
        DC4.value(1)

# Run main
if __name__ == "__main__":
    main()

