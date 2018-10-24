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
#include "EPICSPVARecord.h"
#include "EPICSPVAStructureDataI.h"
#include "ObjectRegistryDatabase.h"
#include "StreamString.h"
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

bool EPICSPVARecord::InitEPICSStructure(StructuredDataI &pvaDB) {
    bool ok = true;
    uint32 nChildren = pvaDB.GetNumberOfChildren();
    uint32 n;
    for (n = 0u; (n < nChildren) && (ok); n++) {
        if (pvaDB.MoveToChild(n)) {
            ok = InitEPICSStructure(pvaDB);
            if (ok) {
                ok = pvaDB.MoveToAncestor(1u);
            }
        }
        else {
            const char8 * const name = pvaDB.GetChildName(n);
            AnyType type = pvaDB.GetType(name);
            bool isArray = (type.GetNumberOfDimensions() > 0u);
            if (isArray) {
                isArray = (type.GetNumberOfElements(0u) > 1u);
            }
            if (isArray) {
                uint32 numberOfElements = type.GetNumberOfElements(0u);
                if (type.GetTypeDescriptor() == UnsignedInteger8Bit) {
                    InitArray<uint8>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    InitArray<uint16>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    InitArray<uint32>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    InitArray<uint64>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger8Bit) {
                    InitArray<int8>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger16Bit) {
                    InitArray<int16>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger32Bit) {
                    InitArray<int32>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == SignedInteger64Bit) {
                    InitArray<int64>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == Float32Bit) {
                    InitArray<float32>(pvaDB, name, numberOfElements);
                }
                else if (type.GetTypeDescriptor() == Float64Bit) {
                    InitArray<float64>(pvaDB, name, numberOfElements);
                }
                else if ((type.GetTypeDescriptor().type == CArray) || (type.GetTypeDescriptor().type == BT_CCString) || (type.GetTypeDescriptor().type == PCString)
                        || (type.GetTypeDescriptor().type == SString)) {
                    Vector<StreamString> vec(numberOfElements);
                    uint32 n;
                    for (n = 0u; n < numberOfElements; n++) {
                        vec[n] = "";
                    }
                    ok = pvaDB.Write(name, vec);

                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type");
                }
            }
        }
    }
    return ok;
}

bool EPICSPVARecord::GetEPICSStructure(epics::pvData::FieldBuilderPtr &fieldBuilder) {
    uint32 i;
    uint32 nOfChildren = cdb.GetNumberOfChildren();
    bool ok = true;

    //If I cannot read the Type it shall be a node. Otherwise it might or not be a node (which contains other nodes but also a type)
    StreamString typeStr;
    if (cdb.Read("Type", typeStr)) {
        //Check if the number of elements is defined
        TypeDescriptor typeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(typeStr.Buffer());
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
        else {
            ok = false;
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported type %s", typeStr.Buffer());
        }
        if (ok) {
            uint32 numberOfElements = 1u;
            (void) cdb.Read("NumberOfElements", numberOfElements);
            if (numberOfElements > 1u) {
                fieldBuilder = fieldBuilder->addBoundedArray(cdb.GetName(), epicsType, numberOfElements);
            }
            else {
                fieldBuilder = fieldBuilder->add(cdb.GetName(), epicsType);
            }
        }
    }
    else {
        for (i = 0u; (i < nOfChildren) && (ok); i++) {
            const char8 * const nodeName = cdb.GetChildName(i);
            if (cdb.MoveRelative(nodeName)) {
                StreamString typeStr;
                bool hasType = (cdb.Read("Type", typeStr));
                if (!hasType) {
                    fieldBuilder = fieldBuilder->addNestedStructure(nodeName);
                }
                ok = GetEPICSStructure(fieldBuilder);
                cdb.MoveToAncestor(1);
                if (!hasType) {
                    fieldBuilder = fieldBuilder->endNested();
                }
            }
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
        epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
        pvStructure = epics::pvData::getPVDataCreate()->createPVStructure(topStructure);
        pvRecordWrapper = std::shared_ptr < MARTe2PVARecord > (new MARTe2PVARecord(recordName.Buffer(), pvStructure));
        pvRecordWrapper->initPvt();
    }
    if (ok) {
        EPICSPVAStructureDataI cdb;
        cdb.SetStructure(pvStructure);
        ok = cdb.MoveToRoot();
        if (ok) {
            ok = InitEPICSStructure(cdb);
        }
    }
    return pvRecordWrapper;

}

void EPICSPVARecord::GetRecordName(StreamString &recName) {
    recName = recordName;
}

CLASS_REGISTER(EPICSPVARecord, "1.0")
}
