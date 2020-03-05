package org.car2x.veins.modules.dataProcessing;

// Interface for application layer modules
moduleinterface IDataStreamProcessor {
    gates:
        input lowerLayerIn; // from network layer
        output lowerLayerOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer

        input upperLayerIn; // from application layer
        output upperLayerOut; // to application layer
        input upperControlIn; // control from application layer
        output upperControlOut; // control to application layer
}

