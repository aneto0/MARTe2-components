/**
 * @file StatisticsGAMGTest.cpp
 * @brief Source file for class StatisticsGAMGTest
 * @date 30/01/2017
 * @author Llorenc Capella
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
 * the class StatisticsGAMGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StatisticsGAM.h"
#include "StatisticsGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StatisticsGAMGTest,TestConstructor) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(StatisticsGAMGTest,TestInitialise) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(StatisticsGAMGTest,TestSetup) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(StatisticsGAMGTest,TestInitialise_WindowSize) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestInitialise_WindowSize());
}

TEST(StatisticsGAMGTest,TestExecute) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(StatisticsGAMGTest,TestPrepareForNextState_Error) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestPrepareForNextState_Error());
}

TEST(StatisticsGAMGTest,TestPrepareForNextState_Success) {
    StatisticsGAMTest test;
    ASSERT_TRUE(test.TestPrepareForNextState_Success());
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

