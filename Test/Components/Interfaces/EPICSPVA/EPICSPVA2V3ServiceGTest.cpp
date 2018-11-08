/**
 * @file EPICSPVA2V3ServiceGTest.cpp
 * @brief Source file for class EPICSPVA2V3ServiceGTest
 * @date 16/10/2018
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
 * the class EPICSPVA2V3ServiceGTest (public, protected, and private). Be aware that some 
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
#include "EPICSPVA2V3ServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EPICSPVA2V3ServiceGTest,TestConstructor) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(EPICSPVA2V3ServiceGTest,TestInitialise_False_CRCFinalXOR) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.TestInitialise_False_CRCFinalXOR());
}

TEST(EPICSPVA2V3ServiceGTest,TestInitialise_False_CRCInitialValue) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.TestInitialise_False_CRCInitialValue());
}

TEST(EPICSPVA2V3ServiceGTest,TestInitialise_False_Polynomial) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Polynomial());
}

TEST(EPICSPVA2V3ServiceGTest,TestInitialise_False_Structure) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Structure());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_qualifier) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_qualifier());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_hash) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_hash());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_value) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_value());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_hash_mismatch) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_hash_mismatch());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_unknown_qualifier) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_unknown_qualifier());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_invalid_dimensions) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_invalid_dimensions());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_invalid_elements) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_invalid_elements());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_invalid_type) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_invalid_type());
}

TEST(EPICSPVA2V3ServiceGTest,Testrequest_False_invalid_node) {
    EPICSPVA2V3ServiceTest test;
    ASSERT_TRUE(test.Testrequest_False_invalid_node());
}

