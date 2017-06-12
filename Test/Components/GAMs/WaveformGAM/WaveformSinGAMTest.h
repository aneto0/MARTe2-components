/**
 * @file WaveFormSinGAMTest.h
 * @brief Header file for class WaveFormSinGAMTest
 * @date 22/05/2017
 * @author Llorenc Capella
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

 * @details This header file contains the declaration of the class WaveFormSinGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMSINGAMTEST_H_
#define WAVEFORMSINGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class WaveformSinGAMTest {
public:
    /**
     * @brief Default constructor.
     */
    WaveformSinGAMTest();

    /**
     * @brief Default destructor
     */
    virtual ~WaveformSinGAMTest();

    /**
     * @brief Test message errors of WaveformSin::Initialise().
     */
    bool TestMissingAmplitude();

    /**
     * @brief Test message errors of WaveformSin::Initialise().
     */
    bool TestMissingFrequency();

    /**
     * @brief Test message errors of WaveformSin::Initialise().
     */
    bool TestMissingPhase();

    /**
     * @brief Test message errors of WaveformSin::Initialise().
     */
    bool TestMissingOffset();

    /**
     * @brief Test message errors of WaveformSin::Initialise().
     */
    bool TestFrequency0();

    /**
     * @brief Test message errors of WaveformSin::Initialise().
     */
    bool TestAmplitude0();

    /**
     * @brief Test message errors of Waveform::Setup().
     */
    bool TestMissingInputSignal();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingOutputSignal();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingInputNumberOfElements();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestWrongInputNumberOfElements();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingOutputNumberOfElements();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestWrongOutputNumberOfElements();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingSecondOutputNumberOfElements();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestWrongSecondOutputNumberOfElements();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingInputNumberOfSamples();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestWrongInputNumberOfSamples();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingOutputNumberOfSamples();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestWrongOutputNumberOfSamples();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingSeondOutputNumberOfSamples();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestWrongSecondOutputNumberOfSamples();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingInputType();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingOutputType();

    /**
     * @brief Test message errors of Waveform::Initialise().
     */
    bool TestMissingSecondOutputType();

    /**
     * @brief Test the correctness of the output with uint8.
     */
    bool TestUInt8Execute();

    /**
     * @brief Test the correctness of the output with int8.
     */
    bool TestInt8Execute();

    /**
     * @brief Test the correctness of the output with int16.
     */
    bool TestInt16Execute();

    /**
     * @brief Test the correctness of the output with uint16.
     */
    bool TestUInt16Execute();

    /**
     * @brief Test the correctness of the output with int32.
     */
    bool TestInt32Execute();

    /**
     * @brief Test the correctness of the output with uint32.
     */
    bool TestUInt32Execute();

    /**
     * @brief Test the correctness of the output with int64.
     */
    bool TestInt64Execute();

    /**
     * @brief Test the correctness of the output with uint64.
     */
    bool TestUInt64Execute();

    /**
     * @brief Test the correctness of the output with float32.
     */
    bool TestFloat32Execute();

    /**
     * @brief Test the correctness of the output with float64.
     */
    bool TestFloat64Execute();

    /**
     * @brief Test warning message of Waveform::Setup() when checking triggering parameters.
     */
    bool TestStopTriggerGreaterThanStartTrigger();

    /**
     * @brief Test warning message of Waveform::Setup() when checking triggering parameters.
     */
    bool TestStartTriggerGreaterThanStopTriggerPlus2();

    /**
     * @brief Test warning message of Waveform::Setup() when checking triggering parameters.
     */
    bool TestStartTriggerTimeArrayInconsistent();

    /**
     * @brief Test the correctness of the output with uint8 and trigger mechanism enabled.
     */
    bool TestUInt8ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with int8 and trigger mechanism enabled.
     */
    bool TestInt8ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with uint16 and trigger mechanism enabled.
     */
    bool TestUInt16ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with int16 and trigger mechanism enabled.
     */
    bool TestInt16ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with uin32 and trigger mechanism enabled.
     */
    bool TestUInt32ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with int32 and trigger mechanism enabled.
     */
    bool TestInt32ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with uint64 and trigger mechanism enabled.
     */
    bool TestUInt64ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with int64 and trigger mechanism enabled.
     */
    bool TestInt64ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with float32 and trigger mechanism enabled.
     */
    bool TestFloat32ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with float64 and trigger mechanism enabled.
     */
    bool TestFloat64ExecuteTrigger();

    /**
     * @brief Test the correctness of the output with int64 and different start/stop trigger length.
     */
    bool TestExecuteTriggerDiffArrayLength();

    /**
     * @brief Test the correctness of the output with 1 elements per Cycle
     */
    bool TestExecuteTigger1Element();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORMSINGAMTEST_H_ */

