#pip install pyserial
import serial
import time

def read_arduino_serial(port: str, baud_rate: int = 9600, timeout: float = 1.0):
    try:
        # Abre el puerto serie
        with serial.Serial(port, baudrate=baud_rate, timeout=timeout) as arduino:
            print(f"Conectado a {port} a {baud_rate} baudios")
            time.sleep(2)  # Espera para asegurar la conexión

            while True:
                if arduino.in_waiting > 0:  # Comprueba si hay datos disponibles
                    data = arduino.readline().decode('utf-8').strip()
                    print(f"Recibido: {data}")

    except serial.SerialException as e:
        print(f"Error al acceder al puerto {port}: {e}")
    except KeyboardInterrupt:
        print("\nPrograma detenido por el usuario")

if __name__ == "__main__":

    port = "COM3"  
    read_arduino_serial(port)
