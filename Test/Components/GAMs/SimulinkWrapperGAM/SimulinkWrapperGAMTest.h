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
#include "StaticList.h"
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
    bool TestInitialise_MissingTunableParamExternalSource();
    bool TestInitialise_MissingParametersLeaf();
    
    bool TestInitialise_Failed_MissingLibrary();
    bool TestInitialise_Failed_MissingSymbolPrefix();
    bool TestInitialise_Failed_LoadLibrary();
    bool TestInitialise_Failed_LoadSymbols();
    
    /**
     * @brief Tests if the GAM stops when a library has no
     *        custom Getmmi function (see GAM documentation for details
     *        about custom Getmmi function).
     */
    bool TestInitialise_Failed_LibraryMissingGetMmiFunction();
    
    /**
     * @brief Tests if the GAM stops when the library has been compiled
     *        without the option to generate an additional function
     *        for memory allocation.
     */
    bool TestInitialise_Failed_LibraryMissingAllocFunction();
    
    /**
     * @brief General method to test GAM setup. The method allows to
     *        specify various details of the configuration file.
     */
    bool TestSetupWithTemplate(MARTe::StreamString scriptCall,
                               MARTe::StreamString skipUnlinkedParams,
                               MARTe::StreamString inputSignals,
                               MARTe::StreamString outputSignals,
                               MARTe::StreamString parameters);
    
    bool TestSetup_StructTunableParameters();
    bool TestSetup_NoTunableParameters();
    
    bool TestSetup_SkipUnlinkedTunableParams();
    
    bool TestSetup_Failed_DontSkipUnlinkedTunableParams();
    bool TestSetup_Failed_WrongNumberOfInputs();
    bool TestSetup_Failed_WrongNumberOfOutputs();
    bool TestSetup_Failed_NoInputs();
    bool TestSetup_Failed_NoOutputs();
    bool TestSetup_Failed_StructArraysAsParams();
    bool TestSetup_Failed_NestedStructArraysAsParams();
    
    /**
     * @brief A general template for the GAM configuration.
     *        The template has printf-style spcifiers (`%s`) where
     *        specialization for each test is required (i.e. model name,
     *        model path, input signals, output signals).
     */
    MARTe::StreamString configTemplate = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = SimulinkWrapperGAM"
        "            Library = \"%s\""
        "            SymbolPrefix = \"%s\""
        "            Verbosity = 2"
        "            TunableParamExternalSource = ExtSource"
        "            SkipUnlinkedTunableParams = %s"
        "               %s" // InputSignals
        "               %s" // OutputSignals
        "            Parameters = {"
        "                one = (uint8) 1"
        "                %s"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "            AllowNoProducers = 1"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = SimulinkWrapperGAMDataSourceHelper"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIMULINKWRAPPERGAMTEST_H_ */

