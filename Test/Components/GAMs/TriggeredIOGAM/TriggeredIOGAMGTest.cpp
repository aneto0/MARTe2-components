/**
 * @file TriggeredIOGAMGTest.cpp
 * @brief Source file for class TriggeredIOGAMGTest
 * @date 25/03/2022
 * @author Andre Neto
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
 * the class TriggeredIOGAMGTest (public, protected, and private). Be aware that some
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
#include "TriggeredIOGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TriggeredIOGAMGTest,TestConstructor) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(TriggeredIOGAMGTest,TestSetup) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(TriggeredIOGAMGTest,TestSetup_NoTrigger) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestSetup_NoTrigger());
}

TEST(TriggeredIOGAMGTest,TestSetup_LessInputs) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestSetup_LessInputs());
}

TEST(TriggeredIOGAMGTest,TestSetup_DifferentType) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestSetup_DifferentType());
}

TEST(TriggeredIOGAMGTest,TestSetup_False_DifferentSize) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentSize());
}

TEST(TriggeredIOGAMGTest,TestExecute) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(TriggeredIOGAMGTest,TestExecute_Samples) {
    TriggeredIOGAMTest test;
    ASSERT_TRUE(test.TestExecute_Samples());
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
