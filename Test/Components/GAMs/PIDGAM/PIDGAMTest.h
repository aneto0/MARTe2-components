/**
 * @file PIDGAMTest.h
 * @brief Header file for class PIDGAMTest
 * @date 22/08/2017
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

 * @details This header file contains the declaration of the class PIDGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PIDGAMTEST_H_
#define PIDGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "PIDGAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class PIDGAMTest: MARTe::PIDGAM{
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API PIDGAMTest)
public:
    /**
     * @brief Default constructor. NOOP
     */
    PIDGAMTest();

    /**
     * @brief Default destructor. NOOP
     */
    virtual ~PIDGAMTest();

    /**
     * @brief Test error message of PIDGAM::Initialise()
     */
    bool TestInitialiseMissingKpKiKd();

    /**
     * @brief Test error message of PIDGAM::Initialise()
     */
    bool TestInitialiseMissingSampleTime();

    /**
     * @brief Test error message of PIDGAM::Initialise()
     */
    bool TestInitialiseWrongSampleTime();

    /**
     * @brief Test error message of PIDGAM::Initialise()
     */
    bool TestInitialiseMissingUpperSaturationLimit();

    /**
     * @brief Test error message of PIDGAM::Initialise()
     */
    bool TestInitialiseMissingLowerSaturationLimit();

    /**
     * @brief Test error message of PIDGAM::Initialise()
     * @details lower limit is larger than upper limit.
     */
    bool TestInitialiseWrongSaturationLimits();

    /**
     * @brief Test error message of PIDGAM::Initialise()
     * @ details lower and upper limit are equal.
     */
    bool TestInitialiseWrongSaturationLimits2();

    /**
     * @brief Test PIDGAM::Initialise() with a valid configuration
     */
    bool TestInitialise();

    /**
     * @brief Test error message of PIDGAM::Setup()
     * @details number of input equal to 0
     */
    bool TestSetupWrongNumberOfInputs();

    /**
     * @brief Test error message of PIDGAM::Setup()
     * @details number of input equal to 3
     */
    bool TestSetupWrongNumberOfInputs2();

    /**
     * @brief Test error message of PIDGAM::Setup()
     * @details number of output equal to 0
     */
    bool TestSetupWrongNumberOfOutputs();

    /**
     * @brief Test error message of PIDGAM::Setup()
     * @details number of output equal to 2
     */
    bool TestSetupWrongNumberOfOutputs2();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoInputElementsReference();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongInputElementsReference();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoInputElementsMeasurement();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongInputElementsMeasurement();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoOutputElements();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongOutputElements();
    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoInputSamplesReference();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongInputSamplesReference();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoInputSamplesMeasurement();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongInputSamplesMeasurement();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoOutputSamples();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongOutputSamples();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoInputDimensionReference();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongInputDimensionReference();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoInputDimensionMeasurement();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongInputDimensionMeasurement();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoOutputDimension();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongOutputDimension();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoReferenceInputType();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongReferenceInputType();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoMeasurementInputType();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongMeasurementInputType();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupNoOutputType();

    /**
     * @brief Test error message of PIDGAM::Setup()
     */
    bool TestSetupWrongOutputType();

    /**
     * @brief Test the PIDGAM::Setup() with a single input and a valid configuration.
     */
    bool TestSetup1InputSignal();

    /**
     * @brief Test the PIDGAM::Setup() with a two inputs and a valid configuration.
     */
    bool TestSetup2InputSignals();

    /**
     * @brief Test the PIDGAM::Execute() with two inputs and only the proportional term.
     */
    bool TestExecutekpSubtract();

    /**
     * @brief Test the PIDGAM::Execute() with a single input and only the proportional term.
     */
    bool TestExecutekp();

    /**
     * @brief Test the PIDGAM::Execute() with two inputs and only the integral term.
     * @details the input reference goes from 1 to 0.
     */
    bool TestExecutekiSubtract();

    /**
     * @brief Test the PIDGAM::Execute() with a single input and only the integral term.
     * @details the input reference goes from 1 to 0.
     */
    bool TestExecuteki();

    /**
     * @brief Test the PIDGAM::Execute() with two inputs and only the integral term.
     * @details the input reference always is 1.
     */
    bool TestExecutekiSubtract2();

    /**
     * @brief Test the PIDGAM::Execute() with a single input and only the integral term.
     * @details the input reference always is 1.
     */
    bool TestExecuteki2();

    /**
     * @brief Test the PIDGAM::Execute() with two inputs and only the derivative term.
     */
    bool TestExecutekdSubtract();

    /**
     * @brief Test the PIDGAM::Execute() with a single input and only the derivative term.
     */
    bool TestExecutekd();

    /**
     * @brief Test the PIDGAM::Execute() with two inputs and proportional and integral term.
     * @details the expected values are extracted from a Simulink simulation.
     */
    bool TestExecutekpkiSubtract();

    /**
     * @brief Test the PIDGAM::Execute() with all terms.
     * @details the expected values are extracted from a Simulink simulation.
     */
    bool TestExecutekpkikdSubtract();

    /**
     * @brief Test the PIDGAM::Execute() with all terms.
     * @details compares two PIDGAM with the same kp, ki and kd but one with a single input and the other with two inputs.
     */
    bool TestExecutekpkikdCompareBoth();

    /**
     * @brief Test the PIDGAM::Execute() with saturation.
     */
    bool TestExecuteSaturationkp();

    /**
     * @brief Test the PIDGAM::Execute() with saturation.
     */
    bool TestExecuteSaturationki();

    /**
     * @brief Test the PIDGAM::Execute() with upper saturation acting.
     * @details First the output is saturated and then the reference is change in order not
     * exceed the output limits.
     */
    bool TestExecuteSaturationki2Subtract();

    /**
     * @brief Test the PIDGAM::Execute() with lower saturation acting.
     * @details First the output is saturated and then the reference is change in order not
     * exceed the output limits.
     */
    bool TestExecuteSaturationki3Subtract();

    /**
     * @brief Test the PIDGAM::Execute() with saturation and single input.
     * @details the behaviour should be the same as TestExecuteSaturationki2Subtract.
     */
    bool TestExecuteSaturationki2();

    /**
     * @brief Test the PIDGAM::Execute() with saturation and single input.
     * @details the behaviour should be the same as TestExecuteSaturationki3Subtract.
     */
    bool TestExecuteSaturationki3();


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PIDGAMTEST_H_ */

