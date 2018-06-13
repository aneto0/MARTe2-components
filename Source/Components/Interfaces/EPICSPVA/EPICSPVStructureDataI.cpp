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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
EPICSPVStructureDataI::EPICSPVStructureDataI() {

}

EPICSPVStructureDataI::~EPICSPVStructureDataI() {

}

bool EPICSPVStructureDataI::Read(const char8 * const name, const AnyType &value) {
    bool ok = true;
    return ok;
}

AnyType EPICSPVStructureDataI::GetType(const char8 * const name) {
    AnyType at;
    epics::pvData::PVFieldPtr fieldPtr = currentStructPtr->getSubField(name);
    bool ok = (fieldPtr);
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
            epics::pvData::PVScalarArrayPtr scalarArrayPtr = std::tr1::dynamic_pointer_cast<epics::pvData::PVScalarArray>(currentStructPtr->getSubField(name));
            ok = (scalarArrayPtr);
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

bool EPICSPVStructureDataI::Write(const char8 * const name, const AnyType &value) {
    bool ok = true;
    return ok;
}

bool EPICSPVStructureDataI::Copy(StructuredDataI &destination) {
    bool ok = true;
    return ok;
}

bool EPICSPVStructureDataI::AddToCurrentNode(Reference node) {
    bool ok = true;
    return ok;
}

bool EPICSPVStructureDataI::MoveToRoot() {
    currentStructPtr = rootStructPtr;
    std::cout << "MoveToRoot>>>>>>>>>>>>>>>>" << std::endl;
    currentStructPtr->dumpValue(std::cout);
    std::cout << "MoveToRoot<<<<<<<<<<<<<<<<" << std::endl;
    return true;
}

bool EPICSPVStructureDataI::MoveToAncestor(uint32 generations) {
    uint32 i;
    bool ok = true;
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
    return ok;
}

bool EPICSPVStructureDataI::MoveAbsolute(const char8 * const path) {
    epics::pvData::PVStructurePtr movePtr;
    bool ok = MoveToRoot();
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
    return ok;
}

bool EPICSPVStructureDataI::MoveRelative(const char8 * const path) {
    epics::pvData::PVStructurePtr movePtr = currentStructPtr->getSubField<epics::pvData::PVStructure>(path);
    bool ok = (movePtr);
    if (ok) {
        currentStructPtr = movePtr;
    }
    std::cout << "MoveRelative>>>>>>>>>>>>>>>>" << std::endl;
    currentStructPtr->dumpValue(std::cout);
    std::cout << "MoveRelative<<<<<<<<<<<<<<<<" << std::endl;
    return ok;
}

bool EPICSPVStructureDataI::MoveToChild(const uint32 childIdx) {

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
    return ok;
}

bool EPICSPVStructureDataI::CreateAbsolute(const char8 * const path) {
    bool ok = true;
    return ok;
}

bool EPICSPVStructureDataI::CreateRelative(const char8 * const path) {
    bool ok = false;
    return ok;
}

bool EPICSPVStructureDataI::Delete(const char8 * const name) {
    bool ok = true;
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
    currentStructPtr = structPtrToSet;
    rootStructPtr = structPtrToSet;
}

void EPICSPVStructureDataI::InitStructure() {
    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
    epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
    epics::pvData::PVDataCreatePtr pvDataCreate = epics::pvData::getPVDataCreate();
    currentStructPtr = pvDataCreate->createPVStructure(topStructure);
}

CLASS_REGISTER(EPICSPVStructureDataI, "")
}
