/**
 * @file DAQDevice.cpp
 * @brief Source file for class DAQDevice
 * @date 09/02/2023
 * @author Xavier Ruche
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class DAQDevice (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DAQDevice.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

DAQDevice::~DAQDevice(){
}

bool DAQDevice::Initialise(StructuredDataI &data){ 
    return false;
}

bool DAQDevice::CheckChannelAndDirection(uint32 channelNumber, uint8 direction){
    return false;
}

bool DAQDevice::ConfigureDevice(int32 DAQ_handle){
    return false;
}


uint8 DAQDevice::GetDevN(){
    return 0u;
}

uint16 DAQDevice::GetModel(){
    return 0u;
}
void DAQDevice::SetHardwareCorrespondence(){
}

bool DAQDevice::GetHardwareCorrespondence(){
    return true;
}

void DAQDevice::SetMapAssignment(){
}

bool DAQDevice::GetMapAssignment(){
    return true;
}

uint32 DAQDevice::GetDeviceChannels(){
    return 0u;
}

CLASS_REGISTER(DAQDevice, "1.0")
}