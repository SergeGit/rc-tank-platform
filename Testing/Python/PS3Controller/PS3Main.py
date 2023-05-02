#!/usr/bin/env python
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

print("Initializing")
p = ps3()  # Create a PS3 object
print("Done")


def remap(old_val, old_min, old_max, new_min, new_max):
    # new_min = 0
    # new_max = 1
    return (new_max - new_min) * (old_val - old_min) / (old_max - old_min) + new_min


AutomaticMode = False
FireMode = False
while True:
    p.update()  # Read the ps3 values

    # Auto mode
    if p.select and p.up:  # Enable auto mode
        AutomaticMode = True
        print("Auto mode - enabled")
    elif p.select and p.down:  # Disable auto mode
        AutomaticMode = False
        print("Auto mode - disabled")

    # Fire control
    elif p.l1 and p.r1:  # Fire turret
        print("Fire mode - Fire")

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

    # Track control
    if p.l2 and p.r2:
        print("Brake")
    elif p.l2:
        print("Reverse: ")
        print(remap(p.a_joystick_L2, -1, 1, 0, 1))
        print("+ LR: ")
        print(p.a_joystick_right_x)
    elif p.r2:
        print("Forward: ")
        print(remap(p.a_joystick_R2, -1, 1, 0, 1))
        print("+ LR: ")
        print(p.a_joystick_right_x)

    # Turret control
    if abs(p.a_joystick_left_x) > 0.1:
        print(p.a_joystick_left_x)
    # else: #send out 0

    if abs(p.a_joystick_left_y) > 0.1:
        print(p.a_joystick_left_y)
    # else: #send out 0

    time.sleep(0.05)
    # else:
    #   if flag:        #If LEFT or RIGHT key was last pressed start moving forward again
    #       tester=0#fwd()
    #       flag=0
    # if p.l2:          #Increase the speed if L2 is pressed
    #   print (s)
    #   s+=2
    #   if s>255:
    #       s=255
    # if p.r2:          #Decrease the speed if R2 is pressed
    #   print (s)
    #   s-=2
    #   if s<0:
    #       s=0
    # x=(p.a_joystick_left_x+1)*90
    # print (int(x))
