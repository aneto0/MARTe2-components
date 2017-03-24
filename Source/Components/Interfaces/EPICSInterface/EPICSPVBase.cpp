/**
 * @file EPICSCAContext.cpp
 * @brief Source file for class EPICSCAContext
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
 * the class EPICSCAContext (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EPICSPVBase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

EPICSPVBase::EPICSPVBase() {
    context = NULL_PTR(struct ca_client_context *);
    timeout = 5.0F;
    pvName = "";
    pvChid = 0;

    modeValue = EPICS_EVENT_VALUE_IS_NOT_SET;
    nOfFunctionMaps = 0u;
    functionMap[0u] = NULL_PTR(StreamString *);
    functionMap[1u] = NULL_PTR(StreamString *);
}

EPICSPVBase::~EPICSPVBase() {
    if (functionMap[0u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[0u];
    }
    if (functionMap[1u] != NULL_PTR(StreamString *)) {
        delete[] functionMap[1u];
    }
}

bool EPICSPVBase::Initialise(StructuredDataI & data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = data.Read("PVName", pvName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "PVName must be specified");
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
            ok = data.Read("ModeValue", modeValueStr);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "ModeValue must be specified");
            }
            else {
                if (modeValueStr == "ValueIsFunction") {
                    modeValue = EPICS_LISTENER_VALUE_IS_FUNCTION;
                }
                else if (modeValueStr == "ValueIsParameter") {
                    modeValue = EPICS_LISTENER_VALUE_IS_PARAMETER;
                }
                else if (modeValueStr == "ValueIsIgnore") {
                    modeValue = EPICS_LISTENER_VALUE_IS_IGNORE;
                }
                else {
                    REPORT_ERROR(ErrorManagement::ParametersError, "ModeValue %s is not supported", modeValueStr.Buffer());
                }
            }
        }
        if (ok) {
            AnyType at = data.GetType("FunctionMap");
            if (!at.IsVoid()) {
                ok = (modeValue != EPICS_LISTENER_VALUE_IS_PARAMETER);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "FunctionMap can only be used with ModeValue=ValueIsFunction");
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
                                functionMap[0u][i] = functionMapMatrix[i][0u];
                                functionMap[1u][i] = functionMapMatrix[i][1u];
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
                ok = (nOfFunctionMaps != 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Either specify Function or FunctionMap, but not both at the same time");
                }
            }
        }
        if (ok) {
            if (modeValue == EPICS_EVENT_VALUE_IS_IGNORE) {
                ok = (function.Size() != 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "With ModeValue=ValueIsIgnore the Function must be specified");
                }
            }
            else if (modeValue == EPICS_EVENT_VALUE_IS_FUNCTION) {
                ok = (function.Size() != 0u);
                if (!ok) {
                    ok = (nOfFunctionMaps > 0u);
                }
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "With ModeValue=ValueIsFunction either the Function or the FunctionMap must be specified");
                }
            }
            else if (modeValue == EPICS_EVENT_VALUE_IS_PARAMETER) {
                ok = (function.Size() != 0u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "With ModeValue=ValueIsParameter the Function must be specified");
                }
            }
        }
    }
    return ok;
}

ErrorManagement::ErrorType EPICSPVBase::SetContext(struct ca_client_context * contextIn, float32 timeoutIn) {
    ErrorManagement::ErrorType err;
    context = contextIn;
    timeout = timeoutIn;
    if (ca_create_channel(pvName.Buffer(), NULL, NULL, 20, &pvChid) != ECA_NORMAL) {
        err = ErrorManagement::FatalError;
        REPORT_ERROR(err, "ca_create_channel failed");
    }
    return err;
}

}

