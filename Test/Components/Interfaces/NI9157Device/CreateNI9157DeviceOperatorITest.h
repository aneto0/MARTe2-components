/**
 * @file CreateNI9157DeviceOperatorITest.h
 * @brief Header file for class CreateNI9157DeviceOperatorITest
 * @date 22/03/2021
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
 * CreateNI9157DeviceOperatorITest with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to be
 * visible to the compiler.
 */

#ifndef CREATENI9157DEVICEOPERATORITEST_H_
#define CREATENI9157DEVICEOPERATORITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CreateNI9157DeviceOperatorI.h"
#include "GlobalObjectsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the CreateNI9157DeviceOperatorI methods.
 */
class CreateNI9157DeviceOperatorITest {
public:

    /**
     * @brief Constructor.
     */
    CreateNI9157DeviceOperatorITest();

    /**
     * @brief Destructor.
     */
    virtual ~CreateNI9157DeviceOperatorITest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the destructor.
     */
    bool TestConstructorArgs();

    /**
     * @brief Tests the CreateNI9157DeviceOperatorI::Create method.
     */
    template<typename T>
    bool TestCreate(T type);

    /**
     * @brief Tests the CreateNI9157DeviceOperatorI::GetIdentifier method.
     */
    bool TestGetIdentifier();

    /**
     * @brief Tests the CreateNI9157DeviceOperatorI::GetTypeDescriptor method.
     */
    bool TestGetTypeDescriptor();

};

template<typename T>
bool CreateNI9157DeviceOperatorITest::TestCreate(T type) {
    CreateNI9157DeviceOperatorI<T> test("ciaoBella");
    ReferenceT<NI9157Device> niDev(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    NI9157DeviceOperatorTI *niOperator = test.Create(niDev);
    bool ret = niOperator != NULL;

    if (ret) {
        ret = dynamic_cast<NI9157DeviceOperatorT<T>*>(niOperator) != NULL;
    }
    if (ret) {
        delete niOperator;
    }

    return ret;
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CREATENI9157DEVICEOPERATORITEST_H_ */
