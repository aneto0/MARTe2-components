/**
 * @file HistogramGAMTest.h
 * @brief Header file for class HistogramGAMTest
 * @date 30/08/2018
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

 * @details This header file contains the declaration of the class HistogramGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HISTOGRAMGAMTEST_H_
#define HISTOGRAMGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HistogramGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the HistogramGAM methods.
 */
class HistogramGAMTest {
public:

    /**
     * @brief Constructor
     */
    HistogramGAMTest();

    /**
     * @brief Destructor
     */
    virtual ~HistogramGAMTest();

    /**
     * @brief Tests the HistogramGAM constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the HistogramGAM::Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the HistogramGAM::Setup method.
     */
    bool TestSetup();

    /**
     * @brief Tests the HistogramGAM::Setup method that fails
     * if the number of input and output signals is not the same
     */
    bool TestSetup_DifferentInputOutputNSignals();

    /**
     * @brief Tests the HistogramGAM::Setup method that fails if one or more
     * input signals have NumberOfElements>1
     */
    bool TestSetup_FalseInputNotScalar();

    /**
     * @brief Tests the HistogramGAM::Setup method that fails if one or more
     * input signals have NumberOfSamples > 1
     */
    bool TestSetup_FalseSamplesNotOne();

    /**
     * @brief Tests the HistogramGAM::Setup method that fails if MaxLim is not
     * defined for some input signal.
     */
    bool TestSetup_UndefinedMaxLim();

    /**
     * @brief Tests the HistogramGAM::Setup method that fails if MinLim is not defined for
     * some input signal.
     */
    bool TestSetup_UndefinedMinLim();

    /**
     * @brief Tests the HistogramGAM::Setup method that fails if the NumberOfElements < 3
     * for some output signal.
     */
    bool TestSetup_NBindMinorThan3();

    /**
     * @brief Tests the HistogramGAM::Setup method that fails if the type is different from uint32
     * for some output signal
     */
    bool TestSetup_OutputTypeNotUint32();

    /**
     * @brief Tests the HistogramGAM::Execute method.
     */
    bool TestExecute();

    /**
     * @brief Tests the HistogramGAM::Execute method with a BeginCycleNumber > 0.
     */
    bool TestExecute_BeginCycleNumber();

    /**
     * @brief Tests the PrepareNextState without a reset
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the PrepareNextState with a reset (all)
     */
    bool TestPrepareNextState_Reset_All();

    /**
     * @brief Tests the PrepareNextState with a reset (of only a specific state)
     */
    bool TestPrepareNextState_Reset_State();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HISTOGRAMGAMTEST_H_ */

