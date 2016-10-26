/**
 * @file LinuxTimerTest.cpp
 * @brief Source file for class LinuxTimerTest
 * @date 25/10/2016
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
#include "LinuxTimer.h"
#include "LinuxTimerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool LinuxTimerTest::TestConstructor() {
    using namespace MARTe;
    LinuxTimer test;
    uint32 *counter;
    uint32 *timer;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    bool ok = (counter == 0);
    ok &= (timer == 0);
    return ok;
}

bool LinuxTimerTest::TestAllocateMemory() {
    using namespace MARTe;
    LinuxTimer test;
    return test.AllocateMemory();
}

bool LinuxTimerTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    LinuxTimer test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool LinuxTimerTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    LinuxTimer test;
    uint32 *counter;
    uint32 *timer;
    test.GetSignalMemoryBuffer(0, 0, (void *&) counter);
    test.GetSignalMemoryBuffer(1, 0, (void *&) timer);
    bool ok = (counter == 0);
    ok &= (timer == 0);
    return ok;
}

bool LinuxTimerTest::TestGetSignalMemoryBuffer_False() {
    using namespace MARTe;
    LinuxTimer test;
    uint32 *ptr;
    return !test.GetSignalMemoryBuffer(2, 0, (void *&) ptr);
}

bool LinuxTimerTest::TestGetBrokerName() {
    return "MemoryMapSynchronisedInputBroker";
}

bool LinuxTimerTest::TestGetInputBrokers() {
    return true;
}

bool LinuxTimerTest::TestGetOutputBrokers() {
    return true;
}

bool LinuxTimerTest::TestSynchronise() {
    return true;
}

bool LinuxTimerTest::TestExecute() {
    return true;
}

bool LinuxTimerTest::TestPrepareNextState() {
    return true;
}

bool LinuxTimerTest::TestInitialise() {
    return true;
}

bool LinuxTimerTest::TestSetConfiguredDatabase() {
    return true;
}
