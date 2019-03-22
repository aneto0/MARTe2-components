/**
 * @file OPCUAReferenceContainerGTest.cpp
 * @brief Source file for class OPCUAReferenceContainerGTest
 * @date 12/03/2019
 * @author Luca Porzio
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
 * the class OPCUAReferenceContainerGTest (public, protected, and private). Be aware that some 
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

#include "OPCUAReferenceContainerTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(OPCUAReferenceContainerGTest,TestConstructor) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(OPCUAReferenceContainerGTest,Test_IsObject) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_IsObject());
}

TEST(OPCUAReferenceContainerGTest,Test_IsNode) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_IsNode());
}

TEST(OPCUAReferenceContainerGTest,Test_GetOPCVariable) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_GetOPCVariable());
}

TEST(OPCUAReferenceContainerGTest,Test_GetOPCObject) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_GetOPCObject());
}

TEST(OPCUAReferenceContainerGTest,Test_SetGetParent) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_SetGetParent());
}

TEST(OPCUAReferenceContainerGTest,Test_SetGetNodeId) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_SetGetNodeId());
}

TEST(OPCUAReferenceContainerGTest,Test_SetGetNodeType) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_SetGetNodeType());
}

TEST(OPCUAReferenceContainerGTest,Test_IsFirst) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_IsFirst());
}

TEST(OPCUAReferenceContainerGTest,Test_SetFirst) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_SetFirst());
}

TEST(OPCUAReferenceContainerGTest,Test_NumberOfElementsDimensions) {
    OPCUAReferenceContainerTest test;
    ASSERT_TRUE(test.Test_NumberOfElementsDimensions());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
