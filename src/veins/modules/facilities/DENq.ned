package org.car2x.veins.modules.facilities;

simple DENq {
    parameters:
        double sendInterval @unit("s") = default(3s);  // time until next poll from buffers
        int windowSize = default(100);
    gates:
        input in; // add item to the queue
        output out; // remove item from the queue
}
