/**
 * @file NI9157DeviceOperatorDatabaseTest.h
 * @brief Header file for class NI9157DeviceOperatorDatabaseTest
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * NI9157DeviceOperatorDatabaseTest with all of its public, protected and
 * private members. It may also include definitions for inline methods which
 * need to be visible to the compiler.
 */

#ifndef NI9157DEVICEOPERATORDATABASETEST_H_
#define NI9157DEVICEOPERATORDATABASETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GlobalObjectsDatabase.h"
#include "NI9157DeviceOperatorDatabase.h"
#include "NI9157DeviceOperatorT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157DeviceOperatorDatabase methods.
 */
class NI9157DeviceOperatorDatabaseTest {
public:

    /**
     * @brief Constructor.
     */
    NI9157DeviceOperatorDatabaseTest();

    /**
     * @brief Destructor.
     */
    virtual ~NI9157DeviceOperatorDatabaseTest();

    /**
     * @brief Tests the NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperatorFromName.
     */
    bool TestGetCreateNI9157DeviceOperatorFromName();

    /**
     * @brief Tests the NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperatorFromTypeDes.
     */
    bool TestGetCreateNI9157DeviceOperatorFromTypeDes();

    /**
     * @brief Tests the NI9157DeviceOperatorDatabase::GetNI9157DeviceOperatorFromName.
     */
    template<typename T>
    bool TestGetNI9157DeviceOperatorFromName(const char8 *opName, T type);

    /**
     * @brief Tests the NI9157DeviceOperatorDatabase::GetNI9157DeviceOperatorFromTypeDes.
     */
    template<typename T>
    bool TestGetNI9157DeviceOperatorFromTypeDes(TypeDescriptor opType, T type);

};

template<typename T>
bool NI9157DeviceOperatorDatabaseTest::TestGetNI9157DeviceOperatorFromName(const char8 *opName, T type) {
    ReferenceT<NI9157Device> niDev(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    NI9157DeviceOperatorTI *niOperator = NI9157DeviceOperatorDatabase::GetNI9157DeviceOperator(opName, niDev);
    bool ret = (niOperator != NULL);

    if (ret) {
        ret = (dynamic_cast<NI9157DeviceOperatorT<T>*>(niOperator) != NULL);
    }
    if (ret) {
        delete niOperator;
    }

    return ret;
}

template<typename T>
bool NI9157DeviceOperatorDatabaseTest::TestGetNI9157DeviceOperatorFromTypeDes(TypeDescriptor opType, T type) {
    ReferenceT<NI9157Device> niDev(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    NI9157DeviceOperatorTI *niOperator = NI9157DeviceOperatorDatabase::GetNI9157DeviceOperator(opType, niDev);
    bool ret = (niOperator != NULL);

    if (ret) {
        ret = (dynamic_cast<NI9157DeviceOperatorT<T>*>(niOperator) != NULL);
    }
    if (ret) {
        delete niOperator;
    }

    return ret;
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157DEVICEOPERATORDATABASETEST_H_ */
