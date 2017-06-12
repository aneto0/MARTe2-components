/**
 * @file Waveform.h
 * @brief Header file for class Waveform
 * @date 19/05/2017
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
 * @details this class contains the generic common functions and variables to implement any waveform signal
 *
 * Three different GAMs are foreseen to use this generic Class:
 * WaveformSiGAM
 * WaveformPointsDefGAM
 * WaveformChirpGAM
 *
 * This class implements the generic function used by the children classes, which basically are the setup of the
 * trigger mechanism and the selection of the output type.
 *
 * The trigger time must be specified in seconds and the type should be float64.
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
     * startTriggerTime = NULL_PTR(float64 *);
     * stopTriggerTime = NULL_PTR(float64 *);
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
     * @brief Default destructor
     * @details Frees only the allocated memory by this class and set the pointers to 0
     * @post
     *  outputValue = NULL_PTR(void **);
     *  startTriggerTime = NULL_PTR(uint32 *);
     */
    virtual ~Waveform();

    /**
     * @brief Initialise the common variable from a configuration file
     * @details Initialise and verifies the trigger mechanism. This function checks the size of the startTriggerTime
     * and stopTriggerTime and checks that startTriggerTime[i] < stopTriggerTime[i]. An invalid trigger time sequence
     * does not give an error only a warning is launched, however the trigger mechanism is disabled and the output always
     * will be on with the expected defined waveform.
     * @param[in] data It is the GAM configuration file
     * @return true if MARTe#GAM#Initialise(StructuredDataI &data) exits without errors.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Setup the input output variables
     * @details Initialises the input and output pointers and verifies the number of inputs and output sizes
     * @return true if the input and outputs are correctly set.
     */
    virtual bool Setup();

    /**
     * @brief Generic implementation of the waveform.
     * @details This function decides type of variable function must be called.
     * The time difference between samples are computed from the time input given in two consecutive cycles and the number of samples
     * per cycle. For this reason the first cycle of this GAM always is 0.
     * @return true always.
     */
    virtual bool Execute();

    /**
     * @brief Generic call function for uint8
     * @details Virtual function which casts the output to uint8 type
     */
    virtual bool GetUInt8Value() = 0;

    /**
     * @brief Generic call function for int8
     * @details Virtual function which casts the output to int8 type
     */
    virtual bool GetInt8Value() = 0;

    /**
     * @brief Generic call function for uint16
     * @details Virtual function which casts the output to uint16 type
     */
    virtual bool GetUInt16Value() = 0;

    /**
     * @brief Generic call function for int16
     * @details Virtual function which casts the output to int16 type
     */
    virtual bool GetInt16Value() = 0;

    /**
     * @brief Generic call function for uint64
     * @details Virtual function which casts the output to uint64 type
     */
    virtual bool GetUInt32Value() = 0;

    /**
     * @brief Generic call function for int64
     * @details Virtual function which casts the output to int64 type
     */
    virtual bool GetInt32Value() = 0;

    /**
     * @brief Generic call function for uint64
     * @details Virtual function which casts the output to uint64 type
     */
    virtual bool GetUInt64Value() = 0;

    /**
     * @brief Generic call function for int64
     * @details Virtual function which casts the output to int64 type
     */
    virtual bool GetInt64Value() = 0;

    /**
     * @brief Generic call function for float32
     * @details Virtual function which casts the output to float32 type
     */
    virtual bool GetFloat32Value() = 0;

    /**
     * @brief Generic call function for float64
     * @details Virtual function which casts the output to float64 type
     */
    virtual bool GetFloat64Value() = 0;

    /**
     * @brief Generic call function to specific waveform type
     * @details Virtual function which calls the specific waveform type with float64 type
     */
    virtual bool GetFloat64OutputValues() = 0;

protected:

    /**
     * Output pointer. It support several output signals
     */
    void **outputValue;

    /**
     * Holds the waveform outputs before being cast to a specific type
     */
    float64 *outputFloat64;

    /**
     * Size of the output arrays. All outputs must have the same numberOfOutputElements
     */
    uint32 numberOfOutputElements;

    /**
     * number of output signal. At least the nOfOutputSignals must be 1
     */
    uint32 nOfOutputSignals;

    /**
     * current time in s
     */
    float64 currentTime;

    /**
     * increment time between consecutive output samples in s
     */
    float64 timeIncrement;

    /**
     * Indicates if the trigger mechanism is enabled. If it is disabled the waveform generated will be connected to the
     * output, otherwise the signal generated will be connected to the output depending on the trigger times.
     */
    bool triggersOn;

    /**
     * Indicates if the signal generated is connected to the output or if the output is connected to 0
     */
    bool signalOn;

    /**
     * Indicates which output signal must be casted
     */
    uint32 indexOutputSignal;

    /**
     * @brief Decides if triggersOn is enabled
     * @details This function decides if the triggersON is enabled allowing to output the waveform generated.
     */
    void TriggerMechanism();
private:

    /**
     * pointer to the input time. It is the input of the GAM
     */
    uint32 *inputTime;

    /**
     * Pointer to the start trigger time array.
     */
    float64 *startTriggerTime;
    /**
     * Pinter to the stop trigger time array.
     */
    float64 *stopTriggerTime;

    /**
     * Number of elements of startTriggerTime array
     */
    uint32 numberOfStartTriggers;

    /**
     * Number of elements of stopTriggerTime array
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
     * first time received in us
     */
    uint32 time0;

    /**
     * second time received in us
     */
    uint32 time1;

    /**
     * it is used to determine when time0, time1 are saved. The first iteration of his GAM is used to calculate the timeIncrement and the output is 0
     */
    uint8 timeState;

    /**
     * Indicates the type of the input variable. The supported input types are uint32 and int32. However it will always be casted to uint32, since the time never is negative.
     */
    TypeDescriptor typeVariableIn;

    /**
     * Pointer to the different output types.
     */
    TypeDescriptor *typeVariableOut;

    /**
     * number of input signals. This number must always  be 1
     */
    uint32 nOfInputSignals;

    /**
     * Number of input elements. It must always  be one
     */
    uint32 numberOfInputElements;

    /**
     * number of input samples. It must always be one
     */
    uint32 numberOfInputSamples;

    /**
     * number of output samples. It must be one.
     */
    uint32 numberOfOutputSamples;

    /**
     * Indicates if the trigger mechanism is enable.
     */
    bool triggersEnable;

    /**
     * @brief validate the trigger times
     * @details checks stopTriggerTime[i] > startTriggerTime[i] && stopTriggerTime[i] < startTriggerTime[i + 1] are true
     * @return true if the trigger times are consistent with the previous expressions.
     */
    bool ValidateTimeTriggers() const {
        bool ret = true;
        if (numberOfStopTriggers > 0u) {
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

    /**
     * @brief Checks that the type is one of the supported types.
     * @details the list of supported types are:\n
     * int8 \n
     * uin8 \n
     * int16 \n
     * uint16 \n
     * int32 \n
     * uin32 \n
     * int64 \n
     * uint64 \n
     * float32 \n
     * float64 \n
     * @return true if the type is one of the supported types.
     */
    bool IsValidType(TypeDescriptor const &typeRef) const {
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
        retVal = ((auxBool[0]) || (auxBool[1]) || (auxBool[2]) || (auxBool[3]) || (auxBool[4]) || (auxBool[5]) || (auxBool[6]) || (auxBool[7]) || (auxBool[8])
                || (auxBool[9]));
        delete[] auxBool;
        return retVal;
    }



};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* WAVEFORM_H_ */

