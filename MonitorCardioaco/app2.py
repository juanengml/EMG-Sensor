import os
import time
import sys
#import Adafruit_DHT as dht
import paho.mqtt.client as mqtt
import json
import serial
import random
import time

THINGSBOARD_HOST = 'delrey.td.utfpr.edu.br'
ACCESS_TOKEN = 'JWISxh43nyEP0P5aBdoB'

# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=2

sensor_data = {'sensor_ecg': 0}

client = mqtt.Client()

# Set access token
client.username_pw_set(ACCESS_TOKEN)

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect(THINGSBOARD_HOST, 1883, 60)

client.loop_start()

def sensor():
     s = random.choice(range(400,1000))
#    with serial.Serial('/dev/ttyACM0', 115200) as ser:
     #x = ser.read()          # read one byte
#     s = ser.read(3)        # read up to ten bytes (timeout)
     #line = ser.readline()   # read a '\n' terminated line
     return int(s)


try:
    while True:
        ECG_data = sensor()
        #ECG_data = round(ECG_data, 2)
        #temperature = round(temperature, 2)
        print "ECG: ",(ECG_data)
        #sensor_data['temperature'] = temperature
        sensor_data['sensor_ecg'] = ECG_data

        # Sending humidity and temperature data to ThingsBoard
        client.publish('v1/devices/me/telemetry', json.dumps(sensor_data), 1)

        #next_reading += INTERVAL
        #sleep_time = next_reading-time.time()
        #if sleep_time > 0:
        #    time.sleep(sleep_time/2)
        time.sleep(1)
except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()
