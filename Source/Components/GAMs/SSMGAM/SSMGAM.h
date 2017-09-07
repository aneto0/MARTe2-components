/**
 * @file SSMGAM.h
 * @brief Header file for class SSMGAM
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

 * @details This header file contains the declaration of the class SSMGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SSMGAM_H_
#define SSMGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"
#include "Matrix.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAM which implements a generic State Space model with constant matrices and float64.
 * @details The GAM implements the following equations:\n
 * \f$
 * x[k+1] = Ax[k]+Bu[k]
 * \f$
 * \n
 * \f$
 * y[k] = Cx[k]+Du[k]
 * \f$
 * \n
 * Where:\n
 * \b u[k] is the input vector. Its dimension [p x 1]\n
 * \b y[k] is the output vector. Its dimension is [q x 1]\n
 * \b x[k] is the state vector. Its dimension is [n x q]\n
 * \b x[k+1] is the next state vector. Its dimension is [n x q]\n
 * \b A[k] is the state matrix. Its dimension is [n x n]\n
 * \b B[k] is the input matrix. Its dimension is [n x p]\n
 * \b C[k] is the output matrix. Its dimension is [q x n]\n
 * \b D[k] is the feedtrough matrix. Its dimension is [q x p]\n
 * \b k denotes the iteration k = 0, 1, 2, ..
 *
 * The inputs of the GAM are the input vector. However the outputs of the GAM are the output vector, the
 * state vector and the next state vector. Consequently the number of outputs of the GAM are the sum
 * of the sizes of theses vectors.\n
 *
 * The number of elements and the number of samples must be one. The dimensions could be 0 or 1.
 *
 * The state matrix the input matrix and the output matrix must be specified, however the feedthrough matrix
 * may or may not be specified. While configuring, the following considerations must be applied:
 * <ul>
 * <li><b> The state matrix dimension must be at least 1x1</b>. No 0 dimension is allowed since a system without states
 * the output depends only on the input and the system has no dynamics.
 * </li>
 * <li><b>The input dimension must be 1x1</b>. O dimension is not allowed since a system without output
 * matrix the number of output is 0.
 * </li>
 * <li> The order of the outputs are important. First must be defined the output vector, then the state vector and finally the
 * next state.
 * </li>
 * <li>
 * The coefficients of the matrix must be already converted in the discrete domain.
 * </li>
 * <li>
 * The sampling frequency used to convert the coefficients to the discrete time should be specified. It could be used to cross check
 * the cycle time of the GAM with time sampling used to transform the system in the discrete time domain.
 * </li>
 * </ul>
 *
 *
 * *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * +SSMGAM1 = {
 *     Class = SSMGAM
 *     StateMatrix = {{0.5 0.5}{1.0 2.0}} //Compulsory
 *     InputMatrix = {{1}{0}} //Compulsory
 *     OutputMatrix = {{1 0}} //Compulsory
 *     FeedthroughMatrix = {{0 1}} //Optional
 *     ResetInEachState = 0//Compulsory. 1--> reset in each state, 0--> reset if the previous state is different from the next state
 *     sampleFrequency = 0.0001 // Currently optional.
 *     InputSignals = {
 *         InputSignal1 = { //input of the SS
 *             DataSource = "DDB1"
 *             Type = float64 //Only supported type.
 *             NumberOfElements = 1
 *             NumberOfDimensions = 1 // or 0
 *             Samples = 1
 *         }
 *         InputSignal2 = {
 *             DataSource = "DDB1"
 *             Type = float32
 *         }
 *     }
 *     OutputSignals = {
 *         OutputSignal1 = {
 *             DataSource = "LCD"
 *             Type = float32
 *         }
 *         OutputSignal2 = {
 *             DataSource = "LCD"
 *             Type = float32
 *         }
 *     }
 * }
 */
class SSMGAM: public GAM {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API SSMGAM)
public:
    CLASS_REGISTER_DECLARATION()
    SSMGAM();
    virtual ~SSMGAM();
    virtual bool Initialise(StructuredDataI & data);
    virtual bool Setup();
    virtual bool Execute();
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

private:
    /**
     * state matrix pointer. In standard naming convention, it corresponds to A matrix.
     */
    float64 **stateMatrixPointer;

    Matrix<float64> stateMatrix;

    float64 **intermediateState1Pointer;
    Matrix<float64> intermediateState1;

    float64 **intermediateState2Pointer;
    Matrix<float64> intermediateState2;

    /**
     * number of rows of the state matrix.
     */
    uint32 stateMatrixNumberOfRows;

    /**
     * number of columns of the state matrix.
     */
    uint32 stateMatrixNumberOfColumns;

    /**
     * Number of elements of the state vector.
     */
    uint32 sizeStateVector;

    /**
     * Number of elements of the derivative state vector. sizeStateVector = sizeDerivativeStateVector
     */
    uint32 sizeDerivativeStateVector;

    /**
     * input matrix pointer. In standard naming convention, it corresponds to B matrix.
     */
    float64 **inputMatrixPointer;

    Matrix<float64> inputMatrix;

    /**
     * number of rows of the input matrix.
     */
    uint32 inputMatrixNumberOfRows;

    /**
     * number of columns of the  input matrix.
     */
    uint32 inputMatrixNumberOfColumns;

    /**
     * output matrix pointer. In standard naming convention, it corresponds to C matrix.
     */
    float64 **outputMatrixPointer;

    Matrix<float64> outputMatrix;

    /**
     * number of rows of the output matrix
     */
    uint32 outputMatrixNumberOfRows;

    /**
     * number of columns of the output matrix.
     */
    uint32 outputMatrixNumberOfColumns;

    /**
     * feedthrough matrix pointer. In standard naming convention, it corresponds to D matrix.
     */
    float64 **feedthroughMatrixPointer;

    Matrix<float64> feedthroughMatrix;

    /**
     * number of rws of the feedthrough matrix
     */
    uint32 feedthroughMatrixNumberOfRows;

    /**
     * number of columns of the feedthrough matrix.
     */
    uint32 feedthroughMatrixNumberOfColumns;

    /**
     * number of input signals
     */
    uint32 numberOfInputSignalsGAM;

    /**
     * number of output signals
     */
    uint32 numberOfOutputSignalsGAM;

    /**
     * number of elements of the output vector. numberOfOutputSignals = sizeOutputVector + sizeStateVector + sizeDerivativeStateVector
     */
    uint32 sizeOutputVector;

    /**
     * number of input elements
     */
    uint32 numberOfInputElements;

    /**
     * number of output elements
     */
    uint32 numberOfOutputElements;

    /**
     * number of input dimensions
     */
    uint32 numberOfInputDimensions;

    /**
     * number of output dimensions
     */
    uint32 numberOfOutputDimensions;

    /**
     * number of input samples
     */
    uint32 numberOfInputSamples;

    /**
     * number of output samples
     */
    uint32 numberOfOutputSamples;

    /**
     * This is the input signal of the system (usually called U). It is the excitation of the system.
     */
    float64 **inputVectorPointer;

    Matrix<float64> inputVector;

    /**
     * Output of the system (usually this vector is represented by a Y).
     */
    float64 **outputVectorPointer;

    Matrix<float64> outputVector;

    float64 **intermediateOutput1Pointer;
    Matrix<float64> intermediateOutput1;

    float64 **intermediateOutput2Pointer;
    Matrix<float64> intermediateOutput2;

    /**
     * State vector (usually it is represented by a X). This vector is an output of the GAM.
     */
    float64 **stateVectorPointer;

    Matrix<float64> stateVector;

    /**
     * It is the derivative of the state vector. This vector is also an ouput of the GAM.
     */
    float64 **derivativeStateVectorPointer;

    Matrix<float64> derivativeStateVector;

    /**
     * sample frequency in which the matrix parameters are given. It will be used for verification
     */
    float64 sampleFrequency;

    bool enableFeedthroughMatrix;

    bool resetInEachState;

    StreamString lastStateExecuted;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SSMGAM_H_ */

