import network, ubinascii

# Initialize the network interface
wlan = network.WLAN(network.STA_IF)

# Activate the interface
wlan.active(True)

def format_mac_addr(addr):

    mac_addr = addr.upper()
    #mac_addr = mac_addr.upper()
    
    new_mac = ""
    
    for i in range(0, len(mac_addr),2):
        #print(mac_addr[i] + mac_addr[i+1])
        
        if (i == len(mac_addr) - 2):
            new_mac = new_mac + mac_addr[i] + mac_addr[i+1]
        else:
            new_mac = new_mac + mac_addr[i] + mac_addr[i+1] + ":"
    #print("My MAC Address:" + new_mac)
    return new_mac

mac_address = wlan.config('mac')
ausgabe = "MAC Address:" + str(format_mac_addr(ubinascii.hexlify(mac_address).decode()))
ausgabe = ausgabe + " - " + str(ubinascii.hexlify(mac_address).decode()) + "  " + str(mac_address)

print(ausgabe+"\n")


