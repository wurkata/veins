//
// Created by wurkata on 06/03/2020.
//

#include "veins/modules/facilities/CAMq.h"

Define_Module(CAMq);

const int DEFAULT_SAMPLING = 0;
const int BERNOULLI_SAMPLING = 1;
const int RESERVOIR_SAMPLING = 2;

std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<> dist(0, 1);

int m = 0;
int receivedCams = 0;

void CAMq::initialize(int bufferSize) {
    srand(42);
    samplingTechnique = par("samplingTechnique");
    samplingRate = par("samplingRate").doubleValue();

    in = findGate("in");
    out = findGate("out");

    cMessage* sendTrigger = new cMessage("initialize");
    switch(samplingTechnique) {
        case BERNOULLI_SAMPLING:
            sampleBernoulli(sendTrigger);
            break;
        case RESERVOIR_SAMPLING:
            sampleBernoulli(sendTrigger);
            break;
        default:
            break;
    }
}

void CAMq::handleMessage(cMessage* msg) {
    receivedCams++;

    switch(samplingTechnique) {
        case BERNOULLI_SAMPLING:
            sampleBernoulli(msg);
            break;
        case RESERVOIR_SAMPLING:
            sampleBernoulli(msg);
            break;
        default:
            send(msg, out);
            break;
    }
}

void CAMq::sampleBernoulli(cMessage* msg) {
    if (m == 0) {
        double U = dist(e2);
        int delta = floor(log2(U) / log10(1 - samplingRate));
        m = delta + 1;
    } else if (receivedCams == m) {
        send(msg, out);
        double U = dist(e2);
        int delta = floor(log2(U) / log10(1 - samplingRate));
        m = m + delta + 1;
    }
}

void CAMq::finish() {
    
}
