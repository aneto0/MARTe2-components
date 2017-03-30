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
     * @brief Tests the Initialise method with all the possible start trigger sources.
     */
    bool TestInitialise_StartTriggerSource();

    /**
     * @brief Tests the Initialise method without specifying a start trigger sources.
     */
    bool TestInitialise_False_NoStartTriggerSource();

    /**
     * @brief Tests the Initialise method with an invalid start trigger source.
     */
    bool TestInitialise_False_BadStartTriggerSource();

    /**
     * @brief Tests the Initialise method with all the possible start trigger polarities.
     */
    bool TestInitialise_StartTriggerPolarity();

    /**
     * @brief Tests the Initialise method without specifying a start trigger polarity.
     */
    bool TestInitialise_False_NoStartTriggerPolarity();

    /**
     * @brief Tests the Initialise method with an invalid start trigger polarity.
     */
    bool TestInitialise_False_BadStartTriggerPolarity();

    /**
     * @brief Tests the Initialise method with all the possible update counter sources.
     */
    bool TestInitialise_UpdateCounterSource();

    /**
     * @brief Tests the Initialise method without specifying an update counter source.
     */
    bool TestInitialise_False_NoUpdateCounterSource();

    /**
     * @brief Tests the Initialise method with an invalid update counter source.
     */
    bool TestInitialise_False_BadUpdateCounterSource();

    /**
     * @brief Tests the Initialise method with all the possible update counter polarities.
     */
    bool TestInitialise_UpdateCounterPolarity();

    /**
     * @brief Tests the Initialise method without specifying an update counter polarity.
     */
    bool TestInitialise_False_NoUpdateCounterPolarity();

    /**
     * @brief Tests the Initialise method with an invalid update counter polarity.
     */
    bool TestInitialise_False_BadUpdateCounterPolarity();

    /**
     * @brief Tests the Initialise method with all the possible update interval counter sources.
     */
    bool TestInitialise_UpdateIntervalCounterSource();

    /**
     * @brief Tests the Initialise method without specifying an update interval counter source.
     */
    bool TestInitialise_False_NoUpdateIntervalCounterSource();

    /**
     * @brief Tests the Initialise method with an invalid update interval counter source.
     */
    bool TestInitialise_False_BadUpdateIntervalCounterSource();

    /**
     * @brief Tests the Initialise method with all the possible update interval counter polarities.
     */
    bool TestInitialise_UpdateIntervalCounterPolarity();

    /**
     * @brief Tests the Initialise method without specifying an update interval counter polarity.
     */
    bool TestInitialise_False_NoUpdateIntervalCounterPolarity();

    /**
     * @brief Tests the Initialise method with an invalid update interval counter polarity.
     */
    bool TestInitialise_False_BadUpdateIntervalCounterPolarity();

    /**
     * @brief Tests the Initialise method without specifying the UpdateIntervalCounterPeriodDivisor.
     */
    bool TestInitialise_False_NoUpdateIntervalCounterPeriodDivisor();

    /**
     * @brief Tests the Initialise method without specifying the UpdateIntervalCounterDelay.
     */
    bool TestInitialise_False_NoUpdateIntervalCounterDelay();

    /**
     * @brief Tests the Initialise method with an invalid UpdateIntervalCounterPeriod.
     */
    bool TestInitialise_False_InvalidUpdateIntervalCounterPeriodDivisor();

    /**
     * @brief Tests the Initialise method with an invalid UpdateIntervalDivisorCounterDelay.
     */
    bool TestInitialise_False_InvalidUpdateIntervalCounterDelay();

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

    /**
     * @brief Tests the Initialise method with a non monotonically increasing channel identifier.
     */
    bool TestInitialise_False_NotMonotonicallyIncreasingChannelId();

    /**
     * @brief Tests the SetConfiguredDatabase specifying more than one sample.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneSample();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368DACTEST_H_ */

