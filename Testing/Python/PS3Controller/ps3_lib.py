#!/usr/bin/env python
########################################################################
# This is the library to read values from PS3 Dualshock 3 controller
#
# History
# ------------------------------------------------
# Author        Date            Comments
# Serge B       07 Aug 20       Initial Authoring
#
# Dependencies- pygame
# Pairing the controller using bluetooth
# http://booting-rpi.blogspot.ro/2012/08/dualshock-3-and-raspberry-pi.html
# PS3 Key configuration http://wiki.ros.org/ps3joy
#
# Key values can be obtained by creating a ps3 object and calling update() regularly
########################################################################
import pygame, time, os, sys

sys.path.insert(0, os.path.dirname(__file__))
from pygame.locals import *


# PS3 functions and variables
class ps3:
    joystick = 0
    joystick_count = 0
    numaxes = 0
    numbuttons = 0
    # left=right=up=down=l1=l2=r1=r2=triangle=circle=square=cross=select=start=ps=joystick_left=joystick_right=0
    # a_left=a_right=a_up=a_down=a_l1=a_l2=a_r1=a_r2=a_triangle=a_circle=a_square=a_cross=a_select=a_start=a_ps=a_joystick_left_x=a_joystick_left_y=a_joystick_right_x=a_joystick_right_y=acc_x=acc_y=acc_z=gyro_yaw=0

    # Initialize the controller when the object is created
    def __init__(self):
        # Make the stdout buffer as 0,because of bug in Pygame which keeps on printing debug statements
        # http://stackoverflow.com/questions/107705/python-output-buffering
        # sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)

        os.environ["DISPLAY"] = ":0"
        pygame.init()  # Initialize pygame
        # Wait for a joystick
        while pygame.joystick.get_count() == 0:
            print("waiting for joystick count = %i" % pygame.joystick.get_count())
            time.sleep(10)
            pygame.joystick.quit()
            pygame.joystick.init()

        ps3.joystick = pygame.joystick.Joystick(0)
        ps3.joystick.init()  # Initialize Joystick
        ps3.numaxes = ps3.joystick.get_numaxes()
        ps3.numbuttons = ps3.joystick.get_numbuttons()
        print(
            "Initialized Joystick : %s" % ps3.joystick.get_name()
        )  # Print joystick if present

        print("joystick count: " + str(ps3.joystick_count))
        print("number axes: " + str(ps3.numaxes))
        print("number buttons: " + str(ps3.numbuttons))
        # get count of joysticks=1, axes=27, buttons=19 for DualShock 3

    # Update the button values
    def update(self):
        loopQuit = False
        button_state = [0] * self.numbuttons
        button_analog = [0] * self.numaxes
        # while loopQuit == False:
        outstr = ""

        # Read analog values
        for i in range(0, self.numaxes):
            button_analog[i] = self.joystick.get_axis(i)

        # Read digital values
        for i in range(0, self.numbuttons):
            button_state[i] = self.joystick.get_button(i)

        self.a_joystick_left_x = button_analog[0]
        self.a_joystick_left_y = button_analog[1]
        self.a_joystick_right_x = button_analog[3]
        self.a_joystick_right_y = button_analog[4]
        self.a_joystick_L2 = button_analog[2]
        self.a_joystick_R2 = button_analog[5]

        self.cross = button_state[0]
        self.circle = button_state[1]
        self.triangle = button_state[2]
        self.square = button_state[3]
        self.l1 = button_state[4]
        self.r1 = button_state[5]
        self.l2 = button_state[6]
        self.r2 = button_state[7]
        self.select = button_state[8]
        self.start = button_state[9]
        self.ps = button_state[10]
        self.l3 = button_state[11]
        self.r3 = button_state[12]
        self.up = button_state[13]
        self.down = button_state[14]
        self.left = button_state[15]
        self.right = button_state[16]

        # refresh
        pygame.event.get()
        # return button_analog, button_state
