/**
 * @file PIDGAM.cpp
 * @brief Source file for class PIDGAM
 * @date 22/08/2017
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
 * the class PID (public, protected, and private). Be aware that some 
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
#include "PIDGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

PIDGAM::PIDGAM() :
        GAM() {
    kp = 0.0;
    proportional = 0.0;
    ki = 0.0;
    kid = 0.0;
    integral = 0.0;
    kd = 0.0;
    kdd = 0.0;
    derivative = 0.0;
    sampleTime = 0.0;
    maxOutput = MAX_FLOAT64;
    minOutput = -MAX_FLOAT64;
    enableIntegral = true;
    lastInput = 0.0;
    lastIntegral = 0.0;
    enableSubstraction = false;
    reference = NULL_PTR(float64 *);
    sizeInputOutput = 0u;
    measurement = NULL_PTR(float64 *);
    output = NULL_PTR(float64 *);
    nOfInputSignals = 0u;
    nOfOutputSignals = 0u;
    numberOfInputElementsReference = 0u;
    numberOfInputElementsMeasurement = 0u;
    numberOfOutputElements = 0u;
    numberOfInputSamplesReference = 0u;
    numberOfInputSamplesMeasurement = 0u;
    numberOfOuputSamples = 0u;
    inputReferenceDimension = 0u;
    inputMeasurementDimension = 0u;
    outputDimension = 0u;

}

PIDGAM::~PIDGAM() {
    reference = NULL_PTR(float64 *);
    measurement = NULL_PTR(float64 *);
    output = NULL_PTR(float64 *);
}

bool PIDGAM::Initialise(StructuredDataI &data) {
    bool ok = GAM::Initialise(data);
    if (ok) {
        ok = data.Read("Kp", kp);
        bool aux;
        aux = data.Read("Ki", ki);
        ok = (ok || aux);
        aux = data.Read("Kd", kd);
        ok = (ok || aux);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "kp, ki and kd missing. At least one parameter must be initialised");
        }
        //lint -e{9007} No side effect on the function IsEqual.
        if (IsEqual(kp, 0.0) && IsEqual(ki, 0.0) && IsEqual(kd, 0.0)) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "kp = ki = kd = 0.");
            ok = false;
        }
    }
    if (ok) {
        ok = data.Read("SampleTime", sampleTime);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading sampleTime");
        }
    }
    if (ok) {
        if (IsEqual(sampleTime, 0.0)) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "sampleTime must be positive");
            ok = false;
        }
    }
    if (ok) {
        kid = ki * sampleTime;
        kdd = kd / sampleTime;
    }
    if (ok) {
        ok = data.Read("MaxOutput", maxOutput);
        if (!ok) {
            maxOutput = MAX_FLOAT64;
        }
        ok = data.Read("MinOutput", minOutput);
        if (!ok) {
            minOutput = -MAX_FLOAT64;
        }
        ok = true;
        if (maxOutput < minOutput) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "maxOutput < minOutput. maxOutput must be larger than minOutput");
            ok = false;
        }
        if (IsEqual(maxOutput, minOutput)) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "maxOutput = minOutput. maxOutput must be larger than minOutput");
            ok = false;
        }
    }
    return ok;
}
bool PIDGAM::Setup() {
    bool ok = true;
    nOfInputSignals = GetNumberOfInputSignals();
    if (nOfInputSignals == 0u) {
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfInputSignals must be positive. The current value is %u", nOfInputSignals);
        ok = false;
    }
    else if (nOfInputSignals == 1u) {
        enableSubstraction = false;
    }
    else if (nOfInputSignals == 2u) {
        enableSubstraction = true;
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Maximum value nOfInputSignals = 2. nOfInputSignals = %u", nOfInputSignals);
        ok = false;
    }
    if (ok) {
        nOfOutputSignals = GetNumberOfOutputSignals();
        if (nOfOutputSignals != 1u) {
            REPORT_ERROR(ErrorManagement::ParametersError, "nOfOutputSignals must be one. The current values is %u", nOfOutputSignals);
            ok = false;
        }
    }
    if (ok) {
        ok = GetSignalNumberOfElements(InputSignals, 0u, numberOfInputElementsReference);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfElements returned an error for numberOfInputElementsReference");
        }
        if (ok) {
            if (numberOfInputElementsReference != 1u) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The numberOfInputElementsReference value must be one. The crrent value is %u",
                             numberOfInputElementsReference);
                ok = false;
            }
        }
        if (ok) {
            if (enableSubstraction) {
                //The order of the input is very very important.
                ok = GetSignalNumberOfElements(InputSignals, 1u, numberOfInputElementsMeasurement);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfElements returned an error for numberOfInputElementsMeasurement");
                }
                if (ok) {
                    if (numberOfInputElementsMeasurement != numberOfInputElementsReference) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfInputElementsReference is different from numberOfInputElementsMeasurement");
                        ok = false;
                    }
                }
            }
        }
    }

    if (ok) {
        ok = GetSignalNumberOfElements(OutputSignals, 0u, numberOfOutputElements);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "GetSignalNumberOfElements() returned an error for numberOfOutputElements");
        }
        if (ok) {
            if (numberOfOutputElements != numberOfInputElementsReference) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of output elements must be the same than the number of input elements");
                ok = false;
            }
        }
    }

    if (ok) {
        ok = GetSignalNumberOfSamples(InputSignals, 0u, numberOfInputSamplesReference);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numberOfInputSamplesReference");
        }
        if (ok) {
            ok = (numberOfInputSamplesReference == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfInputSamplesReference value must be 1. It is %u", numberOfInputSamplesReference);
            }
        }
        if (ok) {
            if (enableSubstraction) {
                ok = GetSignalNumberOfSamples(InputSignals, 1u, numberOfInputSamplesMeasurement);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numberOfInputSamplesMeasurement");
                }
                if (ok) {
                    ok = (numberOfInputSamplesMeasurement == 1u);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfInputSamplesMeasurement value must be 1. It is %u",
                                     numberOfInputSamplesMeasurement);
                    }
                }
            }
        }
    }
    if (ok) {
        ok = GetSignalNumberOfSamples(OutputSignals, 0u, numberOfOuputSamples);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading numberOfOuputSamples");
        }
        if (ok) {
            ok = (numberOfOuputSamples == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfOuputSamples value must be 1. numberOfOuputSamples = %u", numberOfOuputSamples);
            }
        }
    }
    if (ok) {
        ok = GetSignalNumberOfDimensions(InputSignals, 0u, inputReferenceDimension);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading inputReferenceDimension");
        }
        if (ok) {
            ok = (inputReferenceDimension == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "inputReferenceDimension value must be 1. inputReferenceDimension = %u",
                             inputReferenceDimension);
            }
        }
        if (ok) {
            if (enableSubstraction) {
                ok = GetSignalNumberOfDimensions(InputSignals, 1u, inputMeasurementDimension);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading inputMeasurementDimension");
                }
                if (ok) {
                    ok = (inputMeasurementDimension == 1u);
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "inputMeasurementDimension value must be 1. It is %u", inputMeasurementDimension);
                    }
                }
            }
        }
    }
    if (ok) {
        ok = GetSignalNumberOfDimensions(OutputSignals, 0u, outputDimension);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error reading outputDimension");
        }
        if (ok) {
            ok = (outputDimension == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "outputDimension value must be 1. It is %u", outputDimension);
            }
        }
    }
    if (ok) {
        ok = (GetSignalType(InputSignals, 0u) == Float64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The reference data type must be float64.");
        }
        if (ok) {
            if (enableSubstraction) {
                ok = (GetSignalType(InputSignals, 1u) == Float64Bit);
            }
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The measurement data type must be float64.");
            }
        }
    }
    if (ok) {
        ok = (GetSignalType(OutputSignals, 0u) == Float64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The data type must be float64.");
        }
    }

    if (ok) {
        reference = static_cast<float64 *>(GetInputSignalMemory(0u));
        if (enableSubstraction) {
            measurement = static_cast<float64 *>(GetInputSignalMemory(1u));
        }
        output = static_cast<float64 *>(GetOutputSignalMemory(0u));
    }

    return ok;
}
bool PIDGAM::Execute() {
    GetValue();
    Saturation();
    return true;
}

//lint -e{613} The Setup() function guarantee that the pointers are not NULL.
void PIDGAM::GetValue() {
    if (enableSubstraction) {
        float64 error;
        error = *reference - *measurement;
        proportional = kp * error;
        if (enableIntegral) {
            integral = (error * kid) + lastIntegral;
        }
        else {
            integral = error * kid;
        }
        derivative = (error - lastInput) * kdd;
        lastInput = error;
    }
    else {
        proportional = kp * *reference;
        if (enableIntegral) {
            integral = ((*reference * kid) + lastIntegral);
        }
        else {
            integral = *reference * kid;
        }
        derivative = (*reference - lastInput) * kdd;
        lastInput = *reference;
    }
    output[0] = proportional + integral + derivative;
    lastIntegral = integral;
}

//lint -e{613} The Setup() function guarantee that the pointers are not NULL.
void PIDGAM::Saturation() {
    if (output[0] > maxOutput) {
        output[0] = maxOutput;
        enableIntegral = false;
    }
    else if (output[0] < minOutput) {
        output[0] = minOutput;
        enableIntegral = false;
    }
    else {
        enableIntegral = true;
    }
}
CLASS_REGISTER(PIDGAM, "1.0")
}

