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
 * @brief Test all the MDSObjectConnection methods
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
     * @name Test methods
     */
    //@{

    bool TestInitialise();                     //!< @brief Tests the MDSObjectConnectionTest::Initialise method
    bool TestInitialise_ThinClient();          //!< @brief Tests the Initialise method with the Thin client
    bool TestInitialise_DistributedClient();   //!< @brief Tests the Initialise method with the Distributed client
    bool TestInitialise_TargetDim();           //!< @brief Tests the usage of the parameter TargetDim
    bool TestInitialise_StartIdxStopIdx();     //!< @brief Tests the usage of the parameters StartIdx and StopIdx
    bool TestInitialise_UnlinkedParameter();   //!< @brief Tests an unlinked parameter

    bool TestInitialise_NestedParameters();              //!< @brief Tests the correct loading of nested parameters
    bool TestInitialise_NestedParameters_WithDash();     //!< @brief Tests the correct loading of nested parameters declared with a dash

    bool TestInitialise_RowMajor();                      //!< @brief Tests the correct loading of row-major parameters
    bool TestInitialise_ColMajor();                      //!< @brief Tests the correct loading of column-major parameters

    bool TestInitialise_String();                        //!< @brief Tests the loading of a string parameter

    bool TestInitialise_Struct_RowMajor();               //!< @brief Tests the loading of a structured parameter from a structured node in row-major format.

    bool TestInitialise_DictAsStruct_RowMajor();         //!< @brief Tests the loading of a structured parameter from a Dictionary node in row-major format
    bool TestInitialise_DictAsStruct_ColMajor();         //!< @brief Tests the loading of a structured parameter from a Dictionary node in column-major format

    bool TestInitialise_StructArray_RowMajor();          //!< @brief Tests the loading of a struct array parameter in row-major format
    bool TestInitialise_StructArray_ColMajor();          //!< @brief Tests the loading of a struct array parameter in column-major format

    bool TestInitialise_NoTree_Failed();                 //!< @brief Tests the correct erroring when no Tree is specified
    bool TestInitialise_NoShot_Failed();                 //!< @brief Tests the correct erroring when no Shot is specified
    bool TestInitialise_ThinClient_NoServer_Failed();    //!< @brief Tests the correct erroring when Thin client is specified but no Server is declared
    bool TestInitialise_WrongClient_Failed();            //!< @brief Tests the correct erroring when a wrong client type is declared
    bool TestInitialise_NoParameters_Failed();           //!< @brief Tests the correct erroring when no Parameters node is declared
    bool TestInitialise_NoPath_Failed();                 //!< @brief Tests the correct erroring when no Path is specified
    bool TestInitialise_WrongOrientation_Failed();       //!< @brief Tests the correct erroring when a wrong orientation type is declared
    bool TestInitialise_TargetDimAndStartIdx_Failed();   //!< @brief Tests the correct erroring when both TargetDim and StartIdx are specified
    bool TestInitialise_WrongPath_Failed();              //!< @brief Tests the correct erroring when an invalid node path is declared
    bool TestInitialise_InvalidListItems_Failed();       //!< @brief Tests the correct erroring when a MDSplus::List contains invalid items
    bool TestInitialise_Invalid4DMatrix_Failed();        //!< @brief Tests the correct erroring when a matrix with dimensions > 3 is declared
    bool TestInitialise_UnsupportedDataType_Failed();    //!< @brief Tests the correct erroring when an unsupported data type is specified
    bool TestInitialise_StringColMajor_Failed();         //!< @brief Tests the correct erroring a string parameter is declared column-major

    //@}

    ConfigurationDatabase referenceCdbRowMajor;
    ConfigurationDatabase referenceCdbColMajor;
    ConfigurationDatabase referenceCdbAdditional;

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

    bool TestParameterLoading(MDSObjectConnection& loader, ConfigurationDatabase& referenceCdb);

    /**
    * @brief Reference values of the parameters.
    */
    MARTe::StreamString standardParameters;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MDSOBJECTCONNECTIONTEST_H_ */

