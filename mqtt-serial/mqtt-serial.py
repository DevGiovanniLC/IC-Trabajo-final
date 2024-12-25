import threading
from lib.MqttClientPublisher import MqttClientPublisher
from lib.SerialReader import SerialReader
from lib.SerialWriter import SerialWriter
from lib.MqttClientSuscriber import MqttClientSubscriber

def serial_to_mqtt(host, port ,arduino_port, topic):
    mqtt = MqttClientPublisher(
        host=host,
        port=port,
        topic=topic
    )
    
    try:
        SerialReader(
            port=arduino_port  , 
            on_read=lambda message: 
                mqtt.publish(message)
        )
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario")

def mqtt_to_serial(host, port, arduino_port, topic):
    serial = SerialWriter(port=arduino_port)
    try:
        MqttClientSubscriber(
            host=host,
            port=port,
            topic=topic,
            onread=lambda message: serial.write(message) 
        )
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    host = "broker.hivemq.com"
    host_port = 1883
    arduino_port = "COM3"
    parking_topic = "PARKING-SPACE"
    configure_topic = "PARKING-CONFIGURE"
    
    
    thread_serial_to_mqtt = threading.Thread(target=serial_to_mqtt, args=(host, host_port, arduino_port, parking_topic))
    thread_mqtt_to_serial = threading.Thread(target=mqtt_to_serial, args=(host, host_port, arduino_port, parking_topic))
    thread_mqtt_to_serial.start()
    thread_serial_to_mqtt.start()


