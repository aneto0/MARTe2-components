/**
 * @file ConfigurationDatabaseConnectionTest.h
 * @brief Header file for class ConfigurationDatabaseConnectionTest
 * @date 15/5/2025
 * @author nferron
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

 * @details This header file contains the declaration of the class ConfigurationDatabaseConnectionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONDATABASECONNECTIONTEST_H_
#define CONFIGURATIONDATABASECONNECTIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "ConfigurationDatabaseConnection.h"
#include "DataSourceI.h"
#include "Directory.h"
#include "ErrorInformation.h"
#include "ErrorManagement.h"
#include "FastMath.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "GlobalObjectsDatabase.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryOperationsHelper.h"
#include "ObjectConnectionI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "SafeMath.h"
#include "StandardParser.h"
#include "StaticList.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Test all the ConfigurationDatabaseConnectionTest methods
 */
class ConfigurationDatabaseConnectionTest {
public:

    /**
     * @brief Constructor
     */
    ConfigurationDatabaseConnectionTest();

    /**
     * @brief Destructor
     */
    virtual ~ConfigurationDatabaseConnectionTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the ConfigurationDatabaseConnectionTest::Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the ConfigurationDatabaseConnectionTest::Initialise method
     */
    bool TestInitialise_ParametersValues();

    /**
     * @brief Tests the ConfigurationDatabaseConnectionTest::Initialise method
     */
    bool TestInitialise_DotNotation();

    /**
     * @brief Tests the ConfigurationDatabaseConnectionTest::Initialise method
     */
    bool TestInitialise_DashNotation();

    /**
     * @brief Tests the ConfigurationDatabaseConnectionTest::Initialise method
     */
    bool TestInitialise_Failed_ParametersNodeNotFound();



private:

    /**
     * @brief Template to test the Initialise() method.
     * @param[in]  configStreamIn The configuration stream to test the Initialise method.
     * @param[out] statusOut      The returned status of the interface after initialisation.
     * @param[out] cdbOut         The returned configuration database after initalisation.
     */
    bool TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut, ConfigurationDatabaseConnection& loader);

    /**
     * @brief Same, but does not return the configuration database.
     */
    bool TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONDATABASECONNECTIONTEST_H_ */

