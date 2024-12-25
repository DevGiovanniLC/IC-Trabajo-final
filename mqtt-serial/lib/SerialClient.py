#pip install pyserial
import serial
import time
import json

class SerialClient:
    
    def __init__(self, arduino_port, baudrate=9600, timeout=1):
        self.__arduino = serial.Serial(
            port=arduino_port, 
            baudrate=baudrate, 
            timeout=timeout
        )
        self.exit = False

    
    def write(self, data):
        print(f"Escribiendo en serial: 0x{format(data[0], "x")}, 0x{format(data[1], "x")}\n")
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
                    if arduino.in_waiting > 1:  # Comprueba si hay datos disponibles
                        slot = arduino.read(1)
                        occupied = arduino.read(1)
                        slot = int.from_bytes(slot, byteorder='big') 
                        occupied = int.from_bytes(occupied, byteorder='big')
                        print(f"Dato leido del serial: {format(int(slot), "x")}")
                        jsonContent = {
                            "slot": slot,
                            "occupied": occupied 
                        }
                        if callback is not None: callback(json.dumps(jsonContent))
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
