/**
 * @file HistogramComparator.h
 * @brief Header file for class HistogramComparator
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

 * @details This header file contains the declaration of the class HistogramComparator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HISTOGRAM_COMPARATOR_H_
#define HISTOGRAM_COMPARATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief An interface to be used in HistogramGAM to compute
 * to which bin of the histogram belongs a given occurrence.
 */
class HistogramComparator {
public:

    /**
     * @brief Constructor
     */
    HistogramComparator();

    /**
     * @brief Destructor
     */
    virtual ~HistogramComparator();

    /**
     * @brief Sets the lower bound of the allowed range.
     * @param[in] min contains the lower bound.
     */
    virtual void SetMin(const void * const min)=0;

    /**
     * @brief Sets the upper bound of the allowed range.
     * @param[in] max contains the upper bound.
     */
    virtual void SetMax(const void * const max)=0;

    /**
     * @brief Sets the number of bins in the histogram.
     * @param[in] nBinsIn is the number of bins.
     */
    virtual void SetNumberOfBins(const uint32 nBinsIn)=0;

    /**
     * @brief Returns at which bin belongs the value hold in \a mem.
     * @param[in] mem holds the occurrence value.
     */
    virtual uint32 InRange(const void * const mem)=0;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HISTOGRAM_COMPARATOR_H_ */

