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
    staticGain = 0;
    numberOfSamples = 0;
    lastInputs = NULL_PTR(float32 **);
    lastOutputs = NULL_PTR(float32 **);
    output = NULL_PTR(float32 **);
    input = NULL_PTR(float32 **);
    numberOfSignals = 0;
}

FilterGAM::~FilterGAM() {
    if (num != NULL_PTR(float32 *)) {
        delete[] num;
    }
    if (den != NULL_PTR(float32 *)) {
        delete[] den;
    }
    if (lastInputs != NULL_PTR(float32 **)) {
        for (uint32 i = 0; i < numberOfSignals; i++) {
            if (lastInputs[i] != NULL_PTR(float32 *)) {
                delete[] lastInputs[i];
            }
        }
        delete[] lastInputs;
    }
    if (lastOutputs != NULL_PTR(float32 **)) {
        for (uint32 i = 0; i < numberOfSignals; i++) {
            if (lastOutputs[i] != NULL_PTR(float32 *)) {
                delete[] lastOutputs[i];
            }
        }
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
    }
    return ret;
}

bool FilterGAM::Setup() {

    bool errorDetected = false;
    bool ok = true;
    numberOfSignals = GetNumberOfInputSignals();
    if (numberOfSignals == 0) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSignals must be positive", GetName());
        errorDetected = true;
    }
    uint32 numberOfOutputSignals;
    if (!errorDetected) {
        numberOfOutputSignals = GetNumberOfOutputSignals();
        if (numberOfOutputSignals != numberOfSignals) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfOutputSignals = %u != %u = numberOfInputSignals", GetName(),
                                    numberOfOutputSignals, numberOfSignals);
            errorDetected = true;
        }
    }
    //initialise lastInputs & lastOutputs
    if (!errorDetected) {
        lastInputs = new float32 *[numberOfSignals];
        lastOutputs = new float32*[numberOfSignals];
        if (lastInputs == NULL_PTR(float32 **) || lastOutputs == NULL_PTR(float32 **)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::Error allocating memory for lastInputs & outputs array pointers", GetName());
            errorDetected = true;
        }
        for (uint32 m = 0; m < numberOfSignals && !errorDetected; m++) {
            lastInputs[m] = new float32[numberOfNumCoeff - 1u];
            lastOutputs[m] = new float32[numberOfDenCoeff - 1u];
            if (lastInputs[m] == NULL_PTR(float32 *) || lastOutputs[m] == NULL_PTR(float32 *)) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::Error allocating memory for lastInputs & outputs values", GetName());
                errorDetected = true;
            }
            else {
                for (uint32 i = 0; i < numberOfDenCoeff - 1u; i++) {
                    lastOutputs[m][i] = 0.0;
                }
                for (uint32 i = 0; i < numberOfNumCoeff - 1u; i++) {
                    lastInputs[m][i] = 0.0;
                }
            }
        }
    }

    uint32 *numberOfSamplesInput = NULL_PTR(uint32 *);
    uint32 *numberOfElementsInput = NULL_PTR(uint32 *);
    uint32 *numberOfSamplesOutput = NULL_PTR(uint32 *);
    uint32 *numberOfElementsOutput = NULL_PTR(uint32 *);
    if (!errorDetected) {
        numberOfSamplesInput = new uint32[numberOfSignals];
        if (numberOfSamplesInput == NULL_PTR(uint32 *)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::Error allocating memory for numberOfSamplesInput", GetName());
            errorDetected = true;
        }
    }
    if (!errorDetected) {
        numberOfElementsInput = new uint32[numberOfSignals];
        if (numberOfElementsInput == NULL_PTR(uint32 *)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::Error allocating memory for numberOfElementsInput", GetName());
            errorDetected = true;
        }
    }
    if (!errorDetected) {
        numberOfSamplesOutput = new uint32[numberOfSignals];
        if (numberOfSamplesOutput == NULL_PTR(uint32 *)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::Error allocating memory for numberOfSamplesOutput", GetName());
            errorDetected = true;
        }
    }
    if (!errorDetected) {
        numberOfElementsOutput = new uint32[numberOfSignals];
        if (numberOfElementsOutput == NULL_PTR(uint32 *)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::Error allocating memory for numberOfElementsOutput", GetName());
            errorDetected = true;
        }
    }

    //Read input-output samples and elements for each signal.
    if (!errorDetected) {
        for (uint32 i = 0; i < numberOfSignals; i++) {
            ok = GetSignalNumberOfSamples(InputSignals, i, numberOfSamplesInput[i]);
            if (!ok && !errorDetected) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfSamples for the input signal %u failed", GetName(), i);
                errorDetected = true;
            }
            ok = GetSignalNumberOfSamples(OutputSignals, i, numberOfSamplesOutput[i]);
            if (!ok && !errorDetected) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfSamples for the output signal %u failed ", GetName(), i);
                errorDetected = true;
            }
            ok = GetSignalNumberOfElements(InputSignals, i, numberOfElementsInput[i]);
            if (!ok & !errorDetected) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfElements for the input signal %u failed ", GetName(), i);
                errorDetected = true;
            }
            ok = GetSignalNumberOfElements(OutputSignals, i, numberOfElementsOutput[i]);
            if (!ok && !errorDetected) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfElements for the output signal %u failed ", GetName(), i);
                errorDetected = true;
            }
            ok = (numberOfSamplesOutput[i] == 1u);
            if (!ok && !errorDetected) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamplesOutput must be 1 ", GetName());
                errorDetected = true;
            }
        }
    }
    //Check that all the input signals have the same numberOfElementsInput
    ok = true;
    for (uint32 i = 1; i < numberOfSignals && !errorDetected; i++) {
        ok &= (numberOfElementsInput[0] == numberOfElementsInput[i]);
    }
    if (!ok && !errorDetected) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfElementsInput is not the same for each input signal ", GetName());
        errorDetected = true;
    }
    //Check that all the output signals have the same numberOfElementsOutput
    ok = true;
    for (uint32 i = 1; i < numberOfSignals && !errorDetected; i++) {
        ok &= (numberOfElementsOutput[0] == numberOfElementsOutput[i]);
    }
    if (!ok && !errorDetected) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfElementsOutput is not the same for each signal ", GetName());
        errorDetected = true;
    }
    //Check that all the input signals have the same numberOfSamplesInput
    ok = true;
    for (uint32 i = 1; i < numberOfSignals && !errorDetected; i++) {
        ok &= numberOfSamplesInput[0] == numberOfSamplesInput[i];
    }
    if (!ok && !errorDetected) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamplesInput is not the same for each signal ", GetName());
        errorDetected = true;
    }
    //Only the numberOfSamplesInput[0] have to be checked because, previously, it is check that the input dimensions etc are the same for each input signal.
    if (!errorDetected) {
        if (numberOfSamplesInput[0] == 1u) {
            numberOfSamples = numberOfElementsInput[0];
        }
        else {
            ok = (numberOfElementsInput[0] == 1);
            if (!ok) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamplesInput = %u and numberOfElementsInput = %u (should be 1) ",
                                        GetName(), numberOfSamplesInput[0], numberOfElementsInput[0]);
                errorDetected = true;
            }
            numberOfSamples = numberOfSamplesInput[0];
        }
    }
    //Input and output size must be the same
    //Check that all the signals have the same number of input & outputs
    ok = true;
    for (uint32 i = 0; i < numberOfSignals && !errorDetected; i++) {
        ok &= (numberOfSamples == numberOfElementsOutput[i]);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::Different input and output array size.", GetName());
            errorDetected = true;
        }
    }
    uint32 inputDimension;
    for (uint32 i = 0; i < numberOfSignals && !errorDetected; i++) {
        ok = GetSignalNumberOfDimensions(InputSignals, i, inputDimension);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfDimensions for InputSignals fails", GetName());
            errorDetected = true;
        }
        ok = (inputDimension == 1);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::InputDimension = %u. Must be 1 (array)", GetName(), inputDimension);
            errorDetected = true;
        }
    }
    uint32 outputDimension;
    for (uint32 i = 0; i < numberOfSignals && !errorDetected; i++) {
        ok = GetSignalNumberOfDimensions(OutputSignals, 0u, outputDimension);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::GetSignalNumberOfDimensions for OutputSignals fails", GetName());
            errorDetected = true;
        }
        ok = (outputDimension == 1);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::InputDimension = %u. Must be 1 (array)", GetName(), outputDimension);
            errorDetected = true;
        }
    }
    if (!errorDetected) {
        ok = (numberOfSamples > 0u);
        if (!ok) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamples is 0", GetName());
            errorDetected = true;
            ok = true;
        }
    }
    if (!errorDetected) {
        input = new float32 *[numberOfSignals];
        output = new float32 *[numberOfSignals];
        if (input == NULL_PTR(float32 **) || output == NULL_PTR(float32 **)) {
            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "%s::numberOfSamples is 0", GetName());
            errorDetected = true;
        }
        else {
            for (uint32 i = 0; i < numberOfSignals; i++) {
                input[i] = static_cast<float32 *>(GetInputSignalMemory(i));
                output[i] = static_cast<float32 *>(GetOutputSignalMemory(i));
            }
        }
    }
    return !errorDetected;
}

bool FilterGAM::Execute() {
    uint32 n = 0u;
    float32 accumulator = 0.0;
    for (uint32 i = 0; i < numberOfSignals; i++) {
        n = 0u;
        while (n < numberOfSamples) {
            accumulator = 0.0;
            //sum inputs
            for (uint32 k = 0u; k < numberOfNumCoeff; k++) {
                if (n >= k) {
                    accumulator += input[i][n - k] * num[k];
                }
                else {
                    accumulator += lastInputs[i][k - n - 1] * num[k];
                }
            }
            //Sum outputs
            for (uint32 k = 1u; k < numberOfDenCoeff; k++) {
                if (n >= k) {
                    accumulator -= output[i][n - k] * den[k];
                }
                else {
                    accumulator -= lastOutputs[i][k - n - 1] * den[k];
                }
            }
            output[i][n] = accumulator;
            n++;
        }

        //update the last values
        for (uint32 k = 0; k < numberOfNumCoeff - 1; k++) {
            lastInputs[i][k] = input[i][numberOfSamples - 1 - k];
        }
        for (uint32 k = 0; k < numberOfDenCoeff - 1; k++) {
            lastOutputs[i][k] = output[i][numberOfSamples - 1u - k];
        }
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

uint32 FilterGAM::GetNumberOfSignals() {
    return numberOfSignals;
}
uint32 FilterGAM::GetNumberOfSamples() {
    return numberOfSamples;
}
}

