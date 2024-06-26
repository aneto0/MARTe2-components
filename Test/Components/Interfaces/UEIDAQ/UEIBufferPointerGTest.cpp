/**
 * @file UEIBufferPointerGTest.cpp
 * @brief Source file for class UEIBufferPointerGTest
 * @date 22/03/2023
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
 * the class MemoryGateGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UEIBufferPointerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIBufferPointerGTest,TestConstructor) {
    UEIBufferPointerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIBufferPointerGTest,TestSetter) {
    UEIBufferPointerTest test;
    ASSERT_TRUE(test.TestSetter());
}

TEST(UEIBufferPointerGTest,TestPointerSetter) {
    UEIBufferPointerTest test;
    ASSERT_TRUE(test.TestPointerSetter());
}

TEST(UEIBufferPointerGTest,TestCheckPointer) {
    UEIBufferPointerTest test;
    ASSERT_TRUE(test.TestCheckPointer());
}

TEST(UEIBufferPointerGTest,FunctionalTest) {
    UEIBufferPointerTest test;
    ASSERT_TRUE(test.FunctionalTest());
}

