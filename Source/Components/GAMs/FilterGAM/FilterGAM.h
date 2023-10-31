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
#include "FilterT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief GAM which allows to implement FIR & IIR filter with int32 | int64 | float32 | float4 type.
 * @details The GAM configured coefficients of the filter must have
 * the numerator (num) and denominator (den) defined and normalised. If a FIR filter is implemented the Den = 1;
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
 * The GAM supports multiple input signals (and output signals) only if the characteristics of the input arrays are the same
 * (i.e. The input signals have the same number of elements, the same number of samples and the same type). The GAM allows (Samples > 1 && NumberOfElements = 1) || (Samples = 1 && NumberOfElements >1),
 * but (Samples && NumberOfelements) > 1 simultaneous.
 *
 * The inputs and outputs must be arrays (could be arrays of 1 elements).
 *
 * The class holds the coefficients and the last states for the next iteration. The filter can be reset (see PrepareNextState())
 *
 * Moreover the function offers the method StaticGain() in order to make available the real gain of the filter (after converting into appropriate type, only relevant for floating point).
 *
 * @pre The filter must be normalised (den[0] = 1).
 * @pre The size of the numerator and the denominator must be at least 1;
 * @post The output is the input filtered.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 *
 * <pre>
 * +Filter1 = {
 *     Class = FilterGAM
 *     Num = {0.5 0.5} //Compulsory. Filter numerator coefficient.
 *     Den = {1} //Compulsory. Filter denominator coefficient.
 *     ResetInEachState //Optional. If true the filter will be reset on each state change. Otherwise it will be reset only if the filter was not used in the previous state.
 *     InputSignals = {
 *         InputSignal1 = { //Filter will be applied to each signal. The number of input and output signals must be the same.
 *             DataSource = "DDB1"
 *             Type = float32 //Only supported type.
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
 * </pre>
 */
class FilterGAM: public GAM, public StatefulI {
public:CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor.
     *
     * @post
     *   GetNumberOfNumCoeff() = 0 &&
     *   GetNumberOfDenCoeff() = 0 &&
     *   GetNumCoeff() = false &&
     *   GetDenCoeff() = false &&
     *   CheckNormalisation() = false &&
     *   GetStaticGain() = 0 &&
     *   GetNumberOfSamples() = 0 &&
     *   GetNumberOfSignals() = 0 &&
     *   GetResetInEachState() = true
     */
    FilterGAM();

    /**
     * @brief Free the allocated memory
     * @details If num, den, lastInputs and/or lastOutputs are not NULL
     * the associated memory is freed.
     */
    virtual ~FilterGAM();

    /**
     * @brief Initialise the GAM from a configuration file.
     * @details Allocates memory for the numerator and denominator coefficients and load their values.
     * Allocates memory for the last input and output values (final state)
     * Checks that the coefficients are normalised.
     * @param[in] data the GAM configuration.
     * @return true on succeed
     * @pre
     *   GetNumberOfNumCoeff() = 0 &&
     *   GetNumberOfDenCoeff() = 0 &&
     *   GetNumCoeff() = false &&
     *   GetDenCoeff() = false &&
     *   CheckNormalisation() = false &&
     *   GetStaticGain() = 0 &&
     *   GetNumberOfSamples() = 0 &&
     *   GetNumberOfSignals() = 0
     *
     * @post
     *   GetNumberOfNumCoeff() = numberOfNumCoeff &&
     *   GetNumberOfDenCoeff() = numberOfDenCoeff &&
     *   GetNumCoeff() = true &&
     *   GetDenCoeff() = true &&
     *   CheckNormalisation() = true &&
     *   GetStaticGain() = staticGain &&
     *   GetNumberOfSamples() = 0 &&
     *   GetNumberOfSignals() = 0
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Setup the inputs and the outputs of the GAM. Additionally, it verifies the correctness and consistency of the parameters.
     * @details Checks that the input and output pointers are correctly obtained and the number of input and
     * output parameters are equal and non zero.
     * @return true if all the parameters are specified as described in the class description.
     * @pre
     *   Initialise()
     *
     * @post
     *   GetNumberOfSamples() = numberOfSamples &&
     *   GetNumberOfSignals() = numberOfSignals &&
     *   input != NULL &&
     *   output != NULL &&
     *   lastInputs != NULL &&
     *   lastOutputs != NULL
     */
    virtual bool Setup();

    /**
     * @brief Perform the filtering.
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
     * @return true
     * @pre
     *   Initialise() &&
     *   Setup()
     */
    virtual bool Execute();

    /**
     * @brief Queries the value of resetInEachState
     * @return resetInEachState
     */
    bool GetResetInEachState() const;

    /**
     * @brief Resets the lastInputs and lastOutputs if necessary.
     * @details The behaviour of this function can be configured in order to reset the filter every time
     * it is called or when in the previous state the Execute() has not be called not called (and thus the filter has not be applied on such state).
     * @return true if preconditions are met.
     * @pre
     *   lastInputs != NULL &&
     *   lastOutput != NULL  &&
     *
     * @post
     *   lastInputs[m][n] = 0 &&
     *   lastOutput[m][n] = 0
     * where m denote the signal index and the n one of the last state of the signal m.
     */
    virtual bool PrepareNextState(const char8 *const currentStateName,
                                  const char8 *const nextStateName);

    /**
     * @brief Gets the number of numerator coefficients.
     * @return the number of numerator coefficients.
     */
    uint32 GetNumberOfNumCoeff() const;

    /**
     * @brief Gets the number of denominator coefficients.
     * @return the number of denominator coefficients.
     */
    uint32 GetNumberOfDenCoeff() const;

    /**
     * @brief Gets the numerator coefficients.
     * @param[in] coeff pointer to where the coefficients will be copied to.
     * @return true if num != NULL pointer.
     * @pre
     *   Setup()
     */
    bool GetNumCoeff(void *const coeff) const;

    /**
     * @brief Gets the denominator coefficients.
     * @param[in] coeff Pointer to where the coefficients will be copied.
     * @return true if den != NULL pointer.
     * @pre
     *   Setup()
     */
    bool GetDenCoeff(void *const coeff) const;

    /**
     * @brief Checks that the coefficients are normalised.
     * @details Checks that den[0] = 1.
     * @return true if the coefficients are normalised.
     * @pre
     *   Initialise()
     */
    bool CheckNormalisation() const;

    /**
     * @brief Gets the filter static gain value.
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
     * @return the number of signals configured.
     * @pre
     *   Setup()
     */
    uint32 GetNumberOfSignals() const;

private:

    Filter *filterRef;

    Filter **filters;

    TypeDescriptor filterType;

    /**
     * Holds the the static gain of the filter computed from its coefficients
     */
    float32 staticGain;

    /**
     * Array of pointers to the input buffers
     */
    void **input;

    /**
     * Array of pointers to the output buffers
     */
    void **output;

    /**
     * Number of values of each array. All arrays have the same numberOfSamples
     */
    uint32 nOfSamples;

    /**
     * Number of signals
     */
    uint32 nOfSignals;

    /**
     * When the gain is infinite this is set to true
     */
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

