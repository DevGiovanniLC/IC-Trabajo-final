#include <Arduino.h>

// Declaración de variables
uint8_t count = 0;
uint8_t directions[3] = {0x92, 0x93, 0x82};

void setup() {
    // Inicializar la comunicación serial
    Serial.begin(9600);  // Configura la velocidad del puerto serial
    while (!Serial) {
        ;  // Espera a que el puerto serial esté listo (necesario para placas como Leonardo)
    }

    Serial.println("Iniciando programa...");
}

void loop() {
  // Incrementa el contador
  clearSerialBuffer();
  count++;

  // Escribe datos en el puerto serial
  Serial.write(directions[count % 3]);
  Serial.write(count);

  // Verifica si hay datos disponibles para leer
  if (Serial.available() > 0) {
    // Lee el dato recibido
    uint8_t receivedData = Serial.read();
  }

  // Espera 10 segundos
  delay(10000);
}

void clearSerialBuffer() {
    while (Serial.available() > 0) {
        Serial.read();  // Lee y descarta cada byte disponible
    }
}