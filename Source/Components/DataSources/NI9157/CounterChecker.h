/**
 * @file CounterChecker.h
 * @brief Header file for class CounterChecker.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class
 * CounterChecker with all of its public, protected and private members. It
 * may also include definitions for inline methods which need to be visible
 * to the compiler.
 */

#ifndef NI9157COUNTERCHECKER_H_
#define NI9157COUNTERCHECKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "SampleChecker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The CounterChecker class.
 * @see CounterChecker::Initialise for details on the configuration parameters.
 */
class CounterChecker: public SampleChecker {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @post
     *  packetCounter = 1u;
     *  acquireFromCounter = 0u;
     *  nextPacketCheck = 1u;
     *  checkCounterAfterNSteps = 0u;
     *  counterStep = 1u;
     */
    CounterChecker();

    /**
     * @brief Destructor.
     */
    virtual ~CounterChecker();

    /**
     * @see SampleChecker::Initialise.
     * @details Follows the list of parameters to be configured by the user
     * for initialisation: \n
     * FirstPacketCounter: Index of the first packet in counter. If not 
     * defined, the default value is 1. \n
     * AcquireFromCounter: Wait for this counter value to start saving data. If
     *  not defined, the default value is the FirstPacketCounter value. \n
     * CounterStep: The step used in the counter interations. Default is value
     * is 1 and the user set value must be greater than zero. \n
     * CheckCounterAfterNSteps: After how many counts should the check be
     * perfromed. If not defined, the default value is the CounterStep value.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see SampleChecker::Check.
     * @details Checks if the value of sample corresponds to the value of the
     * packet counter.
     * @param[in] sample is the sample to be checked against the value of the
     * class internal packetCounter.
     * @param[in,out] write is true unless the sample and the AcquireFromCounter
     * values do not match.
     * @return true if the packetCounter matches the sample value, otherwise
     * returns false.
     */
    virtual bool Check(uint8 *sample,
                       bool &write);

    /**
     * @see SampleChecker::Synchronise.
     */
    virtual bool Synchronise(uint8 *frames,
                               uint32 sizeToRead,
                               uint32 &idx,
                               bool &write);

protected:

    /**
     * The packet counter.
     */
    uint64 packetCounter;

    /**
     * The first packet to be acquired and put in the circular buffer.
     */
    uint64 acquireFromCounter;

    /**
     * The next packet counter to be checked.
     */
    uint64 nextPacketCheck;

    /**
     * The difference between two consecutive checked counter.
     */
    uint32 checkCounterAfterNSteps;

    /**
     * The difference between two consecutive acquired counter.
     */
    uint32 counterStep;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* NI9157COUNTERCHECKER_H_ */
