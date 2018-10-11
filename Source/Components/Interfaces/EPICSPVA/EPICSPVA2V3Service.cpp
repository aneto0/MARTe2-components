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
//TODO PORT INTO core!
namespace ccs {

namespace HelperTools {

// Global variables

static MARTe::uint32 __table[256];
static bool __table_init = false;

// Function declaration

/**
 * @brief CRC operation
 * @detail This method implements a cyclic redundancy check on a sized byte array.
 * @return The CRC over the sized array.
 */

template<typename Type> inline void CyclicRedundancyCheck(const MARTe::uint8* buffer, const MARTe::uint32 size, Type &chksum);

// Function definition

template<> inline void CyclicRedundancyCheck(const MARTe::uint8* buffer, const MARTe::uint32 size, MARTe::uint32 &chksum) {
    using namespace MARTe;
    if (__builtin_expect((__table_init == false), 0)) {
        for (uint32 table_index = 0u; table_index < 256u; table_index += 1u) {
            uint32 seed = (uint32) table_index;

            for (uint32 bit_index = 0u; bit_index < 8u; bit_index += 1u) {
                if (seed & 1)
                    seed = 0xedb88320L ^ (seed >> 1);
                else
                    seed = seed >> 1;
            }

            __table[table_index] = seed;
        }

        __table_init = true;
    }

    for (uint32 index = 0u; index < size; index += 1u) {
        chksum = __table[(chksum ^ buffer[index]) & 0xff] ^ (chksum >> 8);
    }
}

} // namespace HelperTools

} // namespace ccs

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPVA2V3Service::EPICSPVA2V3Service() :
        EPICSRPCService(), ReferenceContainer(), MessageI() {
    ReferenceContainer::AddBuildToken('_');
    seed = 0LLU;
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
            leafName += ".";
            leafName += pvStruct.GetChildName(n);
            //Check if there was a node defined with this name in the configured structure
            ReferenceT<EPICSPV> pvNode = structureContainer->Find(leafName.Buffer());
            ok = pvNode.IsValid();
            if (ok) {
                //Check if the type matches
                AnyType pvaAnyType = pvStruct.GetType(pvStruct.GetChildName(n));
                AnyType pv3AnyType = pvNode->GetAnyType();

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
                            REPORT_ERROR(ErrorManagement::CommunicationError, "Number of elements mismatch in direction %d between the advertised number (%d) and the received number (%d)",
                                         advertisedElements, receivedElements);
                        }
                    }
                }
                if (ok) {
                    ok = pvStruct.Read(pvStruct.GetChildName(n), pv3AnyType);
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
            leafName += ".";
            leafName += pvStruct.GetChildName(n);
            //Check if there was a node defined with this name in the configured structure
            ReferenceT<EPICSPV> pvNode = structureContainer->Find(leafName.Buffer());
            ok = pvNode.IsValid();
            if (ok) {
                //Check if the type matches
                AnyType pvaAnyType = pvStruct.GetType(pvStruct.GetChildName(n));
                AnyType pv3AnyType = pvNode->GetAnyType();

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
                            REPORT_ERROR(ErrorManagement::CommunicationError, "Number of elements mismatch in direction %d between the advertised number (%d) and the received number (%d)",
                                         advertisedElements, receivedElements);
                        }
                    }
                }
                if (ok) {
                    ok = pvStruct.Read(pvStruct.GetChildName(n), pv3AnyType);
                }
                if (ok) {
                    ccs::HelperTools::CyclicRedundancyCheck(reinterpret_cast<uint8 *>(pv3AnyType.GetDataPointer()), pvNode->GetMemorySize(), chksum);
                    //ok = (pvNode->CAPutRaw() == ErrorManagement::NoError);
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::CommunicationError, "Node with name %s not found in the advertised structure", leafName.Buffer());
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
                uint32 hash = 0xfffffffful;
                ccs::HelperTools::CyclicRedundancyCheck(reinterpret_cast<uint8 *>(&seed), sizeof(uint32), hash);
                ok = config.MoveRelative("value");
                if (ok) {
                    ok = ComputeCRC(config, "", hash, true);
                    if (ok) {
                        hash = hash ^ 0xfffffffful;
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
        }
    }
    replyStructuredDataI.Write("status", ok);
    replyStructuredDataI.FinaliseStructure();
    reply = replyStructuredDataI.GetRootStruct();

    std::cout << reply << std::endl;
    return reply;
}
CLASS_REGISTER(EPICSPVA2V3Service, "1.0")
}
