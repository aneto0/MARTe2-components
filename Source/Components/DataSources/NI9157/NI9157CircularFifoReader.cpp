/**
 * @file NI9157CircularFifoReader.cpp
 * @brief Source file for class NI9157CircularFifoReader.
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
 * the class NI9157CircularFifoReader (public, protected, and private). Be 
 * aware that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157CircularFifoReader.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

NI9157CircularFifoReader::NI9157CircularFifoReader() :
        CircularBufferThreadInputDataSource(), MessageI() {

    checkFrame = 0u;
    nFrameForSync = 1u;
    sampleByteSize = 0u;
    niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    fifoDev = 0u;
    totalReadSize = 0u;
    middleBuffer = NULL_PTR(uint8 *);
    runNi = 0u;
    numberOfPacketsInFIFO = 10u;
    acqTimeout = 0xFFFFFFFFu;
    nonBlockSleepT = 0.F;
    if (eventSem.Create()) {
        REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::NI9157CircularFifoReader EventSem successfully created");
    }
    ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT<RegisteredMethodsMessageFilter> (GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType err = MessageI::InstallMessageFilter(filter);
    bool ret = err.ErrorsCleared();
    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::NI9157CircularFifoReader Install Message Filter returned %s", ret ? "true" : "false");
}

/*lint -e{1551} destructor needs to delete the devices*/
NI9157CircularFifoReader::~NI9157CircularFifoReader() {
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }
    if (middleBuffer != NULL_PTR(uint8 *)) {
        delete[] middleBuffer;
        middleBuffer = NULL_PTR(uint8 *);
    }
}

bool NI9157CircularFifoReader::Synchronise() {

    bool ret;
    ret = CircularBufferThreadInputDataSource::Synchronise();
    if (ret) {
        if (stop > 0) {
            ret = eventSem.ResetWait(TTInfiniteWait);
        }
    }

    return ret;
}

bool NI9157CircularFifoReader::Initialise(StructuredDataI &data) {

    bool ret;
    REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::Initialise");

    ret = CircularBufferThreadInputDataSource::Initialise(data);
    if (ret) {
        ret = data.Read("FifoName", fifoName);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "NI9157CircularFifoReader::Initialise - Could not find the FifoName parameter");
        }
    }
    if (ret) {
        StreamString devicePath;
        ret = data.Read("NI9157DevicePath", devicePath);
        if (ret) {
            niDeviceBoard = ObjectRegistryDatabase::Instance()->Find(devicePath.Buffer());
            ret = niDeviceBoard.IsValid();
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "NI9157CircularFifoReader::Initialise - Could not find the NI9157DevicePath at %s", devicePath.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "NI9157CircularFifoReader::Initialise - Could not find the NI9157DevicePath parameter");
        }
    }
    if (ret) {
        if (!data.Read("RunNi", runNi)) {
            runNi = 0u;
        }
        runNiOriginal = runNi;
        if (!data.Read("NumberOfPacketsInFIFO", numberOfPacketsInFIFO)) {
            numberOfPacketsInFIFO = 10u;
        }
        if (!data.Read("CheckFrame", checkFrame)) {
            checkFrame = 0u;
        }
        if (!data.Read("Timeout", acqTimeout)) {
            acqTimeout = 0xFFFFFFFFu;
        }
        if (!data.Read("NonBlockSleepT", nonBlockSleepT)) {
            acqTimeout = 0xFFFFFFFFu;
        }
        if (checkFrame > 0u) {
            ret = false;
            /*lint -e{9113} -e{9131} known dependence*/
            for (uint32 i = 0u; i < Size() && (!ret); i++) {
                checker = Get(i);
                ret = (checker.IsValid());
            }
            if (ret) {
                nFrameForSync = checker->GetNumberOfFramesToSync();
            }
            REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::Initialise CheckFrame=1 and SampleChecker is returing %s", ret ? "true" : "false");
        }
    }

    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::Initialise returning %s", ret ? "true" : "false");
    return ret;
}

bool NI9157CircularFifoReader::SetConfiguredDatabase(StructuredDataI & data) {

    bool ret;
    REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::SetConfiguredDatabase");

    ret = CircularBufferThreadInputDataSource::SetConfiguredDatabase(data);
    if (ret) {
        ret = (numberOfChannels == 1u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "NI9157CircularFifoReader::SetConfiguredDatabase At least one signal must be defined (besides the InternalTimeStamp and ErrorCheck signals)");
        }
    }
    //check that the signals byte size is multiple of sampleByteSize
    if (ret) {
        totalReadSize = 0u;
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            if ((i != timeStampSignalIndex) && (i != errorCheckSignalIndex)) {
                TypeDescriptor td = GetSignalType(i);
                CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(td);
                ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
                REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::SetConfiguredDatabase GetCreateNI9157DeviceOperator returned %s", ret ? "true" : "false");
                if (ret) {
                    niDeviceOperator = creator->Create(niDeviceBoard);
                    ret = (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *));
                    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::SetConfiguredDatabase Create NiDevice operator returned %s", ret ? "true" : "false");
                    if (ret) {
                        sampleByteSize = niDeviceOperator->GetByteSize();
                        uint32 signalByteSize = 0u;
                        ret = GetSignalByteSize(i, signalByteSize);
                        if (ret) {
                            totalReadSize = signalByteSize;
                            uint32 size = (totalReadSize * nFrameForSync);
                            middleBuffer = new uint8[size];
                            break;
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        /*lint -e{613} NULL pointer checked.*/
        NiFpga_Status status = niDeviceOperator->FindResource(fifoName.Buffer(), fifoDev);
        ret = (status == 0);
        REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::SetConfiguredDatabase FindResource %s returned %s with status %d", fifoName.Buffer(), ret ? "true" : "false", static_cast<int32>(status));
    }

    REPORT_ERROR_PARAMETERS(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::SetConfiguredDatabase returning %s", ret ? "true" : "false");
    return ret;
}

bool NI9157CircularFifoReader::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {

    bool prepare;
    bool ret = true;
    uint32 numberOfProducersCurrentState;
    uint32 numberOfProducersNextState;
    uint32 numberOfConsumersCurrentState;
    uint32 numberOfConsumersNextState;
    uint32 numberOfReadWriteCurrent = 0u;
    uint32 numberOfReadWriteNext = 0u;
    uint32 oldSize = 0u;
    uint8 numberOfAttempts = 2u;
    ErrorManagement::ErrorType err;
    REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::PrepareNextState");

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
    prepare = ((numberOfReadWriteNext > 0u) && (numberOfReadWriteCurrent == 0u)) ||
            ((numberOfReadWriteNext == numberOfReadWriteCurrent) && (numberOfReadWriteCurrent > 0u));
    if (prepare){
        ret = eventSem.Post();
        for (uint32 i = 0u; (i < numberOfSignals); i++) {
            currentBuffer[i] = 0u;
            lastReadBuffer[i] = (numberOfBuffers - 1u);
            lastReadBuffer_1[i] = 0u;
            uint32 nTotalElements = numberOfBuffers * numberOfSignals;
            (void) MemoryOperationsHelper::Set(&isRefreshed[0], '\0', nTotalElements);
        }
        /*lint -e{414} division by 0 checked*/
        uint32 hostFifoSize = (totalReadSize * numberOfPacketsInFIFO) / sampleByteSize;
        NiFpga_Status status = niDeviceBoard->NiConfigureFifo(fifoDev, hostFifoSize, oldSize);
        if (ret) {
            ret = (status == 0);
        }        
        if (ret) {
            uint32 maxSize = (totalReadSize * nFrameForSync);
            (void) MemoryOperationsHelper::Set(middleBuffer, '\0', maxSize);
            stop = 0;
            ret = CircularBufferThreadInputDataSource::PrepareNextState(currentStateName, nextStateName);
        }
    }
    if (numberOfReadWriteNext == 0u) {
        if (executor.GetStatus() != EmbeddedThreadI::OffState) {
            for (uint32 i = 0u; i < numberOfAttempts; i++){
                REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::PrepareNextState Stopping attempt %u of %u", i+1u, numberOfAttempts);
                err = (!executor.Stop());
                if (ret) {
                    ret = err.ErrorsCleared();
                }
                if (ret) {
                    break;
                }
            }
        }
    }

    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::PrepareNextState returning %s with prepare %s", ret ? "true" : "false", prepare ? "true" : "false");
    return ret;
}

/*lint -e{715} symbol not referenced*/
bool NI9157CircularFifoReader::DriverRead(char8 * const bufferToFill, uint32 &sizeToRead, const uint32 signalIdx) {

    bool ret = true;
    bool writeMemory = false;
    //REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::DriverRead");

    if (runNi == 1u) {
        REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::DriverRead Calling Run on the NiDevice");
        NiFpga_Status status = niDeviceBoard->Run();
        ret = (status == 0);
        runNi = 0u;
        REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::DriverRead Run call returned %s with status %d", ret ? "true" : "false" , static_cast<int32>(status));
    }
    if (ret) {
        uint32 fifoRemaining = 0u;
        /*lint -e{613} -e{414} NULL pointer is checked and division by 0 checked*/
        NiFpga_Status status = niDeviceOperator->NiReadFifo(fifoDev, reinterpret_cast<uint8*> (&(middleBuffer[0])),
                                (sizeToRead / sampleByteSize), acqTimeout, fifoRemaining);
        if (!IsEqual(nonBlockSleepT, 0.F)) {
            while(status == NiFpga_Status_FifoTimeout){
                Sleep::Sec(nonBlockSleepT);
                /*lint -e{613} -e{414} NULL pointer is checked and division by 0 checked*/
                status = niDeviceOperator->NiReadFifo(fifoDev, reinterpret_cast<uint8*> (&(middleBuffer[0])),
                                (sizeToRead / sampleByteSize), acqTimeout, fifoRemaining);
            }
        }
        ret = (status == 0);
        if (status == NiFpga_Status_FifoTimeout) {
            if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                uint32 index1 = (currentBuffer[errorCheckSignalIndex]);
                uint32 errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1) * static_cast<uint32> (sizeof(uint32))));
                /*lint -e{340} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
                *reinterpret_cast<uint32*> (&(memory[errorMemIndex])) |= 4u;
            }
        }
    }
    //check the frame
    if ((checkFrame > 0u) && (ret)) {
        //check the frame
        /*lint -e{613} NULL pointer checked*/
        bool ok = checker->Check(reinterpret_cast<uint8*> (&(middleBuffer[0])), writeMemory);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "NI9157CircularFifoReader::DriverRead Checker returned false (failed to synchronise)");
            //if the check fails
            if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                uint32 index1 = (currentBuffer[errorCheckSignalIndex]);
                uint32 errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1) * static_cast<uint32> (sizeof(uint32))));
                /*lint -e{340} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
                *reinterpret_cast<uint32*> (&(memory[errorMemIndex])) |= 8u;
            }
            uint32 idx = 0u;
            while ((!ok) && (ret)) {
                //re-sync procedure: read N other frames
                for (uint32 i = 1u; (i < nFrameForSync) && (ret); i++) {
                    uint32 fifoRemaining = 0u;
                    uint32 index = (sizeToRead * i);
                    /*lint -e{613} -e{414} NULL pointer is checked and division by 0 checked*/
                    NiFpga_Status status = niDeviceOperator->NiReadFifo(fifoDev, &middleBuffer[index], (sizeToRead / sampleByteSize), acqTimeout, fifoRemaining);
                    ret = (status == 0);
                    if (status == NiFpga_Status_FifoTimeout) {
                        if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                            uint32 index1 = (currentBuffer[errorCheckSignalIndex]);
                            uint32 errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1) * static_cast<uint32> (sizeof(uint32))));
                            /*lint -e{340} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
                            *reinterpret_cast<uint32*> (&(memory[errorMemIndex])) |= 4u;
                        }
                    }
                }
                if (ret) {
                    //synchronization
                    /*lint -e{613} NULL pointer checked*/
                    ok = checker->Synchronise(reinterpret_cast<uint8*> (&(middleBuffer[0])), sizeToRead, idx, writeMemory);
                    //shift in any case
                    if (idx > 0u) {
                        uint32 sizeToMove = ((sizeToRead * nFrameForSync) - idx);
                        /*lint -e{613} NULL pointer checked*/
                        (void) MemoryOperationsHelper::Move(&(middleBuffer[0]), &(middleBuffer[idx]), sizeToMove);
                        uint32 fifoRemaining = 0u;
                        /*lint -e{613} -e{414} NULL pointer is checked and division by 0 checked*/
                        NiFpga_Status status = niDeviceOperator->NiReadFifo(fifoDev, &middleBuffer[sizeToMove], (idx / sampleByteSize), acqTimeout, fifoRemaining);
                        ret = (status == 0);
                        if (status == NiFpga_Status_FifoTimeout) {
                            if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                                uint32 index1 = (currentBuffer[errorCheckSignalIndex]);
                                uint32 errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1) * static_cast<uint32> (sizeof(uint32))));
                                /*lint -e{340} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
                                *reinterpret_cast<uint32*> (&(memory[errorMemIndex])) |= 4u;
                            }
                        }
                        if (ret) {
                            uint32 index = sizeToRead * (nFrameForSync - 1u);
                            //copy the sync one
                            /*lint -e{613} NULL pointer checked*/
                            (void) MemoryOperationsHelper::Move(&(middleBuffer[0]), &(middleBuffer[index]), sizeToRead);
                        }
                    }

                }
            }
        }
    }
    if (ret) {
        if (writeMemory) {
            /*lint -e{613} NULL pointer checked*/
            (void) MemoryOperationsHelper::Copy(bufferToFill, &(middleBuffer[0]), sizeToRead);
            if (numberOfInterleavedSamples[signalIdx] > 0u) {
                uint32 cnt = 0u;
                for (uint32 x = 0u; x < signalIdx; x++) {
                    cnt += numberOfInterleavedSamples[x];
                }
                if (headerSize[signalIdx] > 0u) {
                    /*lint -e{613} NULL pointer checked*/
                    ret = MemoryOperationsHelper::Copy(bufferToFill, middleBuffer, headerSize[signalIdx]);
                }
                /*lint -e{613} NULL pointer checked*/
                NI9157MemoryOperationsHelper::InterleavedToFlat(&(middleBuffer[headerSize[signalIdx]]),
                                                                reinterpret_cast<uint8*> (&bufferToFill[headerSize[signalIdx]]), cnt,
                                                                &interleavedPacketMemberByteSize[0], interleavedSignalByteSize[signalIdx],
                                                                numberOfInterleavedSignalMembers[signalIdx], numberOfInterleavedSamples[signalIdx]);
            }
        }
        else {
            sizeToRead = 0u;
        }
    }

    return ret;
}

ErrorManagement::ErrorType NI9157CircularFifoReader::StopAcquisition() {

    ErrorManagement::ErrorType err;
    REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::StopAcquisition");
    
    runNi = runNiOriginal;
    Atomic::Increment (&stop);
    uint8 numberOfAttempts = 2u;
    if (executor.GetStatus() != EmbeddedThreadI::OffState) {
        for (uint32 i = 0u; i < numberOfAttempts; i++){
            REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::StopAcquisition Attempt %u of %u", i+1u, numberOfAttempts);
            err = (!executor.Stop());
            if (err.ErrorsCleared()) {
                break;
            }
        }
    }

    bool ret = err.ErrorsCleared();
    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::StopAcquisition returning %s", ret ? "true" : "false");
    return err;
}

ErrorManagement::ErrorType NI9157CircularFifoReader::StartAcquisition() {

    ErrorManagement::ErrorType err;
    REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::StartAcquisition");

    for (uint32 i = 0u; (i < numberOfSignals); i++) {
        nBrokerOpPerSignalCounter[i] = nBrokerOpPerSignal[i];
        currentBuffer[i] = 0u;
        lastReadBuffer[i] = (numberOfBuffers - 1u);
        lastReadBuffer_1[i] = 0u;
        uint32 nTotalElements = numberOfBuffers * numberOfSignals;
        (void) MemoryOperationsHelper::Set(&isRefreshed[0], '\0', nTotalElements);
    }
    uint32 oldSize = 0u;
    /*lint -e{414} division by 0 checked*/
    uint32 hostFifoSize = (totalReadSize * numberOfPacketsInFIFO) / sampleByteSize;
    NiFpga_Status status = niDeviceBoard->NiConfigureFifo(fifoDev, hostFifoSize, oldSize);
    err = (status != 0);
    if (err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::StartAcquisition FIFO configured");
        uint32 maxSize = (totalReadSize * nFrameForSync);
        (void) MemoryOperationsHelper::Set(middleBuffer, '\0', maxSize);
        stop = 0;
    }
    if(err.ErrorsCleared()){
        if (executor.GetStatus() == EmbeddedThreadI::OffState) {
            err = (!executor.Start());
            if(err.ErrorsCleared()){
                REPORT_ERROR(ErrorManagement::Information, "NI9157CircularFifoReader::StartAcquisition Application started");
            }
        }
    }
    if (err.ErrorsCleared()) {
        err = (!eventSem.Post());
    }

    bool ret = err.ErrorsCleared();
    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::FatalError, "NI9157CircularFifoReader::StartAcquisition returning %s", ret ? "true" : "false");
    return err;
}

CLASS_REGISTER(NI9157CircularFifoReader, "1.0")
CLASS_METHOD_REGISTER(NI9157CircularFifoReader, StopAcquisition)
CLASS_METHOD_REGISTER(NI9157CircularFifoReader, StartAcquisition)

}
