/**
 * @file SampleCheckerGTest.cpp
 * @brief Source file for class SampleCheckerGTest.
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
 * the class SampleCheckerGTest (public, protected, and private). Be aware
 * that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SampleCheckerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(SampleCheckerGTest,TestConstructor) {
    SampleCheckerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(SampleCheckerGTest,TestInitialise) {
    SampleCheckerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(SampleCheckerGTest,TestInitialise_NoNumOfFrameForSync) {
    SampleCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoNumOfFrameForSync());
}

TEST(SampleCheckerGTest,TestInitialise_NoSampleSize) {
    SampleCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoSampleSize());
}

TEST(SampleCheckerGTest,TestInitialise_NumOfFrameForSyncZero) {
    SampleCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NumOfFrameForSyncZero());
}

TEST(SampleCheckerGTest,TestGetNumberOfFramesToSync) {
    SampleCheckerTest test;
    ASSERT_TRUE(test.TestGetNumberOfFramesToSync());
}
