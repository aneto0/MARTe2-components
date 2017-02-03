/**
 * @file NI6368DIOTest.h
 * @brief Header file for class NI6368DIOTest
 * @date 30/1/2017
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

 * @details This header file contains the declaration of the class NI6368DIOTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368DIOTEST_H_
#define NI6368_NI6368DIOTEST_H_

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
class NI6368DIOTest {
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
     * @brief Tests the Initialise method without specifying the InputPortMask.
     */
    bool TestInitialise_False_NoInputPortMask();

    /**
     * @brief Tests the Initialise method without specifying the OutputPortMask.
     */
    bool TestInitialise_False_NoOutputPortMask();

    /**
     * @brief Tests the Initialise method without specifying the signals section.
     */
    bool TestInitialise_False_NoSignals();

    /**
     * @brief Tests the Initialise method with all the possible clock sources.
     */
    bool TestInitialise_ClockSampleSource();

    /**
     * @brief Tests the Initialise method without specifying a clock source.
     */
    bool TestInitialise_False_NoClockSampleSource();

    /**
     * @brief Tests the Initialise method with an invalid clock source.
     */
    bool TestInitialise_False_BadClockSampleSource();

    /**
     * @brief Tests the Initialise method with all the possible clock polarities.
     */
    bool TestInitialise_ClockSamplePolarity();

    /**
     * @brief Tests the Initialise method without specifying a clock polarity.
     */
    bool TestInitialise_False_NoClockSamplePolarity();

    /**
     * @brief Tests the Initialise method with an invalid clock polarity.
     */
    bool TestInitialise_False_BadClockSamplePolarity();

    /**
     * @brief Tests the Initialise method with all the possible ScanIntervalCounterSources.
     */
    bool TestInitialise_ScanIntervalCounterSource();

    /**
     * @brief Tests the Initialise method without specifying a ScanIntervalCounterSource.
     */
    bool TestInitialise_False_NoScanIntervalCounterSource();

    /**
     * @brief Tests the Initialise method with an invalid ScanIntervalCounterSource.
     */
    bool TestInitialise_False_BadScanIntervalCounterSource();

    /**
     * @brief Tests the Initialise method without specifying the ScanIntervalCounterPeriod.
     */
    bool TestInitialise_False_NoScanIntervalCounterPeriod();

    /**
     * @brief Tests the Initialise method without specifying the ScanIntervalCounterDelay.
     */
    bool TestInitialise_False_NoScanIntervalCounterDelay();


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
     * @brief Tests the SetConfiguredDatabase method with a board that has only inputs enabled.
     */
    bool TestSetConfiguredDatabase_OnlyInputs();

    /**
     * @brief Tests the SetConfiguredDatabase method with a board that has only outputs enabled.
     */
    bool TestSetConfiguredDatabase_OnlyOutputs();

    /**
     * @brief Tests the SetConfiguredDatabase without specifying any channels.
     */
    bool TestSetConfiguredDatabase_False_NoChannels();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong signal type.
     */
    bool TestSetConfiguredDatabase_False_SignalTypeMismatch();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a number of samples > 1.
     */
    bool TestSetConfiguredDatabase_False_SignalNumberOfSamplesGreaterThanOne();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong device name.
     */
    bool TestSetConfiguredDatabase_False_WrongDeviceName();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368DIOTEST_H_ */

