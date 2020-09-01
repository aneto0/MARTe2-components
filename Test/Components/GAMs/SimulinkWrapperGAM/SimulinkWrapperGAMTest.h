/**
 * @file SimulinkWrapperGAMTest.h
 * @brief Header file for class SimulinkWrapperGAMTest
 * @date 22/03/2018
 * @author Bertrand Bauvir
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

 * @details This header file contains the declaration of the class SimulinkWrapperGAMTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIMULINKWRAPPERGAMTEST_H_
#define SIMULINKWRAPPERGAMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "StreamString.h"

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "Directory.h"
#include "ErrorInformation.h"
#include "ErrorManagement.h"
#include "FastMath.h"
#include "GAM.h"
#include "GlobalObjectsDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "SimulinkWrapperGAM.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the ConstantGAM public methods.
 */
class SimulinkWrapperGAMTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    //SimulinkWrapperGAMTest();

    /**
     * @brief Destructor. NOOP.
     */
    //virtual ~SimulinkWrapperGAMTest();

    /**
     * @brief  Tests the default constructor
     * @return `true` after GAM instantiation.
     */
    bool TestConstructor();
    
    /**
     * @brief   Tests the Initialise() method
     * @details Verify the post-conditions
     * @return  `true` if Initialise() succeeds.
     */
    bool TestInitialise();
    
    /**
     * @brief   Tests the Setup() method
     * @details Verify the post-conditions
     * @return  `true` if Setup() succeeds.
     */
    bool TestSetup();

    /**
     * @brief  Tests the Execute() method
     * @return `true` if Execute() succeeds.
     */
    bool TestExecute();
    
    /**
     * @brief Helper method to test Initialise by passing a ConfigurationDatabase.
     */
    bool TestInitialiseWithConfiguration(MARTe::ConfigurationDatabase configIn);
    
    bool TestInitialise_MissingOptionalConfigurationSettings();
    
    bool TestInitialise_Failed_MissingLibrary();
    bool TestInitialise_Failed_MissingSymbolPrefix();
    bool TestInitialise_Failed_MissingTunableParamExternalSource();
    bool TestInitialise_Failed_LoadLibrary();
    bool TestInitialise_Failed_LoadSymbols();
    
    bool TestInitialise_MissingParametersLeaf();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIMULINKWRAPPERGAMTEST_H_ */

