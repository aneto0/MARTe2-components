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
#include "ConstantGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ConstantGAM::ConstantGAM() :
        GAM() {

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

	/* This is the basic element type */
	/* Handle the multi-dimensional case */
	
        AnyType signalDefValue (signalType, 0u, GetOutputSignalMemory(signalIndex));

	uint32 signalNumberOfDimensions = 0u;

	if (ret) {
	    ret = GetSignalNumberOfDimensions(OutputSignals, signalIndex, signalNumberOfDimensions);
	}

	if (ret) {
	    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "signalDefValue.SetNumberOfDimensions(%u)", signalNumberOfDimensions);
	    signalDefValue.SetNumberOfDimensions(signalNumberOfDimensions);
	}

	uint32 signalNumberOfElements = 0u;

	if (ret) {
	    ret = GetSignalNumberOfElements(OutputSignals, signalIndex, signalNumberOfElements);
	}

	if (ret) {
	    uint32 dimensionIndex;

	    for (dimensionIndex = 0u; dimensionIndex < signalNumberOfDimensions; dimensionIndex++) {
	        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "signalDefValue.SetNumberOfElements(%u, %u)", dimensionIndex, signalNumberOfElements);
	        signalDefValue.SetNumberOfElements(dimensionIndex, signalNumberOfElements);
	    }
	}

	if (ret) {
	    ret = GetSignalDefaultValue(OutputSignals, signalIndex, signalDefValue);
	}

	if (ret) {
	    REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "ConstantGAM::Setup - '%s' '%!'", signalName.Buffer(), signalDefValue);
	} else {
	    REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "ConstantGAM::Setup - GetSignalDefaultValue '%s'", signalName.Buffer());
	} 
	
    }

    return ret;
}

bool ConstantGAM::Execute() {
    return true;
}

CLASS_REGISTER(ConstantGAM, "1.0")

} /* namespace MARTe */

