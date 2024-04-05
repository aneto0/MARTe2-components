/**
 * @file FilterT.h
 * @brief Header file for class FilterT
 * @date 30/10/2023
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

 * @details This header file contains the declaration of the class FilterT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FILTERGAM_FILTERT_H_
#define FILTERGAM_FILTERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "Filter.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {
//lint -esym(9107, MARTe::FilterT*) [MISRA C++ Rule 3-1-1] required for template implementation
/**
 * @brief Filter template implementation to be used in the FilterGAM
 * @details Typical use of this class:
 *     - Initialise()
 *     - Process()
 *
 * The class offers a reset of the last input/outputs as well of queries of the coefficients used, static gain, and a check of the coefficient normalisations.
 *
 * The class is responsible for the allocation/deallocation of the memory for the last input/output values and the coefficients of the filter.
 */
template<class T>
class FilterT: public Filter {
public:
    /**
     * @brief Default constructor
     * @details null all the pointers and set to false all the internal booleans
     */
    FilterT();

    /**
     * @brief default destructor
     * @details frees all the memory allocated by this GAM and null the pointers.
     */
    virtual ~FilterT();

    /**
     * @brief Initialise the filter based on the coefficients of the filter.
     * @details Allocate memory for the coefficients and last input/output states and zero the last values.
     * @param[in] numIn array of the numerator coefficients.
     * @param[in] nOfNumCoeffIn number of numerator elements.
     * @param[in] denIn array of the denominator coefficients
     * @param[in] nOfDenCoeffIn number of denominator elements.
     */
    virtual bool Initialise(const void *const numIn,
                            const uint32 nOfNumCoeffIn,
                            const void *const denIn,
                            const uint32 nOfDenCoeffIn);
    /**
     * @brief Zero the last input/output states.
     */
    virtual bool Reset();

    /**
     * @brief Applies the filter
     * @details given an input array of n elements filter it and save the filtered signal in the output.
     * @param[in] input array holding the input data
     * @param[out] output array holding the filtered data
     * @param[in] nOfElements the number of elements to filter. It is the inpit or output arry size.
     *
     * @pre
     *    Initialise succeed.
     */
    virtual void Process(void *const input,
                         void *const output,
                         const uint32 nOfElements);
    /**
     * @brief return the number of numerator coefficients already configured.
     * @param[out] coeff hold the numerator coefficients of the configured filter.
     */
    virtual bool GetNumCoeff(void *const coeff);

    /**
     * @brief return the number of denominator coefficients already configured.
     * @param[out] coeff hold the denominator coefficients of the configured filter.
     */
    virtual bool GetDenCoeff(void *const coeff);

    /**
     * @brief Query if the filtered configured is normalised (den[0] = 1)
     */
    virtual bool CheckNormalisation();

    /**
     * @brief Query the static gain of the filter
     * @param[out] isInfinite indicates if the gain is infinite (i.e is an integrator).
     * @return the static gain
     */
    virtual float32 GetStaticGainFloat32(bool &isInfinite);

    /**
     * @brief Query the static gain of the filter
     * @param[out] isInfinite indicates if the gain is infinite (i.e is an integrator).
     * @return the static gain
     */
    virtual float64 GetStaticGainFloat64(bool &isInfinite);

    /**
     * @brief Query the static gain of the filter
     * @param[out] isInfinite indicates if the gain is infinite (i.e is an integrator).
     * @return the static gain
     */
    virtual int32 GetStaticGainInt32(bool &isInfinite);

    /**
     * @brief Query the static gain of the filter
     * @param[out] isInfinite indicates if the gain is infinite (i.e is an integrator).
     * @return the static gain
     */
    virtual int64 GetStaticGainInt64(bool &isInfinite);
private:

    /**
     * Pointer to the numerator coefficients.
     */
    T *num;

    /**
     * Pointer to the denominator coefficients
     */
    T *den;

    /**
     * Array of pointer to the last input values
     */
    T *lastInputs;

    /**
     * Array of pointers to the last output values
     */
    T *lastOutputs;

    /**
     * Static gain of the filter
     */
    T staticGain;

    /*
     * Indicates if the gain is inifinite.
     */
    bool gainInfinite;

    /**
     * Indicates if the filter is initialised.
     */
    bool isInitialised;

    /**
     * @brief Compare two values
     * @param[in] a value to compare
     * @param[in] b value to compare.
     * @return true id both are equal.
     */
    bool IsEqualO(const int32 a,
                  const int32 b) const;
    /**
     * @brief Compare two values
     * @param[in] a value to compare
     * @param[in] b value to compare.
     * @return true id both are equal.
     */
    bool IsEqualO(const int64 a,
                  const int64 b) const;

    /**
     * @brief Compare two values
     * @param[in] a value to compare
     * @param[in] b value to compare.
     * @return true id both are equal.
     */
    bool IsEqualO(const float32 a,
                  const float32 b) const;

    /**
     * @brief Compare two values
     * @param[in] a value to compare
     * @param[in] b value to compare.
     * @return true id both are equal.
     */
    bool IsEqualO(const float64 a,
                  const float64 b) const;

};
template<class T>
//lint
FilterT<T>::FilterT() :
        Filter() {
    lastInputs = NULL_PTR(T*);
    lastOutputs = NULL_PTR(T*);
    num = NULL_PTR(T*);
    den = NULL_PTR(T*);
    nOfNumCoeff = 0u;
    nOfDenCoeff = 0u;
    isInitialised = false;
    staticGain = static_cast<T>(0);
    gainInfinite = false;

}
template<class T>
FilterT<T>::~FilterT() {
    if (lastInputs != NULL_PTR(T*)) {
        delete[] lastInputs;
    }
    if (lastOutputs != NULL_PTR(T*)) {
        delete[] lastOutputs;
    }
    if (num != NULL_PTR(T*)) {
        delete[] num;
    }
    if (den != NULL_PTR(T*)) {
        delete[] den;
    }
}

template<class T>
bool FilterT<T>::Initialise(const void *const numIn,
                            const uint32 nOfNumCoeffIn,
                            const void *const denIn,
                            const uint32 nOfDenCoeffIn) {

    bool ok = (numIn != NULL_PTR(T*)) && (denIn != NULL_PTR(T*));
    if (ok) {
        nOfNumCoeff = nOfNumCoeffIn;
        nOfDenCoeff = nOfDenCoeffIn;
        num = new T[nOfNumCoeff];
        den = new T[nOfDenCoeff];
        ok = MemoryOperationsHelper::Copy(reinterpret_cast<void*>(num), numIn, static_cast<uint32>(sizeof(T)) * nOfNumCoeff);
        if (ok) {
            ok = MemoryOperationsHelper::Copy(reinterpret_cast<void*>(den), denIn, static_cast<uint32>(sizeof(T)) * nOfDenCoeff);
        }

    }
    if (ok) {
        //lint -e{613} Possible use of null pointer. Not possible since the correct initialisation is guaranteed by the ok.
        ok = IsEqualO(den[0], static_cast<T>(1));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "The transfer function should be normalised. The den[0] should be 1");
        }
    }
    if (ok) {
        T sumNumerator = static_cast<T>(0);
        for (uint32 i = 0u; i < nOfNumCoeff; i++) {
            //if due to MISRA rules however it is not necessary. At his line the initialization of num is guaranteed b the ok = true...
            if (num != NULL_PTR(T*)) {
                sumNumerator += num[i];
            }
        }
        T sumDenominator = static_cast<T>(0);
        for (uint32 i = 0u; i < nOfDenCoeff; i++) {
            //if due to MISRA rules however it is not necessary. At his line the initialization of den is guaranteed b the ok = true...
            if (den != NULL_PTR(T*)) {
                sumDenominator += den[i];
            }
        }
        if (!IsEqualO(sumDenominator, static_cast<T>(0))) {
            //lint -e{414} sumDenominator cannot be 0.
            staticGain = sumNumerator / sumDenominator;
        }
        else {
            gainInfinite = true;
        }
    }
    if (ok) {
        lastInputs = new T[nOfNumCoeff];
        lastOutputs = new T[nOfDenCoeff];
        ok = (lastInputs != NULL_PTR(T*)) && (lastOutputs != NULL_PTR(T*));
        if (ok) {
            ok = Reset();
        }
    }
    if (ok) {
        isInitialised = ok;
    }
    return ok;
}

template<class T>
bool FilterT<T>::Reset() {
    bool ok = (lastInputs != NULL_PTR(T*)) || (lastOutputs != NULL_PTR(T*));
    if (ok) {
        ok = MARTe::MemoryOperationsHelper::Set(lastInputs, static_cast<MARTe::char8>(0), static_cast<uint32>(sizeof(T)) * nOfNumCoeff);
        if (ok) {
            ok = MARTe::MemoryOperationsHelper::Set(lastOutputs, static_cast<MARTe::char8>(0), static_cast<uint32>(sizeof(T)) * nOfDenCoeff);
        }
    }
    return ok;
}

template<class T>
//lint -e{613} Possible use of null pointer --> Process cannot be called if initialised does not succeed.
void FilterT<T>::Process(void *const input,
                         void *const output,
                         const uint32 nOfElements) {
    //lint -e{613} Possible use of null pointer --> Process cannot be called if initialised does not succeed.
    for (uint32 elIdx = 0u; elIdx < nOfElements; elIdx++) {
        lastInputs[0] = reinterpret_cast<T*>(input)[elIdx];
        lastOutputs[0] = static_cast<T>(0);
        //lint -e{661} Possible access of out-of-bounds pointer (1 beyond end of data) by operator '[' [MISRA C++ Rule 5-0-16]. lastInputs and num has nOfNumCoeff elements, so not out-of-bound is possible.
        for (MARTe::uint32 i = nOfNumCoeff - 1u; i > 0u; i--) {
            lastOutputs[0] += lastInputs[i] * num[i];
            lastInputs[i] = lastInputs[i - 1u];
        }
        lastOutputs[0] += lastInputs[0u] * num[0u];
        //lint -e{661} Possible access of out-of-bounds pointer (1 beyond end of data) by operator '[' [MISRA C++ Rule 5-0-16]. lastOutputs and den has nOfNumCoeff elements, so not out-of-bound is possible.
        for (MARTe::uint32 i = nOfDenCoeff - 1u; i > 0u; i--) {
            lastOutputs[0] -= lastOutputs[i] * den[i];
            lastOutputs[i] = lastOutputs[i - 1u];
        }
        reinterpret_cast<T*>(output)[elIdx] = lastOutputs[0];
    }
}

template<class T>
//lint -e{429} Custodial pointer 'coeff' (line 246) has not been freed or returned. Is not the responsability of the function to free the memory.
bool FilterT<T>::GetNumCoeff(void *const coeff) {
    bool ok;
    if (isInitialised) {
        ok = MemoryOperationsHelper::Copy(coeff, num, static_cast<uint32>(sizeof(T)) * nOfNumCoeff);
    }
    else {
        ok = false;
    }
    return ok;
    //lint -e{429} Custodial pointer 'coeff' (line 246) has not been freed or returned. Is not the responsability of the function to free the memory.
}

template<class T>
//lint -e{429} Custodial pointer 'coeff' (line 246) has not been freed or returned. Is not the responsability of the function to free the memory.
bool FilterT<T>::GetDenCoeff(void *const coeff) {
    bool ok;
    if (isInitialised) {
        ok = MemoryOperationsHelper::Copy(coeff, den, static_cast<uint32>(sizeof(T)) * nOfDenCoeff);
    }
    else {
        ok = false;
    }
    return ok;
    //lint -e{429} Custodial pointer 'coeff' (line 246) has not been freed or returned. Is not the responsability of the function to free the memory.
}

template<class T>
bool FilterT<T>::CheckNormalisation() {
    bool ok;
    if (isInitialised) {
        //lint -e{613} Possible use of null pointer. Not possible, protected by isInitialised
        T aux = static_cast<T>(den[0]);
        T aux2 = static_cast<T>(1);
        ok = IsEqualO(aux, aux2);
    }
    else {
        ok = false;
    }
    return ok;
}

template<class T>
bool FilterT<T>::IsEqualO(const int32 a,
                          const int32 b) const {
    return a == b;
}

template<class T>
bool FilterT<T>::IsEqualO(const int64 a,
                          const int64 b) const {
    return a == b;
}

template<class T>
bool FilterT<T>::IsEqualO(const float32 a,
                          const float32 b) const {
    return IsEqual(a, b);
}

template<class T>
bool FilterT<T>::IsEqualO(const float64 a,
                          const float64 b) const {
    return IsEqual(a, b);
}

template<class T>
float32 FilterT<T>::GetStaticGainFloat32(bool &isInfinite) {
    float32 ret = 0.0F;
    if (isInitialised) {
        ret = static_cast<float32>(staticGain);
        isInfinite = gainInfinite;
    }
    return ret;
}

template<class T>
float64 FilterT<T>::GetStaticGainFloat64(bool &isInfinite) {
    float64 ret = 0.0;
    if (isInitialised) {
        ret = static_cast<float64>(staticGain);
        isInfinite = gainInfinite;
    }
    return ret;
}

template<class T>
int32 FilterT<T>::GetStaticGainInt32(bool &isInfinite) {
    int32 ret = 0;
    if (isInitialised) {
        ret = static_cast<int32>(staticGain);
        isInfinite = gainInfinite;
    }
    return ret;
}

template<class T>
int64 FilterT<T>::GetStaticGainInt64(bool &isInfinite) {
    int64 ret = 0;
    if (isInitialised) {
        ret = static_cast<int64>(staticGain);
        isInfinite = gainInfinite;
    }
    return ret;
}

}
#endif /* SOURCE_COMPONENTS_GAMS_FILTERGAM_FILTERT_H_ */
