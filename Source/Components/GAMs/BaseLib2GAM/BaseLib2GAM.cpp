/**
 * @file BaseLib2GAM.cpp
 * @brief Source file for class BaseLib2GAM
 * @date 2/12/2016
 * @author Andre' Neto
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
 * the class BaseLib2GAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
//Otherwise lint may stop here when the target system does not have this module
#ifdef EFDA_MARTe_DIR
#include "GAMAdapter.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BaseLib2GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

BaseLib2GAM::BaseLib2GAM() :
        GAM() {
    gamIdx = 0u;
    inputToGAM = NULL_PTR(void *);
    outputFromGAM = NULL_PTR(void *);
    inputToGAMByteSize = 0u;
    outputFromGAMByteSize = 0u;
}

/*lint -e{1551} -e{1540} the destructor must guarantee that the GAMAdapter is removed from the GlobalObjectDatabase of BaseLib2.
 * The BaseLib2::GAMAdapter * is a singleton freed when the application terminates.*/
BaseLib2GAM::~BaseLib2GAM() {
    BaseLib2::GAMAdapter *adapter = BaseLib2::GAMAdapter::Instance();
    if(!adapter->RemoveGAM(gamIdx)) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Failed to remove GAM from adapter.");
    }
}

bool BaseLib2GAM::Initialise(StructuredDataI & data) {
    bool ok = GAM::Initialise(data);
    StreamString baseLib2Config;
    StreamString gamName;

    if (ok) {
        ok = data.Read("GAMName", gamName);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The GAMName parameter shall be specified.");
        }
    }
    if (ok) {
        ok = data.Read("GAMFunctionNumber", gamFunctionNumber);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The GAMFunctionNumber parameter shall be specified.");
        }
    }
    if (ok) {
         ok = data.Read("BaseLib2Config", baseLib2Config);
         if (!ok) {
             REPORT_ERROR(ErrorManagement::ParametersError, "The BaseLib2Config parameter shall be specified.");
         }
    }
    if (ok) {
        ok = baseLib2Config.Seek(0LLU);
    }
    BaseLib2::GAMAdapter *adapter = BaseLib2::GAMAdapter::Instance();
    if (ok) {
        ok = adapter->AddGAM(gamName.Buffer(), baseLib2Config.Buffer(), gamIdx);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to add GAM to Adapter.");
        }
    }
    return ok;
}

bool BaseLib2GAM::Setup() {
    BaseLib2::GAMAdapter *adapter = BaseLib2::GAMAdapter::Instance();
    uint32 numberOfInputs = GetNumberOfInputSignals();
    uint32 numberOfOutputs = GetNumberOfInputSignals();

    uint32 i;
    bool ok = true;
    for (i = 0u; (i < numberOfInputs) && (ok); i++) {
        StreamString signalName;
        StreamString signalTypeName;
        ok = GetSignalName(InputSignals, i, signalName);
        TypeDescriptor desc;
        uint32 numberOfElements = 1u;
        uint32 numberOfDimensions = 0u;
        if (ok) {
            desc = GetSignalType(InputSignals, i);
            ok = TranslateSignalName(desc, signalTypeName);
        }
        if (ok) {
            ok = GetSignalNumberOfDimensions(InputSignals, i, numberOfDimensions);
        }
        if (ok) {
            if (numberOfDimensions > 0u) {
                ok = GetSignalNumberOfElements(InputSignals, i, numberOfElements);

            }
        }
        if (ok) {
            if (numberOfElements > 1u) {
                ok = signalName.Printf("[%d]", numberOfElements);
            }
        }
        if (ok) {
            uint32 byteSize = 0u;
            ok = GetSignalByteSize(InputSignals, i, byteSize);
            if (ok) {
                inputToGAMByteSize += byteSize;
            }
        }
        if (ok) {
            ok = adapter->AddGAMInputSignal(gamIdx, signalName.Buffer(), signalTypeName.Buffer());
        }
    }
    for (i = 0u; (i < numberOfOutputs) && (ok); i++) {
        StreamString signalName;
        StreamString signalTypeName;
        ok = GetSignalName(OutputSignals, i, signalName);
        TypeDescriptor desc;
        uint32 numberOfElements = 1u;
        uint32 numberOfDimensions = 0u;
        if (ok) {
            desc = GetSignalType(OutputSignals, i);
            ok = TranslateSignalName(desc, signalTypeName);
        }
        if (ok) {
            ok = GetSignalNumberOfDimensions(OutputSignals, i, numberOfDimensions);
        }
        if (ok) {
            if (numberOfDimensions > 0u) {
                ok = GetSignalNumberOfElements(OutputSignals, i, numberOfElements);
            }
        }
        if (ok) {
            if (numberOfElements > 1u) {
                ok = signalName.Printf("[%d]", numberOfElements);
            }
        }
        if (ok) {
            uint32 byteSize = 0u;
            ok = GetSignalByteSize(OutputSignals, i, byteSize);
            if (ok) {
                outputFromGAMByteSize += byteSize;
            }
        }
        if (ok) {
            ok = adapter->AddGAMOutputSignal(gamIdx, signalName.Buffer(), signalTypeName.Buffer());
        }
    }
    if (ok) {
        ok = adapter->FinaliseGAM(gamIdx, inputToGAM, outputFromGAM);
    }
    return ok;
}

bool BaseLib2GAM::Execute() {
    BaseLib2::GAMAdapter *adapter = BaseLib2::GAMAdapter::Instance();
    bool ok = true;
    if (inputToGAMByteSize > 0u) {
        ok = MemoryOperationsHelper::Copy(inputToGAM, GetInputSignalsMemory(), inputToGAMByteSize);
    }
    if (ok) {
        ok = adapter->ExecuteGAM(gamIdx, gamFunctionNumber);
    }
    if (ok) {
        if (outputFromGAMByteSize > 0u) {
            ok = MemoryOperationsHelper::Copy(GetOutputSignalsMemory(), outputFromGAM, outputFromGAMByteSize);
        }
    }
    return ok;
}

bool BaseLib2GAM::TranslateSignalName(TypeDescriptor signalType,
                                      StreamString &signalTypeName) {
    bool ok = true;
    if (signalType.type == UnsignedInteger) {
        ok = signalTypeName.Printf("uint%u", signalType.numberOfBits);
    }
    else if (signalType.type == SignedInteger) {
        ok = signalTypeName.Printf("int%u", signalType.numberOfBits);
    }
    else if (signalType == Float32Bit) {
        signalTypeName = "float";
    }
    else if (signalType == Float64Bit) {
        signalTypeName = "double";
    }
    else {
        ok = false;
    }
    return ok;
}

}
#endif
