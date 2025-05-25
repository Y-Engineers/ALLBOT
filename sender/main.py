import network
import espnow

wlan = network.WLAN(network.STA_IF)
wlan.active(True)

esp = espnow.ESPNow()
esp.active(True)   # < esp.init()

# receiver's MAC
peer_mac = b'\xc8\xf0\x9eH\xed\xfc'
esp.add_peer(peer_mac)

esp.send(peer_mac, b"Hello!")


