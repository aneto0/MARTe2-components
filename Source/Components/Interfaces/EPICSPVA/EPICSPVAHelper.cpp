/**
 * @file EPICSPVAHelper.cpp
 * @brief Source file for class EPICSPVAHelper
 * @date 20/11/2018
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
 * the class EPICSPVAHelper (public, protected, and private). Be aware that some 
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
bool EPICSPVAHelperTransverseStructure(const Introspection *intro, epics::pvData::FieldBuilderPtr fieldBuilder) {
    bool ok = (intro != NULL_PTR(const Introspection *));
    if (ok) {
        uint32 numberOfMembers = intro->GetNumberOfMembers();
        uint32 i;
        //For each of the structure members...
        for (i = 0u; (i < numberOfMembers) && (ok); i++) {
            //lint -e{613} intro cannot be NULL as it is checked above.
            const IntrospectionEntry entry = intro->operator[](i);
            const char8 * const memberName = entry.GetMemberName();
            const char8 * const memberTypeName = entry.GetMemberTypeName();
            bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
            uint8 nOfDimensions = entry.GetNumberOfDimensions();
            if (isStructured) {
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
                ok = (cri != NULL_PTR(const ClassRegistryItem *));
                if (ok) {
                    bool isArray = (nOfDimensions > 0u);
                    if (isArray) {
                        fieldBuilder = fieldBuilder->addNestedStructureArray(memberName)->setId(memberTypeName);
                    }
                    else {
                        fieldBuilder = fieldBuilder->addNestedStructure(memberName)->setId(memberTypeName);
                    }
                    ok = EPICSPVAHelperTransverseStructure(cri->GetIntrospection(), fieldBuilder);
                    fieldBuilder = fieldBuilder->endNested();
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Type %s is not registered", memberTypeName);
                }
            }
            else {
                TypeDescriptor typeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(memberTypeName);
                epics::pvData::ScalarType epicsType;
                if (typeDesc == UnsignedInteger8Bit) {
                    epicsType = epics::pvData::pvUByte;
                }
                else if (typeDesc == UnsignedInteger16Bit) {
                    epicsType = epics::pvData::pvUShort;
                }
                else if (typeDesc == UnsignedInteger32Bit) {
                    epicsType = epics::pvData::pvUInt;
                }
                else if (typeDesc == UnsignedInteger64Bit) {
                    epicsType = epics::pvData::pvULong;
                }
                else if (typeDesc == SignedInteger8Bit) {
                    epicsType = epics::pvData::pvByte;
                }
                else if (typeDesc == SignedInteger16Bit) {
                    epicsType = epics::pvData::pvShort;
                }
                else if (typeDesc == SignedInteger32Bit) {
                    epicsType = epics::pvData::pvInt;
                }
                else if (typeDesc == SignedInteger64Bit) {
                    epicsType = epics::pvData::pvLong;
                }
                else if (typeDesc == Float32Bit) {
                    epicsType = epics::pvData::pvFloat;
                }
                else if (typeDesc == Float64Bit) {
                    epicsType = epics::pvData::pvDouble;
                }
                else if ((typeDesc.type == CArray) || (typeDesc.type == BT_CCString) || (typeDesc.type == PCString)
                        || (typeDesc.type == SString)) {
                    epicsType = epics::pvData::pvString;
                }
                else {
                    ok = false;
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported type %s", memberTypeName);
                }
                if (ok) {
                    if (nOfDimensions > 0u) {
                        uint32 numberOfElements = 0u;
                        uint32 ne;
                        for (ne = 0u; ne < nOfDimensions; ne++) {
                            numberOfElements += entry.GetNumberOfElements(ne);
                        }
                        fieldBuilder = fieldBuilder->addBoundedArray(memberName, epicsType, numberOfElements);
                    }
                    else {
                        fieldBuilder = fieldBuilder->add(memberName, epicsType);
                    }
                }
            }
        }
    }
    return ok;
}

bool EPICSPVAHelperTransverseStructureArraysInit(const Introspection *intro, epics::pvData::PVStructurePtr pvStructPtr,
                                           const char8 * const nodeName) {
    bool ok = (intro != NULL_PTR(const Introspection *));
    if (ok) {
        uint32 numberOfMembers = intro->GetNumberOfMembers();
        uint32 i;
        //For each of the structure members...
        for (i = 0u; (i < numberOfMembers) && (ok); i++) {
            //lint -e{613} intro cannot be NULL as it is checked above.
            const IntrospectionEntry entry = intro->operator[](i);
            const char8 * const memberName = entry.GetMemberName();
            const char8 * const memberTypeName = entry.GetMemberTypeName();
            StreamString fieldName = nodeName;
            if (fieldName.Size() > 0u) {
                fieldName += ".";
            }
            (void) fieldName.Printf("%s", memberName);
            uint8 nOfDimensions = entry.GetNumberOfDimensions();
            uint32 numberOfElements = 0u;
            bool isArray = (nOfDimensions > 0u);
            if (isArray) {
                uint32 ne;
                for (ne = 0u; ne < nOfDimensions; ne++) {
                    numberOfElements += entry.GetNumberOfElements(ne);
                }
            }
            bool isStructured = entry.GetMemberTypeDescriptor().isStructuredData;
            if (isStructured) {
                const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(memberTypeName);
                ok = (cri != NULL_PTR(const ClassRegistryItem *));
                if (ok) {
                    if (isArray) {
                        epics::pvData::PVStructureArrayPtr pvStructArr = std::dynamic_pointer_cast < epics::pvData::PVStructureArray
                                > (pvStructPtr->getSubField(fieldName.Buffer()));
                        ok = (pvStructArr ? true : false);
                        if (ok) {
                            pvStructArr->setCapacity(numberOfElements);
                            pvStructArr->append(numberOfElements);
                            pvStructArr->setCapacityMutable(false);
                        }
                    }
                    ok = EPICSPVAHelperTransverseStructureArraysInit(cri->GetIntrospection(), pvStructPtr, fieldName.Buffer());
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Type %s is not registered", memberTypeName);
                }
            }
        }
    }
    return ok;
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

epics::pvData::StructureConstPtr EPICSPVAHelper::GetStructure(const Introspection *intro, const char8 * const typeName) {
    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
    if (intro != NULL_PTR(const Introspection *)) {
        if (typeName != NULL_PTR(const char8 * const)) {
            fieldBuilder = fieldBuilder->setId(typeName);
        }
    }
    bool ok = EPICSPVAHelperTransverseStructure(intro, fieldBuilder);
    epics::pvData::StructureConstPtr topStructure;
    if (ok) {
        topStructure = fieldBuilder->createStructure();
    }
    return topStructure;
}

bool EPICSPVAHelper::InitPVStructureArrays(const Introspection *intro, epics::pvData::PVStructurePtr pvStructPtr,
                                     const char8 * const introFieldName) {
    return EPICSPVAHelperTransverseStructureArraysInit(intro, pvStructPtr, introFieldName);
}
}

