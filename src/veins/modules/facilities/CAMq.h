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
    simtime_t g_sendTime;
    int g_bufferSize {};

    struct Node {
        std::string roadId;
        int vCount;
        bool operator<( const Node & n ) const {
            return vCount > n.vCount;
        }
    };

    struct char_cmp {
        bool operator () (const char *a,const char *b) const
        {
            return strcmp(a,b)<0;
        }
    };

    std::map<int, std::string> m_vid2rid;
    std::map<const char*, Node*, char_cmp> m_rid2node;
    std::vector<Node*> h_rid;
    std::deque<cMessage*> messageBuffer;

protected:
    int in;
    int out;

    simtime_t sendInterval;
    int samplingTechnique;
    double samplingRate;

    Node* h_updateNode(std::string rid, int count);
    void m_updateVid2Rid(int vid, std::string rid);
    void sampleBernoulli(cMessage* msg);
    void sampleReservoir(cMessage* msg);


    void initialize(int bufferSize);
    void handleMessage(cMessage* msg);
    void handleSelfMsg(cMessage* msg);
};


#endif //VEINS_CAMQ_H
