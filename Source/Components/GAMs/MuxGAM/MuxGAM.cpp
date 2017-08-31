/**
 * @file MuxGAM.cpp
 * @brief Source file for class MuxGAM
 * @date 28/08/2017
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
 * the class MuxGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

//#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MuxGAM.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MuxGAM::MuxGAM() :
        GAM() {
    selectors = NULL_PTR(uint32 **);
    inputSignals = NULL_PTR(void **);
    selectorIndex = 0u;
    numberOfSelectorElements = 0u;
    numberOfInputs = 0u;
    numberOfInputSignalsG = 0u;
    //typeSignals = NULL_PTR(TypeDescriptor *);
    numberOfElements = 0u;
    elementIndex = 0u;
    numberOfDimensions = 0u;
    numberOfSamples = 0u;
    outputSignals = NULL_PTR(void **);
    numberOfOutputs = 0u;
    maxSelectorValue = 0u;
    numberOfElements = 0u;
    sizeToCopy = 0u;
}

MuxGAM::~MuxGAM() {
    if (selectors != NULL_PTR(uint32 **)) {
        for (uint32 i = 0u; i < numberOfOutputs; i++) {
            selectors[i] = NULL_PTR(uint32 *);
        }
        delete[] selectors;
    }
    if (inputSignals != NULL_PTR(void **)) {
        for (uint32 i = 0u; i < numberOfInputSignalsG; i++) {
            inputSignals[i] = NULL_PTR(void *);
        }
        delete[] inputSignals;
    }
    if (outputSignals != NULL_PTR(void **)) {
        for (uint32 i = 0u; i < numberOfOutputs; i++) {
            outputSignals[i] = NULL_PTR(void *);
        }
        delete[] outputSignals;
    }
}
bool MuxGAM::Initialise(StructuredDataI &data) {
    return GAM::Initialise(data);
}

bool MuxGAM::Setup() {
    bool ok;
    numberOfOutputs = GetNumberOfOutputSignals();
    ok = (numberOfOutputs > 0u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfOutputs must be positive. numberOfOutputs = %d", numberOfOutputs);
    }
    if (ok) { //number of inputs
        numberOfInputs = GetNumberOfInputSignals();
        if (numberOfOutputs == 1u) {
            ok = (numberOfInputs > (2u * numberOfOutputs));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "numberOfInputs must be larger than twice numberOfOutputs. numberOfInputs = %d", numberOfInputs);
            }
        }
        else {
            ok = (numberOfInputs >= (2u * numberOfOutputs));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "numberOfInputs must be larger or equal than twice numberOfOutputs. numberOfInputs = %d", numberOfInputs);
            }
        }

    }
    if(ok){
        numberOfInputSignalsG = numberOfInputs - numberOfOutputs;
    }
    if (ok) { //Input types
        TypeDescriptor auxType;
        for (uint32 i = 0u; (i < numberOfInputs) && ok; i++) {
            auxType = GetSignalType(InputSignals, i);
            if (i < numberOfOutputs) {
                ok = (auxType == UnsignedInteger32Bit);
                if (!ok) {
                    uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "The input %d is a selector signal. The selector signals must be uint32", aux);
                }
                else {
                    typeSelector = auxType;
                }
            }
            else {
                ok = IsValidType(auxType);
                if (!ok) {
                    uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type of index signal %d is not valid", aux);
                }
                if (ok) {
                    ok = (auxType == GetSignalType(InputSignals, numberOfOutputs));
                    if (!ok) {
                        uint32 aux = i;
                        REPORT_ERROR(ErrorManagement::ParametersError, "All input types (except selectors) must be equal. Input %d is different", aux);
                    }
                }
            }
        }
        typeSignals = auxType;
    }
    if (ok) { //output types
        TypeDescriptor auxType;
        for (uint32 i = 0u; (i < numberOfOutputs) && ok; i++) {
            auxType = GetSignalType(OutputSignals, i);
            ok = (auxType == typeSignals);
            if (!ok) {
                uint32 aux = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "All output signals types must be equal than inputs signal types (except selectors). Output %d is different", aux);
            }
        }
    }
    if (ok) { //output elements
        uint32 auxElements = 0u;
        ok = GetSignalNumberOfElements(OutputSignals, 0u, auxElements);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading output number of elements");
        }
        if(ok){
            ok = (auxElements > 0u);
            if(!ok){
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfElements must be positive");
            }else{
                numberOfElements = auxElements;
            }
        }
        for (uint32 i = 1u; (i < numberOfOutputs) && ok; i++) {
            ok = GetSignalNumberOfElements(OutputSignals, i, auxElements);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading output number of elements");
            }
            if (ok) {
                ok = (auxElements == numberOfElements);
                if (!ok) {
                    uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError, "All inputs and outputs must have the same number of elements. nOfOutputElements[%u] = %u != %u", aux, auxElements, numberOfElements);
                }
            }
        }
    }
    if(ok){//compute sizeToCopy
        sizeToCopy = (typeSignals.numberOfBits*numberOfElements)/8u;
    }
    if (ok) { //input elements
        uint32 auxElements = 0u;
        ok = GetSignalNumberOfElements(InputSignals, 0u, auxElements);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading input number of elements");
        }
        if(ok){
            if(auxElements == numberOfElements){
                numberOfSelectorElements = auxElements;
            }else if (auxElements == 1u){
                numberOfSelectorElements = 1u;
            }else{
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfSelectorElements is not 1 neither numberOfElements");
                ok = false;
            }
        }
        for (uint32 i = 1u; (i < numberOfInputs) && ok; i++) {
            ok = GetSignalNumberOfElements(InputSignals, i, auxElements);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading input number of elements");
            }
            if (ok) {
                if(i < numberOfOutputs){//Selectors case
                    ok = (auxElements == numberOfSelectorElements);
                    if(!ok){
                        uint32 aux = i;
                        REPORT_ERROR(ErrorManagement::InitialisationError, "All input selector elements must be equal. Number of elements for index %u is different", aux);
                    }
                }else{//Input signal case
                    ok = (auxElements == numberOfElements);
                    if(!ok){
                        uint32 aux = i;
                        REPORT_ERROR(ErrorManagement::InitialisationError, "All input and output signal elements (except selectors) must be equal. Number of input elements for index %u is diffrent", aux);
                    }
                }
            }
        }
        numberOfElements = auxElements;
    }

    if (ok) { //input dimension
        uint32 auxDimensions = 0u;
        for (uint32 i = 0u; (i < numberOfInputs) && ok; i++) {
            ok = GetSignalNumberOfDimensions(InputSignals, i, auxDimensions);
            if (!ok) {
                uint32 aux = i;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading input number of dimensions for signal %u", aux);
            }
            if (ok) {
                ok = (auxDimensions == 1u);
                if (!ok) {
                    uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Number of input dimensions must be 1. nOfInputDimension[%u] = %u", aux, auxDimensions);
                }
            }
        }
        numberOfDimensions = auxDimensions;
    }
    if (ok) { //output dimension
        uint32 auxDimensions = 0u;
        for (uint32 i = 0u; (i < numberOfOutputs) && ok; i++) {
            ok = GetSignalNumberOfDimensions(OutputSignals, i, auxDimensions);
            if (!ok) {
                uint32 aux = i;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading output number of dimensions for signal %u", aux);
            }
            if (ok) {
                ok = (auxDimensions == 1u);
                if (!ok) {
                    uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Number of output dimensions must be 1. nOfInputDimension[%u] = %u", aux, auxDimensions);
                }
            }
        }
    }

    if (ok) { //input samples
        uint32 auxSamples = 0u;
        for (uint32 i = 0u; (i < numberOfInputs) && ok; i++) {
            ok = GetSignalNumberOfSamples(InputSignals, i, auxSamples);
            if (!ok) {
                uint32 aux = i;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading input number of samples for signal %u", aux);
            }
            if (ok) {
                ok = (auxSamples == 1u);
                if (!ok) {
                    uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Number of input samples must be 1. nOfInputSamples[%u] = %u", aux, auxSamples);
                }
            }
        }
        numberOfSamples = auxSamples;
    }
    if (ok) { //output samples
        uint32 auxSamples = 0u;
        for (uint32 i = 0u; (i < numberOfOutputs) && ok; i++) {
            ok = GetSignalNumberOfSamples(OutputSignals, i, auxSamples);
            if (!ok) {
                uint32 aux = i;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading output number of samples for signal %u", aux);
            }
            if (ok) {
                ok = (auxSamples == 1u);
                if (!ok) {
                    uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Number of output samples must be 1. nOfOutputSamples[%u] = %u", aux, auxSamples);
                }
            }
        }
    }

    if (ok) {
        maxSelectorValue = numberOfInputs - numberOfOutputs;
    }
    if (ok) {
        selectors = new uint32 *[numberOfOutputs];
        for (uint32 i = 0u; i < numberOfOutputs; i++) {
            selectors[i] = static_cast<uint32 *>(GetInputSignalMemory(i));
        }
    }
    if (ok) { //assign input
        inputSignals = new void *[numberOfInputSignalsG];
        for (uint32 i = numberOfOutputs; i < numberOfInputs; i++) {
            uint32 auxIndx = i - numberOfOutputs;
            inputSignals[auxIndx] = GetInputSignalMemory(i);
        }
    }
    if (ok) { //assign outputs
        outputSignals = new void *[numberOfOutputs];
        for (uint32 i = 0u; i < numberOfOutputs; i++) {
            outputSignals[i] = GetOutputSignalMemory(i);
        }
    }
    return ok;
}

//lint -e{613} Possible use of null pointer 'MARTe::MuxGAM::selectors' in left argument to operator '[.
//MuxGAM::Execute() only is called if the Setup() succeeds and the pointers are initialized.
bool MuxGAM::Execute() {
    bool ok = true;
    if(numberOfSelectorElements == 1u){
        for (selectorIndex = 0u; (selectorIndex < numberOfOutputs) && ok; selectorIndex++) { //goes throughout each selector signal
            if(IsValidSelector(selectors[selectorIndex][0])){
                ok = MemoryOperationsHelper::Copy(outputSignals[selectorIndex],inputSignals[selectors[selectorIndex][0]], sizeToCopy);
                if(!ok){
                    REPORT_ERROR(ErrorManagement::FatalError, "MemoryOperationsHelper::Copy failed");
                }
            }else{
                REPORT_ERROR(ErrorManagement::FatalError,"Invalid selector value. selector value must be lower than %u", maxSelectorValue);
                ok = false;
            }
        }
    }else{
        for (selectorIndex = 0u; (selectorIndex < numberOfOutputs) && ok; selectorIndex++) { //goes throughout each selector signal
            for (elementIndex = 0u; (elementIndex < numberOfElements) && ok; elementIndex++) {
                ok = Copy();
            }
        }
    }
    return ok;
}

bool MuxGAM::IsValidType(TypeDescriptor const &typeRef) const {
    bool retVal;
    bool *auxBool = (new bool[10u]);
    auxBool[0] = typeRef == Float32Bit;
    auxBool[1] = typeRef == Float64Bit;
    auxBool[2] = typeRef == SignedInteger8Bit;
    auxBool[3] = typeRef == UnsignedInteger8Bit;
    auxBool[4] = typeRef == SignedInteger16Bit;
    auxBool[5] = typeRef == UnsignedInteger16Bit;
    auxBool[6] = typeRef == SignedInteger32Bit;
    auxBool[7] = typeRef == UnsignedInteger32Bit;
    auxBool[8] = typeRef == SignedInteger64Bit;
    auxBool[9] = typeRef == UnsignedInteger64Bit;
    retVal = ((auxBool[0]) || (auxBool[1]) || (auxBool[2]) || (auxBool[3]) || (auxBool[4]) || (auxBool[5]) || (auxBool[6]) || (auxBool[7]) || (auxBool[8])
            || (auxBool[9]));
    delete[] auxBool;
    return retVal;
}
//lint -e{613} Possible use of null pointer 'MARTe::MuxGAM::selectors' in left argument to operator '[. Copy() only is used in
//MuxGAM::Execute() and this function only is called if the Setup() succeeds and the pointers are initialized
inline bool MuxGAM::Copy() const {
    bool ret;
    uint32 selectorValue = selectors[selectorIndex][elementIndex];
    ret = IsValidSelector(selectorValue);
    if (!ret) {
        REPORT_ERROR(ErrorManagement::FatalError, "Invalid selector value. selector value ( = %u) must be lower than %u", selectorValue, maxSelectorValue);
    }
    if (ret) {
        if (typeSignals == Float32Bit) {
            static_cast<float32 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<float32 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == Float64Bit) {
            static_cast<float64 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<float64 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == SignedInteger8Bit) {
            static_cast<int8 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<int8 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == UnsignedInteger8Bit) {
            static_cast<uint8 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<uint8 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == SignedInteger16Bit) {
            static_cast<int16 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<int16 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == UnsignedInteger16Bit) {
            static_cast<uint16 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<uint16 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == SignedInteger32Bit) {
            static_cast<int32 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<int32 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == UnsignedInteger32Bit) {
            static_cast<uint32 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<uint32 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == SignedInteger64Bit) {
            static_cast<int64 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<int64 *>(inputSignals[selectorValue])[elementIndex];
        }
        else if (typeSignals == UnsignedInteger64Bit) {
            static_cast<uint64 *>(outputSignals[selectorIndex])[elementIndex] = static_cast<uint64 *>(inputSignals[selectorValue])[elementIndex];
        }
        else {
            ret = false;
        }
    }
    return ret;
}

inline bool MuxGAM::IsValidSelector(const uint32 value) const {
    return (value < maxSelectorValue);
}

CLASS_REGISTER(MuxGAM, "1.0")
}

