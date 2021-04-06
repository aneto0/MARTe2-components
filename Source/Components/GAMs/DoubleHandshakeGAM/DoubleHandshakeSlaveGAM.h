/**
 * @file DoubleHandshakeSlaveGAM.h
 * @brief Header file for class DoubleHandshakeSlaveGAM
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

 * @details This header file contains the declaration of the class DoubleHandshakeSlaveGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKESLAVEGAM_H_
#define SOURCE_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKESLAVEGAM_H_

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
 * @brief This GAM implements a double handshake protocol receiving commands from a master.
 *
 * @details The \a DoubleHandshakeSlaveGAM listens on a number of command channels. Follows a description
 * of the state machine that implements the double handshake for each command channel:\n
 *   - READY: the command signal is zero. Wait for channel to be triggered and when so, go to SENDING.\n
 *   - SENDING: wait that the clear trigger signal is zero. This means that the MARTe application handled the command. Route the
 *   command value to the ack signal and go to DONE.\n
 *   - DONE: wait that the command signal is reset by the master. When so, reset the ack signal and go to READY again.
 *
 * @details When a command is received other GAMs (i.e the MessageGAM) could react. Once the command has been handled, the clear trigger signal
 * is set to zero and the DoubleHandshakeSlaveGAM will send the acknowledge accordingly.\n
 * The DoubleHandshakeSlaveGAM recognizes the signals commands, acks and clear triggers by name convention:\n
 *   - Signals beginning with "CommandIn" are the input commands signals.\n
 *   - Signals beginning with "ClearTrigIn" are the input clear trigger signals.\n
 *   - Signals beginning with "AckOut" are the output acknowledge signals.\n
 *   - Signals beginning with "InternalState" are the output state machine state signals.\n
 *
 * Constraints:\n
 *   - [number of commands]==[number of acknowledges]\n
 *   - [number of commands]==[number of clear triggers]\n
 *   - [number of commands]==[number of internal states]\n
 *   - [commands type] == [acknowledges type]\n
 *   - [clear triggers type] == uint32\n
 *   - [internal states type] == uint32
 *   - The internal states signals have to be defined all together
 *
 * The internal state signals provide in output the current state of the double handshake state machine for each channel:\n
 *   - 0 = READY\n
 *   - 1 = SENDING\n
 *   - 2 = DONE\n
 *   - 3 = ERROR\n
 *
 * The double handshake state machine can go in ERROR state in these cases:\n
 *   - From SENDING if the command signal changes.\n
 *   - From DONE if the command signal changes to something different than zero.\n
 *   - From SENDING or DONE if the specified timeout expires.
 *
 * @details Follows an example of configuration.
 * <pre>
 * +DoubleHandshakeSlaveGAM = {
 *    Class = DoubleHandshakeGAM::DoubleHandshakeSlaveGAM
 *    InputSignals = {
 *        CommandIn = {
 *            Alias = Command
 *            NumberOfElements=2
 *            Type = uint32
 *            DataSource = FromMaster
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
 *        AckOut = {
 *            Trigger = 1
 *            Type = uint32
 *            NumberOfElements=2
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
class DoubleHandshakeSlaveGAM: public GAM {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    DoubleHandshakeSlaveGAM();

    /**
     * @brief Destructor
     */
    virtual ~DoubleHandshakeSlaveGAM();

    /**
     * @brief Initialises the GAM.
     * @details The user can specify the parameter:\n
     *    - Timeout: the timeout in milliseconds of the handshake procedure. If the timeout expires in SENDING or
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
     * DONE state
     */
    static const uint8 DONE=2u;

    /**
     * ERROR state
     */
    static const uint8 ERROR=3u;

    /**
     * The number of input commands
     */
    uint32 numberOfInputCommands;

    /**
     * The size of the input commands
     */
    uint32 *inputCommandSize;

    /**
     * The offset of the command signals in the memory buffer
     */
    uint32 *inputCommandOffset;

    /**
     * The offset of the ack signals in the memory buffer
     */
    uint32 *outputAckOffset;

    /**
     * The offset of the clear triggers signals in the memory buffer
     */
    uint32 *inputClearTrigOffset;

    /**
     * The input memory buffer
     */
    uint8 *inputs;

    /**
     * The output memory buffer
     */
    uint8 *outputs;

    /**
     * A buffer to store the previous input values
     */
    uint8 *previousValue;

    /**
     * Maps the states on the output buffer
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

#endif /* SOURCE_COMPONENTS_GAMS_DOUBLEHANDSHAKEGAM_DOUBLEHANDSHAKESLAVEGAM_H_ */

