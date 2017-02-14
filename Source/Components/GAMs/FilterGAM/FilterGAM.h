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
 * the numerator (num) and denominator (den) defined and normalized. If a FIR filter is implemented the Den = 1;
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
 * The class holds the coefficients and the last states for the next iteration. The filter can be reset (see PrepareNextState())
 *
 * Moreover the function offers the method StaticGain() in order to make available the real gain of the filter (after converting in floats).
 *
 * @pre The filter must be normalized (den[0] = 1).
 * @pre The size of the numerator and the denominator must be at least 1;
 * @post The output is the input filtered.
 */
class FilterGAM: public GAM, public StatefulI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     *
     * @post GetNumberOfNumCoeff() = 0
     * @post GetNumberOfDenCoeff()   = 0
     * @post GetNumCoeff(float32 *coeff) const = false
     * @post GetDenCoeff(float32 *coeff) const = false
     * @post CheckNormalisation()    = false
     * @post GetStaticGain()         = 0
     * @post GetNumberOfSamples()    = 0
     * @post GetNumberOfSignals()    = 0
     * @post GetResetInEachState()   = true
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
     * @pre GetNumCoeff(float32 *coeff) const = false
     * @pre GetDenCoeff(float32 *coeff) const = false
     * @pre CheckNormalisation()    = false
     * @pre GetStaticGain()         = 0
     * @pre GetNumberOfSamples()    = 0
     * @pre GetNumberOfSignals()    = 0
     *
     * @post GetNumberOfNumCoeff()             = numberOfNumCoeff
     * @post GetNumberOfDenCoeff()             = numberOfDenCoeff
     * @post GetNumCoeff(float32 *coeff) const = true
     * @post GetDenCoeff(float32 *coeff) const = true
     * @post CheckNormalisation()              = true
     * @post GetStaticGain()                   = staticGain
     * @post GetNumberOfSamples()              = 0
     * @post GetNumberOfSignals()              = 0
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
     * @post GetNumberOfSamples() const = numberOfSamples
     * @post GetNumberOfSignals() const = numberOfSignals
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
     * @brief Queries the value of resetInEachState
     * @return resetInEachState
     */
    bool GetResetInEachState () const;

    /**
     * @brief reset the the lastInputs and lastOutputs if necessary.
     * @details The behavior of this function can be configurated in order to reset every time
     * it is called or when in the previous state the Execute() was not called and the filter was not applied.
     * @pre lastInputs != NULL
     * @pre lastOutput != NULL
     *
     * @post lastInputs[m][n] = 0
     * @post lastOutput[m][n] = 0
     * where m denote the signal index and the n one of the last state of the signal m.
     * @return true if preconditions are met.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Makes available the numberOfNumCoeff
     * @return numberOfNumCoeff
     */
    uint32 GetNumberOfNumCoeff() const;

    /**
     * @brief Makes available the numberOfDenCoeff.
     * @return numberOfDenCoeff.
     */
    uint32 GetNumberOfDenCoeff() const;

    /**
     * @brief Makes available the numerator coefficients.
     * @param[in] coeff pointer to where the coefficients will be copied.
     *
     * @pre Setup() = true.
     *
     * @return true if num != NULL pointer.
     */
    bool GetNumCoeff(float32 * const coeff) const;

    /**
     * @brief Makes available the denominator coefficients.
     * @param[in] coeff Pointer to where the coefficients will be copied.
     * @pre Setup() = true
     * @return true if den != NULL pointer.
     */
    bool GetDenCoeff(float32 * const coeff) const;

    /**
     * @brief Checks that the coefficients are normalized.
     * @details Checks that den[0] = 1.
     * @pre Initialise(StructuredDataI & data) = true.
     * @return true if the coefficients are normalized.
     */
    bool CheckNormalisation() const;

    /**
     * @brief Makes the staticGain available.
     * @details staticGain= SUM(num)/SUM(den). If the gain is infinite the static gain is 0 and the variable isInfinite is set true
     * @param[in] isInfinite indicates if the gain is infinite.
     * @return the staticGain.
     */
    float32 GetStaticGain(bool &isInfinite) const;

    /**
     * @brief Query the number of samples for each signal.
     * @return the number of samples.
     */
    uint32 GetNumberOfSamples() const;

    /**
     * @brief Query the number of signals configured.
     * @pre Setup() = true.
     * @return the number of signals configured.
     */
    uint32 GetNumberOfSignals() const;

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
    /*When the gain is infinite it is set to 1 */
    bool gainInfinite;
    /*Indicates in which last state the filter was executed */
    StreamString lastStateExecuted;
    /*allows to choose between reset the filter every time the state changes or only when in the previous state the filter was not executed. */
    bool resetInEachState;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FILTERGAM_H_ */

