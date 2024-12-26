#include <SPI.h>
#include <LoRa.h>
#include <Arduino_PMIC.h>
#include <Wire.h>

#define LORA_LOCAL_ADDRESS 0x93
#define LORA_GATEWAY_ADDRESS 0x92
#define LORA_BANDWIDTH_INDEX 6
#define LORA_SPREADING_FACTOR 10
#define LORA_CODING_RATE 5
#define LORA_TRANSMIT_POWER 2

#define USB_BAUD_RATE 9600
#define I2C_ADDRESS byte(0xE0>>1)
#define COMMAND_REGISTER byte(0)
#define REAL_RANGING_MODE_CM byte(81)
#define RANGE_HIGH_BYTE byte(2)
#define RANGE_LOW_BYTE byte(3)

double bandwidth_kHz[10] = {7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3,
                            41.7E3, 62.5E3, 125E3, 250E3, 500E3 };

struct LoraConfig {
    byte deviceAddress;
    byte bandwidthIndex;
    byte spreadingFactor;
    byte codingRate;
    byte transmitPower;
};

long lastMillis = 0;

volatile bool isSending = false;

volatile int detectionDistance_dm = 10;
volatile int frequency = 5;


void setup() 
{
  Serial.begin(USB_BAUD_RATE);
  while (!Serial);
  Wire.begin();
  delay(100);
  if (!init_PMIC()) {
    Serial.println("Initilization of BQ24195L failed!");
  }
  else {
    Serial.println("Initilization of BQ24195L succeeded!");
  }
  LoraConfig loraConfig = {
        .deviceAddress = LORA_LOCAL_ADDRESS,
        .bandwidthIndex = LORA_BANDWIDTH_INDEX,
        .spreadingFactor = LORA_SPREADING_FACTOR,
        .codingRate = LORA_CODING_RATE,
        .transmitPower = LORA_TRANSMIT_POWER
    };
  begin(loraConfig);
}

void loop()
{
  if (millis() - lastMillis >= (frequency * 1000)) {
    SerialUSB.print("Frecuencia: ");
    SerialUSB.println(frequency);
    SerialUSB.print("Distancia de deteccion: ");
    SerialUSB.println(detectionDistance_dm);
    SerialUSB.println("");

    sendMessage(getStatus());
    lastMillis = millis();
  }
}

void begin(LoraConfig config)
{
    if (!LoRa.begin(868E6)) {      // Initicializa LoRa a 868 MHz
      Serial.println("LoRa init failed. Check your connections.");
      while (true);                
    }
    LoRa.setSignalBandwidth(long(bandwidth_kHz[config.bandwidthIndex]));
    LoRa.setSpreadingFactor(config.spreadingFactor);
    LoRa.setCodingRate4(config.codingRate);
    LoRa.setTxPower(config.transmitPower, PA_OUTPUT_PA_BOOST_PIN);
    LoRa.setSyncWord(0x12);
    LoRa.setPreambleLength(8);
    LoRa.onTxDone(finishedSending);
    LoRa.onReceive(receiveMessage);
    LoRa.receive();
}

void sendMessage(byte content) 
{
  isSending = true;
  while (!LoRa.beginPacket()) {
    delay(10);
  }
  LoRa.write(LORA_GATEWAY_ADDRESS);
  LoRa.write(LORA_LOCAL_ADDRESS);
  LoRa.write(content);
  SerialUSB.println("Hemos enviado:");
  SerialUSB.print("0x");
  SerialUSB.println(content, HEX);
  SerialUSB.println("");
  LoRa.endPacket(true);
}

void receiveMessage(int packetSize)
{
  SerialUSB.println("---------------------");
  SerialUSB.println("Estamos recibiendo:");
  if (isSending) return;
  if (packetSize == 0) return;
  const byte recipient = LoRa.read();
  const byte sender = LoRa.read();
  const byte content = LoRa.read();
  if (recipient != LORA_LOCAL_ADDRESS) return;
  if (sender != LORA_GATEWAY_ADDRESS) return;
  SerialUSB.print("0x");
  SerialUSB.println(content, HEX);
  SerialUSB.println("---------------------");
  SerialUSB.println("");
  proccessMessage(content);
}

void finishedSending()
{
  isSending = false;
  LoRa.receive();
}

void proccessMessage(byte content)
{
  int bit0 = (content & 0x80) >> 7;
  int unit1 = (content & 0x70) >> 4;
  int unit2 = content & 0x0F;
  SerialUSB.println(bit0);
  SerialUSB.println(unit1);
  SerialUSB.println(unit2);
  
  switch(bit0)
  {
    case 0:
      detectionDistance_dm = unit1 * 10 + unit2;
      break;
    case 1:
      frequency = unit2 * 60 + (unit1 * 10);
      break;
  }
}

byte getStatus()
{
  uint16_t distance = getDistance();
  SerialUSB.print("Distancia: ");
  SerialUSB.println(distance);
  SerialUSB.println("");
  return distance < (detectionDistance_dm * 10);
}

byte readRegister(byte deviceAddress, byte registerAddress)
{
  Wire.beginTransmission(deviceAddress);
  Wire.write(registerAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, byte(1));
  while(!Wire.available()) {};
  return Wire.read();
}

void writeCommand(byte command)
{
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(COMMAND_REGISTER);
  Wire.write(command);
  Wire.endTransmission();
}

uint16_t getDistance() {
  writeCommand(REAL_RANGING_MODE_CM);
  delay(70);
  byte highByte = readRegister(I2C_ADDRESS, RANGE_HIGH_BYTE);
  byte lowByte = readRegister(I2C_ADDRESS, RANGE_LOW_BYTE);
  uint16_t distance = (highByte << 8) + lowByte;
  return distance;
}