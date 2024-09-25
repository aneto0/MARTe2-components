/**
 * @file ObjectLoader.h
 * @brief Header file for class ObjectLoader
 * @date 16/09/2024
 * @author Cristian Galperti
 * @author Nicolo Ferron
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
 *
 * @details This header file contains the declaration of the class ObjectLoader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTLOADER_H_
#define OBJECTLOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "AdvancedErrorManagement.h"
#include "MessageI.h"
#include "ObjectConnectionI.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief   Loader class that loads parameters from various sources and makes
 *          them available as AnyObject.
 * @details This class is a ReferenceContainer of MARTe::ObjectConnectionI.
 *          For each parameter
 *          declared in the connections, this class inserts in itself
 *          an AnyObject that holds the value of the parameter as
 *          retrieved from the connections.
 *          The parameter is saved as an AnyObject for compatibility with
 *          the rest of the framework: by knowing the name of the instance
 *          of a ObjectLoader class, a Find("instanceName.paramName")
 *          will be enough to retrieve the parameter from any other
 *          object of the framework.
 *
 * This object shall contain only objects inheriting from MARTe::ObjectConnectionI
 * (e.g. ConfigurationDatabaseConnection, MDSObjectConnection).
 *
 * The ObjectLoader shares its configuration parameters with the contained
 * Connections, that is, all parameters declared inside the ObjectLoader
 * (except for the `Class` parameter) shall be also copied in the
 * configuration database of the contained Connections unless a parameter
 * of the same name already exists in the Connection.
 *
 * @warning The MDSObjectConnection library is compiled only if the
 *          `MDSPLUS_DIR` environmental variable is defined and is
 *          is pointing to a valid MDSplus installation directory.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * The configuration syntax is (names and signal quantity are only given
 * as an example):
 *
 * +ParamLoader1 = {
 *     Class = ObjectLoader                         // Compulsory
 *     Shot = "-1"                                  // Optional if defined in MDSObjectConnection
 *
 *     // All parameters declared here (except "Class") are also copied inside the Connections.
 *
 *     +MDSConnection1 = {
 *         Class = MDSObjectConnection              // Compulsory
 *         ClientType = ( "Thin" | "Distributed" )  // Optional. Default: "Thin"
 *         Server = "localhost:8000"                // Compulsory if ClientType = "Thin"
 *         Tree = "tree_name"                       // Compulsory
 *
 *         Parameters = {
 *             // Parameters can be declared in a structured fashion:
 *             gain1 = {
 *                 Path  = "\\TREE_NAME::TOP:NODE1"                     // Compulsory
 *             }
 *             gainStruct1 = {
 *                 gain_a = { Path  = "\\TREE_NAME::TOP:NODE2" }        // Compulsory
 *                 gain_b = { Path  = "\\TREE_NAME::TOP:NODE3" }        // Compulsory
 *             }
 *
 *             // Parameters can also be declared in a flattened fashion:
 *             gainStruct2.gain_one = { Path  = "\\TREE_NAME::TOP:NODE4" }
 *             gainStruct2.gain_two = { Path  = "\\TREE_NAME::TOP:NODE5" }
 *
 *             // Both dots `.` and dashes `-` can be used as separatos:
 *             gainStruct3-gain_1 = { Path  = "\\TREE_NAME::TOP:NODE6" }
 *             gainStruct3-gain_2 = { Path  = "\\TREE_NAME::TOP:NODE7" }
 *         }
 *     }
 *
 *     +CDBConnection2 = {
 *         Class = ConfigurationDatabaseConnection
 *         Parameters = {
 *             gain4 = (float64) 1
 *             gainStruct4 = {
 *                 gain_a = (uint32) 10
 *                 gain_b = (float32) {1, 2, 3, 4}
 *             }
 *         }
 *     }
 *
 *     +Connection3 = {
 *         ...
 *     }
 * }
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * @note Structured nodes can be declared in a structured fashion or in
 *       a flattened fashion. When flattened, both dots (`.`) and dashes
 *       (`-`) can be used as separators.
 *
 * @warning Do not use `-` character for node names except for denoting structures
 *          (see note above).
 *
 *
 */
class ObjectLoader : public ReferenceContainer, public MessageI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    ObjectLoader();

    /**
     * @brief Destructor.
     */
    virtual ~ObjectLoader();

    /**
     * @brief
     */
    virtual bool Initialise(StructuredDataI & data);

private:

    ErrorManagement::ErrorType status;

};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTLOADER_H_ */
