//
// Created by wurkata on 06/03/2020.
//

#ifndef VEINS_CAMQ_H
#define VEINS_CAMQ_H

#include <queue>
#include <cstdlib>

#include "veins/veins.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

class CAMq : public cSimpleModule {
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


#endif //VEINS_CAMQ_H
