/**
 * @file NI1588TimestampTest.h
 * @brief Header file for class NI1588TimestampTest
 * @date 15/05/2018
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class NI1588TimestampTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI1588TIMESTAMPTEST_H_
#define NI1588TIMESTAMPTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI1588Timestamp.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * Tests of the NI1588Timestamp class.
 * @details Connect a signal generator to the NISYNC_PFI0 terminal of the board (the upper input). Generate a TTL square wave (o to 3.3V) with a
 * frequency of 2 Hz.
 */
class NI1588TimestampTest {
public:

    /**
     * @brief Constructor
     */
    NI1588TimestampTest();

    /**
     * @brief Destructor
     */
    virtual ~NI1588TimestampTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise with default PollTimeout
     */
    bool TestInitialise_DefaultPollTimeout();

    /**
     * @brief Tests the TestSetConfiguredDatabase method
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the TestSetConfiguredDatabase method configuring all the possible signals
     */
    bool TestSetConfiguredDatabase_AllSignals();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if TcnTimeStamp PFI0 signal type
     * is different than uint64
     */
    bool TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI0_Type();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if TcnTimeStamp PFI1 signal type
     * is different than uint64
     */
    bool TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI1_Type();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if TcnTimeStamp PFI2 signal type
     * is different than uint64
     */
    bool TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI2_Type();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if EventType signal type
     * is different than uint8
     */
    bool TestSetConfiguredDatabase_False_InvalidEventType_PFI0_Type();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if EventType signal type
     * is different than uint8
     */
    bool TestSetConfiguredDatabase_False_InvalidEventType_PFI1_Type();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if EventType signal type
     * is different than uint8
     */
    bool TestSetConfiguredDatabase_False_InvalidEventType_PFI2_Type();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if TcnTimestamp signal
     * number of elements is greater than one
     */
    bool TestSetConfiguredDatabase_False_InvalidTcnTimestamp_Nelements();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if EventType signal
     * number of elements is greater than one
     */
    bool TestSetConfiguredDatabase_False_InvalidEventType_Nelements();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if a signal that is not allowed
     * is defined
     */
    bool TestSetConfiguredDatabase_False_InvalidSignal();

    /**
     * @brief Tests that the TestSetConfiguredDatabase method fails if an invalid CaptureEvent is
     * is defined
     */
    bool TestSetConfiguredDatabase_False_CaptureEvent();

    /**
     * @brief Tests the DriverRead method
     */
    bool TestDriverRead();

    /**
     * @brief Tests the DriverRead method with all the allowed signals.
     */
    bool TestDriverRead_AllSignals(uint64 expected, int64 tol, uint8 expectedTerminal);

    /**
     * @brief Tests the DriverRead method with a number of samples greater than one
     */
    bool TestDriverRead_MoreSamples(uint64 expected, int64 tol, uint8 expectedTerminal);

    /**
     * @brief Tests the DriverRead method when the timeout expires waiting an event
     */
    bool TestDriverRead_TimeoutExpired(const char8* config, uint64 expected, int64 tol, uint8 expectedTerminal, uint32 nErrorSamples);

    /**
     * @brief Tests the DriverRead method without synchronising signals (no signals defining Frequency > 0)
     */
    bool TestDriverRead_NoSync(uint32 expected, int32 tol);

    /**
     * @brief Tests the DriverRead function passing an external configuration stream and comparing the
     * results with the expected ones.
     */
    bool TestDriverRead_Pattern(const char8* config, uint64 *expectedDeltas, int64 tolerance, uint32 numberOfCycles);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI1588TIMESTAMPTEST_H_ */

