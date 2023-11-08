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
    filterRef = NULL_PTR(Filter*);
    filters = NULL_PTR(Filter**);
//    staticGain = 0.0F;
    nOfSamples = 0u;
    output = NULL_PTR(void**);
    input = NULL_PTR(void**);
    nOfSignals = 0u;
    gainInfinite = false;
    resetInEachState = true;
    isInitialised = false;
    isSetup = false;
}

FilterGAM::~FilterGAM() {
    if (input != NULL_PTR(void**)) {
        for (uint32 i = 0u; i < nOfSignals; i++) {
            if (input[i] != NULL_PTR(void*)) {
                input[i] = NULL_PTR(void*);
            }
        }
        delete[] input;
    }
    if (output != NULL_PTR(void**)) {
        for (uint32 i = 0u; i < nOfSignals; i++) {
            if (output[i] != NULL_PTR(void*)) {
                output[i] = NULL_PTR(void*);
            }
        }
        delete[] output;
    }
    if (filterRef != NULL_PTR(Filter*)) {
        if (filterType == SignedInteger32Bit) {
            delete reinterpret_cast<FilterT<int32>*>(filterRef);
        }
        else if (filterType == SignedInteger64Bit) {
            delete reinterpret_cast<FilterT<int64>*>(filterRef);
        }
        else if (filterType == Float32Bit) {
            delete reinterpret_cast<FilterT<float32>*>(filterRef);
        }
        else if (filterType == Float64Bit) {
            delete reinterpret_cast<FilterT<float64>*>(filterRef);
        }
        filterRef = NULL_PTR(Filter*);
    }
    if (filters != NULL_PTR(Filter**)) {
        for (uint32 i = 0; i < nOfSignals; i++) {
            if (filters[i] != NULL_PTR(Filter*)) {
                if (filterType == SignedInteger32Bit) {
                    delete reinterpret_cast<FilterT<int32>*>(filters[i]);
                }
                else if (filterType == SignedInteger64Bit) {
                    delete reinterpret_cast<FilterT<int64>*>(filters[i]);
                }
                else if (filterType == Float32Bit) {
                    delete reinterpret_cast<FilterT<float32>*>(filters[i]);
                }
                else if (filterType == Float64Bit) {
                    delete reinterpret_cast<FilterT<float64>*>(filters[i]);
                }
                filters[i] = NULL_PTR(Filter*);
            }
        }
        delete[] filters;
        filters = NULL_PTR(Filter**);
    }
}

bool FilterGAM::Initialise(StructuredDataI &data) {

    bool ok = GAM::Initialise(data);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GAM::Initialise returned false");
    }
    if (ok) { //Get Filter type from input signal
        ok = data.MoveRelative("InputSignals");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error moving to InputSignals");
        }
        if (ok) {
            ok = data.MoveToChild(0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error moving to input signal index 0. At least one input signal shall exist");
            }
            if (ok) {
                StreamString typeStr;
                if (data.Read("Type", typeStr)) {
                    ok = typeStr == "int32" || typeStr == "int64" || typeStr == "float32" || typeStr == "float64";
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Unsupported type = %s. Supported types = int32 | int64 | float32 | float64",
                                     typeStr.Buffer());
                    }
                    if (ok) {
                        filterType = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::Warning, "Type for signal 0 not specified. Using default float32");
                    filterType = TypeDescriptor::GetTypeDescriptorFromTypeName("float32");
                }
                if (ok) {
                    ok = data.MoveToAncestor(1u);
                }
            }
            if (ok) {
                ok = data.MoveToAncestor(1u);
            }
        }
    }
    if (ok) {
        if (filterType == SignedInteger32Bit) {
            filterRef = new FilterT<int32>();
        }
        else if (filterType == SignedInteger64Bit) {
            filterRef = new FilterT<int64>;
        }
        else if (filterType == Float32Bit) {
            filterRef = new FilterT<float32>();
        }
        else if (filterType == Float64Bit) {
            filterRef = new FilterT<float64>();
        }
    }

    if (ok) { //Read filter coefficients
        uint32 auxNOfNumCoeff = 0u;
        void *auxNum = NULL_PTR(void*);
        uint32 auxNOfDenCoeff = 0u;
        void *auxDen = NULL_PTR(void*);
        AnyType functionsArrayNum = data.GetType("Num");
        AnyType functionsArrayDen = data.GetType("Den");
        ok = (functionsArrayNum.GetDataPointer() != NULL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting pointer to the numerator. Is Num defined?");
        }
        if (ok) {
            auxNOfNumCoeff = functionsArrayNum.GetNumberOfElements(0u);
            ok = (auxNOfNumCoeff > 0u);
            if (!ok) {
                //Unlikely to be hit as empty arrays should be detected by the condition above.
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfNumCoeff must be > 0");
            }
        }
        if (ok) {
            ok = (functionsArrayDen.GetDataPointer() != NULL);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting pointer to the denominator. Is Den defined?");
            }
        }
        if (ok) {
            auxNOfDenCoeff = functionsArrayDen.GetNumberOfElements(0u);
            ok = (auxNOfDenCoeff > 0u);
            if (!ok) {
                //Unlikely to be hit as empty arrays should be detected by the condition above.
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfDenCoeff must be > 0");
            }
        }
        if (ok) {
            if (filterType == SignedInteger32Bit) {
                auxNum = new int32[auxNOfNumCoeff];
                auxDen = new int32[auxNOfDenCoeff];
                Vector<int32> numVector(reinterpret_cast<int32*>(auxNum), auxNOfNumCoeff);
                Vector<int32> denVector(reinterpret_cast<int32*>(auxDen), auxNOfDenCoeff);
                ok = (data.Read("Num", numVector));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numerator");
                }
                if (ok) {
                    ok = (data.Read("Den", denVector));
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading denominator");
                    }
                }
            }
            else if (filterType == SignedInteger64Bit) {
                auxNum = new int64[auxNOfNumCoeff];
                auxDen = new int64[auxNOfDenCoeff];
                Vector<int64> numVector(reinterpret_cast<int64*>(auxNum), auxNOfNumCoeff);
                Vector<int64> denVector(reinterpret_cast<int64*>(auxDen), auxNOfDenCoeff);
                ok = (data.Read("Num", numVector));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numerator");
                }
                if (ok) {
                    ok = (data.Read("Den", denVector));
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading denominator");
                    }
                }
            }
            else if (filterType == Float32Bit) {
                auxNum = new float32[auxNOfNumCoeff];
                auxDen = new float32[auxNOfDenCoeff];
                Vector<float32> numVector(reinterpret_cast<float32*>(auxNum), auxNOfNumCoeff);
                Vector<float32> denVector(reinterpret_cast<float32*>(auxDen), auxNOfDenCoeff);
                ok = (data.Read("Num", numVector));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numerator");
                }
                if (ok) {
                    ok = (data.Read("Den", denVector));
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading denominator");
                    }
                }
            }
            else if (filterType == Float64Bit) {
                auxNum = new float64[auxNOfNumCoeff];
                auxDen = new float64[auxNOfDenCoeff];
                Vector<float64> numVector(reinterpret_cast<float64*>(auxNum), auxNOfNumCoeff);
                Vector<float64> denVector(reinterpret_cast<float64*>(auxDen), auxNOfDenCoeff);
                ok = (data.Read("Num", numVector));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numerator");
                }
                if (ok) {
                    ok = (data.Read("Den", denVector));
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading denominator");
                    }
                }
            }
            else {
                ok = false;
            }
        }

        if (ok) {
            ok = filterRef->Initialise(auxNum, auxNOfNumCoeff, auxDen, auxNOfDenCoeff);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "filter Initialise failed");
            }

        }
        if (auxNum != NULL_PTR(void*)) {
            if (filterType == SignedInteger32Bit) {
                delete[] reinterpret_cast<int32*>(auxNum);
            }
            else if (filterType == SignedInteger64Bit) {
                delete[] reinterpret_cast<int64*>(auxNum);
            }
            else if (filterType == Float64Bit) {
                delete[] reinterpret_cast<float64*>(auxNum);
            }
            else if (filterType == Float32Bit) {
                delete[] reinterpret_cast<float32*>(auxNum);
            }
            auxNum = NULL_PTR(void*);
        }
        if (auxDen != NULL_PTR(void*)) {
            if (filterType == SignedInteger32Bit) {
                delete[] reinterpret_cast<int32*>(auxDen);
            }
            else if (filterType == SignedInteger64Bit) {
                delete[] reinterpret_cast<int64*>(auxDen);
            }
            else if (filterType == Float64Bit) {
                delete[] reinterpret_cast<float64*>(auxDen);
            }
            else if (filterType == Float32Bit) {
                delete[] reinterpret_cast<float32*>(auxDen);
            }

            auxDen = NULL_PTR(void*);
        }
    }
    if (ok) {
        uint32 aux;
        if (!data.Read("ResetInEachState", aux)) {
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
                ok = false;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Wrong value for ResetInEachState (expected values 0 or 1)");
            }
        }
    }
    if (ok) {
        isInitialised = true;
    }

    return ok;
}

bool FilterGAM::Setup() {
    nOfSignals = GetNumberOfInputSignals();
    bool ok = nOfSignals > 0u;
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfSignals must be positive");
    }
    if (ok) {
        uint32 nOfOutputSignals;
        nOfOutputSignals = GetNumberOfOutputSignals();
        ok = nOfOutputSignals == nOfSignals;
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfOutputSignals = %u != %u = numberOfInputSignals", nOfOutputSignals, nOfSignals);
        }
    }
//initialise all filters
    if (ok) {
        filters = new Filter*[nOfSignals];
        for (uint32 i = 0u; i < nOfSignals && ok; i++) {
            if (filterType == SignedInteger32Bit) {
                filters[i] = new FilterT<int32>();
                int32 *auxNum = new int32[filterRef->GetNumberOfNumCoeff()];
                int32 *auxDen = new int32[filterRef->GetNumberOfDenCoeff()];
                filterRef->GetNumCoeff(auxNum);
                filterRef->GetDenCoeff(auxDen);
                filters[i]->Initialise(auxNum, filterRef->GetNumberOfNumCoeff(), auxDen, filterRef->GetNumberOfDenCoeff());
                delete[] auxNum;
                delete[] auxDen;
            }
            else if (filterType == SignedInteger64Bit) {
                filters[i] = new FilterT<int64>();
                int64 *auxNum = new int64[filterRef->GetNumberOfNumCoeff()];
                int64 *auxDen = new int64[filterRef->GetNumberOfDenCoeff()];
                filterRef->GetNumCoeff(auxNum);
                filterRef->GetDenCoeff(auxDen);
                filters[i]->Initialise(auxNum, filterRef->GetNumberOfNumCoeff(), auxDen, filterRef->GetNumberOfDenCoeff());
                delete[] auxNum;
                delete[] auxDen;
            }
            else if (filterType == Float32Bit) {
                filters[i] = new FilterT<float32>();
                float32 *auxNum = new float32[filterRef->GetNumberOfNumCoeff()];
                float32 *auxDen = new float32[filterRef->GetNumberOfDenCoeff()];
                filterRef->GetNumCoeff(auxNum);
                filterRef->GetDenCoeff(auxDen);
                filters[i]->Initialise(auxNum, filterRef->GetNumberOfNumCoeff(), auxDen, filterRef->GetNumberOfDenCoeff());
                delete[] auxNum;
                delete[] auxDen;
            }
            else if (filterType == Float64Bit) {
                filters[i] = new FilterT<float64>();
                float64 *auxNum = new float64[filterRef->GetNumberOfNumCoeff()];
                float64 *auxDen = new float64[filterRef->GetNumberOfDenCoeff()];
                filterRef->GetNumCoeff(auxNum);
                filterRef->GetDenCoeff(auxDen);
                filters[i]->Initialise(auxNum, filterRef->GetNumberOfNumCoeff(), auxDen, filterRef->GetNumberOfDenCoeff());
                delete[] auxNum;
                delete[] auxDen;
            }
        }
    }
    uint32 *nOfSamplesInput = NULL_PTR(uint32*);
    uint32 *numberOfElementsInput = NULL_PTR(uint32*);
    uint32 *nOfSamplesOutput = NULL_PTR(uint32*);
    uint32 *numberOfElementsOutput = NULL_PTR(uint32*);
    if (ok) {
        nOfSamplesInput = new uint32[nOfSignals];
        numberOfElementsInput = new uint32[nOfSignals];
        nOfSamplesOutput = new uint32[nOfSignals];
        numberOfElementsOutput = new uint32[nOfSignals];
        //lint -e{9113} -e{9131} allow multiple != in the same line since it has no effect and makes the code easier to read.
        ok = (nOfSamplesInput != NULL_PTR(uint32*) && numberOfElementsInput != NULL_PTR(uint32*) && nOfSamplesOutput != NULL_PTR(uint32*)
                && numberOfElementsOutput != NULL_PTR(uint32*));
        if (ok) {
            uint32 numberOfElementsToSet = static_cast<uint32>(nOfSignals * sizeof(uint32));
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void*>(nOfSamplesInput), '\0', numberOfElementsToSet);
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void*>(numberOfElementsInput), '\0', numberOfElementsToSet);
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void*>(nOfSamplesOutput), '\0', numberOfElementsToSet);
            (void) MemoryOperationsHelper::Set(reinterpret_cast<void*>(numberOfElementsOutput), '\0', numberOfElementsToSet);
        }
    }
//Read input-output samples and elements for each signal.
    for (uint32 i = 0u; i < nOfSignals && ok; i++) {
        uint32 auxIndex = i;
        ok = GetSignalNumberOfSamples(InputSignals, i, nOfSamplesInput[i]);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfSamples for the input signal %u failed", auxIndex);
        }
        if (ok) {
            ok = GetSignalNumberOfSamples(OutputSignals, i, nOfSamplesOutput[i]);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfSamples for the output signal %u failed ", auxIndex);
            }
        }
        if (ok) {
            ok = GetSignalNumberOfElements(InputSignals, i, numberOfElementsInput[i]);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfElements for the input signal %u failed ", auxIndex);
            }
        }
        if (ok) {
            ok = GetSignalNumberOfElements(OutputSignals, i, numberOfElementsOutput[i]);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfElements for the output signal %u failed ", auxIndex);
            }
        }
        if (ok) {
            ok = (nOfSamplesOutput[i] == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "nOfSamplesOutput must be 1 ");
            }
        }
        if (ok) {
            TypeDescriptor auxType = GetSignalType(InputSignals, i);
            ok = ((auxType == Float32Bit) || (auxType == Float64Bit) || (auxType == SignedInteger32Bit) || (auxType == SignedInteger64Bit));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "GetSignalType for the input signal %u is %s (supported types are int32 | int64 | float32 | float64)", auxIndex,
                             TypeDescriptor::GetTypeNameFromTypeDescriptor(auxType));
            }
        }
        if (ok) {
            TypeDescriptor auxType = GetSignalType(OutputSignals, i);
            ok = ((auxType == Float32Bit) || (auxType == Float64Bit) || (auxType == SignedInteger32Bit) || (auxType == SignedInteger64Bit));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError,
                             "GetSignalType for the output signal %u is %s (supported types are int32 | int64 | float32 | float64)",
                             TypeDescriptor::GetTypeNameFromTypeDescriptor(auxType));
            }
        }
    }
//Check that all the input signals have the same numberOfElementsInput
    if (ok) {
        //if due to MISRA rules
        if (numberOfElementsInput != NULL_PTR(uint32*)) {
            for (uint32 i = 1u; (i < nOfSignals) && ok; i++) {
                ok = (numberOfElementsInput[0] == numberOfElementsInput[i]);
            }
        }
        if ((!ok)) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfElementsInput is not the same for each input signal ");
        }
    }

//Check that all the output signals have the same numberOfElementsOutput
    if (ok) {
        //if due to MISRA rules
        if (numberOfElementsOutput != NULL_PTR(uint32*)) {
            for (uint32 i = 1u; (i < nOfSignals) && ok; i++) {
                ok = (numberOfElementsOutput[0] == numberOfElementsOutput[i]);
            }
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfElementsOutput is not the same for each signal ");
        }
    }

//Check that all the input signals have the same nOfSamplesInput
    if (ok) {
        //if due to MISRA rules
        if (nOfSamplesInput != NULL_PTR(uint32*)) {
            for (uint32 i = 1u; (i < nOfSignals) && ok; i++) {
                ok = (nOfSamplesInput[0] == nOfSamplesInput[i]);
            }
        }
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "nOfSamplesInput is not the same for each signal ");
        }
    }

//Only the nOfSamplesInput[0] have to be checked because, previously, it is check that the input dimensions etc are the same for each input signal.
    if (ok) {
        //if due to MISRA rules
        if ((nOfSamplesInput != NULL_PTR(uint32*)) && (numberOfElementsInput != NULL_PTR(uint32*))) {
            if (nOfSamplesInput[0] == 1u) {
                nOfSamples = numberOfElementsInput[0];
            }
            else {
                ok = (numberOfElementsInput[0] == 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "nOfSamplesInput = %u and numberOfElementsInput = %u (should be 1) ", nOfSamplesInput[0],
                                 numberOfElementsInput[0]);
                }
                nOfSamples = nOfSamplesInput[0];
            }
        }
    }
//Input and output size must be the same
//Check that all the signals have the same number of input & outputs
    if (ok) {
        if (numberOfElementsOutput != NULL_PTR(uint32*)) {
            for (uint32 i = 0u; (i < nOfSignals) && ok; i++) {
                ok = (nOfSamples == numberOfElementsOutput[i]);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Different input and output array size.");
                }
            }
        }
    }
    uint32 inputDimension;
    if (ok) {
        for (uint32 i = 0u; (i < nOfSignals) && ok; i++) {
            ok = GetSignalNumberOfDimensions(InputSignals, i, inputDimension);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfDimensions for InputSignals fails");
            }
            ok = (inputDimension == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "InputDimension = %u. It must be 1 (array)", inputDimension);
            }
        }
    }
    uint32 outputDimension;
    if (ok) {
        for (uint32 i = 0u; (i < nOfSignals) && ok; i++) {
            ok = GetSignalNumberOfDimensions(OutputSignals, 0u, outputDimension);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "GetSignalNumberOfDimensions for OutputSignals fails");
            }
            if (ok) {
                ok = (outputDimension == 1u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "OutputDimension = %u. It must be 1 (array)", outputDimension);
                }
            }
        }
    }
    if (ok) {
        ok = (nOfSamples > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "%nOfSamples is 0");
        }
    }
    if (ok) {
        input = new void*[nOfSignals];
        output = new void*[nOfSignals];
        if ((input != NULL_PTR(void**)) && (output != NULL_PTR(void**))) {
            for (uint32 i = 0u; i < nOfSignals; i++) {
                input[i] = GetInputSignalMemory(i);
                output[i] = GetOutputSignalMemory(i);
            }
        }
    }
//Free pointers MISRA rules
    if (nOfSamplesInput != NULL_PTR(uint32*)) {
        delete[] nOfSamplesInput;
    }
    if (numberOfElementsInput != NULL_PTR(uint32*)) {
        delete[] numberOfElementsInput;
    }
    if (nOfSamplesOutput != NULL_PTR(uint32*)) {
        delete[] nOfSamplesOutput;
    }
    if (numberOfElementsOutput != NULL_PTR(uint32*)) {
        delete[] numberOfElementsOutput;
    }
    if (ok) {
        isSetup = true;
    }
    return ok;
}

bool FilterGAM::Execute() {
    for (uint32 sIdx = 0; sIdx < nOfSignals; sIdx++) {
        //for (uint32 elIdx = 0; elIdx < nOfSamples; elIdx++) {
        filters[sIdx]->Process(input[sIdx], output[sIdx], nOfSamples);
        //}
    }
//    uint32 n = 0u;
//    float32 accumulator;
//

    return true;
}

bool FilterGAM::GetResetInEachState() const {
    return resetInEachState;
}
bool FilterGAM::PrepareNextState(const char8 *const currentStateName,
                                 const char8 *const nextStateName) {
    bool ret = isInitialised && isSetup;
    if (resetInEachState) {
        for (uint32 sIdx = 0u; (sIdx < nOfSignals) && ret; sIdx++) {
            ret = filters[sIdx]->Reset();
//            if (lastInputs[i] != NULL_PTR(float32*)) {
//                for (uint32 n = 0u; n < (numberOfNumCoeff - 1u); n++) {
//                    lastInputs[i][n] = 0.0F;
//                }
//            }
//            if (lastOutputs[i] != NULL_PTR(float32*)) {
//                for (uint32 n = 0u; n < (numberOfDenCoeff - 1u); n++) {
//                    lastOutputs[i][n] = 0.0F;
//                }
//            }
        }
    }
    else {
        //If the currentStateName and lastStateExecuted are different-> rest values
        if (lastStateExecuted != currentStateName) {
            for (uint32 sIdx = 0u; (sIdx < nOfSignals) && ret; sIdx++) {
                ret = filters[sIdx]->Reset();
//                if (lastInputs[i] != NULL_PTR(float32*)) {
//                    for (uint32 n = 0u; n < (numberOfNumCoeff - 1u); n++) {
//                        lastInputs[i][n] = 0.0F;
//                    }
//                }
//                if (lastOutputs[i] != NULL_PTR(float32*)) {
//                    for (uint32 n = 0u; n < (numberOfDenCoeff - 1u); n++) {
//                        lastOutputs[i][n] = 0.0F;
//                    }
//                }
            }

        }
        lastStateExecuted = nextStateName;
    }
    return ret;
}

uint32 FilterGAM::GetNumberOfNumCoeff() const {
    uint32 ret = 0u;
    if (isInitialised) {
        ret = filterRef->GetNumberOfNumCoeff();
    }
    return ret;
}

uint32 FilterGAM::GetNumberOfDenCoeff() const {
    uint32 ret = 0u;
    if (isInitialised) {
        ret = filterRef->GetNumberOfDenCoeff();
    }
    return ret;
}

bool FilterGAM::GetNumCoeff(void *const coeff) const {
    bool ok;
    if (isInitialised) {
        ok = filterRef->GetNumCoeff(coeff);
    }
    else {
        ok = false;
    }
    return ok;
}

bool FilterGAM::GetDenCoeff(void *const coeff) const {
    bool ok;
    if (isInitialised) {
        ok = filterRef->GetDenCoeff(coeff);
    }
    else {
        ok = false;
    }
    return ok;
}

bool FilterGAM::CheckNormalisation() const {
    bool ok;
    if (isInitialised) {
        ok = filterRef->CheckNormalisation();
    }
    else {
        ok = false;
    }
    return ok;
}

void FilterGAM::GetStaticGain(bool &isInfinite,
                              float32 &gain) {
    if (isInitialised) {
        isInfinite = gainInfinite;
        gain = filterRef->GetStaticGainFloat32(isInfinite);
    }
    else {
        gain = 0.0;
        isInfinite = false;
    }
    return;
}

void FilterGAM::GetStaticGain(bool &isInfinite,
                              float64 &gain) {
    if (isInitialised) {
        isInfinite = gainInfinite;
        gain = filterRef->GetStaticGainFloat64(isInfinite);
    }
    else {
        gain = 0.0;
        isInfinite = false;
    }
    return;
}
void FilterGAM::GetStaticGain(bool &isInfinite,
                              int32 &gain) {
    if (isInitialised) {
        isInfinite = gainInfinite;
        gain = filterRef->GetStaticGainInt32(isInfinite);
    }
    else {
        gain = 0.0;
        isInfinite = false;
    }
    return;
}
void FilterGAM::GetStaticGain(bool &isInfinite,
                              int64 &gain) {
    if (isInitialised) {
        isInfinite = gainInfinite;
        gain = filterRef->GetStaticGainInt64(isInfinite);
    }
    else {
        gain = 0.0;
        isInfinite = false;
    }
    return;
}

uint32 FilterGAM::GetNumberOfSignals() const {
    return nOfSignals;
}
uint32 FilterGAM::GetNumberOfSamples() const {
    return nOfSamples;
}
CLASS_REGISTER(FilterGAM, "1.0")
}

