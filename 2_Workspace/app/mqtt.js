const $ = require('jquery')
require('paho-mqtt')

var client
var mqtt_status = 'mqtt_disconnected'
var esp_mac = null

function onConnect(){
    console.log('onConnect')
    status = 'mqtt_connected'
    client.subscribe('topicTest');
    function mqttGetTopic(message) {
        console.log('onMessageArrived:'+message.payloadString)
        esp_mac = message.payloadString
        let esp_topic_slave = esp_mac + '/slave'
        let esp_topic_master = esp_mac + '/master'
        client.unsubscribe('topicTest')
        client.subscribe(esp_topic_slave)
        client.onMessageArrived = mqttGetMessage
        $('#preloader').hide(0)
        $('#name').text(esp_mac)
        $('#control_box').show(100)
        let tx = "{\"ID\":\""+esp_mac+"\",\"FUNC\":\"Data\",\"ADDR\":\"1\",\"DATA\":\"\"}"
        console.log(tx)
        client.send(esp_topic_master, tx)
        console.log('Done')
    }
    client.onMessageArrived = mqttGetTopic
    $('#preload_mess').text('Waiting for device')
}

function mqttGetMessage(message){
    console.log('onMessageArrived:'+message.payloadString)
    let frame = JSON.parse(message.payloadString)
    if((frame.FUNC == 'Ctrl')|(frame.FUNC == 'Data')){
        if(frame.DATA == 'On'){
            $('#status').text('ON')
            $('#ctrl_btn').text('ON')
        } else if (frame.DATA == 'Off'){
            $('#status').text('OFF')
            $('#ctrl_btn').text('OFF')
        } else {
            $('#status').text('ERR')
            $('#ctrl_btn').text('ERR')
        }
    } else {
        $('#status').text('ERR')
        $('#ctrl_btn').text('ERR')
        console.log("Err frame")
    }
}

function mqttConnect(){
    $('#preload_mess').text('Connecting')
    $('#preloader').show(0)
    $('#control_box').hide(0)
    console.log('Try to connect to ws://iot.eclipse.org:80/ws\n')
    // Connect to ws://iot.eclipse.org:80/ws
    client = new Paho.MQTT.Client('iot.eclipse.org', 80, '/ws', 'HgN')
    client.connect({onSuccess:onConnect})
}

function buttonClick(){
    let esp_topic_master = esp_mac + '/master'
    let tx = null
    if($('#ctrl_btn').text() == 'OFF'){
        tx = "{\"ID\":\""+esp_mac+"\",\"FUNC\":\"Ctrl\",\"ADDR\":\"1\",\"DATA\":\"On\"}"
    } else{
        tx = "{\"ID\":\""+esp_mac+"\",\"FUNC\":\"Ctrl\",\"ADDR\":\"1\",\"DATA\":\"Off\"}"
    }
    client.send(esp_topic_master, tx)
}

$(document).ready( () => {
    $('#control_box').hide(0)
    $('#preloader').hide(0)
})