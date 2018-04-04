/**
 * @file ConstantGAM.cpp
 * @brief Source file for class ConstantGAM
 * @date 08/03/2017
 * @author Bertrand Bauvir
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
 * the class ConstantGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "RegisteredMethodsMessageFilter.h"
#include "ConstantGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ConstantGAM::ConstantGAM() :
        GAM(), MessageI() {

    /* Message filter */
    ReferenceT<RegisteredMethodsMessageFilter> registeredMethodsMessageFilter("RegisteredMethodsMessageFilter");
    registeredMethodsMessageFilter->SetDestination(this);

    if (registeredMethodsMessageFilter.IsValid()) {
        InstallMessageFilter(registeredMethodsMessageFilter);
    }

}

ConstantGAM::~ConstantGAM() {

}

bool ConstantGAM::Setup() {

    bool ret = (GetNumberOfInputSignals() == 0u);

    if (!ret) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "GetNumberOfInputSignals() != 0u");
    }

    uint32 signalIndex;

    for (signalIndex = 0u; (signalIndex < GetNumberOfOutputSignals()) && (ret); signalIndex++) {

        StreamString signalName;

        if (ret) {
            ret = GetSignalName(OutputSignals, signalIndex, signalName);
        }

        uint32 signalByteSize = 0u;

        if (ret) {
            ret = GetSignalByteSize(OutputSignals, signalIndex, signalByteSize);
        }

        TypeDescriptor signalType = InvalidType;

        if (ret) {
            signalType = GetSignalType(OutputSignals, signalIndex);
            ret = (signalType != InvalidType);
        }

        StreamString signalTypeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType);

        if (ret) {
            REPORT_ERROR(ErrorManagement::Information, "Signal '%!' has type '%!'", signalName.Buffer(), signalTypeName.Buffer());
        }

        /* The type of the 'Default' value is string */
        /* Associate an AnyType to the signal memory area */

        if (ret) {
            ret = MoveToSignalIndex(OutputSignals, signalIndex);
        }

        AnyType signalDefType = configuredDatabase.GetType("Default");
        AnyType signalDefValue(signalType, 0u, GetOutputSignalMemory(signalIndex));

        uint32 signalNumberOfDimensions = signalDefType.GetNumberOfDimensions();

        if (ret) {
            signalDefValue.SetNumberOfDimensions(signalNumberOfDimensions);
        }

        uint32 dimensionIndex;

        for (dimensionIndex = 0u; ((dimensionIndex < signalNumberOfDimensions) && (ret)); dimensionIndex++) {
            uint32 dimensionNumberOfElements = signalDefType.GetNumberOfElements(dimensionIndex);
            signalDefValue.SetNumberOfElements(dimensionIndex, dimensionNumberOfElements);
        }

        if (ret) {
            ret = MoveToSignalIndex(OutputSignals, signalIndex);
        }

        if (ret) {
            //ret = GetSignalDefaultValue(OutputSignals, signalIndex, signalDefValue);
            ret = configuredDatabase.Read("Default", signalDefValue);
        }

        if (ret) {
            REPORT_ERROR(ErrorManagement::Information, "Signal '%!' has value '%!'", signalName.Buffer(), signalDefValue);
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "ConstantGAM::Setup - GetSignalDefaultValue '%s'", signalName.Buffer());
        }

    }

    return ret;
}

bool ConstantGAM::Execute() {
    return true;
}

ErrorManagement::ErrorType ConstantGAM::SetOutput(ReferenceContainer& message) {

    ErrorManagement::ErrorType ret = ErrorManagement::NoError;

    /* Assume one ReferenceT<StructuredDataI> contained in the message */

    bool ok = (message.Size() == 1u);
    ReferenceT<StructuredDataI> data = message.Get(0u);

    if (ok) {
        ok = data.IsValid();
    }

    if (!ok) {
        ret = ErrorManagement::ParametersError;
        REPORT_ERROR(ret, "Message does not contain a ReferenceT<StructuredDataI>");
    }

    StreamString signalName;

    if (ok) {
        ok = data->Read("SignalName", signalName);
    }

    uint32 signalIndex = 0u;

    if (ok) {
        ok = GetSignalIndex(OutputSignals, signalIndex, signalName.Buffer());
    }
    else {
	ok = data->Read("SignalIndex", signalIndex);
    }

    if (ok) {
        ok = (signalIndex < GetNumberOfOutputSignals());
    }

    if (!ok) {
        ret = ErrorManagement::ParametersError;
        REPORT_ERROR(ret, "No valid signal name or index provided");
    }
    
    TypeDescriptor signalType = InvalidType;

    if (ok) {
        signalType = GetSignalType(OutputSignals, signalIndex);
	ok = (signalType != InvalidType);
    }

    if (ok) {

        // To be removed
	StreamString signalTypeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(signalType);
	REPORT_ERROR(ErrorManagement::Information, "Signal '%!' has type '%!'", signalName.Buffer(), signalTypeName.Buffer());

        // Signal index and type are tested and valid ... go ahead with AnyType instantiation
	AnyType signalNewValue(signalType, 0u, GetOutputSignalMemory(signalIndex));

	// Use the default value type to query the signal properties (dimensions, ...)
        MoveToSignalIndex(OutputSignals, signalIndex);
	AnyType signalDefType = configuredDatabase.GetType("Default");

	uint32 signalNumberOfDimensions = signalDefType.GetNumberOfDimensions();
	signalNewValue.SetNumberOfDimensions(signalNumberOfDimensions);

	uint32 dimensionIndex;

	for (dimensionIndex = 0u; dimensionIndex < signalNumberOfDimensions; dimensionIndex++) {
	    uint32 dimensionNumberOfElements = signalDefType.GetNumberOfElements(dimensionIndex);
	    signalNewValue.SetNumberOfElements(dimensionIndex, dimensionNumberOfElements);
	}
 
	if (data->Read("SignalValue", signalNewValue)) {
            REPORT_ERROR(ErrorManagement::Information, "Signal '%!' new value '%!'", signalName.Buffer(), signalNewValue);
	}
	else {
	    ret = ErrorManagement::ParametersError;
	    REPORT_ERROR(ret, "Failed to read and apply new signal value");
	}

    }

    return ret;
}

CLASS_REGISTER(ConstantGAM, "1.0")

CLASS_METHOD_REGISTER(ConstantGAM, SetOutput)

} /* namespace MARTe */

