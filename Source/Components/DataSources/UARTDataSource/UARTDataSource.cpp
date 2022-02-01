/**
 * @file UARTDataSource.cpp
 * @brief Source file for class UARTDataSource
 * @date 25/06/2018
 * @author Andre' Neto
 * @author Marta Baldris
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
 * the class UARTDataSource (public, protected, and private). Be aware that some
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
#include "HighResolutionTimestampProvider.h"
#include "RegisteredMethodsMessageFilter.h"
#include "UARTDataSource.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

UARTDataSource::UARTDataSource() :
    MemoryDataSourceI(),
    MessageI(),
    EmbeddedServiceMethodBinderT<UARTDataSource>(*this, &UARTDataSource::CRIOThreadCallback),
    executor(*this) {
        lastReadIdx = 0u;
        lastWrittenIdx = 0u;
        serialTimeout = 0u;
        packetByteSize = 0u;
        lastDataCopyHadTimeout = true;
        timeoutToSynchronise = 500000u;
        timeout = 1000u;
        ReferenceT < RegisteredMethodsMessageFilter > filter = ReferenceT < RegisteredMethodsMessageFilter > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
        if (!ret.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
        }
        writeMark = NULL_PTR(bool *);
}

/*lint -e{1551} the destructor must guarantee that the thread and servers are closed.*/
UARTDataSource::~UARTDataSource() {
    if (executor.Stop() != ErrorManagement::NoError) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the executor");
    }
    if (!eventSem.Close()) {
        REPORT_ERROR(ErrorManagement::OSError, "Failed to close EventSem");
    }
    if (writeMark != NULL_PTR(bool *)) {
        delete[] writeMark;
    }

    serial.Close();
}

bool UARTDataSource::Initialise(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::Initialise(data);

    if (ok) {
        ok = eventSem.Create();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::OSError, "Failed to create EventSem");
        }
    }
    if (ok) {
        ok = eventSem.Reset();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::OSError, "Failed to reset EventSem");
        }
    }
    StreamString portName;
    if (ok) {
        ok = data.Read("PortName", portName);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "The port name is set to %s", portName.Buffer());
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The port name property shall be set");
        }
    }
    uint32 baudRate = 0u;
    if (ok) {
        ok = data.Read("BaudRate", baudRate);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "The baud rate is set to %d", baudRate);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The baud rate property shall be set");
        }
    }
    if (ok) {
        if (!data.Read("Timeout", timeout)) {
            timeout = 1000u;
        }
    }
    if (ok) {
        ok = data.Read("SerialTimeout", serialTimeout);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "The serial timeout is set to %d", serialTimeout);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The serial timeout property shall be set");
        }
    }
    if (ok) {
        ok = serial.SetSpeed(baudRate);
    }
    if (ok) {
        ok = serial.Open(portName.Buffer());
    }

    if (ok) {
        if (!data.Read("TimeoutToSynchronise", timeoutToSynchronise)) {
            REPORT_ERROR(ErrorManagement::Information, "TimeoutToSynchronise not specified: set to %d", timeoutToSynchronise);
        }
    }

    if (ok) {
        ok = (Size() < 2u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Number of pluggable timestamp providers can be 0 (Default) or 1 (Customized and specified)");
            REPORT_ERROR(ErrorManagement::ParametersError, "%d providers where specified instead", Size());
        }
    }
    if (ok) {
        if (Size() == 0u) {
            REPORT_ERROR(ErrorManagement::Information, "No timer provider specified. Falling back to HighResolutionTimestampProvider");
            timeProvider = ReferenceT < HighResolutionTimestampProvider > (GlobalObjectsDatabase::Instance()->GetStandardHeap());
            timeProvider->SetName("DefaultHighResolutionTimestampProvider");
        }
        else {
            timeProvider = Get(0u);
        }
    }

    //Do not allow to add signals in run-time
    if (ok) {
        ok = data.MoveRelative("Signals");
    }
    if (ok) {
        ok = data.Write("Locked", 1u);
    }
    if (ok) {
        ok = data.MoveToAncestor(1u);
    }
    if (ok) {
        ok = executor.Initialise(data);
    }
    if (ok) {
        uint32 cpuMask;
        if (!data.Read("CPUMask", cpuMask)) {
            cpuMask = 0xFFu;
            REPORT_ERROR(ErrorManagement::Warning, "CPUMask not specified using: %d", cpuMask);
        }
        executor.SetPriorityClass(Threads::RealTimePriorityClass);
        executor.SetCPUMask(cpuMask);
    }

    if (ok) {
        muxSem.Create();
    }
    return ok;
}

bool UARTDataSource::SetConfiguredDatabase(StructuredDataI &data) {
    bool ok = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ok) {
        ok = (GetNumberOfSignals() == 3u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Exactly three signals shall be defined (DataOK, TimeStamp and Data)");
        }
    }
    if (ok) {
        ok = (GetSignalType(0u) == UnsignedInteger8Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The type of the first signal (DataOK shall be UnsignedInteger8)");
        }
    }
    if (ok) {
        ok = (GetSignalType(1u) == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The type of the second signal (TimePacket shall be UnsignedInteger64)");
        }
    }
    if (ok) {
        uint32 elements;
        ok = GetSignalNumberOfElements(0u, elements);
        if (ok) {
            ok = (elements == 1u);
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The first signal (DataOK) shall have only one element");
        }
    }
    if (ok) {
        ok = GetSignalByteSize(2u, packetByteSize);
    }
    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "Going to read %d bytes from the serial interface.", packetByteSize);

        writeMark = new bool[numberOfBuffers];
        uint32 i;
        for (i = 0u; i < numberOfBuffers; i++) {
            writeMark[i] = false;
        }

    }
    return ok;
}
/*lint -e{1764} function prototype is derived from upper class*/
ErrorManagement::ErrorType UARTDataSource::CRIOThreadCallback(ExecutionInfo &info) {
    ErrorManagement::ErrorType err;
    if (info.GetStage() == ExecutionInfo::MainStage) {
        if (muxSem.FastLock() == ErrorManagement::NoError) {
            if (writeMark != NULL_PTR(bool*)) {
                if (writeMark[lastWrittenIdx]) {
                    REPORT_ERROR(ErrorManagement::Warning, "Buffer overflow at index %d", lastWrittenIdx);
                }
            }
        }
        muxSem.FastUnLock();

        //Memory is not interleaved, thus there will be numberOfBuffers dataOK before the actual data
        uint32 serialWriteIdx = static_cast<uint32>((numberOfBuffers * (sizeof(uint64) + sizeof(uint8)))) + (lastWrittenIdx * packetByteSize);
        uint32 timeWriteIdx = numberOfBuffers + static_cast<uint32>(lastWrittenIdx * sizeof(uint64));
        uint32 timeWriteIdx_1 = numberOfBuffers;
        if (lastWrittenIdx > 0u) {
            timeWriteIdx_1 += ((lastWrittenIdx - 1u) * static_cast<uint32>(sizeof(uint64)));
        }
        else {
            timeWriteIdx_1 += ((numberOfBuffers - 1u) * static_cast<uint32>(sizeof(uint64)));
        }

        uint32 bytesToRead = packetByteSize;
        uint8 *dataOK = reinterpret_cast<uint8*>(&(memory[lastWrittenIdx]));
        /*lint -e{927, 826} memory is a uint64, thus the casting will not lead to precision error. timePacket will be 8 bytes on the memory array*/
        uint64 *timePacket = reinterpret_cast<uint64*>(&(memory[timeWriteIdx]));
        /*lint -e{927, 826} memory is a uint64, thus the casting will not lead to precision error. timePacket will be 8 bytes on the memory array*/
        *timePacket = *reinterpret_cast<uint64*>(&(memory[timeWriteIdx_1]));

        if (serial.Read(reinterpret_cast<char8*>(&(memory[serialWriteIdx])), bytesToRead, serialTimeout)) {
            if (muxSem.FastLock() == ErrorManagement::NoError) {
                *dataOK = 1u;
                if (writeMark != NULL_PTR(bool *)) {
                    writeMark[lastWrittenIdx] = true;
                    lastWrittenIdx++;
                    if (lastWrittenIdx == numberOfBuffers) {
                        lastWrittenIdx = 0u;
                    }
                    if (!eventSem.Post()) {
                        REPORT_ERROR(ErrorManagement::OSError, "Failed to post EventSem");
                    }
                }
            }
            *timePacket = timeProvider->Timestamp();
            err = (*timePacket != 0LLU);

            muxSem.FastUnLock();
        }
        else {
            //No data received from the serial.Read. Allow the MARTe real-time thread to execute, but set the dataOK to false.
            if (bytesToRead == 0u) {
                if (muxSem.FastLock() == ErrorManagement::NoError) {
                    *dataOK = 0u;
                    if (!eventSem.Post()) {
                        REPORT_ERROR(ErrorManagement::OSError, "Failed to post EventSem");
                    }
                }
                muxSem.FastUnLock();
            }
            //Read only part of the packet from the serial.Read.
            else {
                //We are lost. Ignore anything coming from the serial until we have a silent period. After a period of no data, assume that the data is framed again...
                REPORT_ERROR(ErrorManagement::Warning, "Failed to read %d bytes from serial. Trying to resynchronise by waiting %d us for no data", packetByteSize, timeoutToSynchronise);
                while (serial.WaitRead(timeoutToSynchronise)) {
                    char8 ignoredMem;
                    uint32 ignoredSize = 1u;
                    (void) serial.Read(&ignoredMem, ignoredSize);
                }
                REPORT_ERROR(ErrorManagement::Warning, "No data arrived in the last %d us. As such, the next packet should be synchronised.", timeoutToSynchronise);
            }
        }
    }
    else if (info.GetStage() == ExecutionInfo::StartupStage) {
        //Empty the UART. If in one second no data arrives, assume to be synchronised.
        while (serial.WaitRead(100u)) {
            char8 ignoredMem;
            uint32 ignoredSize = 1u;
            (void) serial.Read(&ignoredMem, ignoredSize);
        }
        REPORT_ERROR(ErrorManagement::Warning, "UART should now be empty");
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Termination stage");
    }

    return err;
}

void UARTDataSource::PrepareInputOffsets() {

    if (muxSem.FastLock() == ErrorManagement::NoError) {
        if (writeMark != NULL_PTR(bool *)) {
            if (!writeMark[lastReadIdx]) {
                if (!eventSem.Reset()) {
                    REPORT_ERROR(ErrorManagement::OSError, "Failed to reset EventSem");
                }
                muxSem.FastUnLock();
                if (!eventSem.Wait(timeout)) {
                    REPORT_ERROR(ErrorManagement::OSError, "Failed to wait EventSem");
                }
            }
            else {
                muxSem.FastUnLock();
            }
        }
    }
}

/*lint -e{715} function prototype is derived from upper class*/
bool UARTDataSource::GetInputOffset(const uint32 signalIdx,
        const uint32 numberOfSamples,
        uint32 &offset) {
    if (signalIdx == 0u) {
        //Remember that the memory is organised as non-interleaved, i.e. signal1_1, signal1_2, ..., signal1_N, signal2_1, ..., signal2_N
        offset = lastReadIdx;
        if (muxSem.FastLock() == ErrorManagement::NoError) {
            uint8 *dataOK = reinterpret_cast<uint8*>(&(memory[offset]));
            //Remember with 100% guarantee if the data was copied with a timeout or not, so that the lastReadIdx does not get incremented if new data
            //become available between here and TerminateInputCopy
            //See comment below in TerminateInputCopy.
            lastDataCopyHadTimeout = (*dataOK == 0u);
        }
        muxSem.FastUnLock();
    }
    else if (signalIdx == 1u) {
        //Remember that the memory pointer returned by the MemoryDataSourceI::GetSignalMemoryBuffer (and used by the broker) already points
        //to the beginning of each signal memory, so that each signal memory starts with a zero offset. Nevertheless, the "memory" variable points to the beginning
        //of the full memory (i.e. at the beginning of the memory of signal zero).
        offset = (lastReadIdx * static_cast<uint32>(sizeof(uint64)));
    }
    else {
        offset = (lastReadIdx * packetByteSize);
    }

    return true;
}
///home/Giuseppe/MARTe2Project/GIT/MARTe2Hieratika/CSS_OPI/CSS_EPICS_3.1.4/configuration/org.eclipse.osgi/bundles/58/1/.cp/:
/*lint -e{715} function prototype is derived from upper class*/
bool UARTDataSource::TerminateInputCopy(const uint32 signalIdx,
        const uint32 offset,
        const uint32 numberOfSamples) {
    //Only clear the dataOK after the data signal (i.e. signalIdx == 1) has been copied.
    if (signalIdx == 2u) {
        //Only increment if there was no timeout. The status of dataOK could have changed between the GetInputOffset and the TerminateInputCopy
        //i.e. GetInputOffset could have copied data with a timeout for a given index, but when getting into TerminateInputCopy new data could
        //be available. This would imply writeMark[lastReadIdx] = true and as such the lastReadIdx would be incremented and the data in lastReadIdx - 1 would not get coppied in
        //the next GetInputOffset
        if (!lastDataCopyHadTimeout) {
            if (muxSem.FastLock() == ErrorManagement::NoError) {
                if (writeMark != NULL_PTR(bool *)) {
                    if (writeMark[lastReadIdx]) {
                        writeMark[lastReadIdx] = false;
                        //Data was already read. Reset to zero the DataOK flag. (Memory is not interleaved, thus there will be numberOfBuffers dataOK before the data)
                        uint8 *dataOK = reinterpret_cast<uint8*>(&(memory[lastReadIdx]));
                        *dataOK = 0u;
                        lastReadIdx++;
                        if (lastReadIdx == numberOfBuffers) {
                            lastReadIdx = 0u;
                        }
                    }
                }
            }
            lastDataCopyHadTimeout = true;
            muxSem.FastUnLock();
        }
    }
    return true;
}

/*lint -e{715} function prototype is derived from upper class*/
const char8* UARTDataSource::GetBrokerName(StructuredDataI &data,
        const SignalDirection direction) {
    return "MemoryMapMultiBufferInputBroker";
}

bool UARTDataSource::Synchronise() {
    return true;
}

/*lint -e{715} function prototype is derived from upper class*/
bool UARTDataSource::PrepareNextState(const char8 *const currentStateName,
        const char8 *const nextStateName) {

    bool ok = true;
    uint32 numberOfReadWrite = 0u;

    for (uint32 i = 0u; i < numberOfSignals; i++) {
        uint32 numberOfProducersCurrentState;
        if (!GetSignalNumberOfProducers(i, nextStateName, numberOfProducersCurrentState)) {
            numberOfProducersCurrentState = 0u;
        }
        numberOfReadWrite += numberOfProducersCurrentState;
        uint32 numberOfConsumersCurrentState;
        if (!GetSignalNumberOfConsumers(i, nextStateName, numberOfConsumersCurrentState)) {
            numberOfConsumersCurrentState = 0u;
        }
        numberOfReadWrite += numberOfConsumersCurrentState;
    }
    if (numberOfReadWrite > 0u) {
        if (executor.GetStatus() == EmbeddedThreadI::OffState) {
            ok = (executor.Start() == ErrorManagement::NoError);
        }
    }
    else {
        if (executor.Stop() != ErrorManagement::NoError) {
            if (executor.Stop() != ErrorManagement::NoError) {
                ok = false;
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop the executor");
            }
        }
    }
    return ok;
}

ErrorManagement::ErrorType UARTDataSource::StopAcquisition() {
    ErrorManagement::ErrorType err;
    if (executor.GetStatus() != EmbeddedThreadI::OffState) {
        REPORT_ERROR(ErrorManagement::FatalError, "First");
        err = (!executor.Stop());
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Second");
            err = (!executor.Stop());
            if (!err.ErrorsCleared()) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not stop SingleThreadService.");
            }
        }
    }
    return err;
}

CLASS_REGISTER(UARTDataSource, "1.0")
    CLASS_METHOD_REGISTER(UARTDataSource, StopAcquisition)
}
