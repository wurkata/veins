//
// Created by wurkata on 08/03/2020.
//

#ifndef VEINS_DENQ_H
#define VEINS_DENQ_H

#include <queue>

#include "veins/veins.h"
#include "veins/modules/messages/DemoSafetyMessage_m.h"

using namespace veins;

class DENq : public cSimpleModule {
private:
    int g_bufferSize {};

protected:
    std::queue<DemoSafetyMessage*> messageBuffer;

    void initialize(int bufferSize);
};

#endif //VEINS_DENQ_H
