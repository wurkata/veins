package org.car2x.veins.modules.dataProcessing;

simple Merge {
    parameters:
        @display("i=block/join");
    gates:
        input in; // add item to the queue
        output out; // remove item from the queue
}
