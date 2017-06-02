/**
 * @file WaveformPointsDef.cpp
 * @brief Source file for class WaveformPointsDef
 * @date 29/05/2017
 * @author Llorenc
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
 * the class WaveformPointsDef (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "WaveformPointsDef.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

WaveformPointsDef::WaveformPointsDef() :
        Waveform() {
    points = NULL_PTR(float64 *);
    times = NULL_PTR(float64 *);
    slopes = NULL_PTR(float64 *);
    numberOfPointsElements = 0u;
    numberOfTimesElements = 0u;
    numberOfSlopeElements = 0u;
    lastOutputValue = 0.0;
    refVal = 0.0;
    timeRefVal = 0.0;
    indexSlopes = 0u;
    beginningSequence = true;
    lastTimeValue = 0.0;

}

WaveformPointsDef::~WaveformPointsDef() {
    if (points != NULL_PTR(float64 *)) {
        delete[] points;
        points = NULL_PTR(float64 *);
    }
    if (times != NULL_PTR(float64 *)) {
        delete[] times;
        times = NULL_PTR(float64 *);
    }
    if (slopes != NULL_PTR(float64 *)) {
        delete[] slopes;
        slopes = NULL_PTR(float64 *);
    }
}
bool WaveformPointsDef::Initialise(StructuredDataI &data) {
    bool ok = Waveform::Initialise(data);
    if (!ok) {//Waveform::Initialise only fails if GAM::Initialise fails.
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error. Waveform::Initialise");
    }
    AnyType functionArray = data.GetType("Points");
    if(ok){
        ok = (functionArray.GetDataPointer() != NULL_PTR(void *));
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "Error. Fails to get the pointer to <Points>");
    }
    if (ok) {
        numberOfPointsElements = functionArray.GetNumberOfElements(0u);
        ok = (numberOfPointsElements != 0u);
        if (!ok) { //If the name Points exist it is difficult to have 0 elements
            REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfPointsElements must be positive");
        }
    }
    if (ok) {
        points = new float64[numberOfPointsElements];
    }
    if (ok) {
        Vector<float64> pointsVector(points, numberOfPointsElements);
        ok = data.Read("Points", pointsVector);
        if (!ok) { //If the name Points exist it is difficult to not read the elements
            REPORT_ERROR(ErrorManagement::InitialisationError,
                         "Fails reading <Points> values. A Scalar is not possible, but a Vector of one element is allowed");
        }
    }
    AnyType functionArray1;
    if (ok) {
        functionArray1 = data.GetType("Times");
        ok = (functionArray1.GetDataPointer() != NULL_PTR(void *));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Fails to get the pointer to <Times>");
        }
    }
    if (ok) {
        numberOfTimesElements = functionArray1.GetNumberOfElements(0u);
        ok = (numberOfTimesElements == numberOfPointsElements);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfTimesElements must be equal than numberOfPointsElements");
        }
    }
    if (ok) {
        times = new float64[numberOfTimesElements];
    }
    if (ok) {
        Vector<float64> timesVector(times, numberOfTimesElements);
        ok = data.Read("Times", timesVector);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "fail reading Times values");
        }
    }
    if (ok) {
        ok = VerifyTimes();
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid time sequence. The first time must be 0.0 and time must be monotonous increasing");
        }
    }
    if (ok) {
        //If is due to MISRA are worried about null pointer...
        if (times != NULL_PTR(float64 *)) {
            lastTimeValue = times[numberOfTimesElements - 1u];
            numberOfSlopeElements = numberOfTimesElements - 1u;
            slopes = new float64[numberOfSlopeElements];
            if ((slopes != NULL_PTR(float64 *)) && (points != NULL_PTR(float64 *))) { //MISRA!!!
                for (uint32 i = 0u; i < numberOfSlopeElements; i++) {
                    uint32 aux = i + 1u;
                    slopes[i] = (points[aux] - points[i]) / (times[aux] - times[i]);
                }
            }
        }
    }
    return ok;
}

bool WaveformPointsDef::Setup() {
    bool ok = Waveform::Setup();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::InitialisationError, " Waveform::Setup() exits with an error");
    }
    if(ok){
        ok = (points != NULL_PTR(float64 *));
    }
    if (ok) {
        if ((points != NULL_PTR(float64 *)) && (times != NULL_PTR(float64 *))) {
            refVal = points[0];
            timeRefVal = times[0];
        }
    }
    return ok;
}

bool WaveformPointsDef::GetFloat64OutputValues() {
    for (uint32 i = 0u; i < numberOfOutputElements; i++) {
        TriggerMechanism();
        //decides which slope should be used
        if (beginningSequence) {
            if ((points != NULL_PTR(float64 *)) && (times != NULL_PTR(float64 *))) {
                refVal = points[indexSlopes];
                timeRefVal = times[indexSlopes];
            }
            beginningSequence = false;
        }
        else {
            uint32 aux = indexSlopes + 1u;
            if (times != NULL_PTR(float64 *)) {
                if (currentTime >= times[aux]) {
                    indexSlopes++;
                    if (points != NULL_PTR(float64 *)) {
                        refVal = points[indexSlopes];
                        timeRefVal = times[indexSlopes];
                    }
                    if (indexSlopes < numberOfSlopeElements) {
                        //Don nothing
                    }
                    else { //reset index
                           //move the points for the next iteration
                        for (uint32 m = 0u; m < numberOfTimesElements; m++) {
                            if (times != NULL_PTR(float64 *)) {
                                times[m] = times[m] + lastTimeValue + timeIncrement;
                            }
                        }
                        beginningSequence = true;
                        indexSlopes = 0u;
                    }
                }
            }
        }
        if (signalOn && triggersOn) {
            if ((outputFloat64 != NULL_PTR(float64 *)) && (slopes != NULL_PTR(float64 *))) {
                outputFloat64[i] = refVal + ((currentTime - timeRefVal) * slopes[indexSlopes]);
            }
        }
        else {
            outputFloat64[i] = 0.0;
        }
        currentTime += timeIncrement;
    }
    return true;
}

template<typename T>
bool WaveformPointsDef::GetValue() {
    for (uint32 i = 0u; i < numberOfOutputElements; i++) {
        static_cast<T *>(outputValue[indexOutputSignal])[i] = static_cast<T>(outputFloat64[i]);
    }
    return true;
}

bool WaveformPointsDef::GetInt8Value() {
    return WaveformPointsDef::GetValue<int8>();
}
bool WaveformPointsDef::GetUInt8Value() {
    return WaveformPointsDef::GetValue<uint8>();
}

bool WaveformPointsDef::GetInt16Value() {
    return WaveformPointsDef::GetValue<int16>();
}

bool WaveformPointsDef::GetUInt16Value() {
    return WaveformPointsDef::GetValue<uint16>();
}

bool WaveformPointsDef::GetInt32Value() {
    return WaveformPointsDef::GetValue<int32>();
}

bool WaveformPointsDef::GetUInt32Value() {
    return WaveformPointsDef::GetValue<uint32>();
}

bool WaveformPointsDef::GetInt64Value() {
    return WaveformPointsDef::GetValue<int64>();
}

bool WaveformPointsDef::GetUInt64Value() {
    return WaveformPointsDef::GetValue<uint64>();
}

bool WaveformPointsDef::GetFloat32Value() {
    return WaveformPointsDef::GetValue<float32>();
}

bool WaveformPointsDef::GetFloat64Value() {
    return WaveformPointsDef::GetValue<float64>();
}
CLASS_REGISTER(WaveformPointsDef, "1.0")

}

