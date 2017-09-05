/**
 * @file SSMGAM.cpp
 * @brief Source file for class SSMGAM
 * @date 01/09/2017
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
 * the class SSMGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SSMGAM.h"
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

SSMGAM::SSMGAM() {
    stateMatrixPointer = NULL_PTR(float64 **);
    stateMatrixNumberOfRows = 0u;
    stateMatrixNumberOfColumns = 0u;

    stateMatrixNumberOfRows = 0u;
    stateMatrixNumberOfColumns = 0u;
    sizeStateVector = 0u;
    sizeDerivativeStateVector = 0u;

    inputMatrixPointer = NULL_PTR(float64 **);
    inputMatrixNumberOfRows = 0u;
    inputMatrixNumberOfColumns = 0u;

    outputMatrixPointer = NULL_PTR(float64 **);
    outputMatrixNumberOfRows = 0u;
    outputMatrixNumberOfColumns = 0u;

    feedthroughMatrixPointer = NULL_PTR(float64 **);
    feedthroughMatrixNumberOfRows = 0u;
    feedthroughMatrixNumberOfColumns = 0u;

    numberOfInputSignals = 0u;
    numberOfOutputSignals = 0u;
    sizeOutputVector = 0u;

    numberOfInputElements = 0u;
    numberOfOutputElements = 0u;
    numberOfInputDimensions = 0u;
    numberOfOutputDimensions = 0u;
    numberOfInputSamples = 0u;
    numberOfOutputSamples = 0u;

    inputVectorPointer = NULL_PTR(float64 **);
    outputVectorPointer = NULL_PTR(float64 **);
    stateVectorPointer = NULL_PTR(float64 **);
    derivativeStateVectorPointer = NULL_PTR(float64 **);
//TODO Verify if manual additions are needed here
}

SSMGAM::~SSMGAM() {
    if (stateMatrixPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < stateMatrixNumberOfRows; row++) {
            delete stateMatrixPointer[row];
            stateMatrixPointer[row] = NULL_PTR(float64 *);
        }
        delete[] stateMatrixPointer;
        stateMatrixPointer = NULL_PTR(float64 **);
    }
    if (inputMatrixPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < inputMatrixNumberOfRows; row++) {
            delete inputMatrixPointer[row];
            inputMatrixPointer[row] = NULL_PTR(float64 *);
        }
        delete[] inputMatrixPointer;
        inputMatrixPointer = NULL_PTR(float64 **);
    }
    if (outputMatrixPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < outputMatrixNumberOfRows; row++) {
            delete outputMatrixPointer[row];
            outputMatrixPointer[row] = NULL_PTR(float64 *);
        }
        delete[] outputMatrixPointer;
        outputMatrixPointer = NULL_PTR(float64 **);
    }
    if (feedthroughMatrixPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < feedthroughMatrixNumberOfRows; row++) {
            delete feedthroughMatrixPointer[row];
            feedthroughMatrixPointer[row] = NULL_PTR(float64 *);
        }
        delete[] feedthroughMatrixPointer;
        feedthroughMatrixPointer = NULL_PTR(float64 **);
    }
    if (inputVectorPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < numberOfInputSignals; row++) {
            inputVectorPointer[row] = NULL_PTR(float64 *);
        }
        delete[] inputVectorPointer;
        inputVectorPointer = NULL_PTR(float64 **);
    }
    if (outputVectorPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < sizeOutputVector; row++) {
            outputVectorPointer[row] = NULL_PTR(float64 *);
        }
        delete[] outputVectorPointer;
        outputVectorPointer = NULL_PTR(float64 **);
    }
    if (stateVectorPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < sizeStateVector; row++) {
            stateVectorPointer[row] = NULL_PTR(float64 *);
        }
        delete[] stateVectorPointer;
        stateVectorPointer = NULL_PTR(float64 **);
    }
    if (derivativeStateVectorPointer != NULL_PTR(float64 **)) {
        for (uint32 row = 0u; row < sizeDerivativeStateVector; row++) {
            derivativeStateVectorPointer[row] = NULL_PTR(float64 *);
        }
        delete[] derivativeStateVectorPointer;
        derivativeStateVectorPointer = NULL_PTR(float64 **);
    }

}

bool SSMGAM::Initialise(StructuredDataI &data) {
    bool ok = GAM::Initialise(data);

    if (ok) { //load state matrix
        AnyType functionsMatrix;
        functionsMatrix = data.GetType("StateMatrix");
        ok = (functionsMatrix.GetDataPointer() != NULL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting type for StateMatrix");
        }
        if (ok) {
            //0u are columns
            stateMatrixNumberOfColumns = functionsMatrix.GetNumberOfElements(0u);
            ok = (stateMatrixNumberOfColumns > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of state matrix columns must be positive");
            }
        }
        if (ok) {
            //1u are rows...
            stateMatrixNumberOfRows = functionsMatrix.GetNumberOfElements(1u);
            ok = (stateMatrixNumberOfRows == stateMatrixNumberOfColumns);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of rows and columns of the state matrix must be the same. stateMatrixNumberOfRows = %u stateMatrixNumberOfColumns = %u",stateMatrixNumberOfRows, stateMatrixNumberOfColumns);
            }
        }

        if (ok) { // allocate state matrix memory and read matrix coefficients
            stateMatrixPointer = new float64 *[stateMatrixNumberOfRows];
            for (uint32 i = 0u; i < stateMatrixNumberOfRows; i++) {
                stateMatrixPointer[i] = new float64[stateMatrixNumberOfColumns];
            }
            Matrix<float64> matrix(stateMatrixPointer, stateMatrixNumberOfRows, stateMatrixNumberOfColumns);
            ok = (data.Read("StateMatrix", matrix));
            if (!ok) { //since data.GetType("StateMatrix") does not fail data.Read("StateMatrix", matrix) cannot fail.
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading StateMatrix");
            }
        }
        sizeStateVector = stateMatrixNumberOfRows;
        sizeDerivativeStateVector = stateMatrixNumberOfRows;
    }
    if (ok) { //load input matrix
        AnyType functionsMatrix;
        functionsMatrix = data.GetType("InputMatrix");
        ok = (functionsMatrix.GetDataPointer() != NULL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting type for InputMatrix");
        }
        if (ok) {
            //0u are columns
            inputMatrixNumberOfColumns = functionsMatrix.GetNumberOfElements(0u);
            ok = (inputMatrixNumberOfColumns > 0);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of input matrix columns must be positive");
            }
        }
        if (ok) {
            //1u are rows...
            inputMatrixNumberOfRows = functionsMatrix.GetNumberOfElements(1u);
            ok = (inputMatrixNumberOfRows == stateMatrixNumberOfRows);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError,"the number of input matrix rows must be the same than the number of state matrix rows. inputMatrixNumberOfRows = %u stateMatrixNumberOfRows = %u",inputMatrixNumberOfRows, stateMatrixNumberOfRows);

            }
        }

        if (ok) { // allocate input matrix memory and read matrix coefficients
            inputMatrixPointer = new float64 *[inputMatrixNumberOfRows];
            for (uint32 i = 0u; i < inputMatrixNumberOfRows; i++) {
                inputMatrixPointer[i] = new float64[inputMatrixNumberOfColumns];
            }
            Matrix<float64> matrix(inputMatrixPointer, inputMatrixNumberOfRows, inputMatrixNumberOfColumns);
            ok = (data.Read("InputMatrix", matrix));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading InputMatrix");
            }
        }

    }

    if (ok) { //load output matrix
        AnyType functionsMatrix;
        functionsMatrix = data.GetType("OutputMatrix");
        ok = (functionsMatrix.GetDataPointer() != NULL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting type for OutputMatrix");
        }
        if (ok) {
            //0u are columns
            outputMatrixNumberOfColumns = functionsMatrix.GetNumberOfElements(0u);
            ok = (outputMatrixNumberOfColumns == stateMatrixNumberOfColumns);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of output matrix columns must be equal than the number of state matrix columns");
            }
        }
        if (ok) {
            //1u are rows...
            outputMatrixNumberOfRows = functionsMatrix.GetNumberOfElements(1u);
            ok = (outputMatrixNumberOfRows > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "the number of output matrix rows must be positive");
            }
        }

        if (ok) { // allocate output matrix memory and read matrix coefficients
            outputMatrixPointer = new float64 *[outputMatrixNumberOfRows];
            for (uint32 i = 0u; i < outputMatrixNumberOfRows; i++) {
                outputMatrixPointer[i] = new float64[outputMatrixNumberOfColumns];
            }
            Matrix<float64> outputMatrix1(&outputMatrixPointer[0][0], outputMatrixNumberOfRows, outputMatrixNumberOfColumns);
            ok = (data.Read("OutputMatrix", outputMatrix1));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading outputMatrix");
            }
        }
    }

    if (ok) { //load feedthrough matrix
        AnyType functionsMatrix;
        functionsMatrix = data.GetType("FeedthroughMatrix");
        bool warning = (functionsMatrix.GetDataPointer() == NULL);
        if (warning) {
            REPORT_ERROR(ErrorManagement::Warning, "Error getting type for FeedthroughMatrix. Matrix not defined");
        }
        else {
            if (ok) {
                //0u are columns
                feedthroughMatrixNumberOfColumns = functionsMatrix.GetNumberOfElements(0u);
                ok = (feedthroughMatrixNumberOfColumns == inputMatrixNumberOfColumns);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The number of feedthrough matrix columns must be equal than the number of input matrix columns");
                }
            }
            if (ok) {
                //1u are rows...
                feedthroughMatrixNumberOfRows = functionsMatrix.GetNumberOfElements(1u);
                ok = (feedthroughMatrixNumberOfRows == outputMatrixNumberOfRows);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "the number of feedthough matrix rows must be equal than the number of output matrix rows");
                }
            }

            if (ok) { // allocate feedthrough matrix memory and read matrix coefficients
                feedthroughMatrixPointer = new float64 *[feedthroughMatrixNumberOfRows];
                for (uint32 i = 0u; i < feedthroughMatrixNumberOfRows; i++) {
                    feedthroughMatrixPointer[i] = new float64[feedthroughMatrixNumberOfColumns];
                }
                Matrix<float64> matrix(feedthroughMatrixPointer, feedthroughMatrixNumberOfRows, feedthroughMatrixNumberOfColumns);
                ok = (data.Read("FeedthroughMatrix", matrix));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading feedthroughMatrix");
                }
            }
        }

    }
    return ok;
}

bool SSMGAM::Setup() {
    bool ok;
    numberOfInputSignals = GetNumberOfInputSignals();
    ok = (numberOfInputSignals == inputMatrixNumberOfColumns);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfInputSignals must be equal than inputMatrixNumberOfColumns");
    }
    if (ok) {
        numberOfOutputSignals = GetNumberOfOutputSignals();
        //There are three types of outputs: output vector, state vector, and derivative state vector.
        //The number of output signals must be larger than sizeOfStateVector+sizeOfDerivativeStateVector
        ok = (numberOfOutputSignals == (2u * sizeStateVector) + outputMatrixNumberOfRows);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfOutputSignals must be equal than outputMatrixNumberOfRows + 2u * sizeStateVector. numberOfOutputSignals = %u, outputMatrixNumberOfRows = %u, sizeStateVector = %u", numberOfOutputSignals, outputMatrixNumberOfRows, sizeStateVector);
        }
        else {
            sizeOutputVector = (numberOfOutputSignals - sizeStateVector) - sizeDerivativeStateVector;
        }
    }
    if(ok){//input type
        TypeDescriptor auxType;
        for (uint32 i = 0u; (i < numberOfInputSignals) && ok; i++) {
            auxType = GetSignalType(InputSignals, i);
            ok = (auxType == Float64Bit);
            if(!ok){
                const uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Input variable type must be float64 for index %u", auxIdx);
            }
        }
    }

    if(ok){//input type
        TypeDescriptor auxType;
        for (uint32 i = 0u; (i < numberOfOutputSignals) && ok; i++) {
            auxType = GetSignalType(OutputSignals, i);
            ok = (auxType == Float64Bit);
            if(!ok){
                const uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Output variable type must be float64 for index %u", auxIdx);
            }
        }
    }
    if (ok) { //input elements
        uint32 auxElements;
        for (uint32 i = 0u; (i < numberOfInputSignals) && ok; i++) {
            ok = GetSignalNumberOfElements(InputSignals, i, auxElements);
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error reading the number of input elements for signal %u", auxIdx);
            }
            if (ok) {
                ok = (auxElements == 1u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of elements for the input %u must be 1", auxIdx);
                }
            }
        }
        if (ok) {
            numberOfInputElements = auxElements;
        }
    }

    if (ok) { //output elements
        uint32 auxElements;
        for (uint32 i = 0u; (i < numberOfOutputSignals) && ok; i++) {
            ok = GetSignalNumberOfElements(OutputSignals, i, auxElements);
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error reading the number of output elements for signal %u", auxIdx);
            }
            if (ok) {
                ok = (auxElements == 1u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of elements for the output %u must be 1", auxIdx);
                }
            }
        }
        if (ok) {
            numberOfOutputElements = auxElements;
        }
    }
    if (ok) { //input dimension
        uint32 auxDimension;
        for (uint32 i = 0u; (i < numberOfInputSignals) && ok; i++) {
            ok = GetSignalNumberOfDimensions(InputSignals, i, auxDimension);
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error reading the number of input dimensions for signal %u", auxIdx);
            }
            if (ok) {
                ok = (auxDimension < 2u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of dimension for the input %u must be 0 or 1", auxIdx);
                }
            }
        }
        if (ok) {
            numberOfInputDimensions = auxDimension;
        }
    }
    if (ok) { //output dimension
        uint32 auxDimension;
        for (uint32 i = 0u; (i < numberOfOutputSignals) && ok; i++) {
            ok = GetSignalNumberOfDimensions(OutputSignals, i, auxDimension);
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error reading the number of output dimensions for signal %u", auxIdx);
            }
            if (ok) {
                ok = (auxDimension < 2u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of dimension for the output %u must be 0 or 1", auxIdx);
                }
            }
        }
        if (ok) {
            numberOfOutputDimensions = auxDimension;
        }
    }
    if (ok) { //input samples
        uint32 auxSamples;
        for (uint32 i = 0u; (i < numberOfInputSignals) && ok; i++) {
            ok = GetSignalNumberOfSamples(InputSignals, i, auxSamples);
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error reading the number of input samples for signal %u", auxIdx);
            }
            if (ok) {
                ok = (auxSamples == 1u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of samples for the input %u must be 1", auxIdx);
                }
            }
        }
        if (ok) {
            numberOfInputSamples = auxSamples;
        }
    }

    if (ok) { //output samples
        uint32 auxSamples;
        for (uint32 i = 0u; (i < numberOfOutputSignals) && ok; i++) {
            ok = GetSignalNumberOfSamples(OutputSignals, i, auxSamples);
            if (!ok) {
                uint32 auxIdx = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Error reading the number of output samples for signal %u", auxIdx);
            }
            if (ok) {
                ok = (auxSamples == 1u);
                if (!ok) {
                    uint32 auxIdx = i;
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of samples for the output %u must be 1", auxIdx);
                }
            }
        }
        if (ok) {
            numberOfOutputSamples = auxSamples;
        }
    }
    if (ok) {
        inputVectorPointer = new float64 *[numberOfInputSignals];
        for (uint32 i = 0u; i < numberOfInputSignals; i++) {
            inputVectorPointer[i] = static_cast<float64 *>(GetInputSignalMemory(i));
        }
        outputVectorPointer = new float64 *[sizeOutputVector];
        for (uint32 i = 0u; i < sizeOutputVector; i++) {
            outputVectorPointer[i] = static_cast<float64 *>(GetOutputSignalMemory(i));
        }
        stateVectorPointer = new float64 *[sizeStateVector];
        uint32 auxIdx = 0;
        for (uint32 i = sizeOutputVector; i < (sizeOutputVector + sizeStateVector); i++) {
            stateVectorPointer[auxIdx] = static_cast<float64 *>(GetOutputSignalMemory(i));
            auxIdx++;
        }
        derivativeStateVectorPointer = new float64 *[sizeDerivativeStateVector];
        auxIdx = 0;
        for (uint32 i = (sizeOutputVector + sizeStateVector); i < numberOfOutputSignals; i++) {
            derivativeStateVectorPointer[auxIdx] = static_cast<float64 *>(GetOutputSignalMemory(i));
            auxIdx++;
        }
        outputMatrix = Matrix<float64>(outputMatrixPointer, outputMatrixNumberOfRows,outputMatrixNumberOfColumns);
        feedthroughMatrix = Matrix<float64>(feedthroughMatrixPointer, feedthroughMatrixNumberOfRows,feedthroughMatrixNumberOfColumns);
        stateVector = Matrix<float64>(stateVectorPointer, stateMatrixNumberOfRows,1);
        outputVector = Matrix<float64>(outputVectorPointer, outputMatrixNumberOfRows,1);
        inputVector = Matrix<float64>(inputVectorPointer, inputMatrixNumberOfRows,1);
        intermediateOutput1 = Matrix<float64>(outputMatrixNumberOfRows, 1);
        intermediateOutput2 = Matrix<float64>(outputMatrixNumberOfRows, 1);
    }
    return ok;
}

bool SSMGAM::Execute() {
    bool ok = outputMatrix.Product(stateVector, intermediateOutput1);
    if(ok){
        ok = feedthroughMatrix.Product(inputVector, intermediateOutput2);
    }if(ok){
        outputVector = intermediateOutput1+intermediateOutput2;
    }
    return ok;
}
CLASS_REGISTER(SSMGAM, "1.0")
}

