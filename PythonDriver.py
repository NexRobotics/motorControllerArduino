#!/usr/bin/python

import serial
from select import select
import socket
import listPorts
from time import sleep
from time import time
import thread

ENABLE_MOTORS = True

HOST = ''
PORT = 50006

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
print portnames
openedPort = ['']*len(portnames)
for i in range(0, len(portnames)):
   openedPort[i] = serial.Serial(portnames[i], 115200, timeout=1)

if(ENABLE_MOTORS):
   MotorDriverPort = openedPort[0]

print 'Waiting for client...'
connection, addr = communicationSocket.accept()

print 'Client connected...'

connection.settimeout(0.05)

motorPower = 60
RPower = 0
LPower = 0
receivedData = ''
lastDataReceivedTime = time()

Commands = ['UP;1', 'DOWN;1', 'LEFT;1', 'RIGHT;1']

sleep(2)

lastDataReceivedTime = time()

if MotorDriverPort.isOpen() or not ENABLE_MOTORS:
   try:
      while 1:
         readable, writable, exceptional = select([connection], [], [], 0)
         if readable:
            print readable
            receivedData = connection.recv(1024)

            if receivedData:
               lastDataReceivedTime = time()
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

               if(ENABLE_MOTORS):
                  MotorDriverPort.write('Set '+ str(LPower) +' '+ str(RPower) +' '+ str(LPower) +' '+ str(RPower) +' '+ str(LPower) +' '+ str(RPower) +'\n')
               print 'Direct: ' + str(RPower) + ' ' + str(LPower)
               sleep(0.1)
         elif (lastDataReceivedTime + 0.5) > time():
            if(ENABLE_MOTORS):
               MotorDriverPort.write('Set '+ str(LPower) +' '+ str(RPower) +' '+ str(LPower) +' '+ str(RPower) +' '+ str(LPower) +' '+ str(RPower) +'\n')
            print 'Holding: ' + str(RPower) + ' ' + str(LPower)
            sleep(0.1)
         else:
            RPower = 0
            LPower = 0
            sleep(0.1)
            if (ENABLE_MOTORS):
               MotorDriverPort.write('Set '+ str(LPower) +' '+ str(RPower) +' '+ str(LPower) +' '+ str(RPower) +' '+ str(LPower) +' '+ str(RPower) +'\n')
            print 'Zeroing: ' + str(RPower) + ' ' + str(LPower)


             

   finally:
      #print '0 0'
      connection.close()
      #MotorDriverPort.write('Set 0 0 0 0 0 0\n')
      #MotorDriverPort.write.close()
