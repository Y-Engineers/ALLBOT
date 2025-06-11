import network
import espnow

def main():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    esp = espnow.ESPNow()
    esp.active(True)

    print("Echo server ready...")

    while True:
        host, msg = esp.recv()
        if msg:
            print(f"Received from {host}: {msg.decode()}")
            esp.send(host, msg)  # Echo back
            print("Echoed message.")

if __name__ == "__main__":
    main()

