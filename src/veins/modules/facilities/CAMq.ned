package org.car2x.veins.modules.facilities;

simple CAMq {
    parameters:
        int windowSize = default(100);
        int samplingTechnique;
        double samplingRate;
    gates:
        input in; // add item to the queue
        output out; // remove item from the queue
        
        input aggrIn @directIn;
}
