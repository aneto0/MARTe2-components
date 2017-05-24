/**
 * @file Waveform.cpp
 * @brief Source file for class Waveform
 * @date May 19, 2017
 * @author aneto
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
 * the class Waveform (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Waveform.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
Waveform::Waveform() {
    inputTime = NULL_PTR(uint32 *);
    outputValue = NULL_PTR(void **);
    numberOfInputSignals = 0u;
    numberOfOutputSignals = 0u;
    numberOfInputElements = 0u;
    numberOfOutputElements = 0u;
    numberOfInputSamples = 0u;
    numberOfOutputSamples = 0u;
}

Waveform::~Waveform() {
    if (outputValue != NULL_PTR(void **)) {
        for (uint32 i = 0; i < numberOfOutputSignals; i++) {
            outputValue[i] = NULL_PTR(void *);
        }
        delete[] outputValue;
        outputValue = NULL_PTR(void **);
    }
}

bool Waveform::Setup() {
    bool ok = true;
    numberOfInputSignals = GetNumberOfInputSignals();
    ok = (numberOfInputSignals == 1u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "numberOfInputSignals must be 1");
    }
    if (ok) {
        numberOfOutputSignals = GetNumberOfOutputSignals();
        ok = (numberOfOutputSignals != 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "numberOfOutputSignals must be positive");
        }
    }
    if (ok) {
        ok = GetSignalNumberOfElements(InputSignals, 0u, numberOfInputElements);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading input numberOfElements");
        }
        else {
            ok = (numberOfInputElements == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of input elements must be 1");
            }
        }
    }
    if (ok) {
        ok = GetSignalNumberOfElements(OutputSignals, 0u, numberOfOutputElements);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading output numberOfElements");
        }
        for (uint32 i = 1u; i < numberOfOutputSignals && ok; i++) {
            uint32 auxNumberOfSamples;
            ok = GetSignalNumberOfElements(OutputSignals, i, auxNumberOfSamples);
            if (ok) {
                if (auxNumberOfSamples != numberOfOutputElements) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples between channels is different");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading the number of samples for signal %u", i);

        }
    }
}
if (ok) {
    ok = GetSignalNumberOfSamples(InputSignals, 0, numberOfInputSamples);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numberOfInputSamples");
    }
    if (ok) {
        ok = (numberOfInputSamples == 1u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfInputSamples must be 1");
        }
    }
}

if (ok) {
    for (uint32 i = 1; i < numberOfOutputSignals && ok; i++) {
        ok = GetSignalNumberOfSamples(OutputSignals, 0, numberOfOutputSamples);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numberOfOutputSamples");
        }
        if (ok) {
            ok = (numberOfOutputSamples == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfOutputSamples must be 1");
            }
        }
    }
}

if (ok) {
    typeVariableIn = GetSignalType(InputSignals, 0u);
    ok = (typeVariableIn == SignedInteger32Bit || typeVariableIn == UnsignedInteger32Bit);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading input typeVariable. Variable type is null");
    }
}

if (ok) {
    typeVariableOut = GetSignalType(OutputSignals, 0u);
    ok = ((typeVariableOut == Float32Bit) || (typeVariableOut == Float64Bit) || (typeVariableOut == SignedInteger8Bit) || (typeVariableOut == UnsignedInteger8Bit) || (typeVariableOut == SignedInteger16Bit) || (typeVariableOut == UnsignedInteger16Bit) || (typeVariableOut == SignedInteger32Bit) || (typeVariableOut == UnsignedInteger32Bit) || (typeVariableOut == SignedInteger64Bit) || (typeVariableOut == UnsignedInteger64Bit));
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Variable type is not supported");
    }
    for (uint32 i = 1u; i < numberOfOutputSignals && ok; i++) {
        TypeDescriptor auxTypeVariable;
        auxTypeVariable = GetSignalType(OutputSignals, i);
        ok = (auxTypeVariable == typeVariableOut);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "All type descriptors should be equal");
        }
    }
}
outputValue = new void *[numberOfOutputSignals];
if (ok) {
    inputTime = static_cast<uint32 *>(GetInputSignalMemory(0));
    for (uint32 i = 0u; i < numberOfOutputSignals; i++) {
        outputValue[i] = (GetOutputSignalMemory(i));
    }
}

return ok;
}

bool Waveform::Execute() {

    bool ok = true;
    //Check type and call correct function.
    if (typeVariableOut == UnsignedInteger8Bit) {
        ok = GetUInt8Value();
    }
    else if (typeVariableOut == SignedInteger8Bit) {
        ok = GetInt8Value();
    }
    else if (typeVariableOut == UnsignedInteger16Bit) {
        ok = GetUInt16Value();
    }
    else if (typeVariableOut == SignedInteger16Bit) {
        ok = GetInt16Value();
    }
    else if (typeVariableOut == UnsignedInteger32Bit) {
        ok = GetUInt32Value();
    }
    else if (typeVariableOut == SignedInteger32Bit) {
        ok = GetInt32Value();
    }
    else if (typeVariableOut == UnsignedInteger64Bit) {
        ok = GetUInt64Value();
    }
    else if (typeVariableOut == SignedInteger64Bit) {
        ok = GetInt64Value();
    }
    else if (typeVariableOut == Float32Bit) {
        ok = GetFloat32Value();
    }
    else if (typeVariableOut == Float64Bit) {
        ok = GetFloat64Value();
    }
    return ok;
}

}

