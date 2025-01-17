#include <lora-handler.h>
#include <usb-handler.h>
#include <Arduino.h>
#include <battery.h>

#define LORA_ADDRESS 0x92
#define LORA_BANDWIDTH_INDEX 7   // 0-9 mayor numero es menor tiempo de transmision
#define LORA_SPREADING_FACTOR 10 // 6-12 mayor numero es mayor tiempo de transmision
#define LORA_CODING_RATE 5       // 5-8 mayor numero es mayor tiempo de transmision
#define LORA_TRANSMIT_POWER 2    // 2-20 elegir valor bajo para corto alcance

#define USB_BAUD_RATE 9600

#define TX_BUFFER_SIZE 64
#define TX_LAPSE_MS 2000

byte txBufferIndex = 0;
byte txBuffer[TX_BUFFER_SIZE];
uint32_t lastLoraSendTime_ms = 0;
uint32_t txInterval_ms = 10000;
uint32_t txBegin_ms;

bool canSendMessage();
void handleUsbMessage(byte toAddress, byte content);
void updateTxInterval(uint32_t txTime_ms);
volatile bool isSending = false;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    pinMode(LED_BUILTIN, OUTPUT);
    init_PMIC();
    LoraConfig loraConfig = {
        .deviceAddress = LORA_ADDRESS,
        .bandwidthIndex = LORA_BANDWIDTH_INDEX,
        .spreadingFactor = LORA_SPREADING_FACTOR,
        .codingRate = LORA_CODING_RATE,
        .transmitPower = LORA_TRANSMIT_POWER};
    LoraHandler::begin(loraConfig);
    USBHandler::onReceive(handleUsbMessage);
    LoraHandler::onReceive(USBHandler::sendMessage);
    LoraHandler::onTxFinished(updateTxInterval);
}

void loop()
{
    USBHandler::update();
    if (canSendMessage()
        && txBufferIndex > 0
    )
    {
        byte content = txBuffer[--txBufferIndex];
        byte toAddress = txBuffer[--txBufferIndex];
        txBegin_ms = millis();
        isSending = true;
        LoraHandler::sendMessage(toAddress, content);
    }
}

void handleUsbMessage(byte toAddress, byte content)
{
    if (txBufferIndex < TX_BUFFER_SIZE - 1)
    {
        txBuffer[txBufferIndex++] = toAddress;
        txBuffer[txBufferIndex++] = content;
    }
}

bool canSendMessage()
{
    return millis() - lastLoraSendTime_ms > txInterval_ms && !isSending;
}

void updateTxInterval(uint32_t txTime_ms)
{
    isSending = false;
    uint32_t txLapse_ms = millis() - lastLoraSendTime_ms;
    lastLoraSendTime_ms = txBegin_ms;
    float dutyCycle = 100.0f * txTime_ms / txLapse_ms;

    // Serial.print("txTime_ms: ");
    // Serial.println(txTime_ms);

    // Serial.print("txLapse_ms: ");
    // Serial.println(txLapse_ms);

    // Serial.print("txBegin_ms: ");
    // Serial.println(txBegin_ms);

    // Serial.print("lastLoraSendTime_ms: ");
    // Serial.println(lastLoraSendTime_ms);

    // Serial.print("txInterval_ms: ");
    // Serial.println(txInterval_ms);

    // Serial.print("dutyCycle: ");
    // Serial.println(dutyCycle);

    if (dutyCycle > 1.0f)
    {
        txInterval_ms = txTime_ms * 100;
    }
}