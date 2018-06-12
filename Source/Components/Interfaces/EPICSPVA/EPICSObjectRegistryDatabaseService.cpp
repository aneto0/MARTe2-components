/**
 * @file EPICSObjectRegistryDatabaseService.cpp
 * @brief Source file for class EPICSObjectRegistryDatabaseService
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
 * the class EPICSObjectRegistryDatabaseService (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "pv/pvData.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSObjectRegistryDatabaseService.h"
#include "ObjectRegistryDatabase.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSObjectRegistryDatabaseService::EPICSObjectRegistryDatabaseService() :
        Object() {

}

EPICSObjectRegistryDatabaseService::~EPICSObjectRegistryDatabaseService() {

}

void EPICSObjectRegistryDatabaseService::GetEPICSStructure(epics::pvData::FieldBuilderPtr &fieldBuilder, ReferenceContainer &rc) {
    uint32 i;
    uint32 nOfChildren = rc.Size();
    for (i = 0u; i < nOfChildren; i++) {
        ReferenceT<Object> child = rc.Get(i);
        StreamString id;
        //Not sure if this is conceptually correct. In theory this method should only return the meta about the structure.
        //By encoding the name of the object as the name of the structure I'm actually mixing data with metadata.
        id.Printf("%s", child->GetName());
        fieldBuilder = fieldBuilder->addNestedStructure(id.Buffer());
        fieldBuilder = fieldBuilder->add("class", epics::pvData::pvString);
        ReferenceT<ReferenceContainer> childRC = child;
        if (childRC.IsValid()) {
            GetEPICSStructure(fieldBuilder, *childRC.operator ->());
        }
        fieldBuilder = fieldBuilder->endNested();
    }
}

void EPICSObjectRegistryDatabaseService::FillEPICSStructure(epics::pvData::PVStructurePtr & pvStructure, ReferenceContainer &rc, MARTe::StreamString fullName) {
    uint32 i;
    uint32 nOfChildren = rc.Size();
    for (i = 0u; i < nOfChildren; i++) {
        ReferenceT<Object> child = rc.Get(i);
        StreamString id;
        if (fullName.Size() > 0u) {
            id.Printf("%s.", fullName.Buffer());
        }
        id.Printf("%s", child->GetName());
        ReferenceT<ReferenceContainer> childRC = child;
        if (childRC.IsValid()) {
            FillEPICSStructure(pvStructure, *childRC.operator ->(), id.Buffer());
        }

        StreamString fieldClassName;
        fieldClassName.Printf("%s.class", id.Buffer());
        epics::pvData::PVStringPtr pvFieldClassName = pvStructure->getSubField<epics::pvData::PVString>(
                fieldClassName.Buffer());
        pvFieldClassName->put(child->GetClassProperties()->GetName());
    }
}

epics::pvData::PVStructurePtr EPICSObjectRegistryDatabaseService::request(epics::pvData::PVStructure::shared_pointer const & args)
        throw (epics::pvAccess::RPCRequestException) {
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    epics::pvData::FieldBuilderPtr fieldBuilder = epics::pvData::getFieldCreate()->createFieldBuilder();
    GetEPICSStructure(fieldBuilder, *ord);
    //fieldBuilder = fieldBuilder->add("name", epics::pvData::pvString);
    epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
    epics::pvData::PVStructurePtr result(epics::pvData::getPVDataCreate()->createPVStructure(topStructure));
    FillEPICSStructure(result, *ord, "");
    return result;
}

CLASS_REGISTER(EPICSObjectRegistryDatabaseService, "1.0")
}
