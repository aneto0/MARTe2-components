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
    stateMatrix = NULL_PTR(float64 **);
    stateMatrixNumberOfRaws = 0u;
    stateMatrixNumberOfColumns = 0u;

    stateMatrixNumberOfRaws = 0u;
    stateMatrixNumberOfColumns = 0u;
    sizeStateVector = 0u;
    sizeDerivativeStateVector = 0u;

    inputMatrix = NULL_PTR(float64 **);
    inputMatrixNumberOfRaws = 0u;
    inputMatrixNumberOfColumns = 0u;

    outputMatrix = NULL_PTR(float64 **);
    outputMatrixNumberOfRaws = 0u;
    outputMatrixNumberOfColumns = 0u;

    feedthroughMatrix = NULL_PTR(float64 **);
    feedthroughMatrixNumberOfRaws = 0u;
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

    inputVector = NULL_PTR(float64 **);
    ouputVector = NULL_PTR(float64 **);
    stateVector = NULL_PTR(float64 **);
    derivativeStateVector = NULL_PTR(float64 **);
//TODO Verify if manual additions are needed here
}

SSMGAM::~SSMGAM() {
    if (stateMatrix != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < stateMatrixNumberOfRaws; raw++) {
            delete stateMatrix[raw];
            stateMatrix[raw] = NULL_PTR(float64 *);
        }
        delete[] stateMatrix;
        stateMatrix = NULL_PTR(float64 **);
    }
    if (inputMatrix != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < inputMatrixNumberOfRaws; raw++) {
            delete inputMatrix[raw];
            inputMatrix[raw] = NULL_PTR(float64 *);
        }
        delete[] inputMatrix;
        inputMatrix = NULL_PTR(float64 **);
    }
    if (outputMatrix != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < outputMatrixNumberOfRaws; raw++) {
            delete outputMatrix[raw];
            outputMatrix[raw] = NULL_PTR(float64 *);
        }
        delete[] outputMatrix;
        outputMatrix = NULL_PTR(float64 **);
    }
    if (feedthroughMatrix != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < feedthroughMatrixNumberOfRaws; raw++) {
            delete feedthroughMatrix[raw];
            feedthroughMatrix[raw] = NULL_PTR(float64 *);
        }
        delete[] feedthroughMatrix;
        feedthroughMatrix = NULL_PTR(float64 **);
    }
    if (inputVector != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < numberOfInputSignals; raw++) {
            inputVector[raw] = NULL_PTR(float64 *);
        }
        delete[] inputVector;
        inputVector = NULL_PTR(float64 **);
    }
    if (ouputVector != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < sizeOutputVector; raw++) {
            ouputVector[raw] = NULL_PTR(float64 *);
        }
        delete[] ouputVector;
        ouputVector = NULL_PTR(float64 **);
    }
    if (stateVector != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < sizeStateVector; raw++) {
            stateVector[raw] = NULL_PTR(float64 *);
        }
        delete[] stateVector;
        stateVector = NULL_PTR(float64 **);
    }
    if (derivativeStateVector != NULL_PTR(float64 **)) {
        for (uint32 raw = 0u; raw < sizeDerivativeStateVector; raw++) {
            derivativeStateVector[raw] = NULL_PTR(float64 *);
        }
        delete[] derivativeStateVector;
        derivativeStateVector = NULL_PTR(float64 **);
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
            //1u are raws...
            stateMatrixNumberOfRaws = functionsMatrix.GetNumberOfElements(1u);
            ok = (stateMatrixNumberOfRaws == stateMatrixNumberOfColumns);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "the number of raws and columns of the state matrix must be the same. stateMatrixNumberOfRaws = %u stateMatrixNumberOfColumns = %u",stateMatrixNumberOfRaws, stateMatrixNumberOfColumns);
            }
        }

        if (ok) { // allocate state matrix memory and read matrix coefficients
            stateMatrix = new float64 *[stateMatrixNumberOfRaws];
            for (uint32 i = 0u; i < stateMatrixNumberOfRaws; i++) {
                stateMatrix[i] = new float64[stateMatrixNumberOfColumns];
            }
            Matrix<float64> matrix(stateMatrix, stateMatrixNumberOfRaws, stateMatrixNumberOfColumns);
            ok = (data.Read("StateMatrix", matrix));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading StateMatrix");
            }
            for (uint32 raw = 0u; raw < stateMatrixNumberOfRaws; raw++) {
                for (uint32 col = 0u; col < stateMatrixNumberOfColumns; col++) {
                    printf("SSMGAM::Initialise:stateMatrix[%u][%u] = %lf\n", raw, col, stateMatrix[raw][col]);
                }
            }
        }
        sizeStateVector = stateMatrixNumberOfRaws;
        sizeDerivativeStateVector = stateMatrixNumberOfRaws;
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
            //1u are raws...
            inputMatrixNumberOfRaws = functionsMatrix.GetNumberOfElements(1u);
            ok = (inputMatrixNumberOfRaws == stateMatrixNumberOfRaws);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError,"the number of input matrix raws must be the same than the number of state matrix raws. inputMatrixNumberOfRaws = %u stateMatrixNumberOfRaws = %u",inputMatrixNumberOfRaws, stateMatrixNumberOfRaws);

            }
        }

        if (ok) { // allocate input matrix memory and read matrix coefficients
            inputMatrix = new float64 *[inputMatrixNumberOfRaws];
            for (uint32 i = 0u; i < inputMatrixNumberOfRaws; i++) {
                inputMatrix[i] = new float64[inputMatrixNumberOfColumns];
            }
            Matrix<float64> matrix(inputMatrix, inputMatrixNumberOfRaws, inputMatrixNumberOfColumns);
            ok = (data.Read("InputMatrix", matrix));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading InputMatrix");
            }
            for (uint32 raw = 0u; raw < inputMatrixNumberOfRaws; raw++) {
                for (uint32 col = 0u; col < inputMatrixNumberOfColumns; col++) {
                    printf("SSMGAM::Initialise:inputMatrix[%u][%u] = %lf\n", raw, col, inputMatrix[raw][col]);
                }
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
            //1u are raws...
            outputMatrixNumberOfRaws = functionsMatrix.GetNumberOfElements(1u);
            ok = (outputMatrixNumberOfRaws > 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "the number of output matrix raws must be positive");
            }
        }

        if (ok) { // allocate output matrix memory and read matrix coefficients
            outputMatrix = new float64 *[outputMatrixNumberOfRaws];
            for (uint32 i = 0u; i < outputMatrixNumberOfRaws; i++) {
                outputMatrix[i] = new float64[outputMatrixNumberOfColumns];
            }
            Matrix<float64> matrix(outputMatrix, outputMatrixNumberOfRaws, outputMatrixNumberOfColumns);
            ok = (data.Read("outputMatrix", matrix));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading outputMatrix");
            }
            for (uint32 raw = 0u; raw < outputMatrixNumberOfRaws; raw++) {
                for (uint32 col = 0u; col < outputMatrixNumberOfColumns; col++) {
                    printf("SSMGAM::Initialise:outputMatrix[%u][%u] = %lf\n", raw, col, outputMatrix[raw][col]);
                }
            }
        }

    }

    if (ok) { //load feedthrough matrix
        AnyType functionsMatrix;
        functionsMatrix = data.GetType("Feedthrough");
        ok = (functionsMatrix.GetDataPointer() != NULL);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error getting type for FeedthroughMatrix");
        }
        if (ok) {
            //0u are columns
            feedthroughMatrixNumberOfColumns = functionsMatrix.GetNumberOfElements(0u);
            ok = (feedthroughMatrixNumberOfColumns == inputMatrixNumberOfColumns);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError,
                             "The number of feedthrough matrix columns must be equal than the number of input matrix columns");
            }
        }
        if (ok) {
            //1u are raws...
            feedthroughMatrixNumberOfRaws = functionsMatrix.GetNumberOfElements(1u);
            ok = (feedthroughMatrixNumberOfRaws == outputMatrixNumberOfRaws);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "the number of feedthough matrix raws must be equal than the number of output matrix raws");
            }
        }

        if (ok) { // allocate feedthrough matrix memory and read matrix coefficients
            feedthroughMatrix = new float64 *[feedthroughMatrixNumberOfRaws];
            for (uint32 i = 0u; i < feedthroughMatrixNumberOfRaws; i++) {
                feedthroughMatrix[i] = new float64[feedthroughMatrixNumberOfColumns];
            }
            Matrix<float64> matrix(feedthroughMatrix, feedthroughMatrixNumberOfRaws, feedthroughMatrixNumberOfColumns);
            ok = (data.Read("feedthroughMatrix", matrix));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading feedthroughMatrix");
            }
            for (uint32 raw = 0u; raw < feedthroughMatrixNumberOfRaws; raw++) {
                for (uint32 col = 0u; col < feedthroughMatrixNumberOfColumns; col++) {
                    printf("SSMGAM::Initialise:feedthroughMatrix[%u][%u] = %lf\n", raw, col, feedthroughMatrix[raw][col]);
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
        ok = (numberOfOutputSignals > 2u * sizeStateVector);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfOutputSignals must be equal than outputMatrixNumberOfRaws");
        }
        else {
            sizeOutputVector = (numberOfOutputSignals - sizeStateVector) - sizeDerivativeStateVector;
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
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of elements for the input %u must be 1", auxIdx);
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
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of elements for the output %u must be 1", auxIdx);
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
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of dimension for the input %u must be 0 or 1", auxIdx);
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
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of samples for the input %u must be 0 or 1", auxIdx);
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
                    REPORT_ERROR(ErrorManagement::ParametersError, "the number of samples for the output %u must be 0 or 1", auxIdx);
                }
            }
        }
        if (ok) {
            numberOfOutputSamples = auxSamples;
        }
    }
    if (ok) {
        inputVector = new float64 * [numberOfInputSignals];
        for (uint32 i = 0u; i < numberOfInputSignals; i++) {
            inputVector[i] = static_cast<float64 *>(GetInputSignalMemory(i));
        }
        ouputVector = new float64 * [sizeOutputVector];
        for (uint32 i = 0u; i < sizeOutputVector; i++) {
            ouputVector[i] = static_cast<float64 *>(GetInputSignalMemory(i));
        }
        stateVector = new float64 * [sizeStateVector];
        uint32 auxIdx = 0;
        for (uint32 i = sizeOutputVector; i < (sizeOutputVector + sizeStateVector); i++) {
            stateVector[auxIdx] = static_cast<float64 *>(GetInputSignalMemory(i));
            auxIdx++;
        }
        derivativeStateVector = new float64 * [sizeDerivativeStateVector];
        auxIdx = 0;
        for (uint32 i = (sizeOutputVector + sizeStateVector); i < numberOfOutputSignals; i++) {
            derivativeStateVector[auxIdx] = static_cast<float64 *>(GetInputSignalMemory(i));
            auxIdx++;
        }
    }
    return ok;
}

bool SSMGAM::Execute() {
    return true;
}
CLASS_REGISTER(SSMGAM, "1.0")
}

