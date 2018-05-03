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
 * \b u[k] is the input vector. Its dimension is [p x 1]\n
 * \b y[k] is the output vector. Its dimension is [q x 1]\n
 * \b x[k] is the state vector. Its dimension is [n x 1]\n
 * \b x[k+1] is the next state vector. Its dimension is [n x 1]\n
 * \b A[k] is the state matrix. Its dimension is [n x n]\n
 * \b B[k] is the input matrix. Its dimension is [n x p]\n
 * \b C[k] is the output matrix. Its dimension is [q x n]\n
 * \b D[k] is the feedtrough matrix. Its dimension is [q x p]\n
 * \b k denotes the iteration k = 0, 1, 2, ..
 *
 * The inputs of the GAM are the input vector. However the outputs of the GAM are the output vector and the
 * state vector. Consequently the number of outputs of the GAM are the sum
 * of the sizes of theses two vectors.\n
 *
 * The number of elements and the number of samples must be one. The dimensions could be 0 or 1.
 *
 * The state matrix, the input matrix and the output matrix must be specified, however the feedthrough matrix
 * may or may not be specified. While configuring, the following considerations must be applied:
 * <ul>
 * <li><b> The state matrix dimension must be at least 1x1</b>. No 0 dimension is allowed since in a system without states
 * the output depends only on the input and the system has no dynamics.
 * </li>
 * <li><b>The input dimension must be 1x1</b>. O dimension is not allowed since a system without output
 * matrix the number of outputs is 0.
 * </li>
 * <li> The order of the outputs are important. First must be defined the output vector and then the state vector.
 * </li>
 * <li>
 * The coefficients of the matrix must be already converted into the discrete domain.
 * </li>
 * <li>
 * The sampling frequency used to convert the coefficients to the discrete time should be specified. It could be used to cross check
 * the cycle time of the GAM with time sampling used to transform the system in the discrete time domain.
 * </li>
 * </ul>
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 *
 * <pre>
 * +SSMGAM1 = {
 *     Class = SSMGAM
 *     StateMatrix = {{0.5 0.5}{1.0 2.0}} //Compulsory
 *     InputMatrix = {{1 1}{0 1}} //Compulsory
 *     OutputMatrix = {{1 0}} //Compulsory
 *     FeedthroughMatrix = {{0 1}} //Optional
 *     ResetInEachState = 0//Compulsory. 1--> reset in each state, 0--> reset if the previous state is different from the next state
 *     SampleFrequency = 0.0001 // Currently optional and not used.
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
 *             Type = float64 //Only supported type.
 *             NumberOfElements = 1
 *             NumberOfDimensions = 1
 *             Samples = 1
 *         }
 *     }
 *     OutputSignals = {
 *         OutputSignal1 = {
 *             DataSource = "OutputVector"
 *             Type = float64
 *         }
 *         OutputSignal2 = {
 *             DataSource = "StateVector1"
 *             Type = float64 //Only supported type.
 *             NumberOfElements = 1
 *             NumberOfDimensions = 1
 *             Samples = 1
 *         }
 *         OutputSignal3 = {
 *             DataSource = "StateVector2"
 *             Type = float64 //Only supported type.
 *             NumberOfElements = 1
 *             NumberOfDimensions = 1
 *             Samples = 1
 *         }
 *     }
 * }
 * </pre>
 */
class SSMGAM: public GAM {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API SSMGAM)
public:
    CLASS_REGISTER_DECLARATION()
    /** @brief Default constructor
     * @post
     * stateMatrixPointer = NULL_PTR(float64 **)\n
     * intermediateState1Pointer = NULL_PTR(float64 **)\n
     * intermediateState2Pointer = NULL_PTR(float64 **)\n
     * stateMatrixNumberOfRows = 0u\n
     * stateMatrixNumberOfColumns = 0u\n
     * stateMatrixNumberOfRows = 0u\n
     * stateMatrixNumberOfColumns = 0u\n
     * sizeStateVector = 0u\n
     * sizeDerivativeStateVector = 0u\n
     * inputMatrixPointer = NULL_PTR(float64 **)\n
     * inputMatrixNumberOfRows = 0u\n
     * inputMatrixNumberOfColumns = 0u\n
     * outputMatrixPointer = NULL_PTR(float64 **)\n
     * outputMatrixNumberOfRows = 0u\n
     * outputMatrixNumberOfColumns = 0u\n
     * feedthroughMatrixPointer = NULL_PTR(float64 **)\n
     * feedthroughMatrixNumberOfRows = 0u\n
     * feedthroughMatrixNumberOfColumns = 0u\n
     * numberOfInputSignalsGAM = 0u\n
     * numberOfOutputSignalsGAM = 0u\n
     * sizeOutputVector = 0u\n
     * numberOfInputElements = 0u\n
     * numberOfOutputElements = 0u\n
     * numberOfInputDimensions = 0u\n
     * numberOfOutputDimensions = 0u\n
     * numberOfInputSamples = 0u\n
     * numberOfOutputSamples = 0u\n
     * inputVectorPointer = NULL_PTR(float64 **)\n
     * outputVectorPointer = NULL_PTR(float64 **)\n
     * intermediateOutput1Pointer = NULL_PTR(float64 **)\n
     * intermediateOutput2Pointer = NULL_PTR(float64 **)\n
     * stateVectorPointer = NULL_PTR(float64 **)\n
     * derivativeStateVectorPointer = NULL_PTR(float64 **)\n
     * sampleFrequency = 0.0\n
     * enableFeedthroughMatrix = false\n
     * resetInEachState = false\n
     */
    SSMGAM();

    /**
     * @brief Destructor
     * @details Frees the memory allocated by the GAM if necessary
     * @post
     * stateMatrixPointer = NULL_PTR(float64 **)\n
     * intermediateState1Pointer = NULL_PTR(float64 **)\n
     * intermediateState2Pointer = NULL_PTR(float64 **)\n
     * inputMatrixPointer = NULL_PTR(float64 **)\n
     * outputMatrixPointer = NULL_PTR(float64 **)\n
     * feedthroughMatrixPointer = NULL_PTR(float64 **)\n
     * inputVectorPointer = NULL_PTR(float64 **)\n
     * outputVectorPointer = NULL_PTR(float64 **)\n
     * intermediateOutput1Pointer = NULL_PTR(float64 **)\n
     * intermediateOutput2Pointer = NULL_PTR(float64 **)\n
     * stateVectorPointer = NULL_PTR(float64 **)\n
     * derivativeStateVectorPointer = NULL_PTR(float64 **)\n
     */
    virtual ~SSMGAM();

    /**
     * @brief Initialise the parameters from a configuration file.
     * @details Initialise the SS matrices, the resetInEachState and cross-check consistencies.
     * @param[in] data is the configuration file previously defined.
     * @return true if the initialisation succeeds.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Initialise the inputs and the output of the GAM.
     * @details Allocate memory for the inputs and outputs, get the input and output pointers
     * and initialise the class Matrix for use their operations.
     * @return true if the dimension matrices are consistent.
     */
    virtual bool Setup();

    /**
     * @brief Performs the SS operations.
     * @details Implements the following equations:\n
     * \f$
     * x[k+1] = Ax[k]+Bu[k]
     * \f$
     * \n
     * \f$
     * y[k] = Cx[k]+Du[k]
     * \f$\n
     *
     * @return true if the operation can be performed.
     */
    virtual bool Execute();

    /**
     * @brief Reset the states if required.
     * @details This functions has two operations modes:
     * <ul>
     * <li> Reset the GAM states every time the state changes.
     * </li>
     * <li> Reset the GAM if it was not executed in the previous state. e.i. if the GAM goes from
     * "A" to "B" and then from "B" to "C" it will not be reset. In the other hand if the GAM goes
     * from "A" to "B" and then from "C" to "D" the GAM will be reset the states.
     * </li>
     * </ul>
     * @param[in] currentStateName indicates the current state.
     * @param[in] nextStateName indicates the next state.
     * @return true if the state vectors are not NULL.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

private:
    /**
     * state matrix pointer. In standard naming convention, it corresponds to A matrix.
     */
    float64 **stateMatrixPointer;

    /**
     * State matrix initialised using stateMatrixPointer.
     */
    Matrix<float64> stateMatrix;

    /**
     * Holds an intermediate result.
     */
    float64 **intermediateState1Pointer;

    /**
     * Holds an intermediate result. It is initialised with the intermediateState1Pointer
     */
    Matrix<float64> intermediateState1;

    /**
     * Holds an intermediate result.
     */
    float64 **intermediateState2Pointer;

    /**
     * Holds an intermediate result. It is initialised with the intermediateState2Pointer
     */
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

    /**
     * Input matrix. It is initialised with inputMatrixPointer
     */
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
     * Output matrix pointer. In standard naming convention, it corresponds to C matrix.
     */
    float64 **outputMatrixPointer;

    /**
     * Output matrix. It is initialised with outputMatrixPointer
     */
    Matrix<float64> outputMatrix;

    /**
     * Number of rows of the output matrix
     */
    uint32 outputMatrixNumberOfRows;

    /**
     * Number of columns of the output matrix.
     */
    uint32 outputMatrixNumberOfColumns;

    /**
     * Feedthrough matrix pointer. In standard naming convention, it corresponds to D matrix.
     */
    float64 **feedthroughMatrixPointer;

    /**
     * Feedthrough matrix. It is initialised with feedthroughMatrixPointer
     */
    Matrix<float64> feedthroughMatrix;

    /**
     * Number of rows of the feedthrough matrix
     */
    uint32 feedthroughMatrixNumberOfRows;

    /**
     * Number of columns of the feedthrough matrix.
     */
    uint32 feedthroughMatrixNumberOfColumns;

    /**
     * Number of input signals
     */
    uint32 numberOfInputSignalsGAM;

    /**
     * Number of output signals
     */
    uint32 numberOfOutputSignalsGAM;

    /**
     * Number of elements of the output vector. numberOfOutputSignals = sizeOutputVector + sizeStateVector + sizeDerivativeStateVector
     */
    uint32 sizeOutputVector;

    /**
     * Number of input elements
     */
    uint32 numberOfInputElements;

    /**
     * Number of output elements
     */
    uint32 numberOfOutputElements;

    /**
     * Number of input dimensions
     */
    uint32 numberOfInputDimensions;

    /**
     * Number of output dimensions
     */
    uint32 numberOfOutputDimensions;

    /**
     * Number of input samples
     */
    uint32 numberOfInputSamples;

    /**
     * Number of output samples
     */
    uint32 numberOfOutputSamples;

    /**
     * This is the input signal of the system (usually called U). It is the excitation of the system.
     */
    float64 **inputVectorPointer;

    /**
     * Input vector. It is initialised with inputVectorPointer
     */
    Matrix<float64> inputVector;

    /**
     * Output of the system (usually this vector is represented by a Y).
     */
    float64 **outputVectorPointer;

    /**
     * Output vector. It is initialised with outputVectorPointer
     */
    Matrix<float64> outputVector;

    /**
     * Holds an intermediate value.
     */
    float64 **intermediateOutput1Pointer;

    /**
     * Holds an intermediate value. It is initialised with intermediateOutput1Pointer
     */
    Matrix<float64> intermediateOutput1;

    /**
     * Holds an intermediate value.
     */
    float64 **intermediateOutput2Pointer;

    /**
     * Holds an intermediate value. It is initialised with intermediateOutput2Pointer
     */
    Matrix<float64> intermediateOutput2;

    /**
     * State vector pointer(usually it is represented by a X). This vector is an output of the GAM.
     */
    float64 **stateVectorPointer;

    /**
     * State vector. It is initialised with stateVectorPointer
     */
    Matrix<float64> stateVector;

    /**
     * It is the derivative of the state vector. This vector is also an ouput of the GAM.
     */
    float64 **derivativeStateVectorPointer;

    /**
     * It is the derivative of the state vector. It is initialised with stateVectorPointer
     */
    Matrix<float64> derivativeStateVector;

    /**
     * sample frequency in which the matrix parameters are given. It will be used for verification
     */
    float64 sampleFrequency;

    /**
     * Indicates if the D matrix was defined. If it was not defined Du(k) is not calculated.
     */
    bool enableFeedthroughMatrix;

    /**
     * Indicates the behaviour of the reset when MARTe changes the state
     */
    bool resetInEachState;

    /**
     * Remember the last executed state.
     */
    StreamString lastStateExecuted;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SSMGAM_H_ */

