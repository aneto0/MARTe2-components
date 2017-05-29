/**
 * @file Waveform.h
 * @brief Header file for class Waveform
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

 * @details This header file contains the declaration of the class Waveform
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef WAVEFORMGENERATOR_WAVEFORM_H_
#define WAVEFORMGENERATOR_WAVEFORM_H_

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
 * @brief Generic class to implement any kind of waveform
 * @details this class contains the generic common functions and variables to implement any output signal
 *
 * Three different GAMs are foreseen to use this generic Class:
 * WaveformSiGAM
 * WaveformDotsDefGAM
 * WaveformChirpGAM
 */

class Waveform: public GAM {
public:

    /**
     * @brief Default constructor
     * @post
     * input = NULL_PTR(uint32 *);
     * output = NULL_PTR(void **);
     * nOfInputSignals = 0u;
     * nOfOutputSignals= 0u;
     * numberOfInputElements = 0u;
     * numberOfOutputElements = 0u;
     * numberOfInputSamples = 0u;
     * numberOfOutputSamples = 0u;
     * startTriggerTime = NULL_PTR(uint32 *);
     * stopTriggerTime = NULL_PTR(uint32 *);
     * numberOfStartTriggers = 0u;
     * numberOfStopTriggers = 0u;
     * triggersOn = false;
     * indexStartTriggersArray = 0u;
     * indexStopTriggersArray = 0u;
     * signalOn = false;
     * utimeIncrement = 0u;
     * ucurrentTime = 0u;
     * time0 = 0u;
     * time1 = 0u;
     * timeIncrement = 0.0;
     * timeState = 0u;
     */
    Waveform();

    /**
     * @brief Default constructor
     * @details Frees only the allocated memory by this class and set the pointers to 0
     * @post
     *  outputValue = NULL_PTR(void **);
     *  startTriggerTime = NULL_PTR(uint32 *);
     */
    virtual ~Waveform();

    /**
     * @brief Initialize the common variable from a configuration file
     * @details Initialise and verifies the trigger mechanism. This function checks the size of the startTriggerTime
     * and stopTriggerTime and checks that startTriggerTime[i] < stopTriggerTime[i]
     * @param[in] data. the GAM configuration
     * @return true if the trigger times are valid
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Setup the input output variables
     * @details Initialize the input output pointers and verify the number of inputs and output sizes
     * @return true if the input and outputs are correctly initialize
     */
    virtual bool Setup();

    /**
     * @brief Generic implementation of the output
     * @details This function decides the type function to call and calls the specific implementation of the signal generator
     * with the correct type defined in the configuration file
     */
    virtual bool Execute();

    /**
     * @brief Generic call function for uint8
     * @details Virtual function which call specific waveform with uint8 type
     */
    virtual bool GetUInt8Value() = 0;

    /**
     * @brief Generic call function for int8
     * @details Virtual function which call specific waveform with int8 type
     */
    virtual bool GetInt8Value() = 0;

    /**
     * @brief Generic call function for uint16
     * @details Virtual function which call specific waveform with uint16 type
     */
    virtual bool GetUInt16Value() = 0;

    /**
     * @brief Generic call function for int16
     * @details Virtual function which call specific waveform with int16 type
     */
    virtual bool GetInt16Value() = 0;

    /**
     * @brief Generic call function for uint64
     * @details Virtual function which call specific waveform with uint64 type
     */
    virtual bool GetUInt32Value() = 0;

    /**
     * @brief Generic call function for int64
     * @details Virtual function which call specific waveform with int64 type
     */
    virtual bool GetInt32Value() = 0;

    /**
     * @brief Generic call function for uint64
     * @details Virtual function which call specific waveform with uint64 type
     */
    virtual bool GetUInt64Value() = 0;

    /**
     * @brief Generic call function for int64
     * @details Virtual function which call specific waveform with int64 type
     */
    virtual bool GetInt64Value() = 0;

    /**
     * @brief Generic call function for float32
     * @details Virtual function which call specific waveform with float32 type
     */
    virtual bool GetFloat32Value() = 0;

    /**
     * @brief Generic call function for float64
     * @details Virtual function which call specific waveform with float64 type
     */
    virtual bool GetFloat64Value() = 0;

protected:
    /**
     * pointer to the input time. It is the input of the GAM
     */
    uint32 *inputTime;
    /**
     * Output pointer. It support several output signals
     */
    void **outputValue;
    /**
     * Size of the output arrays. All outputs must have the same numberOfOutputElements
     */
    uint32 numberOfOutputElements;

    /**
     * number of output signal. At least the nOfOutputSignals must be 1
     */
    uint32 nOfOutputSignals;

    /**
     * Pinter to the start trigger time array.
     */
    uint32 *startTriggerTime;
    /**
     * Pinter to the stop trigger time array.
     */
    uint32 *stopTriggerTime;

    /**
     * Size of startTriggerTime array
     */
    uint32 numberOfStartTriggers;

    /**
     * Size of stopTriggerTime array
     */
    uint32 numberOfStopTriggers;

    /**
     * index used to indicate the current position of the startTriggerTime time
     */
    uint32 indexStartTriggersArray;

    /**
     * index used to indicate the current position of the stopTriggerTime time
     */
    uint32 indexStopTriggersArray;

    /**
     * current time in us
     */
    uint32 ucurrentTime;

    /**
     * increment time between consecutive output samples in us
     */
    uint32 utimeIncrement;

    /**
     * Indicates if the trigger mechanism is enabled.
     */
    bool triggersOn;

    /**
     * Indicates if the signal generated is connected to the output or if the output is connected to 0
     */
    bool signalOn;

    /**
     * first time received in us
     */
    uint32 time0;

    /**
     * first time received in us
     */
    uint32 time1;

    /**
     * diffTime = time1 - time0. It is used to know the sample frequency
     */
    float64 timeIncrement;

    /**
     * it is used to determine when time0, time1 are saved. The first iteration of his GAM is used to calculate the timeIncrement and the output is 0
     */
    uint8 timeState;

    /**
     * Indicates which output signal must be computed
     */
    uint32 indexOutputSignal;

private:
    TypeDescriptor typeVariableIn;
    TypeDescriptor *typeVariableOut;
    uint32 nOfInputSignals;
    uint32 numberOfInputElements;
    uint32 numberOfInputSamples;
    uint32 numberOfOutputSamples;

    bool ValidateTimeTriggers() const {
        bool ret = true;
        if (numberOfStopTriggers > 0u) {
            for (uint32 i = 0u; (i < (numberOfStopTriggers - 1u)) && ret; i++) {
                //ret = ret && (stopTriggerTime[i] > startTriggerTime[i]);
                ret = (stopTriggerTime[i] > startTriggerTime[i]);
                if (ret) {
                    //MISRA...Suspicious truncation..
                    uint32 aux = i + 1u;
                    ret = (stopTriggerTime[i] < startTriggerTime[aux]);
                }
            }
            if (ret) {
                ret = (stopTriggerTime[numberOfStopTriggers - 1u] > startTriggerTime[numberOfStopTriggers - 1u]);
            }
            if (ret) {
                if (numberOfStartTriggers > numberOfStopTriggers) {
                    ret = (stopTriggerTime[numberOfStopTriggers - 1u] < startTriggerTime[numberOfStartTriggers - 1u]);
                }
            }
        }
        return ret;
    }

    bool IsValidType(TypeDescriptor const &typeRef) const{
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
        retVal = ((auxBool[0]) || (auxBool[1]) || (auxBool[2])
                || (auxBool[3]) || (auxBool[4]) || (auxBool[5])
                || (auxBool[6]) || (auxBool[7]) || (auxBool[8])
                || (auxBool[9]));
        delete [] auxBool;
        return retVal;
    }
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORM_H_ */

