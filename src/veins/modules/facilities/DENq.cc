//
// Created by wurkata on 08/03/2020.
//

#include "DENq.h"

using namespace veins;

Define_Module(DENq);

void DENq::initialize(int bufferSize) {
    EV << "DENq: Initializing a queue with size " << bufferSize;
}
