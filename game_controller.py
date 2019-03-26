import serial
from pynput.keyboard import Key, Controller
import time
import os
from pygame import mixer

def game_name(count):
    if count % 3 == 0:
        return 'tetris'
    elif count % 3 == 1:
        return 'snake'
    else:
        return 'pong'

if __name__ == '__main__':
    uart = serial.Serial('/dev/cu.usbserial-AI065VNI', 115200)
    uart.reset_input_buffer()

    keyboard = Controller()

    count = 0
    while True:
        if uart.in_waiting > 0:
            command = uart.read()
            if command == b'b':
                with keyboard.pressed(Key.cmd):
                    keyboard.press('t')
                    time.sleep(0.1)
                    keyboard.release('t')
                keyboard.type('emacs')
                keyboard.press(Key.enter)
                keyboard.release(Key.enter)
                keyboard.press(Key.esc)
                keyboard.release(Key.esc)
                keyboard.press('x')
                keyboard.release('x')
                keyboard.type(game_name(count))
                keyboard.press(Key.enter)
                keyboard.release(Key.enter)
                count += 1
                mixer.init()
                mixer.music.load('Tetris.mp3')
                mixer.music.play()
            if command == b'u':
                keyboard.press(Key.up)
                time.sleep(0.1)
                keyboard.release(Key.up)
            elif command == b'd':
                keyboard.press(Key.down)
                time.sleep(0.1)
                keyboard.release(Key.down)
            elif command == b'l':
                keyboard.press(Key.left)
                time.sleep(0.1)
                keyboard.release(Key.left)
            elif command == b'r':
                keyboard.press(Key.right)
                time.sleep(0.1)
                keyboard.release(Key.right)
    uart.close()
