/**
 * @file ObjectConnectionI.h
 * @brief Source file for class ObjectConnectionI
 * @date 16/09/2024
 * @author Cristian Galperti
 * @author Nicolo Ferron
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
 * the class ObjectConnectionI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ObjectConnectionI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ObjectConnectionI::ObjectConnectionI() :
        ReferenceContainer(), MessageI() {
}

ObjectConnectionI::~ObjectConnectionI() {
    while (GetSize() > 0u) {
        AnyType* toDelete;
        if(Extract(0U, toDelete)) {
            delete toDelete;
        }
    }

    while (paramNames.GetSize() > 0u) {
        StreamString* toDelete;
        if(paramNames.Extract(0U, toDelete)) {
            delete toDelete;
        }
    }
}

bool ObjectConnectionI::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    return ok;
}

StreamString ObjectConnectionI::GetParameterName(uint32 parameterIndex) {

    StreamString paramOut = "";

    if (parameterIndex < paramNames.GetSize()) {
        paramOut = *(paramNames[parameterIndex]);
    }

    return paramOut;
}

ErrorManagement::ErrorType ObjectConnectionI::TransposeAndCopy(void *const destination, const void *const source, const TypeDescriptor typeDesc,
                                                               const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;

    if (typeDesc==UnsignedInteger8Bit) {

        TransposeAndCopyT<uint8>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==UnsignedInteger16Bit) {

        TransposeAndCopyT<uint16>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==UnsignedInteger32Bit) {

        TransposeAndCopyT<uint32>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==UnsignedInteger64Bit) {

        TransposeAndCopyT<uint64>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==SignedInteger8Bit) {

        TransposeAndCopyT<int8>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==SignedInteger16Bit) {

        TransposeAndCopyT<int16>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==SignedInteger32Bit) {

        TransposeAndCopyT<int32>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==SignedInteger64Bit) {

        TransposeAndCopyT<int64>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==Float32Bit) {

        TransposeAndCopyT<float32>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else if (typeDesc==Float64Bit) {

        TransposeAndCopyT<float64>(destination, source, numberOfRows, numberOfColumns, numberOfPages);

    } else {

        ret = ErrorManagement::Exception;
        REPORT_ERROR(ret, "[%s] - Unsupported type %s", GetName(), TypeDescriptor::GetTypeNameFromTypeDescriptor(typeDesc));
    }

    return ret;
}

template<typename T>
void ObjectConnectionI::TransposeAndCopyT(void *const destination, const void *const source,
                                          const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages) {

    if (numberOfPages > 1u) {
        // 3D matrix
        for (uint32 rowIdx = 0u; rowIdx < numberOfRows; rowIdx++) {
            for (uint32 colIdx = 0u; colIdx < numberOfColumns; colIdx++) {
                for (uint32 pagIdx = 0u; pagIdx < numberOfPages; pagIdx++) {
                    *((T *)destination + rowIdx + numberOfRows  * colIdx + numberOfColumns * numberOfRows  * pagIdx)
                    = *((T *)source    + pagIdx + numberOfPages * colIdx + numberOfColumns * numberOfPages * rowIdx);
                }
            }
        }
    }
    else {
        // 2D matrix
        for (uint32 rowIdx = 0u; rowIdx < numberOfRows; rowIdx++) {
            for (uint32 colIdx = 0u; colIdx < numberOfColumns; colIdx++) {
                *( (T*) destination + rowIdx + numberOfRows    * colIdx )
                = *( (T*) source    + colIdx + numberOfColumns * rowIdx);
            }
        }
    }
}

CLASS_REGISTER(ObjectConnectionI, "1.0")
} /* namespace MARTe */
