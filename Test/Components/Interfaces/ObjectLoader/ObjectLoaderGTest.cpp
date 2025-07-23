/**
 * @file ObjectLoaderGTest.cpp
 * @brief Source file for class ObjectLoaderGTest
 * @date 15/5/2025
 * @author nferron
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
 * the class ObjectLoaderGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ObjectLoaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ObjectLoaderGTest,TestConstructor) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ObjectLoaderGTest,TestInitialise) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(ObjectLoaderGTest,TestInitialise_ParametersCopy) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestInitialise_ParametersCopy());
}

TEST(ObjectLoaderGTest,TestSerialiseObjects_ParametersValue) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestSerialiseObjects_ParametersValue());
}

TEST(ObjectLoaderGTest,TestSerialiseObjects_Failed_InvalidType) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestSerialiseObjects_Failed_InvalidType());
}

TEST(ObjectLoaderGTest,TestSerialiseObjects_Failed_CannotSerialise) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestSerialiseObjects_Failed_CannotSerialise());
}

TEST(ObjectLoaderGTest,TestSerialiseObjects_Failed_ParametersWithSameName) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestSerialiseObjects_Failed_ParametersWithSameName());
}
TEST(ObjectLoaderGTest,TestSerialiseObjects) {
    ObjectLoaderTest test;
    ASSERT_TRUE(test.TestSerialiseObjects());
}

