/**
 * @file PIDGAM.cpp
 * @brief Source file for class PID
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

#include "PIDGAM.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

PIDGAM::PIDGAM() {
    kp = 0.0;
    ki = 0.0;
    kd = 0.0;
    sampleTime = 0.0;
    maxOutput = MAX_FLOAT64;
    minOutput = -MAX_FLOAT64;
    enableIntegral = 1u;
    lastInput = 0.0;
    lastOutput = 0.0;
    enableSubstraction = 0u;
    reference = NULL_PTR(double *);
    sizeInputOutput = 0u;
    measurement = NULL_PTR(double *);
    output = NULL_PTR(double *);
    nOfInputSignals = 0u;
    nOfOutputSignals = 0u;
    numberOfInputElementsReference = 0u;
    numberOfInputElementsMeasurement = 0u;
    numberOfOutputElements = 0u;
    numberOfInputSamplesReference = 0u;
    numberOfInputSamplesMeasurement = 0u;
    numberOfOuputSamples = 0u;

}

PIDGAM::~PIDGAM() {
}

bool PIDGAM::Initialise(StructuredDataI &data) {
    bool ok = GAM::Initialise(data);
    if (ok) {
        data.Read("Kp", kp);
        data.Read("Ki", ki);
        data.Read("Kd", kd);
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
        data.Read("MaxOutput", maxOutput);
        data.Read("MinOutput", minOutput);
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
        REPORT_ERROR(ErrorManagement::ParametersError, "nOfInputSignals must be positive");
        ok = false;
    }
    else if (nOfInputSignals == 1u) {
        enableSubstraction = 0u;
    }
    else if (nOfInputSignals == 2u) {
        enableSubstraction = 1u;
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Maximum value nOfInputSignals = 2. nOfInputSignals = %u", nOfInputSignals)
        ;
        ok = false;
    }
    if (ok) {
        nOfOutputSignals = GetNumberOfOutputSignals();
        if (nOfOutputSignals != 1) {
            REPORT_ERROR(ErrorManagement::ParametersError, "nOfOutputSignals must be one");
            ok = false;
        }
    }
    if (ok) {
        ok = GetSignalNumberOfElements(InputSignals, 0u, numberOfInputElementsReference);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error GetSignalNumberOfElements for numberOfInputElementsReference");
        }
        if (numberOfInputElementsReference == 0) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The numberOfInputElementsReference value must be positive");
            ok = false;
        }
        if (ok) {
            if (enableSubstraction) {
                //The order of the input is very very important.
                ok = GetSignalNumberOfElements(InputSignals, 1u, numberOfInputElementsMeasurement);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Error GetSignalNumberOfElements for numberOfInputElementsMeasurement");
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
            REPORT_ERROR(ErrorManagement::InitialisationError, "Error GetSignalNumberOfElements() for numberOfOutputElements");
        }
        if (numberOfOutputElements != numberOfInputElementsReference) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The number of output elements must be the same than input elements");
            ok = false;
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
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfInputSamplesReference value must be 1");
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
                        REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfInputSamplesMeasurement value must be 1");
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
            ok = (numberOfInputSamplesReference == 1u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "numberOfOuputSamples value must be 1");
            }
        }
    }
    if (ok) {
        reference = static_cast<float64 *>(GetInputSignalMemory(0));
        if(enableSubstraction){
            measurement = static_cast<float64 *>(GetInputSignalMemory(0));
        }
        output = static_cast<float64 *>(GetInputSignalMemory(0));
    }

    return ok;
}
bool PIDGAM::Execute() {
    return true;
}
}

