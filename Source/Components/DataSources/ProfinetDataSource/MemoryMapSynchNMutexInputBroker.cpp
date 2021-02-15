/**
 * @file MemoryMapSynchNMutexInputBroker.cpp
 * @brief Source file for class MemoryMapSynchNMutexInputBroker
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapSynchNMutexInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    MemoryMapSynchNMutexInputBroker::MemoryMapSynchNMutexInputBroker() : MemoryMapInputBroker() {

    }
    
    MemoryMapSynchNMutexInputBroker::~MemoryMapSynchNMutexInputBroker() {

    }

    bool MemoryMapSynchNMutexInputBroker::Execute() {
        bool returnValue = true;

        if(dataSource != NULL_PTR(DataSourceI *)) {
	    //lint -e{740} Dynamic casting the DataSource to the ISynchronisable part of it, which is of interest of this particular broker
            ISynchronisableInput *synchDataSource = dynamic_cast<ISynchronisableInput*>(dataSource);
            if(synchDataSource != NULL_PTR(ISynchronisableInput*)) {
                returnValue = synchDataSource->SynchroniseInput();
                if(returnValue) {
                    returnValue = MemoryMapInputBroker::Execute();
                    bool unlockResult = dataSource->TerminateInputCopy(0u, 0u, 0u);
                    returnValue = returnValue && unlockResult;
                }
            }
            else {
                returnValue = false;
            }
        }

        return returnValue;
    }

    CLASS_REGISTER(MemoryMapSynchNMutexInputBroker, "1.0")
}
