/**
 * @file Waveform.cpp
 * @brief Source file for class Waveform
 * @date 19/05/2017
 * @author Llorenc Capella
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
    outputFloat64 = NULL_PTR(float64 *);
    nOfInputSignals = 0u;
    nOfOutputSignals = 0u;
    numberOfInputElements = 0u;
    numberOfOutputElements = 0u;
    numberOfInputSamples = 0u;
    numberOfOutputSamples = 0u;
    startTriggerTime = NULL_PTR(float64 *);
    stopTriggerTime = NULL_PTR(float64 *);
    numberOfStartTriggers = 0u;
    numberOfStopTriggers = 0u;
    triggersOn = false;
    indexStartTriggersArray = 0u;
    indexStopTriggersArray = 0u;
    signalOn = false;
    currentTime = 0.0;
    time0 = 0u;
    time1 = 0u;
    timeIncrement = 0.0;
    timeState = 0u;
    typeVariableOut = NULL_PTR(TypeDescriptor *);
    indexOutputSignal = 0u;
    triggersEnable = false;
}

Waveform::~Waveform() {
    if (typeVariableOut == NULL_PTR(TypeDescriptor *)) {
        delete[] typeVariableOut;
        typeVariableOut = NULL_PTR(TypeDescriptor *);
    }
    if (outputValue != NULL_PTR(void **)) {
        for (uint32 i = 0u; i < nOfOutputSignals; i++) {
            outputValue[i] = NULL_PTR(void *);
        }
        delete[] outputValue;
        outputValue = NULL_PTR(void **);
    }
    if (startTriggerTime != NULL_PTR(float64 *)) {
        delete[] startTriggerTime;
        startTriggerTime = NULL_PTR(float64 *);
    }
    if (stopTriggerTime != NULL_PTR(float64 *)) {
        delete[] stopTriggerTime;
        stopTriggerTime = NULL_PTR(float64 *);
    }
    if (outputFloat64 != NULL_PTR(float64 *)) {
        delete[] outputFloat64;
        outputFloat64 = NULL_PTR(float64 *);
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
            startTriggerTime = new float64[numberOfStartTriggers];
            Vector<float64> startTriggerTimeVector(startTriggerTime, numberOfStartTriggers);
            if (data.Read("StartTriggerTime", startTriggerTimeVector)) {
                functionsArray2 = data.GetType("StopTriggerTime");
                if (functionsArray2.GetDataPointer() != NULL_PTR(void *)) {
                    numberOfStopTriggers = functionsArray2.GetNumberOfElements(0u);
                    if (numberOfStartTriggers >= numberOfStopTriggers) {
                        if (2u > (numberOfStartTriggers - numberOfStopTriggers)) {
                            stopTriggerTime = new float64[numberOfStopTriggers];
                            Vector<float64> stopTriggerTimeVector(stopTriggerTime, numberOfStopTriggers);
                            if (data.Read("StopTriggerTime", stopTriggerTimeVector)) {
                                if (ValidateTimeTriggers()) {
                                    triggersEnable = true;
                                }
                                else {
                                    REPORT_ERROR(ErrorManagement::Warning, "Wrong trigger time sequence. Trigger mechanism ignored.");
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
            uint32 auxNumberOfOutputElements;
            ok = GetSignalNumberOfElements(OutputSignals, i, auxNumberOfOutputElements);
            if (ok) {
                ok = (auxNumberOfOutputElements == numberOfOutputElements);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "The number of elements between channels is different");
                }
            }
            else {
                //MISRA believe that i can be changed by REPORT_ERROR
                const uint32 aux = i;
                REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading output numberOfElements for signal %u", aux);

            }
        }
    }
    if (ok) {
        outputFloat64 = new float64[numberOfOutputElements];
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
                    REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfOutputSamples must be 1 for signal %u", aux);

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

    typeVariableOut = new TypeDescriptor[nOfOutputSignals];
    if (ok) {
        for (uint32 i = 0u; (i < nOfOutputSignals) && ok; i++) {
            typeVariableOut[i] = GetSignalType(OutputSignals, i);
            ok = IsValidType(typeVariableOut[i]);
            if (!ok) {
                const uint32 aux = i;
                REPORT_ERROR(ErrorManagement::ParametersError, "Variable type not supported for signal %u", aux);

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
    if (timeState == static_cast<uint8>(0u)) {
        if (inputTime != NULL_PTR(uint32 *)) {
            time0 = *inputTime;
        }
        timeState++;
        signalOn = false;
        if (inputTime != NULL_PTR(uint32 *)) {
            currentTime = static_cast<float64>(*inputTime) / 1e6;
        }
        ok = PrecomputeValues();
        /*lint -e{613} typeVariableOut cannot be NULL as otherwise Execute will not be called*/
        for (indexOutputSignal = 0u; indexOutputSignal < numberOfOutputSignals; indexOutputSignal++) {
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
                ok = GetInt64Value();
            }
            else if (typeVariableOut[indexOutputSignal] == SignedInteger64Bit) {
                ok = GetUInt64Value();
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
        signalOn = true;
    }
    else if (timeState == static_cast<uint8>(1u)) {
        if (inputTime != NULL_PTR(uint32 *)) {
            time1 = *inputTime;
        }
        if (numberOfOutputElements != 0u) {
            timeIncrement = ((static_cast<float64>(time1) - static_cast<float64>(time0)) / static_cast<float64>(numberOfOutputElements)) / 1e6;
        }
        timeState++;
    }
    else {

    }
    if (timeState == static_cast<uint8>(2u)) {
        if (inputTime != NULL_PTR(uint32 *)) {
            currentTime = static_cast<float64>(*inputTime) / 1e6;
        }
        //Check type and call correct function.
        ok = PrecomputeValues();
        for (indexOutputSignal = 0u; indexOutputSignal < numberOfOutputSignals; indexOutputSignal++) {
            //If due to MISRA warning about null pointer
            if (typeVariableOut != NULL_PTR(TypeDescriptor *)) {
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
    }
    return ok;
}

void Waveform::TriggerMechanism() {
    if (triggersEnable) {
        bool stop = false;
        while ((indexStopTriggersArray < numberOfStopTriggers) && (!stop)) {
            if (stopTriggerTime != NULL_PTR(float64 *)) {
                //lint -e{661} the while prevents the index to be out of-of-bounds
                if (stopTriggerTime[indexStopTriggersArray] > currentTime) {
                    stop = true;
                }
                else {
                    indexStopTriggersArray++;
                }
            }
        }
        if (stop) {
            if (startTriggerTime != NULL_PTR(float64 *)) {
                //lint -e{662} the number of elements of startTriggerTime is equal or grater than elements of stopTriggerTime, then start startTriggerTime will not be out-of-bounds
                //lint -e{661} the previous while ensures that the index is not out-of-bounds
                if (currentTime >= startTriggerTime[indexStopTriggersArray]) {
                    triggersOn = true;
                }
                else {
                    triggersOn = false;
                }
            }
        }
        else {
            if (numberOfStopTriggers < numberOfStartTriggers) {
                if (startTriggerTime != NULL_PTR(float64 *)) {
                    if (currentTime >= startTriggerTime[numberOfStopTriggers]) {
                        triggersOn = true;
                    }
                    else {
                        triggersOn = false;
                    }
                }
            }
            else {
                triggersOn = false;
            }
        }
    }
    else {
        triggersOn = true;
    }
    return;
}

bool Waveform::ValidateTimeTriggers() const {
    bool ret = true;
    if (numberOfStopTriggers > 0u) {
        /*lint -e{613} -e{661} typeVariableOut cannot be NULL as otherwise ValidateTimeTriggers will not be called.
         * (i + 1u) < numberOfStartTriggers and as such cannot go out of bounds*/
        for (uint32 i = 0u; (i < numberOfStopTriggers) && ret; i++) {
            ret = (stopTriggerTime[i] > startTriggerTime[i]);
            if (ret && ((i + 1u) < numberOfStartTriggers)) {
                //MISRA...Suspicious truncation..
                uint32 aux = i + 1u;
                ret = (stopTriggerTime[i] < startTriggerTime[aux]);
            }
        }
    }
    return ret;
}

bool Waveform::IsValidType(TypeDescriptor const &typeRef) const {
    bool retVal;
    bool *auxBool = (new bool[10u]);
    auxBool[0] = typeRef == Float32Bit;
    auxBool[1] = typeRef == Float64Bit;
    auxBool[2] = typeRef == SignedInteger8Bit;
    auxBool[3] = typeRef == UnsignedInteger8Bit;
    auxBool[4] = typeRef == SignedInteger16Bit;
    auxBool[5] = typeRef == UnsignedInteger16Bit;
    auxBool[6] = typeRef == SignedInteger32Bit;
    auxBool[7] = typeRef == UnsignedInteger32Bit;
    auxBool[8] = typeRef == SignedInteger64Bit;
    auxBool[9] = typeRef == UnsignedInteger64Bit;
    retVal = ((auxBool[0]) || (auxBool[1]) || (auxBool[2]) || (auxBool[3]) || (auxBool[4]) || (auxBool[5]) || (auxBool[6]) || (auxBool[7]) || (auxBool[8]) || (auxBool[9]));
    delete[] auxBool;
    return retVal;
}

}
