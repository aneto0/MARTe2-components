/**
 * @file UEIDeviceGTest.cpp
 * @brief Source file for class UEIDeviceGTest
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
#include "UEIDeviceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIDeviceGTest,TestConstructor) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIDeviceGTest,TestBaseMethods) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestBaseMethods());
}

TEST(UEIDeviceGTest,TestHardwareCorrespondence) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestHardwareCorrespondence());
}

TEST(UEIDeviceGTest,TestMapAssignment) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestMapAssignment());
}

TEST(UEIDeviceGTest,TestInitialise) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIDeviceGTest,TestInitialise_NoDevn) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestInitialise_NoDevn());
}

TEST(UEIDeviceGTest,TestInitialise_InvalidDevn) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidDevn());
}

TEST(UEIDeviceGTest,TestInitialise_NoFrequency) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestInitialise_NoFrequency());
}

TEST(UEIDeviceGTest,TestCheckChannelAndDirection_List) {
    UEIDeviceTest test;
    ASSERT_TRUE(test.TestCheckChannelAndDirection_List());
}
