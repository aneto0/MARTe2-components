/**
 * @file TimeProviderTest.h
 * @brief Header file for class TimeProviderTest
 * @date 05/03/2021
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

 * @details This header file contains the declaration of the class LinuxTimerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINUXTIMERTEST_TIMEPROVIDERTEST_H_
#define LINUXTIMERTEST_TIMEPROVIDERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeProvider.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the TimeProvider methods providing stubs for other plugin implementations
 */
class TimeProviderTest {
    public:

        /**
         * @brief Constructor for the test.
         */
        TimeProviderTest();

        /**
         * @brief Destructor for the test.
         */
        virtual ~TimeProviderTest();

        /**
         * @brief Tests the constructor.
         */
        virtual bool TestConstructor();

        /**
        * @brief Tests the internal counter
        */
        virtual bool TestCounter();

        /**
        * @brief Tests the period
        */
        virtual bool TestPeriod();

        /**
        * @brief Tests the frequency
        */
        virtual bool TestFrequency();

        /**
        * @brief Tests the busy sleeping feature
        */
        virtual bool TestSleep();

    protected:
        MARTe::TimeProvider* timeProvider;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINUXTIMERTEST_TIMEPROVIDERTEST_H_ */

