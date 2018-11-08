/**
 * @file HistogramComparatorT.h
 * @brief Header file for class HistogramComparatorT
 * @date 30/08/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class HistogramComparatorT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef COMPARATORT_H_
#define COMPARATORT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HistogramComparator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/*lint -esym(9107, MARTe::HistogramComparator*) the definition must be in the header because it is a template*/

/**
 * @brief Template version of Comparator
 */
template<typename T>
class HistogramComparatorT: public HistogramComparator {
public:

    /**
     * @brief Constructor
     */
    HistogramComparatorT();

    /**
     * @brief Destructor
     */
    virtual ~HistogramComparatorT();

    /**
     * @see HistogramComparator::SetMin()
     */
    virtual void SetMin(const void* const min);

    /**
     * @see HistogramComparator::SetMax()
     */
    virtual void SetMax(const void* const max);

    /**
     * @see HistogramComparator::SetNumberOfBins()
     */
    virtual void SetNumberOfBins(const uint32 nBinsIn);

    /**
     * @see HistogramComparator::GetNumberOfBins()
     */
    virtual uint32 GetNumberOfBins();

    /**
     * @see HistogramComparator::InRange()
     */
    virtual uint32 InRange(const void * const mem);

private:

    /**
     * The upper bound
     */
    T maxLim;

    /**
     * The lower bound
     */
    T minLim;

    /**
     * The size of a bin range
     */
    T delta;

    /**
     * The number of bins
     */
    uint32 nBins;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename T>
HistogramComparatorT<T>::HistogramComparatorT() :
        HistogramComparator() {
    maxLim = static_cast<T>(0);
    minLim = static_cast<T>(0);
    nBins = 3u;
    delta = static_cast<T>(0);

}

template<typename T>
HistogramComparatorT<T>::~HistogramComparatorT() {

}

template<typename T>
void HistogramComparatorT<T>::SetMin(const void * const min) {
    minLim = *reinterpret_cast<T*>(const_cast<void *>(min));
}

template<typename T>
void HistogramComparatorT<T>::SetMax(const void* const max) {
    maxLim = *reinterpret_cast<T*>(const_cast<void *>(max));
}

template<typename T>
void HistogramComparatorT<T>::SetNumberOfBins(const uint32 nBinsIn) {
    nBins = nBinsIn;
    if (nBins > 2u) {
        T range = (maxLim - minLim);
        uint32 nBinsTemp = (nBins - 2u);
        /*lint -e{737} -e{9117} -e{9125} -e{573} -e{9115} Loss of precision is responsibility of the conversion requested by the user.*/
        delta = static_cast<T>(range / nBinsTemp);
    }
}

template<typename T>
uint32 HistogramComparatorT<T>::GetNumberOfBins() {
    return nBins;
}


template<typename T>
uint32 HistogramComparatorT<T>::InRange(const void* const mem) {

    const T* toCompare = reinterpret_cast<T*>(const_cast<void *>(mem));

    uint32 ret = 0u;
    if (*toCompare < minLim) {
        ret = 0u;
    }
    else if (*toCompare >= maxLim) {
        ret = (nBins - 1u);
    }
    else {
        //do the division with a
        for (uint32 i = 0u; (i < (nBins - 2u)); i++) {
            /*lint -e{737} -e{9117} -e{9125} -e{9119} -e{9114} -e{9115} -e{776} Loss of precision is responsibility of the conversion requested by the user.*/
            T minRange = (minLim + static_cast<T>(i * delta));
            /*lint -e{737} -e{9117} -e{9125} -e{9119} -e{9114} -e{9115} -e{776} Loss of precision is responsibility of the conversion requested by the user.*/
            T maxRange = (minLim + static_cast<T>((i + 1u) * delta));
            /*lint -e{737} -e{9117} -e{9125} Loss of precision is responsibility of the conversion requested by the user.*/
            if (((*toCompare) >= (minRange)) && ((*toCompare) < maxRange)) {
                ret = (i + 1u);
                break;
            }
        }
    }

    return ret;
}

}
#endif /* COMPARATORT_H_ */

