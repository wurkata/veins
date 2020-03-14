//
// Created by wurkata on 05/03/2020.
//

#include "veins/modules/facilities/Merge.h"

Define_Module(Merge);

void Merge::initialize(int stage) {
    out = findGate("out");
    inCAM = findGate("inCAM");
    inDEN = findGate("inDEN");
}

void Merge::handleSelfMsg(cMessage* msg) {
    EV << "Merge: Handling self message" << std::endl;
}

void Merge::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        Merge::handleSelfMsg(msg);
    } else {
        send(msg, out);
    }
}
