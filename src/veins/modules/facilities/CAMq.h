//
// Created by wurkata on 06/03/2020.
//

#ifndef VEINS_CAMQ_H
#define VEINS_CAMQ_H

#include <queue>
#include <map>
#include <cstdlib>
#include <cmath>
#include <random>

#include "veins/veins.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"
#include "veins/modules/messages/DemoSafetyMessage_m.h"

using namespace veins;

class CAMq : public cSimpleModule {
private:
    struct char_cmp {
        bool operator () (const char *a,const char *b) const
        {
            return strcmp(a,b)<0;
        }
    };

protected:
    int in;
    int out;

    int samplingTechnique;
    double samplingRate;

    void sampleBernoulli(cMessage* msg);
    void sampleReservoir(cMessage* msg);

    void initialize(int bufferSize);
    void finish();
    void handleMessage(cMessage* msg);
};


#endif //VEINS_CAMQ_H
