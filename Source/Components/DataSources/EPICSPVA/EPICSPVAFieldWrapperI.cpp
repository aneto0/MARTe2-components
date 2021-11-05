/**
 * @file EPICSPVAFieldWrapperI.cpp
 * @brief Source file for class EPICSPVAFieldWrapperI
 * @date 07/06/2021
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
 * the class EPICSPVAFieldWrapperI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <pv/pvData.h>
#include <pva/client.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EPICSPVAFieldWrapperI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

void EPICSPVAFieldWrapperI::SetMemory(uint32 numberOfElementsIn, const char8 *qualifiedNameIn, void *memoryIn) {
    qualifiedName = qualifiedNameIn;
    numberOfElements = numberOfElementsIn;
    memory = memoryIn;
}

void EPICSPVAFieldWrapperI::SetPVAField(epics::pvData::PVFieldPtr pvFieldIn) {
    pvField = pvFieldIn;
}

EPICSPVAFieldWrapperI::EPICSPVAFieldWrapperI() {
    numberOfElements = 0u;
    memory = NULL_PTR(void *);
    qualifiedName = "";
}

EPICSPVAFieldWrapperI::~EPICSPVAFieldWrapperI() {
}
}

