/**
 * @file UEIDataSourceIGTest.cpp
 * @brief Source file for class UEIDataSourceIGTest
 * @date 2/05/2023
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
 * the class UEIDataSourceIGTest (public, protected, and private). Be aware that some 
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
#include "UEIDataSourceITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIDataSourceIGTest,TestConstructor) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIDataSourceIGTest,TestInitialise) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIDataSourceIGTest,TestInitialise_TrueClass) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise_TrueClass());
}

TEST(UEIDataSourceIGTest,TestInitialise_NoDevice) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise_NoDevice());
}

TEST(UEIDataSourceIGTest,TestInitialise_NoMap) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise_NoMap());
}

TEST(UEIDataSourceIGTest,TestInitialise_InvalidDevice) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise_InvalidDevice());
}

TEST(UEIDataSourceIGTest,TestInitialise_InvalidMap) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestInitialise_InvalidMap());
}

TEST(UEIDataSourceIGTest,TestBaseMethods) {
    UEIDataSourceITest test;
    ASSERT_TRUE(test.TestBaseMethods());
}
