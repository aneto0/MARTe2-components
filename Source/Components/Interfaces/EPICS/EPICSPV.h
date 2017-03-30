/**
 * @file EPICSPV.h
 * @brief Header file for class EPICSPV
 * @date 23/03/2017
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

 * @details This header file contains the declaration of the class EPICSPV
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSPV_H_
#define EPICSPV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <cadef.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MessageI.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Describes an EPICS PV.
 * @details This class wraps an EPICS PV, allowing to caput and caget values from it.
 * This class is capable of triggering Messages as a response to a PV value change. The new PV value can
 * either be: 1) the name of the Function to be called; 2) or an ID of a Function to be called (see FunctionMap);
 *  3) or the input parameter of a pre-defined Function to be called; or 4) or a pre-defined Function is to be called with no parameters (i.e. the PV value is to be ignored).
 * The configuration syntax is (names are only given as an example):
 * +PV_1 = {
 *   Class = EPICSInterface::EPICSPV
 *   PVName = PV_ONE //Compulsory. Name of the EPICS PV.
 *   PVType = uint32 //Compulsory. The PV type. Supported types are uint32, int32, float32, float64 and string.
 *   Timeout = 5.0 //Optional but if set shall be > 0. The timeout for the ca_pend_io operations in seconds. Default value is 5.0 s.
 *   Event = { //Optional. Information about the message to be triggered every-time the EPICS PV value changes.
 *     Destination = StateMachine //Compulsory. Destination of the message.
 *     PVValue = Function //Compulsory. Can either be Function, Parameter or Ignore.
 *                        //If Function the PV value will be used as the name of the Function to call.
 *                        //If Parameter the PV value will be used as the parameter of the Function to call. This implies that the Function parameter must be set.
 *                        //If Ignore, the PV value will not be used and the Function will always be called.
 *     Function = STOP //Compulsory if PVValue=Parameter or PVValue=Ignore. Shall not be set if FunctionMap is defined or if PVValue=Function.
 *     FunctionMap = {{"1", "RUN"}, {"0", "STOP"}} //Optional Nx2 matrix. Only allowed if PVValue == Function. If defined then the PV value (first column of the matrix) will be used to map the Function name (second column of the matrix).
 *   }
 * }
 *
 * The CAPut and CAGet class methods are registered as call-backs. The parameter to put/get shall be encoded as "param1" in a StructuredDataI attached to the message.
 *
 * If the Event section is defined the Messages triggered will have the Function defined as above and the parameter (if set) will be written with the key "param1".
 */
class EPICSPV: public Object, public MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. Register the CAPut and CAGet function calls for RPC.
     * @post
     *   GetTimeout() == 5.0
     */
EPICSPV    ();

    /**
     * @brief Frees the FunctionMap memory if needed/
     */
    virtual ~EPICSPV();

    /**
     * @brief Calls Object::Initialise and reads the parameters defined in the class description.
     * @param[in] data see Object::Initialise.
     * @return true if Object::Initialise returns true and if the compulsory parameters are correctly set.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Stores the EPICS client context. It is used to set the context of the thread calling CAPut.
     * @param contextIn the EPICS client context to be stored.
     */
    void SetContext(struct ca_client_context * const contextIn);

    /**
     * @brief Gets the EPICS client context.
     * @return the EPICS client context.
     */
    const struct ca_client_context * GetContext() const;

    /**
     * @brief Triggered when this PV value changes.
     * @details The value of the PV will set to the \a dbr value. If the Event section (see class description) was defined
     * it will trigger the sending of a Message.
     * @param[in] dbr the new PV value.
     */
    void HandlePVEvent(const void *dbr);

    /**
     * @brief Gets the name of the PV.
     * @return the name of the PV.
     */
    StreamString GetPVName() const;

    /**
     * @brief Gets the EPICS chid of the PV.
     * @return the EPICS chid of the PV.
     */
    chid GetPVChid();

    /**
     * @brief Sets the EPICS chid of the PV.
     * @param[in] pvChidIn the EPICS chid of the PV.
     */
    void SetPVChid(const chid pvChidIn);

    /**
     * @brief Gets the EPICS evid of the PV.
     * @return the EPICS evid of the PV.
     */
    evid GetPVEvid();

    /**
     * @brief Sets the EPICS evid of the PV.
     * @param[in] pvChidIn the EPICS evid of the PV.
     */
    void SetPVEvid(const evid pvEvidIn);

    /**
     * @brief Gets the EPICS chtype of the PV.
     * @return the EPICS chtype of the PV.
     */
    chtype GetPVType() const;

    /**
     * @brief Defines the possible EventMode values (Function, Parameter, Ignore).
     */
    /*lint ++flb*/
    union EventMode {
        /**
         * Mode is not set
         */
        BitBoolean<uint8, 0u> notSet;

        /**
         * Value is to be used as the function name
         */
        BitBoolean<uint8, 1u> function;

        /**
         * Value is to be used as the function parameter
         */
        BitBoolean<uint8, 2u> parameter;

        /**
         * Value is to be ignored
         */
        BitBoolean<uint8, 3u> ignore;

        /**
         * Unmapped area
         */
        BitRange<uint8, 4u, 4u> unMapped;

        /**
         * Output as uint16
         */
        uint8 asUint8;
    };
    /*lint --flb*/

    /**
     * @brief Gets the Event mode set for this PV.
     * @return the Event mode set for this PV.
     */
    EventMode GetMode() const;

    /**
     * @brief Gets the ca_pend_io timeout.
     * @return the ca_pend_io timeout.
     */
    float64 GetTimeout() const;

    /**
     * @brief Gets the event message destination.
     * @return the event message destination.
     */
    StreamString GetDestination() const;

    /**
     * @brief Gets the event message destination Function (if set).
     * @return the event message destination Function.
     */
    StreamString GetFunction() const;

    /**
     * @brief Gets the event message destination Function from the FunctionMap.
     * @brief[key] the key to query.
     * @return the event message destination Function assigned to key or an empty StreamString if none was associated.
     */
    StreamString GetFunctionFromMap(const StreamString &key) const;

    /**
     * @brief Updates the value of the PV by calling an EPICS caput.
     * @details This function is registered with CLASS_METHOD_REGISTER and thus available for RPC.
     * @param[in] data shall contain a key named "param1" with the value to caput.
     * @return ErrorManagement::NoError if the value can be successfully caput (which implies that the low-level EPICS function return no Error).
     */
    ErrorManagement::ErrorType CAPut(StructuredDataI &data);

    /**
     * @brief Gets the latest value of the PV (which is updated by EPICS by calling the callback function HandlePVEvent) .
     * @details This function is registered with CLASS_METHOD_REGISTER and thus available for RPC.
     * @param[in] data will be updated with a key named "param1" where the value of the PV will be written into.
     * @return ErrorManagement::NoError if the value can be successfully written into "param1" (i.e. if data.Write succeeds).
     */
    ErrorManagement::ErrorType CAGet(StructuredDataI &data);

private:
    /**
     * @brief Triggers the sending of a Message with the rules defined in the class description.
     * @param[in] newValue the value to be sent (either as the Function name or the Function parameter).
     */
    void TriggerEventMessage(StreamString &newValue);

    /**
     * The EPICS client context.
     */
    struct ca_client_context * context;

    /**
     * The ca_pend_io timeout.
     */
    float64 timeout;

    /**
     * The name of the PV.
     */
    StreamString pvName;

    /**
     * The EPCIS PV chid.
     */
    chid pvChid;

    /**
     * The EPCIS PV evid.
     */
    evid pvEvid;

    /**
     * The EPCIS PV chtype.
     */
    chtype pvType;

    /**
     * The PV TypeDescriptor.
     */
    TypeDescriptor pvTypeDesc;

    /**
     * The name of the event message destination object.
     */
    StreamString destination;

    /**
     * The function to be called on the event destination object.
     */
    StreamString function;

    /**
     * Nx2 matrix where the first column contains the keys to be mapped and the second column the value to be associated to each key.
     */
    StreamString *functionMap[2];

    /**
     * Number of rows in functionMap.
     */
    uint32 nOfFunctionMaps;

    /**
     * The type of event to triggered.
     */
    EventMode eventMode;

    /**
     * The PV value is stored using the StreamString memory.
     */
    StreamString pvMemoryStr;

    /**
     * The memory of the PV (stored using the internal buffer of the pvMemoryStr StreamString)
     */
    void *pvMemory;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSPVCONTEXT_H_ */

