/**
 * @file EPICSPVARecord.cpp
 * @brief Source file for class EPICSPVARecord
 * @date 12/06/2018
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
 * the class EPICSPVARecord (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"
#include "pv/pvDatabase.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVAHelper.h"
#include "EPICSPVARecord.h"
#include "EPICSPVAStructureDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
//The PVRecord has some protected members... that need to be exposed
class epicsShareClass MARTe2PVARecord: public epics::pvDatabase::PVRecord {
public:
    //POINTER_DEFINITIONS(MARTe2PVARecord);
    MARTe2PVARecord(std::string const & recordName, epics::pvData::PVStructurePtr const & pvStructure) :
            epics::pvDatabase::PVRecord(recordName, pvStructure) {
    }

    void initPvt() {
        initPVRecord();
    }
};

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPVARecord::EPICSPVARecord() :
        Object() {

}

EPICSPVARecord::~EPICSPVARecord() {

}

bool EPICSPVARecord::GetArrayNumberOfElements(TypeDescriptor &typeDesc, StructuredDataI & cdb, uint32 &totalElements, uint8 &numberOfDimensions) {
    bool ok = true;
    uint32 *numberOfElements = NULL_PTR(uint32 *);
    AnyType arrayDescription = cdb.GetType("NumberOfElements");
    if (arrayDescription.GetDataPointer() == NULL_PTR(void *)) {
        numberOfElements = new uint32[1u];
        numberOfElements[0u] = 1u;
    }
    else {
        numberOfDimensions = arrayDescription.GetNumberOfElements(0u);
        if (numberOfDimensions < 2u) {
            numberOfElements = new uint32[1u];
            Vector<uint32> readVector(numberOfElements, numberOfDimensions);
            if (!cdb.Read("NumberOfElements", readVector)) {
                uint32 nElementsRead = 1u;
                ok = cdb.Read("NumberOfElements", nElementsRead);
                if (ok) {
                    if (nElementsRead > 1u) {
                        numberOfDimensions = 1u;
                    }
                    numberOfElements[0u] = nElementsRead;
                }
            }
            if (numberOfDimensions == 1u) {
                if (numberOfElements[0u] == 1u) {
                    numberOfDimensions = 0u;
                }
            }
        }
        else {
            numberOfElements = new uint32[numberOfDimensions];
            Vector<uint32> readVector(numberOfElements, numberOfDimensions);
            ok = cdb.Read("NumberOfElements", readVector);
        }
    }
    if (ok) {
        uint32 nd;
        if (typeDesc == Character8Bit) {
            if (numberOfDimensions > 0u) {
                numberOfDimensions -= 1u;
                numberOfElements[numberOfDimensions] = 1u;
            }
        }
        for (nd = 0u; nd < numberOfDimensions; nd++) {
            uint32 nde = numberOfElements[nd];
            if (nde != 0u) {
                if (totalElements == 0u) {
                    totalElements = 1u;
                }
                totalElements *= nde; //[1][1][1] has size 1x
            }
        }
        if (totalElements == 0u) {
            totalElements = numberOfElements[0U];
        }
        if (totalElements == 0u) {
            totalElements = 1u;
        }
    }
    if (numberOfElements != NULL_PTR(uint32 *)) {
        delete[] numberOfElements;
    }
    return ok;
}

bool EPICSPVARecord::GetEPICSStructure(epics::pvData::FieldBuilderPtr &fieldBuilder) {
    uint32 i;
    uint32 nOfChildren = cdb.GetNumberOfChildren();
    bool ok = true;

    for (i = 0u; (i < nOfChildren) && (ok); i++) {
        uint32 totalElements = 0u;
        uint8 numberOfDimensions = 0u;
        ok = cdb.MoveToChild(i);
        StreamString typeStr;
        if (ok) {
            ok = cdb.Read("Type", typeStr);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "A Type shall be defined for every node");
            }
        }
        TypeDescriptor typeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
        if (ok) {
            ok = GetArrayNumberOfElements(typeDesc, cdb, totalElements, numberOfDimensions);
        }
        if (typeDesc == InvalidType) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeStr.Buffer());
            const Introspection *intro = NULL_PTR(const Introspection *);
            ok = (cri != NULL_PTR(const ClassRegistryItem *));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not registered", typeStr.Buffer());
            }
            if (ok) {
                intro = cri->GetIntrospection();
                ok = (intro != NULL_PTR(const Introspection *));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type %s has no introspection", typeStr.Buffer());
                }
            }
            if (ok) {
                epics::pvData::StructureConstPtr memberStruct = EPICSPVAHelper::GetStructure(intro, typeStr.Buffer());
                if (totalElements > 1u) {
                    fieldBuilder = fieldBuilder->addArray(cdb.GetName(), memberStruct);
                }
                else {
                    fieldBuilder = fieldBuilder->add(cdb.GetName(), memberStruct);
                }
            }
        }
        else {
            epics::pvData::ScalarType epicsType;
            if (ok) {
                ok = (typeStr != "string");
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "For node %s use the type char8[]", cdb.GetName());
                }
            }
            if (ok) {
                ok = EPICSPVAHelper::GetType(typeStr.Buffer(), epicsType);
            }
            if (ok) {
                if (totalElements > 1u) {
                    //Do not allow for arrays of strings.
                    if ((epicsType == epics::pvData::pvString) && (numberOfDimensions < 2u)) {
                        fieldBuilder = fieldBuilder->add(cdb.GetName(), epicsType);
                        REPORT_ERROR(
                                ErrorManagement::Warning,
                                "Registering scalar %s with type %s - the number of elements is being ignored. If you wish to register an array of strings, declare it has an nxm matrix {n, m}, where n is the number of strings and m the size of the string ",
                                cdb.GetName(), typeStr.Buffer());
                    }
                    else {
                        fieldBuilder = fieldBuilder->addArray(cdb.GetName(), epicsType);
                        REPORT_ERROR(ErrorManagement::Debug, "Registering scalar array %s with type %s and %d elements", cdb.GetName(), typeStr.Buffer(), totalElements);
                    }
                }
                else {
                    fieldBuilder = fieldBuilder->add(cdb.GetName(), epicsType);
                    REPORT_ERROR(ErrorManagement::Debug, "Registering scalar %s with type %s", cdb.GetName(), typeStr.Buffer());
                }
            }
        }
        if (ok) {
            ok = cdb.MoveToAncestor(1u);
        }

    }
    return ok;
}

bool EPICSPVARecord::InitEPICSStructure(epics::pvData::PVStructurePtr pvStructure) {
    uint32 i;
    uint32 nOfChildren = cdb.GetNumberOfChildren();
    bool ok = true;

    for (i = 0u; (i < nOfChildren) && (ok); i++) {
        uint8 numberOfDimensions = 0u;
        uint32 totalElements = 0u;
        ok = cdb.MoveToChild(i);
        StreamString typeStr;
        if (ok) {
            ok = cdb.Read("Type", typeStr);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "A Type shall be defined for every node");
            }
        }
        TypeDescriptor typeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
        if (ok) {
            ok = GetArrayNumberOfElements(typeDesc, cdb, totalElements, numberOfDimensions);
        }
        if (typeDesc == InvalidType) {
            const ClassRegistryItem *cri = ClassRegistryDatabase::Instance()->Find(typeStr.Buffer());
            const Introspection *intro = NULL_PTR(const Introspection *);
            ok = (cri != NULL_PTR(const ClassRegistryItem *));
            if (!ok) {
                //Should not be reachable as it trapped by the GetEPICSStructure
                REPORT_ERROR(ErrorManagement::ParametersError, "Type %s is not registered", typeStr.Buffer());
            }
            if (ok) {
                intro = cri->GetIntrospection();
                ok = (intro != NULL_PTR(const Introspection *));
                if (!ok) {
                    //Should not be reachable as it trapped by the GetEPICSStructure
                    REPORT_ERROR(ErrorManagement::ParametersError, "Type %s has no introspection", typeStr.Buffer());
                }
            }
            if (ok) {
                if (totalElements > 1u) {
                    epics::pvData::PVStructureArrayPtr pvStructureMemberArray = pvStructure->getSubField<epics::pvData::PVStructureArray>(cdb.GetName());
                    ok = (pvStructureMemberArray ? true : false);
                    if (ok) {
                        ok = EPICSPVAHelper::ReplaceStructureArray(intro, pvStructureMemberArray, totalElements, typeStr.Buffer());
                    }
                }
                else {
                    epics::pvData::PVStructurePtr pvStructureMember = pvStructure->getSubField<epics::pvData::PVStructure>(cdb.GetName());
                    ok = (pvStructureMember ? true : false);
                    if (ok) {
                        ok = EPICSPVAHelper::InitStructure(intro, pvStructureMember);
                    }
                }
            }
        }
        else {
            if (totalElements > 1u) {
                epics::pvData::PVScalarArrayPtr pvScalarMemberArray = pvStructure->getSubField<epics::pvData::PVScalarArray>(cdb.GetName());
                ok = (pvScalarMemberArray ? true : false);
                if (ok) {
                    EPICSPVAHelper::InitArray(pvScalarMemberArray, totalElements);
                }
            }
        }
        if (ok) {
            ok = cdb.MoveToAncestor(1u);
        }

    }
    return ok;
}

bool EPICSPVARecord::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Copy(cdb);
    }
    if (ok) {
        recordName = "";
        if (!cdb.Read("Alias", recordName)) {
            recordName = GetName();
        }
        REPORT_ERROR(ErrorManagement::Information, "Going to use the following record name: [%s]", recordName.Buffer());
        ok = cdb.MoveRelative("Structure");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Structure defined");
        }
    }
    return ok;
}

epics::pvDatabase::PVRecordPtr EPICSPVARecord::CreatePVRecord() {
    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();
    epics::pvData::PVStructurePtr pvStructure;
    std::shared_ptr<MARTe2PVARecord> pvRecordWrapper;
    bool ok = GetEPICSStructure(fieldBuilder);
    if (ok) {
        ok = cdb.MoveAbsolute("Structure");
    }
    if (ok) {
        epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
        pvStructure = epics::pvData::getPVDataCreate()->createPVStructure(topStructure);
        ok = InitEPICSStructure(pvStructure);
    }
    if (ok) {
        pvRecordWrapper = std::shared_ptr < MARTe2PVARecord > (new MARTe2PVARecord(recordName.Buffer(), pvStructure));
        pvRecordWrapper->initPvt();
    }
    return pvRecordWrapper;

}

void EPICSPVARecord::GetRecordName(StreamString &recName) {
    recName = recordName;
}

CLASS_REGISTER(EPICSPVARecord, "1.0")
}
