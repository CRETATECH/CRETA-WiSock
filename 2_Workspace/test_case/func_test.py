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
    global send_num
    global recv_num
    global timeout_num
    global error_num
    global done_num
    recv_num = recv_num + 1
    print(msg.topic+" "+msg.payload.decode('utf-8'))
    # Parse string to dictionary
    mqttMessRx = ast.literal_eval(str(msg.payload.decode('utf-8')))
    print(mqttMessRx['ID'])
    print(mqttMessRx['FUNC'])
    print(mqttMessRx['ADDR'])
    print(mqttMessRx['DATA'])
    if(mqttMessRx['FUNC'] == 'Error'):
        error_num = error_num + 1
    else:
        done_num = done_num + 1

def mqttGetEspId(client, userdata, msg):
    global ESP_ID
    ESP_ID = msg.payload.decode('utf-8')
    print(ESP_ID)
    
def main(argv):
    """Test function"""
    # All variable
    global ESP_ID #Get ESP in topicTest to create esp topic
    global mqttMessRx
    global mqttMessTx
    global mode
    global send_num
    global recv_num
    global timeout_num
    global error_num
    global done_num
    send_num = 0
    recv_num = 0
    timeout_num = 0
    error_num = 0
    done_num = 0
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
            print('No help for tour, good luck')
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
    mqttSlaveTopic  = ESP_ID + "/slave"
    log_file.write('Create new topic ' + mqttMasterTopic + '\n')
    log_file.write('Create new topic ' + mqttSlaveTopic)
    # Subscribe to new topic
    client.subscribe(mqttSlaveTopic)
    client.on_subscribe = mqttSubscribe
    client.on_message = mqttMessage
    counter = 0


    # Reset mqttMessRx
    mqttMessRx['ID'] = ''
    mqttMessRx['FUNC'] = ''
    mqttMessRx['ADDR'] = ''
    mqttMessRx['DATA'] = ''
    # Send cmd to turn device ON
    mqttMessTx['ID'] = ESP_ID
    mqttMessTx['FUNC'] = 'Ctrl'
    mqttMessTx['ADDR'] = '1'
    mqttMessTx['DATA'] = 'On'
    client.publish(topic=mqttMasterTopic, payload=json.dumps(mqttMessTx))
    send_num = send_num + 1
    # Wait for respond
    while mqttMessRx['ID'] == '':
        counter = counter + 1
        client.loop()
        if counter > 400000:
            timeout_num = timeout_num + 1
            break
    mqttMessRx['ID'] = ''
    mqttMessRx['FUNC'] = ''
    mqttMessRx['ADDR'] = ''
    mqttMessRx['DATA'] = ''
    # Send cmd to turn device ON
    mqttMessTx['ID'] = ESP_ID
    mqttMessTx['FUNC'] = 'Ctrl'
    mqttMessTx['ADDR'] = '1'
    mqttMessTx['DATA'] = 'Off'
    client.publish(topic=mqttMasterTopic, payload=json.dumps(mqttMessTx))
    send_num = send_num + 1
    # Wait for respond
    while mqttMessRx['ID'] == '':
        counter = counter + 1
        client.loop()
        if counter > 400000:
            timeout_num = timeout_num + 1
            break

    mqttMessRx['ID'] = ''
    mqttMessRx['FUNC'] = ''
    mqttMessRx['ADDR'] = ''
    mqttMessRx['DATA'] = ''
    # Send cmd to turn device ON
    mqttMessTx['ID'] = ESP_ID
    mqttMessTx['FUNC'] = 'Data'
    mqttMessTx['ADDR'] = '1'
    mqttMessTx['DATA'] = 'Off'
    client.publish(topic=mqttMasterTopic, payload=json.dumps(mqttMessTx))
    send_num = send_num + 1
    # Wait for respond
    while mqttMessRx['ID'] == '':
        counter = counter + 1
        client.loop()
        if counter > 400000:
            timeout_num = timeout_num + 1
            break
    # Wait 5 second between each test
    time.sleep(5)
    print("---------------------------------------------------------")
    print("-------------------------RESULT--------------------------")
    print("| Msg Sent | Msg Recv | Msg Timeout | Msg Suc | Msg Err |") 
    print("|    " + str(send_num) + "     |    " + str(recv_num) + "     |       " + str(timeout_num) + "     |    " + str(done_num) + "    |    " + str(error_num) + "    |") 
    print("--------------------------END----------------------------")
    log_file.write("---------------------------------------------------------")
    log_file.write("-------------------------RESULT--------------------------")
    log_file.write("| Msg Sent | Msg Recv | Msg Timeout | Msg Suc | Msg Err |")
    log_file.write("|    " + str(send_num) + "     |    " + str(recv_num) + "     |       " + str(timeout_num) + "     |    " + str(done_num) + "    |    " + str(error_num) + "    |")
    log_file.write("--------------------------END----------------------------")
if __name__ == "__main__":
    main(sys.argv[1:])

