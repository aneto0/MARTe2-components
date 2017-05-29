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
Waveform::Waveform() :
        GAM() {
    inputTime = NULL_PTR(uint32 *);
    outputValue = NULL_PTR(void **);
    nOfInputSignals = 0u;
    nOfOutputSignals = 0u;
    numberOfInputElements = 0u;
    numberOfOutputElements = 0u;
    numberOfInputSamples = 0u;
    numberOfOutputSamples = 0u;
    startTriggerTime = NULL_PTR(uint32 *);
    stopTriggerTime = NULL_PTR(uint32 *);
    numberOfStartTriggers = 0u;
    numberOfStopTriggers = 0u;
    triggersOn = false;
    indexStartTriggersArray = 0u;
    indexStopTriggersArray = 0u;
    signalOn = false;
    utimeIncrement = 0u;
    ucurrentTime = 0u;
    time0 = 0u;
    time1 = 0u;
    timeIncrement = 0.0;
    timeState = 0u;
    typeVariableOut = NULL_PTR(TypeDescriptor *);
    indexOutputSignal = 0u;
}

Waveform::~Waveform() {
    if(typeVariableOut == NULL_PTR(TypeDescriptor *)){
        delete [] typeVariableOut;
        typeVariableOut = NULL_PTR(TypeDescriptor *);
    }
    if (outputValue != NULL_PTR(void **)) {
        for (uint32 i = 0u; i < nOfOutputSignals; i++) {
            outputValue[i] = NULL_PTR(void *);
        }
        delete[] outputValue;
        outputValue = NULL_PTR(void **);
    }
    if (startTriggerTime != NULL_PTR(uint32 *)) {
        delete[] startTriggerTime;
        startTriggerTime = NULL_PTR(uint32 *);
    }
    if (stopTriggerTime != NULL_PTR(uint32 *)) {
        delete[] stopTriggerTime;
        stopTriggerTime = NULL_PTR(uint32 *);
    }
//lint -e{1740} only delete when new previously used.
}

bool Waveform::Initialise(StructuredDataI& data) {
    bool ok = GAM::Initialise(data);

    AnyType functionsArray = data.GetType("StartTriggerTime");
    AnyType functionsArray2;
    if (functionsArray.GetDataPointer() != NULL_PTR(void *)) {
        numberOfStartTriggers = functionsArray.GetNumberOfElements(0u);
        if (numberOfStartTriggers > 0u) {
            startTriggerTime = new uint32[numberOfStartTriggers];
            Vector<uint32> startTriggerTimeVector(startTriggerTime, numberOfStartTriggers);
            if (data.Read("StartTriggerTime", startTriggerTimeVector)) {
                functionsArray2 = data.GetType("StopTriggerTime");
                if (functionsArray2.GetDataPointer() != NULL_PTR(void *)) {
                    numberOfStopTriggers = functionsArray2.GetNumberOfElements(0u);
                    if (numberOfStartTriggers >= numberOfStopTriggers) {
                        if (2u > (numberOfStartTriggers - numberOfStopTriggers)) {
                            stopTriggerTime = new uint32[numberOfStopTriggers];
                            Vector<uint32> stopTriggerTimeVector(stopTriggerTime, numberOfStopTriggers);
                            if (data.Read("StopTriggerTime", stopTriggerTimeVector)) {
                                if (Waveform::ValidateTimeTriggers()) {
                                    triggersOn = true;
                                }
                                else {
                                    REPORT_ERROR(ErrorManagement::Warning, "Wrong trigger time. Trigger mechanism ignored.");
                                }
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::Warning, "(numberOfStartTriggers - numberOfStopTriggers) > 1. Trigger mechanism ignored.");
                        }
                    }
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::Warning, "numberOfStopTriggers > numberOfStartTriggers. Trigger mechanism ignored.");
            }
        }
        //if the name exist the dimension is 1... So this line will not be executed.
        else {
            REPORT_ERROR(ErrorManagement::Warning, "numberOfStartTriggers is 0. Trigger mechanism ignored.");
        }
    }
    return ok;
}
bool Waveform::Setup() {
    bool ok;
    nOfInputSignals = GetNumberOfInputSignals();
    ok = (nOfInputSignals == 1u);
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfInputSignals must be 1");
    }
    if (ok) {
        nOfOutputSignals = GetNumberOfOutputSignals();
        ok = (nOfOutputSignals != 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "nOfOutputSignals must be positive");
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
        if (ok) {
            ok = (numberOfOutputElements != 0u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfOutputElements must be positive");
            }
        }
        for (uint32 i = 1u; (i < nOfOutputSignals) && ok; i++) {
            uint32 auxNumberOfSamples;
            ok = GetSignalNumberOfElements(OutputSignals, i, auxNumberOfSamples);
            if (ok) {
                ok = auxNumberOfSamples == numberOfOutputElements;
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of samples between channels is different");
                }
            }
            else {
                //MISRA believe that i can be changed by REPORT_ERROR
                const uint32 aux = i;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading output numberOfElements for signal %u", aux)
                ;

            }
        }
    }
    if (ok) {
        ok = GetSignalNumberOfSamples(InputSignals, 0u, numberOfInputSamples);
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
        ok = GetSignalNumberOfSamples(OutputSignals, 0u, numberOfOutputSamples);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numberOfOutputSamples");
        }
        if (ok) {
            ok = (numberOfOutputSamples == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfOutputSamples must be 1");
            }
        }
        for (uint32 i = 1u; (i < nOfOutputSignals) && ok; i++) {
            ok = GetSignalNumberOfSamples(OutputSignals, i, numberOfOutputSamples);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numberOfOutputSamples");
            }
            if (ok) {
                ok = (numberOfOutputSamples == 1u);
                if (!ok) {
                    const uint32 aux = i;
                    REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfOutputSamples must be 1 for signal %u", aux)
                    ;
                }
            }
        }
    }

    if (ok) {
        typeVariableIn = GetSignalType(InputSignals, 0u);
        bool auxBool = (typeVariableIn == UnsignedInteger32Bit);
        ok = ((typeVariableIn == SignedInteger32Bit) || auxBool);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading input typeVariable.");
        }
    }

    typeVariableOut = new TypeDescriptor [nOfOutputSignals];
    if (ok) {
        for (uint32 i = 0u; (i < nOfOutputSignals) && ok; i++) {
            typeVariableOut[i] = GetSignalType(OutputSignals, i);
            ok = IsValidType(typeVariableOut[i]);
            if (!ok) {
                const uint32 aux = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Variable type not supported for signal %u", aux)
                ;
            }
        }
    }

    if (ok) {
        outputValue = new void *[nOfOutputSignals];
        inputTime = static_cast<uint32 *>(GetInputSignalMemory(0u));
        for (uint32 i = 0u; i < nOfOutputSignals; i++) {
            outputValue[i] = (GetOutputSignalMemory(i));
        }
    }

    return ok;
}

bool Waveform::Execute() {

    bool ok = true;
    //Check type and call correct function.
    for(indexOutputSignal = 0u; indexOutputSignal<numberOfOutputSignals; indexOutputSignal++){
        //If due to MISRA warning about null pointer
        if(typeVariableOut != NULL_PTR(TypeDescriptor *)){
            if (typeVariableOut[indexOutputSignal] == UnsignedInteger8Bit) {
                ok = GetUInt8Value();
            }
            else if (typeVariableOut[indexOutputSignal] == SignedInteger8Bit) {
                ok = GetInt8Value();
            }
            else if (typeVariableOut[indexOutputSignal] == UnsignedInteger16Bit) {
                ok = GetUInt16Value();
            }
            else if (typeVariableOut[indexOutputSignal] == SignedInteger16Bit) {
                ok = GetInt16Value();
            }
            else if (typeVariableOut[indexOutputSignal] == UnsignedInteger32Bit) {
                ok = GetUInt32Value();
            }
            else if (typeVariableOut[indexOutputSignal] == SignedInteger32Bit) {
                ok = GetInt32Value();
            }
            else if (typeVariableOut[indexOutputSignal] == UnsignedInteger64Bit) {
                ok = GetUInt64Value();
            }
            else if (typeVariableOut[indexOutputSignal] == SignedInteger64Bit) {
                ok = GetInt64Value();
            }
            else if (typeVariableOut[indexOutputSignal] == Float32Bit) {
                ok = GetFloat32Value();
            }
            else if (typeVariableOut[indexOutputSignal] == Float64Bit) {
                ok = GetFloat64Value();
            }
            else {

            }
        }
    }
    return ok;
}

}

