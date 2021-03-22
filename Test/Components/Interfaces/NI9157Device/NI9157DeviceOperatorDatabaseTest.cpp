/**
 * @file NI9157DeviceOperatorDatabaseTest.cpp
 * @brief Source file for class NI9157DeviceOperatorDatabaseTest
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
 * @details This source file contains the definition of all the methods for
 * the class NI9157DeviceOperatorDatabaseTest (public, protected, and private).
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceOperatorDatabaseTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
NI9157DeviceOperatorDatabaseTest::NI9157DeviceOperatorDatabaseTest() {
}

NI9157DeviceOperatorDatabaseTest::~NI9157DeviceOperatorDatabaseTest() {
}

bool NI9157DeviceOperatorDatabaseTest::TestGetCreateNI9157DeviceOperatorFromName() {
    
    const char8 *names[] = { "NI9157DeviceBool", "NI9157DeviceU8", "NI9157DeviceI8", "NI9157DeviceU16", "NI9157DeviceI16", "NI9157DeviceU32", "NI9157DeviceI32", "NI9157DeviceU64", "NI9157DeviceI64", NULL_PTR(const char8 *)};
    TypeDescriptor typeDes[] = { UnsignedInteger8Bit, UnsignedInteger8Bit, SignedInteger8Bit, UnsignedInteger16Bit, SignedInteger16Bit, UnsignedInteger32Bit, SignedInteger32Bit, UnsignedInteger64Bit, SignedInteger64Bit };
    uint32 i = 0;
    bool ret = true;
    
    while ((names[i] != NULL_PTR(const char8 *)) && ret) {
        CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(names[i]);
        ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
        if (ret) {
            if (StringHelper::Compare(names[i], "NI9157DeviceBool") == 0) {
                ret = true;
            }
            else{
                ret = (creator->GetTypeDescriptor() == typeDes[i]);
            }
        }
        i++;
    }

    return ret;
}

bool NI9157DeviceOperatorDatabaseTest::TestGetCreateNI9157DeviceOperatorFromTypeDes() {
    
    const char8 *names[] = { "NI9157DeviceBool", "NI9157DeviceBool", "NI9157DeviceI8", "NI9157DeviceU16", "NI9157DeviceI16", "NI9157DeviceU32", "NI9157DeviceI32", "NI9157DeviceU64", "NI9157DeviceI64", NULL_PTR(const char8 *)};
    TypeDescriptor typeDes[] = { UnsignedInteger8Bit, UnsignedInteger8Bit, SignedInteger8Bit, UnsignedInteger16Bit, SignedInteger16Bit, UnsignedInteger32Bit, SignedInteger32Bit, UnsignedInteger64Bit, SignedInteger64Bit };
    uint32 i = 0;
    bool ret = true;
    
    while ((names[i] != NULL_PTR(const char8 *)) && ret) {
        CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(typeDes[i]);
        ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
        if (ret) {
            if (StringHelper::Compare(names[i], "NI9157DeviceBool") == 0) {
                ret = true;
            }
            else{
                ret = (StringHelper::Compare(creator->GetIdentifier(), names[i]) == 0);
            }
        }
        i++;
    }

    return ret;
}
