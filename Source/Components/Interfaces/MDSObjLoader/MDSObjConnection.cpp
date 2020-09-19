/**
 * @file MDSObjConnection.cpp
 * @brief Source file for class MDSObjConnection
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
 * the class MDSObjConnection (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MDSObjConnection.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MDSObjConnection::MDSObjConnection() :
        ReferenceContainer() {
    //printf("MDSObjConnection constructor called\n");

    localshot=false;

}

MDSObjConnection::~MDSObjConnection() {
}

bool MDSObjConnection::Initialise(StructuredDataI &data) {


    //alloca e inizializza prima tutti i figli

    bool ret=ReferenceContainer::Initialise(data);

    //server=""; // Server is inside the path <Tree>_path environmental variable using mdsobjects

    if(ret){
        ret=data.Read("Server", server);

        if(!ret)
        {
            REPORT_ERROR(ErrorManagement::ParametersError, "MDSObjConnection %s, Server not set", this->GetName());
        }
    }

    if(ret){
        ret=data.Read("Tree", tree);

        if(!ret)
        {
            REPORT_ERROR(ErrorManagement::ParametersError, "MDSObjConnection %s, Tree not set", this->GetName());
        }
    }

    if(ret){
        if(data.Read("Shot", shot)) localshot=true;
    }


    StreamString name = this->GetName();

    REPORT_ERROR(ErrorManagement::Information, "%s init, Server: %s, Tree: %s, number of subclasses: %d", this->GetName(), server.Buffer(), tree.Buffer(), Size());

    if(ret){
        //inizializzi il blocco padre (leggi i parametri di connessione qui)
       // ...
        for(uint32 i=0; i<Size(); i++){
            ReferenceT<MDSParameter> ref=Get(i);
            if(ref.IsValid()){
                ref->SetConnection(server, tree, name);
            }
        }
    }


    return ret;
}

CLASS_REGISTER(MDSObjConnection, "1.0")

}
