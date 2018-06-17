/**
 * @file EPICSPVStructureDataI.cpp
 * @brief Source file for class EPICSPVStructureDataI
 * @date 13/06/2018
 * @author Andre Neto
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

 * @details This source file contains the definition of all the methods for
 * the class EPICSPVStructureDataI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVStructureDataI.h"
#include "Reference.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
EPICSPVStructureDataI::EPICSPVStructureDataI() {
    structureFinalised = true;
}

EPICSPVStructureDataI::~EPICSPVStructureDataI() {

}

bool EPICSPVStructureDataI::Read(const char8 * const name, const AnyType &value) {
    bool ok = structureFinalised;
    bool isScalar = true;
    epics::pvData::PVScalarPtr scalarFieldPtr;
    epics::pvData::PVScalarArrayPtr scalarArrayPtr;
    AnyType storedType = GetType(name);
    if (ok) {
        ok = (storedType.GetTypeDescriptor() != VoidType);
    }
    if (ok) {
        scalarFieldPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVScalar>(currentStructPtr->getSubField(name));
        ok = (scalarFieldPtr);
        if (!ok) {
            isScalar = false;
            scalarArrayPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVScalarArray>(
                    currentStructPtr->getSubField(name));
            ok = (scalarArrayPtr);
        }
    }
    if (ok) {
        if (isScalar) {
            if (storedType.GetTypeDescriptor() == UnsignedInteger8Bit) {
                *reinterpret_cast<uint8 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<uint8>();
            }
            else if (storedType.GetTypeDescriptor() == UnsignedInteger16Bit) {
                *reinterpret_cast<uint16 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<uint16>();
            }
            else if (storedType.GetTypeDescriptor() == UnsignedInteger32Bit) {
                *reinterpret_cast<uint32 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<uint32>();
            }
            else if (storedType.GetTypeDescriptor() == UnsignedInteger64Bit) {
                *reinterpret_cast<uint64 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<long unsigned int>();
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger8Bit) {
                *reinterpret_cast<int8 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<int8>();
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger16Bit) {
                *reinterpret_cast<int16 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<int16>();
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger32Bit) {
                *reinterpret_cast<int32 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<int32>();
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger64Bit) {
                *reinterpret_cast<int64 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<long int>();
            }
            else if (storedType.GetTypeDescriptor() == Float32Bit) {
                *reinterpret_cast<float32 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<float32>();
            }
            else if (storedType.GetTypeDescriptor() == Float64Bit) {
                *reinterpret_cast<float64 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<float64>();
            }
            else if (storedType.GetTypeDescriptor() == CharString) {
                std::string src = scalarFieldPtr->getAs<std::string>();
                if (value.GetTypeDescriptor().type == SString) {
                    StreamString *dst = static_cast<StreamString *>(value.GetDataPointer());
                    if (dst != NULL_PTR(StreamString *)) {
                        *dst = src.c_str();
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                                 "Only StreamStrings and supported for the deserialisation of strings");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
            }
        }
        else {
            ok = (storedType.GetNumberOfElements(0u) == value.GetNumberOfElements(0u));
            if (ok) {
                if (storedType.GetTypeDescriptor() == UnsignedInteger8Bit) {
                    ok = ReadArray<uint8>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    ok = ReadArray<uint16>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    ok = ReadArray<uint32>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    ok = ReadArray<unsigned long int>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger8Bit) {
                    ok = ReadArray<int8>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger16Bit) {
                    ok = ReadArray<int16>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger32Bit) {
                    ok = ReadArray<int32>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger64Bit) {
                    ok = ReadArray<long int>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == Float32Bit) {
                    ok = ReadArray<float32>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == Float64Bit) {
                    ok = ReadArray<float64>(scalarArrayPtr, storedType, value);
                }
                else if (storedType.GetTypeDescriptor() == CharString) {
                    epics::pvData::shared_vector<const std::string> srcStr;
                    scalarArrayPtr->getAs<std::string>(srcStr);
                    if (value.GetTypeDescriptor().type == SString) {
                        uint32 numberOfElements = storedType.GetNumberOfElements(0u);
                        uint32 i;
                        Vector<StreamString> dst(static_cast<StreamString *>(value.GetDataPointer()), numberOfElements);
                        for (i = 0; i < numberOfElements; i++) {
                            dst[i] = srcStr[i].c_str();
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                                     "Only StreamStrings and supported for the deserialisation of strings");
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Array dimensions must match %d != %d",
                             storedType.GetNumberOfElements(0u), value.GetNumberOfElements(0u));
            }
        }
    }
    return ok;
}

AnyType EPICSPVStructureDataI::GetType(const char8 * const name) {
    AnyType at = voidAnyType;
    epics::pvData::PVFieldPtr fieldPtr = currentStructPtr->getSubField(name);
    bool ok = structureFinalised;
    if (ok) {
        ok = (fieldPtr);
    }
    if (ok) {
        epics::pvData::Type epicsType = fieldPtr->getField()->getType();
        epics::pvData::ScalarType epicsScalarType;
        TypeDescriptor marte2Type;
        uint32 numberOfElements = 1u;
        if (epicsType == epics::pvData::scalar) {
            epics::pvData::PVScalarPtr scalarFieldPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVScalar>(
                    currentStructPtr->getSubField(name));
            ok = (scalarFieldPtr);
            if (ok) {
                epicsScalarType = scalarFieldPtr->getScalar()->getScalarType();
            }
        }
        else if (epicsType == epics::pvData::scalarArray) {
            epics::pvData::PVScalarArrayPtr scalarArrayPtr =
                    std::tr1::dynamic_pointer_cast<epics::pvData::PVScalarArray>(currentStructPtr->getSubField(name));
            ok = (scalarArrayPtr);
            if (ok) {
                epicsScalarType = scalarArrayPtr->getScalarArray()->getElementType();
                numberOfElements = scalarArrayPtr->getScalarArray()->getMaximumCapacity();
            }
        }
        else {
            ok = false;
        }
        if (ok) {
            if (epicsScalarType == epics::pvData::pvByte) {
                marte2Type = SignedInteger8Bit;
            }
            else if (epicsScalarType == epics::pvData::pvShort) {
                marte2Type = SignedInteger16Bit;
            }
            else if (epicsScalarType == epics::pvData::pvInt) {
                marte2Type = SignedInteger32Bit;
            }
            else if (epicsScalarType == epics::pvData::pvLong) {
                marte2Type = SignedInteger64Bit;
            }
            else if (epicsScalarType == epics::pvData::pvUByte) {
                marte2Type = UnsignedInteger8Bit;
            }
            else if (epicsScalarType == epics::pvData::pvUShort) {
                marte2Type = UnsignedInteger16Bit;
            }
            else if (epicsScalarType == epics::pvData::pvUInt) {
                marte2Type = UnsignedInteger32Bit;
            }
            else if (epicsScalarType == epics::pvData::pvULong) {
                marte2Type = UnsignedInteger64Bit;
            }
            else if (epicsScalarType == epics::pvData::pvFloat) {
                marte2Type = Float32Bit;
            }
            else if (epicsScalarType == epics::pvData::pvDouble) {
                marte2Type = Float64Bit;
            }
            else if (epicsScalarType == epics::pvData::pvString) {
                marte2Type = CharString;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported EPICS type");
            }
        }
        if (ok) {
            at = AnyType(marte2Type, 0u, NULL_PTR(void *));
            if (numberOfElements > 1u) {
                at.SetNumberOfDimensions(1u);
                at.SetNumberOfElements(0u, numberOfElements);
            }
            else {
                at.SetNumberOfDimensions(0u);
            }
        }
    }
    return at;
}

bool EPICSPVStructureDataI::CreateFromStoredType(const char8 * const name, AnyType &storedType) {
    bool ok = true;
    epics::pvData::ScalarType epicsType;
    if (storedType.GetTypeDescriptor() == UnsignedInteger8Bit) {
        epicsType = epics::pvData::pvUByte;
    }
    else if (storedType.GetTypeDescriptor() == UnsignedInteger16Bit) {
        epicsType = epics::pvData::pvUShort;
    }
    else if (storedType.GetTypeDescriptor() == UnsignedInteger32Bit) {
        epicsType = epics::pvData::pvUInt;
    }
    else if (storedType.GetTypeDescriptor() == UnsignedInteger64Bit) {
        epicsType = epics::pvData::pvULong;
    }
    else if (storedType.GetTypeDescriptor() == SignedInteger8Bit) {
        epicsType = epics::pvData::pvByte;
    }
    else if (storedType.GetTypeDescriptor() == SignedInteger16Bit) {
        epicsType = epics::pvData::pvShort;
    }
    else if (storedType.GetTypeDescriptor() == SignedInteger32Bit) {
        epicsType = epics::pvData::pvInt;
    }
    else if (storedType.GetTypeDescriptor() == SignedInteger64Bit) {
        epicsType = epics::pvData::pvLong;
    }
    else if (storedType.GetTypeDescriptor() == Float32Bit) {
        epicsType = epics::pvData::pvFloat;
    }
    else if (storedType.GetTypeDescriptor() == Float64Bit) {
        epicsType = epics::pvData::pvDouble;
    }
    else if (storedType.GetTypeDescriptor() == CharString) {
        epicsType = epics::pvData::pvString;
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
        ok = false;
    }
    if (ok) {
        bool isScalar = (storedType.GetNumberOfElements(0u) <= 1u);
        if (isScalar) {
            fieldBuilder = fieldBuilder->add(name, epicsType);
        }
        else {
            fieldBuilder = fieldBuilder->addFixedArray(name, epicsType, storedType.GetNumberOfElements(0u));
        }
    }
    return ok;
}

bool EPICSPVStructureDataI::WriteStoredType(const char8 * const name, AnyType &storedType, const AnyType &value) {
    epics::pvData::PVScalarPtr scalarFieldPtr;
    epics::pvData::PVScalarArrayPtr scalarArrayPtr;

    bool isScalar = (storedType.GetNumberOfElements(0u) <= 1u);
    bool ok = true;
    if (isScalar) {
        scalarFieldPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVScalar>(currentStructPtr->getSubField(name));
        ok = (scalarFieldPtr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "%s should be a scalar but the conversion to PVScalar failed", name);
        }
    }
    else {
        scalarArrayPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVScalarArray>(
                currentStructPtr->getSubField(name));
        ok = (scalarArrayPtr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError,
                         "%s should be an array but the conversion to PVScalarArray failed", name);
        }
    }
    if (ok) {
        if (isScalar) {
            if (storedType.GetTypeDescriptor() == UnsignedInteger8Bit) {
                scalarFieldPtr->putFrom<uint8>(*reinterpret_cast<uint8 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == UnsignedInteger16Bit) {
                scalarFieldPtr->putFrom<uint16>(*reinterpret_cast<uint16 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == UnsignedInteger32Bit) {
                scalarFieldPtr->putFrom<uint32>(*reinterpret_cast<uint32 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == UnsignedInteger64Bit) {
                scalarFieldPtr->putFrom<unsigned long int>(*reinterpret_cast<uint64 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger8Bit) {
                scalarFieldPtr->putFrom<int8>(*reinterpret_cast<int8 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger16Bit) {
                scalarFieldPtr->putFrom<int16>(*reinterpret_cast<int16 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger32Bit) {
                scalarFieldPtr->putFrom<int32>(*reinterpret_cast<int32 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == SignedInteger64Bit) {
                scalarFieldPtr->putFrom<long int>(*reinterpret_cast<int64 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == Float32Bit) {
                scalarFieldPtr->putFrom<float32>(*reinterpret_cast<float32 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == Float64Bit) {
                scalarFieldPtr->putFrom<float64>(*reinterpret_cast<float64 *>(value.GetDataPointer()));
            }
            else if (storedType.GetTypeDescriptor() == CharString) {
                if (value.GetTypeDescriptor().type == SString) {
                    StreamString *src = static_cast<StreamString *>(value.GetDataPointer());
                    scalarFieldPtr->putFrom<std::string>(src->Buffer());
                }
                else if (value.GetTypeDescriptor() == CharString) {
                    char8 *src = static_cast<char8 *>(value.GetDataPointer());
                    scalarFieldPtr->putFrom<std::string>(std::string(src));
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError,
                                 "Only StreamStrings and char8 * are supported for the serialisation of strings");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
            }
        }
        else {
            ok = (storedType.GetNumberOfElements(0u) == value.GetNumberOfElements(0u));
            uint32 numberOfElements = storedType.GetNumberOfElements(0u);
            uint32 size = numberOfElements * storedType.GetTypeDescriptor().numberOfBits / 8u;
            if (ok) {
                if (storedType.GetTypeDescriptor() == UnsignedInteger8Bit) {
                    ok = WriteArray<uint8>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    ok = WriteArray<uint16>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    ok = WriteArray<uint32>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    ok = WriteArray<unsigned long int>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger8Bit) {
                    ok = WriteArray<int8>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger16Bit) {
                    ok = WriteArray<int16>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger32Bit) {
                    ok = WriteArray<int32>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == SignedInteger64Bit) {
                    ok = WriteArray<long int>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == Float32Bit) {
                    ok = WriteArray<float32>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == Float64Bit) {
                    ok = WriteArray<float64>(scalarArrayPtr, storedType, value, size);
                }
                else if (storedType.GetTypeDescriptor() == CharString) {
                    epics::pvData::shared_vector<const std::string> out;
                    out.resize(storedType.GetNumberOfElements(0u));
                    if (value.GetTypeDescriptor().type == SString) {
                        StreamString **src = static_cast<StreamString **>(value.GetDataPointer());
                        uint32 i;
                        for (i = 0; i < numberOfElements; i++) {
                            *const_cast<std::string *>(reinterpret_cast<const std::string *>(&out[i])) =
                                    src[i]->Buffer();
                        }
                        scalarArrayPtr->putFrom<std::string>(out);
                    }
                    else if (value.GetTypeDescriptor() == CharString) {
                        const char8 **src = static_cast<const char8 **>(value.GetDataPointer());
                        uint32 i;
                        for (i = 0; i < numberOfElements; i++) {
                            *const_cast<std::string *>(reinterpret_cast<const std::string *>(&out[i])) = src[i];
                        }
                        scalarArrayPtr->putFrom<std::string>(out);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError,
                                     "Only StreamStrings and char8 * are supported for the serialisation of strings");
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Array dimensions must match %d != %d",
                             storedType.GetNumberOfElements(0u), value.GetNumberOfElements(0u));
            }
        }
    }
    return ok;
}

bool EPICSPVStructureDataI::Write(const char8 * const name, const AnyType &value) {
    bool ok = true;
    if (!structureFinalised) {
        ok = cachedCDB.Write(name, value);
    }
    else {
        AnyType storedType = GetType(name);
        ok = WriteStoredType(name, storedType, value);
    }
    return ok;
}

bool EPICSPVStructureDataI::Copy(StructuredDataI &destination) {
    bool ok = true;
    return ok;
}

bool EPICSPVStructureDataI::AddToCurrentNode(Reference node) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.AddToCurrentNode(node);
    }
    return ok;
}

bool EPICSPVStructureDataI::MoveToRoot() {
    std::cout << "MoveToRoot>>>>>>>>>>>>>>>>" << std::endl;
    bool ok = true;
    if (structureFinalised) {
        rootStructPtr->dumpValue(std::cout);
        currentStructPtr = rootStructPtr;
        currentStructPtr->dumpValue(std::cout);
    }
    else {
        ok = cachedCDB.MoveToRoot();
    }
    std::cout << "MoveToRoot<<<<<<<<<<<<<<<<" << std::endl;
    return ok;
}

bool EPICSPVStructureDataI::MoveToAncestor(uint32 generations) {
    uint32 i;
    bool ok = true;
    if (structureFinalised) {
        epics::pvData::PVStructurePtr startStructPtr = currentStructPtr;
        for (i = 0u; (i < generations) && (ok); i++) {
            ok = (currentStructPtr != rootStructPtr);
            if (ok) {
                currentStructPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVStructure>(
                        currentStructPtr->getParent()->shared_from_this());
            }
        }
        if (!ok) {
            currentStructPtr = startStructPtr;
        }
        std::cout << "MoveToAncestor>>>>>>>>>>>>>>>>" << std::endl;
        currentStructPtr->dumpValue(std::cout);
        std::cout << "MoveToAncestor<<<<<<<<<<<<<<<<" << std::endl;
    }
    else {
        ok = cachedCDB.MoveToAncestor(generations);
    }
    return ok;
}

bool EPICSPVStructureDataI::MoveAbsolute(const char8 * const path) {
    epics::pvData::PVStructurePtr movePtr;
    bool ok = true;
    if (structureFinalised) {
        ok = MoveToRoot();

        if (ok) {
            movePtr = currentStructPtr->getSubField<epics::pvData::PVStructure>(path);
            //shared_ptr operator bool verifies the validity of the underlying ptr.
            ok = (movePtr);
        }
        if (ok) {
            currentStructPtr = movePtr;
        }
        std::cout << "MoveAbsolute>>>>>>>>>>>>>>>>" << std::endl;
        currentStructPtr->dumpValue(std::cout);
        std::cout << "MoveAbsolute<<<<<<<<<<<<<<<<" << std::endl;
    }
    else {
        ok = cachedCDB.MoveAbsolute(path);
    }
    return ok;
}

bool EPICSPVStructureDataI::MoveRelative(const char8 * const path) {
    bool ok = true;
    if (structureFinalised) {
        epics::pvData::PVStructurePtr movePtr = currentStructPtr->getSubField<epics::pvData::PVStructure>(path);
        ok = (movePtr);
        if (ok) {
            currentStructPtr = movePtr;
        }
        std::cout << "MoveRelative>>>>>>>>>>>>>>>>" << std::endl;
        currentStructPtr->dumpValue(std::cout);
        std::cout << "MoveRelative<<<<<<<<<<<<<<<<" << std::endl;
    }
    else {
        ok = cachedCDB.MoveRelative(path);
    }
    return ok;
}

bool EPICSPVStructureDataI::MoveToChild(const uint32 childIdx) {
    bool ok = true;
    if (structureFinalised) {
        const epics::pvData::PVFieldPtrArray & fields = currentStructPtr->getPVFields();
        epics::pvData::PVStructurePtr movePtr;
        bool ok = (childIdx < fields.size());
        if (ok) {
            epics::pvData::PVFieldPtr field = fields[childIdx];
            movePtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVStructure>(field->shared_from_this());
            ok = movePtr;
        }
        if (ok) {
            currentStructPtr = movePtr;
        }
        std::cout << "MoveToChild>>>>>>>>>>>>>>>>" << std::endl;
        currentStructPtr->dumpValue(std::cout);
        std::cout << "MoveToChild<<<<<<<<<<<<<<<<" << std::endl;
    }
    else {
        ok = cachedCDB.MoveToChild(childIdx);
    }
    return ok;
}

bool EPICSPVStructureDataI::ConfigurationDataBaseToPVStructurePtr(ReferenceT<ReferenceContainer> currentNode, bool create) {
    bool ok = true;
    uint32 nOfChildren = currentNode->Size();
    ReferenceT<ReferenceContainer> foundNode;
    StreamString currentNodeName = currentNode->GetName();
    uint32 i;
    if (StringHelper::Length(currentNodeName.Buffer()) > 0u) {
        if (create) {
            fieldBuilder = fieldBuilder->addNestedStructure(currentNodeName.Buffer());
        }
        else {
            ok = MoveRelative(currentNodeName.Buffer());
        }
    }
    for (i = 0; (i < nOfChildren) && (ok); i++) {
        foundNode = currentNode->Get(i);
        if (foundNode.IsValid()) {
            ok = ConfigurationDataBaseToPVStructurePtr(foundNode, create);
        }
        else {
            ReferenceT<AnyObject> foundLeaf = currentNode->Get(i);
            if (foundLeaf.IsValid()) {
                AnyType storedType = foundLeaf->GetType();
                if (create) {
                    ok = CreateFromStoredType(foundLeaf->GetName(), storedType);
                }
                else {
                    ok = WriteStoredType(foundLeaf->GetName(), storedType, storedType);
                }
            }
        }
    }
    if (StringHelper::Length(currentNodeName.Buffer()) > 0u) {
        if (create) {
            fieldBuilder = fieldBuilder->endNested();
        }
        else {
            ok = MoveToAncestor(1u);
        }
    }
    return ok;
}

bool EPICSPVStructureDataI::CreateAbsolute(const char8 * const path) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.CreateAbsolute(path);
    }
    return ok;
}

bool EPICSPVStructureDataI::CreateRelative(const char8 * const path) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.CreateRelative(path);
    }
    return ok;
}

bool EPICSPVStructureDataI::Delete(const char8 * const name) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.Delete(name);
    }
    return ok;
}

const char8 *EPICSPVStructureDataI::GetName() {
    return currentStructPtr->getFieldName().c_str();
}

const char8 *EPICSPVStructureDataI::GetChildName(const uint32 index) {
    const char8 * ret = "";
    const epics::pvData::PVFieldPtrArray & fields = currentStructPtr->getPVFields();
    epics::pvData::PVStructurePtr movePtr;
    bool ok = (index < fields.size());
    if (ok) {
        epics::pvData::PVFieldPtr field = fields[index];
        movePtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVStructure>(field->shared_from_this());
        ok = movePtr;
    }
    if (ok) {
        ret = movePtr->getFieldName().c_str();
    }
    return ret;
}

uint32 EPICSPVStructureDataI::GetNumberOfChildren() {
    uint32 numberOfFields = currentStructPtr->getNumberFields();
    return numberOfFields;
}

void EPICSPVStructureDataI::SetStructure(epics::pvData::PVStructurePtr structPtrToSet) {
    structureFinalised = true;
    currentStructPtr = structPtrToSet;
    rootStructPtr = structPtrToSet;
}

void EPICSPVStructureDataI::InitStructure() {
    structureFinalised = false;
    fieldCreate = epics::pvData::getFieldCreate();
    fieldBuilder = fieldCreate->createFieldBuilder();
}

void EPICSPVStructureDataI::FinaliseStructure() {
    epics::pvData::PVDataCreatePtr pvDataCreate = epics::pvData::getPVDataCreate();
    bool ok = cachedCDB.MoveToRoot();
    ReferenceT<ReferenceContainer> rootNode = cachedCDB.GetCurrentNode();
    if (ok) {
        ok = ConfigurationDataBaseToPVStructurePtr(rootNode);
    }
    if (ok) {
        currentStructPtr = pvDataCreate->createPVStructure(fieldBuilder->createStructure());
        rootStructPtr = currentStructPtr;
        rootStructPtr->dumpValue(std::cout);
    }
    structureFinalised = ok;
    if (ok) {
        ok = ConfigurationDataBaseToPVStructurePtr(rootNode, false);
    }
}

CLASS_REGISTER(EPICSPVStructureDataI, "")
}
