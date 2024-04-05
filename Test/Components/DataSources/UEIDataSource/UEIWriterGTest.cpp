/**
 * @file UEIWriterGTest.cpp
 * @brief Source file for class UEIWriterGTest
 * @date 02/05/2023
 * @author Xavier Ruche
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
 * the class UEIWriterGTest (public, protected, and private). Be aware that some 
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
#include "UEIWriterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIWriterGTest,TestConstructor) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIWriterGTest,TestInitialise) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIWriterGTest,TestInitialise_TrueClass) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestInitialise_TrueClass());
}

TEST(UEIWriterGTest,TestInitialise_TwoDsPerMap) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestInitialise_TwoDsPerMap());
}

TEST(UEIWriterGTest,TestSimpleMethods) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestSimpleMethods());
}

TEST(UEIWriterGTest,TestStartup) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestStartup());
}

TEST(UEIWriterGTest,TestStartup_InvalidChannelNumber) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestStartup_InvalidChannelNumber());
}

TEST(UEIWriterGTest,TestStartup_InvalidNumberOfElements) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestStartup_InvalidNumberOfElements());
}

TEST(UEIWriterGTest,TestStartup_InvalidNumberOfDimensions) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestStartup_InvalidNumberOfDimensions());
}

TEST(UEIWriterGTest,TestStartup_MismatchedSampleNumber) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestStartup_MismatchedSampleNumber());
}

TEST(UEIWriterGTest,TestStartup_MultipleDMapSamples) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestStartup_MultipleDMapSamples());
}

TEST(UEIWriterGTest,TestSync) {
    UEIWriterTest test;
    ASSERT_TRUE(test.TestSync());
}
