/**
 * @file RealTimeThreadAsyncBridgeTest.h
 * @brief Header file for class RealTimeThreadAsyncBridgeTest
 * @date 25/04/2018
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

 * @details This header file contains the declaration of the class RealTimeThreadAsyncBridgeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADASYNCBRIDGETEST_H_
#define REALTIMETHREADASYNCBRIDGETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeThreadAsyncBridge.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the RealTimeThreadAsyncBridge methods
 */
class RealTimeThreadAsyncBridgeTest {
public:

    /**
     * @brief Constructor
     */
    RealTimeThreadAsyncBridgeTest();

    /**
     * @brief Destructor
     */
    virtual ~RealTimeThreadAsyncBridgeTest();

    /**
     * @brief Tests the contructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method that fails if a NumberOfBuffer>64 is specified
     */
    bool TestInitialise_False_GreaterNumberOfBuffers();

    /**
     * @brief Tests the SetConfiguredDatabase method
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the GetBrokerName method
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the SetConfiguredDatabase method that fails if one or more input
     * signals have defined a number of samples > 1
     */
    bool TestSetConfiguredDatabase_False_InputSamples();

    /**
     * @brief Tests the SetConfiguredDatabase method that fails if one or more output
     * signals have defined a number of samples > 1
     */
    bool TestSetConfiguredDatabase_False_OutputSamples();

    /**
     * @brief Tests the Synchronise method
     */
    bool TestSynchronise();

    /**
     * @brief Tests the PrepareNextState method
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the GetInputOffset method
     */
    bool TestGetInputOffset();

    /**
     * @brief Tests the GetOutputOffset method
     */
    bool TestGetOutputOffset();

    /**
     * @brief Tests the GetInputOffset method with 2 reader GAMs
     */
    bool TestGetInputOffset_2Readers();

    /**
     * @brief Tests the GetInputOffset method with a signal with Ranges
     */
    bool TestGetInputOffset_Ranges();

    /**
     * @brief Tests the GetOutputOffset method with a signal with Ranges
     */
    bool TestGetOutputOffset_Ranges();

    /**
     * @brief Tests the GetInputOffset method in blocking mode
     */
    bool TestGetInputOffset_Blocking();

    /**
     * @brief Tests the GetOutputOffset method in blocking mode
     */
    bool TestGetOutputOffset_Blocking();

    /**
     * @brief Tests the GetInputOffset method that returns false if all the buffers are busy
     */
    bool TestGetInputOffset_False_BufferBusy();

    /**
     * @brief Tests the GetOutputOffset method that returns false if all the buffers are busy
     */
    bool TestGetOutputOffset_False_BufferBusy();

    /**
     * @brief Tests the TerminateRead method
     */
    bool TestTerminateRead();

    /**
     * @brief Tests the TerminateWrite method
     */
    bool TestTerminateWrite();

    /**
     * @brief Tests the TerminateWrite method when the counter reaches the overflow and it should be reset
     */
    bool TestTerminateWrite_ResetCounter();

    /**
     * @brief Tests the ResetSignalValue method
     */
    bool TestResetSignalValue();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADASYNCBRIDGETEST_H_ */

