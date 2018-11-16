/**
 * @file EPICSPVAStructureDataI.cpp
 * @brief Source file for class EPICSPVAStructureDataI
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
 * the class EPICSPVAStructureDataI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVAStructureDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
EPICSPVAStructureDataI::EPICSPVAStructureDataI() :
        Object() {
    structureFinalised = true;
}

EPICSPVAStructureDataI::~EPICSPVAStructureDataI() {

}

bool EPICSPVAStructureDataI::Read(const char8 * const name, const AnyType &value) {
    bool ok = structureFinalised;
    bool isScalar = true;
    epics::pvData::PVScalarPtr scalarFieldPtr;
    epics::pvData::PVScalarArrayPtr scalarArrayPtr;
    AnyType storedType = GetType(name);
    if (ok) {
        ok = (storedType.GetTypeDescriptor() != VoidType);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "FinaliseStructure must be called before Read().");
    }
    if (ok) {
        scalarFieldPtr = std::dynamic_pointer_cast < epics::pvData::PVScalar > (currentStructPtr->getSubField(name));
        ok = (scalarFieldPtr ? true : false);
        if (!ok) {
            isScalar = false;
            scalarArrayPtr = std::dynamic_pointer_cast < epics::pvData::PVScalarArray > (currentStructPtr->getSubField(name));
            ok = (scalarArrayPtr ? true : false);
        }
    }
    if (ok) {
        if (isScalar) {
            if (value.GetTypeDescriptor() == UnsignedInteger8Bit) {
                if (scalarFieldPtr->getScalar()->getScalarType() == epics::pvData::pvBoolean) {
                    bool bval = scalarFieldPtr->getAs<epics::pvData::boolean>();
                    *reinterpret_cast<uint8 *>(value.GetDataPointer()) = (bval ? 1u : 0u);
                }
                else {
                    *reinterpret_cast<uint8 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<uint8>();
                }
            }
            else if (value.GetTypeDescriptor() == UnsignedInteger16Bit) {
                *reinterpret_cast<uint16 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<uint16>();
            }
            else if (value.GetTypeDescriptor() == UnsignedInteger32Bit) {
                *reinterpret_cast<uint32 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<uint32>();
            }
            else if (value.GetTypeDescriptor() == UnsignedInteger64Bit) {
                *reinterpret_cast<uint64 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<long unsigned int>();
            }
            else if (value.GetTypeDescriptor() == SignedInteger8Bit) {
                *reinterpret_cast<int8 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<int8>();
            }
            else if (value.GetTypeDescriptor() == SignedInteger16Bit) {
                *reinterpret_cast<int16 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<int16>();
            }
            else if (value.GetTypeDescriptor() == SignedInteger32Bit) {
                *reinterpret_cast<int32 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<int32>();
            }
            else if (value.GetTypeDescriptor() == SignedInteger64Bit) {
                *reinterpret_cast<int64 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<long int>();
            }
            else if (value.GetTypeDescriptor() == Float32Bit) {
                *reinterpret_cast<float32 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<float32>();
            }
            else if (value.GetTypeDescriptor() == Float64Bit) {
                *reinterpret_cast<float64 *>(value.GetDataPointer()) = scalarFieldPtr->getAs<float64>();
            }
            else if (value.GetTypeDescriptor().type == SString) {
                std::string src = scalarFieldPtr->getAs<std::string>();
                StreamString *dst = static_cast<StreamString *>(value.GetDataPointer());
                if (dst != NULL_PTR(StreamString *)) {
                    *dst = src.c_str();
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported read type");
                ok = false;
            }
        }
        else {
            ok = (storedType.GetNumberOfElements(0u) == value.GetNumberOfElements(0u));
            if (ok) {
                if (value.GetTypeDescriptor() == UnsignedInteger8Bit) {
                    if (scalarArrayPtr->getScalarArray()->getElementType() == epics::pvData::pvBoolean) {
                        ok = ReadBooleanArray(scalarArrayPtr, storedType, value);
                    }
                    else {
                        ok = ReadArray<uint8>(scalarArrayPtr, storedType, value);
                    }
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    ok = ReadArray<uint16>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    ok = ReadArray<uint32>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    ok = ReadArray<unsigned long int>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger8Bit) {
                    ok = ReadArray<int8>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger16Bit) {
                    ok = ReadArray<int16>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger32Bit) {
                    ok = ReadArray<int32>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger64Bit) {
                    ok = ReadArray<long int>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == Float32Bit) {
                    ok = ReadArray<float32>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor() == Float64Bit) {
                    ok = ReadArray<float64>(scalarArrayPtr, storedType, value);
                }
                else if (value.GetTypeDescriptor().type == SString) {
                    epics::pvData::shared_vector<const std::string> srcStr;
                    scalarArrayPtr->getAs<std::string>(srcStr);
                    uint32 numberOfElements = storedType.GetNumberOfElements(0u);
                    uint32 i;
                    Vector<StreamString> dst(static_cast<StreamString *>(value.GetDataPointer()), numberOfElements);
                    for (i = 0u; i < numberOfElements; i++) {
                        dst[i] = srcStr[i].c_str();
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
                    ok = false;
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Array dimensions must match %d != %d", storedType.GetNumberOfElements(0u),
                             value.GetNumberOfElements(0u));
                ok = false;
            }
        }
    }
    return ok;
}

bool EPICSPVAStructureDataI::ReadBooleanArray(epics::pvData::PVScalarArrayPtr scalarArrayPtr, AnyType &storedType, const AnyType &value) {
    bool ok = true;
    epics::pvData::shared_vector<const epics::pvData::boolean> out;
    scalarArrayPtr->getAs<epics::pvData::boolean>(out);
    uint32 numberOfElements = storedType.GetNumberOfElements(0u);
    uint32 i;
    Vector<uint8> readVec(reinterpret_cast<uint8 *>(value.GetDataPointer()), numberOfElements);
    Vector<bool> srcVec(const_cast<bool *>(reinterpret_cast<const bool *>(out.data())), numberOfElements);
    for (i = 0u; i < numberOfElements; i++) {
        readVec[i] = (srcVec[i] ? 1u : 0u);
    }

    return ok;
}

AnyType EPICSPVAStructureDataI::GetType(const char8 * const name) {
    AnyType at = voidAnyType;
    epics::pvData::PVFieldPtr fieldPtr;
    bool ok = structureFinalised;
    if (ok) {
        fieldPtr = currentStructPtr->getSubField(name);
        ok = (fieldPtr ? true : false);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "FinaliseStructure must be called before GetType().");
    }
    if (ok) {
        epics::pvData::Type epicsType = fieldPtr->getField()->getType();
        epics::pvData::ScalarType epicsScalarType;
        TypeDescriptor marte2Type;
        uint32 numberOfElements = 1u;
        if (epicsType == epics::pvData::scalar) {
            epics::pvData::PVScalarPtr scalarFieldPtr = std::dynamic_pointer_cast < epics::pvData::PVScalar
                    > (currentStructPtr->getSubField(name));
            ok = (scalarFieldPtr ? true : false);
            if (ok) {
                epicsScalarType = scalarFieldPtr->getScalar()->getScalarType();
            }
        }
        else if (epicsType == epics::pvData::scalarArray) {
            epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::dynamic_pointer_cast < epics::pvData::PVScalarArray
                    > (currentStructPtr->getSubField(name));
            ok = (scalarArrayPtr ? true : false);
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
            else if (epicsScalarType == epics::pvData::pvBoolean) {
                marte2Type = UnsignedInteger8Bit;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported EPICS type");
                ok = false;
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
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Parameter %s not found", name);
    }
    return at;
}

bool EPICSPVAStructureDataI::CreateFromStoredType(const char8 * const name, AnyType &storedType) {
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
    else if ((storedType.GetTypeDescriptor().type == CArray) || (storedType.GetTypeDescriptor().type == BT_CCString)
            || (storedType.GetTypeDescriptor().type == PCString) || (storedType.GetTypeDescriptor().type == SString)) {
        epicsType = epics::pvData::pvString;
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type for node with name %s", name);
        ok = false;
    }
    if (ok) {
        bool isScalar = (storedType.GetNumberOfElements(0u) <= 1u);
        if (isScalar) {
            fieldBuilder = fieldBuilder->add(name, epicsType);
        }
        else {
            fieldBuilder = fieldBuilder->addBoundedArray(name, epicsType, storedType.GetNumberOfElements(0u));
        }
    }
    return ok;
}

bool EPICSPVAStructureDataI::WriteStoredType(const char8 * const name, AnyType &storedType, const AnyType &value) {
    epics::pvData::PVScalarPtr scalarFieldPtr;
    epics::pvData::PVScalarArrayPtr scalarArrayPtr;

    bool isScalar = (value.GetNumberOfElements(0u) <= 1u);
    bool storedTypeIsScalar = (storedType.GetNumberOfElements(0u) <= 1u);

    bool ok = (isScalar == storedTypeIsScalar);
    if (ok) {
        if (storedTypeIsScalar) {
            scalarFieldPtr = std::dynamic_pointer_cast < epics::pvData::PVScalar > (currentStructPtr->getSubField(name));
            ok = (scalarFieldPtr ? true : false);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "%s should be a scalar but the conversion to PVScalar failed", name);
            }
        }
        else {
            scalarArrayPtr = std::dynamic_pointer_cast < epics::pvData::PVScalarArray > (currentStructPtr->getSubField(name));
            ok = (scalarArrayPtr ? true : false);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "%s should be an array but the conversion to PVScalarArray failed", name);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError,
                     "Attempted to write a scalar on a vector (or vice-versa) for attribute with name %s ", name);
    }
    if (ok) {
        if (storedTypeIsScalar) {
            if (value.GetTypeDescriptor() == UnsignedInteger8Bit) {
                scalarFieldPtr->putFrom<uint8>(*reinterpret_cast<uint8 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == UnsignedInteger16Bit) {
                scalarFieldPtr->putFrom<uint16>(*reinterpret_cast<uint16 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == UnsignedInteger32Bit) {
                scalarFieldPtr->putFrom<uint32>(*reinterpret_cast<uint32 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == UnsignedInteger64Bit) {
                scalarFieldPtr->putFrom<unsigned long int>(*reinterpret_cast<uint64 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == SignedInteger8Bit) {
                scalarFieldPtr->putFrom<int8>(*reinterpret_cast<int8 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == SignedInteger16Bit) {
                scalarFieldPtr->putFrom<int16>(*reinterpret_cast<int16 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == SignedInteger32Bit) {
                scalarFieldPtr->putFrom<int32>(*reinterpret_cast<int32 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == SignedInteger64Bit) {
                scalarFieldPtr->putFrom<long int>(*reinterpret_cast<int64 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == Float32Bit) {
                scalarFieldPtr->putFrom<float32>(*reinterpret_cast<float32 *>(value.GetDataPointer()));
            }
            else if (value.GetTypeDescriptor() == Float64Bit) {
                scalarFieldPtr->putFrom<float64>(*reinterpret_cast<float64 *>(value.GetDataPointer()));
            }
            else if ((value.GetTypeDescriptor().type == CArray) || (value.GetTypeDescriptor().type == BT_CCString)
                    || (value.GetTypeDescriptor().type == PCString) || (value.GetTypeDescriptor().type == SString)) {
                if (value.GetTypeDescriptor().type == SString) {
                    StreamString *src = static_cast<StreamString *>(value.GetDataPointer());
                    scalarFieldPtr->putFrom<std::string>(src->Buffer());
                }
                else {
                    char8 *src = static_cast<char8 *>(value.GetDataPointer());
                    scalarFieldPtr->putFrom<std::string>(std::string(src));
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
                if (value.GetTypeDescriptor() == UnsignedInteger8Bit) {
                    ok = WriteArray<uint8>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    ok = WriteArray<uint16>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    ok = WriteArray<uint32>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    ok = WriteArray<unsigned long int>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == SignedInteger8Bit) {
                    ok = WriteArray<int8>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == SignedInteger16Bit) {
                    ok = WriteArray<int16>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == SignedInteger32Bit) {
                    ok = WriteArray<int32>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == SignedInteger64Bit) {
                    ok = WriteArray<long int>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == Float32Bit) {
                    ok = WriteArray<float32>(scalarArrayPtr, storedType, value, size);
                }
                else if (value.GetTypeDescriptor() == Float64Bit) {
                    ok = WriteArray<float64>(scalarArrayPtr, storedType, value, size);
                }
                else if ((value.GetTypeDescriptor().type == CArray) || (value.GetTypeDescriptor().type == BT_CCString)
                        || (value.GetTypeDescriptor().type == PCString) || (value.GetTypeDescriptor().type == SString)) {
                    epics::pvData::shared_vector<const std::string> out;
                    out.resize(storedType.GetNumberOfElements(0u));
                    if (value.GetTypeDescriptor().type == SString) {
                        StreamString *src = static_cast<StreamString *>(value.GetDataPointer());
                        uint32 i;
                        for (i = 0; i < numberOfElements; i++) {
                            *const_cast<std::string *>(reinterpret_cast<const std::string *>(&out[i])) = src[i].Buffer();
                        }
                        scalarArrayPtr->putFrom<std::string>(out);
                    }
                    else {
                        const char8 **src = static_cast<const char8 **>(value.GetDataPointer());
                        uint32 i;
                        for (i = 0; i < numberOfElements; i++) {
                            *const_cast<std::string *>(reinterpret_cast<const std::string *>(&out[i])) = src[i];
                        }
                        scalarArrayPtr->putFrom<std::string>(out);
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Array dimensions must match %d != %d", storedType.GetNumberOfElements(0u),
                             value.GetNumberOfElements(0u));
            }
        }
    }
    return ok;
}

bool EPICSPVAStructureDataI::Write(const char8 * const name, const AnyType &value) {
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

bool EPICSPVAStructureDataI::Copy(StructuredDataI &destination) {
    bool ok = structureFinalised;
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "FinaliseStructure must be called before Copy().");
    }
    ReferenceT<ReferenceContainer> foundNode;
    uint32 nOfChildren = GetNumberOfChildren();
    for (uint32 i = 0u; (i < nOfChildren) && (ok); i++) {
        const char8 * const childName = GetChildName(i);
        const char8 * const childId = GetChildId(i);
        if (MoveRelative(childName)) {
            if (!destination.CreateRelative(childName)) {
                ok = false;
            }
            if (childId != NULL_PTR(const char8 *)) {
                ok = destination.Write("_PVANodeId", childId);
            }
            if (ok) {
                // go recursively !
                ok = Copy(destination);
            }
            if (ok) {
                ok = MoveToAncestor(1u);
            }
            if (ok) {
                ok = destination.MoveToAncestor(1u);
            }
        }
        else {
            AnyType at = GetType(childName);
            if (at.GetTypeDescriptor() != voidAnyType.GetTypeDescriptor()) {
                uint32 nOfElements0 = at.GetNumberOfElements(0u);
                uint32 nOfElements1 = at.GetNumberOfElements(1u);
                if (nOfElements0 < 1u) {
                    nOfElements0 = 1u;
                }
                if (nOfElements1 < 1u) {
                    nOfElements1 = 1u;
                }
                if ((at.GetTypeDescriptor().type == CArray) || (at.GetTypeDescriptor().type == BT_CCString)
                        || (at.GetTypeDescriptor().type == PCString) || (at.GetTypeDescriptor().type == SString)) {
                    if (nOfElements0 > 1u) {
                        Vector<StreamString> ss(nOfElements0);
                        ok = Read(childName, ss);
                        if (ok) {
                            ok = destination.Write(childName, ss);
                        }
                    }
                    else {
                        StreamString ss;
                        ok = Read(childName, ss);
                        if (ok) {
                            ok = ss.Seek(0LLU);
                        }
                        if (ok) {
                            ok = destination.Write(childName, ss);
                        }
                    }
                }
                else {
                    uint32 memSize = nOfElements0 * nOfElements1 * at.GetTypeDescriptor().numberOfBits / 8u;
                    char8 *mem = new char8[memSize];
                    at.SetDataPointer(&mem[0]);
                    ok = Read(childName, at);
                    if (ok) {
                        ok = destination.Write(childName, at);
                    }
                    delete [] mem;
                }
            }
        }
    }

    return ok;
}

bool EPICSPVAStructureDataI::AddToCurrentNode(Reference node) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.AddToCurrentNode(node);
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveToRoot() {
    bool ok = true;
    if (structureFinalised) {
        currentStructPtr = rootStructPtr;
    }
    else {
        ok = cachedCDB.MoveToRoot();
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveToAncestor(uint32 generations) {
    uint32 i;
    bool ok = true;
    if (structureFinalised) {
        epics::pvData::PVStructurePtr startStructPtr = currentStructPtr;
        for (i = 0u; (i < generations) && (ok); i++) {
            ok = (currentStructPtr != rootStructPtr);
            if (ok) {
                currentStructPtr = std::dynamic_pointer_cast < epics::pvData::PVStructure
                        > (currentStructPtr->getParent()->shared_from_this());
            }
        }
        if (!ok) {
            currentStructPtr = startStructPtr;
        }
    }
    else {
        ok = cachedCDB.MoveToAncestor(generations);
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveAbsolute(const char8 * const path) {
    epics::pvData::PVStructurePtr movePtr;
    bool ok = true;
    if (structureFinalised) {
        ok = MoveToRoot();
        if (ok) {
            movePtr = currentStructPtr->getSubField<epics::pvData::PVStructure>(path);
            //shared_ptr operator bool verifies the validity of the underlying ptr.
            ok = (movePtr ? true : false);
        }
        if (ok) {
            currentStructPtr = movePtr;
        }
    }
    else {
        ok = cachedCDB.MoveAbsolute(path);
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveRelative(const char8 * const path) {
    bool ok = true;
    if (structureFinalised) {
        epics::pvData::PVStructurePtr movePtr = currentStructPtr->getSubField<epics::pvData::PVStructure>(path);
        ok = (movePtr ? true : false);
        if (ok) {
            currentStructPtr = movePtr;
        }
    }
    else {
        ok = cachedCDB.MoveRelative(path);
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveToChild(const uint32 childIdx) {
    bool ok = true;
    if (structureFinalised) {
        const epics::pvData::PVFieldPtrArray & fields = currentStructPtr->getPVFields();
        epics::pvData::PVStructurePtr movePtr;
        ok = (childIdx < fields.size());
        if (ok) {
            epics::pvData::PVFieldPtr field = fields[childIdx];
            movePtr = std::dynamic_pointer_cast < epics::pvData::PVStructure > (field->shared_from_this());
            ok = (movePtr ? true : false);
        }
        if (ok) {
            currentStructPtr = movePtr;
        }
    }
    else {
        ok = cachedCDB.MoveToChild(childIdx);
    }
    return ok;
}

bool EPICSPVAStructureDataI::ConfigurationDataBaseToPVStructurePtr(ReferenceT<ReferenceContainer> currentNode, bool create) {
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
                StreamString leafName = foundLeaf->GetName();
                if (leafName == "_PVANodeId") {
                    StreamString nodeId = reinterpret_cast<const char8 *>(storedType.GetDataPointer());
                    fieldBuilder = fieldBuilder->setId(nodeId.Buffer());
                }
                else {
                    if (create) {
                        ok = CreateFromStoredType(leafName.Buffer(), storedType);
                    }
                    else {
                        ok = WriteStoredType(leafName.Buffer(), storedType, storedType);
                    }
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

bool EPICSPVAStructureDataI::CreateAbsolute(const char8 * const path) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.CreateAbsolute(path);
    }
    return ok;
}

bool EPICSPVAStructureDataI::CreateRelative(const char8 * const path) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.CreateRelative(path);
    }
    return ok;
}

bool EPICSPVAStructureDataI::Delete(const char8 * const name) {
    bool ok = !structureFinalised;
    if (ok) {
        ok = cachedCDB.Delete(name);
    }
    return ok;
}

const char8 *EPICSPVAStructureDataI::GetName() {
    const char8 *name = "";
    if (currentStructPtr) {
        name = currentStructPtr->getFieldName().c_str();
    }
    return name;
}

const char8 *EPICSPVAStructureDataI::GetChildName(const uint32 index) {
    const char8 * ret = "";
    if (currentStructPtr) {
        const epics::pvData::PVFieldPtrArray & fields = currentStructPtr->getPVFields();
        epics::pvData::PVFieldPtr movePtr;
        bool ok = (index < fields.size());
        if (ok) {
            epics::pvData::PVFieldPtr field = fields[index];
            movePtr = std::dynamic_pointer_cast < epics::pvData::PVField > (field->shared_from_this());
            ok = (movePtr ? true : false);
        }
        if (ok) {
            ret = movePtr->getFieldName().c_str();
        }
    }
    return ret;
}

const char8 *EPICSPVAStructureDataI::GetChildId(const uint32 index) {
    const char8 * ret = "";
    if (currentStructPtr) {
        const epics::pvData::PVFieldPtrArray & fields = currentStructPtr->getPVFields();
        epics::pvData::PVFieldPtr movePtr;
        bool ok = (index < fields.size());
        if (ok) {
            epics::pvData::PVFieldPtr field = fields[index];
            movePtr = std::dynamic_pointer_cast < epics::pvData::PVField > (field->shared_from_this());
            ok = (movePtr ? true : false);
        }
        if (ok) {
            ret = movePtr->getField()->getID().c_str();
        }
    }
    return ret;
}

uint32 EPICSPVAStructureDataI::GetNumberOfChildren() {
    uint32 numberOfFields = 0u;
    if (currentStructPtr) {
        numberOfFields = (currentStructPtr->getPVFields().size());
    }
    return numberOfFields;
}

void EPICSPVAStructureDataI::SetStructure(epics::pvData::PVStructure::shared_pointer const & structPtrToSet) {
    structureFinalised = true;
    currentStructPtr = structPtrToSet;
    rootStructPtr = structPtrToSet;
}

void EPICSPVAStructureDataI::InitStructure() {
    structureFinalised = false;
    fieldBuilder = epics::pvData::getFieldCreate()->createFieldBuilder();
}

bool EPICSPVAStructureDataI::FinaliseStructure() {
    bool ok = cachedCDB.MoveToRoot();
    ReferenceT<ReferenceContainer> rootNode = cachedCDB.GetCurrentNode();
    if (ok) {
        ok = ConfigurationDataBaseToPVStructurePtr(rootNode);
    }
    if (ok) {
        currentStructPtr = epics::pvData::getPVDataCreate()->createPVStructure(fieldBuilder->createStructure());
        rootStructPtr = currentStructPtr;
    }
    structureFinalised = ok;
    if (ok) {
        ok = ConfigurationDataBaseToPVStructurePtr(rootNode, false);
    }
    cachedCDB.Purge();
    return ok;
}

epics::pvData::PVStructurePtr EPICSPVAStructureDataI::GetRootStruct() {
    return rootStructPtr;
}

bool EPICSPVAStructureDataI::IsStructureFinalised() {
    return structureFinalised;
}

CLASS_REGISTER(EPICSPVAStructureDataI, "")
}
