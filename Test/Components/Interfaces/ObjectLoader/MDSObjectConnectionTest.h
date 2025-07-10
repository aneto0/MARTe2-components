/**
 * @file MDSObjectConnectionTest.h
 * @brief Header file for class MDSObjectConnectionTest
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

 * @details This header file contains the declaration of the class MDSObjectConnectionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MDSOBJECTCONNECTIONTEST_H_
#define MDSOBJECTCONNECTIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "MDSObjectConnection.h"
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
#include "MDSObjectConnection.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "SafeMath.h"
#include "StandardParser.h"
#include "StaticList.h"
#include "StreamString.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

// class TestMDSObjectConnection : public MARTe::MDSObjectConnection {
// public:
//     CLASS_REGISTER_DECLARATION()
//
//     TestMDSObjectConnection();
//
//     ~TestMDSObjectConnection();
//
//     ErrorManagement::ErrorType TestTransposeAndCopy(void *const destination, const void *const source, const TypeDescriptor typeDesc,
//         const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages);
//
// };


/**
 * @brief Test all the MDSObjectConnectionTest methods
 */
class MDSObjectConnectionTest {
public:

    /**
     * @brief Constructor
     */
    MDSObjectConnectionTest();

    /**
     * @brief Destructor
     */
    virtual ~MDSObjectConnectionTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the MDSObjectConnectionTest::Initialise method
     */
    bool TestInitialise();

    bool TestInitialise_DictAsStruct_RowMajor();

    ConfigurationDatabase referenceCdb;

private:

    /**
     * @brief Template to test the Initialise() method.
     * @param[in]  configStreamIn The configuration stream to test the Initialise method.
     * @param[out] statusOut      The returned status of the interface after initialisation.
     * @param[out] cdbOut         The returned configuration database after initalisation.
     */
    bool TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut, MDSObjectConnection& loader);

    /**
     * @brief Same, but does not return the configuration database.
     */
    bool TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut);

    /**
    * @brief Reference values of the parameters.
    */
    MARTe::StreamString standardParameters;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSOBJECTCONNECTIONTEST_H_ */

