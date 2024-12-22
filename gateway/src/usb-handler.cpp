#include <Arduino.h>
#include <usb-handler.h>

void sendUSBMessage(byte fromAdress, byte content)
{
  SerialUSB.write(fromAdress);
  SerialUSB.write(content);
}