//
// Created by wurkata on 10/03/2020.
//

#ifndef VEINS_CTHREADSCHEDULER_H
#define VEINS_CTHREADSCHEDULER_H


#include <omnetpp.h>
using namespace omnetpp;

class cThreadScheduler : public cSequentialScheduler {
public:
    virtual cEvent *takeNextEvent() override;
};

#endif //VEINS_CTHREADSCHEDULER_H
