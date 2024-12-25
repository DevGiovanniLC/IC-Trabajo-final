# pip install paho-mqtt
import paho.mqtt.client as mqtt

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, reason_code, properties):
    print(f"Conexión con MQTT establecida: {reason_code}")
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("PARKING")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    array =bytearray(msg.payload)
    print(msg.topic+" "+f"Direccion: {array[0]} - Count: {array[1]}")

mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect("broker.hivemq.com", 1883, 60)

mqttc.loop_forever()