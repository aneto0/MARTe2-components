/**
 * @file FilterGAM.cpp
 * @brief Source file for class FilterGAM
 * @date Jan 30, 2017 TODO Verify the value and format of the date
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

 * @details This source file contains the definition of all the methods for
 * the class FilterGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "FilterGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
FilterGAM::FilterGAM() {
    num = NULL_PTR(float32 *);
    den = NULL_PTR(float32 *);
    numberOfNumCoeff = 0;
    numberOfDenCoeff = 0;
    lastInputs = NULL_PTR(float32 *);
    lastOutputs = NULL_PTR(float32 *);
    staticGain = 0;
    numberOfSamples = 0;
    output = NULL_PTR(float32 *);
    input = NULL_PTR(float32 *);
}

FilterGAM::~FilterGAM() {
    if (num != NULL_PTR(float32 *)) {
        delete[] num;
    }
    if (den != NULL_PTR(float32 *)) {
        delete[] den;
    }
    if (lastInputs != NULL_PTR(float32 *)) {
        delete[] lastInputs;
    }
    if (lastOutputs != NULL_PTR(float32 *)) {
        delete[] lastOutputs;
    }
}

bool FilterGAM::Initialise(StructuredDataI& data) {
    AnyType functionsArray = data.GetType("Num");
    bool ret = true;
    bool ok = (functionsArray.GetDataPointer() != NULL);
    numberOfNumCoeff = 0u;
    if (ok) {
        numberOfNumCoeff = functionsArray.GetNumberOfElements(0u);
        ok = (numberOfNumCoeff > 0u);
    }
    if (ok) {
        num = new float32[numberOfNumCoeff];
        Vector<float32> numVector(num, numberOfNumCoeff);
        ok = (data.Read("Num", numVector));
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "%s::Error reading numerator", GetName())
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "%s::No numerator defined", GetName())
    }
    ret &= ok;

    AnyType functionsArray1 = data.GetType("Den");
    //ok is reused in the denominator initialization.
    ok = (functionsArray1.GetDataPointer() != NULL);
    numberOfDenCoeff = 0u;
    if (ok) {
        numberOfDenCoeff = functionsArray1.GetNumberOfElements(0u);
        ok &= (numberOfDenCoeff > 0u);
    }
    if (ok) {
        den = new float32[numberOfDenCoeff];
        Vector<float32> denVector(den, numberOfDenCoeff);
        data.Read("Den", denVector);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "%s::Error reading denominator", GetName());
        }
        else {
            ok &= CheckNormalisation();
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
                                        "%s::The coefficients of the filter must be normalised before being introduced into the GAM", GetName());
            }
        }
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "%s::No denominator defined", GetName())
    }
    //Update the ret value
    ret &= ok;
    if (ret) {
        float32 sumNumerator = 0;
        for (uint32 i = 0; i < numberOfNumCoeff; i++) {
            sumNumerator += num[i];
        }
        float32 sumDenominator = 0;
        for (uint32 i = 0; i < numberOfDenCoeff; i++) {
            sumDenominator += den[i];
        }
        staticGain = sumNumerator / sumDenominator;
        //initialise lastInputs & lastOutputs
        lastInputs = new float32[numberOfNumCoeff - 1u];
        lastOutputs = new float32[numberOfDenCoeff - 1u];
        for (uint32 i = 0; i < numberOfDenCoeff - 1u; i++) {
            lastOutputs[i] = 0.0;
        }
        for (uint32 i = 0; i < numberOfNumCoeff - 1u; i++) {
            lastInputs[i] = 0.0;
        }
    }
    return ret;
}

bool FilterGAM::Setup() {
    bool errorDetected = false;
    uint32 numberOfInputSignals = GetNumberOfInputSignals();
    bool ok = (numberOfInputSignals == 1u);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfInputSignals = %u. The number of input signals must be one", GetName(),
                                numberOfInputSignals);
        errorDetected = true;
    }
    uint32 numberOfOutputSignals = GetNumberOfOutputSignals();
    ok = (numberOfOutputSignals == 1u);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::NumberOfOutputSignals = %u. The number of output signals must be one", GetName(),
                                numberOfOutputSignals);
        errorDetected = true;
    }

    uint32 numberOfSamplesInput;
    uint32 numberOfElementsInput;
    uint32 numberOfSamplesOutput;
    uint32 numberOfElementsOutput;
    ok = GetSignalNumberOfSamples(InputSignals, 0u, numberOfSamplesInput);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfSamples for the inputs failed", GetName());
        errorDetected = true;
    }
    ok = GetSignalNumberOfSamples(OutputSignals, 0u, numberOfSamplesOutput);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfSamples for the outputs failed ", GetName());
        errorDetected = true;
    }
    ok = GetSignalNumberOfElements(InputSignals, 0, numberOfElementsInput);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfElements for the inputs failed ", GetName());
        errorDetected = true;
    }
    ok = GetSignalNumberOfElements(OutputSignals, 0, numberOfElementsOutput);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfElements for the outputs failed ", GetName());
        errorDetected = true;
    }
    ok = (numberOfSamplesOutput == 1u);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamplesOutput must be 1 ", GetName());
        errorDetected = true;
    }

    if (numberOfSamplesInput == 1u) {
        numberOfSamples = numberOfElementsInput;
    }
    else {
        ok = (numberOfElementsInput == 1);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamplesInput = %u and numberOfElementsInput = %u (should be 1) ", GetName(),
                                    numberOfSamplesInput, numberOfElementsInput);
            errorDetected = true;
        }
        numberOfSamples = numberOfSamplesInput;
    }
    //Input and output size must be the same
    ok = (numberOfSamples == numberOfElementsOutput);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::number of input samples = %d != %d = number of output samples", GetName(),
                                numberOfSamples, numberOfElementsOutput);
        errorDetected = true;
    }
    uint32 inputDimension;
    ok = GetSignalNumberOfDimensions(InputSignals, 0u, inputDimension);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfDimensions for InputSignals fails", GetName());
        errorDetected = true;
        ok = true;
    }
    uint32 outputDimension;
    ok = GetSignalNumberOfDimensions(OutputSignals, 0u, outputDimension);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfDimensions for OutputSignals fails", GetName());
        errorDetected = true;
    }
    ok = (numberOfSamples > 0u);
    if (!ok) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamples is 0", GetName());
        errorDetected = true;
        ok = true;
    }
    if (!errorDetected) {
        input = static_cast<float32 *>(GetInputSignalMemory(0));
        output = static_cast<float32 *>(GetOutputSignalMemory(0));
    }
    return !errorDetected;
}

bool FilterGAM::Execute() {
    uint32 n = 0u;
    float32 accumulator = 0.0;
    while (n < numberOfSamples) {
        accumulator = 0;
        //sum inputs
        for (uint32 k = 0u; k < numberOfNumCoeff; k++) {
            if (n >= k) {
                accumulator += input[n - k] * num[k];
            }
            else {
                accumulator += lastInputs[k - n - 1] * num[k];
            }
        }
        //Sum outputs
        for (uint32 k = 1u; k < numberOfDenCoeff; k++) {
            if (n >= k) {
                accumulator -= output[n - k] * den[k];
            }
            else {
                accumulator -= lastOutputs[k - n - 1] * den[k];
            }
        }
        output[n] = accumulator;
        n++;
    }

    //update the last values
    for (uint32 k = 0; k < numberOfNumCoeff - 1; k++) {
        lastInputs[k] = input[numberOfSamples - 1 - k];
    }
    for (uint32 k = 0; k < numberOfDenCoeff - 1; k++) {
        lastOutputs[k] = output[numberOfSamples - 1u - k];
    }
    return true;
}

uint32 FilterGAM::GetNumberOfNumCoeff() {
    return numberOfNumCoeff;
}

uint32 FilterGAM::GetNumberOfDenCoeff() {
    return numberOfDenCoeff;
}

bool FilterGAM::GetNumCoeff(float *coeff) {
    bool ret = false;
    if (num != NULL_PTR(float32 *)) {
        for (uint32 i = 0; i < numberOfNumCoeff; i++) {
            coeff[i] = num[i];
        }
        ret = true;
    }
    return ret;
}

bool FilterGAM::GetDenCoeff(float *coeff) {
    bool ret = false;
    if (den != NULL_PTR(float32 *)) {
        for (uint32 i = 0; i < numberOfDenCoeff; i++) {
            coeff[i] = den[i];
        }
        ret = true;
    }
    return ret;
}

bool FilterGAM::CheckNormalisation() {
    bool ret = false;
    if (den != NULL_PTR(float32 *)) {
        ret = (1 == den[0]);
    }
    return ret;
}

float32 FilterGAM::GetStaticGain() {
    return staticGain;
}

}

