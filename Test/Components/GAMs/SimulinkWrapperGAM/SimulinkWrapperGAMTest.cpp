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
        
        BuildTestModel(matlabPtr);

    }

    ~SimulinkGAMGTestEnvironment() {
        DeleteTestModel();
printf("DELETE ENDED SUCCESSFULLY\n");
    }
    
    bool BuildTestModel(std::unique_ptr<matlab::engine::MATLABEngine>& matlabPtr);
    bool DeleteTestModel();
    
    MARTe::StreamString simpleModelName;
    MARTe::StreamString IOModelName;
    MARTe::StreamString modelName;
    MARTe::StreamString modelFolder;
    std::unique_ptr<matlab::engine::MATLABEngine> matlabPtr;
};

/**
 * @brief      Builds a model that can be used to test the SimulinkWrapperGAM from a script.
 * @param[out] modelName   name of the generated model.
 * @param[out] modelFolder folder in which the model was created.
 * @return     `true` if the model was succesfully built.
 */
bool SimulinkGAMGTestEnvironment::BuildTestModel(std::unique_ptr<matlab::engine::MATLABEngine>& matlabPtr) {
    
    using namespace MARTe;
    using namespace matlab::engine;
    
    bool ok = true;
    
    StreamString modelScriptFolder = getenv("MARTe2_Components_DIR");
    modelScriptFolder += "/Test/Components/GAMs/SimulinkWrapperGAM/";
    
    modelFolder = modelScriptFolder;
    
    StreamString addpathCommand = "addpath(\"";
    addpathCommand += modelScriptFolder.Buffer();
    addpathCommand += "\")";
    
    // TODO retrieve dynamically from the scripts
    modelFolder     = getenv("MARTe2_Components_DIR");
    modelName       = "test_model11";
    simpleModelName = "testModel_";
    
    if (ok) { // TODO substitute with ok
        
        
        // Execute the code read from the file
        matlabPtr->eval(u"clear variables;");
        matlabPtr->eval(convertUTF8StringToUTF16String(addpathCommand.Buffer()));
        matlabPtr->eval(u"global model_name model_compiled");
        
        //matlabPtr->eval(u"matrixConstant = [1 1 1; 2 2 2; 3 3 3];");
        //matlabPtr->eval(u"vectorConstant = ones(10,1);");
        
        matlabPtr->feval<bool>(u"createTestModel", true, true); // flags: hasAllocFcn, hasGetmmiFcn
        
        // Get the name of the model
        matlab::data::CharArray modelNameCharArray = matlabPtr->getVariable(u"model_name");
        modelName = (modelNameCharArray.toAscii()).c_str();
        
        // Get the name of the folder containing the model file
        matlabPtr->eval(u"current_folder = pwd;");
        matlab::data::CharArray currentFolder = matlabPtr->getVariable(u"current_folder");
        printf("Model written to this folder: %s\n", (currentFolder.toAscii()).c_str());
        
        modelFolder = (currentFolder.toAscii()).c_str();
        
        // Verify that model has been correctly compiled
        //matlab::data::TypedArray<bool> modelCompiled = matlabPtr->getVariable(u"model_compiled");
        
        //ok = modelCompiled[0];
        //if (ok) {
            //REPORT_ERROR_STATIC(ErrorManagement::Information, "Model succesfully compiled.");
        //}
        //else {
            //REPORT_ERROR_STATIC(ErrorManagement::Information, "Model compilation failed.");
        //}
        
        // ok model
        //matlabPtr->eval(u"createSimpleTestModel();"); // 111100
        
    }

    return ok;
    
}

/**
 * @brief Removes all files created by BuildTestModel. 
 */
bool SimulinkGAMGTestEnvironment::DeleteTestModel() {
    
    using namespace MARTe;
    
    bool ok = false;
    
    StreamString currentPath;
    modelFolder += "/";
    
    Directory toDelete;

    currentPath  = modelFolder;
    currentPath += modelName;
    currentPath += ".so";
    
    toDelete.SetByName(currentPath.Buffer());
printf("TODELETE: %s, is there? %u\n", currentPath.Buffer(), toDelete.Exists());
    if (toDelete.Exists()) {
        //ok = toDelete.Delete();  // TODO DEBUG uncomment this
    }
printf("DELETED? %u\n", ok);

    return ok;
}

// Instantiate a test environment for this GAM.
static const SimulinkGAMGTestEnvironment testEnvironment;

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

StreamString SimulinkWrapperGAMTest::CreateTestModel(StreamString scriptCall) {
    
    StreamString modelName;
    
    testEnvironment.matlabPtr->eval(matlab::engine::convertUTF8StringToUTF16String(scriptCall.Buffer()));
    matlab::data::CharArray modelNameCharArray = testEnvironment.matlabPtr->getVariable(u"model_name");
    modelName = (modelNameCharArray.toAscii()).c_str();
    
    return modelName;
}

bool SimulinkWrapperGAMTest::TestSetupWithTemplate(StreamString scriptCall,
                                                   StreamString skipUnlinkedParams,
                                                   StreamString inputSignals,
                                                   StreamString outputSignals,
                                                   StreamString parameters) {
    
    StreamString modelName, modelFolder, modelFullPath;
    
    // Create the test model
    modelName = CreateTestModel(scriptCall);
    
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
    modelName   = CreateTestModel("createSimpleTestModel();");
    
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
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
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
    modelName   = CreateTestModel("createSimpleTestModel();");
    
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
    modelName   = CreateTestModel("createSimpleTestModel();");
    
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
    modelName   = CreateTestModel("createSimpleTestModel();");
    
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
    modelName   = CreateTestModel("createSimpleTestModel();");
    
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
    modelName   = CreateTestModel("createSimpleTestModel();");
    
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
    modelName   = CreateTestModel("createSimpleTestModel('hasGetmmiFcn', false);");
    
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
    modelName   = CreateTestModel("createSimpleTestModel('hasAllocFcn', false);");
    
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
    modelName   = CreateTestModel("createSimpleTestModel();");
    
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
        "}";


    StreamString outputSignals = ""
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
        "}";


    StreamString outputSignals = ""
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
        "}";


    StreamString outputSignals = ""
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
        "}";


    StreamString outputSignals = ""
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
        "}";
    
    StreamString outputSignals = ""
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
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}";
    
    StreamString outputSignals = ""
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
        "}";
    
    StreamString outputSignals = ""
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 1"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}


bool SimulinkWrapperGAMTest::TestSetup_Failed_StructArraysAsParams() {
    
    StreamString scriptCall = "createSimpleTestModel('hasStructArrayParams', true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
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
            "}";
            
    StreamString outputSignals = ""
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
            "}";
            
    StreamString outputSignals = ""
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

// bool MathExpressionGAMTest::TestSetup_Failed_InputSignalMissingVariable() {
//     
//     const char8 * const config1 = ""
//             "$Test = {"
//             "    Class = RealTimeApplication"
//             "    +Functions = {"
//             "        Class = ReferenceContainer"
//             "        +GAM1 = {"
//             "            Class = MathExpressionGAM"
//             "            Expression = \"GAM1_TotalTime = GAM1_WriteTime + GAM1_ExecTime;\""
//             "            InputSignals = {"
//             "               GAM1_ReadTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_WriteTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_ExecTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "            }"
//             "            OutputSignals = {"
//             "               GAM1_TotalTime = {"
//             "                   DataSource = DDB1"
//             "                   Type = uint32"
//             "               }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Data = {"
//             "        Class = ReferenceContainer"
//             "        DefaultDataSource = DDB1"
//             "        +DDB1 = {"
//             "            Class = GAMDataSource"
//             "        }"
//             "        +Timings = {"
//             "            Class = TimingDataSource"
//             "        }"
//             "    }"
//             "    +States = {"
//             "        Class = ReferenceContainer"
//             "        +State1 = {"
//             "            Class = RealTimeState"
//             "            +Threads = {"
//             "                Class = ReferenceContainer"
//             "                +Thread1 = {"
//             "                    Class = RealTimeThread"
//             "                    Functions = {GAM1}"
//             "                }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Scheduler = {"
//             "        Class = GAMScheduler"
//             "        TimingDataSource = Timings"
//             "    }"
//             "}";
//     bool ok = TestIntegratedInApplication(config1);
//     return !ok;
// }
// 
// 
// bool MathExpressionGAMTest::TestMemory() {
//     
//     const MARTe::char8 * const config1 = ""
//             "$Test = {"
//             "    Class = RealTimeApplication"
//             "    +Functions = {"
//             "        Class = ReferenceContainer"
//             "        +GAM1 = {"
//             "            Class = MathExpressionGAMHelper"
//             "            Expression = \"GAM1_TotalTime = GAM1_ReadTime + GAM1_WriteTime + GAM1_ExecTime;\""
//             "            InputSignals = {"
//             "               GAM1_ReadTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_WriteTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_ExecTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "            }"
//             "            OutputSignals = {"
//             "               GAM1_TotalTime = {"
//             "                   DataSource = DDB1"
//             "                   Type = uint32"
//             "               }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Data = {"
//             "        Class = ReferenceContainer"
//             "        DefaultDataSource = DDB1"
//             "        +DDB1 = {"
//             "            Class = GAMDataSource"
//             "        }"
//             "        +Timings = {"
//             "            Class = TimingDataSource"
//             "        }"
//             "    }"
//             "    +States = {"
//             "        Class = ReferenceContainer"
//             "        +State1 = {"
//             "            Class = RealTimeState"
//             "            +Threads = {"
//             "                Class = ReferenceContainer"
//             "                +Thread1 = {"
//             "                    Class = RealTimeThread"
//             "                    Functions = {GAM1}"
//             "                }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Scheduler = {"
//             "        Class = GAMScheduler"
//             "        TimingDataSource = Timings"
//             "    }"
//             "}";
//             
//     bool ok = TestIntegratedInApplication(config1, false);
//     ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
//     ReferenceT<MathExpressionGAMHelper> gam = god->Find("Test.Functions.GAM1");
//     if (ok) {
//         ok = gam.IsValid();
//     }
//     RuntimeEvaluator* evalPtr;
//     if (ok) {
//         evalPtr = gam->GetEvaluator();
//     }
//     
//     StreamString signalName;
//     for (uint32 index = 0u; (index < gam->GetNumberOfInputSignals()) && ok; index++) {
//         ok = gam->GetSignalName(InputSignals, index, signalName);
//         if (ok) {
//             ok = (gam->GetInputSignalMemory(index) == evalPtr->GetInputVariableMemory(signalName));
//         }
//         signalName = "";
//     }
//     for (uint32 index = 0u; (index < gam->GetNumberOfOutputSignals()) && ok; index++) {
//         ok = gam->GetSignalName(OutputSignals, index, signalName);
//         if (ok) {
//             ok = (gam->GetOutputSignalMemory(index) == evalPtr->GetOutputVariableMemory(signalName));
//         }
//         signalName = "";
//     }
//     god->Purge();
//     return ok;
// }
// 
// bool MathExpressionGAMTest::TestTypes() {
//     
//     const MARTe::char8 * const config1 = ""
//             "$Test = {"
//             "    Class = RealTimeApplication"
//             "    +Functions = {"
//             "        Class = ReferenceContainer"
//             "        +GAM1 = {"
//             "            Class = MathExpressionGAMHelper"
//             "            Expression = \""
//             "                           GAM1_ReadWriteTime = GAM1_ReadTime + GAM1_WriteTime;"
//             "                           GAM1_TotalTime = GAM1_ReadWriteTime + GAM1_ExecTime;"
//             "                           GAM1_A_out = GAM1_A;"
//             "                           GAM1_B_out = GAM1_B;"
//             "                           GAM1_C_out = GAM1_C;"
//             "                         \""
//             "            InputSignals = {"
//             "               GAM1_ReadTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_WriteTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_ExecTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_A = {"
//             "                   DataSource = Drv1"
//             "                   Type = float64"
//             "                   NumberOfDimensions = 0"
//             "                   NumberOfElements = 1"
//             "               }"
//             "               GAM1_B = {"
//             "                   DataSource = Drv1"
//             "                   Type = int16"
//             "                   NumberOfDimensions = 0"
//             "                   NumberOfElements = 1"
//             "               }"
//             "               GAM1_C = {"
//             "                   DataSource = Drv1"
//             "                   Type = float32"
//             "                   NumberOfDimensions = 0"
//             "                   NumberOfElements = 1"
//             "               }"
//             "            }"
//             "            OutputSignals = {"
//             "               GAM1_TotalTime = {"
//             "                   DataSource = DDB1"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_A_out = {"
//             "                   DataSource = Drv1"
//             "                   Type = float64"
//             "                   NumberOfDimensions = 0"
//             "                   NumberOfElements = 1"
//             "               }"
//             "               GAM1_B_out = {"
//             "                   DataSource = Drv1"
//             "                   Type = int16"
//             "                   NumberOfDimensions = 0"
//             "                   NumberOfElements = 1"
//             "               }"
//             "               GAM1_C_out = {"
//             "                   DataSource = Drv1"
//             "                   Type = float32"
//             "                   NumberOfDimensions = 0"
//             "                   NumberOfElements = 1"
//             "               }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Data = {"
//             "        Class = ReferenceContainer"
//             "        DefaultDataSource = DDB1"
//             "        +DDB1 = {"
//             "            Class = GAMDataSource"
//             "        }"
//             "        +Timings = {"
//             "            Class = TimingDataSource"
//             "        }"
//             "        +Drv1 = {"
//             "            Class = MathExpressionGAMDataSourceHelper"
//             "        }"
//             "    }"
//             "    +States = {"
//             "        Class = ReferenceContainer"
//             "        +State1 = {"
//             "            Class = RealTimeState"
//             "            +Threads = {"
//             "                Class = ReferenceContainer"
//             "                +Thread1 = {"
//             "                    Class = RealTimeThread"
//             "                    Functions = {GAM1}"
//             "                }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Scheduler = {"
//             "        Class = GAMScheduler"
//             "        TimingDataSource = Timings"
//             "    }"
//             "}";
//             
//     bool ok = TestIntegratedInApplication(config1, false);
//     ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
//     ReferenceT<MathExpressionGAMHelper> gam = god->Find("Test.Functions.GAM1");
//     if (ok) {
//         ok = gam.IsValid();
//     }
//     RuntimeEvaluator* evalPtr;
//     if (ok) {
//         evalPtr = gam->GetEvaluator();
//     }
//     
//     StreamString signalName;
//     VariableInformation* var;
// 
//     uint32 varIdx = 0u;
//     while (evalPtr->BrowseInputVariable(varIdx, var) && ok) {
//         for (uint32 index = 0u; (index < gam->GetNumberOfInputSignals()) && ok; index++) {
//             ok = gam->GetSignalName(InputSignals, index, signalName);
//             if ((signalName == var->name) && ok) {
//                 ok = (gam->GetSignalType(InputSignals, index) == var->type);
//             }
//             signalName = "";
//         }
//         varIdx++;
//     }
//     
//     varIdx = 0u;
//     while (evalPtr->BrowseOutputVariable(varIdx, var) && ok) {
//         for (uint32 index = 0u; (index < gam->GetNumberOfOutputSignals()) && ok; index++) {
//             ok = gam->GetSignalName(OutputSignals, index, signalName);
//             if ((signalName == var->name) && ok) {
//                 ok = (gam->GetSignalType(OutputSignals, index) == var->type);
//             }
//             signalName = "";
//         }
//         varIdx++;
//     }
//     
//     god->Purge();
//     
//     return ok;
// }
// 
// bool MathExpressionGAMTest::TestExecute_MultipleExpressions() {
//     
//     const char8 * const config1 = ""
//             "$Test = {"
//             "    Class = RealTimeApplication"
//             "    +Functions = {"
//             "        Class = ReferenceContainer"
//             "        +GAM1 = {"
//             "            Class = MathExpressionGAM"
//             "            Expression = \""
//             "                           GAM1_ReadWriteTime = GAM1_ReadTime + GAM1_WriteTime;"
//             "                           GAM1_TotalTime = GAM1_ReadWriteTime + GAM1_ExecTime;"
//             "                         \""
//             "            InputSignals = {"
//             "               GAM1_ReadTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_WriteTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "               GAM1_ExecTime = {"
//             "                   DataSource = Timings"
//             "                   Type = uint32"
//             "               }"
//             "            }"
//             "            OutputSignals = {"
//             "               GAM1_TotalTime = {"
//             "                   DataSource = DDB1"
//             "                   Type = uint32"
//             "               }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Data = {"
//             "        Class = ReferenceContainer"
//             "        DefaultDataSource = DDB1"
//             "        +DDB1 = {"
//             "            Class = GAMDataSource"
//             "        }"
//             "        +Timings = {"
//             "            Class = TimingDataSource"
//             "        }"
//             "    }"
//             "    +States = {"
//             "        Class = ReferenceContainer"
//             "        +State1 = {"
//             "            Class = RealTimeState"
//             "            +Threads = {"
//             "                Class = ReferenceContainer"
//             "                +Thread1 = {"
//             "                    Class = RealTimeThread"
//             "                    Functions = {GAM1}"
//             "                }"
//             "            }"
//             "        }"
//             "    }"
//             "    +Scheduler = {"
//             "        Class = GAMScheduler"
//             "        TimingDataSource = Timings"
//             "    }"
//             "}";
//             
//     bool ok = TestIntegratedInApplication(config1, false);
//     ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
//     ReferenceT<MathExpressionGAM> gam = god->Find("Test.Functions.GAM1");
//     if (ok) {
//         ok = gam.IsValid();
//     }
//     if (ok) {
//         ok = gam->Execute();
//     }
//     god->Purge();
//     return ok;
// }
