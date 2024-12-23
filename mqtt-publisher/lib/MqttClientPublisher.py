import time
import paho.mqtt.client as mqtt


class MqttClientPublisher:
    def __init__(self, server, port):
        self.__unacked_publish = set()
        self.__mqttc = mqtt.Client()
        self.__mqttc.on_publish = self.__on_publish
        self.__mqttc.connect("broker.hivemq.com", 1883, 60)
        self.__mqttc.user_data_set(self.__unacked_publish)
        self.__mqttc.loop_start()
    
    
    def publish(self, topic, payload):
        msg_info = self.__mqttc.publish(topic, payload, qos=1)
        self.__unacked_publish.add(msg_info.mid)
        while len(self.__unacked_publish):
            time.sleep(0.1)
        msg_info.wait_for_publish()
    
    def __on_publish(self,client, userdata, mid, reason_code=None, properties=None):
        try:
            userdata.remove(mid)
            print("Message with mid " + str(mid) + " was published")
        except KeyError:
            print("Failed to remove mid from unacked_publish")
    
    def disconnect(self):
        self.__mqttc.disconnect()
        self.__mqttc.loop_stop()


if __name__ == "__main__":
    mqtt = MqttClientPublisher("broker.hivemq.com", 1883)
    mqtt.publish("PARKING", "1")
    time.sleep(1)
    mqtt.publish("PARKING", "0")
    time.sleep(1)
    mqtt.disconnect()