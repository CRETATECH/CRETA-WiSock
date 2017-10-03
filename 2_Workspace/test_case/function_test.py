import paho.mqtt.client as mqtt
import json

send_num    = 0
recv_num    = 0
timeout_num = 0
done_num    = 0
error_num   = 0
initialized = 0 # =1 if receive macID from topicTest
sended      = 0
timeout     = 0

def funcTest(func):
	mqttPublish(func, 1)
        send_num = sended_num + 1
	sended = 1
	while sended == 1:
		timeout = timeout + 1
		if timeout > 500000:
			timeout = 0
			sended  = 0
			break
	mqttPublish(func, 2)
        send_num = sended_num + 1	
	sended = 1
	while sended == 1:
		timeout = timeout + 1
		if timeout > 500000:
			timeout = 0
			sended  = 0
			break


def mqttPublish(func, data):
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
	print("Connected with result code" + str(rc))
	

#the callback for when receive data from server
def on_message(client, userdata, msg):
	print("Message from: " + msg.topic + "\n")
	if msg.topic == "topicTest":
		print("--- macID: " + str(msg.payload) + "---")
		client.subscribe(msg.topic + "/slave", qos = 0)
		ID = str(msg.payload)
		topicOut = str(msg.payload) + "master"
		initialized = 1
	else:
		sended = 0
		recv_num = recv_num + 1
		parsed_json = json.loads(str(msg.payload))
		print(json.dumps(str(msg.payload)))
		if parsed_json['FUNC'] == 'Error':
			error_num = errr_num + 1
		else:
			done_num = done_num + 1
		
		
		
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
print("Waiting to receive macID")
client.connect ("iot.eclipse.org", 1883, 60)
client.subscribe("topicTest", qos = 0)
client.loop()

while 1:
	if initialized == 1:
		print("Start to sendding test function")
		funcTest(1)
		client.loop()
		funcTest(2)
		client.loop()
		break

#write log






