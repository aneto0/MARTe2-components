/**
 * @file EPICSPV.cpp
 * @brief Source file for class EPICSPV
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

 * @details This source file contains the definition of all the methods for
 * the class EPICSPV (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "../EPICS/EPICSPV.h"

#include "ConfigurationDatabase.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

EPICSPV::EPICSPV() :
        Object(), MessageI() {
    context = NULL_PTR(struct ca_client_context *);
    timeout = 5.0F;
    pvName = "";
    pvChid = NULL_PTR(chid);
    pvType = DBR_INT;

    pvTypeDesc = VoidType;

    eventMode.asUint8 = 0u;
    nOfFunctionMaps = 0u;

    functionMap[0u] = NULL_PTR(StreamString *);
    functionMap[1u] = NULL_PTR(StreamString *);
    pvMemory = NULL_PTR(char8 *);

    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551} -e{1540} functionMap memory is freed in the destructor. The context, pvChid and pvMemory variables are managed (and thus freed) by the EPICS framework.*/
EPICSPV::~EPICSPV() {
    if (functionMap[0u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[0u];
    }
    if (functionMap[1u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[1u];
    }
}

bool EPICSPV::Initialise(StructuredDataI & data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("PVName", pvName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PVName must be specified");
        }
    }
    if (ok) {
        StreamString pvTypeStr;
        ok = data.Read("PVType", pvTypeStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PVType must be specified");
        }
        if (ok) {
            pvTypeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(pvTypeStr.Buffer());
            uint64 memorySize = (static_cast<uint64>(pvTypeDesc.numberOfBits) / 8u);
            if (pvTypeDesc == SignedInteger32Bit) {
                pvType = DBR_INT;
            }
            else if (pvTypeDesc == UnsignedInteger32Bit) {
                pvType = DBR_INT;
            }
            else if (pvTypeDesc == Float32Bit) {
                pvType = DBR_FLOAT;
            }
            else if (pvTypeDesc == Float64Bit) {
                pvType = DBR_DOUBLE;
            }
            else if (pvTypeDesc == CharString) {
                pvType = DBR_STRING;
                pvTypeDesc = TypeDescriptor(false, SString, static_cast<uint16>(sizeof(StreamString) * 8u));
            }
            else {
                ok = false;
                REPORT_ERROR(ErrorManagement::ParametersError, "PVType %s is not supported", pvTypeStr.Buffer());
            }
            if (ok) {
                //Trick to recycle the memory of a string to easily store the other data types
                if (pvType == DBR_STRING) {
                    pvMemory = &pvMemoryStr;
                }
                else {
                    (void) pvMemoryStr.SetSize(memorySize);
                    pvMemory = reinterpret_cast<void *>(pvMemoryStr.BufferReference());
                }
            }
        }
    }
    if (ok) {
        if (!data.Read("Timeout", timeout)) {
            REPORT_ERROR(ErrorManagement::Warning, "ca_pend_io timeout not set. Using default of %f", timeout);
        }
        if (timeout < 1e-6) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Timeout shall be > 0s");
            ok = false;
        }
    }

    if (data.MoveRelative("Event")) {
        if (ok) {
            ok = data.Read("Destination", destination);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Destination must be specified");
            }
        }
        if (ok) {
            StreamString modeValueStr;
            ok = data.Read("PVValue", modeValueStr);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "PVValue must be specified");
            }
            else {
                if (modeValueStr == "Function") {
                    eventMode.function = true;
                }
                else if (modeValueStr == "Parameter") {
                    eventMode.parameter = true;
                }
                else if (modeValueStr == "Ignore") {
                    eventMode.ignore = true;
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "PVValue %s is not supported", modeValueStr.Buffer());
                    ok = false;
                }
            }
        }
        if (ok) {
            AnyType at = data.GetType("FunctionMap");
            if (!at.IsVoid()) {
                ok = (!(eventMode.parameter.operator bool()));
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "FunctionMap can only be used with PVValue=Function");
                }
                if (ok) {
                    ok = at.GetNumberOfDimensions() == 2u;
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "FunctionMap must be a matrix");
                    }
                }
                if (ok) {
                    nOfFunctionMaps = at.GetNumberOfElements(1u);
                    uint32 nCols = at.GetNumberOfElements(0u);
                    ok = (nCols == 2u);
                    if (ok) {
                        Matrix<StreamString> functionMapMatrix(nOfFunctionMaps, nCols);
                        ok = data.Read("FunctionMap", functionMapMatrix);
                        if (!ok) {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Could not read FunctionMap");
                        }
                        if (ok) {
                            functionMap[0u] = new StreamString[nOfFunctionMaps];
                            functionMap[1u] = new StreamString[nOfFunctionMaps];
                            uint32 i;
                            for (i = 0u; i < nOfFunctionMaps; i++) {
                                functionMap[0u][i] = functionMapMatrix(i, 0u);
                                functionMap[1u][i] = functionMapMatrix(i, 1u);
                            }
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "FunctionMap must be a matrix with two columns");
                    }
                }
            }
        }
        if (ok) {
            if (data.Read("Function", function)) {
                ok = (!eventMode.function.operator bool());
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "With PVValue=Function the Function to be called is the PV value. Remove this parameter. At most specify a FunctionMap");
                }
            }
        }
        if (ok) {
            //In these cases the Function parameter shall be specified
            bool isIgnore = eventMode.ignore.operator bool();
            bool isParameter = eventMode.parameter.operator bool();
            if ((isIgnore) || (isParameter)) {
                ok = (function.Size() != 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The Function parameter must be specified");
                }
            }
        }
        (void) data.MoveToAncestor(1u);
    }
    else {
        eventMode.notSet = true;
    }
    return ok;
}

ErrorManagement::ErrorType EPICSPV::CAPut(StructuredDataI &data) {
    ErrorManagement::ErrorType err = (context != NULL_PTR(struct ca_client_context *));
    if (err.ErrorsCleared()) {
        /*lint -e{9130} -e{835} -e{845} Several false positives. lint is getting confused here for some reason.*/
        err = !(ca_attach_context(context) == ECA_NORMAL);
    }
    if (err.ErrorsCleared()) {
        AnyType at(pvTypeDesc, 0u, pvMemory);

        //The StreamString memory has to be treated differently... and the Stream has to be reset.
        void *putMem = pvMemory;
        if (pvType == DBR_STRING) {
            pvMemoryStr = "";
            putMem = reinterpret_cast<void *>(pvMemoryStr.BufferReference());
        }
        err = !data.Read("param1", at);

        if (err.ErrorsCleared()) {
            /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
            if (ca_put(pvType, pvChid, putMem) != ECA_NORMAL) {
                err = ErrorManagement::FatalError;
                REPORT_ERROR(err, "ca_put failed for PV: %s", pvName.Buffer());
            }
            /*lint -e{9130} -e{835} -e{845} Several false positives. lint is getting confused here for some reason.*/
            if (ca_pend_io(timeout) != ECA_NORMAL) {
                err = ErrorManagement::FatalError;
                REPORT_ERROR(err, "ca_pend_io failed for PV: %s", pvName.Buffer());
            }
        }
        ca_detach_context();
    }
    return err;
}

void EPICSPV::HandlePVEvent(const void * const dbr) {
    if (dbr != NULL_PTR(const void *)) {
        if (pvMemory != NULL_PTR(char8 *)) {
            StreamString newValue;
            if (pvType == DBR_INT) {
                *(reinterpret_cast<uint32 *>(pvMemory)) = *(reinterpret_cast<const uint32 *>(dbr));
                (void) newValue.Printf("%d", *(reinterpret_cast<uint32 *>(pvMemory)));
            }
            else if (pvType == DBR_FLOAT) {
                *(reinterpret_cast<float32 *>(pvMemory)) = *(reinterpret_cast<const float32 *>(dbr));
                (void) newValue.Printf("%f", *(reinterpret_cast<float32 *>(pvMemory)));
            }
            else if (pvType == DBR_DOUBLE) {
                *(reinterpret_cast<float64 *>(pvMemory)) = *(reinterpret_cast<const float64 *>(dbr));
                (void) newValue.Printf("%f", *(reinterpret_cast<float64 *>(pvMemory)));
            }
            else if (pvType == DBR_STRING) {
                pvMemoryStr = reinterpret_cast<const char8 *>(dbr);
                newValue = pvMemoryStr;
            }
            else {
                //Should never enter here...
            }

            if (!(eventMode.notSet.operator bool())) {
                TriggerEventMessage(newValue);
            }
        }
    }
}

void EPICSPV::TriggerEventMessage(StreamString &newValue) {
    ConfigurationDatabase cdb;
    //firstTime do not trigger an event so that we only react on value transitions.
    static bool firstTime = true;
    if (!firstTime) {
        (void) newValue.Seek(0LLU);
        bool ok = cdb.Write("Destination", destination.Buffer());
        if (eventMode.function.operator bool()) {
            if (nOfFunctionMaps == 0u) {
                if (ok) {
                    ok = cdb.Write("Function", newValue.Buffer());
                }
            }
            else {
                StreamString functionMapValue = GetFunctionFromMap(newValue);
                if (functionMapValue.Size() > 0u) {
                    ok = cdb.Write("Function", functionMapValue.Buffer());
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Could not find a mapping for key: %s", newValue.Buffer());
                    ok = false;
                }
            }
        }
        else if (eventMode.ignore.operator bool()) {
            if (ok) {
                ok = cdb.Write("Function", function.Buffer());
            }
        }
        //Must be eventMode.parameter.operator bool()
        else {
            if (ok) {
                ok = cdb.Write("Function", function.Buffer());
            }
            if (ok) {
                ok = cdb.CreateAbsolute("+Parameters");
            }
            if (ok) {
                ok = cdb.Write("Class", "ConfigurationDatabase");
            }
            if (ok) {
                ok = cdb.Write("param1", newValue.Buffer());
            }
            if (ok) {
                ok = cdb.MoveToAncestor(1u);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not create ConfigurationDatabase for message");

            }
        }
        if (ok) {
            ReferenceT<Message> message(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ok = message->Initialise(cdb);
            if (ok) {
                if (MessageI::SendMessage(message, this) != ErrorManagement::NoError) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Could not send message to %s with value %s", destination.Buffer(), newValue.Buffer());
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not Initialise message");
            }
        }
    }
    firstTime = false;
}

/*lint -e{1762} function cannot be made const as it is registered as an RPC*/
ErrorManagement::ErrorType EPICSPV::CAGet(StructuredDataI &data) {
    AnyType at(pvTypeDesc, 0u, reinterpret_cast<const void * const >(pvMemory));
    ErrorManagement::ErrorType err = data.Write("param1", at);
    return err;
}

void EPICSPV::SetContext(struct ca_client_context * const contextIn) {
    context = contextIn;
}

const struct ca_client_context * EPICSPV::GetContext() const {
    return context;
}

StreamString EPICSPV::GetPVName() const {
    return pvName;
}

chid EPICSPV::GetPVChid() {
    return pvChid;
}

void EPICSPV::SetPVChid(const chid pvChidIn) {
    pvChid = pvChidIn;
}

evid EPICSPV::GetPVEvid() {
    return pvEvid;
}

void EPICSPV::SetPVEvid(const evid pvEvidIn) {
    pvEvid = pvEvidIn;
}

chtype EPICSPV::GetPVType() const {
    return pvType;
}

EPICSPV::EventMode EPICSPV::GetMode() const {
    return eventMode;
}

float64 EPICSPV::GetTimeout() const {
    return timeout;
}

StreamString EPICSPV::GetDestination() const {
    return destination;
}

StreamString EPICSPV::GetFunction() const {
    return function;
}

StreamString EPICSPV::GetFunctionFromMap(const StreamString &key) const {
    StreamString value;
    uint32 j;
    if (functionMap[0u] != NULL_PTR(StreamString*)) {
        if (functionMap[1u] != NULL_PTR(StreamString*)) {
            bool found = false;
            for (j = 0u; (j < nOfFunctionMaps) && (!found); j++) {
                found = (functionMap[0u][j] == key);
                if (found) {
                    value = functionMap[1u][j];
                }
            }
        }
    }
    return value;
}

CLASS_REGISTER(EPICSPV, "1.0")
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(EPICSPV, CAPut)
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(EPICSPV, CAGet)
}

