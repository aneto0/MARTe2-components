/**
 * @file CreateNI9157DeviceOperatorITest.cpp
 * @brief Source file for class CreateNI9157DeviceOperatorITest
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
 * @details This source file contains the definition of all the methods for
 * the class CreateNI9157DeviceOperatorITest (public, protected, and private).
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CreateNI9157DeviceOperatorITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
CreateNI9157DeviceOperatorITest::CreateNI9157DeviceOperatorITest() {
}

CreateNI9157DeviceOperatorITest::~CreateNI9157DeviceOperatorITest() {
}

bool CreateNI9157DeviceOperatorITest::TestConstructor() {
    CreateNI9157DeviceOperatorI *creator =  NULL_PTR(CreateNI9157DeviceOperatorI *);
    creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    return ret;
}

bool CreateNI9157DeviceOperatorITest::TestCreate() {
    NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    ReferenceT<NI9157Device> niDevice(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        niDeviceOperator = creator->Create(niDevice);
        ret = (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *));
    }
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }
    return ret;
}

bool CreateNI9157DeviceOperatorITest::TestGetIdentifier() {
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        ret = (StringHelper::Compare(creator->GetIdentifier(), "NI9157DeviceU8") == 0);
    }
    return ret;
}

bool CreateNI9157DeviceOperatorITest::TestGetTypeDescriptor() {
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    bool ret = (creator->GetTypeDescriptor() == UnsignedInteger8Bit);
    return ret;
}
