# Motion Mouse

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
c = False # if we're clicking
scroll_direction = 0 # 1 = scroll up, 2 = scroll down, 0 = no scroll
scroll_count = 0 # counter to only scroll once then cool down

def fit(x):
    if x > 50:
        return 50
    if x < -50:
        return -50
    return x
    

def update_mouse():
    pyautogui.move(vx, vy)
    #print(vx, vy, c)
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
    if scroll_count == 0: # if we haven't scrolled recently, scroll
        if scroll_direction == 1:
            pyautogui.scroll(3)
        elif scroll_direction == 2:
            pyautogui.scroll(-3)
        scroll_count += 1  # increment the scroll count after performing the action
    else:
        scroll_count += 1 # increment the scroll count for subsequent data samples
        if scroll_count >= 3:  # Reset scroll count when it reaches the 3 times so we can scroll again after a delay
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
        if scroll_direction > 0:
            print(scroll_direction)
        update_mouse()
        scroll_mouse()
    except KeyboardInterrupt:
        break
    except:
        pass
