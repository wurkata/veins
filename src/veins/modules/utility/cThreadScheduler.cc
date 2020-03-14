//
// Created by wurkata on 10/03/2020.
//

#include "cThreadScheduler.h"
Register_Class(cThreadScheduler);

cEvent* cThreadScheduler::takeNextEvent() {
    
    return cSequentialScheduler::takeNextEvent();
}
