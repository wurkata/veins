package org.car2x.veins.modules.facilities;

simple CAMq {
    parameters:
        int windowSize = default(100);
        int samplingTechnique;
        double samplingRate;
        double sendInterval @unit("s") = default(3s);  // time until next processing of CAMs
    gates:
        input in; // add item to the queue
        output out; // remove item from the queue
        
        input aggrIn @directIn;
}
