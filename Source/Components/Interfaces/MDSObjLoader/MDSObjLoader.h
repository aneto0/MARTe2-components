/**
 * @file MDSObjLoader.h
 * @brief Header file for class MDSObjLoader
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

#ifndef MDSOBJLOADER_H_
#define MDSOBJLOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ConfigurationDatabase.h"
#include "ReferenceT.h"

#include "MDSObjConnection.h"
#include "MDSParameters.h"
#include "ParObjLoader.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * @brief   Loader class that loads parameters from MDSplus and makes
 *          them available as AnyObject.
 * @details This class is a ReferenceContainer. For each parameter
 *          declared in its configuration, this class inserts in itself
 *          an AnyObject that holds the value of the parameter as
 *          retrieved from MDSplus.
 *          The parameter is saved as an AnyObject for compatibility with
 *          the rest of the framework: by knowing the name of the instance
 *          of a MDSObjLoader class, a Find("instanceName.paramName")
 *          will be enough to retrieve the parameter from any other
 *          object of the framework.
 */
class MDSObjLoader: public ParObjLoader {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MDSObjLoader();

    /**
     * @brief Destructor
     */
    virtual ~MDSObjLoader();

    /**
     * @see ReferenceContainer::Initialise()
     */
    virtual bool Initialise(StructuredDataI &data);

   inline ConfigurationDatabase &getCdb() { return privatecdb; }

protected:

private:

    int32 shot;

    //MDSplus::Tree *currenttree;
    //void *currenttree;


    //MDSStructuredDataI mdsi;



};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSOBJLOADER_H_ */

