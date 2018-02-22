/**
 * @file WaveformTest.h
 * @brief Header file for class WaveformTest
 * @date 22/02/2018
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

 * @details This header file contains the declaration of the class WaveformTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMTEST_H_
#define WAVEFORMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "WaveformSin.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class WaveformTest {

public:
    WaveformTest();

    virtual ~WaveformTest();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoName();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoInputSignals();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoOutputSignals();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoInputElements();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongInputElements();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoOutputElements();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongOutputElements();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongOutputElements_2();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongOutputElements_3();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoInputSamples();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongInputSamples();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoOutputSamples();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongOutputSamples();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupInvalitInputType();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupInvalitOutputType();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoInputDimension();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongInputDimension();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNoOutputDimension();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupWrongOutputDimension();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNULLInputPointer();

    /**
     * @brief Test the Waveform::Setup() on error
     */
    bool SetupNULLOutputPointer();

    /**
     * @brief Test the Waveform::Setup() on succeed
     */
    bool Setup();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORMTEST_H_ */

