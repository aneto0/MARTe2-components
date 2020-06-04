/**
 * @file MathExpressionGAMGTest.cpp
 * @brief Source file for class IOGAMGTest
 * @date 03/06/2020
 * @author RFX
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
 * the class IOGAMGTest (public, protected, and private). Be aware that some
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

#include "MathExpressionGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MathExpressionGAMGTest,TestConstructor) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MathExpressionGAMGTest,TestInitialise) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestInitialise("Out1 = In1 + In2;"));
}

TEST(MathExpressionGAMGTest,TestInitialise_Failed_NoExpressionLeaf) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(!test.TestInitialise(""));
}

TEST(MathExpressionGAMGTest,TestInitialise_Failed_BadExpression) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(!test.TestInitialise("Out1 ="));
}

TEST(MathExpressionGAMGTest,TestSetup) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(MathExpressionGAMGTest,TestSetup_Failed_InputSignalMissingVariable) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_InputSignalMissingVariable());
}

TEST(MathExpressionGAMGTest,TestSetup_Failed_OutputSignalMissingVariable) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_OutputSignalMissingVariable());
}

TEST(MathExpressionGAMGTest,TestSetup_Failed_VariableMissingInputSignal) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_VariableMissingInputSignal());
}

TEST(MathExpressionGAMGTest,TestSetup_Succeed_VariableMissingOutputSignal) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestSetup_Succeed_VariableMissingOutputSignal());
}

TEST(MathExpressionGAMGTest,TestSetup_Failed_NonScalarInput) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_NonScalarInput());
}

TEST(MathExpressionGAMGTest,TestSetup_Failed_NonScalarOutput) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestSetup_Failed_NonScalarOutput());
}

TEST(MathExpressionGAMGTest,TestMemory) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestMemory());
}

TEST(MathExpressionGAMGTest,TestExecute_SingleExpression) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestExecute_SingleExpression());
}

TEST(MathExpressionGAMGTest,TestExecute_MultipleExpressions) {
    MathExpressionGAMTest test;
    ASSERT_TRUE(test.TestExecute_MultipleExpressions());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

