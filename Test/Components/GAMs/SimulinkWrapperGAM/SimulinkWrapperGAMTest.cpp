/**
 * @file ConstantGAMTest.cpp
 * @brief Source file for class ConstantGAMTest
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

 * @details This source file contains the definition of all the methods for
 * the class ConstantGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <iostream>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "File.h"
#include "SimulinkWrapperGAMTest.h"
#include "SimulinkWrapperGAM.h"
#include "MatlabDataArray.hpp"
#include "MatlabEngine.hpp"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/**
 * Gives access to the MathExpressionGAM memory for the TestMemory test
 */
// class MathExpressionGAMHelper: public MARTe::MathExpressionGAM {
// public:
//     CLASS_REGISTER_DECLARATION()
//     
//     MathExpressionGAMHelper() : MARTe::MathExpressionGAM() {
//     
//     }
// 
//     ~MathExpressionGAMHelper() {
// 
//     }
//     
//     void* GetInputSignalMemory(uint32 index) {
//         return MathExpressionGAM::GetInputSignalMemory(index);
//     }
//     
//     void* GetOutputSignalMemory(uint32 index) {
//         return MathExpressionGAM::GetOutputSignalMemory(index);
//     }
//     
//     MARTe::RuntimeEvaluator* GetEvaluator() {
//         return evaluator;
//     } 
//     
// };
// CLASS_REGISTER(MathExpressionGAMHelper, "1.0");

/**
 * A dummy DataSource which can be used to test different configuration interfaces
 *  to the MathExpressionGAM
 */
class SimulinkWrapperGAMDataSourceHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

SimulinkWrapperGAMDataSourceHelper    () : MARTe::DataSourceI() {
    }

    virtual ~SimulinkWrapperGAMDataSourceHelper() {

    }

    virtual bool AllocateMemory() {
        return true;
    }

    virtual MARTe::uint32 GetNumberOfMemoryBuffers() {
        return 0;
    }

    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx,
            const MARTe::uint32 bufferIdx,
            void *&signalAddress) {
        return true;
    }

    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data,
            const MARTe::SignalDirection direction) {
        if (direction == MARTe::InputSignals) {
            return "MemoryMapInputBroker";
        }
        return "MemoryMapOutputBroker";
    }

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
            const MARTe::char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(
            MARTe::ReferenceContainer &inputBrokers,
            const MARTe::char8* const functionName,
            void * const gamMemPtr) {
        MARTe::ReferenceT < MARTe::MemoryMapInputBroker > broker("MemoryMapInputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = inputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool GetOutputBrokers(
            MARTe::ReferenceContainer &outputBrokers,
            const MARTe::char8* const functionName,
            void * const gamMemPtr) {
        MARTe::ReferenceT < MARTe::MemoryMapOutputBroker > broker("MemoryMapOutputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = outputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool Synchronise() {
        return true;
    }

};

CLASS_REGISTER(SimulinkWrapperGAMDataSourceHelper, "1.0");

/**
 * Starts a MARTe application that uses this GAM instance.
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config,
                                        bool destroy = true) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    if (destroy) {
        god->Purge();
    }
    return ok;
}

/**
 * @brief Class that initialises a test environment for SimulinkWrapperGAM.
 */
class SimulinkGAMGTestEnvironment {
public:

    SimulinkGAMGTestEnvironment() {
        
        // Start MATLAB engine synchronously
        //matlabPtr = matlab::engine::startMATLAB();
        matlabPtr = matlab::engine::connectMATLAB(u"Engine_1");
        
        SetupTestEnvironment(matlabPtr);
    }

    ~SimulinkGAMGTestEnvironment() {
        DeleteTestModel();
    }
    
    /**
     * @brief      Builds a model that can be used to test the SimulinkWrapperGAM from a script.
     * @param[out] modelName   name of the generated model.
     * @param[out] modelFolder folder in which the model was created.
     * @return     `true` if the model was succesfully built.
     */
    void SetupTestEnvironment(std::unique_ptr<matlab::engine::MATLABEngine>& matlabPtr);
    
    /**
     * @brief This method created a new model for testing purpose. The model
     *        is registered in `createdModel` so that it can be removed
     *        at the end of testing by DeleteTestModel().
     */
    MARTe::StreamString CreateTestModel(MARTe::StreamString scriptCall);
    
    /**
     * @brief Removes all files created during testing with CreateTestModel(). 
     */
    void DeleteTestModel();

    /**
     * @brief Working directory where the engine creates models.
     */
    MARTe::StreamString modelFolder;
    
    /**
     * @brief Pointer to the engine used to create and compile test models.
     */
    std::unique_ptr<matlab::engine::MATLABEngine> matlabPtr;

    /**
     * @brief A list of the names of all models that have been compiled
     *        to perform the tests. It is used to delete the test models
     *        at the end of the tests.
     */
    MARTe::StaticList<MARTe::StreamString*> compiledModels;
    
};

void SimulinkGAMGTestEnvironment::SetupTestEnvironment(std::unique_ptr<matlab::engine::MATLABEngine>& matlabPtr) {
    
    using namespace MARTe;
    using namespace matlab::engine;
    
    StreamString modelScriptFolder, addpathCommand;
    
    // Add the directory where the test script is located.
    modelScriptFolder  = getenv("MARTe2_Components_DIR");
    modelScriptFolder += "/Test/Components/GAMs/SimulinkWrapperGAM/";
    
    addpathCommand  = "addpath(\"";
    addpathCommand += modelScriptFolder.Buffer();
    addpathCommand += "\")";
        
    // Execute setup lines
    matlabPtr->eval(u"clear variables;");
    matlabPtr->eval(convertUTF8StringToUTF16String(addpathCommand.Buffer()));
    matlabPtr->eval(u"global model_name model_compiled");
    
    // Get the name of the work directory
    matlabPtr->eval(u"current_folder = pwd;");
    matlab::data::CharArray currentFolder = matlabPtr->getVariable(u"current_folder");
    
    modelFolder = (currentFolder.toAscii()).c_str();
}

MARTe::StreamString SimulinkGAMGTestEnvironment::CreateTestModel(MARTe::StreamString scriptCall) {
    
    using namespace MARTe;
    
    StreamString modelName = "";
    
    matlabPtr->eval(matlab::engine::convertUTF8StringToUTF16String(scriptCall.Buffer()));
    matlab::data::CharArray modelNameCharArray = matlabPtr->getVariable(u"model_name");
    modelName = (modelNameCharArray.toAscii()).c_str();
    
    // Verify that model has been correctly compiled
    matlab::data::TypedArray<bool> modelCompiled = matlabPtr->getVariable(u"model_compiled");
    
    bool ok = modelCompiled[0];
    if (ok) {
        StreamString* currentModelName = new StreamString(modelName);
        bool copied = compiledModels.Add(currentModelName);
        printf("voilà, string: %s pointer: %s | ok? %u\n", modelName.Buffer(), currentModelName->Buffer(), copied );
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Model %s not compiled (compilation failed or already existent.", modelName.Buffer());
    }
    
    return modelName;
}

void SimulinkGAMGTestEnvironment::DeleteTestModel() {
    
    using namespace MARTe;
    
    bool ok;
    
    for (uint32 modelIdx = 0u; modelIdx < compiledModels.GetSize(); modelIdx++) {
        
        StreamString* currentModelName;
        ok = compiledModels.Peek(modelIdx, currentModelName);
        
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Model %u: %s, ok? %u", modelIdx, currentModelName->Buffer(), ok);
        
        // Remove model .so
        StreamString modelPath;
        Directory toDelete;
        
        modelPath  = modelFolder;
        modelPath += "/";
        modelPath += *currentModelName;
        modelPath += ".so";
        
        toDelete.SetByName(modelPath.Buffer());
        if (toDelete.Exists()) {
            //ok = toDelete.Delete(); // TODO DEBUG uncomment this
        }
        
        delete currentModelName;
    }
}

// Instantiate a test environment for this GAM.
static SimulinkGAMGTestEnvironment testEnvironment;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool SimulinkWrapperGAMTest::TestConstructor() {
    
    SimulinkWrapperGAM gam;
    return (gam.GetNumberOfInputSignals() == 0u) && (gam.GetNumberOfOutputSignals() == 0u);
}

bool SimulinkWrapperGAMTest::TestInitialiseWithConfiguration(ConfigurationDatabase configIn) {
    
    SimulinkWrapperGAM gam;
    bool ok = gam.Initialise(configIn);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetupWithTemplate(StreamString scriptCall,
                                                   StreamString skipUnlinkedParams,
                                                   StreamString inputSignals,
                                                   StreamString outputSignals,
                                                   StreamString parameters) {
    
    StreamString modelName, modelFolder, modelFullPath;
    
    // Create the test model
    modelName = testEnvironment.CreateTestModel(scriptCall);
    
    // Retrieve working directory from the test environment
    modelFolder = testEnvironment.modelFolder;
    
    // Compose model library full path
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    // Insert model name and build folder in the configuration buffer template
    StreamString config;
    config.Printf(configTemplate.Buffer(),
                  modelFullPath.Buffer(),
                  modelName.Buffer(),
                  skipUnlinkedParams.Buffer(),
                  inputSignals.Buffer(),
                  outputSignals.Buffer(),
                  parameters.Buffer()
                 );
    
    // Test setup
    bool ok = TestIntegratedInApplication(config.Buffer());
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestInitialise() {
    
    bool ok = true;
    
    StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";

    SimulinkWrapperGAM    gam;
    ConfigurationDatabase config;
    
    ok &= config.Write("Library",                    modelFullPath);
    ok &= config.Write("SymbolPrefix",               modelName);
    ok &= config.Write("TunableParamExternalSource", "ExtSource");
    ok &= config.Write("Verbosity",                  2);
    ok &= config.Write("SkipUnlinkedTunableParams",  1);
    
    ok &= config.CreateAbsolute("Parameters");
    ok &= config.MoveAbsolute("Parameters");
    
    float32 scalarParam       = 3.141592653589793;
    int64   vectorParam[2]    = {10, -20};
    uint32  matrixParam[2][2] = { {1, 2}, {3, 4} };
    
    ok &= config.Write("ScalarParam", scalarParam);
    ok &= config.Write("VectorParam", vectorParam);
    ok &= config.Write("MatrixParam", matrixParam);
    ok &= config.Write("StringParam", "hello");
    
    ok &= config.MoveToAncestor(1u);
    
    if (ok) {
        ok = gam.Initialise(config);
    }
    return ok;
}


bool SimulinkWrapperGAMTest::TestInitialise_Failed_MissingLibrary() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_MissingSymbolPrefix() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelName   = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",                    modelFullPath);
    
    return !TestInitialiseWithConfiguration(config);
    
}


bool SimulinkWrapperGAMTest::TestInitialise_MissingTunableParamExternalSource() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    
    return TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_MissingOptionalConfigurationSettings() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    return TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LoadLibrary() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    // add an error to model name
    modelName += "_err";
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LoadSymbols() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    // add an error to model name
    modelName += "_err";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LibraryMissingGetMmiFunction() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel('hasGetmmiFcn', false);");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LibraryMissingAllocFunction() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel('hasAllocFcn', false);");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_MissingParametersLeaf() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createSimpleTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    config.Write("Verbosity",                  2);
    config.Write("SkipUnlinkedTunableParams",  1);
    
    return TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestSetup() {
    
    StreamString scriptCall = "createTestModel(true, true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarSingle  = {"
        "    DataSource = Drv1"
        "    Type = float32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In3_ScalarInt8  = {"
        "    DataSource = Drv1"
        "    Type = int8"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In4_VectorDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 9"
        "    NumberOfDimensions = 1"
        "}"
        "In5_VectorSingle  = {"
        "    DataSource = Drv1"
        "    Type = float32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarSingle  = {"
        "    DataSource = DDB1"
        "    Type = float32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "Out8_MatrixSingle = {"
        "    DataSource = DDB1"
        "    Type = float32"
        "    NumberOfElements = 20"
        "    NumberOfDimensions = 1"
        "}"
        "Out9_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 9"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = ""
        "vectorConstant = (float32) {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "matrixConstant = (float64) {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}}";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters() {
    
    StreamString scriptCall = "createSimpleTestModel('hasStructParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = ""
        "structScalar-one = (float64) 1 "
        "structScalar-nested1-one = (float64) 1 "
        "structScalar-nested1-two = (float64) 1 "
        "structScalar-nested2-one = (float64) 1 "
        "structScalar-nested2-two = (float64) 1 ";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_NoTunableParameters() {
    
    StreamString scriptCall = "createSimpleTestModel('hasTunableParams',     false);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_SkipUnlinkedTunableParams() {

    StreamString scriptCall = "createTestModel(true, true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarSingle  = {"
        "    DataSource = Drv1"
        "    Type = float32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In3_ScalarInt8  = {"
        "    DataSource = Drv1"
        "    Type = int8"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In4_VectorDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 9"
        "    NumberOfDimensions = 1"
        "}"
        "In5_VectorSingle  = {"
        "    DataSource = Drv1"
        "    Type = float32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarSingle  = {"
        "    DataSource = DDB1"
        "    Type = float32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "Out8_MatrixSingle = {"
        "    DataSource = DDB1"
        "    Type = float32"
        "    NumberOfElements = 20"
        "    NumberOfDimensions = 1"
        "}"
        "Out9_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 9"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = ""
        "vectorConstant = (float64) {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_DontSkipUnlinkedTunableParams() {
    
    StreamString scriptCall = "createTestModel(true, true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarSingle  = {"
        "    DataSource = Drv1"
        "    Type = float32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In3_ScalarInt8  = {"
        "    DataSource = Drv1"
        "    Type = int8"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In4_VectorDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 9"
        "    NumberOfDimensions = 1"
        "}"
        "In5_VectorSingle  = {"
        "    DataSource = Drv1"
        "    Type = float32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarSingle  = {"
        "    DataSource = DDB1"
        "    Type = float32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "Out8_MatrixSingle = {"
        "    DataSource = DDB1"
        "    Type = float32"
        "    NumberOfElements = 20"
        "    NumberOfDimensions = 1"
        "}"
        "Out9_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 9"
        "    NumberOfDimensions = 1"
        "}"
        "}";
        
    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfInputs() {
    
    StreamString scriptCall = "createSimpleTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfOutputs() {
    
    StreamString scriptCall = "createSimpleTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_NoInputs() {
    
    StreamString scriptCall = "createSimpleTestModel('hasInputs', false);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = "";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}


bool SimulinkWrapperGAMTest::TestSetup_Failed_NoOutputs() {
    
    StreamString scriptCall = "createSimpleTestModel('hasOutputs', false);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "";
    
    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_StructArraysAsParams() {
    
    StreamString scriptCall = "createSimpleTestModel('hasStructArrayParams', true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
            
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_NestedStructArraysAsParams() {
    
    StreamString scriptCall = "createSimpleTestModel('hasStructParams', true, 'hasStructArrayParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
            
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = ""
        "structScalar-one = (float64) 1 "
        "structScalar-nested1-one = (float64) 1 "
        "structScalar-nested1-two = (float64) 1 "
        "structScalar-nested2-one = (float64) 1 "
        "structScalar-nested2-two = (float64) 1 ";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}
