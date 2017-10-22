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
    # Parse string to dictionary
    mqttMessRx = ast.literal_eval(str(msg.payload.decode('utf-8')))
    print('__Packet get: ' + str(mqttMessRx))


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
    global num_send_frame
    global num_get_success
    global num_get_fail
    global start_time
    num_send_frame = 0
    num_get_success = 0
    num_get_fail = 0
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
    print('Get ' + ESP_ID + '\n')
    log_file.write('Get ' + ESP_ID + '\n')
    # Unsubscribe topicTest
    client.on_unsubscribe = mqttUnsubscribe
    client.unsubscribe(mqttTestTopic)
    # Create new topic
    mqttMasterTopic = ESP_ID + "/master"
    mqttSlaveTopic = ESP_ID + "/slave"
    print('Create new topic ' + mqttMasterTopic + '\n')
    print('Create new topic ' + mqttSlaveTopic + '\n')
    log_file.write('Create new topic ' + mqttMasterTopic + '\n')
    log_file.write('Create new topic ' + mqttSlaveTopic + '\n')
    # Subscribe to new topic
    client.subscribe(mqttSlaveTopic)
    client.on_subscribe = mqttSubscribe
    client.on_message = mqttMessage
    # Create loop message
    start_time = datetime.now()
    while True:
        try:
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
            print('__Packet send: ', str(mqttMessTx))
            client.publish(topic=mqttMasterTopic, payload=json.dumps(mqttMessTx))
            # Wait for respond
            timeout = 0
            num_send_frame = num_send_frame + 1
            while mqttMessRx['ID'] == '':
                timeout = timeout + 1
                client.loop()
                if timeout > 100000:
                    num_get_fail = num_get_fail + 1
                    print('__Packet error: TIMEOUT')
                    break
            print('======================================')
            # Clear mqttMessRx
            mqttMessRx['ID'] = ''
            time.sleep(3)
            # Send cmd to turn device OFF
            mqttMessTx['ID'] = ESP_ID
            mqttMessTx['FUNC'] = 'Ctrl'
            mqttMessTx['ADDR'] = '1'
            mqttMessTx['DATA'] = 'Off'
            print('__Packet send: ', str(mqttMessTx))
            client.publish(topic=mqttMasterTopic, payload=json.dumps(mqttMessTx))
            # Wait for respond
            timeout = 0
            num_send_frame = num_send_frame + 1
            while mqttMessRx['ID'] == '':
                timeout = timeout + 1
                client.loop()
                if timeout > 100000:
                    num_get_fail = num_get_fail + 1
                    print('__Packet error: TIMEOUT')
                    break
            print('======================================')
            # Clear mqttMessRx
            mqttMessRx['ID'] = ''
            time.sleep(3)
            # Send cmd to turn device OFF
            mqttMessTx['ID'] = ESP_ID
            mqttMessTx['FUNC'] = 'Data'
            mqttMessTx['ADDR'] = '1'
            mqttMessTx['DATA'] = ''
            print('__Packet send: ', str(mqttMessTx))
            client.publish(topic=mqttMasterTopic, payload=json.dumps(mqttMessTx))
            # Wait for respond
            timeout = 0
            num_send_frame = num_send_frame + 1
            while mqttMessRx['ID'] == '':
                timeout = timeout + 1
                client.loop()
                if timeout > 100000:
                    num_get_fail = num_get_fail + 1
                    print('__Packet error: TIMEOUT')
                    break
            print('======================================')
            # Clear mqttMessRx
            mqttMessRx['ID'] = ''
            time.sleep(3)
        except KeyboardInterrupt:
            print('\n== TEST END ==')
            log_file.write('\nTOTAL RUNTIME: ' + str(datetime.now() - start_time))
            print('\nTOTAL RUNTIME: ' + str(datetime.now() - start_time))
            log_file.write('\nTOTAL FRAME SENT: ' + str(num_send_frame))
            print('\nTOTAL FRAME SENT: ' + str(num_send_frame))
            log_file.write('\nTOTAL FRAME SUCCESS: ' + str(num_send_frame - num_get_fail))
            print('\nTOTAL FRAME SUCCESS: ' + str(num_send_frame - num_get_fail))
            log_file.write('\nTOTAL FRAME FAILURE: ' + str(num_get_fail))
            print('\nTOTAL FRAME FAILURE: ' + str(num_get_fail))
            log_file.write('\nSUCCESS RATE: ' + str(100 - num_get_fail*100/num_send_frame))
            print('\nSUCCESS RATE: ' + str(100 - num_get_fail*100/num_send_frame))
            if((num_get_fail*100/num_send_frame) < 3):
                log_file.write('\nTEST RESULT: OK')
                print('\nTEST RESULT: OK')
            else:
                log_file.write('\nTEST RESULT: FAIL')
                print('\nTEST RESULT: FAIL')
            print('\nLOG FILE: ' + log_file.name)
            sys.exit()

if __name__ == "__main__":
    main(sys.argv[1:])
