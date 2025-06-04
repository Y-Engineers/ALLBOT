# sender

import network
import espnow

# Mapping from command string to code (0–15)
command_map = {
    "WF": 0, "WB": 1, "WL": 2, "WR": 3,
    "TL": 4, "TR": 5, "LF": 6, "LB": 7,
    "LL": 8, "LR": 9, "FL": 10, "FR": 11,
    "RL": 12, "RR": 13, "SC": 14, "CH": 15
}

def handle_user_input(esp, peer_mac) -> None:
    while True:
        raw = input("Enter command (e.g. WF 5 120 or just 0/1): ").strip()

        if raw == "exit":
            return

        if raw in ("0", "1"):
            esp.send(peer_mac, raw.encode())
            print(f"Sent single bit: {raw}")
            continue

        parts = raw.split()
    
        if len(parts) != 3:
            print("Invalid input format. Use: $command $times $speed or just 0/1")
            continue
    
        command_str, times_str, speed_str = parts
        command_str = command_str.upper()
             
        if command_str not in command_map:
            print(f"Unknown command: {command_str}")
            continue
    
        try:
            command_code = command_map[command_str]
            times = int(times_str)
            speed = int(speed_str)
        except ValueError:
            print("Invalid number for times or speed.")
            continue
    
        message = f"{command_code},{times},{speed}"
        esp.send(peer_mac, message.encode())
        print(f"Sent: {message}")

def main() -> None:
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    esp = espnow.ESPNow()
    esp.active(True)

    # receiver's MAC (get_MAC.py on receiver)
    peer_mac = b'@\x91Q\xab\x8b(' # Replace with receiver MAC
    esp.add_peer(peer_mac)

    # loop for user input and sending data
    handle_user_input(esp, peer_mac)

if __name__ == "__main__":
    main()


