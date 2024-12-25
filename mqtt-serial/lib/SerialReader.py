#pip install pyserial
import threading
import serial
import time
from queue import Queue


class SerialReader:
    
    def __init__(self, port: str, baud_rate: int = 9600, timeout: float = 1.0, on_read: callable = None):
        self.exit = False
        self.__read_arduino_serial(port, baud_rate, timeout, on_read)
    
    
    def __read_arduino_serial(self, port: str, baud_rate: int = 9600, timeout: float = 1.0, callback: callable = None):
        try:
            # Abre el puerto serie
            with serial.Serial(port, baudrate=baud_rate, timeout=timeout) as arduino:
                print(f"Conectado a {port} a {baud_rate} baudios")
                time.sleep(2)  # Espera para asegurar la conexión

                while True:
                    if self.exit: break
                    if arduino.in_waiting > 0:  # Comprueba si hay datos disponibles
                        data = int.from_bytes(arduino.read(1), byteorder='little')
                        if callback is not None: callback(data)
                    
        except serial.SerialException as e:
            print(f"Error al acceder al puerto {port}: {e}")
            self.exit = True
            return Exception(f"Error al acceder al puerto {port}: {e}")
        except Exception as e:
            print(f"Error inesperado: {e}")
            self.exit = True
            return Exception(f"Error inesperado: {e}")


    def get_queue(self):
        return self.__serial_buffer

    def stop(self):
        self.exit = True





if __name__ == "__main__":
    port = "COM3"  
    reader = SerialReader(port, on_read=lambda data: print(f"Primer mensaje:{data}"))
    readerQueue = reader.get_queue()

