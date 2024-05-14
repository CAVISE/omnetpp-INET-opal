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

package inet.examples.veneris.opal;
import inet.opaltest.OpalNode;
import inet.physicallayer.common.packetlevel.OpalRadioMedium;
import inet.physicallayer.contract.packetlevel.IRadioMedium;
network StreetCrossing
{
    parameters:
        int count;
    submodules:
        //Create a default radioMedium in case we do not use OpalRadioMedium
        radioMedium: <default("OpalRadioMedium")> like IRadioMedium {
            @display("p=49.704147,85.20711");
        }
        node[count]: OpalNode {
            id=index;
        }
}
