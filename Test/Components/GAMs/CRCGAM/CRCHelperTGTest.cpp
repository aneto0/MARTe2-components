/**
 * @file CRCHelperTGTest.cpp
 * @brief Source file for class CRCHelperTGTest
 * @date Oct 30, 2018 TODO Verify the value and format of the date
 * @author root TODO Verify the name and format of the author
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
 * the class CRCHelperTGTest (public, protected, and private). Be aware that some 
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

#include "CRCHelperTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(CRCHelperTGTest,TestConstructorUint8) {
    CRCHelperTTest test;
    ASSERT_TRUE(test.TestConstructorUint8());
}

TEST(CRCHelperTGTest,TestConstructorUint16) {
    CRCHelperTTest test;
    ASSERT_TRUE(test.TestConstructorUint16());
}

TEST(CRCHelperTGTest,TestConstructorUint32) {
    CRCHelperTTest test;
    ASSERT_TRUE(test.TestConstructorUint32());
}

TEST(CRCHelperTGTest,TestComputeTable) {
    CRCHelperTTest test;
    ASSERT_TRUE(test.TestComputeTable());
}
	
TEST(CRCHelperTGTest,TestCompute) {
    CRCHelperTTest test;
    ASSERT_TRUE(test.TestCompute());
}
