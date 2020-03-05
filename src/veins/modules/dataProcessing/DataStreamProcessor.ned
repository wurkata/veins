package org.car2x.veins.modules.dataProcessing;

import org.car2x.veins.modules.nic.Nic80211p;
import org.car2x.veins.base.modules.IBaseApplLayer;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

// Interface for data stream processing module
module DataStreamProcessor like IDataStreamProcessor {
    parameters:
        string applType = default("DemoBaseApplLayer"); //type of the application layer
        string nicType = default("Nic80211p"); // type of network interface card
        
        string camType = default("Queue");
        string denType = default("Queue");
        string codmType = default("Queue");
        
        @display("bgb=360,280");
    gates:
        input lowerLayerIn; // from network layer
        output lowerLayerOut; // to network layer
        input lowerControlIn; // control from network layer
        output lowerControlOut; // control to network layer

        input upperLayerIn; // from application layer
        output upperLayerOut; // to application layer
        input upperControlIn; // control from application layer
        output upperControlOut; // control to application layer

    submodules:
        nic: <nicType> like org.car2x.veins.modules.nic.INic80211p {
            @display("p=-25,120");
        }

        filter: Filter {
            @display("p=61,120");
        }

        camq: <camType> like Queue {
            @display("p=170,40");
        }

        denq: <denType> like Queue {
            @display("p=170,120");
        }

        codmq: <codmType> like Queue {
            @display("p=170,200");
        }

        merge: Merge {
            @display("p=284,120");
        }

        app: <applType> like IBaseApplLayer {
            @display("p=380,120");
        }
    connections:
        nic.upperLayerOut --> filter.in;
        nic.upperControlOut --> filter.in;

        filter.out --> camq.in;
        filter.out --> denq.in;
        filter.out --> codmq.in;

        camq.out --> merge.in;
        denq.out --> merge.in;
        codmq.out --> merge.in;

        merge.out --> app.lowerControlIn;
        merge.out --> app.lowerLayerIn;
}

