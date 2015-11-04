#!/usr/bin/python

import serial
import socket
import listPorts

from time import sleep

HOST = ''
PORT = 50007

def anyEqual(list, value):
    for i in range(0, len(list)):
        if list[i] == value:
            return 1
    return 0

communicationSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
communicationSocket.bind((HOST, PORT))
communicationSocket.listen(1)

print 'Connecting device...'
portnames = listPorts.serial_ports()
openedPort = []*size(portnames)
for i in range(0, size(portnames)):
   openedPort[i] = serial.Serial(portnames[i], 115200, timeout=1)

MotorDriverPort = openedPort[0]
print 'Waiting for client...'
connection, addr = communicationSocket.accept()

connection.settimeout(0.5)

motorPower = 60
RPower = 0
LPower = 0

Commands = ['UP;1', 'DOWN;1', 'LEFT;1', 'RIGHT;1']

sleep(2)

if MotorDriverPort.isOpen():
   try:
      while 1:
         receivedData = connection.recv(1024)
         if receivedData:
            RPower = 0
            LPower = 0
            receivedData = receivedData.split()
            if anyEqual(receivedData, Commands[0]):
               RPower = RPower + 70
               LPower = LPower + 70
            if anyEqual(receivedData, Commands[1]):
               RPower = RPower - 70
               LPower = LPower - 70
            if anyEqual(receivedData, Commands[2]):
               RPower = RPower + 70
               LPower = LPower - 70
            if anyEqual(receivedData, Commands[3]):
               RPower = RPower - 70
               LPower = LPower + 70

            MotorDriverPort.write('Set '+ LPower +' '+ RPower +' '+ LPower +' '+ RPower +' '+ LPower +' '+ RPower +'\n')
         else
            MotorDriverPort.write('Set 0 0 0 0 0 0\n')
            RPower = 0
            LPower = 0


             

   finally:
      MotorDriverPort.write('Set 0 0 0 0 0 0\n')
      MotorDriverPort.write.close()
