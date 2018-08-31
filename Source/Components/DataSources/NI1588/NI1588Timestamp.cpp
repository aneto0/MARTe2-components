/**
 * @file NI1588Timestamp.cpp
 * @brief Source file for class NI1588Timestamp
 * @date 14/05/2018
 * @author Giuseppe Ferrò
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
 * the class NI1588Timestamp (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <nisync-lib.h>
#include <nisync.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "GlobalObjectsDatabase.h"
#include "NI1588Timestamp.h"
#include "ObjectRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -estring(1066, "*MARTe::NI1588Timestamp::*") cannot include DANAPI.cpp on the files to be linted as otherwise it would go back to the original issue.*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{1541} members capturedEventType, signalTs and pollFd are initialised*/
NI1588Timestamp::NI1588Timestamp() :
        CircularBufferThreadInputDataSource() {
    pollTimeout = -1;

    for (uint32 i = 0u; i < 6u; i++) {
        signalIndexes[i] = 0xFFFFFFFFu;
        if (i < 3u) {
            capturedEventType[i] = 0u;
            signalTs[i] = 0ull;
            pollfd[i].fd = -1;
            pollfd[i].events = 0;
            pollfd[i].revents = 0;
        }
    }

    devNum = -1;
    arrivedMask = 0u;
}

NI1588Timestamp::~NI1588Timestamp() {
    for (uint32 i = 0u; i < 3u; i++) {
        if (pollfd[i].fd > 0) {
            /*lint -e{1551} this function has to be called in the destructor*/
            if(close(pollfd[i].fd) != 0) {
                REPORT_ERROR(ErrorManagement::OSError, "Failed to close device with fd %d", pollfd[i].fd);
            }
        }
    }

}

bool NI1588Timestamp::Initialise(StructuredDataI &data) {
    bool ret = CircularBufferThreadInputDataSource::Initialise(data);

    if (ret) {
        if (!data.Read("PollMsecTimeout", pollTimeout)) {
            //assume blocking
            pollTimeout = -1;
        }
        if (!data.Read("NiSyncDeviceNumber", devNum)) {
            devNum = 0;
        }
    }

    return ret;
}

bool NI1588Timestamp::SetConfiguredDatabase(StructuredDataI & data) {

    bool ret = CircularBufferThreadInputDataSource::SetConfiguredDatabase(data);

    //4 signals max...timestamp, internal timestamp, terminal N, event
    if (ret) {
        uint32 nRealChannels = numberOfChannels;

        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            StreamString signalName;
            ret = GetSignalName(i, signalName);
            if ((i != timeStampSignalIndex) && (i != errorCheckSignalIndex)) {
                uint32 numberOfElements = 0u;
                if (ret) {
                    ret = GetSignalNumberOfElements(i, numberOfElements);
                    if (ret) {
                        ret = (numberOfElements == 1u);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::FatalError, "The signal %s must have NumberOfElements = 1", signalName.Buffer());
                        }
                    }
                }
            }
            uint32 terminalIndex = 0xFFFFFFFFu;
            if (ret) {
                if (signalName == "TerminalPFI0") {
                    //check the type
                    TypeDescriptor td;
                    td = GetSignalType(i);

                    ret = (td == UnsignedInteger64Bit);
                    if (ret) {
                        pollfd[0].fd = nisync_open_terminal(NISYNC_DEVICE_TYPE_PCI1588, devNum, NISYNC_PFI0, NISYNC_READ_NONBLOCKING);
                        pollfd[0].events = POLLIN;
                        terminalIndex = 0u;
                        nRealChannels--;
                        signalIndexes[0] = i;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the signal %s must be uint64", signalName.Buffer());
                    }
                }
                else if (signalName == "TerminalPFI1") {
                    //check the type
                    TypeDescriptor td;
                    td = GetSignalType(i);

                    ret = (td == UnsignedInteger64Bit);
                    if (ret) {
                        pollfd[1].fd = nisync_open_terminal(NISYNC_DEVICE_TYPE_PCI1588, devNum, NISYNC_PFI1, NISYNC_READ_NONBLOCKING);
                        pollfd[1].events = POLLIN;
                        terminalIndex = 1u;
                        nRealChannels--;
                        signalIndexes[1] = i;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the signal %s must be uint64", signalName.Buffer());
                    }
                }
                else if (signalName == "TerminalPFI2") {
                    //check the type
                    TypeDescriptor td;
                    td = GetSignalType(i);

                    ret = (td == UnsignedInteger64Bit);
                    if (ret) {
                        pollfd[2].fd = nisync_open_terminal(NISYNC_DEVICE_TYPE_PCI1588, devNum, NISYNC_PFI2, NISYNC_READ_NONBLOCKING);
                        pollfd[2].events = POLLIN;
                        terminalIndex = 2u;
                        signalIndexes[2] = i;
                        nRealChannels--;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the signal %s must be uint64", signalName.Buffer());
                    }

                }
                else if (signalName == "EventPFI0") {
                    //check the type
                    TypeDescriptor td;
                    td = GetSignalType(i);

                    ret = (td == UnsignedInteger8Bit);
                    if (ret) {
                        signalIndexes[3] = i;
                        nRealChannels--;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the signal %s must be uint8", signalName.Buffer());
                    }
                }
                else if (signalName == "EventPFI1") {
                    //check the type
                    TypeDescriptor td;
                    td = GetSignalType(i);

                    ret = (td == UnsignedInteger8Bit);
                    if (ret) {
                        signalIndexes[4] = i;
                        nRealChannels--;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the signal %s must be uint8", signalName.Buffer());
                    }
                }
                else if (signalName == "EventPFI2") {
                    //check the type
                    TypeDescriptor td;
                    td = GetSignalType(i);

                    ret = (td == UnsignedInteger8Bit);
                    if (ret) {
                        signalIndexes[5] = i;
                        nRealChannels--;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "The type of the signal %s must be uint8", signalName.Buffer());
                    }
                }
                else {

                }

                if ((ret) && (terminalIndex != 0xFFFFFFFFu)) {
                    bool ok = signalsDatabase.MoveAbsolute("Signals");
                    if (ok) {
                        ok = signalsDatabase.MoveRelative(signalName.Buffer());
                    }

                    StreamString captureEventName;
                    if (ok) {
                        if (!signalsDatabase.Read("CaptureEvent", captureEventName)) {
                            REPORT_ERROR(ErrorManagement::Information, "CaptureEvent not specified, set %s by default", captureEventName.Buffer());
                        }
                    }
                    if (captureEventName == "") {
                        captureEventName = "NISYNC_EDGE_ANY";
                    }
                    if (captureEventName == "NISYNC_EDGE_ANY") {
                        ret = (nisync_enable_timestamp_trigger(pollfd[terminalIndex].fd, NISYNC_EDGE_ANY, 1u) == 0);
                    }
                    else if (captureEventName == "NISYNC_EDGE_RISING") {
                        ret = (nisync_enable_timestamp_trigger(pollfd[terminalIndex].fd, NISYNC_EDGE_RISING, 1u) == 0);
                    }
                    else if (captureEventName == "NISYNC_EDGE_FALLING") {
                        ret = (nisync_enable_timestamp_trigger(pollfd[terminalIndex].fd, NISYNC_EDGE_FALLING, 1u) == 0);
                    }
                    else {
                        ret = false;
                        uint32 ii = i;
                        REPORT_ERROR(ErrorManagement::FatalError, "Invalid CaptureEvent %s for signal %d", captureEventName.Buffer(), ii);
                    }

                }
            }
        }

        if (ret) {
            ret = (nRealChannels == 0u);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "This DataSource supports three timestamp signals and their relative events");
            }
        }
    }

    return ret;
}

bool NI1588Timestamp::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    int32 rc = poll(&pollfd[0], static_cast<nfds_t>(3u), 0);
    //empties the queue
    if (rc > 0) {
        nisync_timestamp_nanos_t ts;
        (void) MemoryOperationsHelper::Set(&ts, '\0', static_cast<uint32>(sizeof(nisync_timestamp_nanos_t)));
        for (uint32 i = 0u; (i < 3u); i++) {
            /* Skip non-ready terminals */
            if (((static_cast<uint16>(pollfd[i].revents)) & (static_cast<uint16>(POLLIN))) != 0u) {
                //get the latest
                while (nisync_read_timestamps_ns(pollfd[i].fd, &ts, static_cast<size_t>(1u)) > 0) {
                }
            }
        }
    }

    return CircularBufferThreadInputDataSource::PrepareNextState(currentStateName, nextStateName);

}

bool NI1588Timestamp::DriverRead(char8 * const bufferToFill, uint32 &sizeToRead, const uint32 signalIdx) {

    uint64 tic = HighResolutionTimer::Counter();
    static uint32 max = 0u;
    static uint32 min = 0xFFFFFFFFu;
    static uint32 cnt = 0u;
    int32 readTerm = -1;

    bool ret = true;

    if (arrivedMask == 0u) {
        // At least one event was received, but possibly many
        int32 rc = poll(&pollfd[0], static_cast<nfds_t>(3u), pollTimeout);

        ret = (rc >= 0);
        // Read received events from ready terminals
        if (ret) {
            if (rc > 0) {
                nisync_timestamp_nanos_t ts;
                (void) MemoryOperationsHelper::Set(&ts, '\0', static_cast<uint32>(sizeof(nisync_timestamp_nanos_t)));
                for (uint32 i = 0u; (i < 3u); i++) {
                    int32 nEventsRead = 0;
                    /* Skip non-ready terminals */
                    if (((static_cast<uint16>(pollfd[i].revents)) & (static_cast<uint16>(POLLIN))) != 0u) {
                        int32 count = 1;

                        //get the latest
                        while (count > 0) {
                            count = nisync_read_timestamps_ns(pollfd[i].fd, &ts, static_cast<size_t>(1u));
                            if (count < 0) {
                                count = 0;
                            }
                            nEventsRead += count;
                        }
                    }

                    //report in ErrorCheck that some timestamps have been skipped
                    if (nEventsRead > 1) {

                        if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                            uint32 signalIndex = signalIndexes[i];
                            if (signalIndexes[i] > errorCheckSignalIndex) {
                                signalIndex--;
                            }
                            if (signalIndexes[i] > timeStampSignalIndex) {
                                signalIndex--;
                            }

                            uint32 index1 = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                            uint32 errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1 + signalIndex) * static_cast<uint32>(sizeof(uint32))));

                            /*lint -e{927} -e{826} the memory holds references to 32 bit ErrorCheck signals, so this is safe.*/
                            *reinterpret_cast<uint32*>(&(memory[errorMemIndex])) |= 8u;
                        }
                    }

                    if (nEventsRead > 0) {
                        readTerm = static_cast<int32>(i);
                        signalTs[i] = ts.nanos;
                        capturedEventType[i] = ts.edge;
                        if (signalIndexes[i] != 0xFFFFFFFFu) {
                            arrivedMask |= static_cast<uint8>(1u << i);
                        }
                        uint32 eventIndex = (i + 3u);
                        if (signalIndexes[eventIndex] != 0xFFFFFFFFu) {
                            arrivedMask |= static_cast<uint8>(1u << (3u + i));
                        }
                    }
                }
            }
            else {

                //report in ErrorCheck
                if (errorCheckSignalIndex != 0xFFFFFFFFu) {
                    uint32 signalIndex = signalIdx;
                    if (signalIdx > errorCheckSignalIndex) {
                        signalIndex--;
                    }
                    if (signalIdx > timeStampSignalIndex) {
                        signalIndex--;
                    }

                    uint32 index1 = (currentBuffer[errorCheckSignalIndex] * (numberOfChannels));
                    uint32 errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1 + signalIndex) * static_cast<uint32>(sizeof(uint32))));
                    //timeout error
                    /*lint -e{927} -e{826} the memory holds references to 32 bit ErrorCheck signals, so this is safe.*/
                    *reinterpret_cast<uint32*>(&(memory[errorMemIndex])) |= 4u;
                }
                sizeToRead = 0u;
            }
        }
    }

    if (signalIdx == signalIndexes[0]) {
        if ((arrivedMask & 0x1u) != 0u) {
            /*lint -e{927} -e{826} The bufferToFill should have the correct signal dimensions.*/
            *reinterpret_cast<uint64*>(bufferToFill) = signalTs[0];
            arrivedMask &= ~(0x1u);
        }
        else {
            sizeToRead = 0u;
        }
    }
    else if (signalIdx == signalIndexes[1]) {
        if ((arrivedMask & 0x2u) != 0u) {
            /*lint -e{927} -e{826} The bufferToFill should have the correct signal dimensions.*/
            *reinterpret_cast<uint64*>(bufferToFill) = signalTs[1];
            arrivedMask &= ~(0x2u);
        }
        else {
            sizeToRead = 0u;
        }
    }
    else if (signalIdx == signalIndexes[2]) {
        if ((arrivedMask & 0x4u) != 0u) {
            /*lint -e{927} -e{826} The bufferToFill should have the correct signal dimensions.*/
            *reinterpret_cast<uint64*>(bufferToFill) = signalTs[2];
            arrivedMask &= ~(0x4u);
        }
        else {
            sizeToRead = 0u;
        }
    }
    else if (signalIdx == signalIndexes[3]) {
        if ((arrivedMask & 0x8u) != 0u) {
            /*lint -e{927} -e{826} The bufferToFill should have the correct signal dimensions.*/
            *reinterpret_cast<uint8*>(bufferToFill) = capturedEventType[0];
            arrivedMask &= ~(0x8u);
        }
        else {
            sizeToRead = 0u;
        }
    }
    else if (signalIdx == signalIndexes[4]) {
        if ((arrivedMask & 0x10u) != 0u) {
            /*lint -e{927} -e{826} The bufferToFill should have the correct signal dimensions.*/
            *reinterpret_cast<uint8*>(bufferToFill) = capturedEventType[1];
            arrivedMask &= ~(0x10u);
        }
        else {
            sizeToRead = 0u;
        }
    }
    else if (signalIdx == signalIndexes[5]) {
        if ((arrivedMask & 0x20u) != 0u) {
            /*lint -e{927} -e{826} The bufferToFill should have the correct signal dimensions.*/
            *reinterpret_cast<uint8*>(bufferToFill) = capturedEventType[2];
            arrivedMask &= ~(0x20u);
        }
        else {
            sizeToRead = 0u;
        }
    }
    else {
        ret = false;
    }

    uint64 delta = HighResolutionTimer::Counter() - tic;
    float64 deltaS = static_cast<float64>(delta) * HighResolutionTimer::Period();
    float64 deltaUS = deltaS * 1e6;
    uint32 elapsed = static_cast<uint32>(deltaUS);

    if (cnt > 10u) {
        if (elapsed > max) {
            max = elapsed;

            REPORT_ERROR(ErrorManagement::Information, "max=%d cnt=%d term=%d", max, cnt, readTerm);
        }
        if (elapsed < min) {
            min = elapsed;

            REPORT_ERROR(ErrorManagement::Information, "min=%d cnt=%d term=%d", min, cnt, readTerm);
        }
    }
    cnt++;

    return ret;
}

CLASS_REGISTER(NI1588Timestamp, "1.0")
}
