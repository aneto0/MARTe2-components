/**
 * @file UEIAI217DataSource.cpp
 * @brief Source file for class UEIAI217DataSource
 * @date 07/02/2023
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
 * the class UEIAI217DataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"
#include "UEIAI217DataSource.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UEIAI217DataSource::UEIAI217DataSource() :
    MemoryDataSourceI(),
    MessageI(),
    EmbeddedServiceMethodBinderT<UEIAI217DataSource>(*this, &UEIAI217DataSource::UEIAI217ThreadCallback),
    executor(*this) {
        poll_sleep_period = 100;
        input_map = NULL_PTR(uint32*);
        mapid = 0u;
        hd = 0u;
        ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
        if (!ret.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
        }
}

UEIAI217DataSource::~UEIAI217DataSource() {
/*    if (mapid != 0){
       DqRtDmapStop(hd, mapid);
       DqRtDmapClose(hd, mapid);
    }
    if(hd){
        DqCloseIOM(hd);
    }
    DqCleanUpDAQLib();
*/
}

bool UEIAI217DataSource::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
   if (ok) {
        ok = data.Read("IP", ip);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "IP of UEI DAQ must be set!");
        }
    }
    return ok;
}

bool UEIAI217DataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetNumberOfSignals() == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly one signal must be defined (OutputData)");
        }
    }
    if (ok) {
        ok = (GetSignalType(0u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The type of the first signal (Trigger) must be UnsignedInteger8");
        }
    }
    if (ok) {
        uint32 elements;
        ok = GetSignalNumberOfElements(0u, elements);
//        if (ok) {
//            ok = (elements == 3u);
//        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (Trigger) must have 10 elements");
        }
    }
    if (ok){
        ok = (DqInitDAQLib() == DQ_SUCCESS);
        if (!ok){
           REPORT_ERROR(ErrorManagement::ParametersError, "Unable to init DAQ Lib."); 
        }
    }
    if (ok){
        ok = (DqOpenIOM((char *) "127.0.0.1", 6334, 200, (int *)&hd, NULL) >= 0);
        if(!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to contact IOM");  
        }
    }
    REPORT_ERROR(ErrorManagement::ParametersError, "%d", ok);
    if (ok){
        REPORT_ERROR(ErrorManagement::ParametersError, "%d", ok);
        ok = (DqRtDmapInit(hd, (int *)&mapid, 5.0) >= 0);
        REPORT_ERROR(ErrorManagement::ParametersError, "%d", ok);
        if(!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to initialize DMap");  
        }
    }
    REPORT_ERROR(ErrorManagement::ParametersError, "%d", ok);
    if(ok){
        uint32 chentry;
        for (uint8 i = 0u; i < 10u && ok; i++){
            chentry = (uint32)i | DQ_LNCL_GAIN(DQ_AI217_GAIN_1) | DQ_LNCL_DIFF;
            ok = (DqRtDmapAddChannel((int) hd, (int) mapid, 0u, DQ_SS0IN, &chentry, 1) >=0 );
        }
        if(!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "Error adding channels");  
        }
    }
    if (ok){
        ok = (DqRtDmapStart((int) hd, (int) mapid) >= 0);
        if(!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to start DMap");  
        }
    }
    REPORT_ERROR(ErrorManagement::ParametersError, "%d", ok);
    if (ok){
        ok = GetMapAddr();
        if(!ok){
            REPORT_ERROR(ErrorManagement::ParametersError, "Unable to get DMap memory address");  
        }
    }
    return ok;
}

ErrorManagement::ErrorType UEIAI217DataSource::UEIAI217ThreadCallback(ExecutionInfo &info) {
    ErrorManagement::ErrorType err;
    REPORT_ERROR(ErrorManagement::Warning, "Threading!");
    return err;
}

void UEIAI217DataSource::PrepareInputOffsets() {
}

bool UEIAI217DataSource::GetInputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset) {

    return true;
}

bool UEIAI217DataSource::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples){
    return true;
}

const char8* UEIAI217DataSource::GetBrokerName(StructuredDataI &data,
        const SignalDirection direction) {
    return "MemoryMapSynchronisedInputBroker";
}

bool UEIAI217DataSource::GetMapAddr(){
    unsigned char* map_pointer = NULL;
    uint16 counter = 0u;
    bool ok = true;
    while((((uint32*) map_pointer) == NULL)&& ok){
        counter++;
        ok = (DqRtDmapRefresh(hd, mapid) >= 0);
        if (ok){
            Sleep::MSec(10);
            ok = (DqRtDmapGetInputMap(hd, mapid, 0, &(map_pointer)) >=0);
            if (!ok){
                REPORT_ERROR(ErrorManagement::ParametersError, "GetInputMap failed during the GetMapAddr method");   
            }
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "Refresh failed during the GetMapAddr method");
        }
        if (counter > 200) break; //If NULL map is still present during 2s abort the initialisation of the DS
    }
    if (((uint32*) map_pointer) != NULL){
        input_map = ((uint32*)map_pointer);
        return true;
    }else{
        return false;
    }
}

bool UEIAI217DataSource::Synchronise() {
    bool ok = PollForNextPacket();
    return ok;
}

bool UEIAI217DataSource::PollForNextPacket(){
    bool next_packet = false;
    bool ok = true;
    while(!next_packet && ok){
        //Poll for next packet from UEIDAQ
        ok = (DqRtDmapRefresh(hd, mapid) >= 0);
        if (ok){
            //Check if the response is a new packet or a rerequest.
            //The 0x80000000 bit on the recived samples lets us know if the packet has
            //been previously requested.
            if (*((uint32*)input_map) & 0x80000000){ 
                uint32 mask = 0x00FFFFFF; 
                //The recived packet is a newly converted one not requested yet.
                //Make the signals available to the broker.
		uint32* const destination = reinterpret_cast<uint32*>(memory);
                for (uint8 i = 0; i < 3u; i++){
                    destination[i] = ((uint32*)input_map)[i] & mask;
                }
                //End the while loop to unlock the Synchronize method.
                next_packet = true;
            }else{
                //The packet recived is a re-request, sleep the RT-thread to avoid starving other threads
                Sleep::MSec(poll_sleep_period);
            }
        }else{
            REPORT_ERROR(ErrorManagement::ParametersError, "Refresh failed during Poll for conversion");
        }
    }
    return ok;
}

bool UEIAI217DataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    return true;
}

CLASS_REGISTER(UEIAI217DataSource, "1.0")
}
