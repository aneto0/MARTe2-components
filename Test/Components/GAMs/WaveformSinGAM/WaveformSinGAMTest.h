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

#ifndef TEST_COMPONENTS_GAMS_WAVEFORMSINGAM_WAVEFORMSINGAMTEST_H_
#define TEST_COMPONENTS_GAMS_WAVEFORMSINGAM_WAVEFORMSINGAMTEST_H_

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
    WaveformSinGAMTest();
    virtual ~WaveformSinGAMTest();

    bool TestMissingAmplitude();

    bool TestMissingFrequency();

    bool TestMissingPhase();

    bool TestMissingOffset();

    bool TestFrequency0();

    bool TestAmplitude0();

    bool TestMissingInputSignal();

    bool TestMissingOutputSignal();

    bool TestMissingInputNumberOfElements();

    bool TestWrongInputNumberOfElements();

    bool TestMissingOutputNumberOfElements();

    bool TestWrongOutputNumberOfElements();

    bool TestMissingSecondOutputNumberOfElements();

    bool TestWrongSecondOutputNumberOfElements();

    bool TestMissingInputNumberOfSamples();

    bool TestWrongInputNumberOfSamples();

    bool TestMissingOutputNumberOfSamples();

    bool TestWrongOutputNumberOfSamples();

    bool TestMissingSeondOutputNumberOfSamples();

    bool TestWrongSecondOutputNumberOfSamples();

    bool TestMissingInputType();

    bool TestMissingOutputType();

    bool TestMissingSecondOutputType();

    bool TestUInt8Execute();

    bool TestInt8Execute();

    bool TestInt16Execute();

    bool TestUInt16Execute();

    bool TestInt32Execute();

    bool TestUInt32Execute();

    bool TestInt64Execute();

    bool TestUInt64Execute();

    bool TestFloat32Execute();

    bool TestFloat64Execute();

    bool TestStopTriggerGreaterThanStartTrigger();

    bool TestStartTriggerGreaterThanStopTriggerPluse2();

    bool TestStartTriggerTimeArrayInconsistent();

    bool TestUInt8ExecuteTrigger();

    bool TestInt8ExecuteTrigger();

    bool TestUInt16ExecuteTrigger();

    bool TestInt16ExecuteTrigger();

    bool TestUInt32ExecuteTrigger();

    bool TestInt32ExecuteTrigger();

    bool TestUInt64ExecuteTrigger();

    bool TestInt64ExecuteTrigger();

    bool TestFloat32ExecuteTrigger();

    bool TestFloat64ExecuteTrigger();

    bool TestExecuteTriggerDiffArrayLength();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_GAMS_WAVEFORMSINGAM_WAVEFORMSINGAMTEST_H_ */

