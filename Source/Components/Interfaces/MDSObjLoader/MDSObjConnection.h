/**
 * @file MDSObjConnection.h
 * @brief Header file for class MDSObjConnection
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

 * @details This header file contains the declaration of the class MDSObjLoaderGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSOBJCONNECTION_H_
#define MDSOBJCONNECTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ConfigurationDatabase.h"
#include "ReferenceT.h"
#include "StreamString.h"

#include "MDSParameters.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief
 */
class MDSObjConnection: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MDSObjConnection();

    /**
     * @brief Destructor
     */
    virtual ~MDSObjConnection();

    /**
     * @see ReferenceContainer::Initialise()
     */
    virtual bool Initialise(StructuredDataI &data);

    inline StreamString &getServer() { return server; }
    inline StreamString &getTree()   { return tree; }
    inline bool hasLocalShot() { return localshot; }
    inline int32 getLocalShot() { return shot; }

protected:

private:

    bool localshot;
    StreamString server;
    StreamString tree;
    int32 shot;

};

} /* namespace MARTe */


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSOBJCONNECTION_H_ */
