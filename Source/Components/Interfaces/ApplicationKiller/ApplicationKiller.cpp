/**
 * @file ApplicationKiller.cpp
 * @brief Source file for class ApplicationKiller
 * @date 24/04/2025
 * @author Llorenc Capella
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
 * the class ApplicationKiller (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "signal.h"
#include "ApplicationKiller.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

//lint -e{534} Ignoring return value. In the constructor there is no return value.
ApplicationKiller::ApplicationKiller() :
        ReferenceContainer(),
        MessageI() {
    filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    filter->SetDestination(this);
    //lint -e{534} Ignoring return value. In the constructor there is no return value.
    MessageI::InstallMessageFilter(filter);
}

ApplicationKiller::~ApplicationKiller() {
}

//lint -e{1762} Member function can be constant. Cannot be constant according to the compiler.
ErrorManagement::ErrorType ApplicationKiller::Kill(){
    //if (kill(0, SIGUSR1) == 0) { //0 --> terminate all process in the current PGID. Therefore it kills the script because this process belongs to the PGID of the script as well
    if (kill(getpid(), SIGUSR1) == 0) {
        REPORT_ERROR(ErrorManagement::Information, "Signal successfully send");
    }
    else {
        REPORT_ERROR(ErrorManagement::Information, "Signal sending error");
    }
    return true;
}

CLASS_REGISTER(ApplicationKiller, "1.0")
CLASS_METHOD_REGISTER(ApplicationKiller, Kill)

}
