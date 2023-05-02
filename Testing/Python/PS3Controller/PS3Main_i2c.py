from ps3_lib import *       # Import the custom PS3 library
from smbus import SMBus     # Import i2c library

import serial, time

# Settings i2c communication
addr = 0x8      # bus address slave (8)
bus = SMBus(1)  # indicates /dev/i2c-1

# Setting up PS3 controller
p = ps3()  # Create a PS3 object

# Functions i2c communication
def write_data(value):
    bus.write_byte(addr, value)
    return -1

def read_data():
    data = bus.read_byte(addr)
    return data

# Functions controller read
def remap(old_val, old_min, old_max, new_min, new_max):
    # new_min = 0
    # new_max = 1
    return (new_max - new_min) * (old_val - old_min) / (old_max - old_min) + new_min


AutoMode = False
FireMode = False
Track_X = 0
Track_Y = 0
Turret_X = 0
Turret_Y = 0
Fire_X = False

try:
    print("Press [ESC] or Press PS3 O button to quit")
    # Loop indefinitely
    while True:
    
except KeyboardInterrupt:
    # CTRL+C exit, disable all drives
    print("User shutdown")

finally:
    # disable all drives
    #     MotorOff()
    print("End of the line")
