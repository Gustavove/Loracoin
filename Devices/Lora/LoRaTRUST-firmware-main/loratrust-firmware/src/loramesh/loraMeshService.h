#pragma once

#include <Arduino.h>

#include "LoraMesher.h"

#include "ArduinoLog.h"

#include "loraMeshMessage.h"

#include "message/messageManager.h"

#include "message/messageService.h"

#include "loraMeshCommandService.h"

class LoRaMeshService: public MessageService {

public:

    /**
     * @brief Construct a new LoRaMeshService object
     *
     */
    static LoRaMeshService& getInstance() {
        static LoRaMeshService instance;
        return instance;
    }

    void initLoraMesherService();

    uint16_t getDeviceID();

    void loopReceivedPackets();

    String getRoutingTable();

    bool isGateway();

    void sendReliable(DataMessage* message);

    bool sendClosestGateway(DataMessage* message);

    void setGateway();

    void removeGateway();

    uint16_t lastSentTo = 0;

    uint16_t lastReceivedFrom = 0;

    LoRaMeshCommandService* loraMesherCommandService = new LoRaMeshCommandService();

private:

    LoraMesher& radio = LoraMesher::getInstance();

    TaskHandle_t receiveLoRaMessage_Handle = NULL;

    LoRaMeshService(): MessageService(appPort::LoRaMesherApp, String("LoRaMesherApp")) {
        commandService = loraMesherCommandService;
    };

    void createReceiveMessages();

    LoRaMeshMessage* createLoRaMeshMessage(DataMessage* message);

    DataMessage* createDataMessage(AppPacket<LoRaMeshMessage>* message);

    bool isGatewaySet = false;

public:
    uint32_t numSent() { return radio.getSendPacketsNum(); };

    uint32_t numReceived() { return  radio.getReceivedDataPacketsNum() + radio.getReceivedHelloPacketsNum() + radio.getReceivedNotForMe(); };
};

