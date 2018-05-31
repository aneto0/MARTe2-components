/**
 * @file Interleaved2FlatGAMTest.cpp
 * @brief Source file for class Interleaved2FlatGAMTest
 * @date 24/04/2018
 * @author Giuseppe Ferro
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
 * the class Interleaved2FlatGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "Interleaved2FlatGAMTest.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Provide access to the private members of the Interleaved2FlatGAM for testing.
 */
class Interleaved2FlatGAMTestGAM: public Interleaved2FlatGAM {
public:

    CLASS_REGISTER_DECLARATION()

Interleaved2FlatGAMTestGAM    ();

    virtual ~Interleaved2FlatGAMTestGAM();

    uint32 *GetNumberOfInputSamples();

    uint32 *GetNumberOfOutputSamples();

    uint32 *GetInputByteSize();

    uint32 *GetOutputByteSize();

    uint32 *GetNumberOfInputPacketChunks();

    uint32 *GetNumberOfOutputPacketChunks();

    uint32 *GetPacketInputChunkSize();

    uint32 *GetPacketOutputChunkSize();

    uint32 GetTotalSignalsByteSize();

    void *GetInputSignalsMemory1();

    void *GetOutputSignalsMemory1();
};

Interleaved2FlatGAMTestGAM::Interleaved2FlatGAMTestGAM() {

}

Interleaved2FlatGAMTestGAM::~Interleaved2FlatGAMTestGAM() {

}

uint32 *Interleaved2FlatGAMTestGAM::GetNumberOfInputSamples() {
    return numberOfInputSamples;
}

uint32 *Interleaved2FlatGAMTestGAM::GetNumberOfOutputSamples() {
    return numberOfOutputSamples;
}

uint32 *Interleaved2FlatGAMTestGAM::GetInputByteSize() {
    return inputByteSize;
}

uint32 *Interleaved2FlatGAMTestGAM::GetOutputByteSize() {
    return outputByteSize;
}

uint32 *Interleaved2FlatGAMTestGAM::GetNumberOfInputPacketChunks() {
    return numberOfInputPacketChunks;
}

uint32 *Interleaved2FlatGAMTestGAM::GetNumberOfOutputPacketChunks() {
    return numberOfOutputPacketChunks;
}

uint32 *Interleaved2FlatGAMTestGAM::GetPacketInputChunkSize() {
    return packetInputChunkSize;
}

uint32 *Interleaved2FlatGAMTestGAM::GetPacketOutputChunkSize() {
    return packetOutputChunkSize;
}

uint32 Interleaved2FlatGAMTestGAM::GetTotalSignalsByteSize() {
    return totalSignalsByteSize;
}

void *Interleaved2FlatGAMTestGAM::GetInputSignalsMemory1() {
    return GAM::GetInputSignalsMemory();
}

void *Interleaved2FlatGAMTestGAM::GetOutputSignalsMemory1() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(Interleaved2FlatGAMTestGAM, "1.0")

/**
 * @brief DataSource to generate memory pattern.
 */
class Interleaved2FlatGAMTestDS: public DataSourceI {

public:
    CLASS_REGISTER_DECLARATION()

Interleaved2FlatGAMTestDS    () {
        mem = NULL_PTR(void *);
        inputSignal1Address = NULL_PTR(void *);
        inputSignal2Address = NULL_PTR(void *);
        inputSignalsSize = 0u;
    }

    virtual ~Interleaved2FlatGAMTestDS() {
        if (mem != NULL_PTR(void *)) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(mem);
        }
    }

    virtual bool AllocateMemory() {
        uint32 size;
        //Interleaved Packet
        GetSignalByteSize(0u, size);
        const uint32 inputNumberOfSamples = 3u;

        inputSignalsSize = inputNumberOfSamples * size;
        //Two other signals
        GetSignalByteSize(2u, size);
        inputSignalsSize += size;
        GetSignalByteSize(3u, size);
        inputSignalsSize += size;

        uint32 outputSignalSize;
        GetSignalByteSize(1u, outputSignalSize);
        const uint32 outputNumberOfSamples = 2u;
        outputSignalSize *= outputNumberOfSamples;

        mem = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(inputSignalsSize + outputSignalSize);
        //fill the packets for 3 samples
        uint8 k = 0u;
        uint64 pcounter = 0ull;
        uint16 *ptr16 = (uint16 *) mem;

        for (uint32 i = 0u; i < inputNumberOfSamples; i++) {
            uint64* ptr64 = (uint64*) ptr16;
            *ptr64 = pcounter;
            pcounter++;
            ptr64++;
            ptr16 = (uint16 *) ptr64;
            for (uint32 j = 0u; j < 10; j++) {
                *ptr16 = k;
                ptr16++;
                k++;
            }
            uint32 *ptr32 = (uint32 *) ptr16;
            for (uint32 j = 0u; j < 2; j++) {
                *ptr32 = k;
                ptr32++;
                k++;
            }

            uint8 *ptr8 = (uint8 *) ptr32;
            for (uint32 j = 0u; j < 2; j++) {
                *ptr8 = k;
                ptr8++;
                k++;
            }

            ptr16 = (uint16 *) ptr8;
            *ptr16 = k;
            ptr16++;
            k++;
        }
        //Ignore the output signal
        uint8* ptr8 = (uint8*) ptr16;
        ptr8 += outputSignalSize;

        //second signal and third signal
        uint64* ptr64 = (uint64*) ptr8;
        inputSignal1Address = static_cast<void *>(ptr64);
        uint32 elements;
        GetSignalNumberOfElements(2, elements);
        uint32 counter = 104;
        for (uint32 z = 0u; z < elements; z++) {
            *ptr64 = counter;
            counter++;
            ptr64++;
        }
        GetSignalNumberOfElements(3, elements);
        inputSignal2Address = static_cast<void *>(ptr64);
        for (uint32 z = 0u; z < elements; z++) {
            *ptr64 = counter;
            counter++;
            ptr64++;
        }

        return true;
    }

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName) {
        return true;
    }

    virtual bool Synchronise() {
        return true;
    }

    virtual uint32 GetNumberOfMemoryBuffers() {
        return 1;
    }

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress) {
        char8 *mem8 = reinterpret_cast<char8 *>(mem);
        if (signalIdx == 0u) {
            signalAddress = &mem8[0];
        }
        else if (signalIdx == 1u) {
            signalAddress = &mem8[inputSignalsSize];
        }
        else if (signalIdx == 2u) {
            signalAddress = inputSignal1Address;
        }
        else {
            signalAddress = inputSignal2Address;
        }
        return true;
    }

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction) {
        if (direction == InputSignals) {
            return "MemoryMapInputBroker";
        }
        return "MemoryMapOutputBroker";
    }

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        ReferenceT < MemoryMapInputBroker > broker("MemoryMapInputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            broker->Init(InputSignals, *this, functionName, gamMemPtr);
        }
        if (ret) {
            ret = inputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        ReferenceT < MemoryMapOutputBroker > broker("MemoryMapOutputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            broker->Init(OutputSignals, *this, functionName, gamMemPtr);
        }
        if (ret) {
            ret = outputBrokers.Insert(broker);
        }
        return ret;
    }

private:
    void *mem;
    void *inputSignal1Address;
    void *inputSignal2Address;
    uint32 inputSignalsSize;
};

CLASS_REGISTER(Interleaved2FlatGAMTestDS, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

static const char8 * const config = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = Interleaved2FlatGAMTestGAM"
        "            InputSignals = {"
        "               Signal = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 40"
        "                   Samples = 3"
        "                   Frequency = 1.0"
        "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
        "               }"
        "               Signal2_1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               PacketCounter = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS4 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS5 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS6 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS7 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS8 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS9 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS10 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS11 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS12 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS13 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS14 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS15 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 16"
        "                   Samples = 2"
        "                   PacketMemberSizes = {8, 8}"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = Interleaved2FlatGAMTestDS"
        "            Signals = {"
        "               Signal = {"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 40"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 16"
        "                   Samples = 2"
        "               }"
        "               Signal2_1 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMScheduler"
        "    }"
        "}";

static const char8 * const config2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = Interleaved2FlatGAMTestGAM"
        "            InputSignals = {"
        "               Signal = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 40"
        "                   Samples = 3"
        "                   Frequency = 1.0"
        "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               PacketCounter = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS4 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS5 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS6 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS7 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS8 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS9 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS10 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS11 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS12 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS13 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS14 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS15 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = Interleaved2FlatGAMTestDS"
        "            Signals = {"
        "               Signal = {"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 40"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 16"
        "                   Samples = 2"
        "               }"
        "               Signal2_1 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMScheduler"
        "    }"
        "}";

static const char8 * const config3 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = Interleaved2FlatGAMTestGAM"
        "            InputSignals = {"
        "               Signal2_1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "                   Samples = 2"
        "                   PacketMemberSizes = {8, 8}"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = Interleaved2FlatGAMTestDS"
        "            Signals = {"
        "               Signal = {"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 40"
        "               }"
        "               Signal2 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_1 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMScheduler"
        "    }"
        "}";


static const char8 * const config4 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = Interleaved2FlatGAMTestGAM"
        "            InputSignals = {"
        "               Signal = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 120"
        "                   Frequency = 1.0"
        "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
        "               }"
        "               Signal2_1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               PacketCounter = {"
        "                   DataSource = DDB1"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS4 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS5 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS6 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS7 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS8 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS9 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS10 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS11 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS12 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS13 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS14 = {"
        "                   DataSource = DDB1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               PacketS15 = {"
        "                   DataSource = DDB1"
        "                   Type = uint16"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 3"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 16"
        "                   Samples = 2"
        "                   PacketMemberSizes = {8, 8}"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = Interleaved2FlatGAMTestDS"
        "            Signals = {"
        "               Signal = {"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 120"
        "               }"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = uint8"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 16"
        "                   Samples = 2"
        "               }"
        "               Signal2_1 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "               Signal2_2 = {"
        "                   Type = uint64"
        "                   NumberOfDimensions = 1"
        "                   NumberOfElements = 2"
        "               }"
        "            }"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMScheduler"
        "    }"
        "}";

static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

Interleaved2FlatGAMTest::Interleaved2FlatGAMTest() {
}

Interleaved2FlatGAMTest::~Interleaved2FlatGAMTest() {
}

bool Interleaved2FlatGAMTest::TestConstructor() {
    Interleaved2FlatGAMTestGAM gam;

    bool ret = gam.GetNumberOfInputSamples() == NULL;
    ret &= gam.GetNumberOfOutputSamples() == NULL;
    ret &= gam.GetInputByteSize() == NULL;
    ret &= gam.GetOutputByteSize() == NULL;
    ret &= gam.GetNumberOfInputPacketChunks() == NULL;
    ret &= gam.GetNumberOfOutputPacketChunks() == NULL;
    ret &= gam.GetPacketInputChunkSize() == NULL;
    ret &= gam.GetPacketOutputChunkSize() == NULL;
    ret &= gam.GetTotalSignalsByteSize() == 0;

    return ret;

}

bool Interleaved2FlatGAMTest::TestSetup() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<Interleaved2FlatGAMTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<Interleaved2FlatGAMTestGAM> gam;

    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {

        {
            uint32* val = gam->GetNumberOfInputSamples();
            const uint32 testVal[] = { 3, 1, 1, 0xFFFFFFFF };
            uint32 i = 0u;
            while (testVal[i] != 0xFFFFFFFF) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetNumberOfOutputSamples();
            const uint32 testVal[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetInputByteSize();
            const uint32 testVal[] = { 40, 16, 16, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetOutputByteSize();
            const uint32 testVal[] = { 24, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 12, 12, 3, 3, 6, 16, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetNumberOfInputPacketChunks();
            const uint32 testVal[] = { 16, 0, 0, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetNumberOfOutputPacketChunks();
            const uint32 testVal[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetPacketInputChunkSize();
            const uint32 testVal[] = { 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetPacketOutputChunkSize();
            const uint32 testVal[] = { 8, 8, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        ret &= gam->GetTotalSignalsByteSize() == 152;

    }
    return ret;
}

bool Interleaved2FlatGAMTest::TestSetup_MultiPacketNoSamples() {

    static const char8 * const config1 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 120"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS15 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "                   PacketMemberSizes = {8, 8}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "            Signals = {"
            "               Signal = {"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 120"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);

    ReferenceT<Interleaved2FlatGAMTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<Interleaved2FlatGAMTestGAM> gam;

    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {

        {
            uint32* val = gam->GetNumberOfInputSamples();
            const uint32 testVal[] = { 3, 1, 1, 0xFFFFFFFF };
            uint32 i = 0u;
            while (testVal[i] != 0xFFFFFFFF) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetNumberOfOutputSamples();
            const uint32 testVal[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetInputByteSize();
            const uint32 testVal[] = { 40, 16, 16, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetOutputByteSize();
            const uint32 testVal[] = { 24, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 12, 12, 3, 3, 6, 16, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetNumberOfInputPacketChunks();
            const uint32 testVal[] = { 16, 0, 0, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetNumberOfOutputPacketChunks();
            const uint32 testVal[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetPacketInputChunkSize();
            const uint32 testVal[] = { 8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        if (ret) {
            uint32* val = gam->GetPacketOutputChunkSize();
            const uint32 testVal[] = { 8, 8, 0xFFFFFFFF };
            uint32 i = 0u;
            while ((testVal[i] != 0xFFFFFFFF) && (ret)) {
                ret = (val[i] == testVal[i]);
                i++;
            }
        }

        ret &= gam->GetTotalSignalsByteSize() == 152;

    }
    return ret;
}

bool Interleaved2FlatGAMTest::TestSetup_False_SignalOverlap_In() {

    static const char8 * const config7 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 11"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "                   Samples = 3"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "                   PacketMemberSizes = {8, 8}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";

    return !InitialiseMemoryMapInputBrokerEnviroment(config7);
}

bool Interleaved2FlatGAMTest::TestSetup_False_DoublePacketDef_In() {

    static const char8 * const config8 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "                   Samples = 3"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "                   PacketMemberSizes = {8}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS15 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "                   PacketMemberSizes = {8, 8}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "            Signals = {"
            "               Signal = {"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";
    return !InitialiseMemoryMapInputBrokerEnviroment(config8);

}

bool Interleaved2FlatGAMTest::TestSetup_False_SignalOverlap_Out() {
    static const char8 * const config2 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 120"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS15 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "            Signals = {"
            "               Signal = {"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 120"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";

    return !InitialiseMemoryMapInputBrokerEnviroment(config2);

}

bool Interleaved2FlatGAMTest::TestSetup_False_DoublePacketDef_Out() {
    static const char8 * const config3 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "                   Samples = 3"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "                   PacketMemberSizes = {4, 4, 4}"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS15 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "                   PacketMemberSizes = {8, 8}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "            Signals = {"
            "               Signal = {"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";
    return !InitialiseMemoryMapInputBrokerEnviroment(config3);

}

bool Interleaved2FlatGAMTest::TestSetup_False_DifferentIOSizes() {
    static const char8 * const config4 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "                   Samples = 3"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS15 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 3"
            "                   PacketMemberSizes = {8, 8}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "            Signals = {"
            "               Signal = {"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";
    return !InitialiseMemoryMapInputBrokerEnviroment(config4);

}

bool Interleaved2FlatGAMTest::TestSetup_False_DifferentPacketSize_In() {

    static const char8 * const config5 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "                   Samples = 3"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 1}"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS15 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "                   PacketMemberSizes = {8, 8}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "            Signals = {"
            "               Signal = {"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";
    return !InitialiseMemoryMapInputBrokerEnviroment(config5);
}

bool Interleaved2FlatGAMTest::TestSetup_False_DifferentPacketSize_Out() {
    static const char8 * const config6 = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = Interleaved2FlatGAMTestGAM"
            "            InputSignals = {"
            "               Signal = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "                   Samples = 3"
            "                   Frequency = 1.0"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               Signal2_1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "               Signal2_2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               PacketCounter = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS1 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS2 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS3 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS4 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS5 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS6 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS7 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS8 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS9 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS10 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS11 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS12 = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS13 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS14 = {"
            "                   DataSource = DDB1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               PacketS15 = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "                   PacketMemberSizes = {8, 9}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = Interleaved2FlatGAMTestDS"
            "            Signals = {"
            "               Signal = {"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 40"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 16"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMScheduler"
            "    }"
            "}";
    return !InitialiseMemoryMapInputBrokerEnviroment(config6);
}

bool Interleaved2FlatGAMTest::TestExecute() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<Interleaved2FlatGAMTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<Interleaved2FlatGAMTestGAM> gam;

    if (ret) {

        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        ReferenceT<BrokerI> broker;

        if (ret) {
            for (uint32 i = 0u; (i < inputBrokers.Size()) && (ret); i++) {
                broker = inputBrokers.Get(i);
                ret = broker.IsValid();
                if (ret) {
                    ret = broker->Execute();
                }
            }
        }
        if (ret) {
            ret = gam->Execute();
        }

        if (ret) {
            uint8* mem = (uint8*) gam->GetOutputSignalsMemory1();
            //the packet size
            ret = (*(uint64*) mem == 0);
            mem += 8;
            ret &= (*(uint64*) mem == 1);
            mem += 8;
            ret &= (*(uint64*) mem == 2);
            mem += 8;

            for (uint32 k = 0u; k < 10; k++) {
                ret = (*(uint16*) mem == k);
                mem += 2;
                ret &= (*(uint16*) mem == k + 15);
                mem += 2;
                ret &= (*(uint16*) mem == k + 30);
                mem += 2;
            }

            for (uint32 k = 10u; k < 12; k++) {
                ret = (*(uint32*) mem == k);
                mem += 4;
                ret &= (*(uint32*) mem == k + 15);
                mem += 4;
                ret &= (*(uint32*) mem == k + 30);
                mem += 4;
            }

            for (uint32 k = 12u; k < 14; k++) {
                ret = (*(uint8*) mem == k);
                mem += 1;
                ret &= (*(uint8*) mem == k + 15);
                mem += 1;
                ret &= (*(uint8*) mem == k + 30);
                mem += 1;
            }

            ret = (*(uint16*) mem == 14);
            mem += 2;
            ret &= (*(uint16*) mem == 29);
            mem += 2;
            ret &= (*(uint16*) mem == 44);
            mem += 2;

            uint64 *mem2 = (uint64*) mem;
            ret &= (mem2[0] == 104);
            ret &= (mem2[1] == 106);
            ret &= (mem2[2] == 105);
            ret &= (mem2[3] == 107);
        }
    }
    return ret;
}


bool Interleaved2FlatGAMTest::TestExecute_Interleaved2FlatOnly() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config2);

    ReferenceT<Interleaved2FlatGAMTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<Interleaved2FlatGAMTestGAM> gam;

    if (ret) {

        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        ReferenceT<BrokerI> broker;

        if (ret) {
            for (uint32 i = 0u; (i < inputBrokers.Size()) && (ret); i++) {
                broker = inputBrokers.Get(i);
                ret = broker.IsValid();
                if (ret) {
                    ret = broker->Execute();
                }
            }
        }
        if (ret) {
            ret = gam->Execute();
        }

        if (ret) {
            uint8* mem = (uint8*) gam->GetOutputSignalsMemory1();
            //the packet size
            ret = (*(uint64*) mem == 0);
            mem += 8;
            ret &= (*(uint64*) mem == 1);
            mem += 8;
            ret &= (*(uint64*) mem == 2);
            mem += 8;

            for (uint32 k = 0u; k < 10; k++) {
                ret = (*(uint16*) mem == k);
                mem += 2;
                ret &= (*(uint16*) mem == k + 15);
                mem += 2;
                ret &= (*(uint16*) mem == k + 30);
                mem += 2;
            }

            for (uint32 k = 10u; k < 12; k++) {
                ret = (*(uint32*) mem == k);
                mem += 4;
                ret &= (*(uint32*) mem == k + 15);
                mem += 4;
                ret &= (*(uint32*) mem == k + 30);
                mem += 4;
            }

            for (uint32 k = 12u; k < 14; k++) {
                ret = (*(uint8*) mem == k);
                mem += 1;
                ret &= (*(uint8*) mem == k + 15);
                mem += 1;
                ret &= (*(uint8*) mem == k + 30);
                mem += 1;
            }

            ret = (*(uint16*) mem == 14);
            mem += 2;
            ret &= (*(uint16*) mem == 29);
            mem += 2;
            ret &= (*(uint16*) mem == 44);
        }
    }
    return ret;
}

bool Interleaved2FlatGAMTest::TestExecute_Flat2InterleavedOnly() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config3);

    ReferenceT<Interleaved2FlatGAMTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<Interleaved2FlatGAMTestGAM> gam;

    if (ret) {

        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        ReferenceT<BrokerI> broker;

        if (ret) {
            for (uint32 i = 0u; (i < inputBrokers.Size()) && (ret); i++) {
                broker = inputBrokers.Get(i);
                ret = broker.IsValid();
                if (ret) {
                    ret = broker->Execute();
                }
            }
        }
        if (ret) {
            ret = gam->Execute();
        }

        if (ret) {
            uint64* mem = (uint64*) gam->GetOutputSignalsMemory1();
            ret &= (mem[0] == 104);
            ret &= (mem[1] == 106);
            ret &= (mem[2] == 105);
            ret &= (mem[3] == 107);
        }
    }
    return ret;
}

bool Interleaved2FlatGAMTest::TestExecute_MultiPacketNoSamples() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config4);

    ReferenceT<Interleaved2FlatGAMTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ReferenceT<Interleaved2FlatGAMTestGAM> gam;

    if (ret) {

        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        ReferenceT<BrokerI> broker;

        if (ret) {
            for (uint32 i = 0u; (i < inputBrokers.Size()) && (ret); i++) {
                broker = inputBrokers.Get(i);
                ret = broker.IsValid();
                if (ret) {
                    ret = broker->Execute();
                }
            }
        }
        if (ret) {
            ret = gam->Execute();
        }

        if (ret) {
            uint8* mem = (uint8*) gam->GetOutputSignalsMemory1();
            //the packet size
            ret = (*(uint64*) mem == 0);
            mem += 8;
            ret &= (*(uint64*) mem == 1);
            mem += 8;
            ret &= (*(uint64*) mem == 2);
            mem += 8;

            for (uint32 k = 0u; k < 10; k++) {
                ret = (*(uint16*) mem == k);
                mem += 2;
                ret &= (*(uint16*) mem == k + 15);
                mem += 2;
                ret &= (*(uint16*) mem == k + 30);
                mem += 2;
            }

            for (uint32 k = 10u; k < 12; k++) {
                ret = (*(uint32*) mem == k);
                mem += 4;
                ret &= (*(uint32*) mem == k + 15);
                mem += 4;
                ret &= (*(uint32*) mem == k + 30);
                mem += 4;
            }

            for (uint32 k = 12u; k < 14; k++) {
                ret = (*(uint8*) mem == k);
                mem += 1;
                ret &= (*(uint8*) mem == k + 15);
                mem += 1;
                ret &= (*(uint8*) mem == k + 30);
                mem += 1;
            }

            ret = (*(uint16*) mem == 14);
            mem += 2;
            ret &= (*(uint16*) mem == 29);
            mem += 2;
            ret &= (*(uint16*) mem == 44);
            mem += 2;

            uint64 *mem2 = (uint64*) mem;
            ret &= (mem2[0] == 104);
            ret &= (mem2[1] == 106);
            ret &= (mem2[2] == 105);
            ret &= (mem2[3] == 107);
        }
    }
    return ret;
}
