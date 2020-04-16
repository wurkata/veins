//
// Created by wurkata on 08/03/2020.
//

#include "DENq.h"

using namespace veins;

Define_Module(DENq);

void DENq::initialize(int bufferSize) {
    EV << "DENq: Initializing a queue with size " << bufferSize;

    sendInterval = par("sendInterval");

    g_sendTime = simTime() + sendInterval;
    cMessage* sendTrigger = new cMessage("send");
    // scheduleAt(g_sendTime, sendTrigger);

    in = findGate("in");
    out = findGate("out");
}

void DENq::handleMessage(cMessage* msg) {
    // if (msg->isSelfMessage()) {
    //     DENq::handleSelfMsg(msg);

    //     g_sendTime += sendInterval;
    //     scheduleAt(g_sendTime, msg);
    // } else {
        messageBuffer.push_front(msg);
        messageBuffer.pop_back();
        send(msg, out);
    // }
}

void DENq::handleSelfMsg(cMessage* msg) {
    EV << "[DENq]: Forwarding " << messageBuffer.size() << " messages" << std::endl;
    while (!messageBuffer.empty()) {
        cMessage* m = messageBuffer.back();
        EV << "[DENq]: Sending a " << m->getOwner()->getName() << "'s message" << std::endl;
        send(m, out);

        messageBuffer.pop_back();
    }
}
