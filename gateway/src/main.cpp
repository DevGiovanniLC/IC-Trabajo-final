#include <lora-handler.h>
#include <usb-handler.h>
#include <Arduino.h>

#define LORA_ADDRESS 0x92
#define LORA_BANDWIDTH_INDEX 5 // 0-9 mayor numero es menor tiempo de transmision
#define LORA_SPREADING_FACTOR 8 // 6-12 mayor numero es mayor tiempo de transmision
#define LORA_CODING_RATE 5 // 5-8 mayor numero es mayor tiempo de transmision
#define LORA_TRANSMIT_POWER 2 // 2-20 elegir valor bajo para corto alcance

#define USB_BAUD_RATE 9600

void setup()
{
    USBHandler::begin(USB_BAUD_RATE);
    LoraConfig loraConfig = {
        .deviceAddress = LORA_ADDRESS,
        .bandwidthIndex = LORA_BANDWIDTH_INDEX,
        .spreadingFactor = LORA_SPREADING_FACTOR,
        .codingRate = LORA_CODING_RATE,
        .transmitPower = LORA_TRANSMIT_POWER
    };
    LoraHandler::begin(loraConfig);
    USBHandler::onReceive(LoraHandler::sendMessage);
    LoraHandler::onReceive(USBHandler::sendMessage);
}

void loop()
{
    USBHandler::update();
}
