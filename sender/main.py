# sender

import network
import espnow

# mapping from command string to code (0–15)
command_map = {
    "WF": 0, "WB": 1, "WL": 2, "WR": 3,
    "TL": 4, "TR": 5, "LF": 6, "LB": 7,
    "LL": 8, "LR": 9, "FL": 10, "FR": 11,
    "RL": 12, "RR": 13, "SC": 14, "CH": 15
}

def main():
    # start network
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    # ESP-NOW
    esp = espnow.ESPNow()
    esp.active(True)

    # receiver's MAC (get with get_MAC.py)
    peer_mac = b'\xc8\xf0\x9eH\xed\xfc'  # replace with your receiver MAC
    esp.add_peer(peer_mac)

    # user input
    try:
        raw = input("Enter command (e.g. WF 5 120): ").strip()
        parts = raw.split()

        if len(parts) != 3:
            print("Invalid input format. Use: $command $times $speed")
            return

        command_str, times, speed = parts[0].upper(), int(parts[1]), int(parts[2])

        if command_str not in command_map:
            print("Unknown command:", command_str)
            return

        command_code = command_map[command_str]

        # send message in format: "code,times,speed" (e.g., "0,5,120")
        message = f"{command_code},{times},{speed}"
        esp.send(peer_mac, message.encode())
        print(f"Sent: {message}")

    except Exception as e:
        print("Error:", e)

if __name__ == "__main__":
    main()

