import paho.mqtt.client as mqtt
import json
import sys
import getopt
import os
import time
import ast
global send_num
send_num    = 0
global recv_num
recv_num    = 0
global timeout_num
timeout_num = 0
global done_num
done_num    = 0
global error_num
error_num   = 0
global sub_num
sub_num = 0

def funcTest(func):
	global send_num
	global timeout_num
	mqttPublish(func, 1)
        send_num = send_num + 1
	sended   = 1
	timeout  = 0
	while sended == 1:
		timeout = timeout + 1
		if timeout > 40000000:
			timeout_num = timeout_num + 1
			timeout = 0
			sended  = 0
			print("ERROR: Msg timeout")
			break
	mqttPublish(func, 2)
        send_num = send_num + 1	
	sended   = 1
	while sended == 1:
		timeout = timeout + 1
		if timeout > 40000000:
			timeout_num = timeout_num + 1
			timeout = 0
			sended  = 0
			print("ERROR: Msg timeout")
			break


def mqttPublish(func, data):
	# khai bao bien toan cuc
	global ID
	global topicOut
	print(topicOut)
	if func == 2:
		func_str = 'Ctrl'
	else:
		func_str = 'Data'
	if data == 1:
		data_str = 'On'
	else:
		data_str = 'Off'
	data_out = '{"ID":"' + ID + '","FUNC":"' + func_str + '","ADDR":"1","DATA":"' + data_str +'"}'
	print("Prepare to send: " + data_out)
	#print(data_out)
	client.publish(topicOut, data_out, qos = 0, retain = False)

#The callback for when receive CONNACK from server
def on_connect(client, userdata, flags, rc):
	print("SUCCESS: Connect to broker")
	

#the callback for when receive data from server
def on_message(client, userdata, msg):
	print("Message from: " + msg.topic + "\n")
	if msg.topic == "topicTest":
		# khai bao bien toan cuc
		global ID
		global initialized
		global topicOut
		global sub_num
		print("SUCCESS: Recv topic from topicTest")
		print("-------- macID: " + str(msg.payload) + "------------")
		print("Start to subscribe to topic: " + str(msg.payload) + "/slave")
		client.unsubscribe('topicTest')
		client.on_unsubscribe = on_unsubscribe
		topicIn  = str(msg.payload) + "/slave"
		client.subscribe(topicIn, qos = 0)
		client.on_subscribe = on_subscribe
		ID = str(msg.payload)
		topicOut = str(msg.payload) + "/master"
		while sub_num != 2:
			sub_num = sub_num
		initialized = 1
	else:
		global recv_num
		global error_num
		global done_num
		sended = 0
		recv_num = recv_num + 1
		parsed_json = json.loads(str(msg.payload))
		print(json.dumps(str(msg.payload)))
		if parsed_json['FUNC'] == 'Error':
			error_num = errr_num + 1
		else:
			done_num = done_num + 1
		
def on_publish(client, userdata, mid):
	print("SUCCESS: Publish msg")

def on_subscribe(client, userdata, mid, granted_qos):
	global sub_num
	sub_num = sub_num + 1
	print("subscribe lan " + str(sub_num))
	print("SUCCESS: Subscribe topic")

def on_unsubscribe (client, userdata, mid):
	print("SUCCESS: Unsubscribe topic")

print("-------------------CRETA----------------------")
print("-                                            -")
print("-------------------WiSock---------------------")
print("-                                            -")
print("-----------------Function Test----------------\n")
print("START")
global initialized		
initialized = 0		
client = mqtt.Client()
client.on_connect   = on_connect
client.on_message   = on_message
client.on_publish   = on_publish
client.on_subscribe = on_subscribe
client.on_unsubscribe = on_unsubscribe
client.connect ("iot.eclipse.org", 1883, 60)
client.subscribe("topicTest", qos = 0)
client.loop()
print("Waiting to receive macID")

while 1:
	if initialized == 1:
		print("Start to sendding test function")
		funcTest(1)
		client.loop()
		funcTest(2)
		client.loop()
		break
	client.loop()


#write log
print("---------------------------------------------------------")
print("-------------------------RESULT--------------------------")
print("| Msg Sent | Msg Recv | Msg Timeout | Msg Suc | Msg Err |") 
print("|    " + str(send_num) + "     |    " + str(recv_num) + "     |       " + str(timeout_num) + "     |    " + str(done_num) + "    |    " + str(error_num) + "    |") 
print("--------------------------END----------------------------")





