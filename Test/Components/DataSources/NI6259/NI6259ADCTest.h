/**
 * @file NI6259ADCTest.h
 * @brief Header file for class NI6259ADCTest
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

 * @details This header file contains the declaration of the class NI6259ADCTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI6259_NI6259ADCTEST_H_
#define NI6259_NI6259ADCTEST_H_

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
class NI6259ADCTest {
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
     * @brief Tests the GetInputBrokers method without synchronising on this board.
     */
    bool TestGetInputBrokers_NotSynchronisingGAM();

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
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Initialise method .
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method without specifying the sampling frequency.
     */
    bool TestInitialise_False_NoSamplingFrequency();

    /**
     * @brief Tests the Initialise method specifying a wrong sampling frequency.
     */
    bool TestInitialise_False_BadSamplingFrequency();

    /**
     * @brief Tests the Initialise method specifying a zero sampling frequency.
     */
    bool TestInitialise_False_BadZeroSamplingFrequency();

    /**
     * @brief Tests the Initialise method without specifying the device name.
     */
    bool TestInitialise_False_NoDeviceName();

    /**
     * @brief Tests the Initialise method without specifying the board id.
     */
    bool TestInitialise_False_NoBoardId();

    /**
     * @brief Tests the Initialise method without specifying the delay divisor.
     */
    bool TestInitialise_False_NoDelayDivisor();

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
     * @brief Tests the Initialise method with all the possible input polarity.
     */
    bool TestInitialise_InputPolarity();

    /**
     * @brief Tests the Initialise method with an invalid input polarity.
     */
    bool TestInitialise_False_BadInputPolarity();

    /**
     * @brief Tests the Initialise method with the default input polarity.
     */
    bool TestInitialise_DefaultInputPolarity();

    /**
     * @brief Tests the Initialise method with all the possible input modes.
     */
    bool TestInitialise_InputMode();

    /**
     * @brief Tests the Initialise method with an invalid input mode.
     */
    bool TestInitialise_False_BadInputMode();

    /**
     * @brief Tests the Initialise method with the default input mode.
     */
    bool TestInitialise_DefaultInputMode();

    /**
     * @brief Tests the Initialise method with an invalid channel identifier.
     */
    bool TestInitialise_False_BadChannelId();

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
     * @brief Tests the NI6259 integrated in an application
     */
    bool TestIntegrated();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI6259_NI6259ADCTEST_H_ */

