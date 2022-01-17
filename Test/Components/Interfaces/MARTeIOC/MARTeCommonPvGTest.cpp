/**
 * @file MARTeCommonPvGTest.cpp
 * @brief Source file for class MARTeCommonPvGTest
 * @date 27/09/2019
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
 * the class MARTeCommonPvGTest (public, protected, and private). Be aware that some
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

#include "MARTeCommonPvTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MARTeCommonPvGTest,TestInitialise) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(MARTeCommonPvGTest,TestInitialise_InvalidHHSV) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidHHSV());
}

TEST(MARTeCommonPvGTest,TestInitialise_InvalidHSV) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidHSV());
}

TEST(MARTeCommonPvGTest,TestInitialise_InvalidLSV) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidLSV());
}

TEST(MARTeCommonPvGTest,TestInitialise_InvalidLLSV) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidLLSV());
}

TEST(MARTeCommonPvGTest,Testread_Precision) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Precision());
}

TEST(MARTeCommonPvGTest,Testread_HighLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_HighLimit());
}

TEST(MARTeCommonPvGTest,Testread_LowLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_LowLimit());
}

TEST(MARTeCommonPvGTest,Testread_HighAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_HighAlarm());
}

TEST(MARTeCommonPvGTest,Testread_LowAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_LowAlarm());
}

TEST(MARTeCommonPvGTest,Testread_HighWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_HighWarning());
}

TEST(MARTeCommonPvGTest,Testread_LowWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_LowWarning());
}

TEST(MARTeCommonPvGTest,Testread_Units) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Units());
}

TEST(MARTeCommonPvGTest,Testread_Enums) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Enums());
}

TEST(MARTeCommonPvGTest,Testread_Ackt) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Ackt());
}

TEST(MARTeCommonPvGTest,Testread_Acks) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Acks());
}

TEST(MARTeCommonPvGTest,Testread_NumberOfElements) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_NumberOfElements());
}

TEST(MARTeCommonPvGTest,Testread_Timestamp) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Timestamp());
}

TEST(MARTeCommonPvGTest,Testread_Hyst) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Hyst());
}

TEST(MARTeCommonPvGTest,Testread_Mdel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Mdel());
}

TEST(MARTeCommonPvGTest,Testread_Adel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Adel());
}

TEST(MARTeCommonPvGTest,Testread_Hhsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Hhsv());
}

TEST(MARTeCommonPvGTest,Testread_Hsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Hsv());
}

TEST(MARTeCommonPvGTest,Testread_Lsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Lsv());
}

TEST(MARTeCommonPvGTest,Testread_Llsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Llsv());
}

TEST(MARTeCommonPvGTest,Testread_Sevr) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Sevr());
}

TEST(MARTeCommonPvGTest,Testread_Stat) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_Stat());
}

TEST(MARTeCommonPvGTest,Testwrite_Ackt) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Ackt());
}

TEST(MARTeCommonPvGTest,Testwrite_Acks) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Acks());
}


TEST(MARTeCommonPvGTest,Testwrite_Precision) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Precision());
}

TEST(MARTeCommonPvGTest,Testwrite_HighLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_HighLimit());
}
TEST(MARTeCommonPvGTest,Testwrite_LowLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_LowLimit());
}

TEST(MARTeCommonPvGTest,Testwrite_HighAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_HighAlarm());
}

TEST(MARTeCommonPvGTest,Testwrite_LowAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_LowAlarm());
}

TEST(MARTeCommonPvGTest,Testwrite_HighWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_HighWarning());
}

TEST(MARTeCommonPvGTest,Testwrite_LowWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_LowWarning());
}

TEST(MARTeCommonPvGTest,Testwrite_Units) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Units());
}

TEST(MARTeCommonPvGTest,Testwrite_Enums) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Enums());
}

TEST(MARTeCommonPvGTest,Testwrite_Timestamp) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Timestamp());
}

TEST(MARTeCommonPvGTest,Testwrite_Hyst) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Hyst());
}
TEST(MARTeCommonPvGTest,Testwrite_Mdel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Mdel());
}

TEST(MARTeCommonPvGTest,Testwrite_Adel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Adel());
}

TEST(MARTeCommonPvGTest,Testwrite_Hhsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Hhsv());
}

TEST(MARTeCommonPvGTest,Testwrite_Hsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Hsv());
}
TEST(MARTeCommonPvGTest,Testwrite_Lsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Lsv());
}

TEST(MARTeCommonPvGTest,Testwrite_Llsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_Llsv());
}


TEST(MARTeCommonPvGTest,Testwrite_False1) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_False1());
}

TEST(MARTeCommonPvGTest,Testwrite_False2) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_False2());
}

TEST(MARTeCommonPvGTest,Testwrite_saturated) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_saturated());
}

TEST(MARTeCommonPvGTest,Testread_uint8) {
    MARTeCommonPvTest test;
    uint8 x =1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testread_int8) {
    MARTeCommonPvTest test;
    int8 x =-1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testread_uint16) {
    MARTeCommonPvTest test;
    uint16 x =1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testread_int16) {
    MARTeCommonPvTest test;
    int16 x =-1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testread_uint32) {
    MARTeCommonPvTest test;
    uint32 x =1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testread_int32) {
    MARTeCommonPvTest test;
    int32 x =-1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testread_float32) {
    MARTeCommonPvTest test;
    float32 x =1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testread_float64) {
    MARTeCommonPvTest test;
    float64 x =-1;
    ASSERT_TRUE(test.Testread(x));
}

TEST(MARTeCommonPvGTest,Testwrite_uint8) {
    MARTeCommonPvTest test;
    uint8 x =1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_int8) {
    MARTeCommonPvTest test;
    int8 x =-1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_uint16) {
    MARTeCommonPvTest test;
    uint16 x =1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_int16) {
    MARTeCommonPvTest test;
    int16 x =-1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_uint32) {
    MARTeCommonPvTest test;
    uint32 x =1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_int32) {
    MARTeCommonPvTest test;
    int32 x =-1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_float32) {
    MARTeCommonPvTest test;
    float32 x =1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_float64) {
    MARTeCommonPvTest test;
    float64 x =-1;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_overflow) {
    MARTeCommonPvTest test;
    float64 x =11;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testwrite_underflow) {
    MARTeCommonPvTest test;
    float64 x =-11;
    ASSERT_TRUE(test.Testwrite(x));
}

TEST(MARTeCommonPvGTest,Testread_vector_uint8) {
    MARTeCommonPvTest test;
    uint8 x[] = {1, 2, 3, 4, 5};
    ASSERT_TRUE(test.Testread_vector(x));
}

TEST(MARTeCommonPvGTest,Testwrite_vector_int8) {
    MARTeCommonPvTest test;
    int8 x[] = {1, -2, 3, -4, 5};
    ASSERT_TRUE(test.Testwrite_vector(x));
}

TEST(MARTeCommonPvGTest,Testread_vector_uint16) {
    MARTeCommonPvTest test;
    uint16 x[] = {1, 2, 3, 4, 5};
    ASSERT_TRUE(test.Testread_vector(x));
}

TEST(MARTeCommonPvGTest,Testwrite_vector_int16) {
    MARTeCommonPvTest test;
    int16 x[] = {1, -2, 3, -4, 5};
    ASSERT_TRUE(test.Testwrite_vector(x));
}

TEST(MARTeCommonPvGTest,Testread_vector_uint32) {
    MARTeCommonPvTest test;
    uint32 x[] = {1, 2, 3, 4, 5};
    ASSERT_TRUE(test.Testread_vector(x));
}

TEST(MARTeCommonPvGTest,Testwrite_vector_int32) {
    MARTeCommonPvTest test;
    int32 x[] = {1, -2, 3, -4, 5};
    ASSERT_TRUE(test.Testwrite_vector(x));
}

TEST(MARTeCommonPvGTest,Testread_vector_float32) {
    MARTeCommonPvTest test;
    float32 x[] = {1, 2, 3, 4, 5};
    ASSERT_TRUE(test.Testread_vector(x));
}

TEST(MARTeCommonPvGTest,Testwrite_vector_float64) {
    MARTeCommonPvTest test;
    float64 x[] = {1, -2, 3, -4, 5};
    ASSERT_TRUE(test.Testwrite_vector(x));
}


TEST(MARTeCommonPvGTest,Testread_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_string());
}

TEST(MARTeCommonPvGTest,Testwrite_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_string());
}

TEST(MARTeCommonPvGTest,Testread_vector_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testread_vector_string());
}

TEST(MARTeCommonPvGTest,Testwrite_vector_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testwrite_vector_string());
}

TEST(MARTeCommonPvGTest,TestWriteDirect_uint8) {
    MARTeCommonPvTest test;
    uint8 x=1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_int8) {
    MARTeCommonPvTest test;
    int8 x=-1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_uint16) {
    MARTeCommonPvTest test;
    uint16 x=1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_int16) {
    MARTeCommonPvTest test;
    int16 x=-1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_uint32) {
    MARTeCommonPvTest test;
    uint32 x=1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_int32) {
    MARTeCommonPvTest test;
    int32 x=-1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_float32) {
    MARTeCommonPvTest test;
    float32 x=1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_float64) {
    MARTeCommonPvTest test;
    float64 x=-1;
    ASSERT_TRUE(test.TestWriteDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_uint8) {
    MARTeCommonPvTest test;
    uint8 x=1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_int8) {
    MARTeCommonPvTest test;
    int8 x=-1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_uint16) {
    MARTeCommonPvTest test;
    uint16 x=1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_int16) {
    MARTeCommonPvTest test;
    int16 x=-1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_uint32) {
    MARTeCommonPvTest test;
    uint32 x=1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_int32) {
    MARTeCommonPvTest test;
    int32 x=-1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_float32) {
    MARTeCommonPvTest test;
    float32 x=1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestReadDirect_float64) {
    MARTeCommonPvTest test;
    float64 x=-1;
    ASSERT_TRUE(test.TestReadDirect(x));
}

TEST(MARTeCommonPvGTest,TestWriteDirect_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirect_string());
}

TEST(MARTeCommonPvGTest,TestWriteDirect_saturated) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirect_saturated());
}

TEST(MARTeCommonPvGTest,TestReadDirect_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirect_string());
}

TEST(MARTeCommonPvGTest,TestWriteDirect_False) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirect_False());
}

TEST(MARTeCommonPvGTest,TestWriteDirect_vector_False) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirect_vector_False());
}

TEST(MARTeCommonPvGTest,TestWriteDirect_vector_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirect_vector_string());
}

TEST(MARTeCommonPvGTest,TestReadDirect_vector_string) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirect_vector_string());
}

TEST(MARTeCommonPvGTest,TestReadDirect_False) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirect_False());
}

TEST(MARTeCommonPvGTest,Testscan) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.Testscan());
}

TEST(MARTeCommonPvGTest,TestwriteNotify_uint8) {
    MARTeCommonPvTest test;
    uint8 x =1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}

TEST(MARTeCommonPvGTest,TestwriteNotify_int8) {
    MARTeCommonPvTest test;
    int8 x =-1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}

TEST(MARTeCommonPvGTest,TestwriteNotify_uint16) {
    MARTeCommonPvTest test;
    uint16 x =1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}

TEST(MARTeCommonPvGTest,TestwriteNotify_int16) {
    MARTeCommonPvTest test;
    int16 x =-1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}

TEST(MARTeCommonPvGTest,TestwriteNotify_uint32) {
    MARTeCommonPvTest test;
    uint32 x =1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}

TEST(MARTeCommonPvGTest,TestwriteNotify_int32) {
    MARTeCommonPvTest test;
    int32 x =-1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}

TEST(MARTeCommonPvGTest,TestwriteNotify_float32) {
    MARTeCommonPvTest test;
    float32 x =1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}

TEST(MARTeCommonPvGTest,TestwriteNotify_float64) {
    MARTeCommonPvTest test;
    float64 x =-1;
    ASSERT_TRUE(test.TestwriteNotify(x));
}


TEST(MARTeCommonPvGTest,TestReadDirectPrecision) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectPrecision());
}

TEST(MARTeCommonPvGTest,TestReadDirectHighLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectHighLimit());
}

TEST(MARTeCommonPvGTest,TestReadDirectLowLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectLowLimit());
}

TEST(MARTeCommonPvGTest,TestReadDirectHighAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectHighAlarm());
}

TEST(MARTeCommonPvGTest,TestReadDirectLowAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectLowAlarm());
}

TEST(MARTeCommonPvGTest,TestReadDirectHighWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectHighWarning());
}

TEST(MARTeCommonPvGTest,TestReadDirectLowWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectLowWarning());
}
TEST(MARTeCommonPvGTest,TestReadDirectUnits) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectUnits());
}

TEST(MARTeCommonPvGTest,TestReadDirectEnums) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectEnums());
}

TEST(MARTeCommonPvGTest,TestReadDirectAckt) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectAckt());
}

TEST(MARTeCommonPvGTest,TestReadDirectAcks) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectAcks());
}

TEST(MARTeCommonPvGTest,TestReadDirectElementCount) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectElementCount());
}

TEST(MARTeCommonPvGTest,TestReadDirectTimeStamp) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectTimeStamp());
}

TEST(MARTeCommonPvGTest,TestReadDirectHyst) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectHyst());
}

TEST(MARTeCommonPvGTest,TestReadDirectMdel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectMdel());
}

TEST(MARTeCommonPvGTest,TestReadDirectAdel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectAdel());
}

TEST(MARTeCommonPvGTest,TestReadDirectHhsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectHhsv());
}

TEST(MARTeCommonPvGTest,TestReadDirectHsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectHsv());
}

TEST(MARTeCommonPvGTest,TestReadDirectLsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectLsv());
}

TEST(MARTeCommonPvGTest,TestReadDirectLlsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectLlsv());
}

TEST(MARTeCommonPvGTest,TestReadDirectStat) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectStat());
}

TEST(MARTeCommonPvGTest,TestReadDirectSevr) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestReadDirectSevr());
}

TEST(MARTeCommonPvGTest,TestWriteDirectPrecision) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectPrecision());
}

TEST(MARTeCommonPvGTest,TestWriteDirectHighLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectHighLimit());
}

TEST(MARTeCommonPvGTest,TestWriteDirectLowLimit) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectLowLimit());
}
TEST(MARTeCommonPvGTest,TestWriteDirectHighAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectHighAlarm());
}
TEST(MARTeCommonPvGTest,TestWriteDirectLowAlarm) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectLowAlarm());
}
TEST(MARTeCommonPvGTest,TestWriteDirectHighWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectHighWarning());
}
TEST(MARTeCommonPvGTest,TestWriteDirectLowWarning) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectLowWarning());
}
TEST(MARTeCommonPvGTest,TestWriteDirectUnits) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectUnits());
}
TEST(MARTeCommonPvGTest,TestWriteDirectEnums) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectEnums());
}
TEST(MARTeCommonPvGTest,TestWriteDirectAckt) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectAckt());
}
TEST(MARTeCommonPvGTest,TestWriteDirectAcks) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectAcks());
}
TEST(MARTeCommonPvGTest,TestWriteDirectTimeStamp) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectTimeStamp());
}
TEST(MARTeCommonPvGTest,TestWriteDirectHyst) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectHyst());
}
TEST(MARTeCommonPvGTest,TestWriteDirectMdel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectMdel());
}
TEST(MARTeCommonPvGTest,TestWriteDirectAdel) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectAdel());
}
TEST(MARTeCommonPvGTest,TestWriteDirectHhsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectHhsv());
}
TEST(MARTeCommonPvGTest,TestWriteDirectHsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectHsv());
}
TEST(MARTeCommonPvGTest,TestWriteDirectLsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectLsv());
}
TEST(MARTeCommonPvGTest,TestWriteDirectLlsv) {
    MARTeCommonPvTest test;
    ASSERT_TRUE(test.TestWriteDirectLlsv());
}

