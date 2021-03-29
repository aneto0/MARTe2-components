/**
 * @file CounterChecker.cpp
 * @brief Source file for class CounterChecker.
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
 * the class CounterChecker (public, protected, and private). Be aware that
 * some methods, such as those inline could be defined on the header file,
 * instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CounterChecker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

CounterChecker::CounterChecker() :
        SampleChecker() {

    packetCounter = 1u;
    acquireFromCounter = 0u;
    nextPacketCheck = 1u;
    checkCounterAfterNSteps = 0u;
    counterStep = 1u;

}

CounterChecker::~CounterChecker() {
}

bool CounterChecker::Initialise(StructuredDataI &data) {

    bool ret;
    REPORT_ERROR(ErrorManagement::Information, "CounterChecker::Initialise");
    
    ret = SampleChecker::Initialise(data);
    if (ret) {
        ret = (nFrameForSync > 1u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "CounterChecker::Initialise NumOfFrameForSync must be > 1");
        }
        else {
            if (!data.Read("FirstPacketCounter", packetCounter)) {
                packetCounter = 1u;
            }
            //wait this packet counter before save data
            if (!data.Read("AcquireFromCounter", acquireFromCounter)) {
                acquireFromCounter = packetCounter;
            }
            //the counter step
            if (!data.Read("CounterStep", counterStep)) {
                counterStep = 1u;
            }
            else {
                ret = (counterStep > 0u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "CounterChecker::Initialise CounterStep must be > 0");
                }
            }
            //check the counter each N
            if (!data.Read("CheckCounterAfterNSteps", checkCounterAfterNSteps)) {
                checkCounterAfterNSteps = counterStep;
            }
            if (ret) {
                ret = ((checkCounterAfterNSteps % counterStep) == 0u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "CounterChecker::Initialise CheckCounterAfterNPackets=%d must divide exactly CounterStep=%d", checkCounterAfterNSteps, counterStep);
                }
                else {
                    ret = (((acquireFromCounter - packetCounter) % counterStep) == 0u);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "CounterChecker::Initialise (AcquireFromCounter=%d - FirstPacketCounter=%d) must divide exactly CounterStep=%d", acquireFromCounter, packetCounter, counterStep);
                    }
                }
            }
            if (ret) {
                nextPacketCheck = packetCounter;
            }
        }
    }

    REPORT_ERROR(ret ? ErrorManagement::Information : ErrorManagement::InitialisationError, "CounterChecker::Initialise retruning %s", ret ? "true" : "false");
    return ret;
}

/*lint -e{952} -e{578} parameter 'sample' not declared as const*/
bool CounterChecker::Check(uint8 *sample,
                           bool &write) {

    bool ret = true;
    write = true;

    if (packetCounter == nextPacketCheck) {
        ret = (MemoryOperationsHelper::Compare(&packetCounter, sample, sampleSize) == 0);
    }
    if (ret) {
        nextPacketCheck = (packetCounter + checkCounterAfterNSteps);
        packetCounter += counterStep;
        //before the first packet, check each packet
        if (acquireFromCounter > 0ull) {
            /*lint -e{340} -e{928} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
            /*lint -e{613} NULL pointer checked*/
            if (MemoryOperationsHelper::Compare(&(sample[0]), reinterpret_cast<uint8*>((&acquireFromCounter)), sampleSize) != 0) {
                nextPacketCheck = packetCounter;
                //return without adding nothing to the circular buffer
                write = false;
            }
            else {
                acquireFromCounter = 0ull;
            }
        }
    }

    return ret;
}

/*lint -e{952} -e{578} parameter 'sample' not declared as const*/
bool CounterChecker::Synchronise(uint8 *frames,
                                 uint32 sizeToRead,
                                 uint32 &idx,
                                 bool &write) {

    write = true;
    uint32 syncCnt = 1u;
    uint64 candidate = 0ull;
    idx = 0u;

    while (idx < sizeToRead) {
        //try all the elements and compare them with the relative ones in the other frames
        candidate = 0ull;
        /*lint -e{340} -e{534} -e{927} -e{928} -e{826} -e{740} Allowed cast from pointer to pointer*/
        MemoryOperationsHelper::Copy(reinterpret_cast<uint8*>(&candidate), reinterpret_cast<uint8*>(&frames[idx]), sampleSize);
        syncCnt = 1u;
        for (uint32 j = 1u; (j < nFrameForSync) && (syncCnt < nFrameForSync); j++) {
            /*lint -e{9123} -e{647} allowed cast to larger type*/
            uint64 nextCandidate = candidate + static_cast<uint64>(j * counterStep);
            uint32 index = (j * sizeToRead) + idx;
            /*lint -e{340} -e{927} -e{928} -e{826} -e{740} Allowed cast from pointer to pointer*/
            if (MemoryOperationsHelper::Compare(reinterpret_cast<uint8*>(&nextCandidate), reinterpret_cast<uint8*>(&frames[index]), sampleSize) == 0) {
                syncCnt++;
            }
            else {
                break;
            }
        }
        if (syncCnt >= nFrameForSync) {
            break;
        }
        idx += sampleSize;
    }

    bool ret = (syncCnt >= nFrameForSync);
    if (!ret) {
        /*lint -e{927} -e{826} Allowed cast from pointer to pointer*/
        REPORT_ERROR(ErrorManagement::FatalError, "CounterChecker::Synchronise Failed Sync for Frame %u PacketCounter %u", *reinterpret_cast<uint64*>(&(frames[0])), static_cast<uint64>(packetCounter));
    }
    else {
        /*lint -e{9123} -e{647} allowed cast to larger type*/
        uint64 newCounter = candidate + static_cast<uint64>(nFrameForSync * counterStep);
        REPORT_ERROR(ErrorManagement::Information, "CounterChecker::Synchronise Resync done with newCounter %u", newCounter);

        /*lint -e{9123} -e{647} allowed cast to larger type*/
        packetCounter = candidate + static_cast<uint64>(nFrameForSync * counterStep);
        nextPacketCheck = ((packetCounter - counterStep) + checkCounterAfterNSteps);

        //before the first packet, check each packet
        if (acquireFromCounter > 0ull) {
            /*lint -e{340} -e{928} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
            /*lint -e{613} NULL pointer checked*/
            if (MemoryOperationsHelper::Compare(reinterpret_cast<uint8*>(&(frames[0])), reinterpret_cast<uint8*>((&acquireFromCounter)), sampleSize) != 0) {
                nextPacketCheck = packetCounter;
                write = false;
            }
            else {
                acquireFromCounter = 0ull;
            }
        }
    }

    return ret;
}

CLASS_REGISTER(CounterChecker, "1.0")

}
