#include <lora-handler.h>
#include <usb-handler.h>
#include <Arduino.h>

#define LORA_ADDRESS 0x92

void setup() {
  LoraHandler::begin(LORA_ADDRESS);
  LoraHandler::onReceive(sendUSBMessage);
}

void loop() {
}
