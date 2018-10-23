/**
 * @file HistogramComparatorTTest.h
 * @brief Header file for class HistogramComparatorTTest
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

 * @details This header file contains the declaration of the class HistogramComparatorTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HISTOGRAM_COMPARATORTTEST_H_
#define HISTOGRAM_COMPARATORTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HistogramComparatorT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the HistogramComparatorT methods
 */
template<typename T>
class HistogramComparatorTTest {
public:

    /**
     * @brief Constructor
     */
    HistogramComparatorTTest();

    /**
     * @brief Destructor
     */
    virtual ~HistogramComparatorTTest();

    /**
     * @brief Tests the HistogramComparatorT::SetMin method
     */
    bool TestSetMin();

    /**
     * @brief Tests the HistogramComparatorT::SetMax method
     */
    bool TestSetMax();

    /**
     * @brief Tests the HistogramComparatorT::SetNumberOfBins method
     */
    bool TestSetNumberOfBins();

    /**
     * @brief Tests the HistogramComparatorT::InRange method
     */
    bool TestInRange(T min,
                     T max,
                     uint32 nBins,
                     T value,
                     uint32 expected);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
HistogramComparatorTTest<T>::HistogramComparatorTTest() {
}

template<typename T>
HistogramComparatorTTest<T>::~HistogramComparatorTTest() {
}



template<typename T>
bool HistogramComparatorTTest<T>::TestSetMin(){
    return TestInRange(0, 10, 12, 5, 6);
}

template<typename T>
bool HistogramComparatorTTest<T>::TestSetMax(){
    return TestInRange(0, 10, 12, 5, 6);
}

template<typename T>
bool HistogramComparatorTTest<T>::TestSetNumberOfBins(){
    return TestInRange(0, 10, 12, 5, 6);
}


template<typename T>
bool HistogramComparatorTTest<T>::TestInRange(T min,
                                     T max,
                                     uint32 nBins,
                                     T value,
                                     uint32 expected) {
    HistogramComparatorT<T> test;

    test.SetMin((void*) &min);
    test.SetMax((void*) &max);
    test.SetNumberOfBins(nBins);

    return test.InRange((void*) &value) == expected;

}

#endif /* HISTOGRAM_COMPARATORTTEST_H_ */

