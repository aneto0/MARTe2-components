/**
 * @file CreateNI9157DeviceOperatorIGTest.cpp
 * @brief Source file for class CreateNI9157DeviceOperatorIGTest
 * @date 22/03/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class CreateNI9157DeviceOperatorIGTest (public, protected, and private).
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CreateNI9157DeviceOperatorITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(CreateNI9157DeviceOperatorIGTest,TestConstructor) {
    CreateNI9157DeviceOperatorITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(CreateNI9157DeviceOperatorIGTest,TestCreate) {
    CreateNI9157DeviceOperatorITest test;
    ASSERT_TRUE(test.TestCreate());
}

TEST(CreateNI9157DeviceOperatorIGTest,TestGetIdentifier) {
    CreateNI9157DeviceOperatorITest test;
    ASSERT_TRUE(test.TestGetIdentifier());
}

TEST(CreateNI9157DeviceOperatorIGTest,TestGetTypeDescriptor) {
    CreateNI9157DeviceOperatorITest test;
    ASSERT_TRUE(test.TestGetTypeDescriptor());
}
