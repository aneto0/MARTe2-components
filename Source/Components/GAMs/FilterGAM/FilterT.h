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
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<class T>
class FilterT: public Filter {
public:
    FilterT();
    virtual ~FilterT();
    virtual bool Initialise(void *numIn,
                            uint32 nOfNumCoeffIn,
                            void *denIn,
                            uint32 nOfDenCoeffIn);
    virtual bool Reset();
    virtual void Process(void *input,
                         void *output,
                         uint32 nOflements);
    virtual uint32 GetNumberOfNumCoeff();
    virtual uint32 GetNumberOfDenCoeff();
    virtual bool GetNumCoeff(void *coeff);
    virtual bool GetDenCoeff(void *coeff);
    virtual bool CheckNormalisation();
    virtual float32 GetStaticGainFloat32(bool &isInfinite);
    virtual float64 GetStaticGainFloat64(bool &isInfinite);
    virtual int32 GetStaticGainInt32(bool &isInfinite);
    virtual int64 GetStaticGainInt64(bool &isInfinite);
    //FilterT<T>& operator =(const FilterT<T> &that);
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
     * Holds the number of numerator coefficients
     */
    uint32 nOfNumCoeff;

    /**
     * Holds the number of denominator coefficients
     */
    uint32 nOfDenCoeff;

    /**
     * Array of pointer to the last input values
     */
    T *lastInputs;

    /**
     * Array of pointers to the last output values
     */
    T *lastOutputs;

    T staticGain;

    bool gainInfinite;

    bool isInitialised;

    bool IsEqualO(int32 a,
                  int32 b);
    bool IsEqualO(int64 a,
                  int64 b);
    bool IsEqualO(float32 a,
                  float32 b);
    bool IsEqualO(float64 a,
                  float64 b);

};
template<class T>
FilterT<T>::FilterT() :
        Filter() {
    lastInputs = NULL_PTR(T*);
    lastOutputs = NULL_PTR(T*);
    num = NULL_PTR(T*);
    den = NULL_PTR(T*);
    nOfNumCoeff = 0u;
    nOfDenCoeff = 0u;
    isInitialised = false;
    staticGain = 0;
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
bool FilterT<T>::Initialise(void *numIn,
                            uint32 nOfNumCoeffIn,
                            void *denIn,
                            uint32 nOfDenCoeffIn) {

    bool ok = (numIn != NULL_PTR(T*)) && (denIn != NULL_PTR(T*));
    if (ok) {
        nOfNumCoeff = nOfNumCoeffIn;
        nOfDenCoeff = nOfDenCoeffIn;
        num = new T[nOfNumCoeff];
        den = new T[nOfDenCoeff];
        ok = MemoryOperationsHelper::Copy(reinterpret_cast<void*>(num), reinterpret_cast<void*>(numIn), sizeof(T) * nOfNumCoeff);
        if (ok) {
            ok = MemoryOperationsHelper::Copy(reinterpret_cast<void*>(den), reinterpret_cast<void*>(denIn), sizeof(T) * nOfDenCoeff);
        }
        Reset();
    }
    if (ok) {
        if (ok) {
            T sumNumerator = 0;
            for (uint32 i = 0u; i < nOfNumCoeff; i++) {
                //if due to MISRA rules however it is not necessary. At his line the initialization of num is guaranteed b the ok = true...
                if (num != NULL_PTR(T*)) {
                    sumNumerator += num[i];
                }
            }
            T sumDenominator = 0;
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
    bool ok = (lastInputs != NULL_PTR(T*)) | (lastOutputs != NULL_PTR(T*));
    if (ok) {
        MARTe::MemoryOperationsHelper::Set(lastInputs, static_cast<MARTe::char8>(0), sizeof(T) * nOfNumCoeff);
        MARTe::MemoryOperationsHelper::Set(lastOutputs, static_cast<MARTe::char8>(0), sizeof(T) * nOfDenCoeff);
    }
    return ok;
}

template<class T>
void FilterT<T>::Process(void *input,
                         void *output,
                         uint32 nOfElements) {
    for (uint32 elIdx = 0; elIdx < nOfElements; elIdx++) {
        lastInputs[0] = reinterpret_cast<T*>(input)[elIdx];
        lastOutputs[0] = 0.0;
        for (MARTe::uint32 i = nOfNumCoeff - 1u; i > 0u; i--) {
            lastOutputs[0] += lastInputs[i] * num[i];
            lastInputs[i] = lastInputs[i - 1u];
        }
        lastOutputs[0] += lastInputs[0u] * num[0u];
        for (MARTe::uint32 i = nOfDenCoeff - 1u; i > 0u; i--) {
            lastOutputs[0] -= lastOutputs[i] * den[i];
            lastOutputs[i] = lastOutputs[i - 1u];
        }
        reinterpret_cast<T*>(output)[elIdx] = lastOutputs[0];
    }
}

template<class T>
uint32 FilterT<T>::GetNumberOfNumCoeff() {
    return nOfNumCoeff;
}
template<class T>
uint32 FilterT<T>::GetNumberOfDenCoeff() {
    return nOfDenCoeff;
}
template<class T>
bool FilterT<T>::GetNumCoeff(void *coeff) {
    bool ok;
    if (isInitialised) {
        ok = MemoryOperationsHelper::Copy(coeff, num, sizeof(T) * nOfNumCoeff);
    }
    else {
        ok = false;
    }
    return ok;
}

template<class T>
bool FilterT<T>::GetDenCoeff(void *coeff) {
    bool ok;
    if (isInitialised) {
        ok = MemoryOperationsHelper::Copy(coeff, den, sizeof(T) * nOfDenCoeff);
    }
    else {
        ok = false;
    }
    return ok;
}

template<class T>
bool FilterT<T>::CheckNormalisation() {
    bool ok;
    if (isInitialised) {
        T aux = static_cast<T>(den[0]);
        T aux2 = static_cast<T>(0);
        ok = IsEqualO(aux, aux2);
    }
    else {
        ok = false;
    }
    return ok;
}

template<class T>
bool FilterT<T>::IsEqualO(int32 a,
                          int32 b) {
    return a == b;
}

template<class T>
bool FilterT<T>::IsEqualO(int64 a,
                          int64 b) {
    return a == b;
}

template<class T>
bool FilterT<T>::IsEqualO(float32 a,
                          float32 b) {
    return IsEqual(a, b);
}

template<class T>
bool FilterT<T>::IsEqualO(float64 a,
                          float64 b) {
    return IsEqual(a, b);
}

template<class T>
float32 FilterT<T>::GetStaticGainFloat32(bool &isInfinite) {
    T ret = 0.0;
    if (isInitialised) {
        ret = staticGain;
        isInfinite = gainInfinite;
    }
    return ret;
}

template<class T>
float64 FilterT<T>::GetStaticGainFloat64(bool &isInfinite) {
    T ret = 0.0;
    if (isInitialised) {
        ret = staticGain;
        isInfinite = gainInfinite;
    }
    return ret;
}

template<class T>
int32 FilterT<T>::GetStaticGainInt32(bool &isInfinite) {
    T ret = 0.0;
    if (isInitialised) {
        ret = staticGain;
        isInfinite = gainInfinite;
    }
    return ret;
}

template<class T>
int64 FilterT<T>::GetStaticGainInt64(bool &isInfinite) {
    T ret = 0.0;
    if (isInitialised) {
        ret = staticGain;
        isInfinite = gainInfinite;
    }
    return ret;
}

//template<class T>
//FilterT<T>& FilterT<T>::operator =(const FilterT<T> &that) {
//if (this != that) {
//    if (that->isInitialised) {
//        this->nOfNumCoeff = that->GetNumberOfNumCoeff();
//        this->nOfDenCoeff = that->GetNumberOfDenCoeff();
//        this->num = new T[this->nOfNumCoeff];
//        this->den = new T[this->nOfDenCoeff];
//        this->lastInputs = new T[this->nOfNumCoeff];
//        this->lastOutputs = new T[this->nOfDenCoeff];
//        this->staticGain = that->staticGain;
//        this->gainInfinite = that->gainInfinite;
//    }
//}
//return *this;
//}

}
#endif /* SOURCE_COMPONENTS_GAMS_FILTERGAM_FILTERT_H_ */
