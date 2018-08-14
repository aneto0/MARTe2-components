/**
 * @file TimeCorrectionGAMTest.h
 * @brief Header file for class TimeCorrectionGAMTest
 * @date 07/06/2018
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

 * @details This header file contains the declaration of the class TimeCorrectionGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMECORRECTIONGAMTEST_H_
#define TIMECORRECTIONGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TimeCorrectionGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the TimeCorrectionGAM methods
 */
class TimeCorrectionGAMTest {
public:

    /**
     * @brief Constructor
     */
    TimeCorrectionGAMTest();

    /**
     * @brief Destructor
     */
    virtual ~TimeCorrectionGAMTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the TimeCorrectionGAM::Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the TimeCorrectionGAM::Initialise method that fails if
     * ExpectedDelta is not defined
     */
    bool TestInitialise_FalseNoExpectedDelta();

    /**
     * @brief Tests the TimeCorrectionGAM::Initialise method that fails if
     * DeltaTolerance is not defined
     */
    bool TestInitialise_FalseNoDeltaTolerance();

    /**
     * @brief Tests the TimeCorrectionGAM::Initialise method that fails if
     * FilterGain is not defined
     */
    bool TestInitialise_FalseNoFilterGain();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method
     */
    bool TestSetup();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method with one
     * output signal
     */

    bool TestSetup_OneOutputSignal();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if more
     * than one input signal is defined
     */
    bool TestSetup_FalseNumberOfInputSignals();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if more
     * than two signals is defined
     */
    bool TestSetup_FalseNumberOfOutputSignals();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if the
     * input signal type is different than uint64
     */
    bool TestSetup_FalseBadInputType();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if the
     * first output signal type is different than uint64
     */
    bool TestSetup_FalseBadOutputType1();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if the
     * second output signal type is different than uint8
     */
    bool TestSetup_FalseBadOutputType2();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if the
     * input signal is not scalar
     */
    bool TestSetup_FalseBadInputNElements();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if the
     * first output signal is not scalar
     */
    bool TestSetup_FalseBadOutputNElements1();

    /**
     * @brief Tests the TimeCorrectionGAM::Setup method that fails if the
     * second output signal is not scalar
     */
    bool TestSetup_FalseBadOutputNElements2();

    /**
     * @brief Tests the TimeCorrectionGAM::Execute method
     */
    bool TestExecute();

    /**
     * @brief Tests the TimeCorrectionGAM::Execute method without the second
     * output signal
     */
    bool TestExecute_OneOutputSignal();

    /**
     * @brief Tests the TimeCorrectionGAM::Execute method
     * correcting in case of consecutive wrong deltas
     */
    bool TestExecute_EstimationConsecutiveJumps();

    /**
     * @brief Tests the TimeCorrectionGAM::Execute method
     * correcting if the delta changes slowly
     */
    bool TestExecute_EstimationSlowChange();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMECORRECTIONGAMTEST_H_ */

