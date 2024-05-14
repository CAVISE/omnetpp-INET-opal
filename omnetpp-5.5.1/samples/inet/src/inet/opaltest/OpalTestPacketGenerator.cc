//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "OpalTestPacketGenerator.h"
#include "inet/linklayer/common/MacAddressTag_m.h"
#include "inet/linklayer/common/Ieee802SapTag_m.h"
#include "inet/linklayer/common/MacAddress.h"
#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/Ptr.h"
#include "inet/common/ProtocolGroup.h"
#include "inet/common/ProtocolTag_m.h"
#include "inet/physicallayer/ieee80211/mode/Ieee80211ModeSet.h"
#include "inet/common/Simsignals.h"
#include "CAMPacket_m.h"


namespace inet {


Define_Module(OpalTestPacketGenerator);

simsignal_t OpalTestPacketGenerator::mobilityStateChangedSignal = cComponent::registerSignal("mobilityStateChanged");
OpalTestPacketGenerator::OpalTestPacketGenerator() :    generationTimer(nullptr), beaconFrequency(0),transmitWhenPositionChanges(false), vt(nullptr), info(nullptr)
{
}
OpalTestPacketGenerator::~OpalTestPacketGenerator() {
    if (generationTimer!=nullptr) {
        cancelAndDelete(generationTimer);
    }
}
void OpalTestPacketGenerator::initialize()
{

    lowerLayerOutId=findGate("lowerLayerOut");
    lowerLayerInId =findGate("lowerLayerIn");
    beaconFrequency= par("beaconFrequency");
    packetBytes= par("packetBytes");
    generationTimer = new cMessage("New packet");
    vt=check_and_cast<VehicleTable*>(getParentModule()->getSubmodule("vehicleTable"));
    info = new VehicleInfo(-1);
    transmitWhenPositionChanges = par("transmitWhenPositionChanges");
    EV_INFO<<simTime()<<" beaconFrequency="<<beaconFrequency<<endl;
    if (transmitWhenPositionChanges) {
        getParentModule()->subscribe(mobilityStateChangedSignal,this);

    } else {
        if (beaconFrequency>0) { //If we do not want a vehicle to transmit, just set a zero or negative frequency
            //Randomize init
            scheduleAt(simTime() + uniform(0,0.002) + 1.0/beaconFrequency,generationTimer);
        }
    }
    id=getParentModule()->par("id");

    const physicallayer::Ieee80211ModeSet* modeSet= physicallayer::Ieee80211ModeSet::getModeSet("p");
               emit(modesetChangedSignal, modeSet);

}

void OpalTestPacketGenerator::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage()) {

        processGenerationTime();
    } else {
        //Must be from lowerLayer
        handleCAM(msg);

        std::cout<<simTime()<<":"<<id<<":Received CAM"<< endl;
        delete msg;
    }
}

void OpalTestPacketGenerator::processGenerationTime() {
    if (beaconFrequency>0) {
        scheduleAt(simTime() + 1.0/beaconFrequency,generationTimer);
    }
    //auto data = makeShared<ByteCountChunk>(B(packetBytes));
    // Packet* cam = new Packet("cam", data);
    auto data = makeShared<opaltest::CAMPacket>();
    data->setSource(id);
    data->setCreationTime(simTime());
    data->setChunkLength(B(packetBytes));
    Packet* cam = new Packet("cam", data);
    sendDown(cam);

}
void OpalTestPacketGenerator::sendDown(Packet* p) {
    //Add SAP. I think we may remove this
    p->addTagIfAbsent<Ieee802SapReq>()->setDsap(SapCode::SAP_IP);
    p->addTagIfAbsent<MacAddressReq>()->setDestAddress(MacAddress::BROADCAST_ADDRESS);
    //Should put something sensible here. Keep this to prevent LlcEpd from complaining
    p->addTagIfAbsent<PacketProtocolTag>()->setProtocol(&Protocol::ipv4);


    send(p,lowerLayerOutId);
}

void OpalTestPacketGenerator::handleCAM(cMessage* msg) {
    EV_INFO<<id<<":Received CAM"<< endl;
    Packet* pkt=static_cast<Packet*>(msg);
    auto data=pkt->peekAtFront<opaltest::CAMPacket>(B(packetBytes));
    int sid=data->getSource();
    info->id=sid;
    vt->insertOrUpdate(info);


}

void OpalTestPacketGenerator::receiveSignal(cComponent* source,
        simsignal_t signal, cObject* value, cObject* details) {
    if (signal==mobilityStateChangedSignal) {
        if (beaconFrequency>0) {
            //MobilityBase* mob=check_and_cast<MobilityBase*>(value);
            Enter_Method_Silent();
            auto data = makeShared<ByteCountChunk>(B(packetBytes));
            Packet* cam = new Packet("cam", data);
            sendDown(cam);
        }
    }
}

} //namespace

