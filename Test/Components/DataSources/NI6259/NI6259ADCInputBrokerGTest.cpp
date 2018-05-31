/**
 * @file NI6259ADCInputBrokerGTest.cpp
 * @brief Source file for class NI6259ADCInputBrokerGTest
 * @date 22/05/2018
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
 * the class NI6259ADCInputBrokerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

#include "NI6259ADCInputBrokerTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI6259ADCInputBrokerGTest,TestConstructor) {
    NI6259ADCInputBrokerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI6259ADCInputBrokerGTest,TestConstructor_NI6259ADC) {
    NI6259ADCInputBrokerTest test;
    ASSERT_TRUE(test.TestConstructor_NI6259ADC());
}

TEST(NI6259ADCInputBrokerGTest,TestIntegrated) {
    NI6259ADCInputBrokerTest test;
    ASSERT_TRUE(test.TestExecute());
}
