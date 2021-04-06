/**
 * @file MarkerBitCheckerTest.cpp
 * @brief Source file for class MarkerBitCheckerTest.
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
 * the class MarkerBitCheckerTest (public, protected, and private). Be aware
 * that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MarkerBitCheckerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class MarkerBitCheckerTestHelper: public MarkerBitChecker {
public:
    CLASS_REGISTER_DECLARATION()

    MarkerBitCheckerTestHelper();
    virtual ~MarkerBitCheckerTestHelper();
    uint8 GetSampleSize();
    uint8 GetNFrameForSync();
    uint64 GetMarkerBitMask();
    uint64 GetResetBitMask();

};

MarkerBitCheckerTestHelper::MarkerBitCheckerTestHelper() {
}

MarkerBitCheckerTestHelper::~MarkerBitCheckerTestHelper() {
}

uint8 MarkerBitCheckerTestHelper::GetSampleSize() {
    return sampleSize;
}

uint8 MarkerBitCheckerTestHelper::GetNFrameForSync() {
    return nFrameForSync;
}

uint64 MarkerBitCheckerTestHelper::GetMarkerBitMask() {
    return bitMask;
}

uint64 MarkerBitCheckerTestHelper::GetResetBitMask() {
    return resetBitMask;
}

CLASS_REGISTER(MarkerBitCheckerTestHelper, "1.0")

static const char8 * const markerBitCheckerTestconfig0 = ""
    "+AMarkerBitChecker = {"
    "    Class = MarkerBitCheckerTestHelper"
    "    MarkerBitMask = 2"
    "    ResetBitMask = 1"
    "    SampleSize = 8"
    "    NumOfFrameForSync = 1"
    "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
MarkerBitCheckerTest::MarkerBitCheckerTest() {
}

MarkerBitCheckerTest::~MarkerBitCheckerTest() {
}

bool MarkerBitCheckerTest::TestConstructor() {
    bool ret;
    MarkerBitCheckerTestHelper aMarkerBitChecker;
    ret = (aMarkerBitChecker.GetMarkerBitMask() == 0ull);
    ret &= (aMarkerBitChecker.GetResetBitMask() == 0ull);
    ret &= (aMarkerBitChecker.GetSampleSize() == 0u);
    ret &= (aMarkerBitChecker.GetNFrameForSync() == 0u);
    return ret;
}

bool MarkerBitCheckerTest::TestInitialise() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    if (ret) {
        ret = (aMarkerBitChecker->GetMarkerBitMask() == 2ull);
        ret &= (aMarkerBitChecker->GetResetBitMask() == 1ull);
        ret &= (aMarkerBitChecker->GetSampleSize() == 8u);
        ret &= (aMarkerBitChecker->GetNFrameForSync() == 1u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestInitialise_FalseSampleChecker() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Delete("SampleSize");
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

bool MarkerBitCheckerTest::TestInitialise_FalseNumOfFrameForSyncDiffOne() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("NumOfFrameForSync", "2");
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

bool MarkerBitCheckerTest::TestInitialise_FalseNoMarkerBitMask() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Delete("MarkerBitMask");
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

bool MarkerBitCheckerTest::TestInitialise_NoResetBitMask() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Delete("ResetBitMask");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    if (ret) {
        ret = (aMarkerBitChecker->GetMarkerBitMask() == 2ull);
        ret &= (aMarkerBitChecker->GetResetBitMask() == 2ull);
        ret &= (aMarkerBitChecker->GetSampleSize() == 8u);
        ret &= (aMarkerBitChecker->GetNFrameForSync() == 1u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestCheck() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("MarkerBitMask", "15");
        ret &= cdb.Write("ResetBitMask", "8");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    uint64 aFrame = 0ull;
    bool aWrite = false;
    if (ret) {
        aFrame = 8ull;
        ret = aMarkerBitChecker->Check(reinterpret_cast<uint8 *>(&aFrame), aWrite);
        ret &= (aFrame == 0ull);
        ret &= (aWrite == true);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestCheck_FailMarkerMismatch() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("MarkerBitMask", "16");
        ret &= cdb.Write("ResetBitMask", "8");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    uint64 aFrame = 0ull;
    bool aWrite = false;
    if (ret) {
        aFrame = 8ull;
        ret = !aMarkerBitChecker->Check(reinterpret_cast<uint8 *>(&aFrame), aWrite);
        ret &= (aFrame == 8ull);
        ret &= (aWrite == true);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestSynchronise() {
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("MarkerBitMask", "1");
        ret &= cdb.Write("ResetBitMask", "8");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    uint32 nSamples = 16;
    uint64 testFrames[nSamples];
    for (uint32 i = 0u; i < nSamples; i++) {
        testFrames[i] = static_cast<uint64>(1ull << i);
    }
    bool aWrite = false;
    uint32 aSizeToRead = nSamples * static_cast<uint32>(aMarkerBitChecker->GetSampleSize());
    uint32 markerIdx = 0u;
    uint32 result = 0u;
    if (ret) {
        ret = aMarkerBitChecker->Synchronise(reinterpret_cast<uint8 *>(&testFrames), aSizeToRead, markerIdx, aWrite);
        result = markerIdx/(static_cast<uint32>(aMarkerBitChecker->GetSampleSize()));
        if (ret) {
            ret = (result < nSamples);
            if (ret) {
                ret = (testFrames[result] == 1ull);
                ret &= (aWrite == true);
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestSynchronise_MarkerInTheMidle() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("MarkerBitMask", "256");
        ret &= cdb.Write("ResetBitMask", "8");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    uint32 nSamples = 16;
    uint64 testFrames[nSamples];
    for (uint32 i = 0u; i < nSamples; i++) {
        testFrames[i] = static_cast<uint64>(1ull << i);
    }
    bool aWrite = false;
    uint32 aSizeToRead = nSamples * static_cast<uint32>(aMarkerBitChecker->GetSampleSize());
    uint32 markerIdx = 0u;
    uint32 result = 0u;
    if (ret) {
        ret = aMarkerBitChecker->Synchronise(reinterpret_cast<uint8 *>(&testFrames), aSizeToRead, markerIdx, aWrite);
        result = markerIdx/(static_cast<uint32>(aMarkerBitChecker->GetSampleSize()));
        if (ret) {
            ret = (result < nSamples);
            if (ret) {
                ret = (testFrames[result] == 256ull);
                ret &= (aWrite == true);
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestSynchronise_MarkerInTheTail() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("MarkerBitMask", "32768");
        ret &= cdb.Write("ResetBitMask", "8");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    uint32 nSamples = 16;
    uint64 testFrames[nSamples];
    for (uint32 i = 0u; i < nSamples; i++) {
        testFrames[i] = static_cast<uint64>(1ull << i);
    }
    bool aWrite = false;
    uint32 aSizeToRead = nSamples * static_cast<uint32>(aMarkerBitChecker->GetSampleSize());
    uint32 markerIdx = 0u;
    uint32 result = 0u;
    if (ret) {
        ret = aMarkerBitChecker->Synchronise(reinterpret_cast<uint8 *>(&testFrames), aSizeToRead, markerIdx, aWrite);
        result = markerIdx/(static_cast<uint32>(aMarkerBitChecker->GetSampleSize()));
        if (ret) {
            ret = (result < nSamples);
            if (ret) {
                ret = (testFrames[result] == 32768ull);
                ret &= (aWrite == true);
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestSynchronise_FalseMarkerNotFound() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("MarkerBitMask", "65536");
        ret &= cdb.Write("ResetBitMask", "8");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    uint32 nSamples = 16;
    uint64 testFrames[nSamples];
    for (uint32 i = 0u; i < nSamples; i++) {
        testFrames[i] = static_cast<uint64>(1ull << i);
    }
    bool aWrite = false;
    uint32 aSizeToRead = nSamples * static_cast<uint32>(aMarkerBitChecker->GetSampleSize());
    uint32 markerIdx = 0u;
    uint32 result = 0u;
    if (ret) {
        ret = !aMarkerBitChecker->Synchronise(reinterpret_cast<uint8 *>(&testFrames), aSizeToRead, markerIdx, aWrite);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool MarkerBitCheckerTest::TestSynchronise_FalseSizeToReadZero() {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  markerBitCheckerTestconfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+AMarkerBitChecker");
        ret &= cdb.Write("MarkerBitMask", "1");
        ret &= cdb.Write("ResetBitMask", "8");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<MarkerBitCheckerTestHelper> aMarkerBitChecker;
    if (ret) {
        aMarkerBitChecker = ObjectRegistryDatabase::Instance()->Find("AMarkerBitChecker");
        ret = aMarkerBitChecker.IsValid();
    }
    uint32 nSamples = 16;
    uint64 testFrames[nSamples];
    for (uint32 i = 0u; i < nSamples; i++) {
        testFrames[i] = static_cast<uint64>(1ull << i);
    }
    bool aWrite = false;
    uint32 aSizeToRead = 0;
    uint32 markerIdx = 0u;
    uint32 result = 0u;
    if (ret) {
        ret = !aMarkerBitChecker->Synchronise(reinterpret_cast<uint8 *>(&testFrames), aSizeToRead, markerIdx, aWrite);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}
