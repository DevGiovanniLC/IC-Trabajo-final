#pip install pyserial
import serial
import time


class SerialClient:
    
    def __init__(self, arduino_port, baudrate=9600, timeout=1):
        self.__arduino = serial.Serial(
            port=arduino_port, 
            baudrate=baudrate, 
            timeout=timeout
        )
        self.exit = False

    
    def write(self, data):
        print(f"Escribiendo en serial: {format(data, "x")}\n")
        self.__arduino.write(data)
    
    def on_read(self, callback: callable = None):
        self.__read_arduino_serial(self.__arduino, callback)

    
    def __read_arduino_serial(self, arduino, callback: callable = None):
        try:
            # Abre el puerto serie
            with arduino:
                print(f"Escuchando serial en puerto {arduino.port}")
                time.sleep(2)  # Espera para asegurar la conexión

                while True:
                    if self.exit: break
                    if arduino.in_waiting > 0:  # Comprueba si hay datos disponibles
                        data = arduino.read(1)
                        data = int.from_bytes(data, byteorder='big') 
                        print(f"Dato leido del serial: {format(int(data), "x")}")
                        if callback is not None: callback(data)
                    else:
                        pass
                        #time.sleep(0.01)
        except serial.SerialException as e:
            print(f"Error al acceder al puerto")
            self.exit = True
            return Exception(f"Error al acceder al puerto")
        except Exception as e:
            print(f"Error inesperado: {e}")
            self.exit = True
            return Exception(f"Error inesperado: {e}")
