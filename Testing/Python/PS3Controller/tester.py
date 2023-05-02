import msvcrt

Mode = 0
AutoMode = False
FireMode = False
Track_X = 0
Track_Y = 0
Turret_X = 0
Turret_Y = 0
Fire_X = False
ps = True

try:
    print("Press [ESC] or Press PS3 O button to quit")
    # Loop indefinitely
    while True:
        # Sleep mode [mode=0]
        # Automatic mode [mode=1]
        # Manual mode [mode=2]
        # Six-axis control
        # App control
        if Mode == 0:
            print("Mode 0: No mode selected")
        elif Mode == 1:
            print("Mode 1: Automatic mode")
        elif Mode == 2:
            print("Mode 2: Manual control mode")

        # PS3 control enabled
        if ps == True:
            print("The PS3 is connected!!")

            # changes via key press
            if msvcrt.kbhit():
                # print("Key pressed: %s" % (msvcrt.getch().decode("utf-8")))
                input = msvcrt.getch().decode("utf-8")
                if input == "s":
                    ps = False

        # PS3 controller disabled
        elif ps == False:
            print("The PS3 is disconnected!!")

        else:
            print("No controller location active")

except KeyboardInterrupt:
    # CTRL+C exit, manual interrupt
    print("User shutdown via keyboard")

finally:
    # disable program
    print("The program has been disabled")
