/**
 * @file Interleaved2FlatGAMTest.h
 * @brief Header file for class Interleaved2FlatGAMTest
 * @date 24/04/2018
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

 * @details This header file contains the declaration of the class Interleaved2FlatGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERLEAVED2FLATGAMTEST_H_
#define INTERLEAVED2FLATGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Interleaved2FlatGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the Interleaved2FlatGAM public methods.
 */
class Interleaved2FlatGAMTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    Interleaved2FlatGAMTest();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~Interleaved2FlatGAMTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Setup method.
     */
    bool TestSetup();

    /**
     * @brief Tests the Setup method using a memory with many packets defined using the NumberOfElements (as opposed to the Samples).
     */
    bool TestSetup_MultiPacketNoSamples();

    /**
     * @brief Test that the Setup method fails when there is an overlap of the signal in input.
     */
    bool TestSetup_False_SignalOverlap_In();

    /**
     * @brief Test that the Setup method fails when both input and output signals are requesting to be interleaved. Must go from interleaved to flat or from flat to interleaved.
     */
    bool TestSetup_False_DoublePacketDef_In();

    /**
     * @brief Test that the Setup method fails when there is an overlap of the signal in output.
     */
    bool TestSetup_False_SignalOverlap_Out();

    /**
     * @brief Test that the Setup method fails when both input and output signals are requesting to be interleaved. Must go from interleaved to flat or from flat to interleaved.
     */
    bool TestSetup_False_DoublePacketDef_Out();

    /**
     * @brief Test that the Setup method fails when the size of the input signals is different from the output one.
     */
    bool TestSetup_False_DifferentIOSizes();

    /**
     * @brief Tests that the Setup method fails if the sum of the packet member is not a sub-multiple of the input signal size.
     */
    bool TestSetup_False_DifferentPacketSize_In();

    /**
     * @brief Tests that the Setup method fails if the sum of the packet member is not a sub-multiple of the output signal size.
     */
    bool TestSetup_False_DifferentPacketSize_Out();

    /**
     * @brief Tests the Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the Execute method with only the interleaved input.
     */
    bool TestExecute_Interleaved2FlatOnly();

    /**
     * @brief Tests the Execute method with only the interleaved output.
     */
    bool TestExecute_Flat2InterleavedOnly();

    /**
     * @brief As TestExecute but without using samples (i.e. using NumberOfElements).
     */
    bool TestExecute_MultiPacketNoSamples();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERLEAVED2FLATGAMTEST_H_ */

