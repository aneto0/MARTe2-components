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
        ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
        if (!ret.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
        }
}

UEIAI217DataSource::~UEIAI217DataSource() {

}

bool UEIAI217DataSource::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);
    StreamString DAQmode_;
    if (ok) {
        ok = data.Read("Mode", DAQmode_);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Mode property must be set!");
        }
    }

    if (ok){
        if (DAQmode_ == StreamString("Point-by-Point")){
            DAQmode = POINT_BY_POINT;
        }else if(DAQmode_ == StreamString("RtDMap")){
            DAQmode = RTDMAP;
        }else if(DAQmode_ == StreamString("RtVMap")){
            DAQmode = RTVMAP;
        }else if(DAQmode_ == StreamString("ADMap")){
            DAQmode = ADMAP;
        }else if(DAQmode_ == StreamString("AVMap")){
            DAQmode = AVMAP;
        }else{
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unrecognized DAQ mode!");
        }
        if (ok){
            REPORT_ERROR(ErrorManagement::Information, "UEI DAQ mode set to %s", DAQmode_.Buffer());
        }
        if (DAQmode == ADMAP || DAQmode == AVMAP){
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "DAQ mode not supported!");
        }
    }
    StreamString loopback = StreamString("127.0.0.1");
    StreamString localhost = StreamString("localhost");
    if (ok) {
        ok = data.Read("IP", ip);
        if (ok) {
            if (ip == loopback || ip == localhost){
                REPORT_ERROR(ErrorManagement::Information, "UEI DAQ device set to local");
            }else{
                //ok = (1 != -1);
                if (!ok){
                    REPORT_ERROR(ErrorManagement::ParametersError, "Invalid IP supplied!");    
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "IP of UEI DAQ must be set!");
        }
    }
    return ok;
}

bool UEIAI217DataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetNumberOfSignals() == 2u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly two signals must be defined (Trigger and OutputData)");
        }
    }
    if (ok) {
        ok = (GetSignalType(0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The type of the first signal (Trigger) must be UnsignedInteger8");
        }
    }
    if (ok) {
        ok = (GetSignalType(1u) == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The type of the second signal (OutputData) must be UnsignedInteger32");
        }
    }
    if (ok) {
        uint32 elements;
        ok = GetSignalNumberOfElements(0u, elements);
        if (ok) {
            ok = (elements == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (Trigger) must have only one element");
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
    return "MemoryMapMultiBufferInputBroker";
}

bool UEIAI217DataSource::Synchronise() {
    return true;
}

bool UEIAI217DataSource::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName){
    return true;
}

CLASS_REGISTER(UEIAI217DataSource, "1.0")
}
