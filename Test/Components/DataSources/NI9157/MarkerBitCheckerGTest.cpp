/**
 * @file MarkerBitCheckerGTest.cpp
 * @brief Source file for class MarkerBitCheckerGTest.
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
 * the class MarkerBitCheckerGTest (public, protected, and private). Be aware
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
#include "MarkerBitCheckerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MarkerBitCheckerGTest,TestConstructor) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MarkerBitCheckerGTest,TestInitialise) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(MarkerBitCheckerGTest,TestInitialise_FalseSampleChecker) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_FalseSampleChecker());
}

TEST(MarkerBitCheckerGTest,TestInitialise_FalseNumOfFrameForSyncDiffOne) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNumOfFrameForSyncDiffOne());
}

TEST(MarkerBitCheckerGTest,TestInitialise_FalseNoMarkerBitMask) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_FalseNoMarkerBitMask());
}

TEST(MarkerBitCheckerGTest,TestInitialise_NoResetBitMask) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestInitialise_NoResetBitMask());
}

TEST(MarkerBitCheckerGTest,TestCheck) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestCheck());
}

TEST(MarkerBitCheckerGTest,TestCheck_FailMarkerMismatch) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestCheck_FailMarkerMismatch());
}

TEST(MarkerBitCheckerGTest,TestSynchronise) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(MarkerBitCheckerGTest,TestSynchronise_MarkerInTheMidle) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise_MarkerInTheMidle());
}

TEST(MarkerBitCheckerGTest,TestSynchronise_MarkerInTheTail) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise_MarkerInTheTail());
}

TEST(MarkerBitCheckerGTest,TestSynchronise_FalseMarkerNotFound) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise_FalseMarkerNotFound());
}

TEST(MarkerBitCheckerGTest,TestSynchronise_FalseSizeToReadZero) {
    MarkerBitCheckerTest test;
    ASSERT_TRUE(test.TestSynchronise_FalseSizeToReadZero());
}
