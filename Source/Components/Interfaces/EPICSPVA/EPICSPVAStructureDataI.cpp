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
#include "EPICSPVAHelper.h"
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
    currentStructPtr.resize(0);
    currentStructPtr.reserve(16u);
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
        ok = (currentStructPtr.size() > 0u);
    }
    if (ok) {
        epics::pvData::PVStructurePtr structPtr = currentStructPtr[currentStructPtr.size() - 1u];
        scalarFieldPtr = structPtr->getSubField<epics::pvData::PVScalar>(name);
        ok = (scalarFieldPtr ? true : false);
        if (!ok) {
            isScalar = false;
            scalarArrayPtr = structPtr->getSubField<epics::pvData::PVScalarArray>(name);
            ok = (scalarArrayPtr ? true : false);
        }
    }
    if (ok) {
        if (isScalar) {
            bool isBoolean = (scalarFieldPtr->getScalar()->getScalarType() == epics::pvData::pvBoolean);
            if (isBoolean) {
                ok = ReadValue<bool>(scalarFieldPtr, value);
            }
            else {
                if (value.GetTypeDescriptor() == UnsignedInteger8Bit) {
                    ok = ReadValue<uint8>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    ok = ReadValue<uint16>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    ok = ReadValue<uint32>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    ok = ReadValue<long unsigned int>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger8Bit) {
                    ok = ReadValue<int8>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger16Bit) {
                    ok = ReadValue<int16>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger32Bit) {
                    ok = ReadValue<int32>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == SignedInteger64Bit) {
                    ok = ReadValue<long int>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == Float32Bit) {
                    ok = ReadValue<float32>(scalarFieldPtr, value);
                }
                else if (value.GetTypeDescriptor() == Float64Bit) {
                    ok = ReadValue<float64>(scalarFieldPtr, value);
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
        }
        else {
            ok = (storedType.GetNumberOfElements(0u) == value.GetNumberOfElements(0u));
            if (ok) {
                bool isBoolean = (scalarArrayPtr->getScalarArray()->getElementType() == epics::pvData::pvBoolean);
                if (isBoolean) {
                    ok = ReadArray<bool>(scalarArrayPtr, storedType, value);
                }
                else {
                    if (value.GetTypeDescriptor() == UnsignedInteger8Bit) {
                        ok = ReadArray<uint8>(scalarArrayPtr, storedType, value);
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
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Array dimensions must match %d != %d", storedType.GetNumberOfElements(0u), value.GetNumberOfElements(0u));
                ok = false;
            }
        }
    }
    return ok;
}

AnyType EPICSPVAStructureDataI::GetType(const char8 * const name) {
    AnyType at = voidAnyType;
    epics::pvData::PVFieldPtr fieldPtr;
    bool ok = structureFinalised;
    epics::pvData::PVStructurePtr structPtr;
    if (ok) {
        ok = (currentStructPtr.size() > 0u);
    }
    if (ok) {
        structPtr = currentStructPtr[currentStructPtr.size() - 1u];
    }
    if (ok) {
        fieldPtr = structPtr->getSubField(name);
        ok = (fieldPtr ? true : false);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "FinaliseStructure must be called before GetType().");
    }
    if (ok) {
        epics::pvData::Type epicsType = fieldPtr->getField()->getType();
        epics::pvData::ScalarType epicsScalarType = epics::pvData::pvByte;
        TypeDescriptor marte2Type;
        uint32 numberOfElements = 1u;
        if (epicsType == epics::pvData::scalar) {
            epics::pvData::PVScalarPtr scalarFieldPtr = structPtr->getSubField<epics::pvData::PVScalar>(name);
            ok = (scalarFieldPtr ? true : false);
            if (ok) {
                epicsScalarType = scalarFieldPtr->getScalar()->getScalarType();
            }
        }
        else if (epicsType == epics::pvData::scalarArray) {
            epics::pvData::PVScalarArrayPtr scalarArrayPtr = structPtr->getSubField<epics::pvData::PVScalarArray>(name);
            ok = (scalarArrayPtr ? true : false);
            if (ok) {
                epicsScalarType = scalarArrayPtr->getScalarArray()->getElementType();
                numberOfElements = scalarArrayPtr->getLength();
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

bool EPICSPVAStructureDataI::WriteStoredType(const char8 * const name, AnyType &storedType, const AnyType &value) {
    epics::pvData::PVScalarPtr scalarFieldPtr;
    epics::pvData::PVScalarArrayPtr scalarArrayPtr;

    bool isScalar = (value.GetNumberOfElements(0u) <= 1u);
    bool storedTypeIsScalar = (storedType.GetNumberOfElements(0u) <= 1u);
    if (value.GetTypeDescriptor() == Character8Bit) {
        isScalar = (value.GetNumberOfElements(1u) <= 1u);
    }
    if (storedType.GetTypeDescriptor() == Character8Bit) {
        storedTypeIsScalar = (storedType.GetNumberOfElements(1u) <= 1u);
    }

    bool ok = (isScalar == storedTypeIsScalar);
    epics::pvData::PVStructurePtr structPtr;
    if (ok) {
        ok = (currentStructPtr.size() > 0u);
    }
    if (ok) {
        structPtr = currentStructPtr[currentStructPtr.size() - 1u];
    }
    if (ok) {
        if (storedTypeIsScalar) {
            scalarFieldPtr = std::dynamic_pointer_cast < epics::pvData::PVScalar > (structPtr->getSubField(name));
            ok = (scalarFieldPtr ? true : false);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "%s should be a scalar but the conversion to PVScalar failed", name);
            }
        }
        else {
            scalarArrayPtr = std::dynamic_pointer_cast < epics::pvData::PVScalarArray > (structPtr->getSubField(name));
            ok = (scalarArrayPtr ? true : false);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "%s should be an array but the conversion to PVScalarArray failed", name);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Attempted to write a scalar on a vector (or vice-versa) for attribute with name %s ", name);
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
            else if ((value.GetTypeDescriptor().type == CArray) || (value.GetTypeDescriptor().type == BT_CCString) || (value.GetTypeDescriptor().type == PCString)
                    || (value.GetTypeDescriptor().type == SString)) {
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
                else if ((value.GetTypeDescriptor().type == CArray) || (value.GetTypeDescriptor().type == BT_CCString) || (value.GetTypeDescriptor().type == PCString)
                        || (value.GetTypeDescriptor().type == SString)) {
                    ok = WriteArray<std::string>(scalarArrayPtr, storedType, value, size);
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Array dimensions must match %d != %d", storedType.GetNumberOfElements(0u), value.GetNumberOfElements(0u));
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
            if (at.GetTypeDescriptor() == Character8Bit) {
                if (nOfElements1 > 1u) {
                    Vector<StreamString> ss(nOfElements1);
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
            else if ((at.GetTypeDescriptor().type == BT_CCString) || (at.GetTypeDescriptor().type == PCString) || (at.GetTypeDescriptor().type == SString)) {
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
                delete[] mem;
            }
        }
        else {
            //Must be a node. Is is a structureArray?
            bool isArray = false;
            uint32 numberOfElements = 1u;
            epics::pvData::PVStructurePtr structPtr;
            if (ok) {
                ok = (currentStructPtr.size() > 0u);
            }
            if (ok) {
                structPtr = currentStructPtr[currentStructPtr.size() - 1u];
            }
            epics::pvData::PVStructureArrayPtr arrPtr;
            if (ok) {
                arrPtr = structPtr->getSubField<epics::pvData::PVStructureArray>(childName);
            }
            isArray = (arrPtr ? true : false);
            StreamString childIdStr;
            if (childId != NULL_PTR(const char8 * const)) {
                childIdStr = childId;
            }
            if (isArray) {
                numberOfElements = arrPtr->getLength();
                childIdStr = arrPtr->getField()->getID().c_str();
                (void) childIdStr.Seek(0LLU);
                //Remove any []
                StreamString token;
                char8 term;
                (void) childIdStr.GetToken(token, "[", term);
                childIdStr = token;
            }
            uint32 n;
            for (n = 0u; (n < numberOfElements) && (ok); n++) {
                StreamString fullChildName = childName;
                if (isArray) {
                    fullChildName.Printf("[%d]", n);
                }
                ok = MoveRelative(fullChildName.Buffer());
                if (ok) {
                    if (!destination.CreateRelative(fullChildName.Buffer())) {
                        ok = false;
                    }
                    if (childId != NULL_PTR(const char8 *)) {
                        ok = destination.Write("_PVANodeId", childIdStr.Buffer());
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
        currentStructPtr.resize(0u);
        currentStructPtr.push_back(rootStructPtr);
    }
    else {
        ok = cachedCDB.MoveToRoot();
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveToAncestor(uint32 generations) {
    bool ok = true;
    if (structureFinalised) {
        ok = (currentStructPtr.size() > generations);
        if (ok) {
            currentStructPtr.resize(currentStructPtr.size() - generations);
        }
    }
    else {
        ok = cachedCDB.MoveToAncestor(generations);
    }
    return ok;
}

bool EPICSPVAStructureDataI::Move(const char8 * const path) {
    epics::pvData::PVStructurePtr movePtr;
    epics::pvData::PVStructureArray::svector tempStructPtr;
    tempStructPtr = currentStructPtr;
    //Very important to resize. Otherwise there is a huge performance hit.
    tempStructPtr.reserve(16u);
    StreamString spath = path;
    StreamString token;
    char8 term;
    bool ok = (currentStructPtr.size() > 0u);
    if (ok) {
        movePtr = currentStructPtr[currentStructPtr.size() - 1u];
        ok = spath.Seek(0LLU);
    }
    while ((ok) && (spath.GetToken(token, ".[", term))) {
        int32 idx = -1;
        if (term == '[') {
            StreamString neStr;
            if (ok) {
                ok = spath.GetToken(neStr, "]", term);
            }
            idx = atoi(neStr.Buffer());
        }
        epics::pvData::PVFieldPtr field = movePtr->getSubField(token.Buffer());
        bool found = (field ? true : false);
        if (found) {
            //Cannot be a leaf!
            epics::pvData::Type pvType = field->getField()->getType();
            if (pvType == epics::pvData::structure) {
                ok = (idx == -1);
                if (ok) {
                    movePtr = std::dynamic_pointer_cast < epics::pvData::PVStructure > (field->shared_from_this());
                }
            }
            else if (pvType == epics::pvData::structureArray) {
                ok = (idx != -1);
                if (ok) {
                    epics::pvData::PVStructureArrayPtr arrPtr = std::dynamic_pointer_cast < epics::pvData::PVStructureArray > (field->shared_from_this());
                    movePtr = arrPtr->view()[idx];
                }
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "Trying to index a leaf.");
            }
            if (ok) {
                ok = (movePtr ? true : false);
                if (ok) {
                    tempStructPtr.push_back(movePtr);
                }
            }
        }
        if (ok) {
            ok = found;
        }
        token = "";
    }
    if (ok) {
        currentStructPtr = tempStructPtr;
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveAbsolute(const char8 * const path) {
    bool ok = true;
    if (structureFinalised) {
        ok = MoveToRoot();
        if (ok) {
            ok = Move(path);
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
        ok = Move(path);
    }
    else {
        ok = cachedCDB.MoveRelative(path);
    }
    return ok;
}

bool EPICSPVAStructureDataI::MoveToChild(const uint32 childIdx) {
    bool ok = true;
    if (structureFinalised) {
        ok = (currentStructPtr.size() > 0u);
        if (ok) {
            const epics::pvData::PVFieldPtrArray & fields = currentStructPtr[currentStructPtr.size() - 1u]->getPVFields();
            epics::pvData::PVStructurePtr movePtr;
            ok = (childIdx < fields.size());
            if (ok) {
                epics::pvData::PVFieldPtr field = fields[childIdx];
                movePtr = std::dynamic_pointer_cast < epics::pvData::PVStructure > (field->shared_from_this());
                ok = (movePtr ? true : false);
            }
            if (ok) {
                currentStructPtr.push_back(movePtr);
            }
        }
    }
    else {
        ok = cachedCDB.MoveToChild(childIdx);
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
    if (currentStructPtr.size() > 0) {
        name = currentStructPtr[currentStructPtr.size() - 1u]->getFieldName().c_str();
    }
    return name;
}

const char8 *EPICSPVAStructureDataI::GetChildName(const uint32 index) {
    const char8 * ret = "";
    if (currentStructPtr.size() > 0) {
        const epics::pvData::PVFieldPtrArray & fields = currentStructPtr[currentStructPtr.size() - 1u]->getPVFields();
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
    if (currentStructPtr.size() > 0) {
        const epics::pvData::PVFieldPtrArray & fields = currentStructPtr[currentStructPtr.size() - 1u]->getPVFields();
        bool ok = (index < fields.size());
        if (ok) {
            epics::pvData::PVFieldPtr field = fields[index];
            ret = field->getField()->getID().c_str();
        }
    }
    return ret;
}

uint32 EPICSPVAStructureDataI::GetNumberOfChildren() {
    uint32 numberOfFields = 0u;
    if (currentStructPtr.size() > 0) {
        numberOfFields = (currentStructPtr[currentStructPtr.size() - 1u]->getPVFields().size());
    }
    return numberOfFields;
}

void EPICSPVAStructureDataI::SetStructure(epics::pvData::PVStructurePtr const & structPtrToSet) {
    structureFinalised = true;
    rootStructPtr = structPtrToSet;
    currentStructPtr.resize(0u);
    currentStructPtr.push_back(rootStructPtr);
}

void EPICSPVAStructureDataI::InitStructure() {
    structureFinalised = false;
}

bool EPICSPVAStructureDataI::FinaliseStructure() {
    bool ok = cachedCDB.MoveToRoot();
    epics::pvData::StructureConstPtr topStructure;
    if (ok) {
        topStructure = EPICSPVAHelper::GetStructure(cachedCDB);
        ok = (topStructure ? true : false);
    }
    if (ok) {
        rootStructPtr = epics::pvData::getPVDataCreate()->createPVStructure(topStructure);
    }
    if (ok) {
        ok = EPICSPVAHelper::InitStructure(cachedCDB, rootStructPtr);
    }
    if (ok) {
        currentStructPtr.resize(0u);
        currentStructPtr.push_back(rootStructPtr);
    }
    structureFinalised = ok;
    if (ok) {
        ok = cachedCDB.MoveToRoot();
    }
    if (ok) {
        ok = MoveToRoot();
    }
    if (ok) {
        ok = CopyValuesFrom(cachedCDB);
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

bool EPICSPVAStructureDataI::CopyValuesFrom(StructuredDataI &source) {
    bool ok = structureFinalised;
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "FinaliseStructure must be called before Copy().");
    }
    ReferenceT<ReferenceContainer> foundNode;
    uint32 nOfChildren = GetNumberOfChildren();
    for (uint32 i = 0u; (i < nOfChildren) && (ok); i++) {
        const char8 * const childName = GetChildName(i);
        const char8 * const childId = GetChildId(i);
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
            if (at.GetTypeDescriptor() == Character8Bit) {
                if (nOfElements1 > 1u) {
                    Vector<StreamString> ss(nOfElements1);
                    ok = source.Read(childName, ss);
                    if (ok) {
                        ok = Write(childName, ss);
                    }
                }
                else {
                    StreamString ss;
                    ok = source.Read(childName, ss);
                    if (ok) {
                        ok = ss.Seek(0LLU);
                    }
                    if (ok) {
                        ok = Write(childName, ss);
                    }
                }
            }
            else if ((at.GetTypeDescriptor().type == BT_CCString) || (at.GetTypeDescriptor().type == PCString) || (at.GetTypeDescriptor().type == SString)) {
                bool isArray = (nOfElements0 > 1u);
                if (at.GetTypeDescriptor().type == SString) {
                    isArray = (nOfElements1 > 1u);
                }
                if (isArray) {
                    Vector<StreamString> ss(nOfElements0);
                    ok = source.Read(childName, ss);
                    if (ok) {
                        ok = Write(childName, ss);
                    }
                }
                else {
                    StreamString ss;
                    ok = source.Read(childName, ss);
                    if (ok) {
                        ok = ss.Seek(0LLU);
                    }
                    if (ok) {
                        ok = Write(childName, ss);
                    }
                }
            }
            else {
                uint32 memSize = nOfElements0 * nOfElements1 * at.GetTypeDescriptor().numberOfBits / 8u;
                char8 *mem = new char8[memSize];
                at.SetDataPointer(&mem[0]);
                ok = source.Read(childName, at);
                if (ok) {
                    ok = Write(childName, at);
                }
                delete[] mem;
            }
        }
        else {
            //Must be a node. Is is a structureArray?
            bool isArray = false;
            uint32 numberOfElements = 1u;
            epics::pvData::PVStructurePtr structPtr;
            if (ok) {
                ok = (currentStructPtr.size() > 0u);
            }
            if (ok) {
                structPtr = currentStructPtr[currentStructPtr.size() - 1u];
            }
            epics::pvData::PVStructureArrayPtr arrPtr;
            if (ok) {
                arrPtr = structPtr->getSubField<epics::pvData::PVStructureArray>(childName);
            }
            isArray = (arrPtr ? true : false);
            StreamString childIdStr;
            if (childId != NULL_PTR(const char8 * const)) {
                childIdStr = childId;
            }
            if (isArray) {
                numberOfElements = arrPtr->getLength();
                childIdStr = arrPtr->getField()->getID().c_str();
                (void) childIdStr.Seek(0LLU);
                //Remove any []
                StreamString token;
                char8 term;
                (void) childIdStr.GetToken(token, "[", term);
                childIdStr = token;
            }
            uint32 n;
            for (n = 0u; (n < numberOfElements) && (ok); n++) {
                StreamString fullChildName = childName;
                if (isArray) {
                    fullChildName.Printf("[%d]", n);
                }
                ok = MoveRelative(fullChildName.Buffer());
                if (ok) {
                    if (!source.MoveRelative(fullChildName.Buffer())) {
                        ok = false;
                    }
                    if (childId != NULL_PTR(const char8 *)) {
                        //ok = Write("_PVANodeId", childIdStr.Buffer());
                    }
                    if (ok) {
                        // go recursively !
                        ok = CopyValuesFrom(source);
                    }
                    if (ok) {
                        ok = MoveToAncestor(1u);
                    }
                    if (ok) {
                        ok = source.MoveToAncestor(1u);
                    }
                }
            }
        }
    }
    return ok;
}

bool EPICSPVAStructureDataI::ToString(StreamString &out) {
    bool ok = IsStructureFinalised();
    if (ok) {
        std::ostringstream stream;
        stream << GetRootStruct();
        out = stream.str().c_str();
    }
    return ok;
}


CLASS_REGISTER(EPICSPVAStructureDataI, "")
}
