/**
 * @file NI6368DACTest.h
 * @brief Header file for class NI6368DACTest
 * @date 23/01/2017
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

 * @details This header file contains the declaration of the class NI6368DACTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368DACTEST_H_
#define NI6368_NI6368DACTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the LoggerDataSource public methods.
 */
class NI6368DACTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method with an invalid signal index.
     */
    bool TestGetSignalMemoryBuffer_False();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the GetOutputBrokers with two GAMs (one triggering and one not triggering).
     */
    bool TestGetOutputBrokers_NoTriggerGAM();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Initialise method .
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method without specifying the device name.
     */
    bool TestInitialise_False_NoDeviceName();

    /**
     * @brief Tests the Initialise method without specifying the board id.
     */
    bool TestInitialise_False_NoBoardId();

    /**
     * @brief Tests the Initialise method without specifying the signals section.
     */
    bool TestInitialise_False_NoSignals();

    /**
     * @brief Tests the Initialise method with all the possible output ranges.
     */
    bool TestInitialise_OutputRange();

    /**
     * @brief Tests the Initialise method with an invalid output range.
     */
    bool TestInitialise_False_BadOutputRange();

    /**
     * @brief Tests the Initialise method with the default output range.
     */
    bool TestInitialise_DefaultOutputRange();

    /**
     * @brief Tests the Initialise method with an invalid channel identifier.
     */
    bool TestInitialise_False_BadChannelId();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method with all the channels.
     */
    bool TestSetConfiguredDatabase_AllChannels();

    /**
     * @brief Tests that the SetConfiguredDatabase method sets all the parameters correctly for all the channels.
     */
    bool TestSetConfiguredDatabase_AllChannels_Parameters();

    /**
     * @brief Tests the SetConfiguredDatabase without specifying any channels.
     */
    bool TestSetConfiguredDatabase_False_NoChannels();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong signal type.
     */
    bool TestSetConfiguredDatabase_False_SignalTypeMismatch();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong device name.
     */
    bool TestSetConfiguredDatabase_False_WrongDeviceName();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a triggering signal.
     */
    bool TestSetConfiguredDatabase_False_NoTriggers();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368DACTEST_H_ */

