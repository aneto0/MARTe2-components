/**
 * @file MemoryMapSynchNMutexInputBroker.h
 * @brief Header file for class MemoryMapSynchNMutexInputBroker
 * @date 15/01/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class MemoryMapSynchNMutexInputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_MEMMAPSYNCHNMUTEXINPUTBROKER_H_
#define DATASOURCES_PROFINET_MEMMAPSYNCHNMUTEXINPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ISynchronisableInput.h"
#include "MemoryMapInputBroker.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
    * @brief    Specialization of the MemoryMapInputBroker, which leverages ISynchronisableInput
    *           interface, implemented by the DataSource, to synchronise inputs.
    *           Before starting with the copy the Synchronise is called on inputs (SynchroniseInput)
    *           while the DataSource is expected to mutually exclude memory access, until the broker signals
    *           using the TerminateInputCopy its ending.
    */
    class MemoryMapSynchNMutexInputBroker : public MemoryMapInputBroker {

        public:
            CLASS_REGISTER_DECLARATION()

            /**
             * @brief Constructs the MemoryMapSynchNMutexInputBroker instance.
             *        Refer to the parent classes for detailed info.
            */
            MemoryMapSynchNMutexInputBroker();

            /**
             * @brief Destructs the MemoryMapSynchNMutexInputBroker instance.
             *        Refer to the parent classes for detailed info.
            */
            virtual ~MemoryMapSynchNMutexInputBroker();

            /**
             * @brief Runs the execute method on the DataSource, causing the input/output image copy.
             * This specific implementation locks inputs on the DataSource, using a SynchroniseInput before copying
             * and releases it with the shipped TerminateInputCopy, used without input parameters.
            */
            virtual bool Execute();

    };
    
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_MEMMAPSYNCHNMUTEXINPUTBROKER_H_ */

