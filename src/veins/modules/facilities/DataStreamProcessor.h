//
// Created by wurkata on 05/03/2020.
//

#ifndef VEINS_DATASTREAMPROCESSOR_H
#define VEINS_DATASTREAMPROCESSOR_H

#include "veins/veins.h"

class DataStreamProcessor {
public:
    DataStreamProcessor();
    virtual ~DataStreamProcessor();

protected:
    void handleMessage(cMessage* msg);
};

#endif //VEINS_DATASTREAMPROCESSOR_H
