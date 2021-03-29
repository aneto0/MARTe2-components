/**
 * @file SampleCheckerTest.cpp
 * @brief Source file for class SampleCheckerTest.
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
 * the class SampleCheckerTest (public, protected, and private). Be aware
 * that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SampleCheckerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class SampleCheckerTestHelper: public SampleChecker {
public:
    CLASS_REGISTER_DECLARATION()

    SampleCheckerTestHelper();
    virtual ~SampleCheckerTestHelper();
    virtual bool Check(uint8 *sample, bool &write);
    virtual bool Synchronise(uint8 *frames, uint32 sizeToRead, uint32 &idx, bool &write);
    uint8 GetSampleSize();
    uint8 GetNFrameForSync();

};

SampleCheckerTestHelper::SampleCheckerTestHelper() {
}

SampleCheckerTestHelper::~SampleCheckerTestHelper() {
}

bool SampleCheckerTestHelper::Check(uint8 *sample, bool &write) {
    return false;
}

bool SampleCheckerTestHelper::Synchronise(uint8 *frames, uint32 sizeToRead, uint32 &idx, bool &write) {
    return false;
}

uint8 SampleCheckerTestHelper::GetSampleSize() {
    return sampleSize;
}

uint8 SampleCheckerTestHelper::GetNFrameForSync() {
    return nFrameForSync;
}

CLASS_REGISTER(SampleCheckerTestHelper, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
SampleCheckerTest::SampleCheckerTest() {
}

SampleCheckerTest::~SampleCheckerTest() {
}

bool SampleCheckerTest::TestConstructor() {
    bool ret = false;
    SampleCheckerTestHelper aTest;
    ret = (aTest.GetSampleSize() == 0u);
    ret &= (aTest.GetNFrameForSync() == 0u);
    return ret;
}

bool SampleCheckerTest::TestInitialise() {
    bool ret = false;
    SampleCheckerTestHelper aTest;
    ConfigurationDatabase cdb;
    cdb.Write("SampleSize", "8");
    cdb.Write("NumOfFrameForSync", "2");
    ret = aTest.Initialise(cdb);
    ret &= (aTest.GetSampleSize() == 8u);
    ret &= (aTest.GetNFrameForSync() == 2u);
    return ret;
}

bool SampleCheckerTest::TestInitialise_NoNumOfFrameForSync() {
    bool ret = false;
    SampleCheckerTestHelper aTest;
    ConfigurationDatabase cdb;
    cdb.Write("SampleSize", "8");
    ret = aTest.Initialise(cdb);
    ret &= (aTest.GetSampleSize() == 8u);
    ret &= (aTest.GetNFrameForSync() == 1u);
    return ret;
}

bool SampleCheckerTest::TestInitialise_NoSampleSize() {
    bool ret = false;
    SampleCheckerTestHelper aTest;
    ConfigurationDatabase cdb;
    cdb.Write("NumOfFrameForSync", "2");
    ret = !aTest.Initialise(cdb);
    return ret;
}

bool SampleCheckerTest::TestInitialise_NumOfFrameForSyncZero() {
    bool ret = false;
    SampleCheckerTestHelper aTest;
    ConfigurationDatabase cdb;
    cdb.Write("SampleSize", "8");
    cdb.Write("NumOfFrameForSync", "0");
    ret = !aTest.Initialise(cdb);
    return ret;
}

bool SampleCheckerTest::TestGetNumberOfFramesToSync() {
    bool ret = false;
    SampleCheckerTestHelper aTest;
    ConfigurationDatabase cdb;
    cdb.Write("SampleSize", "8");
    cdb.Write("NumOfFrameForSync", "2");
    ret = (aTest.GetNumberOfFramesToSync() == 0u);
    ret &= aTest.Initialise(cdb);
    ret &= (aTest.GetNumberOfFramesToSync() == 2u);
    return ret;
}
