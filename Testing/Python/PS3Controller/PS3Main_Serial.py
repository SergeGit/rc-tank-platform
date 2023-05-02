# iusr/bin/env python
########################################################################
# This example controls the RC tank using a PS3 Dualshock 3 controller
#
# History
# ------------------------------------------------
# Author        Date            Comments
# Serge B       07-Aug-2020     Initial Authoring
#
# left,right,up,down to control
# cross to stop
# left joy to turn the camera servo
# l2 to increase speed
# r2 to decrease speed
########################################################################
from ps3_lib import *  # Import the PS3 library

# from gopigo import *   #Import the Serial library
import serial, time

print("Initializing")
p = ps3()  # Create a PS3 object

# Settings serial communication
print("Setup serial link")
ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=1)
# ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
ser.flush()
print("Done")


def remap(old_val, old_min, old_max, new_min, new_max):
    # new_min = 0
    # new_max = 1
    return (new_max - new_min) * (old_val - old_min) / (old_max - old_min) + new_min


AutomaticMode = False
FireMode = False
Track_X = 0
Track_Y = 0
Turret_X = 0
Turret_Y = 0
Fire_X = 0

try:
    print("Press [ESC] or Press PS3 O button to quit")
    # Loop indefinitely
    while True:
        p.update()  # Read the ps3 values

        # Auto mode
        if p.select and p.up:  # Enable auto mode
            AutomaticMode = True
            print("Auto mode - enabled")
        elif p.select and p.down:  # Disable auto mode
            AutomaticMode = False
            print("Auto mode - disabled")

        # Extra functions on buttons
        elif p.up:  # If UP is pressed move forward
            print("Button up")
        elif p.left:  # If LEFT is pressed turn left
            print("Button left")
        elif p.right:  # If RIGHT is pressed move right
            print("Button right")
        elif p.down:  # If DOWN is pressed go back
            print("Button down")
        elif p.cross:  # If CROSS is pressed stop
            print("Button cross")

        # Fire control
        if p.l1 and p.r1:  # Fire turret
            # print("Fire mode - Fire")
            Fire_X = 1
        else:
            Fire_X = 0

        string_Fire = "<Fire," + str(Fire_X) + "," + str(0) + ">"
        # print(string_Fire)
        # Track control
        if p.l2 and p.r2:
            print("Brake")
            Track_X = 0
            Track_Y = 0
        elif p.l2:
            #             print ("Reverse: ")
            #             print (remap(p.a_joystick_L2, -1, 1, 0, 1))
            #             print ("+ LR: ")
            #             print (p.a_joystick_right_x)
            Track_X = p.a_joystick_right_x
            Track_Y = remap(p.a_joystick_L2, -1, 1, 0, 1)
        elif p.r2:
            #             print ("Forward: ")
            #             print (remap(p.a_joystick_R2, -1, 1, 0, 1))
            #             print ("+ LR: ")
            #             print (p.a_joystick_right_x)
            Track_X = p.a_joystick_right_x
            Track_Y = -remap(p.a_joystick_R2, -1, 1, 0, 1)

        string_Track = "<Track," + str(Track_X * 100) + "," + str(Track_Y * 100) + ">"

        # Turret control
        if abs(p.a_joystick_left_x) > 0.1:
            # print(p.a_joystick_left_x)
            Turret_X = -p.a_joystick_left_x
        else:  # send out 0
            Turret_X = 0
        if abs(p.a_joystick_left_y) > 0.1:
            # print(p.a_joystick_left_y)
            Turret_Y = p.a_joystick_left_y
        else:  # send out 0
            Turret_Y = 0

        string_Turret = (
            "<Turret," + str(Turret_X * 100) + "," + str(Turret_Y * 100) + ">"
        )

        # Serial print comm
        outstring = string_Turret + string_Fire + string_Track
        ser.write(outstring.encode("utf-8"))
        # ser.write(string_Fire.encode('utf-8'))
        #         print(outstring)
        line = ser.readline().decode("utf-8").rstrip()
        # print(line)
        time.sleep(0.05)
    # Disable all drives
#     MotorOff()
except KeyboardInterrupt:
    # CTRL+C exit, disable all drives
    print("User shutdown")
#     MotorOff()

finally:
    # disable all drives
    #     MotorOff()
    print("End of the line")
