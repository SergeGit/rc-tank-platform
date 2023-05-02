# iusr/bin/env python
########################################################################
# This example controls the RC tank using a PS3 Dualshock 3 controller
#
# History
# ------------------------------------------------
# Author        Date            Comments
# Serge B       10-Apr-2023     Initial Authoring
#
# left,right,up,down to control
# cross to stop
# left joy to turn the camera servo
# l2 to increase speed
# r2 to decrease speed
########################################################################
from ps3_lib import *  # Import the PS3 library
from time import sleep
from smbus import SMBus
import struct
import os

# import bluetooth

print("Initializing")
p = ps3()  # Create a PS3 object
bus = SMBus(1)  # 1 indicates /dev/i2c-1
address = 0x08  # Arduino Nano I2C address


def remap(old_val, old_min, old_max, new_min, new_max):
    # new_min = 0
    # new_max = 1
    return (new_max - new_min) * (old_val - old_min) / (old_max - old_min) + new_min


# function to send data via i2c bus
def send_data(data):
    for d in data:
        bus.write_byte(address, d)
        return -1


while True:
    # handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

        # get joystick and button states
        joystick = pygame.joystick.Joystick(0)
        joystick.init()
        axis_data = []
        button_data = []

        for i in range(joystick.get_numaxes()):
            axis_data.append(struct.pack("f", joystick.get_axis(i)))
        for i in range(joystick.get_numbuttons()):
            button_data.append(joystick.get_button(i))

        # combine data into one list
        data = axis_data + button_data
        print("Axis data")
        print(axis_data)

        print("button_data")
        print(button_data)

        # send data via I2C
        # send_data(data)

        # limit loop speed
        # time.sleep(3)
