#pip install pyserial
import threading
import serial
import time
from queue import Queue


class SerialReader:
    
    def __init__(self, port: str, baud_rate: int = 9600, timeout: float = 1.0):
        
        self.exit = False
        self.__serial_buffer = Queue()
        # Inicia el hilo de lectura de datos
        self.__characteristics_thread = threading.Thread(target=self.__read_arduino_serial, args=(port, baud_rate, timeout))
        self.__characteristics_thread.start()
    
    def __read_arduino_serial(self, port: str, baud_rate: int = 9600, timeout: float = 1.0):
        try:
            # Abre el puerto serie
            with serial.Serial(port, baudrate=baud_rate, timeout=timeout) as arduino:
                print(f"Conectado a {port} a {baud_rate} baudios")
                time.sleep(2)  # Espera para asegurar la conexión

                while True:
                    if self.exit: break
                    if arduino.in_waiting > 0:  # Comprueba si hay datos disponibles
                        data = int.from_bytes(arduino.read(1), byteorder='little')
                        self.__serial_buffer.put(data)
                    
        except serial.SerialException as e:
            print(f"Error al acceder al puerto {port}: {e}")
            self.exit = True
        except Exception as e:
            print(f"Error inesperado: {e}")
            self.exit = True
            

    def get_queue(self):
        return self.__serial_buffer

    def stop(self):
        self.exit = True
        self.__characteristics_thread.join()





if __name__ == "__main__":

    port = "COM3"  
    reader = SerialReader(port)
    readerQueue = reader.get_queue()

    try:
        while True:
            data = readerQueue.get()
            if reader.exit: break 
            print(data)
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario")
        reader.stop()
