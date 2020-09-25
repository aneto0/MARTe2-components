/**
 * @file MDSParameters.cpp
 * @brief Source file for class MDSParameters
 * @date 29/05/2019
 * @author Galperti Cristian
 *
 * @copyright Copyright 2019 EPFL SPC Lausanne
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
 * the class MDSParameters (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "ObjParameters.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ObjParameter::ObjParameter() :
        ReferenceContainer() {
    valid=false;
    size=0;
    //orientation=rtwCAPI_SCALAR;
    numdims=0;
    type="";
    datatypesize=0;
    dims=NULL;
    databuffer=NULL;
    //printf("ObjParameter constructor called\n");
}

ObjParameter::~ObjParameter() {
    if(databuffer!=NULL) free(databuffer);
    if(dims!=NULL) free(dims);

}

bool ObjParameter::Initialise(StructuredDataI &data) {

    //alloca e inizializza prima tutti i figli
    bool ret = ReferenceContainer::Initialise(data);
    return ret;
}

bool ObjParameter::Actualize(ConfigurationDatabase &targetcdb) {
    return true;
}


CLASS_REGISTER(ObjParameter, "1.0")
}






