#include <SPI.h>
#include <LoRa.h>
#include <Arduino_PMIC.h>
#include <Wire.h>

#define LORA_LOCAL_ADDRESS 0x93
#define LORA_GATEWAY_ADDRESS 0x92
#define LORA_BANDWIDTH_INDEX 7
#define LORA_SPREADING_FACTOR 10
#define LORA_CODING_RATE 5
#define LORA_TRANSMIT_POWER 2

#define USB_BAUD_RATE 9600
#define I2C_ADDRESS byte(0xE0>>1)
#define COMMAND_REGISTER byte(0)
#define REAL_RANGING_MODE_CM byte(81)
#define RANGE_HIGH_BYTE byte(2)
#define RANGE_LOW_BYTE byte(3)

#define TX_LAPSE_MS 5000

double bandwidth_kHz[10] = {7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3,
                            41.7E3, 62.5E3, 125E3, 250E3, 500E3 };

struct LoraConfig {
    byte deviceAddress;
    byte bandwidthIndex;
    byte spreadingFactor;
    byte codingRate;
    byte transmitPower;
};

long lastMillis = -55000;

volatile bool isSending = false;

volatile int detectionDistance_dm = 25;
volatile int frequency = 60;

volatile uint32_t TxTime_ms;
volatile uint32_t txInterval_ms = TX_LAPSE_MS;
volatile uint32_t lastSendTime_ms = 0;
uint32_t tx_begin_ms;

byte lastStatus;
byte status = 2;


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
  if (millis() - lastMillis >= (frequency * 1000) && canSendMessage()) {
    SerialUSB.print("Frecuencia: ");
    SerialUSB.println(frequency);
    SerialUSB.print("Distancia de deteccion: ");
    SerialUSB.print(detectionDistance_dm);
    SerialUSB.println("");
    lastStatus = status;
    status = getStatus();
    if (status != lastStatus) {
        tx_begin_ms = millis();
      sendMessage(status);
    }
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
  TxTime_ms = getTransmissionTime(tx_begin_ms);
  adjustTxInterval(tx_begin_ms, TxTime_ms);
  LoRa.receive();
}

bool canSendMessage() {
   return millis() - lastSendTime_ms > txInterval_ms && !isSending;
}

uint32_t getTransmissionTime(uint32_t tx_begin_ms) {
  TxTime_ms = millis() - tx_begin_ms;
  Serial.print("----> TX completed in ");
  Serial.print(TxTime_ms);
  Serial.println(" msecs");
  return TxTime_ms;
}

void adjustTxInterval(uint32_t tx_begin_ms, uint32_t TxTime_ms) {
  uint32_t lapse_ms = tx_begin_ms - lastSendTime_ms;
  lastSendTime_ms = tx_begin_ms;
  float duty_cycle = (100.0f * TxTime_ms) / lapse_ms;

  Serial.print("Duty cycle: ");
  Serial.print(duty_cycle, 1);
  Serial.println(" %\n");

  // Solo si el ciclo de trabajo es superior al 1% lo ajustamos
  if (duty_cycle > 1.0f) {
    txInterval_ms = TxTime_ms * 100;
  }
}

void proccessMessage(byte content)
{
  int bit0 = (content & 0x80) >> 7;
  int unit1 = (content & 0x70) >> 4;
  int unit2 = content & 0x0F;
  
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