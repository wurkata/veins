//
// Copyright (C) 2004 Telecommunication Networks Group (TKN) at Technische Universitaet Berlin, Germany.
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
// author:      Yavor Edipov (Wurkata)
// part of:
// description:
//

//
// Created by wurkata on 05/03/2020.
//

#ifndef VEINS_FILTER_H
#define VEINS_FILTER_H

#include "veins/veins.h"
#include "veins/modules/application/traci/TraCIDemo11pMessage_m.h"

class Filter : public cSimpleModule {
public:
    Filter();
    virtual ~Filter();
protected:
    int outCAM;

    void initialize();
    void handleMessage(cMessage* msg);
};
#endif //VEINS_FILTER_H
