import time
import paho.mqtt.client as mqtt


class MqttClientPublisher:
    def __init__(self, host, port, topic):
        self.__topic = topic
        self.__unacked_publish = set()
        self.__mqttc = mqtt.Client()
        self.__mqttc.on_publish = self.__on_publish
        self.__mqttc.on_connect = self.__on_connect
        self.__mqttc.connect(host, port, 60)
        self.__mqttc.user_data_set(self.__unacked_publish)
        self.__mqttc.loop_start()
    
    
    def publish(self, payload):
        msg_info = self.__mqttc.publish(self.__topic, payload, qos=1)
        self.__unacked_publish.add(msg_info.mid)
        while len(self.__unacked_publish):
            time.sleep(0.1)
        msg_info.wait_for_publish()
    
    def __on_connect(self, client, userdata, flags, reason_code, properties=None):
        print(f"Conexión de {self.__topic} con MQTT establecida: {reason_code}")
    
    def __on_publish(self,client, userdata, mid, reason_code=None, properties=None):
        try:
            userdata.remove(mid)
            print("mensaje publicado con exito")
        except KeyError:
            print("Elemento no encontrado")
    
    def disconnect(self):
        self.__mqttc.disconnect()
        self.__mqttc.loop_stop()





if __name__ == "__main__":
    mqtt = MqttClientPublisher(
        host="broker.hivemq.com", 
        port=1883, 
        topic="MOCK"
    )
    
    mqtt.publish("15")
    time.sleep(1)
    mqtt.publish("24")
    time.sleep(1)
    mqtt.disconnect()