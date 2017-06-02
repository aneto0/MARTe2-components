/**
 * @file WaveformSin.h
 * @brief Header file for class WaveformSin
 * @date May 19, 2017
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

 * @details This header file contains the declaration of the class WaveformSin
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMSIN_H_
#define WAVEFORMSIN_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../WaveformGAM/Waveform.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 *@brief GAM which allows to implement a sinusoidal waveform.
 *@details The configured coefficients (amplitude, frequency, phase, and offset) must be in flot64 and the implementation of the trigonometric functions is as follow
 * \f$
 * $output[i]= Amplitude * FastMath::Sin(2*FastMath::PI*frequency*time+phase)+offset
 * \f$
 * where the phase must be in radiant and he frequency in Hz.
 *
 * The input is a single value indicating the current time. The output could be a single array of N elements or multiple equal outputs of N
 * elements with different types (i.e example output1 type is uint8 output2 type float64.
 * The first iteration the output is 0 due to the second time is needed to compute the time step (or time increment) for each output sample.
 *
 * The GAM supports the following types:
 * int8
 * uint8
 * int16
 * uint16
 * int32
 * uint32
 * int64
 * uint64
 * float32
 * float64
 *
 * Additionally a trigger mechanism is implemented allowing to specify time intervals (in s) in which the signal is switch off and on.
 * The trigger intervals are specified in two arrays: one with the StartTriggerTime and another with  StopTriggerTime. If the
 * two array are not consistent (i.e. start time is later than stop time) a warning is launched, the trigger mechanism is disabled,
 * but the GAM operates in normal conditions.
 *
 * The configuration syntax is (names and signal quantity are only given as an example):
 * +waveformSin1 = {
 *     Class = WaveformSinGAM
 *     Amplitude = 10.0
 *     Frequency = 1.0
 *     Phase = 0.0
 *     Offset = 1.1
 *     StartTriggerTime = {0.1 0.3 0.5 1.8}
 *     StopTriggerTime = {0.2 0.4 0.6} //the StopTriggerTime has one less time, it means that after the sequence of output on and off, the GAM will remain on forever
 *
 *     InputSignals = {
 *         InputSignal1 = { //Filter will be applied to each signal. The number of input and output signals must be the same.
 *             DataSource = "DDB1"
 *             Type = uint32 //Supported type (int32 also valid since time cannot be negative)
 *         }
 *     }
 *     OutputSignals = {
 *         OutputSignal1 = {
 *             DataSource = "LCD"
 *             Type = float32
 *         }
 *         OutputSignal2 = {
 *             DataSource = "LCD"
 *             Type = float64
 *         }
 *     }
 * }
 */

class WaveformSin: public Waveform {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor
     */
    WaveformSin();

    /**
     * @brief Default destructor
     * @post
     * amplitude = 0.0
     * frequency = 0.0
     * phase = 0.0
     * offset = 0.0
     */
    virtual ~WaveformSin();

    /**
     * @brief Initialise the GAM from a configuration file.
     * @brief Load the parameters of for the sin implementation and verify its correctness and consistency
     * @return true if all parameters are valid
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief computes the Sin in uint8.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetUInt8Value();

    /**
     * @brief computes the Sin in int8.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetInt8Value();

    /**
     * @brief computes the Sin in uint16.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetUInt16Value();

    /**
     * @brief computes the Sin in in16.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetInt16Value();

    /**
     * @brief computes the Sin in uin32.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetUInt32Value();

    /**
     * @brief computes the Sin in int32.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetInt32Value();

    /**
     * @brief computes the Sin in uin64.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetUInt64Value();

    /**
     * @brief computes the Sin in int64.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetInt64Value();

    /**
     * @brief computes the Sin in float32.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetFloat32Value();

    /**
     * @brief computes the Sin in float64.
     * @brief this function calls the template implementation of the sin
     */
    virtual bool GetFloat64Value();

    /**
     * @brief computes the Sin in the specified type.
     */
    template<typename T>
    bool GetValue();

    virtual bool GetFloat64OutputValues();


private:
    float64 amplitude;
    float64 frequency;
    float64 phase;
    float64 offset;



};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORMSIN_H_ */

