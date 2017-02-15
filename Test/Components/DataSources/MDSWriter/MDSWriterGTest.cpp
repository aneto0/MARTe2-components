/**
 * @file NI6259ADCGTest.cpp
 * @brief Source file for class NI6259ADCGTest
 * @date 14/02/2017
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
 * the class NI6259ADCGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MDSWriterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MDSWriterGTest,TestConstructor) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_NoTrigger) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_NoTrigger_Flush) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger_Flush());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_NoTrigger_Elements) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_NoTrigger_Elements());
}

TEST(MDSWriterGTest,TestIntegratedInApplication_Trigger_Elements) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestIntegratedInApplication_Trigger_Elements());
}

TEST(MDSWriterGTest,TestOpenTree) {
    MDSWriterTest test;
    ASSERT_TRUE(test.TestOpenTree());
}
