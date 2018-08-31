/**
 * @file MemoryGateTest.cpp
 * @brief Source file for class MemoryGateTest
 * @date 24/07/2018
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
 * the class MemoryGateTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "EventSem.h"
#include "GAMSchedulerI.h"
#include "IOGAM.h"
#include "MemoryDataSourceI.h"
#include "MemoryGateTest.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class MemoryGateTestInterface: public MemoryGate {
public:
    CLASS_REGISTER_DECLARATION();

    MemoryGateTestInterface();

    virtual ~MemoryGateTestInterface();

    uint8 *GetMem();

    uint32 GetMemSize();
    uint32 GetNumberOfBuffers();

    volatile int32 *GetSpinlocksRead();
    FastPollingMutexSem *GetSpinlocksWrite();

    uint32 *GetWhatIsNewestCounter();

    uint32 GetWhatIsNewestGlobCounter();

    uint32 GetOffsetStore();

    uint32 GetResetTimeout();

    virtual bool MemoryRead(uint8 * const bufferToFill);

    virtual bool MemoryWrite(const uint8 * const bufferToFlush);

    bool MemoryRead1(uint8 * const bufferToFill);

    bool MemoryWrite1(const uint8 * const bufferToFlush);

    EventSem *GetReadSem();
    EventSem *GetWriteSem();

    void SetCounters(uint32 globCounter);

    uint32 *GetCounters();

private:
    EventSem readEventSem;
    EventSem writeEventSem;

};

MemoryGateTestInterface::MemoryGateTestInterface() {
    readEventSem.Create();
    writeEventSem.Create();
}

MemoryGateTestInterface::~MemoryGateTestInterface() {
    readEventSem.Close();
    writeEventSem.Close();

}

uint8 *MemoryGateTestInterface::GetMem() {
    return mem;

}

uint32 MemoryGateTestInterface::GetMemSize() {
    return memSize;

}
uint32 MemoryGateTestInterface::GetNumberOfBuffers() {
    return numberOfBuffers;

}

volatile int32 *MemoryGateTestInterface::GetSpinlocksRead() {
    return spinlocksRead;

}
FastPollingMutexSem *MemoryGateTestInterface::GetSpinlocksWrite() {
    return spinlocksWrite;
}

uint32 *MemoryGateTestInterface::GetWhatIsNewestCounter() {
    return whatIsNewestCounter;
}

uint32 MemoryGateTestInterface::GetWhatIsNewestGlobCounter() {
    return whatIsNewestGlobCounter;
}

uint32 MemoryGateTestInterface::GetOffsetStore() {
    return offsetStore;

}

uint32 MemoryGateTestInterface::GetResetTimeout() {
    return resetTimeout.GetTimeoutMSec();
}

EventSem *MemoryGateTestInterface::GetReadSem() {
    return &readEventSem;
}

EventSem *MemoryGateTestInterface::GetWriteSem() {
    return &writeEventSem;
}

bool MemoryGateTestInterface::MemoryRead(uint8 * const bufferToFill) {

    //get input offset
    bool ok = false;
    uint64 checkedMask = 0ull;
    uint32 bufferIdx = 0u;

    for (uint32 k = 0u; (k < numberOfBuffers) && (!ok); k++) {
        //check the last written
        bufferIdx = 0u;
        uint32 max = 0u;
        for (uint32 h = 0u; (h < numberOfBuffers); h++) {
            if (spinlocksWrite[h].FastTryLock()) {
                uint32 tempMax = whatIsNewestCounter[h];
                spinlocksWrite[h].FastUnLock();
                if ((tempMax >= max) && (((1ull << h) & checkedMask) == 0u)) {
                    max = tempMax;
                    bufferIdx = h;
                }
            }
        }

        checkedMask |= (1ull << bufferIdx);
        ok = false;
        //try to lock the next buffer to write

        if (spinlocksWrite[bufferIdx].FastTryLock()) {
            Atomic::Increment(&spinlocksRead[bufferIdx]);
            spinlocksWrite[bufferIdx].FastUnLock();
            ok = true;
        }
    }

    if (ok) {
        readEventSem.ResetWait(TTInfiniteWait);
    }

    //copy the memory to the data source buffer
    if (ok) {
        uint32 offset = (memSize * bufferIdx);
        ok = MemoryOperationsHelper::Copy(bufferToFill, &mem[offset], memSize);
    }

    if (ok) {

        Atomic::Decrement(&spinlocksRead[bufferIdx]);
        REPORT_ERROR(ErrorManagement::Information, "buffer read=%d", bufferIdx);

    }

    return ok;

}

bool MemoryGateTestInterface::MemoryWrite(const uint8 * const bufferToFlush) {

    bool ok = false;
    uint32 bufferIdx = 0u;
    uint64 checkedMask = 0ull;
    for (uint32 k = 0u; (k < numberOfBuffers) && (!ok); k++) {
        //check the oldest written
        uint32 min = 0xFFFFFFFFu;
        bufferIdx = 0u;
        for (uint32 h = 0u; (h < numberOfBuffers); h++) {
            uint32 index = h;
            uint32 tempMin = whatIsNewestCounter[index];
            if ((tempMin < min) && (((1ull << h) & checkedMask) == 0ull)) {
                min = tempMin;
                bufferIdx = h;
            }
        }
        checkedMask |= (1ull << bufferIdx);
        ok = false;
        //try to lock the next buffer to write
        if (spinlocksWrite[bufferIdx].FastTryLock()) {
            //no readers!
            if (spinlocksRead[bufferIdx] == 0) {
                ok = true;
            }
            else {
                spinlocksWrite[bufferIdx].FastUnLock();
            }
        }
    }
    if (ok) {
        writeEventSem.ResetWait(TTInfiniteWait);
    }

    //copy the memory to the data source buffer
    if (ok) {
        uint32 offset = (memSize * bufferIdx);
        ok = MemoryOperationsHelper::Copy(&mem[offset], bufferToFlush, memSize);
    }

    if (ok) {
        REPORT_ERROR(ErrorManagement::Information, "buffer write=%d", bufferIdx);

        whatIsNewestGlobCounter++;
        whatIsNewestCounter[bufferIdx] = whatIsNewestGlobCounter;

        //overflow... subtract the smaller one
        if (whatIsNewestGlobCounter == 0u) {

            uint32 min = 0xFFFFFFFFu;
            for (uint32 h = 0u; (h < numberOfBuffers); h++) {
                if (h != bufferIdx) {
                    uint32 index1 = h;
                    if (whatIsNewestCounter[index1] < min) {
                        min = whatIsNewestCounter[index1];
                    }
                }
            }

            for (uint32 h = 0u; (h < numberOfBuffers); h++) {
                uint32 index1 = h;
                if (h != bufferIdx) {
                    bool blocked = spinlocksWrite[index1].FastLock(resetTimeout);
                    if (blocked) {
                        whatIsNewestCounter[index1] -= min;
                        spinlocksWrite[index1].FastUnLock();
                    }
                }
            }

            whatIsNewestGlobCounter -= min;
            whatIsNewestCounter[bufferIdx] -= min;
        }
        spinlocksWrite[bufferIdx].FastUnLock();

    }
    return ok;
}

bool MemoryGateTestInterface::MemoryRead1(uint8 * const bufferToFill){
    bool ret=MemoryGate::MemoryRead(bufferToFill);
    if(ret){
        readEventSem.ResetWait(TTInfiniteWait);
    }
    return ret;
}

bool MemoryGateTestInterface::MemoryWrite1(const uint8 * const bufferToFlush){
    bool ret=MemoryGate::MemoryWrite(bufferToFlush);
    if(ret){
        writeEventSem.ResetWait(TTInfiniteWait);
    }
    return ret;

}

void MemoryGateTestInterface::SetCounters(uint32 globCounter) {
    uint32 initCounter = globCounter - numberOfBuffers + 1;
    for (uint32 i = 0u; i < numberOfBuffers; i++) {
        whatIsNewestCounter[i] = initCounter;
        initCounter++;
    }
    whatIsNewestGlobCounter = globCounter;
}

uint32 *MemoryGateTestInterface::GetCounters() {
    return whatIsNewestCounter;
}

CLASS_REGISTER(MemoryGateTestInterface, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryGateTest::MemoryGateTest() {
}

MemoryGateTest::~MemoryGateTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool MemoryGateTest::TestConstructor() {
    MemoryGateTestInterface test;
    bool ret = test.GetMem() == NULL;
    ret &= test.GetMemSize() == 0u;
    ret &= test.GetNumberOfBuffers() == 2u;

    ret &= test.GetSpinlocksRead() == NULL;

    ret &= test.GetSpinlocksWrite() == NULL;

    ret &= test.GetWhatIsNewestCounter() == NULL;
    ret &= test.GetWhatIsNewestGlobCounter() == 0u;

    ret &= test.GetOffsetStore() == 0u;

    ret &= test.GetResetTimeout() == 0xffffffffu;

    return ret;
}

bool MemoryGateTest::TestInitialise() {
    const char8* config = ""
            "NumberOfBuffers = 10\n"
            "ResetMSecTimeout = 100\n"
            "MemorySize= 10";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 10u;
        ok &= test.GetResetTimeout() == 100;
        ok &= test.GetMem() != NULL;
        ok &= test.GetMemSize() == 10u;

    }
    return ok;
}

bool MemoryGateTest::TestInitialise_FalseTooManyBuffers() {
    const char8* config = ""
            "NumberOfBuffers = 65\n"
            "ResetMSecTimeout = 100\n"
            "MemorySize= 10";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = !test.Initialise(cdb);
    }
    return ok;
}

bool MemoryGateTest::TestInitialise_DefaultTimeout() {
    const char8* config = ""
            "NumberOfBuffers = 64\n"
            "MemorySize= 10";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 64u;
        ok &= test.GetResetTimeout() == 0xFFFFFFFF;
        ok &= test.GetMem() != NULL;
        ok &= test.GetMemSize() == 10u;

    }
    return ok;
}

bool MemoryGateTest::TestInitialise_DefaultNBuffers() {
    const char8* config = ""
            "ResetMSecTimeout = 100\n"
            "MemorySize= 10";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 2u;
        ok &= test.GetResetTimeout() == 100;
        ok &= test.GetMem() != NULL;
        ok &= test.GetMemSize() == 10u;

    }
    return ok;

}

bool MemoryGateTest::TestInitialise_NoMemSize() {
    const char8* config = ""
            "NumberOfBuffers = 10\n"
            "ResetMSecTimeout = 100";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 10u;
        ok &= test.GetResetTimeout() == 100;
        ok &= test.GetMem() == NULL;
        ok &= test.GetMemSize() == 0u;
    }
    return ok;
}

bool MemoryGateTest::TestSetMemorySize_Initialised() {
    const char8* config = ""
            "NumberOfBuffers = 10\n"
            "ResetMSecTimeout = 100\n"
            "MemorySize= 10";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 10u;
        ok &= test.GetResetTimeout() == 100;
        ok &= test.GetMem() != NULL;
        ok &= test.GetMemSize() == 10u;

    }
    if (ok) {
        ok = test.SetMemorySize(10);
        ok = !test.SetMemorySize(9);

    }
    return ok;
}

bool MemoryGateTest::TestSetMemorySize_NotInitialised() {
    const char8* config = ""
            "NumberOfBuffers = 10\n"
            "ResetMSecTimeout = 100";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 10u;
        ok &= test.GetResetTimeout() == 100;
        ok &= test.GetMem() == NULL;
        ok &= test.GetMemSize() == 0u;

    }
    if (ok) {
        ok = test.SetMemorySize(10);
        ok &= test.GetMem() != NULL;
        ok = test.SetMemorySize(10);
        ok = !test.SetMemorySize(9);
    }
    return ok;
}

struct ThreadArg {
    MemoryGateTestInterface *ptr;
    uint8 buffer;
    bool ret;
    volatile int32 done;
};

static void ReadFunction(ThreadArg &tt) {
    Atomic::Increment(&tt.done);

    //tells to the main process that the thread begins
    tt.ret = tt.ptr->MemoryRead(&(tt.buffer));
    Atomic::Increment(&tt.done);
}

static void WriteFunction(ThreadArg &tt) {
    Atomic::Increment(&tt.done);

    //tells to the main process that the thread begins
    tt.ret = tt.ptr->MemoryWrite(&(tt.buffer));
    Atomic::Increment(&tt.done);
}


static void ReadFunction1(ThreadArg &tt) {
    Atomic::Increment(&tt.done);

    //tells to the main process that the thread begins
    tt.ret = tt.ptr->MemoryRead1(&(tt.buffer));
    Atomic::Increment(&tt.done);
}

static void WriteFunction1(ThreadArg &tt) {
    Atomic::Increment(&tt.done);

    //tells to the main process that the thread begins
    tt.ret = tt.ptr->MemoryWrite1(&(tt.buffer));
    Atomic::Increment(&tt.done);
}


bool MemoryGateTest::TestMemoryRead() {
    const char8* config = ""
            "NumberOfBuffers = 2\n"
            "MemorySize= 1";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 2u;
        ok &= test.GetMem() != NULL;
        ok &= test.GetMemSize() == 1u;
    }

    EventSem *readEventSem = test.GetReadSem();
    EventSem *writeEventSem = test.GetWriteSem();

    if (ok) {

        ThreadArg writeArg;
        writeArg.ptr = &test;
        writeArg.buffer = 1;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done < 2) {
            Sleep::MSec(100);
        }

        ThreadArg readArg;
        readArg.ptr = &test;
        readArg.buffer = 0;
        readArg.ret = false;
        readArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) ReadFunction, &readArg);

        Sleep::MSec(100);
        readEventSem->Post();
        while (readArg.done < 2) {
            Sleep::MSec(100);
        }
        ok &= readArg.ret;
        ok &= readArg.buffer == 1;
    }

    if (ok) {
        //test 2... take always the last written
        ThreadArg writeArg;
        writeArg.ptr = &test;
        writeArg.buffer = 1;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done == 0) {
            Sleep::MSec(100);
        }

        writeArg.ptr = &test;
        writeArg.buffer = 2;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done < 2) {
            Sleep::MSec(100);
        }

        ThreadArg readArg;
        readArg.ptr = &test;
        readArg.buffer = 0;
        readArg.ret = false;
        readArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) ReadFunction, &readArg);

        Sleep::MSec(100);
        readEventSem->Post();
        while (readArg.done < 2) {
            Sleep::MSec(100);
        }
        ok &= readArg.ret;
        ok &= readArg.buffer == 2;
    }

    if (ok) {
        //test 3... return false if buffers are busy
        ThreadArg writeArg[2];
        writeArg[0].ptr = &test;
        writeArg[0].buffer = 1;
        writeArg[0].ret = false;
        writeArg[0].done = 0;

        writeArg[1].ptr = &test;
        writeArg[1].buffer = 2;
        writeArg[1].ret = false;
        writeArg[1].done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg[0]);
        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg[1]);

        while ((writeArg[0].done == 0) || (writeArg[1].done == 0)) {
            Sleep::MSec(100);
        }

        ThreadArg readArg;
        readArg.ptr = &test;
        readArg.buffer = 0;
        readArg.ret = false;
        readArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) ReadFunction, &readArg);

        Sleep::MSec(100);
        readEventSem->Post();
        while (readArg.done < 2) {
            Sleep::MSec(100);
        }
        //read should fail... no buffer available
        ok &= !readArg.ret;

        Sleep::MSec(200);
        writeEventSem->Post();
        while (writeArg[0].done < 2) {
            Sleep::MSec(100);
        }
        while (writeArg[1].done < 2) {
            Sleep::MSec(100);
        }
    }

    return ok;
}

bool MemoryGateTest::TestMemoryWrite() {
    const char8* config = ""
            "NumberOfBuffers = 2\n"
            "MemorySize= 1";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 2u;
        ok &= test.GetMem() != NULL;
        ok &= test.GetMemSize() == 1u;
    }

    EventSem *readEventSem = test.GetReadSem();
    EventSem *writeEventSem = test.GetWriteSem();

    if (ok) {

        ThreadArg writeArg;
        writeArg.ptr = &test;
        writeArg.buffer = 1;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done < 2) {
            Sleep::MSec(100);
        }

        ThreadArg readArg;
        readArg.ptr = &test;
        readArg.buffer = 0;
        readArg.ret = false;
        readArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) ReadFunction, &readArg);

        Sleep::MSec(100);
        readEventSem->Post();
        while (readArg.done < 2) {
            Sleep::MSec(100);
        }
        ok &= readArg.ret;
        ok &= readArg.buffer == 1;
    }

    if (ok) {
        //test 2... write nn the oldest written
        uint8* mem = test.GetMem();

        ThreadArg writeArg;
        writeArg.ptr = &test;
        writeArg.buffer = 1;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done < 2) {
            Sleep::MSec(100);
        }

        //remember the write before
        ok = (mem[1]) == 1;
        ok &= writeArg.ret;

        writeArg.ptr = &test;
        writeArg.buffer = 2;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done < 2) {
            Sleep::MSec(100);
        }
        ok &= (mem[1]) == 1;
        ok &= (mem[0]) == 2;
        ok &= writeArg.ret;

        writeArg.ptr = &test;
        writeArg.buffer = 3;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done < 2) {
            Sleep::MSec(100);
        }
        ok &= (mem[1]) == 3;
        ok &= (mem[0]) == 2;
        ok &= writeArg.ret;
    }

    if (ok) {
        //test 3... return false if all buffers are busy

        ThreadArg writeArg[2];
        writeArg[0].ptr = &test;
        writeArg[0].buffer = 1;
        writeArg[0].ret = false;
        writeArg[0].done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg[0]);

        ThreadArg readArg;
        readArg.ptr = &test;
        readArg.buffer = 0;
        readArg.ret = false;
        readArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) ReadFunction, &readArg);
        while ((writeArg[0].done == 0) || (readArg.done == 0))
            ;

        writeArg[1].ptr = &test;
        writeArg[1].buffer = 2;
        writeArg[1].ret = false;
        writeArg[1].done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction, &writeArg[1]);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg[1].done < 2) {
            Sleep::MSec(100);
        }
        ok &= !writeArg[1].ret;

        while (writeArg[0].done < 2) {
            Sleep::MSec(100);
        }

        Sleep::MSec(100);
        readEventSem->Post();
        while (readArg.done < 2) {
            Sleep::MSec(100);
        }

    }

    return ok;
}

bool MemoryGateTest::TestMemoryWrite_Overflow() {
    const char8* config = ""
            "NumberOfBuffers = 2\n"
            "MemorySize= 1";

    MemoryGateTestInterface test;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    if (ok) {
        ok = test.Initialise(cdb);
    }

    if (ok) {
        ok &= test.GetNumberOfBuffers() == 2u;
        ok &= test.GetMem() != NULL;
        ok &= test.GetMemSize() == 1u;
    }

    EventSem *readEventSem = test.GetReadSem();
    EventSem *writeEventSem = test.GetWriteSem();

    if (ok) {

        test.SetCounters(0xFFFFFFFF);
        uint32 *counters = test.GetCounters();

        ok &= (counters[0] == 0xFFFFFFFE);
        ok &= (counters[1] == 0xFFFFFFFF);

        ThreadArg writeArg;
        writeArg.ptr = &test;
        writeArg.buffer = 1;
        writeArg.ret = false;
        writeArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) WriteFunction1, &writeArg);

        Sleep::MSec(100);
        writeEventSem->Post();
        while (writeArg.done < 2) {
            Sleep::MSec(100);
        }

        ThreadArg readArg;
        readArg.ptr = &test;
        readArg.buffer = 0;
        readArg.ret = false;
        readArg.done = 0;

        Threads::BeginThread((ThreadFunctionType) ReadFunction1, &readArg);

        Sleep::MSec(100);
        readEventSem->Post();
        while (readArg.done < 2) {
            Sleep::MSec(100);
        }
        ok &= readArg.ret;
        ok &= readArg.buffer == 1;

        //write on 0!
        ok &= (counters[0] == 1);
        ok &= (counters[1] == 0);

    }
    return ok;
}

