//
// Copyright (C) 2011 David Eckhoff <eckhoff@cs.fau.de>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

package org.car2x.veins.modules.facilities;

import org.car2x.veins.modules.nic.Nic80211p;
import org.car2x.veins.base.modules.IBaseApplLayer;
import org.car2x.veins.modules.application.ieee80211p.DemoBaseApplLayer;

//
// @author Yavor Edipov (Wurkata)
//
// @see Filter
// @see Merge
// @see Queue
//

// Interface for data stream processing module
module DataStreamProcessor like IDataStreamProcessor {
    parameters:
        // string applType = default("TraCIDemo11p"); //type of the application layer
        // string nicType = default("Nic80211p"); // type of network interface card
        
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
        // nic: <nicType> like org.car2x.veins.modules.nic.INic80211p {
        //     @display("p=-25,120");
        // }

        filter: Filter {
            @display("p=61,120");
        }

        camq: <camType> like IQueue {
            @display("p=170,40");
        }

        denq: <denType> like IQueue {
            @display("p=170,120");
        }

        codmq: <codmType> like IQueue {
            @display("p=170,200");
        }

        merge: Merge {
            @display("p=284,120");
        }

        // app: <applType> like IBaseApplLayer {
        //     @display("p=380,120");
        // }
    connections:
        upperLayerIn --> lowerLayerOut;
        upperControlIn --> lowerControlOut;
        lowerControlIn --> upperControlOut;

        // nic.upperLayerOut --> filter.in;
        // nic.upperControlOut --> filter.in;
        lowerLayerIn --> filter.in;

        filter.outCAM --> camq.in;
        filter.outDEN --> denq.in;
        filter.outCoDM --> codmq.in;

        camq.out --> merge.inCAM;
        denq.out --> merge.inDEN;
        codmq.out --> merge.inCoDM;

        merge.out --> upperLayerOut;

        // merge.out --> app.lowerControlIn;
        // merge.out --> app.lowerLayerIn;
}

