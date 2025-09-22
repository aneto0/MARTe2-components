/**
 * @file SimulinkWrapperGAMTest.h
 * @brief Header file for class SimulinkWrapperGAMTest
 * @date 10/08/2020
 * @author Nicolo Ferron
 * @author Giuseppe Avon
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
     * @param[out] status  the status of the GAM
     */
    bool TestInitialiseWithConfiguration(MARTe::ConfigurationDatabase configIn, MARTe::ErrorManagement::ErrorType& status);
    
    /**
     * @brief General method to test GAM setup. The method allows to
     *        specify various details of the configuration file.
     * @param[in] skipUnlinkedParams whether to skip not ok tunable parameters
     * @param[in] inputSignals a string containing the input signal configuration for current test
     * @param[in] outputSignals a string containing the output signal configuration for current test
     * @param[in] parameters a string containing the parameter configuration for current test
     * @param[out] status the error status of the GAM for further testing
     * @param[in] objRegDatabase (optional) if specified, a pointer to the registry
     *          of the current MARTe2 is returned (but then it is
     *          up to the user to call objRegDatabase->Purge()).
     */
    bool TestSetupWithTemplate(MARTe::StreamString scriptCall,
                               MARTe::StreamString inputSignals,
                               MARTe::StreamString outputSignals,
                               MARTe::StreamString parameters,
                               MARTe::StreamString configOptions,
                               MARTe::ErrorManagement::ErrorType& status,
                               MARTe::ObjectRegistryDatabase* objRegDatabase /* = NULL_PTR(ObjectRegistryDatabase*)*/
                                );
    
    /**
     * @brief General method to test GAM execute. The input strings shall be in MARTe2
     *        configuration syntax and are parsed by a StandardParser. The values are
     *        compared with that of the signals.
     */
    bool TestExecuteGeneric(MARTe::StreamString marteInputs,
                            MARTe::StreamString modelExpectedInputs,
                            MARTe::StreamString modelExpectedOutputs,
                            MARTe::StreamString marteExpectedOutputs,
                            MARTe::StreamString marteExpectedLoggingSignals,
                            MARTe::ErrorManagement::ErrorType& status,
                            MARTe::ObjectRegistryDatabase* objRegDatabase
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
     * @brief Tests the Setup() method when block signals are logged.
     */
    bool TestSetup_WithLoggingSignals();

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
     * @brief Tests the Setup() method when verbosity level is set to 1.
     */
    bool TestSetup_LowVerbosity();

    /**
     * @brief Tests the Setup() method when verbosity level is set to 0.
     */
    bool TestSetup_ZeroVerbosity();

    /**
     * @brief Tests the GetInterfaceBytesize method of SimulinkRootInterface.
     */
    bool TestSetup_StructSignalBytesize();

    /**
     * @brief Tests the Setup() method with 3D signals in the bus input.
     */
    bool TestSetup_With3DSignals();
    
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
     * @brief Tests the Setup() method when the number of model logging signals
     *        do not match the ones declared in the configuration.
     */
    bool TestSetup_Failed_WrongNumberOfLoggingSignals();

    /**
     * @brief Tests the Setup() method when an input signal name does
     *        not match.
     */
    bool TestSetup_Failed_EmptyInterfaceName();

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
     * @brief Tests the Setup() method when an output signal name does
     *        not match.
     */
    bool TestSetup_Failed_WrongLoggingSignalName();
    
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
     * @brief Tests the Setup() method  when a logging signal type
     *        does not match.
     */
    bool TestSetup_Failed_LoggingSignalWrongDatatype();
    
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
    bool TestSetup_StructArraysAsParams();
    
    /**
     * @brief Tests the Setup() method when the model has nested struct arrays
     *        used as parameters.
     */
    bool TestSetup_NestedStructArraysAsParams();
    
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

#ifdef ROW_MAJOR_ND_FEATURE    
    /**
     * @brief Tests the correct actualisation of parameters in a model
     *        using row-major data orientation.
     */
    bool TestParameterActualisation_RowMajorModel();
#endif /* ROW_MAJOR_ND_FEATURE */ 
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
     * @brief Test the behaviour when working in pure structured signal mode
     *        and having logging signals
     */
    bool TestSetup_WithStructuredLoggingSignals();

    /**
     * @brief Test execution and coherence with logging signals
     */
    bool TestExecute_WithLoggingSignals();

    /**
     * @brief Test execution and coherence when working in pure structured mode
     */
    bool TestExecute_WithStructuredSignals();

    /**
     * @brief Test execution and coherence when working in pure structured mode
     */
    bool TestExecute_WithStructuredLoggingSignals();

    /**
     * @brief Test execution and coherence when working with mixed signals and transposition takes place
     */
    bool TestExecute_MultiMixedSignalsTranspose(bool transpose);

    /**
     * @brief Test execution and coherence when working with 3D signals
     */
    bool TestExecute_With3DSignals(bool transpose);

    /**
     * @brief Test execution and coherence when working with 3D signals
     */
    bool TestExecute_With3DSignals_NoInputs(bool transpose);

    /**
     * @brief Test execution and coherence when working with struct array signals
     */
    bool TestExecute_WithStructArraySignals(bool transpose);

    /**
     * @brief Test execution and coherence when working with struct array signals
     */
    bool TestExecute_WithStructArraySignals_NoInputs();

    /**
     * @brief Test the behaviour when working in pure structured signal mode, while enforcing
     * MARTe2 - Simulink parameter coverage (1:1 mapping)
     */
    bool TestSetup_DisconnectedOutputSignal_Failed();

    /**
     * @brief Test the behaviour when working in pure structured signal mode, while enforcing
     * MARTe2 - Simulink parameter coverage (1:1 mapping)
     */
    bool TestSetup_DisconnectedOutputStructuredSignal_Failed();

    /**
     * @brief Test the behaviour when working in pure structured signal mode, while enforcing
     * MARTe2 - Simulink parameter coverage (1:1 mapping)
     */
    bool TestSetup_DisconnectedInputSignal_Failed();

    /**
     * @brief Test the behaviour when working in pure structured signal mode, while enforcing
     * MARTe2 - Simulink parameter coverage (1:1 mapping)
     */
    bool TestSetup_DisconnectedInputStructuredSignal_Failed();


    /**
     * @brief Tests the behaviour when an unlinked parameter is not matched in the slx model
     */
    bool TestSetup_WithNotFoundParameter_Failed(bool skipUnlinked);


    /**
     * @brief Tests the setup of a model with nested signals
     */
    bool TestSetup_WithNestedSingleSignals();

    /**
     * @brief Tests the setup of a model with struct parameters from external AnyObject source
     */
    bool TestSetup_StructTunableParametersFromExternalSource_Unlinked();
    
#ifdef ENUM_FEATURE
    /**
     * @brief Tests the setup of a model with enums in output signals
     */
    bool TestSetup_WithOutputEnumSignals();
    
    /**
     * @brief Tests the setup of a model with enums as signals
     */
    bool TestSetup_WithEnumSignals();
    
    /**
     * @brief Tests the setup of a model with enums as parameters
     */
    bool TestSetup_WithEnumParameters();
    
    /**
     * @brief Tests the setup of a model with enums in output signals
     */
    bool TestSetup_WithOutputEnumSignals_FailedWrongType();
    
    /**
     * @brief Tests execution of a model with enums as signals
     */
    bool TestExecute_WithEnumSignals();
#endif

    bool TestLinearIndexToSubscripts();
    
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
        "               %s " // Configuration options
        "            TunableParamExternalSource = ExtSource"
        "               %s " // InputSignals
        "               %s " // OutputSignals
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
        "    } "
        "} "
        "+Types = { "
        "    Class = ReferenceContainer "
        "    +Scalar_Structured_t = { "
        "        Class = IntrospectionStructure "
        "        ScalarDouble = { NumberOfElements = {1}    Type = float64 } "
        "        ScalarUint32 = { NumberOfElements = {1}    Type = uint32  } "
        "    } "
        "    +Vector_Structured_t = { "
        "        Class = IntrospectionStructure "
        "        VectorDouble = { NumberOfElements = {8}    Type = float64 } "
        "        VectorUint32 = { NumberOfElements = {8}    Type = uint32  } "
        "    } "
        "    +Matrix_Structured_t = { "
        "        Class = IntrospectionStructure "
        "        MatrixDouble = { NumberOfElements = {6, 6} Type = float64 } "
        "        MatrixUint32 = { NumberOfElements = {6, 6} Type = uint32  } "
        "    } "
        "    +Matrix3D_Structured_t = { "
        "        Class = IntrospectionStructure "
        "        Matrix3DDouble = { NumberOfElements = {2, 3, 4} Type = float64 } "
        "        Matrix3DUint32 = { NumberOfElements = {2, 3, 4} Type = uint32  } "
        "    } "
        "    +VectorMatrix_Structured_t = { "
        "        Class = IntrospectionStructure "
        "        Vector_Structured = { NumberOfElements = {1}    Type = Vector_Structured_t } "
        "        Matrix_Structured = { NumberOfElements = {1}    Type = Matrix_Structured_t } "
        "    } "
        "    +VectorMatrix_StructuredArray_t = { "
        "        Class = IntrospectionStructure "
        "        Vector_Structured = { NumberOfElements = {2, 3}  Type = Vector_Structured_t } "
        "        Matrix_Structured = { NumberOfElements = {2}     Type = Matrix_Structured_t } "
        "    } "
        "    +ScalarVector_Structured_t = { "
        "        Class = IntrospectionStructure "
        "        Scalar_Structured = { NumberOfElements = {1}    Type = Scalar_Structured_t } "
        "        Vector_Structured = { NumberOfElements = {1}    Type = Vector_Structured_t } "
        "    } "
        "    +ScalarVector_StructuredArray_t = { "
        "        Class = IntrospectionStructure "
        "        Scalar_Structured = { NumberOfElements = {2, 3, 4} Type = Scalar_Structured_t } "
        "        Vector_Structured = { NumberOfElements = {2, 3}    Type = Vector_Structured_t } "
        "    } "
        "} "
        ;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SIMULINKWRAPPERGAMTEST_H_ */

