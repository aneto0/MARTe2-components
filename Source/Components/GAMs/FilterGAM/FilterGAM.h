/**
 * @file FilterGAM.h
 * @brief Header file for class FilterGAM
 * @date Jan 30, 2017 TODO Verify the value and format of the date
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

#ifndef SOURCE_COMPONENTS_GAMS_FILTERGAM_FILTERGAM_H_
#define SOURCE_COMPONENTS_GAMS_FILTERGAM_FILTERGAM_H_

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
 * @brief GAM which allows to implement FIR & IIR filter with floats
 * @details The GAM loads its on configuration from a configuration file. It must have
 * the Num and Den defined and normalized. If FIR filter is implemented the Den = 1;
 *
 * The filter returns the solution to the following linear,
 * time-invariant difference equation:
 * y[n] = SUM(k=0 to M-1)num[k]*x[n-k]-SUM(k=1 to N-1)den[k]*y[n-k]
 * where N is the number of denominator coefficients, M is the number of numerator
 * coefficients, X is the input vector of the filter and Y is the output vector of the
 * filter the Z transform of the filter is:
 * H(z)=SUM(k=0 to M-1) num[k]*z^(-k)/(1+SUM(k=1 to N-1) den[k]*z^(-k))
 *
 * Currently the type filter supported is float32.
 *
 * The class hold the coefficients and the last state for the next iteration.
 *
 * @pre The filter must be normalized (den[0] = 1)
 *  the dimension of the numerator and the denominator must be at least 1;
 * @post The output is the input filtered.
 */
class FilterGAM: public GAM {
public:
    /**
     * @brief Constructor
     * @post
     *   GetNumberOfNumCoeff()   = 0
     *   GetNumberOfDenCoeff()   = 0
     *   GetNumCoeff(float32 *n) = false
     *   GetDenCoeff(float32 *d) = false
     *   CheckNormalisation()    = false
     *   GetStaticGain           = 0
     */
    FilterGAM();

    /**
     * @brief free the allocated memory
     * @details if num, den, lastInputs and lastOutputs is not NULL
     * then delete memory.
     */
    virtual ~FilterGAM();

    /**
     * @brief initialise the GAM from a configuration file
     * @details allocate memory for the numerator and denominator coefficients and load their values.
     * Allocate memory for the last inputs and outputs (final state)
     * Check that the coefficients are normalized
     * @pre
     *   GetNumberOfNumCoeff()   = 0
     *   GetNumberOfDenCoeff()   = 0
     *   GetNumCoeff(float32 *n) = false
     *   GetDenCoeff(float32 *d) = false
     *   CheckNormalisation()    = false
     *   GetStaticGain()         = 0
     * @post if Initialise() = true; then
     *   GetNumberOfNumCoeff()   = numberOfNumCoeff
     *   GetNumberOfDenCoeff()   = numberOfDenCoeff
     *   GetNumCoeff(float32 *n) = true
     *   GetDenCoeff(float32 *d) = true
     *   CheckNormalisation()    = true
     *   GetStaticGain()         = staticGain
     * @param[in] data configuration file
     *
     * @return true on succeed
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Setup the input and the output of the GAM and verify the correctness and consistency of the parameters
     * @details Checks that the input and output pointers are correctly obtained and the number of input and
     * output parameters are equal and non zero.
     */
    virtual bool Setup();

    /**
     * @brief Perform the filtering
     * @details Computes the following operations:
     * y[n] = SUM(k=0 to M-1)num[k]*x[n-k]-SUM(k=1 to N-1)den[k]*y[n-k]
     * where N is the number of denominator coefficients, M is the number of numerator coefficients, X is the input vector
     * of the filter and Y is the output vector of the filter
     * @pre
     *   Initialise(StructuredDataI & data) = true
     * @return true
     */
    virtual bool Execute();

    /**
     * @brief makes available the numberOfNumCoeff
     * @return numberOfNumCoeff
     */
    uint32 GetNumberOfNumCoeff();

    /**
     * @brief makes available the numberOfDenCoeff
     * @return numberOfDenCoeff
     */
    uint32 GetNumberOfDenCoeff();

    /**
     * @brief makes available the numerator coefficients
     * @param[in] coeff pointer where the coefficients will be copied
     * @return true of num is not a NULL pointer
     */
    bool GetNumCoeff(float *coeff);

    /**
     * @brief makes available the denominator coefficients
     * @param[in] coeff pointer where the coefficients will be copied
     * @return true of den is not a NULL pointer
     */
    bool GetDenCoeff(float *coeff);

    /**
     * @brief Checks that the coefficients are normalized
     * @details checks that den[0] = 1
     * @return true if the coefficients are normalized
     */
    bool CheckNormalisation();

    /**
     * @brief Makes the staticGain available
     * @details staticGain= SUM(num)/SUM(den)
     * @return the staticGain
     */
    float32 GetStaticGain();

private:
    /* pointer to the numerator coefficients */
    float32 *num;
    /* pointer to the denominator coefficients */
    float32 *den;
    /* Holds the number of numerator coefficients */
    uint32 numberOfNumCoeff;
    /* Holds the number of denominator coefficients */
    uint32 numberOfDenCoeff;
    /* Pointer to the last input values */
    float32 *lastInputs;
    /*Pointer to the last outpt values */
    float32 *lastOutputs;
    /*Holds the the static gain of the filter computed from its coefficients */
    float32 staticGain;

    float32 *input;
    float32 *output;
    uint32 numberOfSamples;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_GAMS_FILTERGAM_FILTERGAM_H_ */

