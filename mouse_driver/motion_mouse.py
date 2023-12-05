# Motion Mouse: Interaction Driver

import serial
import pyautogui
from time import sleep

port = input('Enter device name: ')

if port == '':
    port = '/dev/tty.usbmodem1302'

serial = serial.Serial(port, baudrate=38400, timeout=0.1)

pyautogui.PAUSE = 0

# Speed per 0.01 seconds
vx = 1
vy = 1

is_mouse_down = False

# if we're clicking
c = False
# 1 = scroll up, 2 = scroll down, 0 = no scroll
scroll_direction = 0

# counter to only scroll once then cool down
scroll_count = 0

def fit(x):
    if x > 50:
        return 50
    if x < -50:
        return -50
    return x

def move_mouse():
    pyautogui.move(vx, vy)

def click_mouse():
    global is_mouse_down

    if c:
        if not is_mouse_down:
            pyautogui.mouseDown()
            is_mouse_down = True
    else:
        if is_mouse_down:
            pyautogui.mouseUp()
            is_mouse_down = False

def scroll_mouse():
    global scroll_count

    # if we haven't scrolled recently, scroll
    if scroll_count == 0:
        if scroll_direction == 1:
            pyautogui.scroll(3)
        elif scroll_direction == 2:
            pyautogui.scroll(-3)
        # increment the scroll count after performing the action
        scroll_count += 1
    else:
        # increment the scroll count for subsequent data samples
        scroll_count += 1

        # Reset scroll count when it reaches the 3 times so we can scroll again after a delay
        if scroll_count >= 3:
            scroll_count = 0

while True:
    try:
        serial.flushInput()
        data = serial.readline().decode('utf-8').strip()

        velocities = data.split(',')
        vx = fit(float(velocities[0]))
        vy = fit(float(velocities[1]))
        c = velocities[2] == '1'
        scroll_direction = int(velocities[3])

        move_mouse()
        click_mouse()
        scroll_mouse()
    except KeyboardInterrupt:
        break
    except:
        pass
