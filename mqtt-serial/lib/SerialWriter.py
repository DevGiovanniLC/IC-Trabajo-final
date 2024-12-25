import serial


class SerialWriter:
    def __init__(self, port: str, baud_rate: int = 9600, timeout: float = 1.0):
        self.__arduino = serial.Serial(port=port, baudrate=baud_rate, timeout=timeout)
    
    def write(self, data):
        self.__arduino.write(data[0])
        self.__arduino.write(data[1])



if __name__ == "__main__":
    port = "COM3"  
    writer = SerialWriter(port)

    try:
        while True:
            data = [input("Introduce un dato: ")]
            data.append(input("Introduce otro dato: "))
            writer.write(data)
    except KeyboardInterrupt:
        writer.write("0")