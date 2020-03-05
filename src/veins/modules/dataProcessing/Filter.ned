package org.car2x.veins.modules.dataProcessing;

simple Filter {
    parameters:
        @display("i=block/filter");
    gates:
        input in; // add item to the queue
        output out; // remove item from the queue
}
