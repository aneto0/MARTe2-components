/**
 * @file OPCUAObject.h
 * @brief Header file for class OPCUAObject
 * @date 12/03/2019
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class OPCUAObject
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAOBJECT_H_
#define SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAOBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "open62541.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "OPCUAReferenceContainer.h"
#include "ReferenceContainer.h"
#include "string.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Class that manages the OPCUA Object structure
 * @details The class inherit from OPCUAReferenceContainer and implements the GetOPCObject method
 */
class OPCUAObject: public OPCUAReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

/**
 * @brief Default constructor
 */
OPCUAObject    ();

    /**
     * @brief Default Destructor
     */
    ~OPCUAObject();

    /**
     * @see OPCUAReferenceContainer::GetOPCObject
     * @return true if all the parameters and attributes are set correctly
     */
    virtual bool GetOPCObject(OPCUAObjectSettings &settings, uint32 nodeNumber);

    /**
     * @see OPCUAReferenceContainer::IsObject
     * @return true
     */
    virtual bool IsObject();

    /**
     * @see OPCUAReferenceContainer::SetFirst
     */
    virtual void SetFirst(const bool value);

    /**
     * @see OPCUAReferenceContainer::IsFirstObject
     * @return true
     */
    virtual const bool IsFirstObject();

private:

    /**
     * Boolean flag that indicates if the current Node is the first of the Address Space
     */
    bool isFirstObject;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_COMPONENTS_INTERFACES_OPCUA_OPCUAOBJECT_H_ */

