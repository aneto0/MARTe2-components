/**
 * @file SimulinkWrapperGAMTest.h
 * @brief Header file for class SimulinkWrapperGAMTest
 * @date 10/08/2020
 * @author RFX
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
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
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
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "SafeMath.h"
#include "SimulinkWrapperGAM.h"
#include "StandardParser.h"
#include "StaticList.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the ConstantGAM public methods.
 */
class SimulinkWrapperGAMTest {
public:

    /**
     * @brief  Tests the default constructor.
     * @return `true` after GAM instantiation.
     */
    bool TestConstructor();
    
    /**
     * @brief   Tests the Initialise() method.
     * @details Verify the post-conditions.
     * @return  `true` if Initialise() succeeds.
     */
    bool TestInitialise();
    
    /**
     * @brief   Tests the Setup() method.
     * @details Verify the post-conditions.
     * @return  `true` if Setup() succeeds.
     */
    bool TestSetup();

    /**
     * @brief  Tests the Execute() method.
     * @return `true` if Execute() succeeds.
     */
    bool TestExecute();
    
    /**
     * @brief Helper method to test Initialise by passing a ConfigurationDatabase.
     * @param[in] configIn the configuration database with the configuration to be tested.
     */
    bool TestInitialiseWithConfiguration(MARTe::ConfigurationDatabase configIn);
    
    /**
     * @brief General method to test GAM setup. The method allows to
     *        specify various details of the configuration file.
     * @param[in] skipUnlinkedParams whether to skip not ok tunable parameters
     * @param[in] inputSignals a string containing the input signal configuration for current test
     * @param[in] outputSignals a string containing the output signal configuration for current test
     * @param[in] parameters a string containing the parameter configuration for current test
     * @param[in] objRegDatabase (optional) if specified, a pointer to the registry
     *          of the current MARTe2 is returned (but then it is
     *          up to the user to call objRegDatabase->Purge()).
     */
    bool TestSetupWithTemplate(MARTe::StreamString scriptCall,
                                                   MARTe::StreamString skipUnlinkedParams,
                                                   MARTe::StreamString inputSignals,
                                                   MARTe::StreamString outputSignals,
                                                   MARTe::StreamString parameters,
                                                   MARTe::ObjectRegistryDatabase* objRegDatabase, /* = NULL_PTR(ObjectRegistryDatabase*)*/
                                                   bool         structuredSignalsAsByteArrays, /* = true */
                                                   bool         enforceModelSignalCoverage /*= false */
                                                   );
    
    /**
     * @brief Tests the Initialise() method if optional settings are missing.
     */
    bool TestInitialise_MissingOptionalConfigurationSettings();
    
    /**
     * @brief Tests the Initialise() method if TunableParamsExternalSource setting is missing.
     */
    bool TestInitialise_MissingTunableParamExternalSource();
    
    /**
     * @brief Tests the Initialise() method if the Parameters node is missing.
     */
    bool TestInitialise_MissingParametersLeaf();
    
    /**
     * @brief Tests the Initialise() method if Library setting is missing.
     */
    bool TestInitialise_Failed_MissingLibrary();
    
    /**
     * @brief Tests the Initialise() method if SymbolPrefix setting is missing.
     */
    bool TestInitialise_Failed_MissingSymbolPrefix();
    
    /**
     * @brief Tests the Initialise() method if NonVirtualBusMode is wrong.
     */
    bool TestInitialise_Failed_WrongNonVirtualBusMode();
    
    /**
     * @brief Tests the Initialise() method if the external .so cannot be loaded.
     */
    bool TestInitialise_Failed_LoadLibrary();
    
    /**
     * @brief Tests the Initialise() method if the expected symbols
     *        exposed by the external .so cannot be loaded.
     */
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
     * @brief Tests the correct actualisation of structured parameters.
     */
    bool TestSetup_StructTunableParameters_1();
    
    /**
     * @brief Tests the correct actualisation of structured parameters.
     */
    bool TestSetup_StructTunableParameters_2();
    
    /**
     * @brief Tests the correct actualisation of structured parameters.
     */
    bool TestSetup_StructTunableParameters_3();
    
    /**
     * @brief Tests the correct actualisation of structured parameters
     *        from an external ReferenceContainer containing References
     *        to AnyObjects.
     */
    bool TestSetup_StructTunableParametersFromExternalSource();
    
    /**
     * @brief Tests the Setup() method when no tunable parameters are found.
     */
    bool TestSetup_NoTunableParameters();
    
    /**
     * @brief Tests the Setup() method when structured signals (non-virtual
     *        buses) are found.
     */
    bool TestSetup_WithStructSignals();
    
    /**
     * @brief Tests the Setup() method with nested structured signals.
     */
    bool TestSetup_WithNestedStructSignals();
    
    /**
     * @brief Tests the Setup() method when told to skip not ok tunable parameters.
     */
    bool TestSetup_SkipInvalidTunableParams();
    
    /**
     * @brief Tests the Setup() method when there are no inputs.
     */
    bool TestSetup_NoInputs();
    
    /**
     * @brief Tests the Setup() method when there are no outputs.
     */
    bool TestSetup_NoOutputs();
    
    /**
     * @brief Tests the Setup() method when told not to skip not ok tunable parameters.
     */
    bool TestSetup_Failed_DontSkipUnlinkedTunableParams();
    
    /**
     * @brief Tests the Setup() method when the number of model inputs
     *        do not match the one declared in the configuration.
     */
    bool TestSetup_Failed_WrongNumberOfInputs();
    
    /**
     * @brief Tests the Setup() method when the number of model outputs
     *        do not match the one declared in the configuration.
     */
    bool TestSetup_Failed_WrongNumberOfOutputs();
    
    /**
     * @brief Tests the Setup() method when an input signal name does
     *        not match.
     */
    bool TestSetup_Failed_WrongInputName();
    
    /**
     * @brief Tests the Setup() method when an output signal name does
     *        not match.
     */
    bool TestSetup_Failed_WrongOutputName();
    
    /**
     * @brief Tests the Setup() method  when a signal number of elements
     *        does not match.
     */
    bool TestSetup_Failed_WrongNumberOfElements();
    
    /**
     * @brief Tests the Setup() method  when a signal number of dimensions
     *        does not match.
     */
    bool TestSetup_Failed_WrongNumberOfDimensions();
    
    /**
     * @brief Tests the Setup() method  when a signal type
     *        does not match.
     */
    bool TestSetup_Failed_WrongDatatype();
    
    /**
     * @brief Tests the Setup() method  when a parameter number of dimensions
     *        does not match.
     */
    bool TestSetup_Failed_ParamWrongNumberOfDimensions();
    
    /**
     * @brief Tests the Setup() method  when a parameter number of elements
     *        does not match.
     */
    bool TestSetup_Failed_ParamWrongDimensions();
    
    /**
     * @brief Tests the Setup() method  when a parameter number of elements
     *        does not match (matrix case).
     */
    bool TestSetup_Failed_ParamWrongDimensions_Matrix();
    
    /**
     * @brief Tests the Setup() method  when a parameter type
     *        does not match.
     */
    bool TestSetup_Failed_ParamWrongDatatype();
    
    /**
     * @brief Tests the Setup() method when the model has struct arrays
     *        used as parameters.
     */
    bool TestSetup_Failed_StructArraysAsParams();
    
    /**
     * @brief Tests the Setup() method when the model has nested struct arrays
     *        used as parameters.
     */
    bool TestSetup_Failed_NestedStructArraysAsParams();
    
    /**
     * @brief Tests the Setup() method when the configuration has
     *        NumberOfDimensions != 1 for a struct signal.
     */
    bool TestSetup_Failed_WrongNumberOfDimensionsWithStructSignals();
    
    /**
     * @brief Tests the Setup() method when the configuration has
     *        Type != uint8 for a struct signal.
     */
    bool TestSetup_Failed_WrongDatatypeWithStructSignals();
    
    /**
     * @brief Tests the correct actualisation of parameters in a model
     *        using row-major data orientation.
     */
    bool TestParameterActualisation_RowMajorModel();
    
    /**
     * @brief Tests the correct actualisation of parameters in a model
     *        using column-major data orientation.
     */
    bool TestParameterActualisation_ColumnMajorModel();
    
    /**
     * @brief Tests the correct actualisation of parameters in a model
     *        using uint datatypes.
     */
    bool TestParameterActualisation_Uint();
    
    /**
     * @brief Tests the correct actualisation of parameters in a model
     *        using int datatypes.
     */
    bool TestParameterActualisation_Int();
    
    /**
     * @brief Tests the correct actualisation of parameters in a model
     *        using float datatypes.
     */
    bool TestParameterActualisation_Float();
    
    /**
     * @brief Tests the PrintAlgoInfo() method.
     */
    bool TestPrintAlgoInfo();
    
    /**
     * @brief Test the behaviour when working in pure structured signal mode
     */
    bool Test_StructuredSignals();


    /**
     * @brief Test execution and coherence when working in pure structured mode
     */
    bool TestExecute_WithStructuredSignals();


    /**
     * @brief Test execution and coherence when working with mixed signals and transposition takes place
     */
    bool Test_MultiMixedSignalsTranspose(bool transpose);

    /**
     * @brief Test the behaviour when working in pure structured signal mode, while enforcing
     * MARTe2 - Simulink parameter coverage (1:1 mapping)
     */
    bool Test_StructuredSignals_Failed();


    /**
     * @brief Tests the behaviour when an unlinked parameter is not matched in the slx model
     */
    bool TestSetup_WithNotFoundParameter_Failed(bool skipUnlinked);


    /**
     * @brief Tests the setup of a model with nested signals
     */
    bool TestSetup_WithNestedSingleSignals();

    bool TestSetup_StructTunableParametersFromExternalSource_Failed();

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
        "            Class = SimulinkWrapperGAMHelper"
        "            Library = \"%s\""
        "            SymbolPrefix = \"%s\""
        "            Verbosity = 2"
        "            TunableParamExternalSource = ExtSource"
        "            NonVirtualBusMode = %s"
        "            EnforceModelSignalCoverage = %s"
        "            SkipInvalidTunableParams = %s"
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

