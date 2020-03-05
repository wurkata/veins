package org.car2x.veins.modules.dataProcessing;

moduleinterface Queue {
    parameters:
        @display("i=block/queue");
        int windowSize @unit("int"); // size of the buffer/window (in number of packets)
    gates:
        input in; // add item to the queue
        output out; // remove item from the queue
}
