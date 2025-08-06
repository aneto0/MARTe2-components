/**
 * @file ConfigurationDatabaseConnection.h
 * @brief Header file for class ConfigurationDatabaseConnection
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
 * @details This header file contains the declaration of the class ConfigurationDatabaseConnection
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSOBJECTCONNECTION_H_
#define MDSOBJECTCONNECTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "ObjectConnectionI.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Loads MARTe::AnyObject from configuration file
 * @details This class is a StaticList<AnyType*>. It populates itself
 *          with the information declared in the `Parameters` node
 *          of its configuration (see below).
 *
 * The StaticList<AnyType*> is meant to be read and serialised by a
 * MARTe::ObjectLoader class that makes the parameters available
 * to any component of the RealTimeApplication.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * The configuration syntax is (names and signal quantity are only given
 * as an example):
 *
 * +ParamLoader1 = {
 *     Class = ObjectLoader                         // Compulsory
 *
 *     // All parameters declared here (except "Class") are also copied inside the Connections.
 *
 *     +CDBConnection2 = {
 *         Class = ConfigurationDatabaseConnection
 *         Parameters = {
 *             scalarParam   = (float64) 1
 *             vectorParam   = (int16)   {1, 2, 3}
 *             matrixParam   = (uint32)  { {1, 2, 3, 4}, {5, 6, 7, 8} }  // 2x3 matrix
 *             structParam = {
 *                 structScalar = (uint32) 10
 *                 structVector = (float32) {1, 2, 3, 4}
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
 * Release notes
 * ==========================================================================
 *
 * Version |    Date    | Notes
 * ------: | :--------: | :----
 * 1.0     | 16/09/2024 | initial release
 * 2.0     | 31/07/2025 | tested and linted version
 *
 */
class ConfigurationDatabaseConnection : public ObjectConnectionI {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    ConfigurationDatabaseConnection();

    /**
     * @brief Destructor.
     */
    virtual ~ConfigurationDatabaseConnection();

    /**
     * @brief Initialise
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief All the stored `AnyType`s are deleted and reloaded from source.
     */
    virtual ErrorManagement::ErrorType UpdateParameters();

    /**
     * @brief Remove all parameters, deallocate memory.
     */
    virtual ErrorManagement::ErrorType CleanUp();

private:

    /**
     * @brief Holds a copy of the parameters database
     */
    ConfigurationDatabase parametersCdb;
};

} /* namespace MARTe */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSOBJECTCONNECTION_H_ */
