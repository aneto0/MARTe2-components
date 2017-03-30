/**
 * @file EpicsInputDataSource.cpp
 * @brief Source file for class EpicsInputDataSource
 * @date 01/12/2016
 * @author Ivan Herrero Molina
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
 * the class EpicsInputDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EpicsInputDataSource.h"

#include "FastPollingMutexSem.h"
#include "HeapManager.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "Platform.h"
#include "RealTimeApplication.h"
#include "SharedDataArea.h"
#include "StringHelper.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

/**
 * @brief Gets the application's name.
 */
static MARTe::StreamString GetApplicationName() {
    MARTe::StreamString result;
    MARTe::ObjectRegistryDatabase *objDb;
    bool found = false;
    objDb = MARTe::ObjectRegistryDatabase::Instance();
    if (objDb != NULL_PTR(MARTe::ObjectRegistryDatabase*)) {
        MARTe::uint32 nOfObjs = objDb->Size();
        for (MARTe::uint32 i = 0u; (i < nOfObjs) && (!found); i++) {
            MARTe::ReferenceT < MARTe::RealTimeApplication > rtApp = objDb->Get(i);
            found = rtApp.IsValid();
            if (found) {
                result = rtApp->GetName();
            }
        }
    }
    if (!found) {
        result = "MARTeApp";
    }
    return result;
}

/**
 * @brief Builds the name for the shared memory area, based on application's
 * name and name passed as parameter.
 */
static MARTe::StreamString BuildSharedMemoryIdentifier(const MARTe::StreamString& name) {
    MARTe::StreamString result;
    //Add the initial mark:
    result += "/";
    //Add the MARTe's application name as prefix:
    result += GetApplicationName();
    //Add a separator mark:
    result += "_";
    //Add the name as last token:
    result += name;
    return result;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

EpicsInputDataSource::EpicsInputDataSource() :
        DataSourceI(),
        consumer(SDA_NULL_PTR(SDA::SharedDataArea::SigblockConsumer*)),
        signals(SDA_NULL_PTR(SDA::Sigblock*)) {
}

EpicsInputDataSource::~EpicsInputDataSource() {
    if (signals != SDA_NULL_PTR(SDA::Sigblock*)) {
        void* mem = reinterpret_cast<void*>(signals);
        /*lint -e{1551} HeapManager::Free does not throw exceptions*/
        (void) HeapManager::Free(mem);
        signals = SDA_NULL_PTR(SDA::Sigblock*);
    }
    if (consumer != SDA_NULL_PTR(SDA::SharedDataArea::SigblockConsumer*)) {
        consumer = SDA_NULL_PTR(SDA::SharedDataArea::SigblockConsumer*);
        /*lint -e{1551} Platform::DestroyShm does not throw exceptions*/
        (void) SDA::Platform::DestroyShm(sharedDataAreaName.Buffer());
    }
}

bool EpicsInputDataSource::Synchronise() {
    bool ok;
    if ((consumer != SDA_NULL_PTR(SDA::SharedDataArea::SigblockConsumer*)) && (signals != SDA_NULL_PTR(SDA::Sigblock*))) {
        ok = consumer->ReadSigblock(*signals);
    }
    else {
        ok = false;
    }
    return ok;
}

bool EpicsInputDataSource::AllocateMemory() {
    bool ret;
    uint32 numSignals = GetNumberOfSignals();
    sharedDataAreaName = BuildSharedMemoryIdentifier(GetName());
    SDA::Signal::Metadata smd_for_init[numSignals]; //sigblock description for initialization;

    //{for all signals in datasource add it to smd_for_init}
    ret = (numSignals > 0u);
    for (uint32 i = 0u; (i < numSignals) && (ret); i++) {
        StreamString signalName;
        uint32 memorySize;
        ret = GetSignalByteSize(i, memorySize);
        if (ret) {
            ret = GetSignalName(i, signalName);
            if (ret) {
                /*lint -e{9132} array's length given by NAME_MAX_LEN*/
                ret = MARTe::StringHelper::CopyN(smd_for_init[i].name, signalName.Buffer(), SDA::Signal::Metadata::NAME_MAX_LEN);
            }
            if (ret) {
                smd_for_init[i].size = memorySize;
            }
        }
    }

    SDA::SharedDataArea sbpm;
    /*lint -e{9132} array's length given by numberOfSignals*/
    ret = SDA::SharedDataArea::BuildSharedDataAreaForMARTe(sbpm, sharedDataAreaName.Buffer(), numSignals, smd_for_init);
    if (ret) {
        consumer = sbpm.GetSigblockConsumerInterface();
        SDA::Sigblock::Metadata* sbmd = consumer->GetSigblockMetadata();
        SDA::size_type totalSize = sbmd->GetTotalSize();
        /*lint -e{9119} -e{712} -e{747} calls to Malloc and Set are protected*/
        if (totalSize <= MAX_UINT32) {
            void* mem = HeapManager::Malloc(totalSize);
            (void) MemoryOperationsHelper::Set(mem, '\0', totalSize);
            signals = static_cast<SDA::Sigblock*>(mem);
        }
        else {
            ret = false;
        }
    }
    return ret;
}

uint32 EpicsInputDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool EpicsInputDataSource::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                 const uint32 bufferIdx,
                                                 void *&signalAddress) {
    bool ok;

    /*lint --e{9007} GetNumberOfMemoryBuffers() has no side effects*/
    ok = ((signalIdx < GetNumberOfSignals()) && (bufferIdx < GetNumberOfMemoryBuffers()));

    if (ok) {
        if (consumer != SDA_NULL_PTR(SDA::SharedDataArea::SigblockConsumer*)) {
            SDA::Sigblock::Metadata* sbmd = consumer->GetSigblockMetadata();
            if ((signals != SDA_NULL_PTR(SDA::Sigblock*)) && (sbmd != SDA_NULL_PTR(SDA::Sigblock::Metadata*))) {
                signalAddress = signals->GetSignalAddress(sbmd->GetSignalOffsetByIndex(signalIdx));
            }
            else {
                ok = false;
            }
        }
        else {
            ok = false;
        }
    }

    return ok;
}

/*lint -e{715} parameter data not used in this implementation*/
const char8 *EpicsInputDataSource::GetBrokerName(StructuredDataI &data,
                                                 const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        brokerName = "MemoryMapSynchronisedInputBroker";
    }
    else {
        brokerName = "";
    }
    return brokerName;
}

bool EpicsInputDataSource::GetInputBrokers(ReferenceContainer &inputBrokers,
                                           const char8* const functionName,
                                           void * const gamMemPtr) {
    ReferenceT < MemoryMapSynchronisedInputBroker > broker("MemoryMapSynchronisedInputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

/*lint -e{715} parameters outputBrokers, functionName, and gamMemPtr not used in this implementation*/
bool EpicsInputDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                            const char8* const functionName,
                                            void * const gamMemPtr) {
    return false;
}

/*lint -e{715} parameters currentStateName and nextStateName not used in this implementation*/
bool EpicsInputDataSource::PrepareNextState(const char8 * const currentStateName,
                                            const char8 * const nextStateName) {
    return true;
}

MARTe::StreamString EpicsInputDataSource::GetSharedDataAreaName() const {
    return sharedDataAreaName;
}

CLASS_REGISTER(EpicsInputDataSource, "1.0")

}
