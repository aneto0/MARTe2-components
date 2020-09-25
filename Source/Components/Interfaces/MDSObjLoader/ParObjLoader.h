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

#ifndef PAROBJLOADER_H_
#define PAROBJLOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ConfigurationDatabase.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


class ParObjLoader: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    ParObjLoader();
    virtual ~ParObjLoader();
    virtual bool Initialise(StructuredDataI &data);
    inline ConfigurationDatabase &getCdb() { return privatecdb; }

protected:

    ConfigurationDatabase privatecdb;
private:
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PAROBJLOADER_H_ */

