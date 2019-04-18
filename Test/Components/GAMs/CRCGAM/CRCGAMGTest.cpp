/**
 * @file CRCGAMGTest.cpp
 * @brief Source file for class CRCGAMGTest
 * @date 30/10/2018
 * @author Luca Porzio
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
 * the class CRCGAMGTest (public, protected, and private). Be aware that some 
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

#include "CRCGAM.h"
#include "CRCGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(CRCGAMGTest,TestConstructor) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(CRCGAMGTest,TestInitialise) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(CRCGAMGTest,TestInitialiseMissingParameters) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingParameters());
}

TEST(CRCGAMGTest,TestInitialiseMissingPolynomial) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingPolynomial());
}

TEST(CRCGAMGTest,TestInitialiseMissingInitialCRCValue) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingInitialCRCValue());
}

TEST(CRCGAMGTest,TestInitialiseMissingInverted) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestInitialiseMissingInverted());
}

TEST(CRCGAMGTest,TestInitialiseWrongInverted) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestInitialiseWrongInverted());
}

TEST(CRCGAMGTest,TestSetupUint8) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestSetupUint8());
}


TEST(CRCGAMGTest,TestSetupUint16) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestSetupUint16());
}

TEST(CRCGAMGTest,TestSetupUint32) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestSetupUint32());
}

TEST(CRCGAMGTest,TestSetupWrongNumberOfInput) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongNumberOfInput());
}

TEST(CRCGAMGTest,TestSetupWrongNumberOfOutput) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongNumberOfOutput());
}

TEST(CRCGAMGTest,TestSetupWrongOutputType) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestSetupWrongOutputType());
}

TEST(CRCGAMGTest,TestExecuteUint8) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestExecuteUint8());
}

TEST(CRCGAMGTest,TestExecuteUint16) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestExecuteUint16());
}

TEST(CRCGAMGTest,TestExecuteUint32) {
    CRCGAMTest test;
    ASSERT_TRUE(test.TestExecuteUint32());
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
