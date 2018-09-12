/**
 * @file EPICSPVA2V3Service.cpp
 * @brief Source file for class EPICSPVA2V3Service
 * @date 18/06/2018
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
 * the class EPICSPVA2V3Service (public, protected, and private). Be aware that some
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
#include "EPICSCAClient.h"
#include "EPICSPV.h"
#include "EPICSPVA2V3Service.h"
#include "EPICSPVAStructureDataI.h"
#include "Message.h"
#include "ObjectRegistryDatabase.h"
#include "ReplyMessageCatcherMessageFilter.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPVA2V3Service::EPICSPVA2V3Service() :
        ReferenceContainer(), MessageI() {
    ReferenceContainer::AddBuildToken('_');
}

EPICSPVA2V3Service::~EPICSPVA2V3Service() {

}

bool EPICSPVA2V3Service::Initialise(StructuredDataI &data) {
    bool ok = ReferenceContainer::Initialise(data);
    ReferenceT<EPICSCAClient> caClient;
    if (ok) {
        caClient = ReferenceT<EPICSCAClient>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ok = Insert(caClient);
    }

    //Add all the PVs to the caClient
    if (ok) {
        structureContainer = Find("Structure");
        ok = structureContainer.IsValid();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No structure was specified");
        }
    }
    if (ok) {
        ok = RegisterPVs(structureContainer, caClient);
    }
    if (ok) {
        ok = (caClient->Start() == ErrorManagement::NoError);
    }
    ConfigurationDatabase cdb;
    float32 aaa[3];
    cdb.Write("A", aaa);
    StreamString b;
    cdb.Read("A", b);
    REPORT_ERROR(ErrorManagement::ParametersError, "specified %s", b.Buffer());

    return ok;
}

bool EPICSPVA2V3Service::RegisterPVs(ReferenceT<ReferenceContainer> rc, ReferenceT<EPICSCAClient> caClient) {
    bool ok = true;
    uint32 nChilds = rc->Size();
    uint32 n;
    for (n = 0u; (n < nChilds) && (ok); n++) {
        ReferenceT<EPICSPV> epicsPV = rc->Get(n);
        if (epicsPV.IsValid()) {
            ok = caClient->Insert(epicsPV);
        }
        else {
            ReferenceT<ReferenceContainer> rcn = rc->Get(n);
            if (rc.IsValid()) {
                ok = RegisterPVs(rcn, caClient);
            }
        }
    }

    return ok;
}

bool EPICSPVA2V3Service::CAPutAll(StructuredDataI &pvStruct, StreamString currentNodeName) {
    bool ok = true;
    uint32 nChilds = pvStruct.GetNumberOfChildren();
    uint32 n;

    if (currentNodeName.Size() > 0LLU) {
        currentNodeName += ".";
    }
    currentNodeName += pvStruct.GetName();
    for (n = 0u; (n < nChilds) && (ok); n++) {
        if (pvStruct.MoveToChild(n)) {
            ok = CAPutAll(pvStruct, currentNodeName);
            if (ok) {
                ok = pvStruct.MoveToAncestor(1u);
            }
        }
        else {
            //We are at a leaf!
            StreamString leafName = currentNodeName;
            leafName += ".";
            leafName += pvStruct.GetChildName(n);
            //Check if there was a node defined with this name in the configured structure
            ReferenceT<EPICSPV> pvNode = structureContainer->Find(leafName.Buffer());
            ok = pvNode.IsValid();
            if (ok) {
                //Check if the type matches
                AnyType at = pvStruct.GetType(pvStruct.GetChildName(n));
                ok = (at.GetTypeDescriptor() == pvNode->GetType());
                if (!ok) {
                    StreamString advertisedType = TypeDescriptor::GetTypeNameFromTypeDescriptor(pvNode->GetType());
                    StreamString receivedType = TypeDescriptor::GetTypeNameFromTypeDescriptor(at.GetTypeDescriptor());
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Type mismatch between the advertised structure (%s) and the received type (%s)", advertisedType.Buffer(), receivedType.Buffer());
                }
                //TODO implement array support
                if (ok) {
                //    ok = at.GetNumberOfDimensions() == pvNode->Get
                }
                if (ok) {
                    at.SetDataPointer(pvNode->GetMemory());
                    ok = pvStruct.Read(pvStruct.GetChildName(n), at);
                }
                if (ok) {
                    ok = (pvNode->CAPutRaw() == ErrorManagement::NoError);
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::CommunicationError, "Node with name %s not found in the advertised structure", leafName.Buffer());
            }
        }
    }

    return ok;
}

epics::pvData::PVStructurePtr EPICSPVA2V3Service::request(epics::pvData::PVStructure::shared_pointer const & args) throw (epics::pvAccess::RPCRequestException) {

    EPICSPVAStructureDataI config;
    config.InitStructure();
    config.SetStructure(args);

    bool ok = CAPutAll(config, "");
    if (ok) {
        //...
    }

    epics::pvData::FieldBuilderPtr fieldBuilder = epics::pvData::getFieldCreate()->createFieldBuilder();
    epics::pvData::StructureConstPtr topStructure = fieldBuilder->createStructure();
    epics::pvData::PVStructurePtr reply(epics::pvData::getPVDataCreate()->createPVStructure(topStructure));

    return reply;
}

CLASS_REGISTER(EPICSPVA2V3Service, "1.0")
}
