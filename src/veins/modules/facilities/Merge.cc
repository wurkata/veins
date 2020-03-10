//
// Created by wurkata on 05/03/2020.
//

#include "veins/modules/facilities/Merge.h"

Define_Module(Merge);

void Merge::initialize(int stage) {
    simtime_t pollInterval = par("pollInterval");

    out = findGate("out");
    inCAM = findGate("inCAM");
    inDEN = findGate("inDEN");

    mod = getParentModule()->getSubmodule("camq");
    camq = check_and_cast<CAMq*>(mod);

    cMessage* pollTrigger = new cMessage("poll");
    scheduleAt(simTime() + pollInterval, pollTrigger);
}

void Merge::poll() {
    camq->forwardMessages();
}

void Merge::handleSelfMsg(cMessage* msg) {
    EV << "Merge: Handling self message" << std::endl;
    Merge::poll();

    pollInterval += pollInterval;
    cMessage* pollTrigger = new cMessage("poll");
    scheduleAt(simTime() + pollInterval, pollTrigger);
}

void Merge::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        Merge::handleSelfMsg(msg);
    } else {
        send(msg, out);
    }
}
