/**
 * @file RealTimeThreadAsyncBridgeTest.cpp
 * @brief Source file for class RealTimeThreadAsyncBridgeTest
 * @date 25/04/2018
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
 * the class RealTimeThreadAsyncBridgeTest (public, protected, and private). Be aware that some 
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
#include "FastPollingEventSem.h"
#include "GAMScheduler.h"
#include "GAMSchedulerI.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "RealTimeThreadAsyncBridgeTest.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Helper DataSource to expose and test protected methods of RealTimeThreadAsyncBridge.
 */
class RealTimeThreadAsyncBridgeTestDS: public RealTimeThreadAsyncBridge {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadAsyncBridgeTestDS    ();

    virtual ~RealTimeThreadAsyncBridgeTestDS();

    volatile int32 *GetSpinlocksRead();

    FastPollingMutexSem *GetSpinlocksWrite();

    EventSem *GetEventRead();

    EventSem *GetEventWrite();

    uint32 *GetWhatIsNewestCounter();

    virtual void PrepareInputOffsets();

    virtual void PrepareOutputOffsets();

    void SetNewestCounter(uint32 newestCounter, uint32 signalIdx);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    void Done();

    EventSem goSemRead;
    EventSem goSemWrite;
    volatile int32 cnt;
    bool done;

};

RealTimeThreadAsyncBridgeTestDS::RealTimeThreadAsyncBridgeTestDS() {
    goSemRead.Create();
    goSemWrite.Create();
    cnt = -1;
    done = false;
}

void RealTimeThreadAsyncBridgeTestDS::SetNewestCounter(uint32 newestCounter, uint32 signalIdx) {
    whatIsNewestGlobCounter[signalIdx] = newestCounter;

    for (uint32 i = 0u; i < numberOfBuffers; i++) {
        uint32 index = signalIdx * numberOfBuffers + i;
        whatIsNewestCounter[index] = newestCounter;
    }

}

void RealTimeThreadAsyncBridgeTestDS::Done() {
    done = true;
}

RealTimeThreadAsyncBridgeTestDS::~RealTimeThreadAsyncBridgeTestDS() {
}

EventSem *RealTimeThreadAsyncBridgeTestDS::GetEventRead() {
    return &goSemRead;
}

EventSem *RealTimeThreadAsyncBridgeTestDS::GetEventWrite() {
    return &goSemWrite;
}

void RealTimeThreadAsyncBridgeTestDS::PrepareInputOffsets() {
    if (!done) {
        goSemRead.ResetWait(TTInfiniteWait);
    }

}

void RealTimeThreadAsyncBridgeTestDS::PrepareOutputOffsets() {
    if (!done) {
        goSemWrite.ResetWait(TTInfiniteWait);
    }
}

volatile int32 *RealTimeThreadAsyncBridgeTestDS::GetSpinlocksRead() {
    return spinlocksRead;
}
FastPollingMutexSem *RealTimeThreadAsyncBridgeTestDS::GetSpinlocksWrite() {
    return spinlocksWrite;
}
uint32 *RealTimeThreadAsyncBridgeTestDS::GetWhatIsNewestCounter() {
    return whatIsNewestCounter;
}

const char8 *RealTimeThreadAsyncBridgeTestDS::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    const char8* brokerName = NULL_PTR(const char8 *);

    if (direction == InputSignals) {
        brokerName = "RealTimeThreadAsyncBridgeTestInputBroker";
    }

    if (direction == OutputSignals) {
        brokerName = "RealTimeThreadAsyncBridgeTestOutputBroker";
    }

    return brokerName;
}

CLASS_REGISTER(RealTimeThreadAsyncBridgeTestDS, "1.0")

class RealTimeThreadAsyncBridgeTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadAsyncBridgeTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();
    void *GetInputMemoryBuffer();

    void *GetOutputMemoryBuffer();
private:
};

RealTimeThreadAsyncBridgeTestGAM1::RealTimeThreadAsyncBridgeTestGAM1() :
        GAM() {
}

bool RealTimeThreadAsyncBridgeTestGAM1::Setup() {
    return true;
}

bool RealTimeThreadAsyncBridgeTestGAM1::Execute() {

    return true;
}

void *RealTimeThreadAsyncBridgeTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *RealTimeThreadAsyncBridgeTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(RealTimeThreadAsyncBridgeTestGAM1, "1.0")

class RealTimeThreadAsyncBridgeTestGAMWriter: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadAsyncBridgeTestGAMWriter    ();

    virtual bool Setup();

    virtual bool Execute();
    void *GetInputMemoryBuffer();

    void *GetOutputMemoryBuffer();
private:
    uint32 *inputMem;
    uint32 totalSize;
};

RealTimeThreadAsyncBridgeTestGAMWriter::RealTimeThreadAsyncBridgeTestGAMWriter() :
        GAM() {
    totalSize = 0u;
    inputMem = NULL;
}

bool RealTimeThreadAsyncBridgeTestGAMWriter::Setup() {
    inputMem = (uint32 *) GetOutputSignalsMemory();
    bool ret = GetNumberOfInputSignals() == 0;

    if (ret) {
        ret = inputMem != NULL;
    }
    if (ret) {
        uint32 nSignals = GetNumberOfOutputSignals();
        for (uint32 i = 0u; (i < nSignals) && (ret); i++) {
            uint32 byteSize = 0u;
            ret = GetSignalByteSize(OutputSignals, i, byteSize);
            if (ret) {
                totalSize += byteSize;
            }
        }
    }

    if (ret) {
        MemoryOperationsHelper::Set(inputMem, '\0', totalSize);
        uint32 nElements = totalSize / 4;
        for (uint32 i = 0u; i < nElements; i++) {
            inputMem[i] = i;
        }
    }

    return ret;
}

bool RealTimeThreadAsyncBridgeTestGAMWriter::Execute() {
    uint32 nElements = totalSize / 4;
    for (uint32 i = 0u; i < nElements; i++) {
        inputMem[i]++;
    }
    return true;
}

void *RealTimeThreadAsyncBridgeTestGAMWriter::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *RealTimeThreadAsyncBridgeTestGAMWriter::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(RealTimeThreadAsyncBridgeTestGAMWriter, "1.0")

class RealTimeThreadAsyncBridgeTestInputBroker: public MemoryMapMultiBufferBroker {
public:
    CLASS_REGISTER_DECLARATION()RealTimeThreadAsyncBridgeTestInputBroker();

    virtual ~RealTimeThreadAsyncBridgeTestInputBroker();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void * const gamMemoryAddress);
    int32 GetOffset(uint32 n);

    void SetSignalPostMiddle(uint32 signalIdx);

    int32 *myOffset;
    EventSem *goSemRead;
    uint32 signalPostMiddle;
};

RealTimeThreadAsyncBridgeTestInputBroker::RealTimeThreadAsyncBridgeTestInputBroker() {
    myOffset = NULL;
    goSemRead = NULL;
    signalPostMiddle = 0xFFFFFFFF;
}

RealTimeThreadAsyncBridgeTestInputBroker::~RealTimeThreadAsyncBridgeTestInputBroker() {
    if (myOffset != NULL) {
        delete[] myOffset;
    }
}

void RealTimeThreadAsyncBridgeTestInputBroker::SetSignalPostMiddle(uint32 signalIdx) {
    signalPostMiddle = signalIdx;
}

bool RealTimeThreadAsyncBridgeTestInputBroker::Init(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName,
                                                    void * const gamMemoryAddress) {
    bool ret = MemoryMapMultiBufferBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    myOffset = new int32[numberOfCopies];
    goSemRead = ((RealTimeThreadAsyncBridgeTestDS*) dataSource)->GetEventRead();

    return ret;
}

bool RealTimeThreadAsyncBridgeTestInputBroker::Execute() {
    dataSource->PrepareInputOffsets();

    uint32 n;
    bool ret = true;
    uint32 currentBuffer = dataSource->GetCurrentStateBuffer();

    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            uint32 ioffset = 0u;
            if (signalIdxArr[n] == signalPostMiddle) {
                goSemRead->ResetWait(TTInfiniteWait);
            }
            bool ok = dataSource->GetInputOffset(signalIdxArr[n], samples[n], ioffset);
            if (ok) {
                uint32 dataSourceIndex = ((currentBuffer * numberOfCopies) + n);
                int32 allowedSize = static_cast<int32>(copyTable[n].copySize);
                uint32 gamOffset = 0u;
                int32 copyOffsetN = static_cast<int32>(GetCopyOffset(n));
                ioffset %= maxOffset[n];

                //Trap the circular buffer exceptions
                //Note that the offset is allowed to be negative, meaning that it has to copy N bytes before the current position of the dataSourcePointer
                int32 offset = static_cast<int32>(ioffset);
                //copyOffsetN will take into account the number of samples to be copied. There will be one copy for every sample and some of the samples might be requesting ranges that require the circular buffer to restart
                //Copy would go over boundary? This is only possible if the number of samples is > than the number of memory buffers and assumes that a circular buffer is required.
                //Only do (at most) one of the following whiles
                bool offsetChanged = false;
                while ((copyOffsetN + offset) >= maxOffset[n]) {
                    offset = (offset - maxOffset[n]);
                    offsetChanged = true;
                }
                if (!offsetChanged) {
                    //Circular buffer where one might need to copy the a sub-buffer from the end, N buffers in the middle and the reminder at the end again
                    if (allowedSize > (maxOffset[n] - (copyOffsetN + offset))) {
                        //allowedSize is the number of bytes that are to be copied from the data source: N samples x size of the type to be copied
                        //maxOffset is the maximum number of bytes that can be copied from the data source at a time: Number of buffers x size of the type to be copied. If the allowedSize is greater than the maxOffset, the copy must restart from the beginning of the memory
                        //overSize if how much bigger is the allowedSize w.r.t. to the overSize (e.g. copy 3 samples of an uint32 => allowedSize = 12, dual buffer in the data source => 2 x 4 = 8) => overSize = 4
                        int32 overSize = allowedSize % maxOffset[n];

                        //The allowedSize may be many times bigger than the maxOffset, so that the copy of all the data source memory may need to be performed many times
                        uint32 numberOfFullCopies = ((allowedSize - overSize) / maxOffset[n]);
                        bool isMultiple = (((allowedSize - overSize) % maxOffset[n]) == 0) && (allowedSize > overSize);

                        //If the numberOfFullCopies is a multiple do one less copy, because at the end
                        if (isMultiple) {
                            overSize += maxOffset[n];
                            numberOfFullCopies--;
                        }
                        uint32 allowedSizePhase = 0u;
                        if (overSize > (maxOffset[n] - (copyOffsetN + offset))) {
                            allowedSizePhase = (maxOffset[n] - offset);

                            (void) MemoryOperationsHelper::Copy(&(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]),
                                                                &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]),
                                                                allowedSizePhase);
                            gamOffset = allowedSizePhase;
                            offset = 0;
                        }

                        for (uint32 z = 0u; z < numberOfFullCopies; z++) {
                            (void) MemoryOperationsHelper::Copy(&(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]),
                                                                &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]),
                                                                maxOffset[n]);

                            gamOffset += maxOffset[n];
                        }
                        allowedSize = allowedSize - (maxOffset[n] * numberOfFullCopies) - allowedSizePhase;
                    }
                }
                (void) MemoryOperationsHelper::Copy(&(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]),
                                                    &((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]), allowedSize);
                if (signalIdxArr[n] == signalPostMiddle) {
                    goSemRead->ResetWait(TTInfiniteWait);
                }

                /*lint -e{613} null pointer checked before.*/
                ret = dataSource->TerminateInputCopy(signalIdxArr[n], ioffset, samples[n]);
                myOffset[n] = offset;
            }
            else {
                myOffset[n] = 0xFFFFFFFF;
            }

        }
    }
    return ret;
}

int32 RealTimeThreadAsyncBridgeTestInputBroker::GetOffset(uint32 n) {
    return myOffset[n];
}
CLASS_REGISTER(RealTimeThreadAsyncBridgeTestInputBroker, "1.0")

class RealTimeThreadAsyncBridgeTestOutputBroker: public MemoryMapMultiBufferBroker {
public:
    CLASS_REGISTER_DECLARATION()RealTimeThreadAsyncBridgeTestOutputBroker();

    virtual ~RealTimeThreadAsyncBridgeTestOutputBroker();
    virtual bool Execute();

    virtual bool Init(const SignalDirection direction, DataSourceI &dataSourceIn,
            const char8 * const functionName, void * const gamMemoryAddress);

    void SetSignalPostMiddle(uint32 signalIdx);

    int32 GetOffset(uint32 n);

    int32 *myOffset;
    EventSem *goSemWrite;
    uint32 signalPostMiddle;
};

RealTimeThreadAsyncBridgeTestOutputBroker::RealTimeThreadAsyncBridgeTestOutputBroker() {
    myOffset = NULL;
    goSemWrite = NULL;
    signalPostMiddle = 0xFFFFFFFFu;
}

RealTimeThreadAsyncBridgeTestOutputBroker::~RealTimeThreadAsyncBridgeTestOutputBroker() {
    if (myOffset != NULL) {
        delete[] myOffset;
    }
}

void RealTimeThreadAsyncBridgeTestOutputBroker::SetSignalPostMiddle(uint32 signalIdx) {
    signalPostMiddle = signalIdx;
}

bool RealTimeThreadAsyncBridgeTestOutputBroker::Init(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName,
                                                     void * const gamMemoryAddress) {
    bool ret = MemoryMapMultiBufferBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    myOffset = new int32[numberOfCopies];
    goSemWrite = ((RealTimeThreadAsyncBridgeTestDS*) dataSource)->GetEventWrite();
    return ret;
}

bool RealTimeThreadAsyncBridgeTestOutputBroker::Execute() {
    dataSource->PrepareOutputOffsets();

    uint32 n;
    bool ret = true;
    uint32 currentBuffer = dataSource->GetCurrentStateBuffer();
    if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            uint32 ioffset = 0u;
            if (signalIdxArr[n] == signalPostMiddle) {
                goSemWrite->ResetWait(TTInfiniteWait);
            }
            bool ok = dataSource->GetOutputOffset(signalIdxArr[n], samples[n], ioffset);
            if (ok) {
                uint32 dataSourceIndex = ((currentBuffer * numberOfCopies) + n);
                int32 allowedSize = static_cast<int32>(copyTable[n].copySize);
                uint32 gamOffset = 0u;
                int32 copyOffsetN = static_cast<int32>(GetCopyOffset(n));
                ioffset %= maxOffset[n];
                //Trap the circular buffer exceptions
                //Note that the offset is allowed to be negative, meaning that it has to copy N bytes before the current position of the dataSourcePointer
                int32 offset = static_cast<int32>(ioffset);
                //Only do (at most) one of the following whiles
                bool offsetChanged = false;

                while ((copyOffsetN + offset) >= maxOffset[n]) {
                    offset = (offset - maxOffset[n]);
                    offsetChanged = true;
                }
                if (!offsetChanged) {
                    while (allowedSize > (maxOffset[n] - (copyOffsetN + offset))) {
                        allowedSize = (maxOffset[n] - offset);
                        (void) MemoryOperationsHelper::Copy(&((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]),
                                                            &(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]), allowedSize);
                        gamOffset = allowedSize;
                        offset = (offset - maxOffset[n]) + allowedSize;
                        allowedSize = (copyTable[n].copySize - allowedSize);
                    }
                }

                (void) MemoryOperationsHelper::Copy(&((reinterpret_cast<uint8 *>(copyTable[dataSourceIndex].dataSourcePointer))[offset]),
                                                    &(reinterpret_cast<uint8 *>(copyTable[n].gamPointer)[gamOffset]), allowedSize);
                if (signalIdxArr[n] == signalPostMiddle) {
                    goSemWrite->ResetWait(TTInfiniteWait);
                }
                ret = dataSource->TerminateOutputCopy(signalIdxArr[n], ioffset, samples[n]);
                myOffset[n] = offset;
            }
            else {
                myOffset[n] = 0xFFFFFFFF;
            }
        }
    }
    return ret;
}

int32 RealTimeThreadAsyncBridgeTestOutputBroker::GetOffset(uint32 n) {
    return myOffset[n];
}
CLASS_REGISTER(RealTimeThreadAsyncBridgeTestOutputBroker, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Helper function to setup a MARTe execution environment
 */
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

RealTimeThreadAsyncBridgeTest::RealTimeThreadAsyncBridgeTest() {
// Auto-generated constructor stub for RealTimeThreadAsyncBridgeTest
// TODO Verify if manual additions are needed
}

RealTimeThreadAsyncBridgeTest::~RealTimeThreadAsyncBridgeTest() {
// Auto-generated destructor stub for RealTimeThreadAsyncBridgeTest
// TODO Verify if manual additions are needed
}

bool RealTimeThreadAsyncBridgeTest::TestConstructor() {
    RealTimeThreadAsyncBridgeTestDS dataSource;

    bool ret = true;

    ret = dataSource.GetSpinlocksRead() == NULL;
    ret &= dataSource.GetSpinlocksWrite() == NULL;
    ret &= dataSource.GetWhatIsNewestCounter() == NULL;
    return ret;

}

bool RealTimeThreadAsyncBridgeTest::TestInitialise() {
    RealTimeThreadAsyncBridgeTestDS dataSource;
    const char8 *conf1 = "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 64"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = conf1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveAbsolute("+Drv1");
        ret = dataSource.Initialise(cdb);
    }

    return ret;
}

bool RealTimeThreadAsyncBridgeTest::TestInitialise_False_GreaterNumberOfBuffers() {
    RealTimeThreadAsyncBridgeTestDS dataSource;
    const char8 *conf1 = "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 65"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = conf1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveAbsolute("+Drv1");
        ret = !dataSource.Initialise(cdb);
    }

    return ret;
}

bool RealTimeThreadAsyncBridgeTest::TestSetConfiguredDatabase() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 4"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 64"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        volatile int32 *spinRead = dataSource->GetSpinlocksRead();
        FastPollingMutexSem *spinWrite = dataSource->GetSpinlocksWrite();
        uint32 *newCount = dataSource->GetWhatIsNewestCounter();

        uint32 nElements = dataSource->GetNumberOfSignals() * dataSource->GetNumberOfMemoryBuffers();
        for (uint32 i = 0u; (i < nElements) && (ret); i++) {
            ret &= spinRead[i] == 0;
            ret &= spinWrite[i].FastTryLock();
            spinWrite[i].FastUnLock();
            ret &= newCount[i] == 0;
        }
    }
    return ret;
}

bool RealTimeThreadAsyncBridgeTest::TestGetBrokerName() {
    RealTimeThreadAsyncBridge test;
    ConfigurationDatabase cdb;
    cdb.Write("Frequency", 1.0);
    cdb.Write("Trigger", 1);
    StreamString ret = test.GetBrokerName(cdb, InputSignals);
    bool ok = (ret == "MemoryMapMultiBufferInputBroker");
    ret = test.GetBrokerName(cdb, OutputSignals);
    ok &= (ret == "MemoryMapMultiBufferOutputBroker");
    return ok;
}

bool RealTimeThreadAsyncBridgeTest::TestSetConfiguredDatabase_False_InputSamples() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 2"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 64"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    return !InitialiseMemoryMapInputBrokerEnviroment(config);

}

bool RealTimeThreadAsyncBridgeTest::TestSetConfiguredDatabase_False_OutputSamples() {
    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,0}, {2,2}}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal3 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 2"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 64"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    return !InitialiseMemoryMapInputBrokerEnviroment(config);
}

bool RealTimeThreadAsyncBridgeTest::TestSynchronise() {
    RealTimeThreadAsyncBridge dataSource;
    return dataSource.Synchronise();
}

bool RealTimeThreadAsyncBridgeTest::TestPrepareNextState() {
    RealTimeThreadAsyncBridge dataSource;
    return dataSource.PrepareNextState("", "");
}

bool RealTimeThreadAsyncBridgeTest::TestGetInputOffset() {
    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 3"
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
            "                    CPUs = 2"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 3"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    if (ret) {
        app->StartNextStateExecution();

        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();

        ReferenceContainer inputBrokers;
        gamReader1->GetInputBrokers(inputBrokers);

        ReferenceT<RealTimeThreadAsyncBridgeTestInputBroker> broker = inputBrokers.Get(0);
        ret = broker.IsValid();
        Sleep::MSec(100);

        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();

        for (uint32 cnt = 0u; (cnt < 10 * sizeof(uint32)) && (ret); cnt += sizeof(uint32)) {
            //write

            goWrite->Post();

            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            int32 x = broker->GetOffset(0);
            ret = (cnt % (nBuffers * sizeof(uint32))) == (uint32) x;
        }

        dataSource->Done();
        goWrite->Post();
        goRead->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeThreadAsyncBridgeTest::TestGetOutputOffset() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 3"
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
            "                    CPUs = 1"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 2"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    if (ret) {
        app->StartNextStateExecution();

        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();
        ReferenceContainer outputBrokers;
        gamWriter->GetOutputBrokers(outputBrokers);

        ReferenceT<RealTimeThreadAsyncBridgeTestOutputBroker> broker = outputBrokers.Get(0);
        ret = broker.IsValid();
        Sleep::MSec(100);
        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();
        for (uint32 cnt = 0u; (cnt < 10 * sizeof(uint32)) && (ret); cnt += sizeof(uint32)) {
            //Sleep::MSec(100);
            //write

            goWrite->Post();

            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            int32 x = broker->GetOffset(0);
            ret = (cnt % (nBuffers * sizeof(uint32))) == (uint32) x;
        }

        dataSource->Done();
        goWrite->Post();
        goRead->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool RealTimeThreadAsyncBridgeTest::TestGetInputOffset_2Readers() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +GAMC = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 3"
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
            "                    CPUs = 1"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 2"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "                +Thread3 = {"
            "                    CPUs = 3"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader2;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }

    if (ret) {
        gamReader2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMC");
        ret = gamReader2.IsValid();
    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    if (ret) {
        app->StartNextStateExecution();

        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();

        ReferenceContainer inputBrokers;
        gamReader1->GetInputBrokers(inputBrokers);

        ReferenceT<RealTimeThreadAsyncBridgeTestInputBroker> broker = inputBrokers.Get(0);
        ret = broker.IsValid();

        ReferenceT<RealTimeThreadAsyncBridgeTestInputBroker> broker2;
        if (ret) {
            ReferenceContainer inputBrokers2;
            gamReader2->GetInputBrokers(inputBrokers2);

            broker2 = inputBrokers2.Get(0);
            ret = broker2.IsValid();

        }

        Sleep::MSec(100);
        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();
        
        uint32 nOfRetries = 5;
        for (uint32 cnt = 0u; (cnt < 10 * sizeof(uint32)) && (ret); cnt += sizeof(uint32)) {
            //write
            goWrite->Post();
            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            int32 x = broker->GetOffset(0);
            int32 x2 = broker2->GetOffset(0);
            ret = ((cnt % (nBuffers * sizeof(uint32))) == (uint32) x) || (((cnt - sizeof(uint32)) % (nBuffers * sizeof(uint32))) == (uint32) x);
            ret &= ((cnt % (nBuffers * sizeof(uint32))) == (uint32) x2) || (((cnt - sizeof(uint32)) % (nBuffers * sizeof(uint32))) == (uint32) x2);
            if (!ret) {
                Sleep::MSec(1000);
                nOfRetries--;
                if (nOfRetries > 0) {
                    ret = true;
                    cnt = 0;
                }
            }
        }

        dataSource->Done();
        goWrite->Post();
        goRead->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeThreadAsyncBridgeTest::TestGetInputOffset_Ranges() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,3}, {8,9}}"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 3"
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
            "                    CPUs = 1"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 2"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    if (ret) {
        app->StartNextStateExecution();

        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();

        ReferenceContainer inputBrokers;
        gamReader1->GetInputBrokers(inputBrokers);

        ReferenceT<RealTimeThreadAsyncBridgeTestInputBroker> broker = inputBrokers.Get(0);
        ret = broker.IsValid();
        Sleep::MSec(100);

        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();

        for (uint32 cnt = 0u; (cnt < 100 * sizeof(uint32)) && (ret); cnt += 10 * sizeof(uint32)) {
            //write

            goWrite->Post();

            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            int32 x = broker->GetOffset(1);
            int32 x1 = broker->GetOffset(2);

            ret = (cnt % (nBuffers * 10 * sizeof(uint32))) == (uint32) x;
            ret &= (cnt % (nBuffers * 10 * sizeof(uint32))) == (uint32) x1;
        }

        dataSource->Done();
        goWrite->Post();
        goRead->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeThreadAsyncBridgeTest::TestGetOutputOffset_Ranges() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0,3}, {0,9}}"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 3"
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
            "                    CPUs = 1"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 2"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    if (ret) {
        app->StartNextStateExecution();

        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();
        ReferenceContainer outputBrokers;
        gamWriter->GetOutputBrokers(outputBrokers);

        ReferenceT<RealTimeThreadAsyncBridgeTestOutputBroker> broker = outputBrokers.Get(0);
        ret = broker.IsValid();
        Sleep::MSec(100);

        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();

        for (uint32 cnt = 0u; (cnt < 100 * sizeof(uint32)) && (ret); cnt += 10 * sizeof(uint32)) {
            //write

            goWrite->Post();

            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            int32 x = broker->GetOffset(1);
            int32 x1 = broker->GetOffset(2);

            ret = (cnt % (nBuffers * 10 * sizeof(uint32))) == (uint32) x;
            ret &= (cnt % (nBuffers * 10 * sizeof(uint32))) == (uint32) x1;
        }

        dataSource->Done();
        goWrite->Post();
        goRead->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeThreadAsyncBridgeTest::TestGetInputOffset_False_BufferBusy() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 1"
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
            "                    CPUs = 1"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 2"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestInputBroker> broker;
    if (ret) {
        ReferenceContainer inputBrokers;
        gamReader1->GetInputBrokers(inputBrokers);

        broker = inputBrokers.Get(0);
        ret = broker.IsValid();
        if (ret) {
            broker->SetSignalPostMiddle(0);
        }
    }
    ReferenceT<RealTimeThreadAsyncBridgeTestOutputBroker> broker1;

    if (ret) {
        ReferenceContainer outputBrokers;
        gamWriter->GetOutputBrokers(outputBrokers);

        broker1 = outputBrokers.Get(0);
        ret = broker1.IsValid();
        if (ret) {
            broker1->SetSignalPostMiddle(0);
        }
    }
    if (ret) {
        app->StartNextStateExecution();

        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();
        Sleep::MSec(100);

        for (uint32 cnt = 0u; (cnt < 100 * sizeof(uint32)) && (ret); cnt += 10 * sizeof(uint32)) {
            //write

            goWrite->Post();
            Sleep::MSec(100);
            goWrite->Post();
            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            int32 x = broker->GetOffset(0);
            ret = (x == -1);
            goWrite->Post();
            Sleep::MSec(100);
        }

        broker->SetSignalPostMiddle(1);
        broker1->SetSignalPostMiddle(1);
        dataSource->Done();
        goWrite->Post();
        goWrite->Post();
        goWrite->Post();

        goRead->Post();
        goRead->Post();
        goRead->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool RealTimeThreadAsyncBridgeTest::TestGetOutputOffset_False_BufferBusy() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 1"
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
            "                    CPUs = 1"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 2"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestInputBroker> broker;
    if (ret) {
        ReferenceContainer inputBrokers;
        gamReader1->GetInputBrokers(inputBrokers);

        broker = inputBrokers.Get(0);
        ret = broker.IsValid();
        if (ret) {
            broker->SetSignalPostMiddle(0);
        }
    }
    ReferenceT<RealTimeThreadAsyncBridgeTestOutputBroker> broker1;

    if (ret) {
        ReferenceContainer outputBrokers;
        gamWriter->GetOutputBrokers(outputBrokers);

        broker1 = outputBrokers.Get(0);
        ret = broker1.IsValid();
        if (ret) {
            broker1->SetSignalPostMiddle(0);
        }
    }
    if (ret) {
        app->StartNextStateExecution();

        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();
        Sleep::MSec(100);

        for (uint32 cnt = 0u; (cnt < 100 * sizeof(uint32)) && (ret); cnt += 10 * sizeof(uint32)) {
            //write

            goRead->Post();
            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            goWrite->Post();
            Sleep::MSec(100);
            goWrite->Post();
            Sleep::MSec(100);
            goWrite->Post();
            Sleep::MSec(100);
            int32 x = broker1->GetOffset(0);
            ret = (x == -1);
            goRead->Post();
            Sleep::MSec(100);
        }

        broker->SetSignalPostMiddle(1);
        broker1->SetSignalPostMiddle(1);
        dataSource->Done();
        goRead->Post();
        goRead->Post();
        goRead->Post();

        goWrite->Post();
        goWrite->Post();
        goWrite->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool RealTimeThreadAsyncBridgeTest::TestTerminateRead() {
    return RealTimeThreadAsyncBridgeTest::TestGetInputOffset();
}

bool RealTimeThreadAsyncBridgeTest::TestTerminateWrite() {
    return RealTimeThreadAsyncBridgeTest::TestGetOutputOffset();
}

bool RealTimeThreadAsyncBridgeTest::TestTerminateWrite_ResetCounter() {

    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAMWriter"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = RealTimeThreadAsyncBridgeTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = RealTimeThreadAsyncBridgeTestDS"
            "            NumberOfBuffers = 3"
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
            "                    CPUs = 2"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 3"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ReferenceT<RealTimeThreadAsyncBridgeTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT<RealTimeThreadAsyncBridgeTestGAMWriter> gamWriter;
    ReferenceT<RealTimeThreadAsyncBridgeTestGAM1> gamReader1;
    if (ret) {
        gamWriter = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamWriter.IsValid();

    }
    if (ret) {
        gamReader1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamReader1.IsValid();
    }
    if (ret) {
        uint32 newestCounter = (uint32) (-5);
        uint32 nSignals = dataSource->GetNumberOfSignals();
        for (uint32 i = 0u; i < nSignals; i++) {
            dataSource->SetNewestCounter(newestCounter, i);
        }

    }

    ReferenceT<RealTimeApplication> app;
    if (ret) {
        app = ObjectRegistryDatabase::Instance()->Find("Application1");
        ret = app.IsValid();
    }

    if (ret) {
        ret = app->PrepareNextState("State1");
    }

    if (ret) {
        app->StartNextStateExecution();
        EventSem *goRead = dataSource->GetEventRead();
        EventSem *goWrite = dataSource->GetEventWrite();
        ReferenceContainer inputBrokers;
        gamReader1->GetInputBrokers(inputBrokers);

        ReferenceT<RealTimeThreadAsyncBridgeTestInputBroker> broker = inputBrokers.Get(0);
        ret = broker.IsValid();
        Sleep::MSec(100);

        uint32 nBuffers = dataSource->GetNumberOfMemoryBuffers();

        for (uint32 cnt = 0u; (cnt < 10 * sizeof(uint32)) && (ret); cnt += sizeof(uint32)) {
            //write
            goWrite->Post();

            Sleep::MSec(100);
            goRead->Post();
            Sleep::MSec(100);
            int32 x = broker->GetOffset(0);
            ret = (cnt % (nBuffers * sizeof(uint32))) == (uint32) x;
        }

        dataSource->Done();
        goWrite->Post();
        goRead->Post();

        app->StopCurrentStateExecution();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}
