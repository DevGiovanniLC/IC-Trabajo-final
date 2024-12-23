from lib.MqttClientPublisher import MqttClientPublisher
from lib.SerialReader import SerialReader
from queue import Queue

def check_direction(queue, data):
    if data in directions:
        queue.put([data])
        return None
    else:
        if queue.empty(): return None
        
        list =queue.get()
        list.append(data)
        return list


if __name__ == "__main__":
    directions = [0x92, 0x93, 0x82]
    port = "COM3"  
    reader = SerialReader(port)
    readerQueue = reader.get_queue()
    mqtt = MqttClientPublisher("broker.hivemq.com", 1883)
    dataset = Queue()
    

    try:
        
        while True:
            data = readerQueue.get()
            if reader.exit: break 
            
            message = check_direction(dataset, data)
            if message is None: continue
            
            mqtt.publish("PARKING", bytearray(message))
        
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario")
        reader.stop()