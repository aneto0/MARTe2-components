/**
 * @file TimeProviderTest.cpp
 * @brief Source file for class TimeProviderTest
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

 * @details This source file contains the definition of all the methods for
 * the class LinuxTimerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "SafeMath.h"
#include "Sleep.h"
#include "TimeProvider.h"
#include "TimeProviderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TimeProviderTest::TimeProviderTest() {
    timeProvider = NULL;
}

TimeProviderTest::~TimeProviderTest() {
    if(timeProvider != NULL) {
        delete timeProvider;	
    }
}

bool TimeProviderTest::TestConstructor() {
    return true;
}

bool TimeProviderTest::TestCounter() {
    bool ok = (timeProvider != NULL);

    if(ok) {
        uint64 counterAtStart = timeProvider->Counter();
        Sleep::Sec(1);
        uint64 counterAtEnd = timeProvider->Counter();
        ok = (counterAtStart < counterAtEnd);
    }

    return ok;
}

bool TimeProviderTest::TestPeriod() {
    bool ok = (timeProvider != NULL);
    
    if(ok) {
        ok = (timeProvider->Period() != 0);
        if(!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Period is == 0");
        }
    }

    if(ok) {
        float64 frequencyInverse = 1.0 / static_cast<float64>(timeProvider->Frequency());
        ok = SafeMath::IsEqual<float64>(static_cast<float64>(timeProvider->Period()), frequencyInverse);
        if(!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Period value is not equivalent to the inverse of the frequency");
        }
    }
    return ok;
}

bool TimeProviderTest::TestFrequency() {
    bool ok = (timeProvider != NULL);

    if(ok) {
        ok = (timeProvider->Frequency() != 0);
        if(!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Frequency is == 0");
        }
    }

    if(ok) {
        float64 frequencyInverse = 1.0 / static_cast<float64>(timeProvider->Frequency());
        ok = SafeMath::IsEqual<float64>(static_cast<float64>(timeProvider->Period()), frequencyInverse);
        if(!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Frequency value is not equivalent to the inverse of the period");
        }
    }

    return ok;
}

bool TimeProviderTest::TestSleep() {
    bool ok = (timeProvider != NULL);

    if(ok) {
        uint64 startTime = timeProvider->Counter();
        uint64 deltaTime = 1000;
        timeProvider->Sleep(startTime, deltaTime);
        uint64 endTime = timeProvider->Counter();

        ok = ((endTime - startTime) >= deltaTime);
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Busy sleep tolerance after 1k delta %d", (endTime - startTime));
    }

    return ok;
}


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

