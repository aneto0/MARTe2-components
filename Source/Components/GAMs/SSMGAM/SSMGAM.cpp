/**
 * @file SSMGAM.cpp
 * @brief Source file for class SSMGAM
 * @date 01/09/2017
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
 * the class SSMGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SSMGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

SSMGAM::SSMGAM() {
//Auto-generated constructor stub for SSMGAM

//TODO Verify if manual additions are needed here
}

SSMGAM::~SSMGAM() {
//Auto-generated destructor stub for SSMGAM

//TODO Verify if manual additions are needed here
}

bool SSMGAM::Initialise(StructuredDataI &data){
    bool ok = GAM::Initialise(data);
    if(ok){

    }
    return ok;
}

bool SSMGAM::Setup(){
    bool ok = true;
    return ok;
}

bool SSMGAM::Execute(){
    return true;
}
CLASS_REGISTER(SSMGAM, "1.0")
}

