import threading
import time

from lib.SerialClient import SerialClient
from lib.MqttClient import MqttClient

if __name__ == "__main__":
    host = "brokermqtt.duckdns.org"
    host_port = 1883
    arduino_port = "COM6"
    parking_topic = "PARKING-SPACE"
    configure_topic = "PARKING-CONFIGURE"
    
    serial_client = SerialClient(arduino_port)
    
    mqtt_client = MqttClient(
        host=host,
        port=host_port,
        topic=configure_topic,
    )
    
    thread_mqtt=threading.Thread(target=mqtt_client.on_read, args=(lambda data: serial_client.write(data),), daemon=True)
    thread_serial=threading.Thread(target=serial_client.on_read, args=(lambda data: mqtt_client.publish(parking_topic, data),), daemon=True)
    
    thread_mqtt.start()
    thread_serial.start()
    
    time.sleep(1)
    input("Presione cualquier tecla para terminar\n\n")
    



