#!/usr/bin/python3

import time
import pycurl
import serial
import re
from bs4 import BeautifulSoup
from io import BytesIO

kspage = "https://www.kickstarter.com/projects/udoo/udoo-x86-the-most-powerful-maker-board-ever"

serial_device='/dev/ttyMCC'
serial_baud = 115200

#interval between polls
delay = 30

#SERIAL  
try:
	ser = serial.Serial(serial_device, serial_baud)
	ser.flushOutput()
except FileNotFoundError as detail:
	print("FATAL: Cannot find " + serial_device )
	exit(1)
except Exception as detail:
	print("FATAL: " + str(detail.args[1]) )
	exit(1)

print("Serial device " + serial_device + " opened")

def getPledged( page ):

	buffer = BytesIO()
	c = pycurl.Curl()
	c.setopt(c.URL, page)
	c.setopt(c.WRITEDATA, buffer)

	print("Downloading...",end="")
	try: 
		c.perform()

	except:
		print("cannot download page")
		exit()

	c.close()
	print("done!")

	print("Buffering...",end="")
	dlpage = buffer.getvalue()
	
	print("done!")

	pledgediv = re.findall('\<div [^>]* id=\"\S+\"\>',str(dlpage))
	pledgespan = re.findall('\<span [^>]* id=\"\S+\"\>',str(dlpage))

	divs=""
	for i in pledgediv:
		divs += i
	for i in pledgespan:
		divs += i
	#print(divs)

	print("Souping...",end="")
	soup = BeautifulSoup(divs, "html5lib")
	print("done!")
	#<div class="num h1 bold nowrap" data-goal="100000.0" data-percent-raised="3.54275" data-pledged="354275.0" data-ref-tag="direct" id="pledged">


	print("Finding...")

	data={}
	try:
		data["pledged"] = soup.find(id="pledged")["data-pledged"].split(".")[0]
		print( "pledged:", data["pledged"] )
		data["backers"] = soup.find(id="backers_count")["data-backers-count"]
		print( "backers:", data["backers"] )
		data["hours"] = soup.find(id="project_duration_data")["data-hours-remaining"].split(".")[0]
		print( "hours:", data["hours"] )
	except:
		print("cannot find data in page")

	print("done!")

	return data

i=1
while i:
	
	print("Polling: ", time.ctime())
	now = time.time()
	try:
		data = getPledged(kspage)
	except:
		print("fail.. trying again!")

	#sending data
	ser.write(data["pledged"].encode('ascii'))
	ser.write(",".encode('ascii'))
	ser.write(data["backers"].encode('ascii'))
	ser.write(",".encode('ascii'))
	ser.write(data["hours"].encode('ascii'))
	ser.write("\n".encode('ascii'))

	interval = time.time() - now

	if interval < delay:
		print("waiting...")
		time.sleep(delay - interval)
	
	print()

ser.close()

