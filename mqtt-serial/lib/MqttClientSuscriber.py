# pip install paho-mqtt
import paho.mqtt.client as mqtt

class MqttClientSubscriber:
    def __init__(self, host, port, topic, onread: callable = None):
        self.__topic = topic
        self.__callback = onread
        self.__mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
        self.__mqttc.on_connect = self.__on_connect
        self.__mqttc.on_message = self.__on_message
        self.__mqttc.connect(host, port, 60)
        self.__mqttc.loop_forever()
    
    def __on_connect(self,client, userdata, flags, reason_code, properties):
        print(f"Conexión de {self.__topic} con MQTT establecida: {reason_code}")
        client.subscribe(self.__topic)

    def __on_message(self, client, userdata, msg):
        array =bytearray(msg.payload)
        if self.__callback is not None: self.__callback(array)
    
    def stop(self):
        self.__mqttc.disconnect()
        self.__mqttc.loop_stop()



if __name__ == "__main__":
    try:
        mqtt = MqttClientSubscriber(
            host="broker.hivemq.com", 
            port=1883, 
            topic="PARKING-SPACE", 
            onread=lambda data: print(f"Mensaje recibido: {data}")
        )
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario")
