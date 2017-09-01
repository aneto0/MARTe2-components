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
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class SSMGAM: public GAM {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API SSMGAM)
public:
    CLASS_REGISTER_DECLARATION()
    SSMGAM();
    virtual ~SSMGAM();
    virtual bool Initialise(StructuredDataI & data);
    virtual bool Setup();
    virtual bool Execute();

private:
    /**
     * state matrix pointer. In standard naming convention, it corresponds to A matrix.
     */
    float64 **stateMatrix;

    /**
     * number of raws of the state matrix.
     */
    uint32 stateMatrixNumberOfRaws;

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
    float64 **inputMatrix;

    /**
     * number of raws of the input matrix.
     */
    uint32 inputMatrixNumberOfRaws;

    /**
     * number of columns of the  input matrix.
     */
    uint32 inputMatrixNumberOfColumns;

    /**
    * output matrix pointer. In standard naming convention, it corresponds to C matrix.
    */
    float64 **outputMatrix;

    /**
     * number of raws of the output matrix
     */
    uint32 outputMatrixNumberOfRaws;

    /**
     * number of columns of the output matrix.
     */
    uint32 outputMatrixNumberOfColumns;

    /**
    * feedthrough matrix pointer. In standard naming convention, it corresponds to D matrix.
    */
    float64 **feedthroughMatrix;

    /**
     * number of raws of the feedthrough matrix
     */
    uint32 feedthroughMatrixNumberOfRaws;

    /**
     * number of columns of the feedthrough matrix.
     */
    uint32 feedthroughMatrixNumberOfColumns;

    /**
     * number of input signals
     */
    uint32 numberOfInputSignals;

    /**
     * number of output signals
     */
    uint32 numberOfOutputSignals;

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
     *
     */
    float64 **inputVector;

    float64 **ouputVector;

    float64 **stateVector;

    float64 **derivativeStateVector;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SSMGAM_H_ */

