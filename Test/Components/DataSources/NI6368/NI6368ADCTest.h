/**
 * @file NI6368ADCTest.h
 * @brief Header file for class NI6368ADCTest
 * @date 29/11/2016
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

 * @details This header file contains the declaration of the class NI6368ADCTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6368_NI6368ADCTEST_H_
#define NI6368_NI6368ADCTEST_H_

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
class NI6368ADCTest {
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
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method in the context of the RealTimeThread.
     */
    bool TestExecute_RealTimeThread();

    /**
     * @brief Tests the Execute method in the context of the RealTimeMode.
     */
    bool TestExecute_RealTimeMode();

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
     * @brief Tests the Initialise method without specifying the DMABufferSize.
     */
    bool TestInitialise_False_NoDMABufferSize();

    /**
     * @brief Tests the Initialise method without specifying the ScanIntervalCounterPeriod.
     */
    bool TestInitialise_False_NoScanIntervalCounterPeriod();

    /**
     * @brief Tests the Initialise method without specifying the ScanIntervalCounterDelay.
     */
    bool TestInitialise_False_NoScanIntervalCounterDelay();

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
     * @brief Tests the Initialise method with all the possible clock sources.
     */
    bool TestInitialise_ClockConvertSource();

    /**
     * @brief Tests the Initialise method without specifying a clock source.
     */
    bool TestInitialise_False_NoClockConvertSource();

    /**
     * @brief Tests the Initialise method with an invalid clock source.
     */
    bool TestInitialise_False_BadClockConvertSource();

    /**
     * @brief Tests the Initialise method with all the possible clock polarities.
     */
    bool TestInitialise_ClockConvertPolarity();

    /**
     * @brief Tests the Initialise method without specifying a clock polarity.
     */
    bool TestInitialise_False_NoClockConvertPolarity();

    /**
     * @brief Tests the Initialise method with an invalid clock polarity.
     */
    bool TestInitialise_False_BadClockConvertPolarity();

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
     * @brief Tests the Initialise method with all the possible ScanIntervalCounterPolarities.
     */
    bool TestInitialise_ScanIntervalCounterPolarity();

    /**
     * @brief Tests the Initialise method without specifying a ScanIntervalCounterPolarity.
     */
    bool TestInitialise_False_NoScanIntervalCounterPolarity();

    /**
     * @brief Tests the Initialise method with an ScanIntervalCounterPolarity.
     */
    bool TestInitialise_False_BadScanIntervalCounterPolarity();

    /**
     * @brief Tests the Initialise method without specifying the signals section.
     */
    bool TestInitialise_False_NoSignals();

    /**
     * @brief Tests the Initialise method with all the possible input ranges.
     */
    bool TestInitialise_InputRange();

    /**
     * @brief Tests the Initialise method with an invalid input range.
     */
    bool TestInitialise_False_BadInputRange();

    /**
     * @brief Tests the Initialise method with the default input range.
     */
    bool TestInitialise_DefaultInputRange();

    /**
     * @brief Tests the Initialise method with all the possible input types.
     */
    bool TestInitialise_InputType();

    /**
     * @brief Tests the Initialise method with an invalid input type.
     */
    bool TestInitialise_False_BadInputType();

    /**
     * @brief Tests the Initialise method with the default input type.
     */
    bool TestInitialise_DefaultInputType();

    /**
     * @brief Tests the Initialise method with an invalid channel identifier.
     */
    bool TestInitialise_False_BadChannelId();

    /**
     * @brief Tests the Initialise method with a non monotonically increasing channel identifier.
     */
    bool TestInitialise_False_NotMonotonicallyIncreasingChannelId();

    /**
     * @brief Tests the SetConfiguredDatabase method with one channel.
     */
    bool TestSetConfiguredDatabase_OneChannel();

    /**
     * @brief Tests the SetConfiguredDatabase method with all the channel.
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
     * @brief Tests the SetConfiguredDatabase specifying a wrong counter type.
     */
    bool TestSetConfiguredDatabase_False_CounterTypeMismatch();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong timer type.
     */
    bool TestSetConfiguredDatabase_False_TimerTypeMismatch();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong signal type.
     */
    bool TestSetConfiguredDatabase_False_SignalTypeMismatch();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a number of samples mismatch.
     */
    bool TestSetConfiguredDatabase_False_SignalNumberOfSamplesMismatch();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong device name.
     */
    bool TestSetConfiguredDatabase_False_WrongDeviceName();

    /**
     * @brief Tests the SetConfiguredDatabase specifying a wrong combination of the number of samples and frequency.
     */
    bool TestSetConfiguredDatabase_False_WrongFrequencyVsSamples();

    /**
     * @brief Tests the SetConfiguredDatabase specifying more than one sample for the timer.
     */
    bool TestSetConfiguredDatabase_False_CounterSamples();

    /**
     * @brief Tests the SetConfiguredDatabase specifying more than one sample for the counter.
     */
    bool TestSetConfiguredDatabase_False_TimerSamples();

    /**
     * @brief Tests the SetConfiguredDatabase specifying more than one GAM.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneGAM();

    /**
     * @brief Tests the SetConfiguredDatabase without specifying a synchronising GAM.
     */
    bool TestSetConfiguredDatabase_False_NotSynchronisingGAM();

    /**
     * @brief Tests the GetLastBufferIdx function.
     */
    bool TestGetLastBufferIdx();

    /**
     * @brief Tests the IsSynchronising function.
     */
    bool TestIsSynchronising();

    /**
     * @brief Tests the NI6368 integrated in an application
     */
    bool TestIntegrated();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6368_NI6368ADCTEST_H_ */

