/**
 * @file BaseLib2GAMGTest.cpp
 * @brief Source file for class BaseLib2GAMGTest
 * @date 2/12/2016
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
 * the class BaseLib2GAMGTest (public, protected, and private). Be aware that some 
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
#include "BaseLib2GAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BaseLib2GAMGTest,TestConstructor) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BaseLib2GAMGTest,TestInitialise) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(BaseLib2GAMGTest,TestInitialise_False_NoGAMName) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoGAMName());
}

TEST(BaseLib2GAMGTest,TestInitialise_False_WrongGAMName) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestInitialise_False_WrongGAMName());
}

TEST(BaseLib2GAMGTest,TestInitialise_False_NoGAMFunctionNumber) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoGAMFunctionNumber());
}

TEST(BaseLib2GAMGTest,TestInitialise_False_NoBaseLib2Config) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoBaseLib2Config());
}

TEST(BaseLib2GAMGTest,TestInitialise_False_WrongBaseLib2Config) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestInitialise_False_WrongBaseLib2Config());
}

TEST(BaseLib2GAMGTest,TestSetup) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(BaseLib2GAMGTest,TestSetup_InputSignals_Mismatch) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestSetup_InputSignals_Mismatch());
}

TEST(BaseLib2GAMGTest,TestSetup_OutputSignals_Mismatch) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestSetup_OutputSignals_Mismatch());
}

TEST(BaseLib2GAMGTest,TestExecute) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(BaseLib2GAMGTest,TestExecute_Arrays) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestExecute_Arrays());
}

TEST(BaseLib2GAMGTest,TestTranslateSignalName) {
    BaseLib2GAMTest test;
    ASSERT_TRUE(test.TestTranslateSignalName());
}
