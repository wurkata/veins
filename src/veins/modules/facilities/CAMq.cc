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

void CAMq::initialize(int bufferSize) {
    srand(42);

    std::make_heap(h_rid.begin(), h_rid.end());

    sendInterval = par("sendInterval");
    samplingTechnique = par("samplingTechnique");
    samplingRate = par("samplingRate").doubleValue();
    maxSampleSize = par("maxSampleSize");

    RECEIVED_CAMs = 0;
    elementi = 0;
    m = 0;

    g_sendTime = simTime() + sendInterval;
    cMessage* sendTrigger = new cMessage("send", SEND_EVT);
    scheduleAt(g_sendTime, sendTrigger);

    in = findGate("in");
    out = findGate("out");

    switch(samplingTechnique) {
        case BERNOULLI_SAMPLING:
            sampleBernoulli(sendTrigger, elementi);
            break;
        case RESERVOIR_SAMPLING:
            m = maxSampleSize;
            alpha = 22 * maxSampleSize;
            break;
        default:
            break;
    }
    cMessage* recordTrigger = new cMessage("record", RECORD_MAX_SAMPLE_SIZE_EVT);
    scheduleAt(simTime() + 1, recordTrigger);
}

void CAMq::handleMessage(cMessage* msg) {
    if (msg->isSelfMessage()) {
        CAMq::handleSelfMsg(msg);
    } else {
        elementi++;
        switch(samplingTechnique) {
            case BERNOULLI_SAMPLING:
                sampleBernoulli(msg, elementi);
                break;
            case RESERVOIR_SAMPLING:
                sampleReservoir(msg, elementi);
                break;
            default:
                break;
        }
    }
}

void CAMq::sampleBernoulli(cMessage* msg, int ei) {
    if (m == 0) {
        double U = dist(e2);
        int delta = floor(log2(U) / log10(1 - samplingRate));
        m = delta + 1;
    } else if (ei == m) {
        RECEIVED_CAMs++;

        DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(msg);
        int vid = bsm->getSenderId();
        std::string rid = bsm->getDemoData();
        m_updateVid2Rid(vid, rid, msg);

        double U = dist(e2);
        int delta = floor(log2(U) / log10(1 - samplingRate));
        m = m + delta + 1;
    }
}

void CAMq::sampleReservoir(cMessage* msg, int ei) {
    if (ei < maxSampleSize) {
        RECEIVED_CAMs++;
        reservoir[ei - 1] = msg;

        DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(msg);
        int vid = bsm->getSenderId();
        std::string rid = bsm->getDemoData();
        m_updateVid2Rid(vid, rid, msg);
    }
    if (ei >= maxSampleSize && ei == m) {
        RECEIVED_CAMs++;

        if (ei == maxSampleSize) {
            DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(msg);
            int vid = bsm->getSenderId();
            std::string rid = bsm->getDemoData();
            m_updateVid2Rid(vid, rid, msg);
            reservoir[maxSampleSize - 1] = msg;
        } else {
            DemoSafetyMessage* bsm = dynamic_cast<DemoSafetyMessage*>(msg);
            int vid = bsm->getSenderId();
            std::string rid = bsm->getDemoData();
            m_updateVid2Rid(vid, rid, msg);

            int U = dist(e2);
            int I = floor(maxSampleSize*U);
            reservoir[I] = msg;
        }

        int delta;
        if (ei <= alpha) {
            delta = searchDelta(ei);
        } else {
            double c = (double) (ei + 1) / (ei - maxSampleSize + 1);
            double w = dist(e2);
            long s;
            while (true) {
                double u = dist(e2);
                // TODO: Check the correctness of the algorithm
                double x = ei * (pow(w, (-1 / (double) maxSampleSize))- 1.0);
                s = (long) x;
                double g = (maxSampleSize) / (ei + x) * pow(ei / (ei + x), maxSampleSize);
                double h = ((double) maxSampleSize / (ei + 1))
                           * pow((double) (ei - maxSampleSize + 1) / (ei + s - maxSampleSize + 1), maxSampleSize + 1);
                if (u <= h / (c * g)) {
                    break;
                }
                double f = 1;
                for (int i = 0; i <= s; ++i) {
                    f *= (double) (ei - maxSampleSize + i) / (ei + 1 + i);
                }
                f *= maxSampleSize;
                f /= (ei - maxSampleSize);
                if (u <= f / (c * g)) {
                    break;
                }
                w = dist(e2);
            }
            delta = s;
        }

        m = ei + delta + 1;
    }
}

int CAMq::searchDelta(int ei) {
    long s = 0;
    double u = dist(e2);
    double quotient = (double)(ei + 1 - maxSampleSize)/(ei + 1);
    int i = 1;
    do {
        quotient *= (double)(ei + 1 + i - maxSampleSize)/(ei + i + 1);
        ++s;
        ++i;
    } while (quotient > u);
    return s + 1;
}

CAMq::Node* CAMq::h_updateNode(std::string rid, int count, cMessage* msg) {
    Node* np_curRid = new Node;
    auto res = m_rid2node.find(rid.c_str());
    if (res != m_rid2node.end()) {
        np_curRid = res->second;
        np_curRid->vCount += count;
        np_curRid->msg = msg;
    } else {
        if (count < 0) {
            EV_WARN << "Node with rid " << rid << " will be created with a negative count" << std::endl;
        }
        np_curRid->roadId = rid;
        np_curRid->vCount = 1;
        np_curRid->msg = msg;

        h_rid.push_back(np_curRid);
    }
    std::push_heap(h_rid.begin(), h_rid.end());

    return np_curRid;
}

void CAMq::m_updateVid2Rid(int vid, std::string rid, cMessage* msg) {
    auto const result = m_vid2rid.insert(std::make_pair(vid, rid));

    if (not result.second && strcmp(result.first->second.c_str(), rid.c_str())) {
        std::string old_rid = result.first->second;
        Node* n_oldRid = h_updateNode(old_rid, -1, msg);

        result.first->second = rid;
        Node* n_newRid = h_updateNode(rid, 1, msg);
        m_rid2node.insert(std::make_pair(rid.c_str(), n_newRid));
    } else if (not result.second && !strcmp(result.first->second.c_str(), rid.c_str())) {
        // do nothing
    } else {
        Node* n_newRid = h_updateNode(rid, 1, msg);
        m_rid2node.insert(std::make_pair(rid.c_str(), n_newRid));
    }
}

void CAMq::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
        case SEND_EVT: {
            if (!h_rid.empty()) {
                cMessage *m = h_rid.front()->msg;
                DemoSafetyMessage *bsm = dynamic_cast<DemoSafetyMessage *>(m);
                bsm->setVCount(h_rid.front()->vCount);
                send(m, out);

                h_rid.clear();
                m_vid2rid.clear();
                m_rid2node.clear();

                switch (samplingTechnique) {
                    case BERNOULLI_SAMPLING: {
                        break;
                    }
                    case RESERVOIR_SAMPLING: {
                        elementi = 0;
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }

            g_sendTime += sendInterval;
            scheduleAt(g_sendTime, msg);
            break;
        }
        case RECORD_MAX_SAMPLE_SIZE_EVT: {
            maxSampleSize = h_rid.size() > maxSampleSize ? h_rid.size() : maxSampleSize;

            scheduleAt(simTime() + 1, msg);
            break;
        }
        default:
            break;
    }
}

void CAMq::finish() {
    recordScalar("receivedBSMs", RECEIVED_CAMs);
    recordScalar("maxSampleSize", maxSampleSize);
}
