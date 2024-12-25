# pip install paho-mqtt
import paho.mqtt.client as mqtt
import time
import json


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
        payload = msg.payload.decode("utf-8")
        data = json.loads(payload)
        print(f"Mensaje recibido desde MQTT: {data}")
        
        firstByte = data.get("device")
        command = data.get("command")
        value = data.get("value")
        secondByte = self.__command_to_byte(command, value)

        
        if self.__callback is not None and secondByte is not None: 
            self.__callback(bytearray([firstByte, secondByte]))
            
    def __command_to_byte(self, command, value):
        secondByte = None
        match command:
            case "period":
                minutes = value.get("minutes")
                seconds = value.get("seconds")
                notValidMinutes = minutes == None or type(minutes) != int or minutes > 8 or minutes < 0
                notValidSeconds = seconds == None or type(seconds) != int or seconds > 50 or seconds < 0
                if notValidMinutes:
                    print(f"Valores incorrectos para minutos: {minutes}")
                    return
                if notValidSeconds:
                    print(f"Valores incorrectos para segundos: {seconds}")
                    return
                secondByte = (1 << 7) + (int(seconds / 10) << 4) + minutes

            case "distance":
                meters = value.get("meters")
                decimeters = value.get("decimeters")
                notValidMeters = meters == None or type(meters) != int or meters > 7 or meters < 0
                notValidDecimeters = decimeters == None or type(decimeters) != int or decimeters > 9 or decimeters < 0
                if notValidMeters:
                    print(f"Valores incorrectos para metros: {meters}")
                    return
                if notValidDecimeters:
                    print(f"Valores incorrectos para decimetros: {decimeters}")
                    return
                secondByte = (meters << 4) + decimeters
            case _:
                print(f"Comando no reconocido: {command}")
        return secondByte
    
    def on_read(self, callback: callable = None):
        self.__callback = callback
        self.__mqttc.loop_forever()
    
    def publish(self, topic,payload):
        msg_info = self.__mqttc.publish(topic, payload, qos=1)
        self.__unacked_publish.add(msg_info.mid)
        while len(self.__unacked_publish):
            time.sleep(0.1)
        msg_info.wait_for_publish()

