/**
 * @file CounterCheckerTest.cpp
 * @brief Source file for class CounterCheckerTest.
 * @date 26/03/2021
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
 * the class CounterCheckerTest (public, protected, and private). Be aware
 * that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CounterCheckerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class CounterCheckerTestHelper: public CounterChecker {
public:
    CLASS_REGISTER_DECLARATION()

    CounterCheckerTestHelper();
    virtual ~CounterCheckerTestHelper();
    uint64 GetPacketCounter();
    uint64 GetAcquireFromCounter();
    uint64 GetNextPacketCheck();
    uint32 GetCheckCounterAfterNSteps();
    uint32 GetCounterStep();
    uint8 GetSampleSize();
    uint8 GetNFrameForSync();

};

CounterCheckerTestHelper::CounterCheckerTestHelper() {
}

CounterCheckerTestHelper::~CounterCheckerTestHelper() {
}

uint64 CounterCheckerTestHelper::GetPacketCounter() {
    return packetCounter;
}

uint64 CounterCheckerTestHelper::GetAcquireFromCounter() {
    return acquireFromCounter;
}

uint64 CounterCheckerTestHelper::GetNextPacketCheck() {
    return nextPacketCheck;
}

uint32 CounterCheckerTestHelper::GetCheckCounterAfterNSteps() {
    return checkCounterAfterNSteps;
}

uint32 CounterCheckerTestHelper::GetCounterStep() {
    return counterStep;
}

uint8 CounterCheckerTestHelper::GetSampleSize() {
    return sampleSize;
}

uint8 CounterCheckerTestHelper::GetNFrameForSync() {
    return nFrameForSync;
}

CLASS_REGISTER(CounterCheckerTestHelper, "1.0")

static const char8 * const counterCheckerTestconfig0 = ""
    "+ACounterChecker = {"
    "    Class = CounterCheckerTestHelper"
    "    FirstPacketCounter = 2"
    "    AcquireFromCounter = 1"
    "    CounterStep = 1"
    "    CheckCounterAfterNSteps = 1"
    "    SampleSize = 8"
    "    NumOfFrameForSync = 2"
    "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
CounterCheckerTest::CounterCheckerTest() {
}

CounterCheckerTest::~CounterCheckerTest() {
}

bool CounterCheckerTest::TestConstructor() {
    bool ret = false;
    CounterCheckerTestHelper aCounterChecker;
    ret = (aCounterChecker.GetPacketCounter() == 1ull);
    ret &= (aCounterChecker.GetAcquireFromCounter() == 0ull);
    ret &= (aCounterChecker.GetCheckCounterAfterNSteps() == 0u);
    ret &= (aCounterChecker.GetCounterStep() == 1u);
    ret &= (aCounterChecker.GetNextPacketCheck() == 1ull);
    ret &= (aCounterChecker.GetSampleSize() == 0u);
    ret &= (aCounterChecker.GetNFrameForSync() == 0u);
    return ret;
}

bool CounterCheckerTest::TestInitialise() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestInitialise_FailSampleChecker() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("NumOfFrameForSync", "0");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestInitialise_FailNumOfFrameForSync() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("NumOfFrameForSync", "1");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestInitialise_NoFirstPacketCounter() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Delete("FirstPacketCounter");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 1ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 1ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestInitialise_NoAcquireFromCounter() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Delete("AcquireFromCounter");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 2ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestInitialise_NoCounterStep() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Delete("CounterStep");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}


bool CounterCheckerTest::TestInitialise_CounterStepZero() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("CounterStep", "0");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestInitialise_NoCheckCounterAfterNSteps() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("AcquireFromCounter", "2");
        ret &= cdb.Write("CounterStep", "2");
        ret &= cdb.Delete("CheckCounterAfterNSteps");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 2ull);
        ret &= (aCounterChecker->GetCounterStep() == 2u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 2u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}


bool CounterCheckerTest::TestInitialise_NoCheckCounterAfterNStepsDivCounterStep() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("CheckCounterAfterNSteps", "4");
        ret &= cdb.Write("CounterStep", "3");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestInitialise_NoAcquireFromCounterMinusPacketCounterDivCounterStep() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("CheckCounterAfterNSteps", "4");
        ret &= cdb.Write("AcquireFromCounter", "4");
        ret &= cdb.Write("FirstPacketCounter", "1");
        ret &= cdb.Write("CounterStep", "2");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestCheck() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("AcquireFromCounter", "2");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 2ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    if (ret) {
        bool aWrite = false;
        uint64 aSample = 2ull;
        ret = aCounterChecker->Check(reinterpret_cast<uint8 *>(&aSample), aWrite);
        ret &= aWrite;
    }
    if (ret) {
        ret = (aCounterChecker->GetAcquireFromCounter() == 0ull);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestCheck_FalseWrite() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    if (ret) {
        bool aWrite = false;
        uint64 aSample = 2ull;
        ret = aCounterChecker->Check(reinterpret_cast<uint8 *>(&aSample), aWrite);
        ret &= !aWrite;
    }
    if (ret) {
        ret = (aCounterChecker->GetAcquireFromCounter() == 1ull);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestCheck_ReturnFalse() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    if (ret) {
        bool aWrite = false;
        uint64 aSample = 128ull;
        ret = !aCounterChecker->Check(reinterpret_cast<uint8 *>(&aSample), aWrite);
        ret &= aWrite;
    }
    if (ret) {
        ret = (aCounterChecker->GetAcquireFromCounter() == 1ull);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestCheck_ConsecutiveCalls() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("AcquireFromCounter", "0");
        ret &= cdb.Write("CounterStep", "2");
        ret &= cdb.Write("CheckCounterAfterNSteps", "2");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 0ull);
        ret &= (aCounterChecker->GetCounterStep() == 2u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 2u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    uint64 aSample = 0ull;
    bool aWrite = false;
    uint32 runs = 10u;
    for (uint32 i = 0u; (i < runs) && (ret) ; i++) {
        aSample += aCounterChecker->GetCounterStep();
        ret = aCounterChecker->Check(reinterpret_cast<uint8 *>(&aSample), aWrite);
        ret &= aWrite;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestSynchronise() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    if (ret) {
        uint32 numberOfPackets = 4;
        uint32 numberOfSamples = 8;
        uint64 testPackets[numberOfSamples*numberOfPackets];
        for (uint32 i = 0u; i < numberOfPackets; i++) {
            for (uint32 j = 0u; j < numberOfSamples; j++) {
                if (j == 0u) {
                    testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u);
                }
                else {
                    testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u)*1000ull + static_cast<uint64>(j);
                }
                
            }
        }
        uint32 aSizeToRead = numberOfSamples * aCounterChecker->GetSampleSize();
        uint32 aIdx = 0u;
        bool aWrite = false;
        ret = aCounterChecker->Synchronise(reinterpret_cast<uint8 *>(&testPackets[0]), aSizeToRead, aIdx, aWrite);
        ret &= (aIdx == 0u);
        ret &= aWrite;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestSynchronise_WriteFalse() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+ACounterChecker");
        ret &= cdb.Write("AcquireFromCounter", "2");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 2ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    if (ret) {
        uint32 numberOfPackets = 4;
        uint32 numberOfSamples = 8;
        uint64 testPackets[numberOfSamples*numberOfPackets];
        for (uint32 i = 0u; i < numberOfPackets; i++) {
            for (uint32 j = 0u; j < numberOfSamples; j++) {
                if (j == 0u) {
                    testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u);
                }
                else {
                    testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u)*1000ull + static_cast<uint64>(j);
                }
                
            }
        }
        uint32 aSizeToRead = numberOfSamples * aCounterChecker->GetSampleSize();
        uint32 aIdx = 0u;
        bool aWrite = false;
        ret = aCounterChecker->Synchronise(reinterpret_cast<uint8 *>(&testPackets[0]), aSizeToRead, aIdx, aWrite);
        ret &= (aIdx == 0u);
        ret &= !aWrite;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestSynchronise_FalseSizeToReadZero() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    if (ret) {
        uint32 numberOfPackets = 4;
        uint32 numberOfSamples = 8;
        uint64 testPackets[numberOfSamples*numberOfPackets];
        for (uint32 i = 0u; i < numberOfPackets; i++) {
            for (uint32 j = 0u; j < numberOfSamples; j++) {
                if (j == 0u) {
                    testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u);
                }
                else {
                    testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u)*1000ull + static_cast<uint64>(j);
                }
                
            }
        }
        uint32 aSizeToRead = 0u;
        uint32 aIdx = 0u;
        bool aWrite = false;
        ret = !aCounterChecker->Synchronise(reinterpret_cast<uint8 *>(&testPackets[0]), aSizeToRead, aIdx, aWrite);
        ret &= (aIdx == 0u);
        ret &= aWrite;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool CounterCheckerTest::TestSynchronise_FalseFrameIsWrong() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  counterCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<CounterCheckerTestHelper> aCounterChecker;
    if (ret) {
        aCounterChecker = ObjectRegistryDatabase::Instance()->Find("ACounterChecker");
        ret = aCounterChecker.IsValid();
    }
    if (ret) {
        ret = (aCounterChecker->GetPacketCounter() == 2ull);
        ret &= (aCounterChecker->GetAcquireFromCounter() == 1ull);
        ret &= (aCounterChecker->GetCounterStep() == 1u);
        ret &= (aCounterChecker->GetCheckCounterAfterNSteps() == 1u);
        ret &= (aCounterChecker->GetNextPacketCheck() == 2ull);
        ret &= (aCounterChecker->GetSampleSize() == 8u);
        ret &= (aCounterChecker->GetNFrameForSync() == 2u);
    }
    if (ret) {
        uint32 numberOfPackets = 4;
        uint32 numberOfSamples = 8;
        uint64 testPackets[numberOfSamples*numberOfPackets];
        for (uint32 i = 0u; i < numberOfPackets; i++) {
            for (uint32 j = 0u; j < numberOfSamples; j++) {
                if (j == 0u) {
                    if (i == 1u) {
                        testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+2u);    
                    }
                    else {
                        testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u);
                    }
                }
                else {
                    testPackets[i*numberOfSamples + j] = static_cast<uint64>(i+1u)*1000ull + static_cast<uint64>(j);
                }
                
            }
        }
        uint32 aSizeToRead = numberOfSamples * aCounterChecker->GetSampleSize();
        uint32 aIdx = 0u;
        bool aWrite = false;
        ret = !aCounterChecker->Synchronise(reinterpret_cast<uint8 *>(&testPackets[0]), aSizeToRead, aIdx, aWrite);
        ret &= (aIdx == aSizeToRead);
        ret &= aWrite;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}
