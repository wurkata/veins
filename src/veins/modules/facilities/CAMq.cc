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

    srand(42);
}

void CAMq::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        CAMq::handleSelfMsg(msg);

        g_sendTime += sendInterval;
        scheduleAt(g_sendTime, msg);
    } else {
        messageBuffer.push_front(msg);
    }
}

void CAMq::handleSelfMsg(cMessage* msg) {
    EV << "[CAMq]: Forwarding " << messageBuffer.size() << " messages" << std::endl;
    while (!messageBuffer.empty()) {
        cMessage* m = messageBuffer.back();
        EV << "[CAMq]: Sending a " << m->getOwner()->getName() << "'s message" << std::endl;
        send(m, out);

        messageBuffer.pop_back();
    }
}
