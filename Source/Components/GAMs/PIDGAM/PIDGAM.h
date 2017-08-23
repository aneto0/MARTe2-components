/**
 * @file PIDGAM.h
 * @brief Header file for class PID
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

 * @details This header file contains the declaration of the class PID
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PIDGAM_H_
#define PIDGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class PIDGAM: public GAM {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API PID)
public:
    PIDGAM();
    virtual ~PIDGAM();
    virtual bool Initialise(StructuredDataI &data);
    virtual bool Setup();
    virtual bool Execute();
private:

    /**
     * proportional term in the time domain
     */
    double kp;

    /**
     * integral term in the time domain
     */
    double ki;

    /**
     * derivative term in the time domain
     */
    double kd;

    /**
     * Indicates the time between samples.
     */
    double sampleTime;

    /**
     * upper limit saturation
     */
    double maxOutput;

    /**
     * lower limit saturation
     */
    double minOutput;

    /**
     * Enables/disable the integral term when saturation is acting (anti-windup function)
     */
    uint16 enableIntegral;

    /**
     * Save the last input value
     */
    double lastInput;

    /**
     * Save the last output value
     */
    double lastOutput;

    /**
     * When enableSubstraction is 1 the GAM expects two inputs: reference value and the feedback value (the actual measurement).
     * If enableSubstraction is 0 the GAM expects one input: the error value (which is the reference-measurement)
     */
    uint16 enableSubstraction;

    /**
     * Points to the input reference array of the GAM
     */
    double *reference;

    /**
     * Size of the input or output of the GAM. The arrays of reference, measurement and output must have the same size.
     */
    uint32 sizeInputOutput;

    /**
     * Points to the input measurement array of the GAM
     */
    double *measurement;

    /**
     * Points to the output array of the GAM (which is the output of the PID controller)
     */
    double *output;

    /**
     * Number of input signal.
     */
    uint32 nOfInputSignals;

    /**
     * Number of output signal.
     */
    uint32 nOfOutputSignals;

    /**
     * Number of input elements for the reference signal. Indicates the input array size
     */
    uint32 numberOfInputElementsReference;

    /**
     * Number of input elements for the feedback signal. Indicates the input array size
     */
    uint32 numberOfInputElementsMeasurement;

    /**
     * Number of output elements
     */
    uint32 numberOfOutputElements;

    /**
     * Number of input samples of the reference signal.
     */
    uint32 numberOfInputSamplesReference;

    /**
     * Number of input samples of the feedback signal.
     */
    uint32 numberOfInputSamplesMeasurement;

    /**
     * Number of output samples.
     */
    uint32 numberOfOuputSamples;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*PIDGAM_H_ */

