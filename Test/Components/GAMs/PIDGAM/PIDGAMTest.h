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
    PIDGAMTest();
    virtual ~PIDGAMTest();
    bool TestInitialiseMissingKpKiKd();
    bool TestInitialiseMissingSampleTime();
    bool TestInitialiseWrongSampleTime();
    bool TestInitialiseWrongSaturationLimits();
    bool TestInitialiseWrongSaturationLimits2();
    bool TestInitialise();
    bool TestSetupWrongNumberOfInputs();
    bool TestSetupWrongNumberOfInputs2();
    bool TestSetupWrongNumberOfOutputs();
    bool TestSetupWrongNumberOfOutputs2();
    bool TestSetupNoInputElementsReference();
    bool TestSetupWrongInputElementsReference();
    bool TestSetupNoInputElementsMeasurement();
    bool TestSetupWrongInputElementsMeasurement();
    bool TestSetupNoOutputElements();
    bool TestSetupWrongOutputElements();
    bool TestSetupNoInputSamplesReference();
    bool TestSetupWrongInputSamplesReference();
    bool TestSetupNoInputSamplesMeasurement();
    bool TestSetupWrongInputSamplesMeasurement();
    bool TestSetupNoOutputSamples();
    bool TestSetupWrongOutputSamples();
    bool TestSetupNoInputDimensionReference();
    bool TestSetupWrongInputDimensionReference();
    bool TestSetupNoInputDimensionMeasurement();
    bool TestSetupWrongInputDimensionMeasurement();
    bool TestSetupNoOutputDimension();
    bool TestSetupWrongOutputDimension();
    bool TestSetupNoReferenceInputType();
    bool TestSetupWrongReferenceInputType();
    bool TestSetupNoMeasurementInputType();
    bool TestSetupWrongMeasurementInputType();
    bool TestSetupNoOutputType();
    bool TestSetupWrongOutputType();
    bool TestSetup1InputSignal();
    bool TestSetup2InputSignals();
    bool TestExecutekpSubtract();
    bool TestExecutekp();
    bool TestExecutekiSubtract();
    bool TestExecuteki();
    bool TestExecutekiSubtract2();
    bool TestExecuteki2();
    bool TestExecutekdSubtract();
    bool TestExecutekd();
    bool TestExecutekpkikdSubtract();
    bool TestExecutekpkikdCompareBoth();
    bool TestExecuteSaturationkp();
    bool TestExecuteSaturationki();
    bool TestExecuteSaturationki2();


};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PIDGAMTEST_H_ */

