import paho.mqtt.client as mqtt
from datetime import datetime
import json
import sys
import getopt
import os

def mqttConnected(client, userdata, flags, rc):
    print("MQTT broker connected")

    
def main(argv):
    """Test system performance and stability"""
    # All variable
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
    # Connect to MQTT broker
    client = mqtt.Client();

if __name__ == "__main__":
    main(sys.argv[1:])


