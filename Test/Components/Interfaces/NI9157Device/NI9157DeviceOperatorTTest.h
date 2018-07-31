/**
 * @file NI9157DeviceOperatorTTest.h
 * @brief Header file for class NI9157DeviceOperatorTTest
 * @date 23/05/2018
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class NI9157DeviceOperatorTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157DEVICEOPERATORTTEST_H_
#define NI9157DEVICEOPERATORTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceOperatorT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the NI9157DeviceOperatorT methods
 */
template<typename T>
class NI9157DeviceOperatorTTest {
public:

    /**
     * @brief Constructor
     */
    NI9157DeviceOperatorTTest();

    /**
     * @brief Destructor
     */
    virtual ~NI9157DeviceOperatorTTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestGetByteSize method
     */
    bool TestGetByteSize();

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestCompare method
     */
    bool TestCompare(T a, T b);

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestCopy method
     */
    bool TestCopy(T source);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
NI9157DeviceOperatorTTest<T>::NI9157DeviceOperatorTTest() {
}

template<typename T>
NI9157DeviceOperatorTTest<T>::~NI9157DeviceOperatorTTest() {
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestConstructor() {
    NI9157DeviceOperatorT<T> test;
    return !test.IsValid();
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestGetByteSize() {
    NI9157DeviceOperatorT<T> test;
    return test.GetByteSize() == sizeof(T);
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestCompare(T a, T b) {
    int32 isEqual = 0;
    if (a < b) {
        isEqual = -1;
    }
    if (a > b) {
        isEqual = 1;
    }
    NI9157DeviceOperatorT<T> test;
    return test.Compare((uint8 *) &a, (uint8 *) &b) == isEqual;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestCopy(T source) {
    T dest;
    NI9157DeviceOperatorT<T> test;
    test.Copy((uint8 *) &dest, (uint8 *) &source);
    return dest == source;
}

#endif /* TEST_COMPONENTS_INTERFACES_NI9157DEVICE_NI9157DEVICEOPERATORTTEST_H_ */

