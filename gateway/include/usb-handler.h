#include <Arduino.h>

class USBHandler {
public:
    static void begin(int baudRate);
    static void sendMessage(byte fromAdress, byte content);
    static void onReceive(void (*callback)(byte toAddress, byte content));
    static void update();
private:
    static void (*receiveCallback)(byte toAddress, byte content);
};
