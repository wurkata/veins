//
// Created by wurkata on 08/03/2020.
//

#ifndef VEINS_DENQ_H
#define VEINS_DENQ_H

#include <queue>

#include "veins/veins.h"
#include "veins/modules/messages/DemoSafetyMessage_m.h"

using namespace veins;

class DENq : public cSimpleModule {
private:
    simtime_t g_sendTime;
    int g_bufferSize {};
    std::deque<cMessage*> messageBuffer;

protected:
    int in;
    int out;

    simtime_t sendInterval;

    void initialize(int bufferSize);
    void handleMessage(cMessage* msg);
    void handleSelfMsg(cMessage* msg);
};

#endif //VEINS_DENQ_H
