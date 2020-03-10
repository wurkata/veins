//
// Created by wurkata on 06/03/2020.
//

#include "veins/modules/facilities/CAMq.h"

Define_Module(CAMq);

void CAMq::initialize(int bufferSize) {
    EV << "CAMq: Initializing a queue with size " << bufferSize;

    in = findGate("in");
    out = findGate("out");
}

void CAMq::handleMessage(cMessage* msg) {
    EV << "[CAMq]: Received a message" << std::endl;

    messageBuffer.push_back(msg);
}

void CAMq::forwardMessages() {
    EV << "[CAMq]: Forwarding " << messageBuffer.size() << " messages" << std::endl;
    for (cMessage* msg : messageBuffer) {
        EV << "[CAMq]: Forwarding message" << std::endl;
        if (msg != nullptr) send(msg, out);
    }
}
