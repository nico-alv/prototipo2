from evdev import InputDevice, categorize


joystick_dictionary = {
    0: "L_JOY_X",#0 left to 255 right center at 128
    1: "L_JOY_Y",#0 left to 255 right center at 128
    2: "R_JOY_X",#0 left to 255 right center at 128
    5: "R_JOY_Y",#0 left to 255 right center at 128
    16: "GAMEPAD_X",# -1 left to 1 right
    17: "GAMEPAD_Y",# -1 up to 1 down
    307: "BTN_X",
    308: "BTN_Y",
    305: "BTN_B",
    304: "BTN_A",
    10: "L_TRIGGER",#(312 BOOLEAN)
    312: "L_TRIGGER_START",#(312 BOOLEAN)
    310: "L_BTN",
    9: "R_TRIGGER",#(313 BOOLEAN)
    313: "R_TRIGGER_START",#(313 BOOLEAN)
    311: "R_BTN",
    314: "SELECT_BTN",
    315: "START_BTN",
    318: "R3",
    317: "L3",
}

gamepad = InputDevice("/dev/input/event8")

print(gamepad)

for event in gamepad.read_loop():
    if(event.type == 0 or event.type == 4): continue
    print(f"{joystick_dictionary[event.code]}")
    #print(f"{event.code}: {event.type}")

