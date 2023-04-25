/**
 * @file UEIMockupManager.h
 * @brief Header file for class UEIMockupManager
 * @date 22/03/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEIMockupManager
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIMockupManager_H_
#define UEIMockupManager_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
//MARTe includes
#include "StructuredDataI.h"
#include "StructuredDataIHelper.h"
#include "ReferenceContainer.h"
#include "StreamString.h"
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"

//Interface specific includes
#include "UEIDevice.h"
#include "UEIMapContainer.h"
#include "UEIDefinitions.h"

//PowerDNA includes
#include "PDNA.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class responsible to represent and manage a UEIDAQ device (Whole device, cpu + hardware layers installed)
 * @details This class implements the needed methods to manage and communicate with a UEIDAQ device. The class
 */
class UEIMockupManager : public Object {
    public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details NOOP.
     */
    UEIMockupManager();

    /**
     * @brief Default destructor.
     * @details NOOP.
     */
    virtual ~UEIMockupManager();

    /**
     * @brief Initialise the Object from a configuration file.
     * @details Reads the parameters from a ConfigurationDatabase and check
     * their validity.
     * @return true if every parameter has been read correctly and validated.
     */
    virtual bool Initialise(StructuredDataI &data);

protected:
    StreamString name;
    UEIDAQMockupManager* mockup;
    IOM* myIOM;
    uint32* Devices;
};
}
#endif /* UEIMockupManager_H_ */