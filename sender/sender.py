import network
import espnow

# Mapping from command string to code (0–15)
command_map = {
    "WF": 0, "WB": 1, "WL": 2, "WR": 3,
    "TL": 4, "TR": 5, "LF": 6, "LB": 7,
    "LL": 8, "LR": 9, "FL": 10, "FR": 11,
    "RL": 12, "RR": 13, "SC": 14, "CH": 15
}

def handle_user_input(esp, peer_mac):
    while True:
        raw = input("Enter command (e.g. wf or just 0/1): ").strip()

        if raw.lower() == "exit":
            return

        if raw in ("on", "off"):
            esp.send(peer_mac, raw.encode())
            print(f"Sent single bit: {raw}")
            continue

        command_str = raw.upper()

        if command_str not in command_map:
            print(f"Unknown command: {command_str}")
            continue

        command_code = command_map[command_str]
        message = str(command_code)
        esp.send(peer_mac, message.encode())
        print(f"Sent: {message}")

def main():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    esp = espnow.ESPNow()
    esp.active(True)

    # Receiver's MAC address
    peer_mac = b'@\x91Q\xab\x8b(' # Replace with actual MAC address
    esp.add_peer(peer_mac)

    handle_user_input(esp, peer_mac)

if __name__ == "__main__":
    main()

