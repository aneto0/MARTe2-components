/**
 * @file EPICSPV.cpp
 * @brief Source file for class EPICSPV
 * @date 04/02/2021
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
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
#include "CLASSMETHODREGISTER.h"
#include "ConfigurationDatabase.h"
#include "EPICSPV.h"
#include "RegisteredMethodsMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

EPICSPV::EPICSPV() : 
        ReferenceContainer(), MessageI() {
    context = NULL_PTR(struct ca_client_context *);
    timeout = 5.0F;
    pvName = "";
    pvChid = NULL_PTR(chid);
    pvType = DBR_LONG;

    eventMode.asUint8 = 0u;
    nOfFunctionMaps = 0u;
    numberOfElements = 1u;

    functionMap[0u] = NULL_PTR(StreamString *);
    functionMap[1u] = NULL_PTR(StreamString *);
    pvMemory = NULL_PTR(void *);
    memorySize = 0u;
    typeSize = 0u;
    changedPvVal = 0u;
    handlePVEventNthTime = 0u;

    ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
    if (!ret.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
    }
}

/*lint -e{1551} -e{1540} -e{1740} functionMap memory is freed in the destructor. The context, pvChid and pvMemory variables are managed (and thus freed) by the EPICS framework.*/
EPICSPV::~EPICSPV() {
    if (functionMap[0u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[0u];
    }
    if (functionMap[1u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[1u];
    }
    if (pvMemory != NULL_PTR(void *)) {
        if (pvAnyType.GetTypeDescriptor().type == SString) {
            if (numberOfElements > 1u) {
                StreamString *str = static_cast<StreamString *>(pvMemory);
                delete[] str;
            }
            else {
                StreamString *str = static_cast<StreamString *>(pvMemory);
                delete str;
            }
        }
        else {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(pvMemory);
        }
    }

}

bool EPICSPV::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    if (ok) {
        ok = data.Read("PVName", pvName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PVName must be specified");
        }
    }
    if (ok) {
        if (!data.Read("NumberOfElements", numberOfElements)) {
            REPORT_ERROR(ErrorManagement::Warning, "NumberOfElements not set. Using default of %d", numberOfElements);
        }
    }
    TypeDescriptor pvTypeDesc = VoidType;
    if (ok) {
        StreamString pvTypeStr;
        ok = data.Read("PVType", pvTypeStr);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PVType must be specified");
        }
        if (ok) {
            pvTypeDesc = TypeDescriptor::GetTypeDescriptorFromTypeName(pvTypeStr.Buffer());
            typeSize = (static_cast<uint32>(pvTypeDesc.numberOfBits) / 8u);
            memorySize = (typeSize * numberOfElements);
            if (pvTypeDesc == SignedInteger16Bit) {
                pvType = DBR_SHORT;
            }
            else if (pvTypeDesc == UnsignedInteger16Bit) {
                pvType = DBR_SHORT;
            }
            else if (pvTypeDesc == SignedInteger32Bit) {
                pvType = DBR_LONG;
            }
            else if (pvTypeDesc == UnsignedInteger32Bit) {
                pvType = DBR_LONG;
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
                if (pvType == DBR_STRING) {
                    if (numberOfElements > 1u) {
                        StreamString *str = new StreamString[numberOfElements];
                        pvMemory = static_cast<void *>(&str[0u]);
                        //lint -e{429} str is freed in the destructor
                    }
                    else {
                        StreamString *str = new StreamString();
                        pvMemory = static_cast<void *>(str);
                    }
                }
                else {
                    pvMemory = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(memorySize);
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
                else if (modeValueStr == "ParameterName") {
                    eventMode.parameterName = true;
                }
                else if (modeValueStr == "Ignore") {
                    eventMode.ignore = true;
                }
                else if (modeValueStr == "Message") {
                    eventMode.message = true;
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "PVValue %s is not supported", modeValueStr.Buffer());
                    ok = false;
                }
            }
        }
        if (ok) {
            if (!(eventMode.message.operator bool())) {
                ok = data.Read("Destination", destination);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Destination must be specified");
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
            bool isParameterName = eventMode.parameterName.operator bool();
            if ((isIgnore) || (isParameter) || (isParameterName)) {
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
    if (ok) {
        pvAnyType = AnyType(pvTypeDesc, 0u, pvMemory);
        if (numberOfElements > 1u) {
            pvAnyType.SetNumberOfDimensions(1u);
            pvAnyType.SetNumberOfElements(0u, numberOfElements);
        }
    }
    return ok;
}

ErrorManagement::ErrorType EPICSPV::CAPut(StructuredDataI & data) {
    if (pvAnyType.GetTypeDescriptor().type == SString) {
        StreamString *str = static_cast<StreamString *>(pvAnyType.GetDataPointer());
        if (numberOfElements > 1u) {
            uint32 n;
            for (n = 0u; n < numberOfElements; n++) {
                str[n] = "";
            }
        }
        else {
            *str = "";
        }
    }
    ErrorManagement::ErrorType err;
    err.parametersError = !data.Read("param1", pvAnyType);
    if (err.ErrorsCleared()) {
        err = CAPutRaw();
    }
    return err;
}

ErrorManagement::ErrorType EPICSPV::CAPutRaw() {
    ErrorManagement::ErrorType err = (context != NULL_PTR(struct ca_client_context *));
    if (err.ErrorsCleared()) {
        /*lint -e{9130} -e{835} -e{845} Several false positives. lint is getting confused here for some reason.*/
        err = !(ca_attach_context(context) == ECA_NORMAL);
    }
    if (err.ErrorsCleared()) {
        //Arrays of strings are encoded with space separated tokens!
        char8 *strArrayTemp = NULL_PTR(char8 *);
        void *mem = pvMemory;
        //StreamString has to be treated differently
        if (pvAnyType.GetTypeDescriptor().type == SString) {
            StreamString *str = static_cast<StreamString *>(pvAnyType.GetDataPointer());
            if (numberOfElements > 1u) {
                uint32 idx = static_cast<uint32>(MAX_STRING_SIZE) * numberOfElements;
                strArrayTemp = new char8[idx];
                //Arrays of strings are encoded as a single buffer of length 40 chars x numberOfDimensions
                uint32 n;
                bool ok = true;
                for (n = 0u; (n < numberOfElements) && (ok); n++) {
                    uint32 copySize = static_cast<uint32>(str[n].Size());
                    if (copySize > static_cast<uint32>(MAX_STRING_SIZE)) {
                        copySize = static_cast<uint32>(MAX_STRING_SIZE);
                    }
                    idx = n * static_cast<uint32>(MAX_STRING_SIZE);
                    void *dest = &strArrayTemp[idx];
                    ok = MemoryOperationsHelper::Copy(dest, str[n].Buffer(), copySize);
                }
                err = !ok;
                mem = static_cast<void *>(&strArrayTemp[0u]);
            }
            else {
                mem = const_cast<void *>(static_cast<const void *>(str->Buffer()));
            }
        }
        /*lint -e{9130} -e{835} -e{845} -e{747} Several false positives. lint is getting confused here for some reason.*/
        if (ca_array_put(pvType, numberOfElements, pvChid, mem) != ECA_NORMAL) {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "ca_put failed for PV: %s", pvName.Buffer());
        }
        /*lint -e{9130} -e{835} -e{845} Several false positives. lint is getting confused here for some reason.*/
        if (ca_pend_io(timeout) != ECA_NORMAL) {
            err = ErrorManagement::FatalError;
            REPORT_ERROR(err, "ca_pend_io failed for PV: %s", pvName.Buffer());
        }
        if (strArrayTemp != NULL_PTR(char8 *)) {
            delete[] strArrayTemp;
        }
        ca_detach_context();
    }
    return err;
}

void EPICSPV::HandlePVEvent(struct event_handler_args const & args) {
    const void * const dbr = args.dbr;
    if (dbr != NULL_PTR(const void *)) {
        if (pvMemory != NULL_PTR(char8 *)) {
            if (pvAnyType.GetTypeDescriptor().type == SString) {
                StreamString *str = static_cast<StreamString *>(pvAnyType.GetDataPointer());
                if (numberOfElements > 1u) {
                    //Arrays of strings are encoded as a single buffer of length 40 chars x numberOfDimensions
                    const char8 * tempStr = reinterpret_cast<const char8 *>(dbr);
                    uint32 n;
                    for (n = 0u; n < static_cast<uint32>(args.count); n++) {
                        uint32 idx = static_cast<uint32>(MAX_STRING_SIZE) * n;
                        str[n] = &tempStr[idx];
                    }
                }
                else {
                    *str = reinterpret_cast<const char8 *>(dbr);
                }
            }
            else {
                uint32 copySize = (static_cast<uint32>(args.count) * typeSize);
                (void) MemoryOperationsHelper::Copy(pvMemory, dbr, copySize);
            }
            if (!(eventMode.notSet.operator bool())) {
                TriggerEventMessage();
            }
        }
    }
}

void EPICSPV::TriggerEventMessage() {
    ConfigurationDatabase cdb;
    //if (handlePVEventNthTime == 0u) do not trigger an event so that we only react on value transitions.
    if (handlePVEventNthTime >= 1u) {
        bool ok = true;
        if (!eventMode.message.operator bool()) {
            ok = cdb.Write("Destination", destination.Buffer());

            if (eventMode.function.operator bool()) {
                if (nOfFunctionMaps == 0u) {
                    if (ok) {
                        ok = cdb.Write("Function", pvAnyType);
                    }
                }
                else {
                    StreamString newValue;
                    AnyType newValueAnyType(TypeDescriptor(false, SString, static_cast<uint16>(sizeof(StreamString) * 8u)), 0u, &newValue);
                    if (TypeConvert(newValueAnyType, pvAnyType)) {
                        StreamString functionMapValue = GetFunctionFromMap(newValue.Buffer());
                        if (functionMapValue.Size() > 0u) {
                            ok = cdb.Write("Function", functionMapValue.Buffer());
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "Could not find a mapping for key: %s", newValue.Buffer());
                            ok = false;
                        }
                    }
                }
            }
            else if (eventMode.ignore.operator bool()) {
                if (ok) {
                    ok = cdb.Write("Function", function.Buffer());
                }
            }
            else if (eventMode.parameterName.operator bool()) {
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
                    ok = cdb.Write("param1", GetName());
                }
                if (ok) {
                    ok = cdb.Write("param2", pvAnyType);
                }
                if (ok) {
                    ok = cdb.MoveToAncestor(1u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Could not create ConfigurationDatabase for message");
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
                    ok = cdb.Write("param1", pvAnyType);
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
                        StreamString val;
                        (void) val.Printf("%!", pvAnyType);
                        REPORT_ERROR(ErrorManagement::FatalError, "Could not send message to %s with value %s", destination.Buffer(), val.Buffer());
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Could not Initialise message");
                }
            }
        }
        else {
            uint32 numberOfMessages = Size();
            ReferenceT < Message > message;
            for (uint32 i = 0u; (i < numberOfMessages) && ok; i++) {
                message = Get(i);
                if (message.IsValid()) {
                    ReferenceT < ConfigurationDatabase > parameters = message->Get(0u);
                    if (parameters.IsValid()) {
                        uint32 numberOfParameters = parameters->GetNumberOfChildren();
                        for (uint32 j = 0u; (j < numberOfParameters) && ok; j++) {
                            StreamString childName = parameters->GetChildName(j);
                            if (handlePVEventNthTime >= 2u) {
                                if (((1ull << j) & changedPvVal) != 0u) {
                                    ok = parameters->Delete(childName.Buffer());
                                    if (ok) {
                                        ok = parameters->Write(childName.Buffer(), pvAnyType);
                                    }
                                }
                            }
                            else {
                                StreamString templ;
                                if (parameters->Read(childName.Buffer(), templ)) {
                                    if (templ == "$PVName") {
                                        ok = parameters->Delete(childName.Buffer());
                                        if (ok) {
                                            ok = parameters->Write(childName.Buffer(), GetName());
                                        }
                                    }
                                    else {
                                        if (templ == "$PVValue") {
                                            ok = parameters->Delete(childName.Buffer());
                                            if (ok) {
                                                ok = parameters->Write(childName.Buffer(), pvAnyType);
                                            }
                                            if (ok) {
                                                changedPvVal |= (1ull << j);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            StreamString messageDestination;
            messageDestination = message->GetDestination();
            StreamString val;
            (void) val.Printf("%!", pvAnyType);
            if (MessageI::SendMessage(message, this) != ErrorManagement::NoError) {
                REPORT_ERROR(ErrorManagement::FatalError, "Could not send message to %s with value %s", messageDestination.Buffer(), val.Buffer());
            }
            else{
                REPORT_ERROR(ErrorManagement::Information, "Sent message to %s with value %s", messageDestination.Buffer(), val.Buffer());
            }
        }
        if (handlePVEventNthTime > 2u) {
            handlePVEventNthTime = 2u;
        }
    }
    handlePVEventNthTime++;
}

/*lint -e{1762} function cannot be made const as it is registered as an RPC*/
ErrorManagement::ErrorType EPICSPV::CAGet(StructuredDataI & data) {
    ErrorManagement::ErrorType err = data.Write("param1", pvAnyType);
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

AnyType EPICSPV::GetAnyType() const {
    return pvAnyType;
}

EPICSPV::EventMode EPICSPV::GetMode() const {
    return eventMode;
}

float64 EPICSPV::GetCATimeout() const {
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

uint32 EPICSPV::GetMemorySize() const {
    return memorySize;
}

CLASS_REGISTER(EPICSPV, "1.0")
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(EPICSPV, CAPut)
/*lint -e{1023} There is no ambiguity on the function to be called as the compiler can distinguish between both template definitions.*/
CLASS_METHOD_REGISTER(EPICSPV, CAGet)

}
