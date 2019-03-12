/**
 * @file EPICSRPCClientMessageFilter.h
 * @brief Header file for class EPICSRPCClientMessageFilter
 * @date 25/06/2018
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

 * @details This header file contains the declaration of the class EPICSRPCClientMessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPVA_EPICSRPCCLIENTMESSAGEFILTER_H_
#define EPICSPVA_EPICSRPCCLIENTMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "MessageFilter.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief MessageFilter for the EPICSRPCClient.
 * @details Serialises the received Messages as PVStructures.
 *
 * The name of the Message is expected to be the destination to be set on the epics::pvAccess::RPCClient.
 *
 * The first element of the Message is expected to be a StructuredDataI with the structure to be serialised as a PVStructure.
 */
class EPICSRPCClientMessageFilter: public Object, public MessageFilter {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. Sets this filter as a permanent filter.
     */
    EPICSRPCClientMessageFilter();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EPICSRPCClientMessageFilter();

    /**
     * @brief Serialises the messageToTest as a PVStructure.
     * @details The name of the messageToTest is expected to be the epics::pvAccess::RPCClient message destination.
     * @param[in] messageToTest the Message to be serialised.
     * @return ErrorManagement::NoError if the Message could be successfully serialised as a PVStructure and sent with the epics::pvAccess::RPCClient.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

    /**
     * @brief Sets the RPCClient::request timeout.
     * @param[in] timeoutIn the timeout to set.
     */
    void SetTimeout(float64 timeoutIn);

    /**
     * @brief Gets the RPCClient::request timeout.
     * @return the RPCClient::request timeout.
     */
    uint32 GetTimeout();

private:
    /**
     * The timeout for the RPCClient::request.
    */
    float64 timeout;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVA_EPICSRPCCLIENTMESSAGEFILTER_H_ */
