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

package inet.opaltest;


import inet.linklayer.contract.IWirelessInterface;
import inet.mobility.contract.IMobility;
import inet.networklayer.common.InterfaceTable;
import inet.common.veneris.OpalLinearPolarizationAntenna;

module OpalNode
{
    parameters:
        @networkNode;
       
        @statistic[x](source="xCoord(mobilityPos(mobility.mobilityStateChanged))";record=vector);
        @statistic[y](source="yCoord(mobilityPos(mobility.mobilityStateChanged))";record=vector);
        @statistic[z](source="zCoord(mobilityPos(mobility.mobilityStateChanged))";record=vector);
        @statistic[receivedPower](source="receptionMinSignalPower(wlan.radio.receptionEnded)";record=vector);
        //To add thess signals is necessary to modify Radio.ned and Radio.h/cc to make them emit these signals
        //@statistic[signalPower](source="receptionMinSignalPower(wlan.radio.signalArrivalStarted)"; record=vector); 
 		//@statistic[sqrDistanceToTx](source="wlan.radio.sqrDistanceToTx";record=vector);
 		//@statistic[sqrDistanceToSignal](source="wlan.radio.sqrDistanceToSignalArrival";record=vector);
        int id;
        double receptionRadius=default(1.0);
        string mobilityType=default("StationaryMobility");
        wlan.mgmt.typename = default("Ieee80211MgmtAdhoc");
        wlan.agent.typename = default("");
        wlan.opMode = "p";
        wlan.radio.channelNumber = 1;
		//wlan.radio.carrierFrequency = 5.9 GHz;
        
        mobility.typename = mobilityType;
        wlan.**.mobilityModule = default(exists(mobility) ? absPath(".mobility") : "");
        //wlan.interfaceTableModule = default("InterfaceTable");
        
        *.interfaceTableModule = default(absPath(".interfaceTable"));
       // wlan.radio.antenna.mobilityModule = default("StationaryMobility");
         wlan.radio.antenna.typename=default("OpalLinearPolarizationAntenna");

    gates:
        input radioIn @directIn;
    submodules:

        interfaceTable: InterfaceTable {
            parameters:
              @display("p=125,240;is=s");
        }
 		mobility: <default("")> like IMobility if typename != "" {
            parameters:
                @display("p=125,160;is=s");
        }
        generator: OpalTestPacketGenerator {
            parameters:
                @display("p=366,422;is=s");
        }
        vehicleTable: VehicleTable {
            @display("p=100,200;is=s");
        }
        // TODO: move wlan interfaces after eth interfaces, but it changes IP address assignment and breaks examples/inet/configurator/complex.ini
        wlan: <default("Ieee80211Interface")> like IWirelessInterface {
            parameters:
                @display("p=375,616,row,150;q=queue");
        }


    connections allowunconnected:

        generator.lowerLayerOut --> { @display("m=s"); } --> wlan.upperLayerIn;
        wlan.upperLayerOut --> { @display("m=s"); } --> generator.lowerLayerIn;
        radioIn --> { @display("m=s"); } --> wlan.radioIn;




}
