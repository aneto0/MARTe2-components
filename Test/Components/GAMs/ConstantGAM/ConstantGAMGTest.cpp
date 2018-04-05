/**
 * @file ConstantGAMGTest.cpp
 * @brief Source file for class ConstantGAMGTest
 * @date 22/03/2018
 * @author Bertrand Bauvir
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
 * the class ConstantGAMGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConstantGAM.h"
#include "ConstantGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ConstantGAMGTest,TestConstructor) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ConstantGAMGTest,TestExecute) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(ConstantGAMGTest,TestSetup) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(ConstantGAMGTest,TestSetup_OutputSignal_Scalar) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetup_OutputSignal_Scalar());
}

TEST(ConstantGAMGTest,TestSetup_OutputSignal_int8) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetup_OutputSignal_int8());
}

TEST(ConstantGAMGTest,TestSetup_Error_InputSignal) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetup_Error_InputSignal());
}

TEST(ConstantGAMGTest,TestSetup_Error_NoOutputSignal) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetup_Error_NoOutputSignal());
}

TEST(ConstantGAMGTest,TestSetup_Error_NoDefault) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetup_Error_NoDefault());
}

TEST(ConstantGAMGTest,TestSetOutput_int8) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_int8());
}

TEST(ConstantGAMGTest,TestSetOutput_1Darray) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_1Darray());
}

TEST(ConstantGAMGTest,TestSetOutput_2Darray) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_2Darray());
}

TEST(ConstantGAMGTest,TestSetOutput_WithIndex) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_WithIndex());
}

TEST(ConstantGAMGTest,TestSetOutput_Error_NoStructuredDataI) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_Error_NoStructuredDataI());
}

TEST(ConstantGAMGTest,TestSetOutput_Error_NoName) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_Error_NoName());
}

TEST(ConstantGAMGTest,TestSetOutput_Error_InvalidName) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_Error_InvalidName());
}

TEST(ConstantGAMGTest,TestSetOutput_Error_NoValue) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_Error_NoValue());
}

TEST(ConstantGAMGTest,TestSetOutput_Error_InvalidValue) {
    ConstantGAMTest test;
    ASSERT_TRUE(test.TestSetOutput_Error_InvalidValue());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

