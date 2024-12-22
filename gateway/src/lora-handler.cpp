#include <lora-handler.h>
#include <Arduino.h>
#include <LoRa.h>

void (*LoraHandler::receiveCallback)(byte fromAdress, byte content) = nullptr;

void LoraHandler::begin(byte address)
{
    localAddress = address;
    LoRa.onReceive(receiveMessage);
    LoRa.receive();
}

void LoraHandler::receiveMessage(int packetSize)
{
    if (packetSize == 0) return;
    if (receiveCallback == NULL) return;
    byte recipient = LoRa.read();
    byte sender = LoRa.read();
    byte content = LoRa.read();
    if (recipient != localAddress) return;
    receiveCallback(sender, content);
}

void LoraHandler::onReceive(void (*callback)(byte fromAdress, byte content))
{
    receiveCallback = callback;
}
