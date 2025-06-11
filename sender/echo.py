import network
import espnow

def main():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    esp = espnow.ESPNow()
    esp.active(True)

    peer_mac = b'@\x91Q\xab\x8b('  # Replace with your receiver's MAC
    esp.add_peer(peer_mac)

    while True:
        raw = input("Enter message to send (or 'exit'): ").strip()
        if raw.lower() == "exit":
            break

        esp.send(peer_mac, raw.encode())
        print(f"Sent: {raw}")

        host, msg = esp.recv(timeout=3000)  # Wait max 3 seconds for reply
        if msg:
            print(f"Echoed back from {host}: {msg.decode()}")
        else:
            print("No response (timeout)")

if __name__ == "__main__":
    main()

