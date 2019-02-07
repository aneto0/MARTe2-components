/**
 * @file EPICSPVA2V3Service.h
 * @brief Header file for class EPICSPVA2V3Service
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

 * @details This header file contains the declaration of the class EPICSPVA2V3Service
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVAMESSAGEI_H_
#define EPICSPVAMESSAGEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "pv/rpcService.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CRC.h"
#include "EPICSCAClient.h"
#include "EPICSPV.h"
#include "EPICSPVAStructureDataI.h"
#include "EPICSRPCService.h"
#include "MessageI.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Serialises the declared Structure into a flat list of EPICS V3 PVs.
 *
 * @details To be used with an EPICSRPCServer.
 *
 * The following hand-shaking protocol shall be implemented by the PVA service caller:
 * - If the request contains a "qualifier" field with the value "read", this component will reply with a "value" field that contains the PVStructure describing the Structure defined below;
 * - If the request contains a "qualifier" field with the value "init", this component will reply with a "value" field that contains a 32 bit seed;
 * - If the request contains a "qualifier" field with the value "load", this component will verify that:
 *   - there is an "hash" field that matches the CRC of a memory block that contains the seed (that was returned in the "init"), followed by all the values set in the Structure defined below;
 *   - if the "hashes" match the component will caput all the values of the Structure, defined inside the "value" field, into individual PV fields.
 * - The component will always reply with "status" field whose value can be either 0 or 1.
 *
 * The configuration syntax is  (names are only given as an example):
 * <pre>
 * +EPICSPVARPC = {
 *   Class = EPICSPVA::EPICSRPCServer
 *   StackSize = 1048576 //Optional the EmbeddedThread stack size. Default value is THREADS_DEFAULT_STACKSIZE * 4u
 *   CPUs = 0xff //Optional the affinity of the EmbeddedThread (where the EPICS context is attached).
 *   AutoStart = 0 //Optional. Default = 1. If false the service will only be started after receiving a Start message (see Start method).
 *   +Service1 = {
 *       Class = EPICSPVA2V3Service
 *       Polynomial = 0x12345678 //32 bit polynomial to use in the CRC computation
 *       CRCInitialValue = 0x0 //32 bit initial CRC value
 *       CRCFinalXOR = 0xFFFFFFFF //32 bit value to XOR against the computed CRC
 *       +Structure = { //Structure to be replied
 *           Class = ReferenceContainer
 *           +value1 = {//The leaf shall match against an existent EPICS3 PV.
 *               Class = EPICS::EPICSPV
 *               PVName = "MARTE2::EPICSPVA2V3::TEST::VALUE1"
 *               PVType = uint32
 *           }
 *           +value2 = {
 *               Class = EPICS::EPICSPV
 *               PVName = "MARTE2::EPICSPVA2V3::TEST::VALUE2"
 *               PVType = uint32
 *           }
 *       }
 *   }
 * }
 * </pre>
 *
 * Note that the seed will be generated after every "init", so that if more than one client use this component, there is the possibility that a "load" will be refused due to an unexpected seed.
 */
namespace MARTe {
class EPICSPVA2V3Service: public EPICSRPCService, public ReferenceContainer, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
EPICSPVA2V3Service    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSPVA2V3Service();

    /**
     * @brief Initialises the component against the rules and structure described above.
     * @param[in] data the object configuration.
     * @return true if the configuration respects the rules and structure described above.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Implements the protocol detailed in the class description.
     * @param[in] args shall contain one of the qualifiers described above.
     * @return a PVStructure with "status" field whose value can be either 0 or 1.
     */
    epics::pvData::PVStructurePtr request(epics::pvData::PVStructure::shared_pointer const & args);

    /**
     * @brief Unlink the structureContainer (which is also added as an Object to the ReferenceContainer).
     * @details See ReferenceContainer::Purge
     * @param[in] purgeList See ReferenceContainer::Purge
     */
    virtual void Purge(ReferenceContainer &purgeList);
private:

    /**
     * @brief Recursively add all the EPICS::EPICSPV found in the Structure leafs.
     * @param[in] rc the current structure branch.
     * @param[in] caClient the holder of the EPICS::EPICSPV.
     * @return true if all leafs contain a valid EPICS::EPICSPV.
     */
    bool RegisterPVs(ReferenceT<ReferenceContainer> rc, ReferenceT<EPICSCAClient> caClient);

    /**
     * @brief Recursively caputs all the registered EPICS::EPICSPV.
     * @param[in] pvStruct the structure declared during initialisation.
     * @param[in] currentNodeName the current being handled.
     * @param[in] ignoreRootName the first time it is called, do not use the current node name to compute the full variable name.
     * @return true if all variables are successfully caput.
     */
    bool CAPutAll(StructuredDataI &pvStruct, StreamString currentNodeName, bool ignoreRootName = false);

    /**
     * @brief Recursively computes the CRC based on all the registered EPICS::EPICSPV.
     * @param[in] pvStruct the structure declared during initialisation.
     * @param[in] currentNodeName the current being handled.
     * @param[in] ignoreRootName the first time it is called, do not use the current node name to compute the full variable name.
     * @return true if CRC can be successfully computed.
     */
    bool ComputeCRC(StructuredDataI &pvStruct, StreamString currentNodeName, uint32 &chksum, bool ignoreRootName = false);

    /**
     * @brief Retrieves the pvNode and pv3AnyType of a given leaf in the structure.
     * @param[in] leafName the name of the leaf (full path).
     * @param[in] pvStruct the Structure.
     * @param[in] n the child index of the node currently being handled.
     * @param[out] the pvNode assigned to the leaf.
     * @return true if the pvNode and the AnyType were successfully queried.
     */
    bool HandleLeaf(StreamString leafName, StructuredDataI &pvStruct, uint32 n, ReferenceT<EPICSPV> &pvNode);

    /**
     * The structure constructed during the initialisation.
     */
    ReferenceT<ReferenceContainer> structureContainer;

    /**
     * Cached version of structureContainer described as an EPICSPVAStructureDataI so that it can be immediately returned with the "read" request.
     */
    ReferenceT<EPICSPVAStructureDataI> epicsPVAStructure;

    /**
     * The seed computed @ init.
     */
    uint32 seed;

    /**
     * CRC calculator
     */
    CRC<uint32> crc;

    /**
     * The CRC polynomial
     */
    uint32 polynomial;

    /**
     * The CRC initial value
     */
    uint32 crcInitialValue;

    /**
     * The final XOR value.
     */
    uint32 crcFinalXOR;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVAMESSAGEI_H_ */
