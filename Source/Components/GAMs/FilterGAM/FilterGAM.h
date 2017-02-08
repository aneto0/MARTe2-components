/**
 * @file FilterGAM.h
 * @brief Header file for class FilterGAM
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

 * @details This header file contains the declaration of the class FilterGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILTERGAM_H_
#define FILTERGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief GAM which allows to implement FIR & IIR filter with float32 type.
 * @details The GAM loads its own configuration from a configuration file. the coefficients of the filter must have
 * the numerator (num) and denominator (den) defined and normalized. If a FIR filter is implemented the den = 1;
 *
 * The filter returns the solution to the following linear,
 * time-invariant difference equation:
 *
 * \f$
 * y[n] = \sum_{k=0}^{M-1}num[k]*x[n-k]-\sum_{k=1}^{N-1}den[k]*y[n-k]
 * \f$
 *
 * where N is the number of denominator coefficients, M is the number of numerator
 * coefficients, X is the input vector of the filter and Y is the output vector of the
 * filter
 *
 * The Z transform of the filter is:
 *
 * \f$
 * H(z)=\frac{\sum_{k=0}^{M-1} num[k]*z^{(-k)}}{1+\sum_{k=1}^{N-1} den[k]*z^{-k}}
 * \f$
 *
 * The GAM supports a multiple input signals (and output signal) only if the characteristics of the input arrays are the same
 * (i.e. The input signals have the same number of elements, the same number of samples and the same type).Currently, the type supported is float32.
 *
 * The inputs and outputs must be arrays (could be arrays of 1 elements).
 *
 * The class holds the coefficients and the last states (if more than one input signal is configured) for the next iteration.
 *
 * @pre The filter must be normalized (den[0] = 1).
 *
 * The size of the numerator and the denominator must be at least 1;
 * @post The output is the input filtered.
 */
class FilterGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     *
     * @post GetNumberOfNumCoeff() = 0
     * @post GetNumberOfDenCoeff()   = 0
     * @post GetNumCoeff(float *coeff) = false
     * @post GetDenCoeff(float *coeff) = false
     * @post CheckNormalisation()    = false
     * @post GetStaticGain()         = 0
     * @post GetNumberOfSamples()    = 0
     * @post GetNumberOfSignals()    = 0
     */
    FilterGAM();

    /**
     * @brief Free the allocated memory
     * @details If num, den, lastInputs and/or lastOutputs is not NULL
     * then free memory.
     */
    virtual ~FilterGAM();

    /**
     * @brief initialise the GAM from a configuration file
     * @details allocate memory for the numerator and denominator coefficients and load their values.
     * Allocate memory for the last input and output values (final state)
     * Check that the coefficients are normalized.
     *
     * @pre GetNumberOfNumCoeff() = 0
     * @pre GetNumberOfDenCoeff()   = 0
     * @pre GetNumCoeff(float *coeff) = false
     * @pre GetDenCoeff(float *coeff) = false
     * @pre CheckNormalisation()    = false
     * @pre GetStaticGain()         = 0
     * @pre GetNumberOfSamples()    = 0
     * @pre GetNumberOfSignals()    = 0
     *
     * @post GetNumberOfNumCoeff()   = numberOfNumCoeff
     * @post GetNumberOfDenCoeff()   = numberOfDenCoeff
     * @post GetNumCoeff(float *coeff) = true
     * @post GetDenCoeff(float *coeff) = true
     * @post CheckNormalisation()    = true
     * @post GetStaticGain()         = staticGain
     * @post GetNumberOfSamples()    = 0
     * @post GetNumberOfSignals()    = 0
     *
     * @param[in] data Configuration file
     *
     * @return true on succeed
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Setup the inputs and the outputs of the GAM. Additionally, it verifies the correctness and consistency of the parameters.
     * @details Checks that the input and output pointers are correctly obtained and the number of input and
     * output parameters are equal and non zero.
     *
     * @pre Initialise() = true;
     *
     * @post GetNumberOfSamples() = numberOfSamples
     * @post GetNumberOfSignals() = numberOfSignals
     * @post input != NULL
     * @post output != NULL
     * @post lastInputs != NULL
     * @post lastOutputs != NULL
     * @return true on succeed.
     */
    virtual bool Setup();

    /**
     * @brief Perform the filtering
     * @details Computes the following operations for each signal:
     *
     * \f$
     * y[n] = \sum_{k=0}^{M-1}num[k]*x[n-k]-\sum_{k=1}^{N-1}den[k]*y[n-k]
     * \f$
     *
     * where N is the number of denominator coefficients, M is the number of numerator
     * coefficients, X is the input vector of the filter and Y is the output vector of the
     * filter
     *
     * @pre Initialise(StructuredDataI & data) = true
     * @pre Setup() = true
     * @return true
     */
    virtual bool Execute();

    /**
     * @brief Makes available the numberOfNumCoeff
     * @return numberOfNumCoeff
     */
    uint32 GetNumberOfNumCoeff();

    /**
     * @brief Makes available the numberOfDenCoeff.
     * @return numberOfDenCoeff.
     */
    uint32 GetNumberOfDenCoeff();

    /**
     * @brief Makes available the numerator coefficients.
     * @param[in] coeff pointer to where the coefficients will be copied.
     *
     * @pre Setup() = true.
     *
     * @return true if num != NULL pointer.
     */
    bool GetNumCoeff(float *coeff);

    /**
     * @brief Makes available the denominator coefficients.
     * @param[in] coeff Pointer to where the coefficients will be copied.
     * @pre Setup() = true
     * @return true if den != NULL pointer.
     */
    bool GetDenCoeff(float *coeff);

    /**
     * @brief Checks that the coefficients are normalized.
     * @details Checks that den[0] = 1.
     * @pre Initialise(StructuredDataI & data) = true.
     * @return true if the coefficients are normalized.
     */
    bool CheckNormalisation();

    /**
     * @brief Makes the staticGain available.
     * @details staticGain= SUM(num)/SUM(den).
     * @return the staticGain.
     */
    float32 GetStaticGain();

    /**
     * @brief Query the number of samples for each signal.
     * @return the number of samples.
     */
    uint32 GetNumberOfSamples();

    /**
     * @brief Query the number of signals configured.
     * @pre Setup() = true.
     * @return the number of signals configured.
     */
    uint32 GetNumberOfSignals();

private:
    /* Pointers to the numerator coefficients. */
    float32 *num;
    /* pointer to the denominator coefficients */
    float32 *den;
    /* Holds the number of numerator coefficients */
    uint32 numberOfNumCoeff;
    /* Holds the number of denominator coefficients */
    uint32 numberOfDenCoeff;
    /*Array of pointer to the last input values */
    float32 **lastInputs;
    /*Array of pointers to the last outpt values */
    float32 **lastOutputs;
    /*Holds the the static gain of the filter computed from its coefficients */
    float32 staticGain;
    /*Array of pointers to the input buffers*/
    float32 **input;
    /*Array of pointers to the output buffers*/
    float32 **output;
    /*Number of values of each array. All arrays has the same numberOfSamples*/
    uint32 numberOfSamples;
    /*Number of signals*/
    uint32 numberOfSignals;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FILTERGAM_H_ */

