# pip install paho-mqtt
import paho.mqtt.client as mqtt
import time


class MqttClient:
    def __init__(self, host, port, topic):
        self.host = host
        self.port = port
        self.__callback = None
        self.__topic = topic
        self.__unacked_publish = set()
        self.__mqttc = mqtt.Client()
        self.__mqttc.on_publish = self.__on_publish
        self.__mqttc.on_connect = self.__on_connect
        self.__mqttc.on_message = self.__on_message
        self.__mqttc.connect(host, port, 60)
        self.__mqttc.user_data_set(self.__unacked_publish)

    def __on_connect(self, client, userdata, flags, reason_code, properties=None):
        print(f"Conexión con MQTT establecida con host {self.host} en puerto {self.port}")
        client.subscribe(self.__topic)
    
    def __on_publish(self,client, userdata, mid, reason_code=None, properties=None):
        try:
            userdata.remove(mid)
            print("Mensaje publicado a MQTT con exito")
        except KeyError:
            print("Elemento no encontrado")
    
    def __on_message(self, client, userdata, msg):
        print(f"Mensaje recibido desde MQTT: {msg.payload}")
        message = msg.payload.decode("utf-8")
        message = int(message)
        if self.__callback is not None: self.__callback(message)
    
    def on_read(self, callback: callable = None):
        self.__callback = callback
        self.__mqttc.loop_forever()
    
    def publish(self, topic,payload):
        msg_info = self.__mqttc.publish(topic, payload, qos=1)
        self.__unacked_publish.add(msg_info.mid)
        while len(self.__unacked_publish):
            time.sleep(0.1)
        msg_info.wait_for_publish()

