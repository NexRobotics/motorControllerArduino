#!/usr/bin/python

import socket
HOST = '192.168.43.6'
PORT = 50007


class _Getch:
    """Gets a single character from standard input.  Does not echo to the
screen."""
    def __init__(self):
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()


class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        return msvcrt.getch()

getch = _Getch()

CONNECTION = True

if CONNECTION:
   sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   sock.connect((HOST, PORT))

while True:
   KeyPressed = getch()
   if KeyPressed == 'w':
      print 'UP'
      if CONNECTION:
         sock.sendall('UP;1\nDOWN;0\nLEFT;0\nRIGHT;0\n')
   elif KeyPressed == 'q':
      print 'UPLEFT'
      if CONNECTION:
         sock.sendall('UP;1\nDOWN;0\nLEFT;1\nRIGHT;0\n')
   elif KeyPressed == 'e':
      print 'UPRIGHT'
      if CONNECTION:
         sock.sendall('UP;1\nDOWN;0\nLEFT;0\nRIGHT;1\n')
   elif KeyPressed == 's':
      print 'DOWN'
      if CONNECTION:
         sock.sendall('UP;0\nDOWN;1\nLEFT;0\nRIGHT;0\n')
   elif KeyPressed == 'd':
      print 'RIGHT'
      if CONNECTION:
         sock.sendall('UP;0\nDOWN;0\nLEFT;0\nRIGHT;1\n')
   elif KeyPressed == 'a':
      print 'LEFT'
      if CONNECTION:
         sock.sendall('UP;0\nDOWN;0\nLEFT;1\nRIGHT;0\n')
   elif KeyPressed == 'z':
      print 'DOWNLEFT'
      if CONNECTION:
         sock.sendall('UP;0\nDOWN;1\nLEFT;0\nRIGHT;1\n')
   elif KeyPressed == 'c':
      print 'DOWNRIGHT'
      if CONNECTION:
         sock.sendall('UP;0\nDOWN;1\nLEFT;1\nRIGHT;0\n')
   elif KeyPressed == ' ':
      print 'HALT'
      if CONNECTION:
         sock.sendall('UP;0\nDOWN;0\nLEFT;0\nRIGHT;0\n')
   elif KeyPressed == 'p':
      break













