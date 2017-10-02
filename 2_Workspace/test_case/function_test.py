#import paho.mqtt.client as mqtt
import json

#The callback for when receive CONNACK from server
#def on_connect(client, userdata, flags, rc):
#	print("connected with result code" + str(rc))
	

#the callback for when receive data form server
#def on_message(client, userdata, msg):
#	print("Message from: "msg.topic + "\n")
#	if msg.topic == "topicTest":
#		print("--- macID: " + str(msg.payload) + "---\n")
#	else:
		

#client = mqtt.Client()
#client.on_connect = on_connect
#client.on_message = on_message

#client.connect ("iot.eclipse.org", 1883, 60)

#client.loop_forever()

print json.dumps("nghia", sort_keys = False,  separators = (',',':'))
