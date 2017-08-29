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
/**
 * @brief GAM which implement a generic PID with saturation and anti-windup
 * @details This GAM has two variants: single input or two inputs.\n
 * When a single input is defined, the input of the GAM should be the error signal, computed subtracting
 * the feedback (also called measurement) from the reference signal. The PID transfer function in the Z domain
 * and using the backward Euler discretization method is: \n
 * \f$
 * G_{PID} =  kp + sampleTime *  ki * z/(z - 1) + kd/sampleTime * (z-1)/z)
 * \f$
 * \n
 * where \n
 * \b kp is the proportional constant\n
 * \b ki is the integral constant\n
 * \b kd is the derivative term\n
 * \b sampleTime is the time between samples\n
 * Hence, the differential equation implemented is: \n
 * \f$
 * output = kp * error + ki * sampleTime * error + lastIntegral + kd/sampleTime * (error - lastError)
 * \f$ \n
 *\n
 * When two inputs are defined the behaviour is very similar as a single input. But before applying the PID, the GAM computes the error from the two inputs.
 * The \b order \b of \b the \b inputs \b is \b very \b import for the correct functioning. The first input must be the reference and the second the measurement
 * or (feedback). See below the configuration syntax for the proper input definition.
 *
 *\n
 * The GAM also provides a saturation output and anti-windup function.
 * When the output is saturated a flag is set to prevent the integral term from accumulating the error.
 * In saturation mode the output is computed as follow:\n
 * \f$
 * output = kp * error + ki * sampleTime * error + kd/sampleTime * (error - lastError)
 * \f$ \n
 * Notice that the lastIntegral is not added to the output.\n
 *
 *
 *The configuration syntax is (names and signal quantity are only given as an example):
 *<pre>
 * +PID1 = {
 *     Class = PIDGAM
 *     kp = 10.0 // or kp or ki or kd must be defined, otherwise an error will occur
 *     ki = 1.0
 *     kd = 0.0
 *     sampleFrequency = 0.001
 *     maxOutput = 500.0 //optional
 *     minOutput = -500.0 //optional
 *     InputSignals = {
 *         Reference = {
 *             DataSource = "DDB1"
 *             Type = float64
 *             elements = 1
 *             samples = 1
 *             dimension = 1
 *         }
 *         Measurement = { //Notice that the measurement is the second signal.
 *             DataSource = "DDB2"
 *             Type = float64
 *             elements = 1
 *             samples = 1
 *             dimension = 1
 *         }
 *     }
 *     OutputSignals = {
 *         OutputSignal1 = {
 *             DataSource = "LCD"
 *             Type = float64
 *             elements = 1
 *             samples = 1
 *             dimension = 1
 *         }
 *     }
 * }
 * </pre>
 *
 */
class PIDGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor
     * @post
     * kp = 0.0\n
     * proportional = 0.0\n
     * ki = 0.0\n
     * kid = 0.0\n
     * integral = 0.0\n
     * kd = 0.0\n
     * kdd = 0.0\n
     * derivative = 0.0\n
     * sampleTime = 0.0\n
     * maxOutput = MAX_FLOAT64\n
     * minOutput = -MAX_FLOAT64\n
     * enableIntegral = true\n
     * lastInput = 0.0\n
     * lastIntegral = 0.0
     * enableSubstraction = false\n
     * reference = NULL_PTR(float64 *)\n
     * sizeInputOutput = 0u\n
     * measurement = NULL_PTR(float64 *)\n
     * output = NULL_PTR(float64 *)\n
     * nOfInputSignals = 0u\n
     * nOfOutputSignals = 0u\n
     * numberOfInputElementsReference = 0u\n
     * numberOfInputElementsMeasurement = 0u\n
     * numberOfOutputElements = 0u\n
     * numberOfInputSamplesReference = 0u\n
     * numberOfInputSamplesMeasurement = 0u\n
     * numberOfOuputSamples = 0u\n
     * inputReferenceDimension = 0u\n
     * inputMeasurementDimension = 0u\n
     * outputDimension = 0u
     */
PIDGAM    ();

    /**
     * @brief Default constructor.
     * @post
     * reference = NULL_PTR(float64 *)\n
     * measurement = NULL_PTR(float64 *)\n
     * output = NULL_PTR(float64 *)\n
     */
    virtual ~PIDGAM();

    /**
     * @brief Initialise the parameters of the GAM according to the configuration file
     * @param[in] data GAM configuration
     * @details Load the following parameters from a predefined configuration \n
     * kp\n
     * ki\n
     * kd\n
     * sampleTime\n
     * maxOutput (optional)\n
     * minOutput (optional)\n
     * @post
     * kp != 0.0 || ki != 0.0 || kd != 0.0\n
     * sampleTime > 0.0\n
     * maxOutpt > minOutput\n
     * @return true if all postconditions are met
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Setup the input/output variables.
     * @details Initialise the input and output pointers and verify the number of elements, number
     * of samples and dimension. Moreover, a flag is set to 1 if the error must be calculated internally.
     * @post
     * nOfInputSignals = 1 || nOfInputSignals = 2\n
     * nOfOutputSignals = 1\n
     * numberOfInputElementsReference = 1\n
     * numberOfInputElementsMeasurement = 1\n
     * numberOfOutputElements = 1\n
     * numberOfInputSamplesReference = 1\n
     * numberOfInputSamplesMeasurement = 1\n
     * numberOfOuputSamples = 1\n
     * inputReferenceDimension = 1\n
     * inputMeasurementDimension = 1\n
     * outputDimension = 1\n
     * reference != NULL\n
     * if enableSubstraction = true --> measurement != NULL\n
     * output != NULL\n
     * @return true if all postconditions are met.
     */
    virtual bool Setup();

    /**
     * @brief Implements the PID.
     * @details First computes the PID, then saturates the output if needed. If the output
     * is saturated a flag prevents the integral term to continuing growing.
     * @return true.
     */
    virtual bool Execute();
private:

    /**
     * proportional coefficient in the time domain in the time domain
     */
    float64 kp;

    /**
     * contribution of the proportional term
     */
    float64 proportional;

    /**
     * integral coefficient in the time domain
     */
    float64 ki;

    /**
     * Integral coefficient in the discrete domain. kid = kid * sampleTime. It is used to speed up the operations
     */
    float64 kid;

    /**
     * contribution of the integral term
     */
    float64 integral;

    /**
     * Derivative coefficient in the time domain
     */
    float64 kd;

    /**
     * Derivative coefficient in the discrete domain. kdd= kd/sampleTime. It is used to speed up the operations
     */
    float64 kdd;

    /**
     * Contribution of the derivative term.
     */
    float64 derivative;

    /**
     * Indicates the time between samples.
     */
    float64 sampleTime;

    /**
     * upper limit saturation
     */
    float64 maxOutput;

    /**
     * lower limit saturation
     */
    float64 minOutput;

    /**
     * Enables/disable the integral term when saturation is acting (anti-windup function)
     */
    bool enableIntegral;

    /**
     * Save the last input value
     */
    float64 lastInput;

    /**
     * Save the last integrated term
     */
    float64 lastIntegral;

    /**
     * When enableSubstraction is 1 the GAM expects two inputs: reference value and the feedback value (the actual measurement).
     * If enableSubstraction is 0 the GAM expects one input: the error value (which is the reference-measurement)
     */
    bool enableSubstraction;

    /**
     * Points to the input reference array of the GAM
     */
    float64 *reference;

    /**
     * Size of the input or output of the GAM. The arrays of reference, measurement and output must have the same size.
     */
    uint32 sizeInputOutput;

    /**
     * Points to the input measurement array of the GAM
     */
    float64 *measurement;

    /**
     * Points to the output array of the GAM (which is the output of the PID controller)
     */
    float64 *output;

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

    /**
     * Input reference dimension. It must be one
     */
    uint32 inputReferenceDimension;

    /**
     * Input measurement dimension. It must be one
     */
    uint32 inputMeasurementDimension;

    /**
     * Output dimension. It must be one
     */
    uint32 outputDimension;

    /**
     * @brief Implements the PID
     */
    inline void GetValue();

    /**
     * Saturates the output of the PID if needed.
     */
    inline void Saturation();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*PIDGAM_H_ */

