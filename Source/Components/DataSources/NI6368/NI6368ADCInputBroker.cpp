/**
 * @file NI6368ADCInputBroker.cpp
 * @brief Source file for class NI6368ADCInputBroker
 * @date 13/01/2017
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

 * @details This source file contains the definition of all the methods for
 * the class NI6368ADCInputBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI6368ADCInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

NI6368ADCInputBroker::NI6368ADCInputBroker() :
        MemoryMapStatefulBroker() {
    adcBoard = NULL_PTR(NI6368ADC *);
}

NI6368ADCInputBroker::NI6368ADCInputBroker(NI6368ADC * const adcBoardIn) :
        MemoryMapStatefulBroker() {
    adcBoard = adcBoardIn;
}

/*lint -e{1540} the adcBoard is freed by the DataSource*/
NI6368ADCInputBroker::~NI6368ADCInputBroker() {

}

bool NI6368ADCInputBroker::Execute() {
    uint32 n;
    bool ret = true;
    if (adcBoard != NULL_PTR(NI6368ADC *)) {
        if (adcBoard->IsSynchronising()) {
            ret = adcBoard->Synchronise();
        }

        uint32 idx = adcBoard->GetLastBufferIdx();
        void *dataSourceSignalPointer;
        for (n = 0u; (n < numberOfCopies) && (ret); n++) {
            if (copyTable != NULL_PTR(MemoryMapStatefulBrokerCopyTableEntry *)) {
                /*lint -e{9025}  [MISRA C++ Rule 5-0-19]. Justification: two pointer indirection required to access the address of the variable that holds the final address of the double buffer*/
                dataSourceSignalPointer = copyTable[n].dataSourcePointer[idx];
                ret = MemoryOperationsHelper::Copy(copyTable[n].gamPointer, dataSourceSignalPointer, copyTable[n].copySize);
            }
        }
    }
    return ret;
}

CLASS_REGISTER(NI6368ADCInputBroker, "1.0")

}

