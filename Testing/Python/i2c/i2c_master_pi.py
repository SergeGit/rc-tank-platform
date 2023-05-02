#   RaspBerry Pi I2c Master for Arduino Slave
#   i2c_master_pi.py
#   Connects to Arduino via I2C

from smbus import SMBus

addr = 0x8      # bus address slave
bus = SMBus(1)  # indicates /dev/i2c-1

#   Functions
def write_data(value):
    bus.write_byte(addr, value)
    return -1

def read_data():
    data = bus.read_byte(addr)
    return data

#   Loop
numb = 1

print ("Enter 1 for ON or 0 for OFF")
while numb == 1:
    ledstate = input(">>>>   ")

    if ledstate == "1":
        write_data(1)
        # bus.write_byte(addr, 0x1) # switch light ON

    elif ledstate == "0":
        write_data(0)
        # bus.write_byte(addr, 0x0) # switch light OFF

    elif ledstate =="3":
        data = read_data()
        print(data)

    else:
        numb = 0