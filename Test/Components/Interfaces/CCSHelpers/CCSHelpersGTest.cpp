/**
 * @file CCSHelpersGTest.cpp
 * @brief Source file for class CCSHelpersGTest
 * @date 25/05/2022
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
 * the class CCSHelpersGTest (public, protected, and private). Be aware that some
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
#include "CCSHelpersTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(CCSHelpersTest, TestMARTeToCCSAnyValue) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestMARTeToCCSAnyValue());
}

TEST(CCSHelpersTest, TestMARTeToCCSAnyValue_StructuredDataI) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestMARTeToCCSAnyValue_StructuredDataI());
}

TEST(CCSHelpersTest, TestGetMARTeBasicType) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestGetMARTeBasicType());
}

TEST(CCSHelpersTest, TestCCSToMARTeAnyObject) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestCCSToMARTeAnyObject());
}

TEST(CCSHelpersTest, TestCCSToMARTeAnyObject_StringArray) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestCCSToMARTeAnyObject_StringArray());
}

TEST(CCSHelpersTest, TestCCSToMARTeAnyObject_IntrospectedType) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestCCSToMARTeAnyObject_IntrospectedType());
}

TEST(CCSHelpersTest, TestCCSToMARTeStructuredDataI) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestCCSToMARTeStructuredDataI());
}

TEST(CCSHelpersTest, TestGetCCSBasicType) {
    CCSHelpersTest test;
    ASSERT_TRUE(test.TestGetMARTeBasicType());
}

