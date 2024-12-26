#include <Arduino.h>
#include <usb-handler.h>

void (*USBHandler::receiveCallback)(byte toAddress, byte content) = nullptr;

void USBHandler::sendMessage(byte fromAdress, byte content)
{
    SerialUSB.write(fromAdress);
    SerialUSB.write(content);
}

void USBHandler::begin(int baudRate)
{
    // SerialUSB.begin(baudRate);
    // while (!SerialUSB) {
    //     digitalWrite(LED_BUILTIN, HIGH);
    // }
    // digitalWrite(LED_BUILTIN, LOW);
}

void USBHandler::onReceive(void (*callback)(byte toAddress, byte content))
{
    receiveCallback = callback;
}

void USBHandler::update()
{
    if (SerialUSB.available() > 1)
    {
        if (receiveCallback == NULL) return;
        const byte toAddress = SerialUSB.read();
        const byte content = SerialUSB.read();
        receiveCallback(toAddress, content);
    }
}
