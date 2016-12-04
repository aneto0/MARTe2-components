/**
 * @file BufferGAMGTest.cpp
 * @brief Source file for class BufferGAMGTest
 * @date 31/10/2016
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
 * the class BufferGAMGTest (public, protected, and private). Be aware that some 
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
#include "BufferGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BufferGAMGTest,TestConstructor) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BufferGAMGTest,TestSetup) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(BufferGAMGTest,TestSetup_False_DifferentType_Input) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentType_Input());
}

TEST(BufferGAMGTest,TestSetup_False_DifferentType_Output) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentType_Output());
}

TEST(BufferGAMGTest,TestSetup_False_LessInputs) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_LessInputs());
}

TEST(BufferGAMGTest,TestSetup_False_LessOutputs) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_LessOutputs());
}

TEST(BufferGAMGTest,TestSetup_False_DifferentSize_Input) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentSize_Input());
}

TEST(BufferGAMGTest,TestSetup_False_DifferentSize_Output) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentSize_Output());
}

TEST(BufferGAMGTest,TestExecute) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(BufferGAMGTest,TestExecute_Samples) {
    BufferGAMTest test;
    ASSERT_TRUE(test.TestExecute_Samples());
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
