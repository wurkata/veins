//
// Created by wurkata on 06/03/2020.
//

#ifndef VEINS_CAMQ_H
#define VEINS_CAMQ_H

#include <queue>

#include "veins/veins.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

using namespace veins;

class CAMq : public cSimpleModule {
private:
    int g_bufferSize {};
    std::vector<cMessage*> messageBuffer;

protected:
    int in;
    int out;

    void initialize(int bufferSize);

    void handleMessage(cMessage* msg);

public:
    void forwardMessages();
};


#endif //VEINS_CAMQ_H
