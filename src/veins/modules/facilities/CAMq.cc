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

    std::make_heap(h_rid.begin(), h_rid.end());

    sendInterval = par("sendInterval");
    samplingTechnique = par("samplingTechnique");
    samplingRate = par("samplingRate").doubleValue();

    g_sendTime = simTime() + sendInterval;
    cMessage* sendTrigger = new cMessage("send");
    scheduleAt(g_sendTime, sendTrigger);

    in = findGate("in");
    out = findGate("out");

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
    if (msg->isSelfMessage()) {
        CAMq::handleSelfMsg(msg);

        g_sendTime += sendInterval;
        scheduleAt(g_sendTime, msg);
    } else {
        receivedCams++;

        switch(samplingTechnique) {
            case BERNOULLI_SAMPLING:
                sampleBernoulli(msg);
                break;
            case RESERVOIR_SAMPLING:
                sampleBernoulli(msg);
                break;
            default:
                break;
        }
    }
}

void CAMq::sampleBernoulli(cMessage* msg) {
    if (m == 0) {
        double U = dist(e2);
        int delta = floor(log2(U) / log10(1 - samplingRate));
        m = delta + 1;
    } else if (receivedCams == m) {
        DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(msg);
        int vid = bsm->getSenderId();
        std::string rid = bsm->getDemoData();

        m_updateVid2Rid(vid, rid);
        messageBuffer.push_front(msg);

        double U = dist(e2);
        int delta = floor(log2(U) / log10(1 - samplingRate));
        m = m + delta + 1;
    }
}

CAMq::Node* CAMq::h_updateNode(std::string rid, int count) {
    Node* np_curRid = new Node;
    auto res = m_rid2node.find(rid.c_str());
    if (res != m_rid2node.end()) {
        np_curRid = res->second;
        np_curRid->vCount += count;
    } else {
        if (count < 0) {
            EV_WARN << "Node with rid " << rid << " will be created with a negative count" << std::endl;
        }
        np_curRid->roadId = rid;
        np_curRid->vCount = 1;

        h_rid.push_back(np_curRid);
    }
    std::push_heap(h_rid.begin(), h_rid.end());

    return np_curRid;
}

void CAMq::m_updateVid2Rid(int vid, std::string rid) {
    auto const result = m_vid2rid.insert(std::make_pair(vid, rid));

    if (not result.second && strcmp(result.first->second.c_str(), rid.c_str())) {
        std::string old_rid = result.first->second;
        Node* n_oldRid = h_updateNode(old_rid, -1);

        result.first->second = rid;
        Node* n_newRid = h_updateNode(rid, 1);
        m_rid2node.insert(std::make_pair(rid.c_str(), n_newRid));
    } else if (not result.second && !strcmp(result.first->second.c_str(), rid.c_str())) {
        // do nothing
    } else {
        Node* n_newRid = h_updateNode(rid, 1);
        m_rid2node.insert(std::make_pair(rid.c_str(), n_newRid));
    }
}

void CAMq::handleSelfMsg(cMessage* msg) {
    EV << "[CAMq]: Forwarding " << messageBuffer.size() << " messages" << std::endl;
    while (!messageBuffer.empty()) {
        cMessage* m = messageBuffer.back();
        DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(m);
        std::string rid = bsm->getDemoData();

        if (strcmp(rid.c_str(), h_rid.front()->roadId.c_str())) {
            EV << "[CAMq]: Sending a " << m->getOwner()->getName() << "'s message" << std::endl;
            bsm->setVCount(h_rid.front()->vCount);
            send(m, out);

            h_rid.clear();
            m_vid2rid.clear();
            m_rid2node.clear();
            messageBuffer.clear();
            break;
        }

        messageBuffer.pop_back();
    }
}
