/**
 * @file MARTePvFieldGTest.cpp
 * @brief Source file for class MARTePvFieldGTest
 * @date 09/09/2019
 * @author Giuseppe Ferro
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
 * the class MARTePvFieldGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTePvFieldTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#if 0
"            NELM = 5"
"            PREC = 4"
"            EGU = \"volt\""
"            HOPR = 10.0"
"            LOPR = -10.0"
"            HIHI = 9.0"
"            HIGH = 8.0"
"            LOW = 0.5"
"            LOLO = 0.1"
"            HHSV =  \"MAJOR\""
"            HSV = \"MINOR\""
"            LSV = \"MINOR\""
"            LLSV = \"MAJOR\""
"            HYST = 0.0001"
"            ADEL = 0.0001"
"            MDEL = 0.01";
#endif

TEST(MARTePvFieldGTest,Testread_prec) {
    MARTePvFieldTest test;
    uint32 testPrec=4;
    ASSERT_TRUE(test.Testread("PREC", testPrec));
}
TEST(MARTePvFieldGTest,Testread_nelm) {
    MARTePvFieldTest test;
    uint32 testNelm=5;
    ASSERT_TRUE(test.Testread("NELM", testNelm));
}

TEST(MARTePvFieldGTest,Testread_egu) {
    MARTePvFieldTest test;
    const char8 * testEgu="volt";
    ASSERT_TRUE(test.Testread_string("EGU", testEgu));
}

TEST(MARTePvFieldGTest,Testread_hopr) {
    MARTePvFieldTest test;
    float64 testHopr=10.;
    ASSERT_TRUE(test.Testread("HOPR", testHopr));
}

TEST(MARTePvFieldGTest,Testread_lopr) {
    MARTePvFieldTest test;
    float64 testLopr=-10.;
    ASSERT_TRUE(test.Testread("LOPR", testLopr));
}
TEST(MARTePvFieldGTest,Testread_hihi) {
    MARTePvFieldTest test;
    float64 testHihi=9.;
    ASSERT_TRUE(test.Testread("HIHI", testHihi));
}
TEST(MARTePvFieldGTest,Testread_high) {
    MARTePvFieldTest test;
    float64 testHigh=8.;
    ASSERT_TRUE(test.Testread("HIGH", testHigh));
}
TEST(MARTePvFieldGTest,Testread_low) {
    MARTePvFieldTest test;
    float64 testLow=0.5;
    ASSERT_TRUE(test.Testread("LOW", testLow));
}
TEST(MARTePvFieldGTest,Testread_lolo) {
    MARTePvFieldTest test;
    uint32 testLolo=0.1;
    ASSERT_TRUE(test.Testread("LOLO", testLolo));
}


TEST(MARTePvFieldGTest,Testread_hhsv) {
    MARTePvFieldTest test;
    const char8 * testHhsv="MAJOR";
    ASSERT_TRUE(test.Testread_string("HHSV", testHhsv));
}
TEST(MARTePvFieldGTest,Testread_hsv) {
    MARTePvFieldTest test;
    const char8 * testHsv="MINOR";
    ASSERT_TRUE(test.Testread_string("HSV", testHsv));
}

TEST(MARTePvFieldGTest,Testread_lsv) {
    MARTePvFieldTest test;
    const char8 * testLsv="MINOR";
    ASSERT_TRUE(test.Testread_string("LSV", testLsv));
}

TEST(MARTePvFieldGTest,Testread_llsv) {
    MARTePvFieldTest test;
    const char8 * testLlsv="MAJOR";
    ASSERT_TRUE(test.Testread_string("LLSV", testLlsv));
}

TEST(MARTePvFieldGTest,Testread_hyst) {
    MARTePvFieldTest test;
    float64 testHyst=0.0001;
    ASSERT_TRUE(test.Testread("HYST", testHyst));
}
TEST(MARTePvFieldGTest,Testread_adel) {
    MARTePvFieldTest test;
    float64 testAdel=0.0001;
    ASSERT_TRUE(test.Testread("ADEL", testAdel));
}
TEST(MARTePvFieldGTest,Testread_mdel) {
    MARTePvFieldTest test;
    float64 testMdel=0.01;
    ASSERT_TRUE(test.Testread("MDEL", testMdel));
}

TEST(MARTePvFieldGTest,Testwrite_prec) {
    MARTePvFieldTest test;
    uint8 testPrec=10;
    ASSERT_TRUE(test.Testwrite("PREC", testPrec));
}


TEST(MARTePvFieldGTest,Testwrite_egu) {
    MARTePvFieldTest test;
    const char8 * testEgu="ampere";
    ASSERT_TRUE(test.Testwrite_string("EGU", testEgu));
}

TEST(MARTePvFieldGTest,Testwrite_hopr) {
    MARTePvFieldTest test;
    float64 testHopr=20.;
    ASSERT_TRUE(test.Testwrite("HOPR", testHopr));
}

TEST(MARTePvFieldGTest,Testwrite_lopr) {
    MARTePvFieldTest test;
    float64 testLopr=-20.;
    ASSERT_TRUE(test.Testwrite("LOPR", testLopr));
}
TEST(MARTePvFieldGTest,Testwrite_hihi) {
    MARTePvFieldTest test;
    float64 testHihi=18.;
    ASSERT_TRUE(test.Testwrite("HIHI", testHihi));
}
TEST(MARTePvFieldGTest,Testwrite_high) {
    MARTePvFieldTest test;
    float64 testHigh=16.;
    ASSERT_TRUE(test.Testwrite("HIGH", testHigh));
}
TEST(MARTePvFieldGTest,Testwrite_low) {
    MARTePvFieldTest test;
    float64 testLow=1.0;
    ASSERT_TRUE(test.Testwrite("LOW", testLow));
}
TEST(MARTePvFieldGTest,Testwrite_lolo) {
    MARTePvFieldTest test;
    uint32 testLolo=0.2;
    ASSERT_TRUE(test.Testwrite("LOLO", testLolo));
}


TEST(MARTePvFieldGTest,Testwrite_hhsv) {
    MARTePvFieldTest test;
    const char8 * testHhsv="NO_ALARM";
    ASSERT_TRUE(test.Testwrite_string("HHSV", testHhsv));
}
TEST(MARTePvFieldGTest,Testwrite_hsv) {
    MARTePvFieldTest test;
    const char8 * testHsv="INVALID";
    ASSERT_TRUE(test.Testwrite_string("HSV", testHsv));
}

TEST(MARTePvFieldGTest,Testwrite_lsv) {
    MARTePvFieldTest test;
    const char8 * testLsv="MAJOR";
    ASSERT_TRUE(test.Testwrite_string("LSV", testLsv));
}

TEST(MARTePvFieldGTest,Testwrite_llsv) {
    MARTePvFieldTest test;
    const char8 * testLlsv="MINOR";
    ASSERT_TRUE(test.Testwrite_string("LLSV", testLlsv));
}

TEST(MARTePvFieldGTest,Testwrite_hyst) {
    MARTePvFieldTest test;
    float64 testHyst=0.0002;
    ASSERT_TRUE(test.Testwrite("HYST", testHyst));
}
TEST(MARTePvFieldGTest,Testwrite_adel) {
    MARTePvFieldTest test;
    float64 testAdel=0.0002;
    ASSERT_TRUE(test.Testwrite("ADEL", testAdel));
}
TEST(MARTePvFieldGTest,Testwrite_mdel) {
    MARTePvFieldTest test;
    float64 testMdel=0.02;
    ASSERT_TRUE(test.Testwrite("MDEL", testMdel));
}


TEST(MARTePvFieldGTest,TestReadDirect_prec) {
    MARTePvFieldTest test;
    uint32 testPrec=4;
    ASSERT_TRUE(test.TestReadDirect("PREC", testPrec));
}
TEST(MARTePvFieldGTest,TestReadDirect_nelm) {
    MARTePvFieldTest test;
    uint32 testNelm=5;
    ASSERT_TRUE(test.TestReadDirect("NELM", testNelm));
}

TEST(MARTePvFieldGTest,TestReadDirect_egu) {
    MARTePvFieldTest test;
    StreamString testEgu="volt";
    ASSERT_TRUE(test.TestReadDirect("EGU", testEgu));
}

TEST(MARTePvFieldGTest,TestReadDirect_hopr) {
    MARTePvFieldTest test;
    float64 testHopr=10.;
    ASSERT_TRUE(test.TestReadDirect("HOPR", testHopr));
}

TEST(MARTePvFieldGTest,TestReadDirect_lopr) {
    MARTePvFieldTest test;
    float64 testLopr=-10.;
    ASSERT_TRUE(test.TestReadDirect("LOPR", testLopr));
}
TEST(MARTePvFieldGTest,TestReadDirect_hihi) {
    MARTePvFieldTest test;
    float64 testHihi=9.;
    ASSERT_TRUE(test.TestReadDirect("HIHI", testHihi));
}
TEST(MARTePvFieldGTest,TestReadDirect_high) {
    MARTePvFieldTest test;
    float64 testHigh=8.;
    ASSERT_TRUE(test.TestReadDirect("HIGH", testHigh));
}
TEST(MARTePvFieldGTest,TestReadDirect_low) {
    MARTePvFieldTest test;
    float64 testLow=0.5;
    ASSERT_TRUE(test.TestReadDirect("LOW", testLow));
}
TEST(MARTePvFieldGTest,TestReadDirect_lolo) {
    MARTePvFieldTest test;
    uint32 testLolo=0.1;
    ASSERT_TRUE(test.TestReadDirect("LOLO", testLolo));
}


TEST(MARTePvFieldGTest,TestReadDirect_hhsv) {
    MARTePvFieldTest test;
    StreamString testHhsv="MAJOR";
    ASSERT_TRUE(test.TestReadDirect("HHSV", testHhsv));
}
TEST(MARTePvFieldGTest,TestReadDirect_hsv) {
    MARTePvFieldTest test;
    StreamString testHsv="MINOR";
    ASSERT_TRUE(test.TestReadDirect("HSV", testHsv));
}

TEST(MARTePvFieldGTest,TestReadDirect_lsv) {
    MARTePvFieldTest test;
    StreamString testLsv="MINOR";
    ASSERT_TRUE(test.TestReadDirect("LSV", testLsv));
}

TEST(MARTePvFieldGTest,TestReadDirect_llsv) {
    MARTePvFieldTest test;
    StreamString testLlsv="MAJOR";
    ASSERT_TRUE(test.TestReadDirect("LLSV", testLlsv));
}

TEST(MARTePvFieldGTest,TestReadDirect_hyst) {
    MARTePvFieldTest test;
    float64 testHyst=0.0001;
    ASSERT_TRUE(test.TestReadDirect("HYST", testHyst));
}
TEST(MARTePvFieldGTest,TestReadDirect_adel) {
    MARTePvFieldTest test;
    float64 testAdel=0.0001;
    ASSERT_TRUE(test.TestReadDirect("ADEL", testAdel));
}
TEST(MARTePvFieldGTest,TestReadDirect_mdel) {
    MARTePvFieldTest test;
    float64 testMdel=0.01;
    ASSERT_TRUE(test.TestReadDirect("MDEL", testMdel));
}


TEST(MARTePvFieldGTest,TestWriteDirect_prec) {
    MARTePvFieldTest test;
    uint32 testPrec=10;
    ASSERT_TRUE(test.TestWriteDirect("PREC", testPrec));
}

TEST(MARTePvFieldGTest,TestWriteDirect_egu) {
    MARTePvFieldTest test;
    StreamString testEgu="ampere";
    ASSERT_TRUE(test.TestWriteDirect("EGU", testEgu));
}

TEST(MARTePvFieldGTest,TestWriteDirect_hopr) {
    MARTePvFieldTest test;
    float64 testHopr=20.;
    ASSERT_TRUE(test.TestWriteDirect("HOPR", testHopr));
}

TEST(MARTePvFieldGTest,TestWriteDirect_lopr) {
    MARTePvFieldTest test;
    float64 testLopr=-20.;
    ASSERT_TRUE(test.TestWriteDirect("LOPR", testLopr));
}
TEST(MARTePvFieldGTest,TestWriteDirect_hihi) {
    MARTePvFieldTest test;
    float64 testHihi=18.;
    ASSERT_TRUE(test.TestWriteDirect("HIHI", testHihi));
}
TEST(MARTePvFieldGTest,TestWriteDirect_high) {
    MARTePvFieldTest test;
    float64 testHigh=16.;
    ASSERT_TRUE(test.TestWriteDirect("HIGH", testHigh));
}
TEST(MARTePvFieldGTest,TestWriteDirect_low) {
    MARTePvFieldTest test;
    float64 testLow=1.0;
    ASSERT_TRUE(test.TestWriteDirect("LOW", testLow));
}
TEST(MARTePvFieldGTest,TestWriteDirect_lolo) {
    MARTePvFieldTest test;
    float64 testLolo=0.2;
    ASSERT_TRUE(test.TestWriteDirect("LOLO", testLolo));
}


TEST(MARTePvFieldGTest,TestWriteDirect_hhsv) {
    MARTePvFieldTest test;
    StreamString testHhsv="MINOR";
    ASSERT_TRUE(test.TestWriteDirect("HHSV", testHhsv));
}
TEST(MARTePvFieldGTest,TestWriteDirect_hsv) {
    MARTePvFieldTest test;
    StreamString testHsv="MAJOR";
    ASSERT_TRUE(test.TestWriteDirect("HSV", testHsv));
}

TEST(MARTePvFieldGTest,TestWriteDirect_lsv) {
    MARTePvFieldTest test;
    StreamString testLsv="INVALID";
    ASSERT_TRUE(test.TestWriteDirect("LSV", testLsv));
}

TEST(MARTePvFieldGTest,TestWriteDirect_llsv) {
    MARTePvFieldTest test;
    StreamString testLlsv="MINOR";
    ASSERT_TRUE(test.TestWriteDirect("LLSV", testLlsv));
}

TEST(MARTePvFieldGTest,TestWriteDirect_hyst) {
    MARTePvFieldTest test;
    float64 testHyst=0.0002;
    ASSERT_TRUE(test.TestWriteDirect("HYST", testHyst));
}
TEST(MARTePvFieldGTest,TestWriteDirect_adel) {
    MARTePvFieldTest test;
    float64 testAdel=0.0002;
    ASSERT_TRUE(test.TestWriteDirect("ADEL", testAdel));
}
TEST(MARTePvFieldGTest,TestWriteDirect_mdel) {
    MARTePvFieldTest test;
    float64 testMdel=0.02;
    ASSERT_TRUE(test.TestWriteDirect("MDEL", testMdel));
}


TEST(MARTePvFieldGTest,Testscan) {
    MARTePvFieldTest test;
    ASSERT_TRUE(test.Testscan());
}


TEST(MARTePvFieldGTest,TestwriteNotify_prec) {
    MARTePvFieldTest test;
    uint8 testPrec=10;
    ASSERT_TRUE(test.TestwriteNotify("PREC", testPrec));
}


TEST(MARTePvFieldGTest,TestwriteNotify_egu) {
    MARTePvFieldTest test;
    const char8 * testEgu="ampere";
    ASSERT_TRUE(test.TestwriteNotify_string("EGU", testEgu));
}

TEST(MARTePvFieldGTest,TestwriteNotify_hopr) {
    MARTePvFieldTest test;
    float64 testHopr=20.;
    ASSERT_TRUE(test.TestwriteNotify("HOPR", testHopr));
}

TEST(MARTePvFieldGTest,TestwriteNotify_lopr) {
    MARTePvFieldTest test;
    float64 testLopr=-20.;
    ASSERT_TRUE(test.TestwriteNotify("LOPR", testLopr));
}
TEST(MARTePvFieldGTest,TestwriteNotify_hihi) {
    MARTePvFieldTest test;
    float64 testHihi=18.;
    ASSERT_TRUE(test.TestwriteNotify("HIHI", testHihi));
}
TEST(MARTePvFieldGTest,TestwriteNotify_high) {
    MARTePvFieldTest test;
    float64 testHigh=16.;
    ASSERT_TRUE(test.TestwriteNotify("HIGH", testHigh));
}
TEST(MARTePvFieldGTest,TestwriteNotify_low) {
    MARTePvFieldTest test;
    float64 testLow=1.0;
    ASSERT_TRUE(test.TestwriteNotify("LOW", testLow));
}
TEST(MARTePvFieldGTest,TestwriteNotify_lolo) {
    MARTePvFieldTest test;
    uint32 testLolo=0.2;
    ASSERT_TRUE(test.TestwriteNotify("LOLO", testLolo));
}


TEST(MARTePvFieldGTest,TestwriteNotify_hhsv) {
    MARTePvFieldTest test;
    const char8 * testHhsv="NO_ALARM";
    ASSERT_TRUE(test.TestwriteNotify_string("HHSV", testHhsv));
}
TEST(MARTePvFieldGTest,TestwriteNotify_hsv) {
    MARTePvFieldTest test;
    const char8 * testHsv="INVALID";
    ASSERT_TRUE(test.TestwriteNotify_string("HSV", testHsv));
}

TEST(MARTePvFieldGTest,TestwriteNotify_lsv) {
    MARTePvFieldTest test;
    const char8 * testLsv="MAJOR";
    ASSERT_TRUE(test.TestwriteNotify_string("LSV", testLsv));
}

TEST(MARTePvFieldGTest,TestwriteNotify_llsv) {
    MARTePvFieldTest test;
    const char8 * testLlsv="MINOR";
    ASSERT_TRUE(test.TestwriteNotify_string("LLSV", testLlsv));
}

TEST(MARTePvFieldGTest,TestwriteNotify_hyst) {
    MARTePvFieldTest test;
    float64 testHyst=0.0002;
    ASSERT_TRUE(test.TestwriteNotify("HYST", testHyst));
}
TEST(MARTePvFieldGTest,TestwriteNotify_adel) {
    MARTePvFieldTest test;
    float64 testAdel=0.0002;
    ASSERT_TRUE(test.TestwriteNotify("ADEL", testAdel));
}
TEST(MARTePvFieldGTest,TestwriteNotify_mdel) {
    MARTePvFieldTest test;
    float64 testMdel=0.02;
    ASSERT_TRUE(test.TestwriteNotify("MDEL", testMdel));
}


TEST(MARTePvFieldGTest,TestSetPvFather) {
    MARTePvFieldTest test;
    ASSERT_TRUE(test.TestSetPvFather());
}


