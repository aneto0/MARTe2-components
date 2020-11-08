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
/*lint -efile(766,EPICSPVA2V3Service.cpp) Message.h and EPICSPVARecord.h is used in this file.*/
#include "AdvancedErrorManagement.h"
#include "EPICSCAClient.h"
#include "EPICSPV.h"
#include "EPICSPVA2V3Service.h"
#include "EPICSPVAStructureDataI.h"
#include "Message.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPVA2V3Service::EPICSPVA2V3Service() :
        EPICSRPCService(), ReferenceContainer(), MessageI() {
    ReferenceContainer::AddBuildToken('_');
    seed = 0LLU;
    polynomial = 0x0u;
    crcInitialValue = 0x0u;
    crcFinalXOR = 0x0u;
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
    if (ok) {
        ok = data.Read("Polynomial", polynomial);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "Going to use polynomial 0x%x for CRC computations", polynomial);
            crc.ComputeTable(polynomial);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The CRC Polynomial shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("CRCInitialValue", crcInitialValue);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "Going to use the following CRC initial value 0x%x", crcInitialValue);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The CRCInitialValue shall be specified");
        }
    }
    if (ok) {
        ok = data.Read("CRCFinalXOR", crcFinalXOR);
        if (ok) {
            REPORT_ERROR(ErrorManagement::Information, "Going to use the following CRC final value 0x%x", crcFinalXOR);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "The CRCFinalXOR shall be specified");
        }
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
        epicsPVAStructure = ReferenceT<EPICSPVAStructureDataI>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        epicsPVAStructure->InitStructure();
        ok = RegisterPVs(structureContainer, caClient);
    }
    if (ok) {
        epicsPVAStructure->FinaliseStructure();
        epicsPVAStructure->MoveToRoot();
        REPORT_ERROR(ErrorManagement::Debug, "Structure created");
        std::cout << epicsPVAStructure->GetRootStruct() << std::endl;
    }
    if (ok) {
        caClient->SetName(GetName());
        ok = (caClient->Start() == ErrorManagement::NoError);
    }

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
            if (ok) {
                ok = epicsPVAStructure->Write(epicsPV->GetName(), epicsPV->GetAnyType());
            }
        }
        else {
            ReferenceT<ReferenceContainer> rcn = rc->Get(n);
            if (rcn.IsValid()) {
                ok = epicsPVAStructure->CreateRelative(rcn->GetName());
                if (ok) {
                    ok = RegisterPVs(rcn, caClient);
                }
                if (ok) {
                    ok = epicsPVAStructure->MoveToAncestor(1u);
                }
            }
        }
    }
    return ok;
}

bool EPICSPVA2V3Service::HandleLeaf(StreamString leafName, StructuredDataI &pvStruct, uint32 n, ReferenceT<EPICSPV> &pvNode) {
    //Check if there was a node defined with this name in the configured structure
    pvNode = structureContainer->Find(leafName.Buffer());
    bool ok = pvNode.IsValid();
    if (ok) {
        AnyType pv3AnyType = pvNode->GetAnyType();
        //Check if the type matches
        AnyType pvaAnyType = pvStruct.GetType(pvStruct.GetChildName(n));
        ok = (pvaAnyType.GetTypeDescriptor() == pv3AnyType.GetTypeDescriptor());
        if (!ok) {
            StreamString advertisedType = TypeDescriptor::GetTypeNameFromTypeDescriptor(pv3AnyType.GetTypeDescriptor());
            StreamString receivedType = TypeDescriptor::GetTypeNameFromTypeDescriptor(pvaAnyType.GetTypeDescriptor());
            REPORT_ERROR(ErrorManagement::CommunicationError, "Type mismatch between the advertised type (%s) and the received type (%s)", advertisedType.Buffer(), receivedType.Buffer());
        }
        if (ok) {
            ok = (pvaAnyType.GetNumberOfDimensions() == pv3AnyType.GetNumberOfDimensions());
            if (!ok) {
                uint32 advertisedDims = pv3AnyType.GetNumberOfDimensions();
                uint32 receivedDims = pvaAnyType.GetNumberOfDimensions();
                REPORT_ERROR(ErrorManagement::CommunicationError, "Dimensions mismatch between the advertised number (%d) and the received number (%d)", advertisedDims, receivedDims);
            }
        }
        if (ok) {
            uint32 j;
            for (j = 0u; (j < pvaAnyType.GetNumberOfDimensions()) && (ok); j++) {
                ok = (pvaAnyType.GetNumberOfElements(j) == pv3AnyType.GetNumberOfElements(j));
                if (!ok) {
                    uint32 advertisedElements = pv3AnyType.GetNumberOfElements(j);
                    uint32 receivedElements = pvaAnyType.GetNumberOfElements(j);
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Number of elements mismatch in direction %d between the advertised number (%d) and the received number (%d)", j, advertisedElements,
                                 receivedElements);
                }
            }
        }
        if (ok) {
            ok = pvStruct.Read(pvStruct.GetChildName(n), pv3AnyType);
        }

    }
    else {
        REPORT_ERROR(ErrorManagement::CommunicationError, "Node with name %s not found in the advertised structure", leafName.Buffer());
    }
    return ok;
}

bool EPICSPVA2V3Service::CAPutAll(StructuredDataI &pvStruct, StreamString currentNodeName, bool ignoreRootName) {
    bool ok = true;
    uint32 nChilds = pvStruct.GetNumberOfChildren();
    uint32 n;

    if (currentNodeName.Size() > 0LLU) {
        currentNodeName += ".";
    }
    if (!ignoreRootName) {
        currentNodeName += pvStruct.GetName();
    }
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
            if (leafName.Size() > 0u) {
                leafName += ".";
            }
            leafName += pvStruct.GetChildName(n);
            ReferenceT<EPICSPV> pvNode;
            ok = HandleLeaf(leafName, pvStruct, n, pvNode);
            if (ok) {
                ok = (pvNode->CAPutRaw() == ErrorManagement::NoError);
            }
        }
    }

    return ok;
}

bool EPICSPVA2V3Service::ComputeCRC(StructuredDataI &pvStruct, StreamString currentNodeName, uint32 &chksum, bool ignoreRootName) {
    bool ok = true;
    uint32 nChilds = pvStruct.GetNumberOfChildren();
    uint32 n;

    if (currentNodeName.Size() > 0LLU) {
        currentNodeName += ".";
    }
    if (!ignoreRootName) {
        currentNodeName += pvStruct.GetName();
    }
    for (n = 0u; (n < nChilds) && (ok); n++) {
        if (pvStruct.MoveToChild(n)) {
            ok = ComputeCRC(pvStruct, currentNodeName, chksum);
            if (ok) {
                ok = pvStruct.MoveToAncestor(1u);
            }
        }
        else {
            //We are at a leaf!
            StreamString leafName = currentNodeName;
            if (leafName.Size() > 0u) {
                leafName += ".";
            }
            leafName += pvStruct.GetChildName(n);
            ReferenceT<EPICSPV> pvNode;
            ok = HandleLeaf(leafName, pvStruct, n, pvNode);
            if (ok) {
                AnyType pv3AnyType = pvNode->GetAnyType();
                chksum = crc.Compute(reinterpret_cast<uint8 *>(pv3AnyType.GetDataPointer()), pvNode->GetMemorySize(), chksum, false);
            }
        }
    }

    return ok;
}

epics::pvData::PVStructurePtr EPICSPVA2V3Service::request(epics::pvData::PVStructure::shared_pointer const & args) {

    EPICSPVAStructureDataI config;
    config.InitStructure();
    config.SetStructure(args);
    std::ostringstream str;
    args->dumpValue(str);
    REPORT_ERROR(ErrorManagement::Debug, "Serving %s", str.str().c_str());

    EPICSPVAStructureDataI replyStructuredDataI;
    replyStructuredDataI.InitStructure();

    epics::pvData::PVStructurePtr reply;

    StreamString qualifier;
    bool ok = config.Read("qualifier", qualifier);
    if (ok) {
        if (qualifier == "read") {
            replyStructuredDataI.Write("value", *epicsPVAStructure.operator->());
        }
        else if (qualifier == "load") {
            uint32 clientHash;
            ok = config.Read("hash", clientHash);
            if (ok) {
                uint32 hash = crc.Compute(reinterpret_cast<uint8 *>(&seed), sizeof(uint32), crcInitialValue, false);
                ok = config.MoveRelative("value");
                if (ok) {
                    ok = ComputeCRC(config, "", hash, true);
                    if (ok) {
                        hash = hash ^ crcFinalXOR;
                        REPORT_ERROR(ErrorManagement::Debug, "Computed hash: %d", hash);
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "@load: value field was not specified");
                }
                if (ok) {
                    ok = (hash == clientHash);
                    if (ok) {
                        ok = config.MoveAbsolute("value");
                    }
                    if (ok) {
                        ok = CAPutAll(config, "", true);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "@load: hash mismatch %d != %d", hash, clientHash);
                    }
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "@load: hash field was not specified");
            }
        }
        else if (qualifier == "init") {
            seed = HighResolutionTimer::Counter32();
            replyStructuredDataI.Write("value", seed);
            REPORT_ERROR(ErrorManagement::Debug, "Retrieved seed: %d", seed);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Unrecognised qualifier string");
            ok = false;
        }
    }
    replyStructuredDataI.Write("status", ok);
    replyStructuredDataI.FinaliseStructure();
    reply = replyStructuredDataI.GetRootStruct();
    std::cout << "Replying << " << reply << std::endl;
    return reply;
}

void EPICSPVA2V3Service::Purge(ReferenceContainer &purgeList) {
    structureContainer = ReferenceT<ReferenceContainer>();
    ReferenceContainer::Purge(purgeList);
}


CLASS_REGISTER(EPICSPVA2V3Service, "1.0")
}
