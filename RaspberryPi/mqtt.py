#!/usr/bin/env python
import paho.mqtt.client as mqtt
import time
import sys
import subprocess, os
import signal
import json
from neopixel import *

#-------------------- Inital Setup: --------------------
LED_COUNT      = 110     # Number of LED pixels.
LED_PIN        = 12      # GPIO pin connected to the pixels (must support PWM!).
LED_FREQ_HZ    = 800000  # LED signal frequency in hertz (usually 800khz)
LED_DMA        = 10      # DMA channel to use for generating signal (try 10)
LED_BRIGHTNESS = 255     # Set to 0 for darkest and 255 for brightest
LED_INVERT     = False   # True to invert the signal (when using NPN transistor level shift)
LED_CHANNEL    = 0
LED_STRIP      = ws.WS2811_STRIP_RGB
run = 0
pid = 0

strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, LED_FREQ_HZ, LED_DMA, LED_INVERT, LED_BRIGHTNESS, LED_CHANNEL, LED_STRIP)
# Intialize the library (must be called once before other functions).
strip.begin()

#-------------------- Functions ------------------------
def on_connect(client, userdata, flags, rc):
	if rc==0:
		client.connected_flag=True
    		print("Connected with result code " + str(rc))
    		client.subscribe("pi_keller/funktionen/#")
	else:
		print("Bad connection Returned code=",rc)

def on_message(client, userdata, msg):
	try:
		stop()
		if msg.topic == "pi_keller/funktionen/color":
			color(msg.payload)
		elif msg.topic == "pi_keller/funktionen/react":
			react(msg.payload)
		elif msg.topic == "pi_keller/funktionen/rgb":
			rgb(msg.payload)
		elif msg.topic == "pi_keller/funktionen/hex":
			hex(msg.payload)
		elif msg.topic == "pi_keller/funktionen/effect":
			effect(msg.payload)
		elif msg.topic == "pi_keller/funktionen/brightness":
			setbrightness(msg.payload)
	except Exception as e:
		print("Error in Publish " + "Topic: "+ msg.topic + " Payload: " + str(msg.payload))
		print(e)

def setbrightness(msg):
	strip.setBrightness(int(msg))
	strip.show()

def color(msg):
	print("Set Color to: " + msg)
	for i in range(LED_COUNT):
		if msg == "off":
			strip.setPixelColor(i, Color(0, 0, 0))
		elif msg == "green":
			strip.setPixelColor(i, Color(255, 0, 0))
		elif msg == "red":
			strip.setPixelColor(i, Color(0, 255, 0))
		elif msg == "blue":
			strip.setPixelColor(i, Color(0, 0, 255))
		else:
			strip.setPixelColor(i, Color(255, 255, 255))
	strip.show()

def rgb(msg):
	arr = msg.split(',')
	for i in range(LED_COUNT):
		strip.setPixelColor(i, Color(int(arr[0]),int(arr[1]),int(arr[2])))
	strip.show()

def hex(msg):
	msg = msg.lstrip('#')
	col = tuple(int(msg[i:i+2], 16) for i in (0, 2, 4))
	for i in range(LED_COUNT):
		strip.setPixelColor(i, Color(col[1],col[0],col[2]))
	strip.show()

def react(msg):
	global run
	global pid
	reactstr = "python3 /home/pi/dancyPi-audio-reactive-led/python/visualization.py " + msg
	p=subprocess.Popen(reactstr, shell=True, preexec_fn=os.setsid)
	run = 1
	pid = p.pid
	print("Created React Process with ID: " + str(pid))

def effect(msg):
	global run
	global pid
	effectstr = "python /home/pi/effects.py " + msg
	#p=subprocess.Popen(effectstr, shell=True, preexec_fn=os.setsid)
	#run = 1
	#pid = p.pid
	print("Created Effects Process with ID: " + str(pid) + "Paused")

def stop():
	global run
	global pid
	if run == 1 and pid != 0:
		run = 0
		os.killpg(pid, signal.SIGTERM)
		print("Stopped Process with ID " + str(pid))

#-------------------- Mqtt Setup ------------------------
mqtt.Client.connected_flag=False

client = mqtt.Client("piKeller")
client.on_connect = on_connect
client.on_message = on_message
"""
while not client.connected_flag:
	try:
		print("connecting to broker")
		client.connect("192.168.178.34",1884,60)
	except:
		print("connection failed")
		sleep(4)
"""
client.connect("192.168.178.34",1884,60)
client.loop_forever()
#client.loop_start()
