/**
 * @file LinuxTimer.h
 * @brief Header file for class LinuxTimer
 * @date 24/10/2016
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

 * @details This header file contains the declaration of the class LinuxTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINUXTIMER_H_
#define LINUXTIMER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "EventSem.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief TODO
 * @details TODO
 */
class LinuxTimer: public DataSourceI, public EmbeddedServiceMethodBinderI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief TODO
     */
LinuxTimer    ();

    /**
     * @brief TODO
     */
    virtual ~LinuxTimer();

    /**
     * @brief TODO
     */
    virtual bool AllocateMemory();

    /**
     * @brief TODO
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief TODO
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    /**
     * @brief TODO
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief TODO
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief TODO
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    /**
     * @brief TODO
     */
    virtual bool Synchronise();

    /**
     * @brief TODO
     */
    virtual ErrorManagement::ErrorType Execute(const ExecutionInfo & info);

    /**
     * @brief TODO
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief TODO
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief TODO
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

private:
    /**
     * @brief TODO
     */
    enum LinuxTimerSleepNature {
        Default = 0,
        Busy = 1
    };

    /**
     * @brief TODO
     */
    LinuxTimerSleepNature sleepNature;

    /**
     * TODO
     */
    uint32 counterAndTimer[2];

    /**
     * TODO
     */
    EventSem synchSem;

    /**
     * TODO
     */
    SingleThreadService executor;

    /**
     * TODO
     */
    uint64 lastTimeTicks;

    /**
     * TODO
     */
    uint64 sleepTimeTicks;

    /**
     * TODO
     */
    uint32 synchronisingFunctionIdx;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINUXTIMER_H_ */

