/**
 * @file DoubleHandshakeMasterGAM.h
 * @brief Header file for class DoubleHandshakeMasterGAM
 * @date 01/08/2019
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class DoubleHandshakeMasterGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKEMASTERGAM_H_
#define SOURCE_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKEMASTERGAM_H_

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

namespace MARTe{

/**
 * @brief This GAM implements a double handshake protocol sending commands to a slave .
 *
 * @details The \a DoubleHandshakeMasterGAM provides commands to a slave. It routes the command signals in input (that
 * can arrive from other component in the application or from a supervisor) to a slave in output, implementing the double
 * handshake protocol. The type of signals managed by this GAM are:\n
 *   - CommandIn: the input command to be provided
 *   - CommandOut: the command to the slave
 *   - AckIn: the acknowledge from the slave
 *   - ClearIn: the trigger to clear the channel
 *   - InternalState: the double handshake state machine state
 *  Follows a description of the state machine that implements the double handshake for each command channel:\n
 *   - READY: the input command signal has not changed. If the command signal changes to a value different than zero, then go to SENDING.\n
 *   - SENDING: routes the command in output to the slave. Wait for the command to be acked from the slave, then go to CLEAR state.\n
 *   - CLEAR: waits the clear signals to be zero. This means that the acknowledge response has been managed by the other components and
 *   it is possible to clear the channel. Clears the channel and go to done.
 *   - DONE: waits that the command signal is reset by the slave. Then, go to READY again.
 *
 * @details When the ack from the slave is received, other GAMs (i.e the MessageGAM) could react. Once the acknowledge response has been handled,
 * the clear trigger signal is set to zero and the DoubleHandshakeMasterGAM will clear the channel.\n
 * The DoubleHandshakeSlaveGAM recognizes the signals commands, acks and clear triggers by name convention:\n
 *   - Signals beginning with "CommandIn" are the input commands signals.\n
 *   - Signals beginning with "AckIn" are the input acknowledge signals.\n
 *   - Signals beginning with "CleatTrigIn" are the input clear trigger signals.\n
 *   - Signals beginning with "CommandOut" are the output commands signals.\n
 *   - Signals beginning with "InternalState" are the output state machine state signals.\n
 *
 * Constraints:\n
 *   - [number of in commands]==[number of out command]\n
 *   - [number of in commands]==[number of acknowledges]\n
 *   - [number of in commands]==[number of clear triggers]\n
 *   - [number of in commands]==[number of internal states]\n
 *   - [in commands type] == [out commands type]\n
 *   - [in commands type] == [acknowledges type]\n
 *   - [clear triggers type] == uint32\n
 *   - [internal states type] == uint32
 *   - The internal states signals have to be defined all together
 *
 * The internal state signals provide in output the current state of the double handshake state machine for each channel:\n
 *   - 0 = READY\n
 *   - 1 = SENDING\n
 *   - 2 = CLEAR\n
 *   - 3 = DONE\n
 *   - 4 = ERROR\n
 *
 * The double handshake state machine can go in ERROR state in this cases:\n
 *   - From READY if the slave changes the ack.\n
 *   - From SENDING if the slave sends a bad ack (different than the command value).\n
 *   - From CLEAR if the slave changes or reset the ack.\n
 *   - From DONE if the slave changes the ack to something different than zero.\n
 *   - From SENDING, CLEAR or DONE if the specified timeout expires.
 *
 * @details Follows an example of configuration:
 * <pre>
 * +DoubleHandshakeMasterGAM = {
 *    Class = DoubleHandshakeGAM::DoubleHandshakeMasterGAM
 *    InputSignals = {
 *        CommandIn = {
 *            Alias = Command
 *            NumberOfElements=2
 *            Type = uint32
 *            DataSource = DDB1
 *        }
 *        AckIn = {
 *            Trigger = 1
 *            Type = uint32
 *            NumberOfElements=2
 *            DataSource = FromSlave
 *        }
 *        ClearIn0 = {
 *            Type = uint32
 *            DataSource = DDB1
 *        }
 *        ClearIn1 = {
 *            Type = uint32
 *            DataSource = DDB1
 *        }
 *    }
 *    OutputSignals = {
 *        CommandOut0 = {
 *            Type = uint32
 *            DataSource = ToSlave
 *        }
 *        CommandOut1 = {
 *            Type = uint32
 *            DataSource = ToSlave
 *        }
 *        InternalState0 = {
 *            Type = uint32
 *            DataSource = DDB1
 *        }
 *        InternalState1 = {
 *            Type = uint32
 *            DataSource = DDB1
 *        }
 *    }
 * }
 * </pre>
 */
class DoubleHandshakeMasterGAM: public GAM {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    DoubleHandshakeMasterGAM();

    /**
     * @brief Destructor
     */
    virtual ~DoubleHandshakeMasterGAM();

    /**
     * @brief Initialises the GAM.
     * @details The user can specify the parameter:\n
     *    - Timeout: the timeout in milliseconds of the handshake procedure. If the timeout expires in SENDING, CLEAR or
     *    DONE states, the state machine goes in ERROR state.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Setup the GAM.
     * @details Checks the constraints and initialises the internal variables to execute the double handshake state machine.
     */
    virtual bool Setup();

    /**
     * @brief Executes the double handshake state machine.
     */
    virtual bool Execute();

protected:

    /**
     * @brief Checks if a command signal has changed with respect the previous cycle.
     * @param[in] cIdx the command signal index.
     * @return -1 if the commands changed to zero, 1 if the command changed from zero, -2 if the command changed, 0 if the command did not change.
     */
    int32 IsChanged(const uint32 cIdx) const;


    /**
     * READY state
     */
    static const uint8 READY=0u;

    /**
     * SENDING state
     */
    static const uint8 SENDING=1u;

    /**
     * CLEAR state
     */
    static const uint8 CLEAR=2u;

    /**
     * DONE state
     */
    static const uint8 DONE=3u;

    /**
     * ERROR state
     */
    static const uint8 ERROR=4u;

    /**
     * The number of input commands
     */
    uint32 numberOfInputCommands;

    /**
     * The input command sizes
     */
    uint32 *inputCommandSize;

    /**
     * The input command offsets
     */
    uint32 *inputCommandOffset;

    /**
     * The input ack offsets
     */
    uint32 *inputAckOffset;

    /**
     * The clear trigger signal offsets
     */
    uint32 *inputClearTrigOffset;

    /**
     * The output command offsets
     */
    uint32 *outputCommandOffset;

    /**
     * The input memory buffer
     */
    uint8 *inputs;

    /**
     * The output memory buffer
     */
    uint8 *outputs;

    /**
     * Holds the inputs of the previous cycle
     */
    uint8 *previousValue;

    /**
     * Pointer to the double handshake protocol state
     */
    uint8 *state;

    /**
     * Stores the time when the state machine passes from ready to sending state
     */
    uint64 *startSMCycleTime;

    /**
     * The double handshake procedure timeout
     */
    uint64 timeout;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKEMASTERGAM_H_ */

