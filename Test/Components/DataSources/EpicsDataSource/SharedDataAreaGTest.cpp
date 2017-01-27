/**
 * @file SharedDataAreaGTest.cpp
 * @brief Source file for class SharedDataAreaGTest
 * @date 25/01/2017
 * @author Ivan Herrero
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
 * the class SharedDataAreaGTest (public, protected, and private). Be aware that some
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

#include "SharedDataAreaTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(SharedDataAreaGTest,TestConstructor) {
//	SharedDataAreaTest test;
//    ASSERT_TRUE(test.TestConstructor());
}

TEST(SharedDataAreaGTest,TestProducerConsumerWithSingleThreadUsingIntegers) {
	SharedDataAreaTest test;
	ASSERT_TRUE(test.TestProducerConsumerInSingleThread<int>("TestProducerConsumerWithSingleThreadUsingIntegers", 50));
}

TEST(SharedDataAreaGTest,TestProducerConsumerWithSingleThreadUsingDoubles) {
	SharedDataAreaTest test;
	ASSERT_TRUE(test.TestProducerConsumerInSingleThread<double>("TestProducerConsumerWithSingleThreadUsingDoubles", 25));
}

TEST(SharedDataAreaGTest,TestProducerConsumerWithTwoThreadsUsingIntegers) {
	SharedDataAreaTest test;
	ASSERT_TRUE(test.TestProducerConsumerWithTwoThreads<int>("TestProducerConsumerWithTwoThreadsUsingIntegers", 75));
}

TEST(SharedDataAreaGTest,TestProducerConsumerWithTwoThreadsUsingDoubles) {
	SharedDataAreaTest test;
	ASSERT_TRUE(test.TestProducerConsumerWithTwoThreads<double>("TestProducerConsumerWithTwoThreadsUsingDoubles"));
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
