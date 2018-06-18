/**
 * @file EPICSPVRecord.cpp
 * @brief Source file for class EPICSPVRecord
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
 * the class EPICSPVRecord (public, protected, and private). Be aware that some
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
#include "EPICSPVRecord.h"
#include "ObjectRegistryDatabase.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
//The PVRecord has some protected members... that need to be exposed
class epicsShareClass MARTe2PVRecord: public epics::pvDatabase::PVRecord {
public:
    POINTER_DEFINITIONS(MARTe2PVRecord);
    MARTe2PVRecord(std::string const & recordName, epics::pvData::PVStructurePtr const & pvStructure) :
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

EPICSPVRecord::EPICSPVRecord() :
    Object() {

}

EPICSPVRecord::~EPICSPVRecord() {

}

bool EPICSPVRecord::GetEPICSStructure(epics::pvData::FieldBuilderPtr &fieldBuilder, ConfigurationDatabase &cdb) {
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
                fieldBuilder = fieldBuilder->addFixedArray("value", epicsType, numberOfElements);
            }
            else {
                fieldBuilder = fieldBuilder->add("value", epicsType);
            }
        }

        for (i = 0u; (i < nOfChildren) && (ok); i++) {
            const char8 * const nodeName = cdb.GetChildName(i);
            if (cdb.MoveRelative(nodeName)) {
                fieldBuilder = fieldBuilder->addNestedStructure(nodeName);
                ok = GetEPICSStructure(fieldBuilder, cdb);
                cdb.MoveToAncestor(1);
                fieldBuilder = fieldBuilder->endNested();
            }
        }
    }

    return ok;
}

bool EPICSPVRecord::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        cdb = dynamic_cast<ConfigurationDatabase &>(data);
    }
    if (ok) {
        ok = cdb.MoveRelative("Structure");
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No Structure defined");
        }
    }
    return ok;
}

bool EPICSPVRecord::CreatePVRecord(epics::pvDatabase::PVRecordPtr &pvRecordPtr) {
    epics::pvData::FieldCreatePtr fieldCreate = epics::pvData::getFieldCreate();
    epics::pvData::FieldBuilderPtr fieldBuilder = fieldCreate->createFieldBuilder();

    bool ok = GetEPICSStructure(fieldBuilder, cdb);
    if (ok) {
        epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
        topStructure->dump(std::cout);
        epics::pvData::PVStructurePtr pvStructure = epics::pvData::getPVDataCreate()->createPVStructure(topStructure);
        std::tr1::shared_ptr<MARTe2PVRecord> pvRecordWrapper = std::tr1::shared_ptr<MARTe2PVRecord>(new MARTe2PVRecord(GetName(), pvStructure));
        pvRecordWrapper->initPvt();
        pvRecordPtr = pvRecordWrapper;
    }
    return ok;

}

CLASS_REGISTER(EPICSPVRecord, "1.0")
}
