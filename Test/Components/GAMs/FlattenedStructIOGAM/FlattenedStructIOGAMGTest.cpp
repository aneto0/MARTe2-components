/**
 * @file FlattenedStructIOGAMGTest.cpp
 * @brief Source file for class FlattenedStructIOGAMGTest
 * @date 25/03/2022
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
 * the class FlattenedStructIOGAMGTest (public, protected, and private). Be aware that some
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
#include "FlattenedStructIOGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FlattenedStructIOGAMGTest,TestConstructor) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FlattenedStructIOGAMGTest,TestInitialise) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FlattenedStructIOGAMGTest,TestSetup) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

TEST(FlattenedStructIOGAMGTest,TestInitialise_MoreThanOneInput) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MoreThanOneInput());
}

TEST(FlattenedStructIOGAMGTest,TestInitialise_NoType) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestInitialise_NoType());
}

TEST(FlattenedStructIOGAMGTest,TestInitialise_NoDataSource) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestInitialise_NoDataSource());
}

TEST(FlattenedStructIOGAMGTest,TestSetup_False_DifferentSize) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestSetup_False_DifferentSize());
}

TEST(FlattenedStructIOGAMGTest,TestExecute) {
    FlattenedStructIOGAMTest test;
    ASSERT_TRUE(test.TestExecute());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
