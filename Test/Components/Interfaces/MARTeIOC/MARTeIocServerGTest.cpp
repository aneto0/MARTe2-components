/**
 * @file MARTeIocServerGTest.cpp
 * @brief Source file for class MARTeIocServerGTest
 * @date 12/07/2019
 * @author Giuseppe Ferro
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
 * the class MARTeIocServerGTest (public, protected, and private). Be aware that some 
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

#include "MARTeIocServerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#if 1

TEST(MARTeIocServerGTest,TestInitialise) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestInitialise());
}
TEST(MARTeIocServerGTest,TestCreateTimer) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestCreateTimer());
}
TEST(MARTeIocServerGTest,TestPvExistTest) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestPvExistTest());
}
TEST(MARTeIocServerGTest,TestPvAttach) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestPvAttach());
}
TEST(MARTeIocServerGTest,TestExecute) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestExecute());
}
TEST(MARTeIocServerGTest,TestIocWrite) {
    MARTeIocServerTest test;
    uint32 input=10;
    float32 output=10.0;
    ASSERT_TRUE(test.TestIocWrite(input, output));
}
TEST(MARTeIocServerGTest,TestIocWriteVector) {
    MARTeIocServerTest test;
    uint32 input[]={1,2,3,4,5,6,7,8,9,10};
    float32 output[]={1.,2.,3.,4.,5.,6.,7.,8.,9.,10.};
    ASSERT_TRUE(test.TestIocWriteVector(input, output));
}
TEST(MARTeIocServerGTest,TestIocRead) {
    MARTeIocServerTest test;
    uint32 input=10;
    float32 output=10.0;
    ASSERT_TRUE(test.TestIocRead(input, output));
}
TEST(MARTeIocServerGTest,TestIocReadVector) {
    MARTeIocServerTest test;
    uint32 input[]={1,2,3,4,5,6,7,8,9,10};
    float32 output[]={1.,2.,3.,4.,5.,6.,7.,8.,9.,10.};
    ASSERT_TRUE(test.TestIocReadVector(input, output));
}
TEST(MARTeIocServerGTest,TestIocAddPv) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestIocAddPv());
}

TEST(MARTeIocServerGTest,TestGetNumberOfPVs) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestGetNumberOfPVs());
}

#else

TEST(MARTeIocServerGTest,TestInteractive) {
    MARTeIocServerTest test;
    ASSERT_TRUE(test.TestInteractive());
}

#endif
