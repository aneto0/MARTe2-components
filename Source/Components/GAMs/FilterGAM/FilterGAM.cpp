/**
 * @file FilterGAM.cpp
 * @brief Source file for class FilterGAM
 * @date 30/01/2017
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
FilterGAM::FilterGAM() :
        GAM() {
    num = NULL_PTR(float32*);
    den = NULL_PTR(float32*);
    numberOfNumCoeff = 0u;
    numberOfDenCoeff = 0u;
    staticGain = 0.0F;
    numberOfSamples = 0u;
    lastInputs = NULL_PTR(float32**);
    lastOutputs = NULL_PTR(float32**);
    output = NULL_PTR(float32**);
    input = NULL_PTR(float32**);
    numberOfSignals = 0u;
    gainInfinite = false;
    resetInEachState = true;
}

FilterGAM::~FilterGAM() {
    if (num != NULL_PTR(float32*)) {
        delete[] num;
    }
    if (den != NULL_PTR(float32*)) {
        delete[] den;
    }
    if (lastInputs != NULL_PTR(float32**)) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            if (lastInputs[i] != NULL_PTR(float32*)) {
                delete[] lastInputs[i];
            }
        }
        delete[] lastInputs;
    }
    if (lastOutputs != NULL_PTR(float32**)) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            if (lastOutputs[i] != NULL_PTR(float32*)) {
                delete[] lastOutputs[i];
            }
        }
        delete[] lastOutputs;
    }
    if (input != NULL_PTR(float32**)) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            if (input[i] != NULL_PTR(float32*)) {
                input[i] = NULL_PTR(float32*);
            }
        }
        delete[] input;
    }
    if (output != NULL_PTR(float32**)) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            if (output[i] != NULL_PTR(float32*)) {
                output[i] = NULL_PTR(float32*);
            }
        }
        delete[] output;
    }
}

bool FilterGAM::Initialise(StructuredDataI &data) {
    AnyType functionsArray = data.GetType("Num");
    bool errorDetected = false;
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = (functionsArray.GetDataPointer() != NULL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting pointer to the numerator");
            errorDetected = true;
        }
    }
    if (ok) {
        numberOfNumCoeff = functionsArray.GetNumberOfElements(0u);
        ok = (numberOfNumCoeff > 0u);
    }
    if ((!ok) && (!errorDetected)) {
        //Unlikely to be hit as empty arrays should be detected by the condition above.
        REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfNumCoeff must be > 0");
        errorDetected = true;
    }
    if (ok) {
        num = new float32[numberOfNumCoeff];
        Vector<float32> numVector(num, numberOfNumCoeff);
        ok = (data.Read("Num", numVector));
        if ((!ok) && (!errorDetected)) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numerator");
            errorDetected = true;
        }
    }
    AnyType functionsArray1 = data.GetType("Den");
    if (ok) {
        ok = (functionsArray1.GetDataPointer() != NULL);
    }
    if ((!ok) && (!errorDetected)) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting pointer to the denominator");
        errorDetected = true;
    }
    if (ok) {
        numberOfDenCoeff = functionsArray1.GetNumberOfElements(0u);
        ok = (numberOfDenCoeff > 0u);
    }
    if ((!ok) && (!errorDetected)) {
        //Unlikely to be hit as empty arrays should be detected by the condition above.
        REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfDenCoeff must be > 0");
        errorDetected = true;
    }
    if (ok) {
        den = new float32[numberOfDenCoeff];
        Vector<float32> denVector(den, numberOfDenCoeff);
        ok = data.Read("Den", denVector);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading denominator");
            errorDetected = true;
        }
        else {
            ok = CheckNormalisation();
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The coefficients of the filter must be normalised before being introduced into the GAM");
                errorDetected = true;
            }
        }
    }
    if (ok) {
        float32 sumNumerator = 0.0F;
        for (uint32 i = 0u; i < numberOfNumCoeff; i++) {
            //if due to MISRA rules however it is not necessary. At his line the initialization of num is guaranteed b the ok = true...
            if (num != NULL_PTR(float32*)) {
                sumNumerator += num[i];
            }
        }
        float32 sumDenominator = 0.0F;
        for (uint32 i = 0u; i < numberOfDenCoeff; i++) {
            //if due to MISRA rules however it is not necessary. At his line the initialization of den is guaranteed b the ok = true...
            if (den != NULL_PTR(float32*)) {
                sumDenominator += den[i];
            }
        }
        if (!IsEqual(sumDenominator, 0.0F)) {
            //lint -e{414} sumDenominator cannot be 0.
            staticGain = sumNumerator / sumDenominator;
        }
        else {
            gainInfinite = true;
        }
    }
    if (ok) {
        uint32 aux;
        ok = data.Read("ResetInEachState", aux);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::Warning, "ResetInEachState not specified. resetInEachState = true by default ");
        }
        else {
            if (aux == 1u) {
                resetInEachState = true;
            }
            else if (aux == 0u) {
                resetInEachState = false;
            }
            else {
                if (!errorDetected) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Wrong value for ResetInEachState (expected values 0 or 1)");
                    errorDetected = true;
                }
            }
        }
    }
    return !errorDetected;
}

bool FilterGAM::Setup() {
    bool errorDetected = false;

    bool ok = true;
    numberOfSignals = GetNumberOfInputSignals();
    if (numberOfSignals == 0u) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfSignals must be positive");
        errorDetected = true;
    }
    uint32 numberOfOutputSignalsFilter;
    if (!errorDetected) {
        numberOfOutputSignalsFilter = GetNumberOfOutputSignals();
        if (numberOfOutputSignalsFilter != numberOfSignals) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfOutputSignalsFilter = %u != %u = numberOfInputSignals", numberOfOutputSignalsFilter,
                         numberOfSignals);
            errorDetected = true;
        }
    }
    //initialise lastInputs & lastOutputs
    if (!errorDetected) {
        lastInputs = new float32*[numberOfSignals];
        lastOutputs = new float32*[numberOfSignals];
        uint32 numberOfElementsToSet1 = static_cast<uint32>(sizeof(float32*) * numberOfSignals);
        (void) MemoryOperationsHelper::Set(reinterpret_cast<void *>(lastInputs), '\0', numberOfElementsToSet1);
        (void) MemoryOperationsHelper::Set(reinterpret_cast<void *>(lastOutputs), '\0', numberOfElementsToSet1);
        //if due to MISRA rules
        if ((lastInputs != NULL_PTR(float32**)) && (lastOutputs != NULL_PTR(float32**))) {
            /*lint -e{9114} implicit conversion is safe*/
            for (uint32 m = 0u; (m < numberOfSignals) && (!errorDetected); m++) {
                if (numberOfNumCoeff > 1u) {
                    lastInputs[m] = new float32[numberOfNumCoeff - 1u];
                    if (lastInputs[m] != NULL_PTR(float32*)) {
                        for (uint32 i = 0u; i < (numberOfNumCoeff - 1u); i++) {
                            lastInputs[m][i] = 0.0F;
                        }
                    }
                }
                if (numberOfDenCoeff > 1u) {
                    lastOutputs[m] = new float32[numberOfDenCoeff - 1u];
                    if (lastOutputs[m] != NULL_PTR(float32*)) {
                        for (uint32 i = 0u; i < (numberOfDenCoeff - 1u); i++) {
                            lastOutputs[m][i] = 0.0F;
                        }
                    }
                }
            }
        }
    }
    uint32 *numberOfSamplesInput = NULL_PTR(uint32*);
    uint32 *numberOfElementsInput = NULL_PTR(uint32*);
    uint32 *numberOfSamplesOutput = NULL_PTR(uint32*);
    uint32 *numberOfElementsOutput = NULL_PTR(uint32*);
    if (!errorDetected) {
        numberOfSamplesInput = new uint32[numberOfSignals];
        numberOfElementsInput = new uint32[numberOfSignals];
        numberOfSamplesOutput = new uint32[numberOfSignals];
        numberOfElementsOutput = new uint32[numberOfSignals];
        //lint -e{9113} -e{9131} allow multiple != in the same line since it has no effect and makes the code easier to read.
        ok = (numberOfSamplesInput != NULL_PTR(uint32 *) && numberOfElementsInput != NULL_PTR(uint32 *) && numberOfSamplesOutput != NULL_PTR(uint32 *) && numberOfElementsOutput != NULL_PTR(uint32 *));
        if (ok) {
            uint32 numberOfElementsToSet = static_cast<uint32>(numberOfSignals * sizeof(numberOfSignals));
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void *>(numberOfSamplesInput), '\0', numberOfElementsToSet);
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void *>(numberOfElementsInput), '\0', numberOfElementsToSet);
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void *>(numberOfSamplesOutput), '\0', numberOfElementsToSet);
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void *>(numberOfElementsOutput), '\0', numberOfElementsToSet);
        }
    }
//if due to MISRA rules
    if ((numberOfSamplesInput != NULL_PTR(uint32*)) && (numberOfElementsInput != NULL_PTR(uint32*)) && (numberOfSamplesOutput != NULL_PTR(uint32*))
            && (numberOfElementsOutput != NULL_PTR(uint32*))) {
        //Read input-output samples and elements for each signal.
        if (!errorDetected) {
            for (uint32 i = 0u; i < numberOfSignals; i++) {
                uint32 auxIndex = i;
                ok = GetSignalNumberOfSamples(InputSignals, i, numberOfSamplesInput[i]);
                if ((!ok) && (!errorDetected)) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfSamples for the input signal %u failed", auxIndex);
                    errorDetected = true;
                }
                ok = GetSignalNumberOfSamples(OutputSignals, i, numberOfSamplesOutput[i]);
                if ((!ok) && (!errorDetected)) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfSamples for the output signal %u failed ", auxIndex);
                    errorDetected = true;
                }
                ok = GetSignalNumberOfElements(InputSignals, i, numberOfElementsInput[i]);
                if ((!ok) && (!errorDetected)) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfElements for the input signal %u failed ", auxIndex);
                    errorDetected = true;
                }
                ok = GetSignalNumberOfElements(OutputSignals, i, numberOfElementsOutput[i]);
                if ((!ok) && (!errorDetected)) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfElements for the output signal %u failed ", auxIndex);
                    errorDetected = true;
                }
                ok = (numberOfSamplesOutput[i] == 1u);
                if ((!ok) && (!errorDetected)) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "numberOfSamplesOutput must be 1 ");
                    errorDetected = true;
                }
                ok = (GetSignalType(InputSignals, i) == Float32Bit);
                if ((!ok) && (!errorDetected)) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType for the input signal %u failed (not float32 as it should be) ", auxIndex);
                    errorDetected = true;
                }
                ok = (GetSignalType(OutputSignals, i) == Float32Bit);
                if ((!ok) && (!errorDetected)) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalType for the input signal %u failed (not float32 as it should be)", auxIndex);
                    errorDetected = true;
                }
            }
        }
    }
//Check that all the input signals have the same numberOfElementsInput
    if (!errorDetected) {
        //if due to MISRA rules
        if (numberOfElementsInput != NULL_PTR(uint32*)) {
            for (uint32 i = 1u; (i < numberOfSignals) && ok; i++) {
                ok = (numberOfElementsInput[0] == numberOfElementsInput[i]);
            }
        }
    }
    if ((!ok) && (!errorDetected)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfElementsInput is not the same for each input signal ");
        errorDetected = true;
    }
//Check that all the output signals have the same numberOfElementsOutput
    if (!errorDetected) {
        //if due to MISRA rules
        if (numberOfElementsOutput != NULL_PTR(uint32*)) {
            for (uint32 i = 1u; (i < numberOfSignals) && ok; i++) {
                ok = (numberOfElementsOutput[0] == numberOfElementsOutput[i]);
            }
        }
    }
    if ((!ok) && (!errorDetected)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfElementsOutput is not the same for each signal ");
        errorDetected = true;
    }
//Check that all the input signals have the same numberOfSamplesInput
    if (!errorDetected) {
        //if due to MISRA riles
        if (numberOfSamplesInput != NULL_PTR(uint32*)) {
            for (uint32 i = 1u; (i < numberOfSignals) && ok; i++) {
                ok = (numberOfSamplesInput[0] == numberOfSamplesInput[i]);
            }
        }
    }
    if ((!ok) && (!errorDetected)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfSamplesInput is not the same for each signal ");
        errorDetected = true;
    }
//Only the numberOfSamplesInput[0] have to be checked because, previously, it is check that the input dimensions etc are the same for each input signal.
    if (!errorDetected) {
        //if due to MISRA rules
        if ((numberOfSamplesInput != NULL_PTR(uint32*)) && (numberOfElementsInput != NULL_PTR(uint32*))) {
            if (numberOfSamplesInput[0] == 1u) {
                numberOfSamples = numberOfElementsInput[0];
            }
            else {
                ok = (numberOfElementsInput[0] == 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "numberOfSamplesInput = %u and numberOfElementsInput = %u (should be 1) ",
                                 numberOfSamplesInput[0], numberOfElementsInput[0]);
                    errorDetected = true;
                }
                numberOfSamples = numberOfSamplesInput[0];
            }
        }
    }
//Input and output size must be the same
//Check that all the signals have the same number of input & outputs
    if (!errorDetected) {
        if (numberOfElementsOutput != NULL_PTR(uint32*)) {
            for (uint32 i = 0u; (i < numberOfSignals) && ok; i++) {
                ok = (numberOfSamples == numberOfElementsOutput[i]);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Different input and output array size.");
                    errorDetected = true;
                }
            }
        }
    }
    uint32 inputDimension;
    if (!errorDetected) {
        for (uint32 i = 0u; (i < numberOfSignals) && ok; i++) {
            ok = GetSignalNumberOfDimensions(InputSignals, i, inputDimension);
            if ((!ok) && (!errorDetected)) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfDimensions for InputSignals fails");
                errorDetected = true;
            }
            ok = (inputDimension == 1u);
            if ((!ok) && (!errorDetected)) {
                REPORT_ERROR(ErrorManagement::ParametersError, "InputDimension = %u. It must be 1 (array)", inputDimension);
                errorDetected = true;
            }
        }
    }
    uint32 outputDimension;
    if (!errorDetected) {
        for (uint32 i = 0u; (i < numberOfSignals) && ok; i++) {
            ok = GetSignalNumberOfDimensions(OutputSignals, 0u, outputDimension);
            if ((!ok) && (!errorDetected)) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfDimensions for OutputSignals fails");
                errorDetected = true;
            }
            ok = (outputDimension == 1u);
            if ((!ok) && (!errorDetected)) {
                REPORT_ERROR(ErrorManagement::ParametersError, "OutputDimension = %u. It must be 1 (array)", outputDimension);
                errorDetected = true;
            }
        }
    }
    if (!errorDetected) {
        ok = (numberOfSamples > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "%numberOfSamples is 0");
            errorDetected = true;
        }
    }
    if (!errorDetected) {
        input = new float32*[numberOfSignals];
        output = new float32*[numberOfSignals];
        if ((input != NULL_PTR(float32**)) && (output != NULL_PTR(float32**))) {
            for (uint32 i = 0u; i < numberOfSignals; i++) {
                input[i] = static_cast<float32*>(GetInputSignalMemory(i));
                output[i] = static_cast<float32*>(GetOutputSignalMemory(i));
            }
        }
    }
//Free pointers MISRA rules
    if (numberOfSamplesInput != NULL_PTR(uint32*)) {
        delete[] numberOfSamplesInput;
    }
    if (numberOfElementsInput != NULL_PTR(uint32*)) {
        delete[] numberOfElementsInput;
    }
    if (numberOfSamplesOutput != NULL_PTR(uint32*)) {
        delete[] numberOfSamplesOutput;
    }
    if (numberOfElementsOutput != NULL_PTR(uint32*)) {
        delete[] numberOfElementsOutput;
    }
    return !errorDetected;
}

bool FilterGAM::Execute() {
    uint32 n = 0u;
    float32 accumulator;
//if due to MISRA rules...
    if ((input != NULL_PTR(float32**)) && (output != NULL_PTR(float32**)) && (lastInputs != NULL_PTR(float32**)) && (lastOutputs != NULL_PTR(float32**))
            && (num != NULL_PTR(float32*)) && (den != NULL_PTR(float32*))) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            n = 0u;
            while (n < numberOfSamples) {
                accumulator = 0.0F;
                //sum inputs
                for (uint32 k = 0u; k < numberOfNumCoeff; k++) {
                    if (n >= k) {
                        accumulator += input[i][n - k] * num[k];
                    }
                    else {
                        accumulator += (lastInputs[i][(k - n) - 1u]) * num[k];
                    }
                }
                //Sum outputs
                for (uint32 k = 1u; k < numberOfDenCoeff; k++) {
                    if (n >= k) {
                        accumulator -= output[i][n - k] * den[k];
                    }
                    else {
                        accumulator -= lastOutputs[i][(k - n) - 1u] * den[k];
                    }
                }
                output[i][n] = accumulator;
                n++;
            }

            //update the last values
            if (numberOfNumCoeff > 2u) {
                /*lint -e{9117} implicit conversion is safe*/
                for (uint32 k = (numberOfNumCoeff - 2u); k > 0u; k--) {
                    if (numberOfSamples > k) {
                        lastInputs[i][k] = input[i][(numberOfSamples - k) - 1u];
                    }
                    else {
                        lastInputs[i][k] = lastInputs[i][k - 1];
                    }
                }
            }
            if (numberOfNumCoeff > 1u) {
                lastInputs[i][0] = input[i][numberOfSamples - 1u];
            }
            if (numberOfDenCoeff > 2u) {
                /*lint -e{9117} implicit conversion is safe*/
                for (uint32 k = (numberOfDenCoeff - 2u); k > 0u; k--) {
                    if (numberOfSamples > k) {
                        lastOutputs[i][k] = output[i][(numberOfSamples - k) - 1u];
                    }
                    else {
                        lastOutputs[i][k] = lastOutputs[i][k - 1];
                    }
                }
            }
            if (numberOfDenCoeff > 1u) {
                lastOutputs[i][0] = output[i][numberOfSamples - 1u];
            }
        }
    }
    return true;
}

bool FilterGAM::GetResetInEachState() const {
    return resetInEachState;
}
bool FilterGAM::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    bool ret = true;
    if (resetInEachState) {
        if ((lastInputs != NULL_PTR(float32**)) && (lastOutputs != NULL_PTR(float32**))) {
            for (uint32 i = 0u; i < numberOfSignals; i++) {
                if (lastInputs[i] != NULL_PTR(float32*)) {
                    for (uint32 n = 0u; n < (numberOfNumCoeff - 1u); n++) {
                        lastInputs[i][n] = 0.0F;
                    }
                }
                if (lastOutputs[i] != NULL_PTR(float32*)) {
                    for (uint32 n = 0u; n < (numberOfDenCoeff - 1u); n++) {
                        lastOutputs[i][n] = 0.0F;
                    }
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "lastInputs or lastOutputs = NULL ");
            ret = false;
        }
    }
    else {
        //If the currentStateName and lastStateExecuted are different-> rest values
        if (lastStateExecuted != currentStateName) {
            if ((lastInputs != NULL_PTR(float32**)) && (lastOutputs != NULL_PTR(float32**))) {
                for (uint32 i = 0u; i < numberOfSignals; i++) {
                    if (lastInputs[i] != NULL_PTR(float32*)) {
                        for (uint32 n = 0u; n < (numberOfNumCoeff - 1u); n++) {
                            lastInputs[i][n] = 0.0F;
                        }
                    }
                    if (lastOutputs[i] != NULL_PTR(float32*)) {
                        for (uint32 n = 0u; n < (numberOfDenCoeff - 1u); n++) {
                            lastOutputs[i][n] = 0.0F;
                        }
                    }
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "lastInputs or lastOutputs = NULL ");
                ret = false;
            }
        }
        lastStateExecuted = nextStateName;
    }
    return ret;
}

uint32 FilterGAM::GetNumberOfNumCoeff() const {
    return numberOfNumCoeff;
}

uint32 FilterGAM::GetNumberOfDenCoeff() const {
    return numberOfDenCoeff;
}

bool FilterGAM::GetNumCoeff(float32 *const coeff) const {
    bool ret = false;
    if (num != NULL_PTR(float32*)) {
        for (uint32 i = 0U; i < numberOfNumCoeff; i++) {
            coeff[i] = num[i];
        }
        ret = true;
    }
    return ret;
}

bool FilterGAM::GetDenCoeff(float32 *const coeff) const {
    bool ret = false;
    if (den != NULL_PTR(float32*)) {
        for (uint32 i = 0u; i < numberOfDenCoeff; i++) {
            coeff[i] = den[i];
        }
        ret = true;
    }
    return ret;
}

bool FilterGAM::CheckNormalisation() const {
    bool ret = false;
    if (den != NULL_PTR(float32*)) {
        ret = (IsEqual(static_cast<float32>(1.0), den[0]));
    }
    return ret;
}

float32 FilterGAM::GetStaticGain(bool &isInfinite) const {
    isInfinite = gainInfinite;
    return staticGain;
}

uint32 FilterGAM::GetNumberOfSignals() const {
    return numberOfSignals;
}
uint32 FilterGAM::GetNumberOfSamples() const {
    return numberOfSamples;
}
CLASS_REGISTER(FilterGAM, "1.0")
}

