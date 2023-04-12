/**
 * @file UEIMasterObjectGTest.cpp
 * @brief Source file for class UEIMasterObjectGTest
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
#include "UEIMasterObjectTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(UEIMasterObjectGTest,TestConstructor) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(UEIMasterObjectGTest,TestInitialise) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(UEIMasterObjectGTest,TestInitialise_TrueClass) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_TrueClass());
}

TEST(UEIMasterObjectGTest,TestInitialise_NoConnectionTimeout) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_NoConnectionTimeout());
}

TEST(UEIMasterObjectGTest,TestInitialise_NoIp) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_NoIp());
}

TEST(UEIMasterObjectGTest,TestInitialise_BadIp) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_BadIp());
}

TEST(UEIMasterObjectGTest,TestInitialise_RemoteIpNoPort) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_RemoteIpNoPort());
}

TEST(UEIMasterObjectGTest,TestInitialise_RemoteIp) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_RemoteIp());
}

TEST(UEIMasterObjectGTest,TestInitialise_NoDevices) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_NoDevices());
}

TEST(UEIMasterObjectGTest,TestInitialise_ZeroDevices) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_ZeroDevices());
}

TEST(UEIMasterObjectGTest,TestInitialise_TooManyDevices) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_TooManyDevices());
}

TEST(UEIMasterObjectGTest,TestInitialise_RepeatedDevices) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_RepeatedDevices());
}

TEST(UEIMasterObjectGTest,TestInitialise_DaqLibError) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_DaqLibError());
}

TEST(UEIMasterObjectGTest,TestInitialise_UnableToConnectToIOM) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_UnableToConnectToIOM());
}

TEST(UEIMasterObjectGTest,TestInitialise_BadHandle) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_BadHandle());
}

TEST(UEIMasterObjectGTest,TestInitialise_BadDevnHandle) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_BadDevnHandle());
}

TEST(UEIMasterObjectGTest,TestInitialise_DevnModelMismatch) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_DevnModelMismatch());
}

TEST(UEIMasterObjectGTest,TestInitialise_DeviceConfiguredNotInstalled) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_DeviceConfiguredNotInstalled());
}

TEST(UEIMasterObjectGTest,TestInitialise_DeviceUnableToConfigure) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_DeviceUnableToConfigure());
}

TEST(UEIMasterObjectGTest,TestInitialise_MapsDefined) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_MapsDefined());
}

TEST(UEIMasterObjectGTest,TestInitialise_InconsistentMaps) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_InconsistentMaps());
}

TEST(UEIMasterObjectGTest,TestInitialise_InvalidSetDevices) {
    UEIMasterObjectTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidSetDevices());
}
