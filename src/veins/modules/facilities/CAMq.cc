//
// Created by wurkata on 06/03/2020.
//

#include "veins/modules/facilities/CAMq.h"

Define_Module(CAMq);

void CAMq::initialize(int bufferSize) {
    EV << "CAMq: Initializing a queue with size " << bufferSize;

    sendInterval = par("sendInterval");

    g_sendTime = simTime() + sendInterval;
    cMessage* sendTrigger = new cMessage("send");
    scheduleAt(g_sendTime, sendTrigger);

    in = findGate("in");
    out = findGate("out");
}

void CAMq::handleMessage(cMessage* msg) {
    EV << "[CAMq]: Received a message" << std::endl;

    if (msg->isSelfMessage()) {
        CAMq::handleSelfMsg(msg);

        g_sendTime += sendInterval;
        scheduleAt(g_sendTime, msg);
    } else {
        messageBuffer.push_back(msg);
    }
}

void CAMq::handleSelfMsg(cMessage* msg) {
    EV << "[CAMq]: Forwarding " << messageBuffer.size() << " messages" << std::endl;
    for (cMessage* msg : messageBuffer) {
        EV << "[CAMq]: Forwarding message" << std::endl;
        if (msg != nullptr) {
            EV << "[CAMq]: Sending a " << msg->getOwner()->getName() << "'s message" << std::endl;
            send(msg, out);
        }
    }
}
