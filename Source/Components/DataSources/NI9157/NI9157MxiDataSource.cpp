/**
 * @file NI9157MxiDataSource.cpp
 * @brief Source file for class NI9157MxiDataSource
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class NI9157MxiDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157MxiDataSource.h"
#include "AdvancedErrorManagement.h"
#include "ObjectRegistryDatabase.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"
#include "CreateNI9157DeviceOperatorI.h"
#include "NI9157DeviceOperatorDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

#define NULL_PTR(x) NULL

NI9157MxiDataSource::NI9157MxiDataSource() :
        MemoryDataSourceI(),
        MessageI() {

    // TODO Auto-generated constructor stub
    numberOfPacketsInFifo = 0u;
    niDevice = NULL_PTR(NI9157DeviceOperatorTI **);
    runNi = 0u;
    numberOfSignals = 0u;
    signalFlag = NULL_PTR(uint8 *);
    varId = NULL_PTR(uint32 *);
    numberOfElements = NULL_PTR(uint32 *);
    initialPatterns = NULL_PTR(uint64*);
    useInitialPattern = NULL_PTR(uint8*);
    resetInitialPattern = NULL_PTR(uint8*);

    blockIfNotRunning = 0u;
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }

}

NI9157MxiDataSource::~NI9157MxiDataSource() {

    if (niDevice != NULL_PTR(NI9157DeviceOperatorTI **)) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            if (niDevice[i] != NULL_PTR(NI9157DeviceOperatorTI *)) {
                /*lint -e{1551} this will not throw any exception*/
                delete niDevice[i];
            }
        }
        delete[] niDevice;
        niDevice = NULL_PTR(NI9157DeviceOperatorTI **);
    }
    if (varId != NULL_PTR(uint32 *)) {
        delete[] varId;
        varId = NULL_PTR(uint32 *);
    }
    if (signalFlag != NULL_PTR(uint8 *)) {
        delete[] signalFlag;
        signalFlag = NULL_PTR(uint8 *);
    }
    if (numberOfElements != NULL_PTR(uint32 *)) {
        delete[] numberOfElements;
        numberOfElements = NULL_PTR(uint32 *);
    }
    if (initialPatterns != NULL_PTR(uint64*)) {
        delete[] initialPatterns;
    }
    if (useInitialPattern != NULL_PTR(uint64*)) {
        delete[] useInitialPattern;
    }
    if (resetInitialPattern != NULL_PTR(uint64*)) {
        delete[] resetInitialPattern;
    }
    //REPORT_ERROR(ErrorManagement::Information, "MXI closed");
}

bool NI9157MxiDataSource::Initialise(StructuredDataI & data) {

    bool ret = MemoryDataSourceI::Initialise(data);

    if (ret) {
        StreamString devicePath;
        ret = data.Read("NI9157DevicePath", devicePath);
        if (ret) {
            niDeviceBoard = ObjectRegistryDatabase::Instance()->Find(devicePath.Buffer());
            ret = niDeviceBoard.IsValid();
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Cannot find the the NI-Dev at path %s", devicePath.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify the NI-Dev path (NI9157DevicePath)");
        }
    }
    if (ret) {
        if (!data.Read("NumberOfPacketsInFifo", numberOfPacketsInFifo)) {
            numberOfPacketsInFifo = 5u;
        }
        if (!data.Read("RunNi", runNi)) {
            runNi = 0u;
        }
    }
    if (ret) {
        if (!data.Read("BlockIfNotRunning", blockIfNotRunning)) {
            blockIfNotRunning = 0u;
        }
    }

    return ret;
}

bool NI9157MxiDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        niDevice = new NI9157DeviceOperatorTI*[numberOfSignals];
        varId = new uint32[numberOfSignals];
        numberOfElements = new uint32[numberOfSignals];
        signalFlag = new uint8[numberOfSignals];

        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            niDevice[i] = NULL_PTR(NI9157DeviceOperatorTI *);
        }
        initialPatterns = new uint64[numberOfSignals];
        useInitialPattern = new uint8[numberOfSignals];
        resetInitialPattern = new uint8[numberOfSignals];

        //Look for all the "PacketMemberSizes". Each signal is potentially a packet
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            initialPatterns[i] = 0;
            useInitialPattern[i] = 0u;
            resetInitialPattern[i] = 0u;

            StreamString signalName;
            ret = GetSignalName(i, signalName);
            bool found = false;
            if (ret) {
                found = signalsDatabase.MoveAbsolute("Signals");
                if (found) {
                    found = signalsDatabase.MoveRelative(signalName.Buffer());
                }
            }
            if (ret && found) {
                //read the packet config
                if (signalsDatabase.Read("InitialPattern", initialPatterns[i])) {
                    useInitialPattern[i] = 1u;
                    resetInitialPattern[i] = useInitialPattern[i];
                }
            }
        }

        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            signalFlag[i] = 0u;
            StreamString varName;
            ret = GetSignalName(i, varName);

            if (ret) {
                //treat the special case of bool
                CreateNI9157DeviceOperatorI *creator = NULL_PTR(CreateNI9157DeviceOperatorI *);
                const char8 *typeInName = StringHelper::SearchString(varName.Buffer(), "Bool");
                if (typeInName != NULL) {
                    uint32 begIndex = 5u;
                    varName = &typeInName[begIndex];
                    creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator("NI9157DeviceBool");
                }
                else {
                    TypeDescriptor td = GetSignalType(i);
                    creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(td);
                }
                ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
                if (ret) {
                    niDevice[i] = creator->Create(niDeviceBoard);
                    ret = (niDevice[i] != NULL_PTR(NI9157DeviceOperatorTI *));
                    if (!ret) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed the creation of the NI9157DeviceOperatorTI for the signal %s",
                                                varName.Buffer());
                    }
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Cannot find the NI9157DeviceOperatorTI for the signal %s", varName.Buffer());
                }
            }
            if (ret) {
                NiFpga_Status status = niDevice[i]->FindResource(varName.Buffer(), varId[i]);
                ret = (status == 0);
                if (!ret) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Cannot find the exported Labview variable %s, status=%d", varName.Buffer(), status);
                }
            }
        }
    }

    return ret;
}

/*lint -e{715} currentStateName and nextStateName are not referenced*/
bool NI9157MxiDataSource::PrepareNextState(const char8 * const currentStateName,
                                           const char8 * const nextStateName) {
    uint32 numberOfReadWriteCurrent = 0u;
    uint32 numberOfReadWriteNext = 0u;

    for (uint32 i = 0u; i < numberOfSignals; i++) {
        uint32 numberOfProducersCurrentState;
        uint32 numberOfProducersNextState;
        if (!GetSignalNumberOfProducers(i, currentStateName, numberOfProducersCurrentState)) {
            numberOfProducersCurrentState = 0u;
        }
        if (!GetSignalNumberOfProducers(i, nextStateName, numberOfProducersNextState)) {
            numberOfProducersNextState = 0u;
        }
        numberOfReadWriteCurrent += numberOfProducersCurrentState;
        numberOfReadWriteNext += numberOfProducersNextState;
        uint32 numberOfConsumersCurrentState;
        uint32 numberOfConsumersNextState;
        if (!GetSignalNumberOfConsumers(i, currentStateName, numberOfConsumersNextState)) {
            numberOfConsumersCurrentState = 0u;
        }
        if (!GetSignalNumberOfConsumers(i, nextStateName, numberOfConsumersNextState)) {
            numberOfConsumersNextState = 0u;
        }
        numberOfReadWriteCurrent += numberOfConsumersCurrentState;
        numberOfReadWriteNext += numberOfConsumersNextState;
    }

    bool ret = true;

    if (numberOfReadWriteNext > 0u && numberOfReadWriteCurrent == 0u) {
        NiFpga_Status status;
        for (uint32 i = 0u; (i < numberOfSignals); i++) {

            //get the type and create the device accordingly
            /*lint -e{613} NULL pointer checked*/
            signalFlag[i] = 0u;
            uint32 numberOfConsumers = 0u;
            if (!GetSignalNumberOfConsumers(i, nextStateName, numberOfConsumers)) {
                numberOfConsumers = 0u;
            }

            if (numberOfConsumers > 0u) {
                /*lint -e{613} NULL pointer checked*/
                signalFlag[i] |= 1u;
            }
            uint32 numberOfProducers = 0u;
            if (!GetSignalNumberOfProducers(i, nextStateName, numberOfProducers)) {
                numberOfProducers = 0u;
            }
            if (numberOfProducers > 0u) {
                /*lint -e{613} NULL pointer checked*/
                signalFlag[i] |= 2u;
            }

            uint8 numberOfDimensions = 0u;
            ret = GetSignalNumberOfDimensions(i, numberOfDimensions);
            if (ret) {
                /*lint -e{613} NULL pointer checked*/
                if (numberOfDimensions > 0u) {
                    /*lint -e{613} NULL pointer checked*/
                    signalFlag[i] |= 4u;
                }
            }

            if (ret) {
                ret = GetSignalNumberOfElements(i, numberOfElements[i]);
                if (ret) {
                    uint8 numberOfDimensions = 0u;
                    ret = GetSignalNumberOfDimensions(i, numberOfDimensions);
                    if (ret) {
                        if (numberOfDimensions > 0u) {
                            //need to configure the number of elements in the fifo
                            uint32 oldSize = 0u;
                            /*lint -e{613} NULL pointer checked*/
                            uint32 fifoSize = (numberOfElements[i] * numberOfPacketsInFifo);
                            NiFpga_Status status = niDeviceBoard->NiConfigureFifo(varId[i], fifoSize, oldSize);
                            ret = (status == 0);
                            if (!ret) {
                                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed to configure host FIFO size to %d, status=%d", fifoSize, status);
                            }
                        }
                    }
                }
            }

        }
        if (ret) {
            for (uint32 i = 0u; i < numberOfSignals; i++) {
                useInitialPattern[i] = resetInitialPattern[i];
            }
            if (runNi == 1u) {
                REPORT_ERROR(ErrorManagement::Information, "Running...");
                status = niDeviceBoard->Run();
                ret = (status == 0);
                if (!ret) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed to Run the application, status=%d", status);
                }
            }
        }
    }
    REPORT_ERROR(ErrorManagement::FatalError, "NI9157MxiDataSource::Prepared");

    return ret;
}

const char8* NI9157MxiDataSource::GetBrokerName(StructuredDataI& data,
                                                const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == OutputSignals) {
        uint32 trigger = 0u;
        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }
        float32 freq = -1.F;
        if (!data.Read("Frequency", freq)) {
            freq = -1.F;
        }

        if ((trigger > 0u) || (freq >= 0.F)) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    else {
        if (direction == InputSignals) {

            uint32 trigger = 0u;
            if (!data.Read("Trigger", trigger)) {
                trigger = 0u;
            }

            float32 freq = -1.F;
            if (!data.Read("Frequency", freq)) {
                freq = -1.F;
            }

            if ((trigger > 0u) || (freq >= 0.F)) {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
            else {
                brokerName = "MemoryMapInputBroker";
            }
        }
    }

    return brokerName;
}

bool NI9157MxiDataSource::Synchronise() {
    //read or write on crio when calling this
    bool ret = true;
    
    if (blockIfNotRunning > 0u) {
        while (!niDeviceBoard->IsRunning()) {
            Sleep::MSec(10u);
        }
    }

    for (uint32 i = 0u; (i < numberOfSignals) && (ret) && (niDeviceBoard->IsRunning()); i++) {
        /*lint -e{613} NULL pointer checked*/
        if ((signalFlag[i] & 0x1u) != 0u) {
            /*lint -e{613} NULL pointer checked*/
            if ((signalFlag[i] & 0x4u) != 0u) {
                uint32 elementsRemaining = 0u;
                if (useInitialPattern[i] > 0u) {
                    uint64 pattern;
                    int32 status = niDevice[i]->NiReadFifo(varId[i], &pattern, 1, 0xFFFFFFFFu, elementsRemaining);
                    ret = (status == 0);
                    if (ret) {
                        if (niDevice[i]->Compare(reinterpret_cast<uint8*>(&pattern), reinterpret_cast<uint8*>(&initialPatterns[i])) == 0) {
                            useInitialPattern[i] = 0u;
                        }
                    }
                }
                if (useInitialPattern[i] == 0u) {
                /*lint -e{613} NULL pointer checked*/
                int32 status = niDevice[i]->NiReadFifo(varId[i], &memory[signalOffsets[i]], numberOfElements[i], 0xFFFFFFFFu, elementsRemaining);
                ret = (status == 0);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device FIFO read. Status = %d", status);
                    }
                }
            }
            else {
                if (useInitialPattern[i] > 0u) {
                    uint64 pattern;
                    int32 status = niDevice[i]->NiRead(varId[i], &pattern);
                    ret = (status == 0);
                    if (ret) {
                        if (niDevice[i]->Compare(reinterpret_cast<uint8*>(&pattern), reinterpret_cast<uint8*>(&initialPatterns[i])) == 0) {
                            useInitialPattern[i] = 0u;
                        }
                    }
                }
                if (useInitialPattern[i] == 0u) {
                    int32 status = niDevice[i]->NiRead(varId[i], &memory[signalOffsets[i]]);
                    ret = (status == 0);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device read. Status = %d", status);
                    }
                }
            }
        }
        if (ret) {
            /*lint -e{613} NULL pointer checked*/
            if ((signalFlag[i] & 0x2u) != 0u) {
                /*lint -e{613} NULL pointer checked*/
                if ((signalFlag[i] & 0x4u) != 0u) {
                    uint32 emptyElementsRemaining = 0u;
                    if (useInitialPattern[i] > 0u) {
                        REPORT_ERROR(ErrorManagement::Information, "sending pattern %0x", initialPatterns[i]);
                        int32 status = niDevice[i]->NiWriteFifo(varId[i], &initialPatterns[i], 1u, 0xFFFFFFFFu, emptyElementsRemaining);
                        ret = (status == 0);
                        if (ret) {
                            useInitialPattern[i] = 0u;
                        }
                    }
                    if (useInitialPattern[i] == 0u) {
                        /*lint -e{613} NULL pointer checked*/
                        /*printf("\n\n");
                        for (uint32 k = 0u; k < numberOfElements[i]; k++) {
                            REPORT_ERROR(ErrorManagement::Information, "sending %0x", *((uint64*) (&memory[signalOffsets[i]]) + k));
                        }
                        printf("\n\n");*/
                        
                        int32 status = niDevice[i]->NiWriteFifo(varId[i], &memory[signalOffsets[i]], numberOfElements[i], 0xFFFFFFFFu, emptyElementsRemaining);
                        ret = (status == 0);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device FIFO write. Status = %d", status);
                        }
                    }
                    //REPORT_ERROR(ErrorManagement::Information, "elements remaining = %d", emptyElementsRemaining);
                }
                else {
                    if (useInitialPattern[i] > 0u) {
                        int32 status = niDevice[i]->NiWrite(varId[i], &initialPatterns[i]);
                        ret = (status == 0);
                        if (ret) {
                            useInitialPattern[i] = 0;
                        }
                    }
                    if (useInitialPattern[i] == 0u) {
                        /*lint -e{613} NULL pointer checked*/
                        int32 status = niDevice[i]->NiWrite(varId[i], &memory[signalOffsets[i]]);
                        ret = (status == 0);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device write. Status = %d", status);
                        }
                    }
                }
            }
        }
    }

    return true;
}

ErrorManagement::ErrorType NI9157MxiDataSource::AsyncRead(StreamString varName,
                                                          uint64 &varValue) {
    uint32 i = 0u;
    ErrorManagement::ErrorType ret = GetSignalIndex(i, varName.BufferReference());

    REPORT_ERROR(ErrorManagement::Information, "Reading %s", varName.Buffer());
    if (static_cast<bool>(ret)) {
        /*lint -e{613} NULL pointer checked*/
        NiFpga_Status status = niDevice[i]->NiRead(varId[i], reinterpret_cast<void*>(&varValue));
        ret.fatalError = static_cast<bool>(status != 0);
    }

    return ret;
}

ErrorManagement::ErrorType NI9157MxiDataSource::AsyncWrite(StreamString varName,
                                                           uint64 varValue) {
    uint32 i = 0u;
    ErrorManagement::ErrorType ret = GetSignalIndex(i, varName.BufferReference());

    REPORT_ERROR(ErrorManagement::Information, "Writing %s %!", varName.Buffer(), varValue);
    if (static_cast<bool>(ret)) {
        /*lint -e{613} NULL pointer checked*/
        NiFpga_Status status = niDevice[i]->NiWrite(varId[i], reinterpret_cast<void*>(&varValue));
        ret.fatalError = static_cast<bool>(status != 0);
    }

    return ret;
}

ErrorManagement::ErrorType NI9157MxiDataSource::Reset() {
    NiFpga_Status status;
    ErrorManagement::ErrorType err;
    REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::Reset");

    for (uint32 i = 0u; (i < numberOfSignals); i++) {
        err = !GetSignalNumberOfElements(i, numberOfElements[i]);
        if (err.ErrorsCleared()) {
            uint8 numberOfDimensions = 0u;
            err = !GetSignalNumberOfDimensions(i, numberOfDimensions);
            if (err.ErrorsCleared()) {
                if (numberOfDimensions > 0u) {
                    //need to configure the number of elements in the fifo
                    uint32 oldSize = 0u;
                    /*lint -e{613} NULL pointer checked*/
                    uint32 fifoSize = (numberOfElements[i] * numberOfPacketsInFifo);
                    NiFpga_Status status = niDeviceBoard->NiConfigureFifo(varId[i], fifoSize, oldSize);
                    err = (status != 0);
                    if (!err.ErrorsCleared()) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed to configure host FIFO size to %d, status=%d", fifoSize, status);
                    }
                }
            }
        }
    }

    if (err.ErrorsCleared()) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            useInitialPattern[i] = resetInitialPattern[i];
        }
        if (runNi == 1u) {
            REPORT_ERROR(ErrorManagement::Information, "Running...");
            status = niDeviceBoard->Run();
            err = (status != 0);
            if (!err.ErrorsCleared()) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed to Run the application, status=%d", status);
            }
        }
    }

    return err;
}

CLASS_REGISTER(NI9157MxiDataSource, "1.0")
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(NI9157MxiDataSource, AsyncRead)
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(NI9157MxiDataSource, AsyncWrite)
CLASS_METHOD_REGISTER(NI9157MxiDataSource, Reset)

}
