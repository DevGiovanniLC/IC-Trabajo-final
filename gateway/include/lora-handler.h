#ifndef LORA_RECEIVER_H
#define LORA_RECEIVER_H

#include <Arduino.h>

class LoraHandler
{
public:
    static void begin(byte address);
    static void onReceive(void (*callback)(byte fromAdress, byte content));
private:
    static void receiveMessage(int packetSize);
    static void (*receiveCallback)(byte fromAdress, byte content);
    static byte localAddress;
};

#endif