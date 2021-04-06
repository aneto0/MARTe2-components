/**
 * @file NI9157DeviceOperatorDatabase.cpp
 * @brief Source file for class NI9157DeviceOperatorDatabase.
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
 * the class NI9157DeviceOperatorDatabase (public, protected, and private). Be 
 * aware that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CreateNI9157DeviceOperator.h"
#include "NI9157DeviceOperatorDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
	
namespace NI9157DeviceOperatorDatabase {

/**
 * bool implementation.
 */
CreateNI9157DeviceOperator<bool> CreateNI9157DeviceOperatorIBool("NI9157DeviceBool");

/**
 * uint8 implementation.
 */
CreateNI9157DeviceOperator<uint8> CreateNI9157DeviceOperatorIU8("NI9157DeviceU8");

/**
 * int8 implementation.
 */
CreateNI9157DeviceOperator<int8> CreateNI9157DeviceOperatorII8("NI9157DeviceI8");

/**
 * uint16 implementation.
 */
CreateNI9157DeviceOperator<uint16> CreateNI9157DeviceOperatorIU16("NI9157DeviceU16");

/**
 * int16 implementation.
 */
CreateNI9157DeviceOperator<int16> CreateNI9157DeviceOperatorII16("NI9157DeviceI16");

/**
 * uint32 implementation.
 */
CreateNI9157DeviceOperator<uint32> CreateNI9157DeviceOperatorIU32("NI9157DeviceU32");

/**
 * int32 implementation.
 */
CreateNI9157DeviceOperator<int32> CreateNI9157DeviceOperatorII32("NI9157DeviceI32");

/**
 * uint64 implementation.
 */
CreateNI9157DeviceOperator<uint64> CreateNI9157DeviceOperatorIU64("NI9157DeviceU64");

/**
 * int64 implementation.
 */
CreateNI9157DeviceOperator<int64> CreateNI9157DeviceOperatorII64("NI9157DeviceI64");

static CreateNI9157DeviceOperatorI *createNI9157DeviceOperatorIList[] = { &CreateNI9157DeviceOperatorIBool, &CreateNI9157DeviceOperatorIU8,
        &CreateNI9157DeviceOperatorII8, &CreateNI9157DeviceOperatorIU16, &CreateNI9157DeviceOperatorII16, &CreateNI9157DeviceOperatorIU32,
        &CreateNI9157DeviceOperatorII32, &CreateNI9157DeviceOperatorIU64, &CreateNI9157DeviceOperatorII64, NULL_PTR(CreateNI9157DeviceOperatorI *) };

CreateNI9157DeviceOperatorI *GetCreateNI9157DeviceOperator(const char8 * const niDevId) {

    uint8 i = 0u;
    while (createNI9157DeviceOperatorIList[i] != NULL_PTR(CreateNI9157DeviceOperatorI *)) {
        if (StringHelper::Compare(niDevId, createNI9157DeviceOperatorIList[i]->GetIdentifier()) == 0) {
            break;
        }
        i++;
    }

    return createNI9157DeviceOperatorIList[i];
}

CreateNI9157DeviceOperatorI *GetCreateNI9157DeviceOperator(const TypeDescriptor &td) {

    uint8 i = 0u;
    while (createNI9157DeviceOperatorIList[i] != NULL_PTR(CreateNI9157DeviceOperatorI *)) {
        if (createNI9157DeviceOperatorIList[i]->GetTypeDescriptor() == td) {
            break;
        }
        i++;
    }

    return createNI9157DeviceOperatorIList[i];
}

NI9157DeviceOperatorTI *GetNI9157DeviceOperator(const char8 * const niDevId,
                                                ReferenceT<NI9157Device> niDev) {

    uint8 i = 0u;
    NI9157DeviceOperatorTI *ret = NULL_PTR(NI9157DeviceOperatorTI *);
    while (createNI9157DeviceOperatorIList[i] != NULL_PTR(CreateNI9157DeviceOperatorI *)) {
        if (StringHelper::Compare(niDevId, createNI9157DeviceOperatorIList[i]->GetIdentifier()) == 0) {
            ret = createNI9157DeviceOperatorIList[i]->Create(niDev);
        }
        i++;
    }

    return ret;
}

NI9157DeviceOperatorTI *GetNI9157DeviceOperator(const TypeDescriptor &td,
                                                ReferenceT<NI9157Device> niDev) {

    uint8 i = 0u;
    NI9157DeviceOperatorTI *ret = NULL_PTR(NI9157DeviceOperatorTI *);
    while (createNI9157DeviceOperatorIList[i] != NULL) {
        if (createNI9157DeviceOperatorIList[i]->GetTypeDescriptor() == td) {
            ret = createNI9157DeviceOperatorIList[i]->Create(niDev);
        }
        i++;
    }

    return ret;
}

}

}
