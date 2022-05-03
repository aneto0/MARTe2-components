/**
 * @file TriggeredIOGAM.h
 * @brief Header file for class TriggeredIOGAM
 * @date 25/03/2022
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class TriggeredIOGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TRIGGEREDIOGAM_H_
#define TRIGGEREDIOGAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"
#include "GAM.h"
#include "StatefulI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief A "triggered" IOGAM. This GAM will copy its inputs to its outputs AND will
 *  only enabled its output brokers iff a trigger signal is set to 1.
 *
 * @details The Trigger signal shall exist, shall be of type uint8 and shall be in position 0.
 * 
 * The configuration syntax is (names and signal quantity are only given as an example):
 * <pre>
 * +Buffer = {
 *     Class = TriggeredIOGAM
 *     InputSignals = {
 *         Trigger = {
 *             DataSource = "DDB1"
 *             Type = uint8
 *         }
 *         Signal1 = {
 *             DataSource = "Drv1"
 *             Type = uint64
 *         }
 *     }
 *     OutputSignals = {
 *         Signal1O = {
 *             DataSource = "LCD"
 *             Type = uint32
 *         }
 *         Signal2O = {
 *             DataSource = "LCD"
 *             Type = int32
 *         }
 *     }
 * }
 * </pre>
 */
class TriggeredIOGAM: public GAM, public StatefulI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    TriggeredIOGAM();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TriggeredIOGAM();

    /**
     * @brief Checks that the total input signal memory size is equal to the total output signal memory size, without taking into 
     * account the Trigger signal.
     * Verifies that a Trigger signal of type uint8 exists in the zero position.
     * @return true is the pre-conditions are met.
     */
    virtual bool Setup();

    /**
     * @brief Checks if the trigger signal is set to 1. If so, it enables the OutputBrokers and copies the input 
     * memory to the output memory (without including the Trigger signal). If the trigger signal is set to zero, the OutputBrokers are disabled and the memory is not copied.
     * @return true if all the signals memory can be successfully copied.
     */
    virtual bool Execute();


    /**
     * @brief Initialise the broker accelerators.
     * @param[in] currentStateName ignored.
     * @param[in] nextStateName ignored.
     * @return true.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

private:
    /**
     * Total number of bytes to copy.
     */
    uint32 totalSignalsByteSize;

    /**
     * Output brokers - accelerator.
     */
    BrokerI **outputBrokersAccel;

    /**
     * Number of output brokers.
     */
    uint32 nOfOutputBrokers;

    /**
     * Trigger signal.
     */
    uint8 *triggerSignal;

    /**
     * The input signal memory source without taking into account the Trigger signal
     */
    uint8 *inputSignalsMemoryNoTrigger;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TRIGGEREDIOGAM_H_ */

