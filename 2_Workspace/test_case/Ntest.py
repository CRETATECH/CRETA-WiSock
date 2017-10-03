import paho.mqtt.client as mqtt
from datetime import datetime
import json
import sys
import getopt
import os
import time
import ast

def mqttConnected(client, userdata, flags, rc):
    print("SUCCESS: MQTT broker connected")

def mqttSubscribe(client, userdata, mid, granted_qos):
    print("SUCCESS: MQTT topic subcribed")

def mqttUnsubscribe(client, userdata, mid):
    print("SUCCESS: MQTT topic unsubcribed")

def mqttMessage(client, userdata, msg):
    global mqttMessRx
    print(msg.topic+" "+msg.payload.decode('utf-8'))
    # Parse string to dictionary
    mqttMessRx = ast.literal_eval(str(msg.payload.decode('utf-8')))
    print(mqttMessRx['ID'])
    print(mqttMessRx['FUNC'])
    print(mqttMessRx['ADDR'])
    print(mqttMessRx['DATA'])


def mqttGetEspId(client, userdata, msg):
    global ESP_ID
    ESP_ID = msg.payload.decode('utf-8')
    print(ESP_ID)
    
def main(argv):
    """Test system performance and stability"""
    # All variable
    global ESP_ID #Get ESP in topicTest to create esp topic
    global mqttMessRx
    global mqttMessTx
    global mode
    ESP_ID = None
    mqttMessTx = {}
    mqttMessRx = {}   
    mode = 'default'
    # Get argument
    try:
        opts, args = getopt.getopt(argv, "dh", ["default", "help"])
    except getopt.GetoptError:
        sys.exit(2)
    # Parse argument
    for opt, agr in opts:
        if opt in ('-h','--help'):
            print('This is help')
            sys.exit()
        elif opt in ('-d','--default'):
            mode = 'default'
    # Set mode
    if mode == 'default':
        mqttBroker = 'iot.eclipse.org'
        mqttTestTopic = 'topicTest'
    # Create log file
    log_file_name = 'log ' + str(datetime.now()) + '.txt'
    print(log_file_name)
    log_file = open(log_file_name, 'w')
    # Connect to MQTT broker and get esp id
    client = mqtt.Client();
    client.on_connect = mqttConnected
    client.on_subscribe = mqttSubscribe
    print("Try to connect to "+mqttBroker)
    client.connect(mqttBroker, port=1883, keepalive=60)
    print("Try to subscribe to "+mqttTestTopic)
    client.subscribe(mqttTestTopic)
    client.on_message = mqttGetEspId
    # While to get ESP_ID
    while ESP_ID == None:
        client.loop()
    # Write to log
    log_file.write('Get ' + ESP_ID + '\n')
    # Unsubscribe topicTest
    client.on_unsubscribe = mqttUnsubscribe
    client.unsubscribe(mqttTestTopic)
    # Create new topic
    mqttMasterTopic = ESP_ID + "/master"
    mqttSlaveTopic = ESP_ID + "/slave"
    log_file.write('Create new topic ' + mqttMasterTopic + '\n')
    log_file.write('Create new topic ' + mqttSlaveTopic)
    # Subscribe to new topic
    client.subscribe(mqttSlaveTopic)
    client.on_subscribe = mqttSubscribe
    client.on_message = mqttMessage
    # Create loop message
    while True:
        # Reset mqttMessRx
        mqttMessRx['ID'] = ''
        mqttMessRx['FUNC'] = ''
        mqttMessRx['ADDR'] = ''
        mqttMessRx['DATA'] = ''
        # Send cmd to turn device ON
        mqttMessTx['ID'] = ESP_ID
        mqttMessTx['FUNC'] = 'Ctrl'
        mqttMessTx['ADDR'] = '1'
        mqttMessTx['DATA'] = 'ON'
        client.publish(topic=mqttMasterTopic, payload=json.dumps(mqttMessTx))
        # Wait for respond
        while mqttMessRx['ID'] == '':
            client.loop()
        # TODO: Add timeout condition
        # TODO: Add other function test
        # TODO: Add log write
        # Wait 5 second between each test
        time.sleep(5)
    # TODO: Add out condition, add summary to log

    

if __name__ == "__main__":
    main(sys.argv[1:])


