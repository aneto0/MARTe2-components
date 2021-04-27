/**
 * @file NI9157MxiDataSource.cpp
 * @brief Source file for class NI9157MxiDataSource.
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
 * the class NI9157MxiDataSource (public, protected, and private). Be aware
 * that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157MxiDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

NI9157MxiDataSource::NI9157MxiDataSource() :
        MemoryDataSourceI(),
        MessageI() {

    blockIfNotRunning = 0u;
    numberOfPacketsInFifo = 0u;
    niDevice = NULL_PTR(NI9157DeviceOperatorTI **);
    runNi = 0u;
    numberOfSignals = 0u;
    signalFlag = NULL_PTR(uint8 *);
    varId = NULL_PTR(uint32 *);
    numberOfElements = NULL_PTR(uint32 *);
    initialPatterns = NULL_PTR(uint64 *);
    useInitialPattern = NULL_PTR(uint8 *);
    resetInitialPattern = NULL_PTR(uint8 *);

    //Install message filter
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType err = MessageI::InstallMessageFilter(filter);
    bool ret = err.ErrorsCleared();
    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::NI9157MxiDataSource InstallMessageFilter() returned %s", ret ? "true" : "false");
}

/*lint -e{1551} possible thrown exception non critical*/
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
    if (initialPatterns != NULL_PTR(uint64 *)) {
        delete[] initialPatterns;
    }
    if (useInitialPattern != NULL_PTR(uint8 *)) {
        delete[] useInitialPattern;
    }
    if (resetInitialPattern != NULL_PTR(uint8 *)) {
        delete[] resetInitialPattern;
    }
    REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::~NI9157MxiDataSource MXI Device closed.");
}

bool NI9157MxiDataSource::Initialise(StructuredDataI & data) {

    bool ret;
    StreamString devicePath;
    REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::Initialise");

    ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        ret = data.Read("NI9157DevicePath", devicePath);
        if (ret) {
            niDeviceBoard = ObjectRegistryDatabase::Instance()->Find(devicePath.Buffer());
            ret = niDeviceBoard.IsValid();
            if (!ret) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "NI9157MxiDataSource::Initialise Cannot find the the NI-Dev at path %s", devicePath.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "NI9157MxiDataSource::Initialise Please specify the NI-Dev path (NI9157DevicePath)");
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

    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::Initialise returning %s", ret ? "true" : "false");
    return ret;
}

bool NI9157MxiDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    
    bool ret;
    REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::SetConfiguredDatabase");

    ret = DataSourceI::SetConfiguredDatabase(data);
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
            initialPatterns[i] = 0u;
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
                //treat the special case of bool, and for (i) varBool_<varName> or (ii) <varNameBool>
                CreateNI9157DeviceOperatorI *creator = NULL_PTR(CreateNI9157DeviceOperatorI *);
                const char8 *typeInName = StringHelper::SearchString(varName.Buffer(), "Bool");
                if (typeInName != NULL) {
                    if (StringHelper::SearchString(varName.Buffer(), "Bool_") != NULL) {
                        uint32 begIndex = 5u;
                        varName = &typeInName[begIndex];
                    }
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
                    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "NI9157MxiDataSource::SetConfiguredDatabase Creation of the NI9157DeviceOperatorTI for the signal %s returned ", varName.Buffer(), ret ? "true" : "false");
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "NI9157MxiDataSource::SetConfiguredDatabase Cannot find the NI9157DeviceOperatorTI for the signal %s", varName.Buffer());
                }
            }
            if (ret) {
                NiFpga_Status status = niDevice[i]->FindResource(varName.Buffer(), varId[i]);
                ret = (status == 0);
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "NI9157MxiDataSource::SetConfiguredDatabase FindResource %s returned %s with status=%d", varName.Buffer(), ret ? "true" : "false" , status);
            }
        }
    }

    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::SetConfiguredDatabase returning %s", ret ? "true" : "false");
    return ret;
}

/*lint -e{715} currentStateName and nextStateName are not referenced*/
bool NI9157MxiDataSource::PrepareNextState(const char8 * const currentStateName,
                                            const char8 * const nextStateName) {

    bool prepare;
    bool ret = true;
    uint32 numberOfReadWriteCurrent = 0u;
    uint32 numberOfReadWriteNext = 0u;
    uint32 numberOfProducersCurrentState = 0u;
    uint32 numberOfProducersNextState = 0u;
    uint32 numberOfConsumersCurrentState = 0u;
    uint32 numberOfConsumersNextState = 0u;
    NiFpga_Status status;
    uint32 numberOfConsumers = 0u;
    uint32 numberOfProducers = 0u;
    uint8 numberOfDimensions = 0u;
    uint32 oldSize = 0u;
    uint32 fifoSize = 0u;
    REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::PrepareNextState");

    for (uint32 i = 0u; i < numberOfSignals; i++) {
        if (!GetSignalNumberOfProducers(i, currentStateName, numberOfProducersCurrentState)) {
            numberOfProducersCurrentState = 0u;
        }
        if (!GetSignalNumberOfProducers(i, nextStateName, numberOfProducersNextState)) {
            numberOfProducersNextState = 0u;
        }
        numberOfReadWriteCurrent += numberOfProducersCurrentState;
        numberOfReadWriteNext += numberOfProducersNextState;
        if (!GetSignalNumberOfConsumers(i, currentStateName, numberOfConsumersCurrentState)) {
            numberOfConsumersCurrentState = 0u;
        }
        if (!GetSignalNumberOfConsumers(i, nextStateName, numberOfConsumersNextState)) {
            numberOfConsumersNextState = 0u;
        }
        numberOfReadWriteCurrent += numberOfConsumersCurrentState;
        numberOfReadWriteNext += numberOfConsumersNextState;
    }
    /*lint -e{9113} -e{9131} known dependence*/
    prepare = (numberOfReadWriteNext > 0u && numberOfReadWriteCurrent == 0u) ||
            (numberOfReadWriteNext == numberOfReadWriteCurrent && numberOfReadWriteCurrent > 0u);
    if (prepare) {
        for (uint32 i = 0u; (i < numberOfSignals); i++) {
            //get the type and create the device accordingly
            /*lint -e{613} NULL pointer checked*/
            signalFlag[i] = 0u;
            if (!GetSignalNumberOfConsumers(i, nextStateName, numberOfConsumers)) {
                numberOfConsumers = 0u;
            }
            if (numberOfConsumers > 0u) {
                /*lint -e{613} NULL pointer checked*/
                signalFlag[i] |= 1u;
            }
            if (!GetSignalNumberOfProducers(i, nextStateName, numberOfProducers)) {
                numberOfProducers = 0u;
            }
            if (numberOfProducers > 0u) {
                /*lint -e{613} NULL pointer checked*/
                signalFlag[i] |= 2u;
            }
            ret = GetSignalNumberOfDimensions(i, numberOfDimensions);
            if (ret) {
                /*lint -e{613} NULL pointer checked*/
                if (numberOfDimensions > 0u) {
                    /*lint -e{613} NULL pointer checked*/
                    signalFlag[i] |= 4u;
                }
            }
            if (ret) {
                /*lint -e{613} NULL pointer checked*/
                ret = GetSignalNumberOfElements(i, numberOfElements[i]);
                if (ret) {
                    /*lint -e{613} NULL pointer checked*/
                    ret = GetSignalNumberOfDimensions(i, numberOfDimensions);
                    if (ret) {
                        if (numberOfDimensions > 0u) {
                            /*lint -e{613} NULL pointer checked*/
                            fifoSize = (numberOfElements[i] * numberOfPacketsInFifo);
                            /*lint -e{613} NULL pointer checked*/
                            status = niDeviceBoard->NiConfigureFifo(varId[i], fifoSize, oldSize);
                            ret = (status == 0);
                            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "NI9157MxiDataSource::PrepareNextState NiConfigureFifo to size %d returned %s with status %d", fifoSize, ret ? "true" : "false" , static_cast<int32> (status));
                        }
                    }
                }
            }
        }
        if (ret) {
            /*lint -e{613} NULL pointer checked*/
            for (uint32 i = 0u; i < numberOfSignals; i++) {
                useInitialPattern[i] = resetInitialPattern[i];
            }
            if (runNi == 1u) {
                REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::PrepareNextState Calling Run on NI Device");
                status = niDeviceBoard->Run();
                ret = (status == 0);
                REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::PrepareNextState Run returning %s with status=%d", ret ? "true" : "false", static_cast<int32> (status));
            }
        }
    }

    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::PrepareNextState returning %s with prepare %s", ret ? "true" : "false", prepare ? "true" : "false");
    return ret;
}

const char8* NI9157MxiDataSource::GetBrokerName(StructuredDataI& data,
                                            const SignalDirection direction) {

    const char8 *brokerName = NULL_PTR(const char8 *);
    uint32 trigger = 0u;
    float32 freq = -1.F;
    REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::GetBrokerName");
    /*lint -e{9113} -e{9131} known dependence, non-postfix expression used with logical operator*/
    if ((direction == OutputSignals) || (direction == InputSignals)) {
        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }
        if (!data.Read("Frequency", freq)) {
            freq = -1.F;
        }
        if ((trigger > 0u) || (freq >= 0.F)) {
            if(direction == OutputSignals) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
            else {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
        else{
            if(direction == OutputSignals) {
                brokerName = "MemoryMapOutputBroker";
            }
            else {
                brokerName = "MemoryMapInputBroker";
            }
        }
    }

    return brokerName;
}

bool NI9157MxiDataSource::Synchronise() {

    bool ret = true;
    
    if (blockIfNotRunning > 0u) {
        while (niDeviceBoard->IsRunning() == 0u) {
            Sleep::MSec(10u);
        }
    }
    /*lint -e{9007} accounted for side effects on right hand of logical operator*/
    for (uint32 i = 0u; (i < numberOfSignals) && (ret) && (niDeviceBoard->IsRunning() != 0u); i++) {
        /*lint -e{613} NULL pointer checked*/
        if ((signalFlag[i] & 0x1u) != 0u) {
            /*lint -e{613} NULL pointer checked*/
            if ((signalFlag[i] & 0x4u) != 0u) {
                uint32 elementsRemaining = 0u;
                if (useInitialPattern[i] > 0u) {
                    uint64 pattern;
                    int32 status = niDevice[i]->NiReadFifo(varId[i], &pattern, 1u, 0xFFFFFFFFu, elementsRemaining);
                    ret = (status == 0);
                    if (ret) {
                        /*lint -e{928} known recasting from pointer to pointer*/
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
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device FIFO read. Status = %d", static_cast<int32> (status));
                    }
                }
            }
            else {
                if (useInitialPattern[i] > 0u) {
                    uint64 pattern;
                    int32 status = niDevice[i]->NiRead(varId[i], &pattern);
                    ret = (status == 0);
                    if (ret) {
                        /*lint -e{928} known recasting from pointer to pointer*/
                        if (niDevice[i]->Compare(reinterpret_cast<uint8*>(&pattern), reinterpret_cast<uint8*>(&initialPatterns[i])) == 0) {
                            useInitialPattern[i] = 0u;
                        }
                    }
                }
                if (useInitialPattern[i] == 0u) {
                    int32 status = niDevice[i]->NiRead(varId[i], &memory[signalOffsets[i]]);
                    ret = (status == 0);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device read. Status = %d", static_cast<int32> (status));
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
                        int32 status = niDevice[i]->NiWriteFifo(varId[i], &memory[signalOffsets[i]], numberOfElements[i], 0xFFFFFFFFu, emptyElementsRemaining);
                        ret = (status == 0);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device FIFO write. Status = %d", static_cast<int32> (status));
                        }
                    }
                }
                else {
                    if (useInitialPattern[i] > 0u) {
                        int32 status = niDevice[i]->NiWrite(varId[i], &initialPatterns[i]);
                        ret = (status == 0);
                        if (ret) {
                            useInitialPattern[i] = 0u;
                        }
                    }
                    if (useInitialPattern[i] == 0u) {
                        /*lint -e{613} NULL pointer checked*/
                        int32 status = niDevice[i]->NiWrite(varId[i], &memory[signalOffsets[i]]);
                        ret = (status == 0);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failed Ni9155Device write. Status = %d", static_cast<int32> (status));
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

    ErrorManagement::ErrorType err;
    uint32 i = 0u;
    NiFpga_Status status;
    //REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::AsyncRead");

    if (GetSignalIndex(i, varName.BufferReference())) {
        //REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::AsyncRead Reading %s", varName.Buffer());
        /*lint -e{613} NULL pointer checked*/
        status = niDevice[i]->NiRead(varId[i], reinterpret_cast<void*>(&varValue));
        err.fatalError = static_cast<bool>(status != 0);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "NI9157MxiDataSource::AsyncRead Could not find %s", varName.Buffer());
        err.fatalError = true;
    }

    //bool ret = err.ErrorsCleared();
    //REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::AsyncRead returning %s", ret ? "true" : "false");
    return err;
}

ErrorManagement::ErrorType NI9157MxiDataSource::AsyncWrite(StreamString varName,
                                            uint64 varValue) {

    ErrorManagement::ErrorType err;
    uint32 i = 0u;
    NiFpga_Status status;
    //REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::AsyncWrite");

    if (GetSignalIndex(i, varName.BufferReference())) {
        //REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::AsyncWrite Writing %s %!", varName.Buffer(), varValue);
        /*lint -e{613} NULL pointer checked*/
        status = niDevice[i]->NiWrite(varId[i], reinterpret_cast<void*>(&varValue));
        err.fatalError = static_cast<bool>(status != 0);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not find %s", varName.Buffer());
        err.fatalError = true;
    }

    //bool ret = err.ErrorsCleared();
    //REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::AsyncWrite returning %s", ret ? "true" : "false");
    return err;
}

ErrorManagement::ErrorType NI9157MxiDataSource::Reset() {

    bool ret;
    ErrorManagement::ErrorType err;
    uint8 numberOfDimensions = 0u;
    uint32 oldSize = 0u;
    uint32 fifoSize = 0u;
    NiFpga_Status status;
    REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::Reset");

    for (uint32 i = 0u; (i < numberOfSignals); i++) {
        /*lint -e{613} NULL pointer checked*/
        err = !GetSignalNumberOfElements(i, numberOfElements[i]);
        if (err.ErrorsCleared()) {
            /*lint -e{613} NULL pointer checked*/
            err = !GetSignalNumberOfDimensions(i, numberOfDimensions);
            if (err.ErrorsCleared()) {
                if (numberOfDimensions > 0u) {
                    /*lint -e{613} NULL pointer checked*/
                    fifoSize = (numberOfElements[i] * numberOfPacketsInFifo);
                    /*lint -e{613} NULL pointer checked*/
                    status = niDeviceBoard->NiConfigureFifo(varId[i], fifoSize, oldSize);
                    err = (status != 0);
                    ret = err.ErrorsCleared();
                    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::Reset NiConfigureFifo to size %d returned %s with status %d", fifoSize, ret ? "true" : "false", static_cast<int32> (status));
                }
            }
        }
    }
    if (err.ErrorsCleared()) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            /*lint -e{613} NULL pointer checked*/
            useInitialPattern[i] = resetInitialPattern[i];
        }
        if (runNi == 1u) {
            REPORT_ERROR(ErrorManagement::Information, "NI9157MxiDataSource::Reset Calling Run on NI Device");
            status = niDeviceBoard->Run();
            err = (status != 0);
            ret = err.ErrorsCleared();
            REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::Reset Run returned %s with status %d", ret ? "true" : "false", static_cast<int32> (status));
        }
    }

    ret = err.ErrorsCleared();
    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157MxiDataSource::Reset returning %s", ret ? "true" : "false");
    return err;
}

CLASS_REGISTER(NI9157MxiDataSource, "1.0")
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(NI9157MxiDataSource, AsyncRead)
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(NI9157MxiDataSource, AsyncWrite)
CLASS_METHOD_REGISTER(NI9157MxiDataSource, Reset)

}
