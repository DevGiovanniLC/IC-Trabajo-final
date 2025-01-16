#include <lora-handler.h>
#include <Arduino.h>
#include <LoRa.h>

void (*LoraHandler::receiveCallback)(byte fromAddress, byte content) = nullptr;
void (*LoraHandler::txFinishedCallback)(uint32_t txTime_ms) = nullptr;
byte LoraHandler::localAddress;
double bandwidth_kHz[10] = {7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3,
                            41.7E3, 62.5E3, 125E3, 250E3, 500E3 };
volatile bool LoraHandler::isSending = false;
uint32_t LoraHandler::txBegin_ms;

void LoraHandler::begin(LoraConfig config)
{
    localAddress = config.deviceAddress;
    if (!LoRa.begin(868E6)) {
        digitalWrite(LED_BUILTIN, HIGH);
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

void LoraHandler::sendMessage(byte toAddress, byte content)
{
    isSending = true;
    while (!LoRa.beginPacket()) {
        delay(10);
    }
    LoRa.write(toAddress);
    LoRa.write(localAddress);
    LoRa.write(content);
    txBegin_ms = millis();
    LoRa.endPacket(true);
}

void LoraHandler::receiveMessage(int packetSize)
{
    if (isSending) return;
    if (packetSize == 0) return;
    if (receiveCallback == NULL) return;
    const byte recipient = LoRa.read();
    const byte sender = LoRa.read();
    const byte content = LoRa.read();
    if (recipient != localAddress) return;
    receiveCallback(sender, content);
}

void LoraHandler::onReceive(void (*callback)(byte fromAddress, byte content))
{
    receiveCallback = callback;
}

void LoraHandler::onTxFinished(void (*callback)(uint32_t txTime_ms))
{
    txFinishedCallback = callback;
}

void LoraHandler::finishedSending()
{
    txFinishedCallback(millis() - txBegin_ms);
    isSending = false;
    LoRa.receive();
}