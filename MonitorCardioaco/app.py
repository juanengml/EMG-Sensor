import serial
import time

def sensor():
    with serial.Serial('/dev/ttyACM0', 115200) as ser:
     #x = ser.read()          # read one byte
     s = ser.read(3)        # read up to ten bytes (timeout)
     #line = ser.readline()   # read a '\n' terminated line
     return int(s)

while True:
  print sensor()    
  time.sleep(0.01)   