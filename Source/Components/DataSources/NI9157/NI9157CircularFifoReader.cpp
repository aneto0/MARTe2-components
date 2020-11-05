/**
 * @file NI9157CircularFifoReader.cpp
 * @brief Source file for class NI9157CircularFifoReader
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
 * the class NI9157CircularFifoReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CreateNI9157DeviceOperatorI.h"
#include "MemoryOperationsHelper.h"
#include "NI9157CircularFifoReader.h"
#include "NI9157DeviceOperatorDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "Optim/NI9157MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

NI9157CircularFifoReader::NI9157CircularFifoReader() :
        CircularBufferThreadInputDataSource() {

    checkPacketCounter = 0u;
    nFrameForSync = 1u;
    sampleByteSize = 0u;
    niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    fifoDev = 0u;
    totalReadSize = 0u;
    middleBuffer = NULL_PTR(uint8 *);
    runNi = 0u;
    packetCounter = 1u;
    acquireFromCounter = 0u;
    nextPacketCheck = 1u;
    checkCounterAfterNSteps = 0u;
    counterStep = 1u;
    numberOfPacketsInFIFO = 10u;
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

bool NI9157CircularFifoReader::Initialise(StructuredDataI &data) {
    bool ret = CircularBufferThreadInputDataSource::Initialise(data);

    //read the FIFO name
    if (ret) {
        ret = data.Read("FifoName", fifoName);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot find FifoName");
        }
    }

    //read the ni device path
    if (ret) {
        StreamString devicePath;
        ret = data.Read("NI9157DevicePath", devicePath);
        if (ret) {
            niDeviceBoard = ObjectRegistryDatabase::Instance()->Find(devicePath.Buffer());
            ret = niDeviceBoard.IsValid();
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Cannot find the the NI-Dev at path %s", devicePath.Buffer());
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Please specify the NI-Dev path (NI9157DevicePath)");
        }
    }

    if (ret) {
//the device must be run in this data source?
        if (!data.Read("RunNi", runNi)) {
            runNi = 0u;
        }
//the number of packets in FIFO
        if (!data.Read("NumberOfPacketsInFIFO", numberOfPacketsInFIFO)) {
            numberOfPacketsInFIFO = 10u;
        }

        if (!data.Read("CheckPacketCounter", checkPacketCounter)) {
            checkPacketCounter = 0u;
        }

        if (checkPacketCounter > 0u) {
            //read the number of frame to use to resync
            if (data.Read("NumOfFrameForSync", nFrameForSync)) {
                ret = (nFrameForSync > 1u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "NumOfFrameForSync must be >= 2");
                }
            }
            else {
                nFrameForSync = 2u;
            }
            if (ret) {

                if (!data.Read("FirstPacketCounter", packetCounter)) {
                    packetCounter = 0u;
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
                    ret = counterStep > 0u;
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "CounterStep must be >0");
                    }
                }

                //check the counter each N
                if (!data.Read("CheckCounterAfterNSteps", checkCounterAfterNSteps)) {
                    checkCounterAfterNSteps = counterStep;
                }
                if (ret) {
                    ret = ((checkCounterAfterNSteps % counterStep) == 0u);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "CounterStep=%d must divide exactly CheckCounterAfterNPackets=%d", counterStep, checkCounterAfterNSteps);
                    }
                    if (ret) {
                        ret = (((acquireFromCounter - packetCounter) % counterStep) == 0u);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::InitialisationError, "AcquireFromCounter=%d must divide exactly CounterStep=%d", acquireFromCounter, counterStep);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

bool NI9157CircularFifoReader::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = CircularBufferThreadInputDataSource::SetConfiguredDatabase(data);

    if (ret) {
        ret = (numberOfChannels == 1u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "One signal (over the InternalTimeStamp and ErrorCheck signals) must be defined");
        }
    }

    //check that the signals byte size is multiple of sampleByteSize
    if (ret) {
        totalReadSize = 0u;
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            if ((i != timeStampSignalIndex) && (i != errorCheckSignalIndex)) {
                TypeDescriptor td = GetSignalType(i);

                CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(td);
                ret = (creator != NULL);
                if (ret) {
                    niDeviceOperator = creator->Create(niDeviceBoard);
                    ret = (niDeviceOperator != NULL);
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
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "Failed creating the NiDevice operator");
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Cannot find the CreateNI9157DeviceOperator");
                }
            }
        }
    }

    if (ret) {
        /*lint -e{613} NULL pointer checked.*/
        NiFpga_Status status = niDeviceOperator->FindResource(fifoName.Buffer(), fifoDev);
        ret = (status == 0);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed FindResource(%s), status=%d", fifoName.Buffer(), status);
        }
    }

    if (ret) {
        nextPacketCheck = packetCounter;
    }
    return ret;
}

bool NI9157CircularFifoReader::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    /*lint -e{414} division by 0 checked*/
    uint32 hostFifoSize = (totalReadSize * numberOfPacketsInFIFO) / sampleByteSize;
    uint32 oldSize = 0u;
    NiFpga_Status status = niDeviceBoard->NiConfigureFifo(fifoDev, hostFifoSize, oldSize);
    bool ret = (status == 0);
    if (!ret) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to configure host FIFO size to %d, status=%d", hostFifoSize, status);
    }
    else {
        ret = CircularBufferThreadInputDataSource::PrepareNextState(currentStateName, nextStateName);
    }

    return ret;
}

/*lint -e{715} symbol not referenced*/
bool NI9157CircularFifoReader::DriverRead(char8 * const bufferToFill, uint32 &sizeToRead, const uint32 signalIdx) {

    bool ret = true;
    if (runNi == 1u) {
        REPORT_ERROR(ErrorManagement::Information, "Running...");
        NiFpga_Status status = niDeviceBoard->Run();
        ret = (status == 0);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to Run the application, status=%d", status);
        }
        runNi = 0u;
    }
    if (ret) {
        uint32 fifoRemaining = 0u;
        /*lint -e{613} NULL pointer is checked */
        /*lint -e{414} division by 0 checked*/
        NiFpga_Status status = niDeviceOperator->NiReadFifo(fifoDev, reinterpret_cast<uint8*>(&(middleBuffer[0])), (sizeToRead / sampleByteSize), 0xFFFFFFFFu, fifoRemaining);
        if (status != 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed reads, status=%d", status);
        }
        ret = (status == 0);

    }
    //volatile uint64 tic = HighResolutionTimer::Counter();

    if ((checkPacketCounter > 0u) && (ret)) {
        uint32 syncCnt = 1u;
        uint64 candidate = 0ull;

        if (packetCounter == nextPacketCheck) {

            //if the packet counter is correct go directly to the package procedure
            /*lint -e{340} -e{927} -e{928} -e{826} -e{740} Allowed cast from pointer to pointer*/
            /*lint -e{613} NULL pointer checked*/
            if (niDeviceOperator->Compare(reinterpret_cast<uint8*>(&packetCounter), reinterpret_cast<uint8*>(&(middleBuffer[0]))) == 0) {
                //REPORT_ERROR(ErrorManagement::Information, "Counter=%d", *(uint64*) &(bufferToFill[0]));
                //wait the first packet counter
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed Sync %d %d %d", *reinterpret_cast<uint64*>(&(middleBuffer[0])), packetCounter, signalIdx);
                if (errorCheckSignalIndex != 0xFFFFFFFFu) {

                    uint32 index1 = (currentBuffer[errorCheckSignalIndex]);
                    uint32 errorMemIndex = (signalOffsets[errorCheckSignalIndex] + ((index1) * static_cast<uint32>(sizeof(uint32))));
                    //failed sync error
                    /*lint -e{340} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
                    *reinterpret_cast<uint32*>(&(memory[errorMemIndex])) |= 8u;
                }
                uint32 idx = 0u;
                while (syncCnt < nFrameForSync) {

                    //re-sync procedure: read N other frames
                    for (uint32 i = 1u; (i < nFrameForSync) && (ret); i++) {
                        uint32 fifoRemaining = 0u;
                        uint32 index = (sizeToRead * i);
                        /*lint -e{414} division by 0 checked*/
                        NiFpga_Status status = niDeviceOperator->NiReadFifo(fifoDev, &middleBuffer[index], (sizeToRead / sampleByteSize), 0xFFFFFFFFu, fifoRemaining);
                        ret = (status == 0);
                    }
                    if (ret) {
                        idx = 0u;
                        while (idx < sizeToRead) {
                            //try all the elements and compare them with the relative
                            //ones in the other frames
                            candidate = 0ull;
                            /*lint -e{340} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
                            niDeviceOperator->Copy(reinterpret_cast<uint8*>(&candidate), reinterpret_cast<uint8*>(&middleBuffer[idx]));
                            syncCnt = 1u;
                            for (uint32 j = 1u; (j < nFrameForSync) && (syncCnt < nFrameForSync); j++) {
                                /*lint -e{9123} -e{647} allowed cast to larger type*/
                                uint64 nextCandidate = candidate + static_cast<uint64>(j * counterStep);
                                uint32 index = (j * sizeToRead) + idx;
                                //REPORT_ERROR(ErrorManagement::Information, "Comparing %d %d", nextCandidate, *(uint64*)(&middleBuffer[j * totalReadSize + idx]));
                                /*lint -e{340} -e{927} -e{928} -e{826} -e{740} Allowed cast from pointer to pointer*/
                                if (niDeviceOperator->Compare(reinterpret_cast<uint8*>(&nextCandidate), reinterpret_cast<uint8*>(&middleBuffer[index])) == 0) {
                                    syncCnt++;
                                }
                                else {
                                    break;
                                }
                            }
                            if (syncCnt >= nFrameForSync) {
                                break;
                            }
                            idx += sampleByteSize;
                        }
                        //shift in any case
                        if (idx > 0u) {
                            uint32 sizeToMove = ((sizeToRead * nFrameForSync) - idx);
                            (void) MemoryOperationsHelper::Move(&(middleBuffer[0]), &(middleBuffer[idx]), sizeToMove);
                            //REPORT_ERROR(ErrorManagement::Information, "Here1");

                            uint32 fifoRemaining = 0u;
                            /*lint -e{414} division by 0 checked*/
                            NiFpga_Status status = niDeviceOperator->NiReadFifo(fifoDev, &middleBuffer[sizeToMove], (idx / sampleByteSize), 0xFFFFFFFFu, fifoRemaining);
                            ret = (status == 0);

                            if (ret) {
                                uint32 index = sizeToRead * (nFrameForSync - 1u);
                                //copy the sync one
                                (void) MemoryOperationsHelper::Move(&(middleBuffer[0]), &(middleBuffer[index]), sizeToRead);
                            }
                        }
                    }
                }
                /*lint -e{9123} -e{647} allowed cast to larger type*/
                uint64 newCounter = candidate + static_cast<uint64>(nFrameForSync * counterStep);
                REPORT_ERROR(ErrorManagement::FatalError, "Resync done %d", newCounter);
            }

        }

        if (ret) {
            packetCounter += counterStep;
            if (syncCnt == nFrameForSync) {
                /*lint -e{9123} -e{647} allowed cast to larger type*/
                packetCounter = candidate + static_cast<uint64>(nFrameForSync * counterStep);

            }

            nextPacketCheck = ((packetCounter - counterStep) + checkCounterAfterNSteps);
            //before the first packet, check each packet
            if (acquireFromCounter > 0ull) {

                //REPORT_ERROR(ErrorManagement::Information, "Here");
                /*lint -e{340} -e{928} -e{927} -e{826} -e{740} Allowed cast from pointer to pointer*/
                /*lint -e{613} NULL pointer checked*/
                if (niDeviceOperator->Compare(reinterpret_cast<uint8*>(&(middleBuffer[0])), reinterpret_cast<uint8*>((&acquireFromCounter))) != 0) {

                    nextPacketCheck = packetCounter;
                    //return without adding nothing to the circular buffer
                    sizeToRead = 0u;
                }
                else {
                    acquireFromCounter = 0ull;
                }
            }
        }
    }

    //static uint32 max = 200;
    if (ret) {
        /*lint -e{613} NULL pointer checked*/
        (void) MemoryOperationsHelper::Copy(bufferToFill, &(middleBuffer[0]), sizeToRead);
        if (numberOfInterleavedSamples[signalIdx] == 0u) {
            /*lint -e{613} NULL pointer checked*/
            (void) MemoryOperationsHelper::Copy(bufferToFill, &(middleBuffer[0]), sizeToRead);
        }
        else {
            uint32 cnt = 0u;
            for (uint32 x = 0u; x < signalIdx; x++) {
                cnt += numberOfInterleavedSamples[x];
            }

            /*lint -e{613} NULL pointer checked*/
            NI9157MemoryOperationsHelper::InterleavedToFlat(&(middleBuffer[0]), reinterpret_cast<uint8*>(bufferToFill), cnt, &interleavedPacketMemberByteSize[0], interleavedSignalByteSize[signalIdx],
                                                            numberOfInterleavedSignalMembers[signalIdx], numberOfInterleavedSamples[signalIdx]);

        }
    }
    return ret;
}
CLASS_REGISTER(NI9157CircularFifoReader, "1.0")
}
