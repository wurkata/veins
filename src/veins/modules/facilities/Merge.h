//
// Created by wurkata on 05/03/2020.
//

#ifndef VEINS_MERGE_H
#define VEINS_MERGE_H

#include <queue>
#include <thread>
#include <chrono>

#include "veins/veins.h"
#include "veins/modules/facilities/CAMq.h"

class Merge : public cSimpleModule {
private:
    simtime_t pollInterval;

    int out;
    int inCAM;
    int inDEN;

    cModule* mod;
    CAMq* camq;
protected:
    void initialize(int stage);
    void handleMessage(cMessage* msg);
    void handleSelfMsg(cMessage* msg);
    void poll();
};


#endif //VEINS_MERGE_H
