/**
 * @file SimulinkWrapperGAMTest.cpp
 * @brief Source file for class SimulinkWrapperGAMTest
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

 * @details This source file contains the definition of all the methods for
 * the class ConstantGAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "File.h"
#include "MatlabDataArray.hpp"
#include "MatlabEngine.hpp"
#include "SimulinkWrapperGAMTest.h"
#include "SimulinkWrapperGAM.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/**
 * @brief Gives access to the SimulinkWrapperGAM memory for testing
 */
class SimulinkWrapperGAMHelper: public MARTe::SimulinkWrapperGAM {
public:
    CLASS_REGISTER_DECLARATION()

    SimulinkWrapperGAMHelper() : MARTe::SimulinkWrapperGAM() {
    }

    ~SimulinkWrapperGAMHelper() {
    }
    
    MARTe::uint32 GetNumberOfParameters() {
        return modelNumOfParameters;
    }

    MARTe::SimulinkRootInterface* GetParameters() {
        return params;
    }

    MARTe::SimulinkRootInterface* GetInputs() {
        return inputs;
    }

    MARTe::SimulinkRootInterface* GetOutputs() {
        return outputs;
    }

    MARTe::SimulinkRootInterface* GetSignals() {
        return signals;
    }

    MARTe::uint32 GetNumberOfLoggingSignals() {
        return modelNumOfSignals;
    }
    
    MARTe::ErrorManagement::ErrorType GetStatus() {
        return this->status;
    }

    void* GetOutputSignalMemoryTest(MARTe::uint32 signalIdx) {
        return GetOutputSignalMemory(signalIdx);
    }
    
    void* GetInputSignalMemoryTest(MARTe::uint32 signalIdx) {
        return GetInputSignalMemory(signalIdx);
    }

    void PrintAlgoInfoTest() {
        PrintAlgoInfo();
    }
     
 };

CLASS_REGISTER(SimulinkWrapperGAMHelper, "1.0");

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
 * Polymorphic version of TestIntegratedInApplication that also returns
 * the pointer to the ObjectRegistryDatabase for further testing
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config,
                                        MARTe::ObjectRegistryDatabase* objRegDatabase,
                                        MARTe::ErrorManagement::ErrorType& status,
                                        bool destroy = true) {

    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    objRegDatabase = ObjectRegistryDatabase::Instance();

    if (ok) {
        objRegDatabase->Purge();
        ok = objRegDatabase->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = objRegDatabase->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
        ReferenceT<SimulinkWrapperGAMHelper> gam = objRegDatabase->Find("Test.Functions.GAM1");
        if (gam.IsValid()) {
            status = gam->GetStatus();
        }
    }

    if (destroy) {
        objRegDatabase->Purge();
    }
    return ok;

}

/**
 * Starts a MARTe application that uses this GAM instance.
 */
static bool TestIntegratedInApplication(const MARTe::char8 * const config,
                                        MARTe::ErrorManagement::ErrorType& status,
                                        bool destroy = true) {
    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    return TestIntegratedInApplication(config, god, status, destroy);
}

/**
 * @brief Class that initialises a test environment for SimulinkWrapperGAM.
 */
class SimulinkGAMGTestEnvironment {
public:

    SimulinkGAMGTestEnvironment() {
        
        // Start MATLAB engine synchronously
	matlabPtr = matlab::engine::startMATLAB({u"-nojvm", u"-nodisplay", u"-softwareopengl"});
        
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
   
    const char8 *envTestFolder = getenv("MARTe2_MATLAB_TEST_SCRIPT_FOLDER");

    if (envTestFolder == NULL) {
        // Add the directory where the test script is located.
        modelScriptFolder  = getenv("MARTe2_Components_DIR");
        modelScriptFolder += "/Test/Components/GAMs/SimulinkWrapperGAM/";
    }
    else {
        modelScriptFolder  = envTestFolder;
    }
    
    addpathCommand  = "addpath(\"";
    addpathCommand += modelScriptFolder.Buffer();
    addpathCommand += "\")";
        
    // Execute setup lines
    matlabPtr->eval(u"clear variables;");
    matlabPtr->eval(convertUTF8StringToUTF16String(addpathCommand.Buffer()));
    
    // Get the name of the work directory
    matlabPtr->eval(u"current_folder = pwd;");
    matlab::data::CharArray currentFolder = matlabPtr->getVariable(u"current_folder");
    
    modelFolder = (currentFolder.toAscii()).c_str();
}

MARTe::StreamString SimulinkGAMGTestEnvironment::CreateTestModel(MARTe::StreamString scriptCallIn) {
    
    using namespace MARTe;
    
    StreamString scriptCall = "[model_compiled, model_name] = ";
    scriptCall += scriptCallIn;

    matlabPtr->eval(matlab::engine::convertUTF8StringToUTF16String(scriptCall.Buffer()));
    matlab::data::CharArray modelNameCharArray = matlabPtr->getVariable(u"model_name");
    StreamString modelName = (modelNameCharArray.toAscii()).c_str();
    
    // Verify that model has been correctly compiled
    matlab::data::TypedArray<bool> modelCompiled = matlabPtr->getVariable(u"model_compiled");
    
    bool ok = modelCompiled[0];
    if (ok) {
        StreamString* currentModelName = new StreamString(modelName);
        compiledModels.Add(currentModelName);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Model %s not compiled (compilation failed or already existent)", modelName.Buffer());
    }
    
    return modelName;
}

void SimulinkGAMGTestEnvironment::DeleteTestModel() {
    
    using namespace MARTe;
    
    bool ok;
    
    for (uint32 modelIdx = 0u; modelIdx < compiledModels.GetSize(); modelIdx++) {
        
        StreamString* currentModelName;
        ok = compiledModels.Peek(modelIdx, currentModelName);
        
        // Remove model .so
        StreamString modelPath;
        Directory toDelete;
        
        modelPath  = modelFolder;
        modelPath += "/";
        modelPath += *currentModelName;
        modelPath += ".so";
        
        toDelete.SetByName(modelPath.Buffer());
        if (toDelete.Exists()) {

            //Comment to avoid recompilation
            ok = toDelete.Delete();
        }
        
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed to delete model %s", currentModelName->Buffer());
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

bool SimulinkWrapperGAMTest::TestInitialiseWithConfiguration(ConfigurationDatabase configIn, ErrorManagement::ErrorType& status) {
    
    SimulinkWrapperGAMHelper gam;
    bool ok = gam.Initialise(configIn);
    status = gam.GetStatus();
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetupWithTemplate(StreamString scriptCall,
                                                   StreamString verbosity,
                                                   StreamString skipUnlinkedParams,
                                                   StreamString inputSignals,
                                                   StreamString outputSignals,
                                                   StreamString parameters,
                                                   ErrorManagement::ErrorType& status,
                                                   ObjectRegistryDatabase* objRegDatabase = NULL_PTR(ObjectRegistryDatabase*),
                                                   bool         structuredSignalsAsByteArrays = true,
                                                   bool         enforceModelSignalCoverage = false
                                                   ) {
    
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
                  verbosity.Buffer(),
                  structuredSignalsAsByteArrays ? "ByteArray" : "Structured",
                  enforceModelSignalCoverage ? "1" : "0",
                  skipUnlinkedParams.Buffer(),
                  inputSignals.Buffer(),
                  outputSignals.Buffer(),
                  parameters.Buffer()
                 );

    // Test setup
    bool ok = false;
    if (objRegDatabase == NULL) {
        ok = TestIntegratedInApplication(config.Buffer(), status);
    }
    else {
        ok = TestIntegratedInApplication(config.Buffer(), objRegDatabase, status, false);
    }
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestInitialise() {
    
    bool ok = true;
    
    StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
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
    ok &= config.Write("SkipInvalidTunableParams",  1);
    
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
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;

    bool ok = TestInitialiseWithConfiguration(config, status);
    
    return !ok && (status.parametersError);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_MissingSymbolPrefix() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelName   = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",                    modelFullPath);
    
    bool ok = TestInitialiseWithConfiguration(config, status);

    return !ok && (status.parametersError);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_WrongNonVirtualBusMode() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",           modelFullPath);
    config.Write("SymbolPrefix",      modelName);
    config.Write("NonVirtualBusMode", "Structure");
    
    bool ok = TestInitialiseWithConfiguration(config, status);

    return !ok && (status.parametersError);
    
}


bool SimulinkWrapperGAMTest::TestInitialise_MissingTunableParamExternalSource() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);

    bool ok = TestInitialiseWithConfiguration(config, status);
    
    return ok && (status.ErrorsCleared());
    
}

bool SimulinkWrapperGAMTest::TestInitialise_MissingOptionalConfigurationSettings() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    bool ok = TestInitialiseWithConfiguration(config, status);

    return ok && (status.ErrorsCleared());
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LoadLibrary() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
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

    bool ok = TestInitialiseWithConfiguration(config, status);
    
    return (!ok) && (status.initialisationError);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LoadSymbols() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
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
    
    bool ok = TestInitialiseWithConfiguration(config, status);

    return (!ok) && (status.initialisationError);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LibraryMissingGetMmiFunction() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel('hasGetmmiFcn', false);");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    bool ok = TestInitialiseWithConfiguration(config, status);

    return (!ok) && (status.initialisationError);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LibraryMissingAllocFunction() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel('hasAllocFcn', false);");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    
    bool ok = TestInitialiseWithConfiguration(config, status);

    return (!ok) && (status.initialisationError);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_MissingParametersLeaf() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    config.Write("TunableParamExternalSource", "ExtSource");
    config.Write("Verbosity",                  2);
    config.Write("SkipInvalidTunableParams",  1);
    
    bool ok = TestInitialiseWithConfiguration(config, status);
    
    return (ok) && (status.ErrorsCleared());
}


bool SimulinkWrapperGAMTest::TestSetup() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true');";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In3_VectorDouble = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "In4_VectorUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "In5_MatrixDouble = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "In6_MatrixUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_1() {
    //TODO Check if hasTunableParams has to be added

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructParams', true, 'hasTunableParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = ""
        "structScalar.one = (float64) 1 "
        "structScalar.pnested1.one = (float64) 1 "
        "structScalar.pnested1.two = (float64) 1 "
        "structScalar.pnested2.one = (float64) 1 "
        "structScalar.pnested2.two = (float64) 1 ";
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_2() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true', 'hasInputs', false);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar = {"
        "    one         = (float64) 3.141592653 "
        "    pnested1 = {"
        "        one = (float64) 2.718281828 "
        "        two = (float64) 2.718281828 "
        "    }"
        "    pnested2 = {"
        "        one = (float64) 1.414213562 "
        "        two = (float64) 1.414213562 "
        "    }"
        "}"
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_3() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 4, 'hasTunableParams', true, 'hasStructParams', true', 'hasInputs', false);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out7_3DMatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "Out8_3DMatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-pnested1-one = (float64) 2.718281828 "
        "structScalar-pnested1-two = (float64) 2.718281828 "
        "structScalar-pnested2-one = (float64) 1.414213562 "
        "structScalar-pnested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
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
                  "2",
                  "ByteArray",
                  "0",
                  skipUnlinkedParams.Buffer(),
                  inputSignals.Buffer(),
                  outputSignals.Buffer(),
                  parameters.Buffer()
                 );
    
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
    
    // The GAM external source of parameters is expected to be a
    // ReferenceContainer populated by AnyObject
    if (ok) {
        
        // Create the ReferenceContainer
        ReferenceT<ReferenceContainer> cfgParameterContainer("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cfgParameterContainer->SetName("ExtSource");
        
        ok = cfgParameterContainer.IsValid();
        
        // Populate with AnyObjects
        uint32 param1[4u][4u][4u] = { { {1u,1u,1u,1u}, {1u,1u,1u,1u}, {1u,1u,1u,1u}, {1u,1u,1u,1u} },
                                      { {1u,1u,1u,1u}, {1u,1u,1u,1u}, {1u,1u,1u,1u}, {1u,1u,1u,1u} },
                                      { {1u,1u,1u,1u}, {1u,1u,1u,1u}, {1u,1u,1u,1u}, {1u,1u,1u,1u} }
                                    };
                               
        TypeDescriptor type1 = TypeDescriptor::GetTypeDescriptorFromTypeName("uint32");
        AnyType anyParam1(type1, 0u, param1);
        anyParam1.SetNumberOfDimensions(3u);
        anyParam1.SetNumberOfElements(0u, 4u);
        anyParam1.SetNumberOfElements(1u, 4u);
        anyParam1.SetNumberOfElements(2u, 4u);
        
        ReferenceT<AnyObject> objParam1("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        objParam1->Serialise(anyParam1);
        objParam1->SetName("structMixed.mat3d");
        cfgParameterContainer->Insert("structMixed.mat3d", objParam1);

        Reference foo = cfgParameterContainer->Find("structMixed.mat3d");

        float64 param2[4u][4u][4u] = { { {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0} },
                                       { {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0} },
                                       { {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0}, {1.0,1.0,1.0,1.0} }
                                     };
                               
        TypeDescriptor type2 = TypeDescriptor::GetTypeDescriptorFromTypeName("float64");
        AnyType anyParam2(type2, 0u, param2);
        anyParam2.SetNumberOfDimensions(3u);
        anyParam2.SetNumberOfElements(0u, 4u);
        anyParam2.SetNumberOfElements(1u, 4u);
        anyParam2.SetNumberOfElements(2u, 4u);
        
        ReferenceT<AnyObject> objParam2("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        objParam2->Serialise(anyParam2);
        objParam2->SetName("matrixConstant3d");
        cfgParameterContainer->Insert(objParam2);

        if (ok) {
            ok = ObjectRegistryDatabase::Instance()->Insert(cfgParameterContainer);
        }
        
    }
    
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = god->Find("Test.Functions.GAM1");
        if (gam.IsValid()) {
            status = gam->GetStatus();
        }
    }

    god->Purge();
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParametersFromExternalSource() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    
    StreamString scriptCall = "createTestModel('hasStructParams', true, 'hasTunableParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = ""
        "structScalar.pnested1.one = (float64) 1 "
        "structScalar.pnested1.two = (float64) 1 "
        "structScalar.pnested2.one = (float64) 1 ";
        
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
                  "2",
                  "ByteArray",
                  "0",
                  skipUnlinkedParams.Buffer(),
                  inputSignals.Buffer(),
                  outputSignals.Buffer(),
                  parameters.Buffer()
                 );
    
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
    
    // The GAM external source of parameters is expected to be a
    // ReferenceContainer populated by AnyObject
    if (ok) {
        
        // Create the ReferenceContainer
        ReferenceT<ReferenceContainer> cfgParameterContainer("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cfgParameterContainer->SetName("ExtSource");
        
        ok = cfgParameterContainer.IsValid();
        
        if (ok) {
            ok = ObjectRegistryDatabase::Instance()->Insert(cfgParameterContainer);
        }
        
        // Populate with AnyObjects
        float64 param1 = 1.0;
        AnyType anyParam(param1);
        
        ReferenceT<AnyObject> objParam1("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        objParam1->Serialise(anyParam);
        objParam1->SetName("structScalar.pnested2.two");
        cfgParameterContainer->Insert("structScalar.pnested2.two", objParam1);
        
        ReferenceT<AnyObject> objParam2("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        objParam2->Serialise(anyParam);
        objParam2->SetName("structScalar.one");
        cfgParameterContainer->Insert("structScalar.one", objParam2);
        
        // Verify that non-AnyObject references are ignored
        ReferenceT<ReferenceContainer> refContainer("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        refContainer->SetName("structScalar.pnested2.one");
        cfgParameterContainer->Insert("structScalar.pnested2.one", refContainer);
    }
    
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = god->Find("Test.Functions.GAM1");
        if (gam.IsValid()) {
            status = gam->GetStatus();
        }
    }

    god->Purge();
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_NoTunableParameters() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasTunableParams',     false);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_WithStructSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    
    StreamString scriptCall = "createTestModel('hasStructSignals', true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured  = {"
        "        DataSource = Drv1"
        "        Type = uint8"
        "        NumberOfElements = 16"
        "        NumberOfDimensions = 1"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out20_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint8"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_WithNestedStructSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'modelComplexity', 2, 'hasInputs', false);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = "";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out20_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint8"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 1"
        "}"
        "Out21_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint8"
        "    NumberOfElements = 96"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_WithLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasLoggingSignals', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured  = { DataSource = Drv1   Type = uint8   NumberOfElements = 16   NumberOfDimensions = 1 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble    = { DataSource = DDB1    Type = float64    NumberOfElements = 1   NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32    = { DataSource = DDB1    Type = uint32     NumberOfElements = 1   NumberOfDimensions = 0 }"
        "    Out20_NonVirtualBus  = { DataSource = DDB1    Type = uint8      NumberOfElements = 16  NumberOfDimensions = 1 }"
        "    Log1_ScalarDouble    = { DataSource = DDB1    Type = float64    NumberOfElements = 1   NumberOfDimensions = 0 }"
        "    Log2_ScalarUint32    = { DataSource = DDB1    Type = uint32     NumberOfElements = 1   NumberOfDimensions = 0 }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_SkipInvalidTunableParams() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasTunableParams',     true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = ""
        "vectorConstant = (float64) {1, 1, 1, 1, 1, 1, 1, 1}";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_DontSkipUnlinkedTunableParams() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    StreamString scriptCall = "createTestModel('hasTunableParams',     true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = ""
        "vectorConstant = (float64) {1, 1, 1, 1, 1, 1, 1, 1}";

    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfInputs() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfOutputs() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasLoggingSignals', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In2_ScalarUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log3_Error        = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_EmptyInterfaceName() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasSignalNames', false);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In2_ScalarUINT32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongInputName() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUINT32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongOutputName() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUINT32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}


bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongLoggingSignalName() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasLoggingSignals', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In2_ScalarUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log2_Error        = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongDatatype() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfElements() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfDimensions() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
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
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_LoggingSignalWrongDatatype() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasLoggingSignals', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In2_ScalarUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log1_ScalarDouble = { DataSource = DDB1    Type = uint64     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongNumberOfDimensions() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = ""
        "vectorConstant = (uint32) { {1, 1}, {1, 1} }";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDimensions() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = ""
        "vectorConstant = (uint32) {1, 1}";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDimensions_Matrix() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasInputs', false);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        ;
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDatatype() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "}";
    
    StreamString parameters = ""
        "vectorConstant = (float64) {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,}";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_NoInputs() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasInputs', false);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = "";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return ok && status.ErrorsCleared();
}


bool SimulinkWrapperGAMTest::TestSetup_NoOutputs() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasOutputs', false);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString outputSignals = ""
        "";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return ok && status.ErrorsCleared();
}


bool SimulinkWrapperGAMTest::TestSetup_LowVerbosity() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel();";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "   In1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "   In2_ScalarUint32 = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}"
        ;

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 1 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return ok && status.ErrorsCleared();
}


bool SimulinkWrapperGAMTest::TestSetup_ZeroVerbosity() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "   In1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "   In2_ScalarUint32 = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "}"
        ;
    
    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 0 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return ok && status.ErrorsCleared();
}

bool SimulinkWrapperGAMTest::TestSetup_StructArraysAsParams() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true, 'hasStructArrayParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
            
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = ""
        "structParamArray[0].one = (float64) 1 "
        "structParamArray[0].two = (float64) 1 "
        "structParamArray[1].one = (float64) 2 "
        "structParamArray[1].two = (float64) 2 ";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status);
}

bool SimulinkWrapperGAMTest::TestSetup_NestedStructArraysAsParams() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true, 'hasStructParams', true, 'hasStructArrayParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
            
    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";
    
    StreamString parameters = ""
        "structScalar.one = (float64) 1 "
        "structParamArray[0].one = (float64) 1 "
        "structParamArray[0].two = (float64) 1 "
        "structParamArray[1].one = (float64) 2 "
        "structParamArray[1].two = (float64) 2 "
        "structMixed.structParamArray[0].one = (uint32) 1 "
        "structMixed.structParamArray[0].two = (float64) 1 "
        "structMixed.structParamArray[1].one = (uint32) 2 "
        "structMixed.structParamArray[1].two = (float64) 2 ";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfDimensionsWithStructSignals() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals',     true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_Structured = { "
        "   DataSource = Drv1"
        "   Type = uint8"
        "   NumberOfElements = 16"
        "   NumberOfDimensions = 1"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out20_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint8"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongDatatypeWithStructSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals',     true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_Structured = { "
        "   DataSource = Drv1"
        "   Type = uint8"
        "   NumberOfElements = 16"
        "   NumberOfDimensions = 1"
        "}"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out20_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

#ifdef ROW_MAJOR_ND_FEATURE

bool SimulinkWrapperGAMTest::TestParameterActualisation_RowMajorModel() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    // Notice that model has to be row-major, otherwise raw memory
    // comparison between matrices is not feasible (row-major in MARTe2
    // but column-major in the model).
    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true', 'hasInputs', false, 'dataOrientation', 'Row-major');";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord);
    
    // Now check if parameter values have been correctly loaded.
    
    // First, build a database with what has been loaded in the model
    ConfigurationDatabase cdb;
    if (ok) {
        parameters.Seek(0u);
        StandardParser parser(parameters, cdb);
        ok = parser.Parse();
    }
    
    // Then, import the parameters back from the model and compare
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");
        
        ok = gam.IsValid();
        if (ok) {

            SimulinkRootInterface* params = gam->GetParameters();

            for (uint32 rootParamIdx = 0u; (rootParamIdx < gam->GetNumberOfParameters()) && ok; rootParamIdx++) {

                for (uint32 elemIdx = 0u; (elemIdx < params[rootParamIdx].GetSize()) && ok; elemIdx++) {

                    StreamString paramName = params[rootParamIdx][elemIdx]->fullPath;
                    uint32       paramSize = params[rootParamIdx][elemIdx]->byteSize;
                    void*        paramAddr = params[rootParamIdx][elemIdx]->dataAddr;

                    AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                    ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                    if (ok) {
                        ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                    }
                }

            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}

#endif /* ROW_MAJOR_ND_FEATURE */

bool SimulinkWrapperGAMTest::TestParameterActualisation_ColumnMajorModel() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true', 'hasInputs', false, 'dataOrientation', 'Column-major');";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
    // Since the model is column-major, parameters are transposed
    // when loaded, so raw memory comparison must be carried out
    // with transposed parameters.
    StreamString transposedParameters = ""
        "matrixConstant = (float64) { {10, 11, 12}, {10, 11, 12}, {10, 11, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint32) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord);
    
    // Now check if parameter values have been correctly loaded.
    
    // First, build a database with what has been loaded in the model
    ConfigurationDatabase cdb;
    if (ok) {
        transposedParameters.Seek(0u);
        StandardParser parser(transposedParameters, cdb);
        ok = parser.Parse();
    }
    
    // Then, import the parameters back from the model and compare
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");
        
        ok = gam.IsValid();
        if (ok) {

            SimulinkRootInterface* params = gam->GetParameters();

            for (uint32 rootParamIdx = 0u; (rootParamIdx < gam->GetNumberOfParameters()) && ok; rootParamIdx++) {

                for (uint32 elemIdx = 0u; (elemIdx < params[rootParamIdx].GetSize()) && ok; elemIdx++) {

                    StreamString paramName = params[rootParamIdx][elemIdx]->fullPath;
                    uint32       paramSize = params[rootParamIdx][elemIdx]->byteSize;
                    void*        paramAddr = params[rootParamIdx][elemIdx]->dataAddr;

                    AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                    ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                    if (ok) {
                        ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                    }
                }

            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_Uint() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'useType', 1, 'hasInputs', false, 'hasTunableParams', true, 'hasStructParams', true, 'hasStructArrayParams', false);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = uint8"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint16"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (uint8) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint16)  { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        ;
    
    // Since the model is column-major, parameters are transposed
    // when loaded, so raw memory comparison must be carried out
    // with transposed parameters.
    StreamString transposedParameters = ""
        "matrixConstant = (float64) { {10, 11, 12}, {10, 11, 12}, {10, 11, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (uint8) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (uint16) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord);
    
    // Now check if parameter values have been correctly loaded.
    
    // First, build a database with what has been loaded in the model
    ConfigurationDatabase cdb;
    if (ok) {
        transposedParameters.Seek(0u);
        StandardParser parser(transposedParameters, cdb);
        ok = parser.Parse();
    }
    
    // Then, import the parameters back from the model and compare
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");
        
        ok = gam.IsValid();
        if (ok) {

            SimulinkRootInterface* params = gam->GetParameters();

            for (uint32 rootParamIdx = 0u; (rootParamIdx < gam->GetNumberOfParameters()) && ok; rootParamIdx++) {

                for (uint32 elemIdx = 0u; (elemIdx < params[rootParamIdx].GetSize()) && ok; elemIdx++) {

                    StreamString paramName = params[rootParamIdx][elemIdx]->fullPath;
                    uint32       paramSize = params[rootParamIdx][elemIdx]->byteSize;
                    void*        paramAddr = params[rootParamIdx][elemIdx]->dataAddr;

                    AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                    ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                    if (ok) {
                        ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                    }
                }

            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_Int() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'useType', 2, 'hasInputs', false, 'hasTunableParams', true, 'hasStructParams', true, 'hasStructArrayParams', false);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = int8"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = int16"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int8) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (int16)  { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        ;
    
    // Since the model is column-major, parameters are transposed
    // when loaded, so raw memory comparison must be carried out
    // with transposed parameters.
    StreamString transposedParameters = ""
        "matrixConstant = (float64) { {10, 11, 12}, {10, 11, 12}, {10, 11, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int8) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (int16) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord);
    
    // Now check if parameter values have been correctly loaded.

    // First, build a database with what has been loaded in the model
    ConfigurationDatabase cdb;
    if (ok) {
        transposedParameters.Seek(0u);
        StandardParser parser(transposedParameters, cdb);
        ok = parser.Parse();
    }
    
    // Then, import the parameters back from the model and compare
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");
        
        ok = gam.IsValid();
        if (ok) {

            SimulinkRootInterface* params = gam->GetParameters();

            for (uint32 rootParamIdx = 0u; (rootParamIdx < gam->GetNumberOfParameters()) && ok; rootParamIdx++) {

                for (uint32 elemIdx = 0u; (elemIdx < params[rootParamIdx].GetSize()) && ok; elemIdx++) {

                    StreamString paramName = params[rootParamIdx][elemIdx]->fullPath;
                    uint32       paramSize = params[rootParamIdx][elemIdx]->byteSize;
                    void*        paramAddr = params[rootParamIdx][elemIdx]->dataAddr;

                    AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                    ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                    if (ok) {
                        ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                    }
                }

            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_Float() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'useType', 4, 'hasInputs', false, 'hasTunableParams', true, 'hasStructParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = int32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = float32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (float32)  { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        ;
    
    // Since the model is column-major, parameters are transposed
    // when loaded, so raw memory comparison must be carried out
    // with transposed parameters.
    StreamString transposedParameters = ""
        "matrixConstant = (float64) { {10, 11, 12}, {10, 11, 12}, {10, 11, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int32) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed.mat = (float32) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord);
    
    // Now check if parameter values have been correctly loaded.
    
    // First, build a database with what has been loaded in the model
    ConfigurationDatabase cdb;
    if (ok) {
        transposedParameters.Seek(0u);
        StandardParser parser(transposedParameters, cdb);
        ok = parser.Parse();
    }
    
    // Then, import the parameters back from the model and compare
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");
        
        ok = gam.IsValid();
        if (ok) {

            SimulinkRootInterface* params = gam->GetParameters();

            for (uint32 rootParamIdx = 0u; (rootParamIdx < gam->GetNumberOfParameters()) && ok; rootParamIdx++) {

                for (uint32 elemIdx = 0u; (elemIdx < params[rootParamIdx].GetSize()) && ok; elemIdx++) {

                    StreamString paramName = params[rootParamIdx][elemIdx]->fullPath;
                    uint32       paramSize = params[rootParamIdx][elemIdx]->byteSize;
                    void*        paramAddr = params[rootParamIdx][elemIdx]->dataAddr;

                    AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                    ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                    if (ok) {
                        ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                    }
                }

            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}

bool SimulinkWrapperGAMTest::TestExecute() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = " createTestModel('modelComplexity', 4, 'hasTunableParams', true);";
    
    StreamString skipUnlinkedParams = "0";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "In4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "In5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "In6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "In7_3DMatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "In8_3DMatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 10"
        "    NumberOfDimensions = 1"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out7_3DMatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "Out8_3DMatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }";
    
    StreamString inputValues = ""
        "In1_ScalarDouble = (float64) 3.141592653 "
        "In2_ScalarUint32 = (uint32)  2 "
        "In3_VectorDouble = (float64) { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "In4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 } "
        "In5_MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "In6_MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "In7_3DMatrixDouble = (float64) { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        "In8_3DMatrixUint32 = (uint32)  { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        ;
    
    // Model is column-major, so matrices are expected to be transposed when copied
    StreamString expectedInputValues = ""
        "In1_ScalarDouble = (float64) 3.141592653 "
        "In2_ScalarUint32 = (uint32)  2 "
        "In3_VectorDouble = (float64) { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "In4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 } "
        "In5_MatrixDouble = (float64) { { 10, 10, 10, 10, 10, 10}, "
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 12, 12, 12, 12, 12, 12} }"
        "In6_MatrixUint32 = (uint32)  { { 10, 10, 10, 10, 10, 10}, "
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 12, 12, 12, 12, 12, 12} }"
        "In7_3DMatrixDouble = (float64) { { 20, 20, 20, 20 },"
        "                                 { 21, 21, 21, 21 },"
        "                                 { 22, 22, 22, 22 },"
        "                                 { 23, 23, 23, 23 } }"
        "In8_3DMatrixUint32 = (uint32)  { { 20, 20, 20, 20 },"
        "                                 { 21, 21, 21, 21 },"
        "                                 { 22, 22, 22, 22 },"
        "                                 { 23, 23, 23, 23 } }"
        ;
        
    StreamString expectedOutputValues = ""
        "Out1_ScalarDouble = (float64) 3.141592653 "
        "Out2_ScalarUint32 = (uint32)  { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 } "
        "Out3_VectorDouble = (float64) { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Out4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 } "
        "Out5_MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "Out6_MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "Out7_3DMatrixDouble = (float64) { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        "Out8_3DMatrixUint32 = (uint32)  { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        ;
    
    // Setup GAM
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord);
    
    // Build a database with what shall be loaded as input
    ConfigurationDatabase cdb;
    if (ok) {
        inputValues.Seek(0u);
        StandardParser parser(inputValues, cdb);
        ok = parser.Parse();
    }

    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");
        
        ok = gam.IsValid();
        
        // Copy inputValues to the GAM input signal memory
        if (ok) {
            
            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {
                
                StreamString signalName;
                ok = gam->GetSignalName(InputSignals, signalIdx, signalName);
                
                
                AnyType arrayDescription = cdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                
                //
                uint32 memoryAllocationSize = 0u;
                switch (arrayDescription.GetNumberOfDimensions()) {
                    
                    case 0u:
                        memoryAllocationSize = arrayDescription.GetByteSize();
                        break;
                    
                    case 1u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u);
                        break;
                    
                    case 2u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u) * arrayDescription.GetNumberOfElements(1u);
                        break;
                }
                if (ok) {
                    ok = MemoryOperationsHelper::Copy(gam->GetInputSignalMemoryTest(signalIdx), arrayDescription.GetDataPointer(), memoryAllocationSize);
                }
            }
            
            ok = gam->Execute();
            
        }
        
        // Check if signals were correctly copied in the model
        
        ConfigurationDatabase inCdb;
        if (ok) {
            expectedInputValues.Seek(0u);
            StandardParser parser(expectedInputValues, inCdb);
            ok = parser.Parse();
        }
        
        ConfigurationDatabase outCdb;
        if (ok) {
            expectedOutputValues.Seek(0u);
            StandardParser parser(expectedOutputValues, outCdb);
            ok = parser.Parse();
        }
        
        // Compare input and outputs with expected values
        if (ok) {
            
            SimulinkRootInterface* inputs = gam->GetInputs();

            for (uint32 rootInputIdx = 0u; (rootInputIdx < gam->GetNumberOfInputSignals()) && ok; rootInputIdx++) {

                StreamString inputName = inputs[rootInputIdx].fullPath;
                uint32       inputSize = inputs[rootInputIdx].byteSize;
                void*        inputAddr = inputs[rootInputIdx].destPtr;

                AnyType arrayDescription = inCdb.GetType(inputName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(inputAddr, arrayDescription.GetDataPointer(), inputSize) == 0u);
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s comparison failed.", inputName.Buffer());
                    }
                }
            }

            SimulinkRootInterface* outputs = gam->GetOutputs();

            for (uint32 rootOutputIdx = 0u; (rootOutputIdx < gam->GetNumberOfOutputSignals()) && ok; rootOutputIdx++) {

                StreamString outputName = outputs[rootOutputIdx].fullPath;
                uint32       outputSize = outputs[rootOutputIdx].byteSize;
                void*        outputAddr = outputs[rootOutputIdx].destPtr;

                AnyType arrayDescription = outCdb.GetType(outputName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(outputAddr, arrayDescription.GetDataPointer(), outputSize) == 0u);
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s comparison failed.", outputName.Buffer());
                    }
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}

bool SimulinkWrapperGAMTest::TestPrintAlgoInfo() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    bool ok = true;
    
    StreamString modelName, modelFolder, modelFullPath;
    
    modelFolder = testEnvironment.modelFolder;
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";

    SimulinkWrapperGAMHelper gam;
    ConfigurationDatabase    config;
    
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
    
    if (ok) {
        gam.PrintAlgoInfoTest();
    }

    if (ok) {
        status = gam.GetStatus();
    }
    
    return ok;
    
}

bool SimulinkWrapperGAMTest::Test_StructuredSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { "
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        In2_ScalarUint32  = { "
        "            DataSource = Drv1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out20_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        Signal2  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, false);


    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithStructuredLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true, 'hasLoggingSignals', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        In2_ScalarUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out20_NonVirtualBus = { "
        "        Signal1  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        Signal2  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    Log20_NonVirtualBus = { "
        "        Signal1  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        Signal2  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, false);


    return ok && status;
}

bool SimulinkWrapperGAMTest::TestExecute_WithLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = " createTestModel('modelComplexity', 4, 'hasTunableParams', true, 'hasLoggingSignals', true);";

    StreamString skipUnlinkedParams = "0";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    In4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    In5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    In6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    In7_3DMatrixDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 16   NumberOfDimensions = 2 }"
        "    In8_3DMatrixUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 16   NumberOfDimensions = 2 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 10   NumberOfDimensions = 1 }"
        "    Out3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out7_3DMatrixDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 16   NumberOfDimensions = 2 }"
        "    Out8_3DMatrixUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 16   NumberOfDimensions = 2 }"
        "    Log1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 10   NumberOfDimensions = 1 }"
        "    Log3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Log4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Log5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Log6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "}";

    StreamString parameters = ""
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }";

    StreamString inputValues = ""
        "In1_ScalarDouble = (float64) 3.141592653 "
        "In2_ScalarUint32 = (uint32)  2 "
        "In3_VectorDouble = (float64) { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "In4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 } "
        "In5_MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "In6_MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "In7_3DMatrixDouble = (float64) { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        "In8_3DMatrixUint32 = (uint32)  { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        ;

    // Model is column-major, so matrices are expected to be transposed when copied
    StreamString expectedInputValues = ""
        "In1_ScalarDouble = (float64) 3.141592653 "
        "In2_ScalarUint32 = (uint32)  2 "
        "In3_VectorDouble = (float64) { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "In4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 } "
        "In5_MatrixDouble = (float64) { { 10, 10, 10, 10, 10, 10}, "
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 12, 12, 12, 12, 12, 12} }"
        "In6_MatrixUint32 = (uint32)  { { 10, 10, 10, 10, 10, 10}, "
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 11, 11, 11, 11, 11, 11},"
        "                               { 12, 12, 12, 12, 12, 12} }"
        "In7_3DMatrixDouble = (float64) { { 20, 20, 20, 20 },"
        "                                 { 21, 21, 21, 21 },"
        "                                 { 22, 22, 22, 22 },"
        "                                 { 23, 23, 23, 23 } }"
        "In8_3DMatrixUint32 = (uint32)  { { 20, 20, 20, 20 },"
        "                                 { 21, 21, 21, 21 },"
        "                                 { 22, 22, 22, 22 },"
        "                                 { 23, 23, 23, 23 } }"
        ;

    StreamString expectedOutputValues = ""
        "Out1_ScalarDouble = (float64) 3.141592653 "
        "Out2_ScalarUint32 = (uint32)  { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 } "
        "Out3_VectorDouble = (float64) { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Out4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 } "
        "Out5_MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "Out6_MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "Out7_3DMatrixDouble = (float64) { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        "Out8_3DMatrixUint32 = (uint32)  { { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 },"
        "                                 { 20, 21, 22, 23 } }"
        ;

    StreamString expectedSignalValues = ""
        "Log1_ScalarDouble = (float64) 3.141592653 "
        "Log2_ScalarUint32 = (uint32)  { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 } "
        "Log3_VectorDouble = (float64) { 10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Log4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 } "
        "Log5_MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        "Log6_MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12},"
        "                               { 10, 11, 11, 11, 11, 12} }"
        ;

    // Setup GAM
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord);

    // Build a database with what shall be loaded as input
    ConfigurationDatabase cdb;
    if (ok) {
        inputValues.Seek(0u);
        StandardParser parser(inputValues, cdb);
        ok = parser.Parse();
    }

    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");

        ok = gam.IsValid();

        // Copy inputValues to the GAM input signal memory
        if (ok) {

            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {

                StreamString signalName;
                ok = gam->GetSignalName(InputSignals, signalIdx, signalName);

                AnyType arrayDescription = cdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);

                //
                uint32 memoryAllocationSize = 0u;
                switch (arrayDescription.GetNumberOfDimensions()) {

                    case 0u:
                        memoryAllocationSize = arrayDescription.GetByteSize();
                        break;

                    case 1u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u);
                        break;

                    case 2u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u) * arrayDescription.GetNumberOfElements(1u);
                        break;
                }
                if (ok) {
                    ok = MemoryOperationsHelper::Copy(gam->GetInputSignalMemoryTest(signalIdx), arrayDescription.GetDataPointer(), memoryAllocationSize);
                }
            }

            ok = gam->Execute();

        }

        // Check if signals were correctly copied in the model

        ConfigurationDatabase inCdb;
        if (ok) {
            expectedInputValues.Seek(0u);
            StandardParser parser(expectedInputValues, inCdb);
            ok = parser.Parse();
        }

        ConfigurationDatabase outCdb;
        if (ok) {
            expectedOutputValues.Seek(0u);
            StandardParser parser(expectedOutputValues, outCdb);
            ok = parser.Parse();
        }

        ConfigurationDatabase sigCdb;
        if (ok) {
            expectedSignalValues.Seek(0u);
            StandardParser parser(expectedSignalValues, sigCdb);
            ok = parser.Parse();
        }

        // Compare input, outputs and logging signals with expected values
        if (ok) {

            SimulinkRootInterface* inputs = gam->GetInputs();

            for (uint32 rootInputIdx = 0u; (rootInputIdx < gam->GetNumberOfInputSignals()) && ok; rootInputIdx++) {

                StreamString inputName = inputs[rootInputIdx].fullPath;
                uint32       inputSize = inputs[rootInputIdx].byteSize;
                void*        inputAddr = inputs[rootInputIdx].destPtr;

                AnyType arrayDescription = inCdb.GetType(inputName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(inputAddr, arrayDescription.GetDataPointer(), inputSize) == 0u);
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s comparison failed.", inputName.Buffer());
                    }
                }
            }

            SimulinkRootInterface* outputs = gam->GetOutputs();

            for (uint32 rootOutputIdx = 0u; (rootOutputIdx < (gam->GetNumberOfOutputSignals() - gam->GetNumberOfLoggingSignals())) && ok; rootOutputIdx++) {

                StreamString outputName = outputs[rootOutputIdx].fullPath;
                uint32       outputSize = outputs[rootOutputIdx].byteSize;
                void*        outputAddr = outputs[rootOutputIdx].destPtr;

                AnyType arrayDescription = outCdb.GetType(outputName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);

                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(outputAddr, arrayDescription.GetDataPointer(), outputSize) == 0u);
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s comparison failed.", outputName.Buffer());
                    }
                }
            }

            SimulinkRootInterface* signals = gam->GetSignals();

            for (uint32 loggingSignalIdx = 0u; (loggingSignalIdx < gam->GetNumberOfLoggingSignals()) && ok; loggingSignalIdx++) {

                StreamString signalName = signals[loggingSignalIdx].fullPath;
                uint32       signalSize = signals[loggingSignalIdx].byteSize;
                void*        signalAddr = signals[loggingSignalIdx].destPtr;

                AnyType arrayDescription = sigCdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);

                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(signalAddr, arrayDescription.GetDataPointer(), signalSize) == 0u);
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s comparison failed.", signalName.Buffer());
                    }
                }
            }
        }
    }

    if (ok) {
        ord->Purge();
    }

    return ok && status;
}


bool SimulinkWrapperGAMTest::TestExecute_WithStructuredSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { "
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        In2_ScalarUint32  = { "
        "            DataSource = Drv1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out20_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        Signal2  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString inputValues = ""
        "In1_Structured.In1_ScalarDouble = (float64) 3.141592653 "
        "In1_Structured.In2_ScalarUint32 = (uint32)  2";
        
    StreamString expectedOutputValues = ""
        "Out20_NonVirtualBus.Signal2 = (float64) 3.141592653 "
        "Out20_NonVirtualBus.Signal1 = (uint32)  2";

    StreamString parameters = "";

    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord, false, false);

    ConfigurationDatabase cdb;
    if (ok) {
        inputValues.Seek(0u);
        StandardParser parser(inputValues, cdb);
        ok = parser.Parse();
    }
    
    ConfigurationDatabase outCdb;
    if (ok) {
        expectedOutputValues.Seek(0u);
        StandardParser parser(expectedOutputValues, outCdb);
        ok = parser.Parse();
    }

    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");

        ok = gam.IsValid();

        // Copy inputValues to the GAM input signal memory
        if (ok) {

            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {

                StreamString signalName;
                ok = gam->GetSignalName(InputSignals, signalIdx, signalName);


                AnyType arrayDescription = cdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);

                //
                uint32 memoryAllocationSize = 0u;
                switch (arrayDescription.GetNumberOfDimensions()) {

                    case 0u:
                        memoryAllocationSize = arrayDescription.GetByteSize();
                        break;

                    case 1u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u);
                        break;

                    case 2u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u) * arrayDescription.GetNumberOfElements(1u);
                        break;
                }
                if (ok) {
                    ok = MemoryOperationsHelper::Copy(gam->GetInputSignalMemoryTest(signalIdx), arrayDescription.GetDataPointer(), memoryAllocationSize);
                }
            }

            ok = gam->Execute();
        }

        if(ok) {
            SimulinkRootInterface* inputs = gam->GetInputs();
            SimulinkInterface* in1 = inputs[0][0];
            SimulinkInterface* in2 = inputs[0][1];

            AnyType adIn1 = cdb.GetType(in1->fullPath.Buffer());
            AnyType adIn2 = cdb.GetType(in2->fullPath.Buffer());

            ok =    adIn1.GetDataPointer() != NULL_PTR(void *) &&
                    adIn2.GetDataPointer() != NULL_PTR(void *);

            if(ok) {
                ok =    (MemoryOperationsHelper::Compare(in1->destPtr, adIn1.GetDataPointer(), in1->byteSize) == 0u) &&
                        (MemoryOperationsHelper::Compare(in2->destPtr, adIn2.GetDataPointer(), in2->byteSize) == 0u);
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Wrong data pointers");
            }
        }

        if (ok) {
            SimulinkRootInterface* outputs = gam->GetOutputs();
            SimulinkInterface* out1 = outputs[2][0];
            SimulinkInterface* out2 = outputs[2][1];

            AnyType adOut1 = outCdb.GetType(out1->fullPath.Buffer());
            AnyType adOut2 = outCdb.GetType(out2->fullPath.Buffer());

            ok =    adOut1.GetDataPointer() != NULL_PTR(void *) &&
                    adOut2.GetDataPointer() != NULL_PTR(void *);
            if(ok) {
                ok =    (SafeMath::IsEqual<float64>( *( (float64*) out2->destPtr), *( (float64*) adOut2.GetDataPointer() ) ) ) &&
                        (SafeMath::IsEqual<uint32> ( *( (uint32*)  out1->destPtr), *( (uint32*)  adOut1.GetDataPointer() ) ) );
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}


bool SimulinkWrapperGAMTest::TestExecute_WithStructuredLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true, 'hasLoggingSignals', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        In2_ScalarUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out20_NonVirtualBus = { "
        "        Signal1  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        Signal2  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    Log20_NonVirtualBus = { "
        "        Signal1  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        Signal2  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString inputValues = ""
        "In1_Structured.In1_ScalarDouble = (float64) 3.141592653 "
        "In1_Structured.In2_ScalarUint32 = (uint32)  2";

    StreamString expectedLoggingSignalValues = ""
        "Log20_NonVirtualBus.Signal2 = (float64) 3.141592653 "
        "Log20_NonVirtualBus.Signal1 = (uint32)  2";

    StreamString parameters = "";

    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord, false, false);

    ConfigurationDatabase cdb;
    if (ok) {
        inputValues.Seek(0u);
        StandardParser parser(inputValues, cdb);
        ok = parser.Parse();
    }

    ConfigurationDatabase sigCdb;
    if (ok) {
        expectedLoggingSignalValues.Seek(0u);
        StandardParser parser(expectedLoggingSignalValues, sigCdb);
        ok = parser.Parse();
    }

    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");

        ok = gam.IsValid();

        // Copy inputValues to the GAM input signal memory
        if (ok) {

            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {

                StreamString signalName;
                ok = gam->GetSignalName(InputSignals, signalIdx, signalName);


                AnyType arrayDescription = cdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);

                //
                uint32 memoryAllocationSize = 0u;
                switch (arrayDescription.GetNumberOfDimensions()) {

                    case 0u:
                        memoryAllocationSize = arrayDescription.GetByteSize();
                        break;

                    case 1u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u);
                        break;

                    case 2u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u) * arrayDescription.GetNumberOfElements(1u);
                        break;
                }
                if (ok) {
                    ok = MemoryOperationsHelper::Copy(gam->GetInputSignalMemoryTest(signalIdx), arrayDescription.GetDataPointer(), memoryAllocationSize);
                }
            }

            ok = gam->Execute();
        }

        if(ok) {
            SimulinkRootInterface* inputs = gam->GetInputs();
            SimulinkInterface* in1 = inputs[0][0];
            SimulinkInterface* in2 = inputs[0][1];

            AnyType adIn1 = cdb.GetType(in1->fullPath.Buffer());
            AnyType adIn2 = cdb.GetType(in2->fullPath.Buffer());

            ok =    adIn1.GetDataPointer() != NULL_PTR(void *) &&
                    adIn2.GetDataPointer() != NULL_PTR(void *);

            if(ok) {
                ok =    (MemoryOperationsHelper::Compare(in1->destPtr, adIn1.GetDataPointer(), in1->byteSize) == 0u) &&
                        (MemoryOperationsHelper::Compare(in2->destPtr, adIn2.GetDataPointer(), in2->byteSize) == 0u);
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Wrong data pointers");
            }
        }

        if (ok) {
            SimulinkRootInterface* signals = gam->GetSignals();
            SimulinkInterface* sig1 = signals[0][0];
            SimulinkInterface* sig2 = signals[0][1];

            AnyType adOut1 = sigCdb.GetType(sig1->fullPath.Buffer());
            AnyType adOut2 = sigCdb.GetType(sig2->fullPath.Buffer());

            ok =    adOut1.GetDataPointer() != NULL_PTR(void *) &&
                    adOut2.GetDataPointer() != NULL_PTR(void *);
            if(ok) {
                ok =    (SafeMath::IsEqual<float64>( *( (float64*) sig2->destPtr), *( (float64*) adOut2.GetDataPointer() ) ) ) &&
                        (SafeMath::IsEqual<uint32> ( *( (uint32*)  sig1->destPtr), *( (uint32*)  adOut1.GetDataPointer() ) ) );
            }
        }
    }

    if (ok) {
        ord->Purge();
    }

    return ok && status;
}


bool SimulinkWrapperGAMTest::TestSetup_DisconnectedOutputSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { "
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        In2_ScalarUint32  = { "
        "            DataSource = Drv1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out20_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        Signal2  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, true);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestSetup_DisconnectedOutputStructuredSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { "
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        In2_ScalarUint32  = { "
        "            DataSource = Drv1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out20_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, true);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestSetup_DisconnectedInputSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasInputs', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble  = { "
        "        DataSource = Drv1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out1_ScalarDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, true);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestSetup_DisconnectedInputStructuredSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { "
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out20_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        Signal2  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, true);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestExecute_MultiMixedSignalsTranspose(bool transpose) {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall;

    if(transpose) {
        scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true, 'modelComplexity', 3, 'dataOrientation', 'Column-major');";
    }
    else {
        scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true, 'modelComplexity', 3, 'dataOrientation', 'Row-major');";
    }


    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_Structured = { "
        "        In1_ScalarDouble  = { "
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        In2_ScalarUint32  = { "
        "            DataSource = Drv1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "    In2_Structured = { "
        "        In3_VectorDouble = {"
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 8"
        "            NumberOfDimensions = 1"
        "        }"
        "        In4_VectorUint32  = {"
        "            DataSource = Drv1"
        "            Type = uint32"
        "            NumberOfElements = 8"
        "            NumberOfDimensions = 1"
        "        }"
        "    }"
        "    In3_Structured = { "
        "        In5_MatrixDouble = {"
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 36"
        "            NumberOfDimensions = 2"
        "        }"
        "        In6_MatrixUint32  = {"
        "            DataSource = Drv1"
        "            Type = uint32"
        "            NumberOfElements = 36"
        "            NumberOfDimensions = 2"
        "        }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out3_VectorDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 8"
        "        NumberOfDimensions = 1"
        "    }"
        "    Out4_VectorUint32 = {"
        "        DataSource = DDB1"
        "        Type = uint32"
        "        NumberOfElements = 8"
        "        NumberOfDimensions = 1"
        "    }"
        "    Out5_MatrixDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 36"
        "        NumberOfDimensions = 2"
        "    }"
        "    Out20_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        Signal2  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "    Out21_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 8"
        "            NumberOfDimensions = 1"
        "        }"
        "        Signal2  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 8"
        "            NumberOfDimensions = 1"
        "        }"
        "    }"
        "    Out31_NonVirtualBus = { "
        "        Signal1 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 36"
        "            NumberOfDimensions = 2"
        "        }"
        "        Signal2  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 36"
        "            NumberOfDimensions = 2"
        "        }"
        "    }"
        "}";

    StreamString inputValues = ""
            "In1_Structured.In2_ScalarUint32 = (uint32)  2 "
            "In1_Structured.In1_ScalarDouble = (float64) 3.141592653 "
            "In2_Structured.In4_VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
            "In2_Structured.In3_VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0} "
            "In3_Structured.In6_MatrixUint32 = (uint32)  { { 9, 11, 11, 11, 11, 12},"
            "                                               { 10, 11, 11, 11, 11, 12},"
            "                                               { 11, 11, 11, 11, 11, 12},"
            "                                               { 12, 11, 11, 11, 11, 12},"
            "                                               { 13, 11, 11, 11, 11, 12},"
            "                                               { 14, 11, 11, 11, 11, 12} }"
            "In3_Structured.In5_MatrixDouble = (float64) { { 1.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                              { 2.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                              { 3.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                              { 4.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                              { 5.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                              { 6.0, 11.0, 11.0, 11.0, 11.0, 12.0} }";

    StreamString checkValues = ""
            "In1_Structured.In2_ScalarUint32 = (uint32)  2 "
            "In1_Structured.In1_ScalarDouble = (float64) 3.141592653 "
            "In2_Structured.In4_VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
            "In2_Structured.In3_VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0} "
            "In3_Structured.In6_MatrixUint32 = (uint32)  { {  9, 10, 11, 12, 13, 14 },"
            "                                               { 11, 11, 11, 11, 11, 11 },"
            "                                               { 11, 11, 11, 11, 11, 11 },"
            "                                               { 11, 11, 11, 11, 11, 11 },"
            "                                               { 11, 11, 11, 11, 11, 11 },"
            "                                               { 12, 12, 12, 12, 12, 12 } }"
            "In3_Structured.In5_MatrixDouble = (float64) { { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0},"
            "                                              { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                              { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                              { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                              { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                              { 12.0, 12.0, 12.0, 12.0, 12.0, 12.0} }";


    StreamString parameters = "";

    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord, false, false);

    ConfigurationDatabase cdb;
    if (ok) {
        inputValues.Seek(0u);
        StandardParser parser(inputValues, cdb);
        ok = parser.Parse();
    }

    ConfigurationDatabase cdbIn;

    if(ok && transpose) {
        checkValues.Seek(0u);
        StandardParser parser(checkValues, cdbIn);
        ok = parser.Parse();
    }
    else {
        cdbIn = cdb;
    }


    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");

        ok = gam.IsValid();

        // Copy inputValues to the GAM input signal memory
        if (ok) {

            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {

                StreamString signalName;

                ok = gam->GetSignalName(InputSignals, signalIdx, signalName);

                AnyType arrayDescription = cdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);

                //
                uint32 memoryAllocationSize = 0u;

                switch (arrayDescription.GetNumberOfDimensions()) {

                    case 0u:
                        memoryAllocationSize = arrayDescription.GetByteSize();
                        break;

                    case 1u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u);
                        break;

                    case 2u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u) * arrayDescription.GetNumberOfElements(1u);
                        break;
                }
                if (ok) {
                    ok = MemoryOperationsHelper::Copy(gam->GetInputSignalMemoryTest(signalIdx), arrayDescription.GetDataPointer(), memoryAllocationSize);
                }
            }

            ok = gam->Execute();
        }

        if (ok) {
            SimulinkRootInterface* inputs = gam->GetInputs();

            SimulinkInterface* in1 = inputs[0][0];
            SimulinkInterface* in2 = inputs[0][1];
            SimulinkInterface* in3 = inputs[1][0];
            SimulinkInterface* in4 = inputs[1][1];
            SimulinkInterface* in5 = inputs[2][0];
            SimulinkInterface* in6 = inputs[2][1];

            AnyType adIn1 = cdbIn.GetType(in1->fullPath.Buffer());
            AnyType adIn2 = cdbIn.GetType(in2->fullPath.Buffer());
            AnyType adIn3 = cdbIn.GetType(in3->fullPath.Buffer());
            AnyType adIn4 = cdbIn.GetType(in4->fullPath.Buffer());
            AnyType adIn5 = cdbIn.GetType(in5->fullPath.Buffer());
            AnyType adIn6 = cdbIn.GetType(in6->fullPath.Buffer());

            ok =    adIn1.GetDataPointer() != NULL_PTR(void *) &&
                    adIn2.GetDataPointer() != NULL_PTR(void *) &&
                    adIn3.GetDataPointer() != NULL_PTR(void *) &&
                    adIn4.GetDataPointer() != NULL_PTR(void *) &&
                    adIn5.GetDataPointer() != NULL_PTR(void *) &&
                    adIn6.GetDataPointer() != NULL_PTR(void *);

            if(ok) {
                bool ok1, ok2, ok3, ok4, ok5, ok6;
                ok1 = (MemoryOperationsHelper::Compare(in1->destPtr, adIn1.GetDataPointer(), in1->byteSize) == 0u);
                ok2 = (MemoryOperationsHelper::Compare(in2->destPtr, adIn2.GetDataPointer(), in2->byteSize) == 0u);
                ok3 = (MemoryOperationsHelper::Compare(in3->destPtr, adIn3.GetDataPointer(), in3->byteSize) == 0u);
                ok4 = (MemoryOperationsHelper::Compare(in4->destPtr, adIn4.GetDataPointer(), in4->byteSize) == 0u);
                ok5 = (MemoryOperationsHelper::Compare(in5->destPtr, adIn5.GetDataPointer(), in5->byteSize) == 0u);
                ok6 = (MemoryOperationsHelper::Compare(in6->destPtr, adIn6.GetDataPointer(), in6->byteSize) == 0u);

                ok = ok1 && ok2 && ok3 && ok4 && ok5 && ok6;
            }

            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Input comparison failed");
            }

            // Check also outputs
            if (ok) {
                SimulinkRootInterface* outputs = gam->GetOutputs();

                SimulinkInterface* out1 = outputs[6][0];
                SimulinkInterface* out2 = outputs[6][1];
                SimulinkInterface* out3 = outputs[7][0];
                SimulinkInterface* out4 = outputs[7][1];
                SimulinkInterface* out5 = outputs[8][0];
                SimulinkInterface* out6 = outputs[8][1];

                AnyType adOut1 = cdb.GetType(in1->fullPath.Buffer());
                AnyType adOut2 = cdb.GetType(in2->fullPath.Buffer());
                AnyType adOut3 = cdb.GetType(in3->fullPath.Buffer());
                AnyType adOut4 = cdb.GetType(in4->fullPath.Buffer());
                AnyType adOut5 = cdb.GetType(in5->fullPath.Buffer());
                AnyType adOut6 = cdb.GetType(in6->fullPath.Buffer());

                ok =    adOut1.GetDataPointer() != NULL_PTR(void *) &&
                        adOut2.GetDataPointer() != NULL_PTR(void *) &&
                        adOut3.GetDataPointer() != NULL_PTR(void *) &&
                        adOut4.GetDataPointer() != NULL_PTR(void *) &&
                        adOut5.GetDataPointer() != NULL_PTR(void *) &&
                        adOut6.GetDataPointer() != NULL_PTR(void *);

                if(ok) {
                    bool ok1, ok2, ok3, ok4, ok5, ok6;
                    ok1 = SafeMath::IsEqual<uint32> ( *( (uint32*)  out1->destPtr), *( (uint32*)  adOut1.GetDataPointer() ) );
                    ok2 = SafeMath::IsEqual<float64>( *( (float64*) out2->destPtr), *( (float64*) adOut2.GetDataPointer() ) );
                    
                    ok3 = true;
                    for (uint32 idx = 0u; idx < out3->numElements; idx++) {
                        ok3 &= SafeMath::IsEqual<uint32> ( *( (uint32*)  out3->destPtr + idx), *( (uint32*)  adOut3.GetDataPointer() + idx ) );
                    }

                    ok4 = true;
                    for (uint32 idx = 0u; idx < out4->numElements; idx++) {
                        ok4 &= SafeMath::IsEqual<float64> ( *( (float64*)  out4->destPtr + idx), *( (float64*)  adOut4.GetDataPointer() + idx ) );
                    }
                    
                    ok5 = true;
                    for (uint32 idx = 0u; idx < out5->numElements; idx++) {
                        ok5 &= SafeMath::IsEqual<uint32> ( *( (uint32*)  out5->destPtr + idx), *( (uint32*)  adOut5.GetDataPointer() + idx ) );
                    }
                    
                    ok6 = true;
                    for (uint32 idx = 0u; idx < out6->numElements; idx++) {
                        ok6 &= SafeMath::IsEqual<float64> ( *( (float64*)  out6->destPtr + idx), *( (float64*)  adOut6.GetDataPointer() + idx ) );
                    }
                    ok = ok1 && ok2 && ok3 && ok4 && ok5 && ok6;

                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Output comparison failed");
                    }
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithNotFoundParameter_Failed(bool skipUnlinked) {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true');";

    StreamString skipUnlinkedParams = skipUnlinked?"0":"1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In3_VectorDouble = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "In4_VectorUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "In5_MatrixDouble = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "In6_MatrixUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out5_MatrixDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "Out6_MatrixUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 36"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = ""
        "matrixConstant = (float64) { {10, 10, 10}, {11, 11, 11}, {12, 12, 12} }"
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10 }"
        "structScalar.one         = (float64) 3.141592653 "
        "structScalar.pnested1.one = (float64) 2.718281828 "
        "structScalar.pnested1.two = (float64) 2.718281828 "
        "structScalar.pnested2.one = (float64) 1.414213562 "
        "structScalar.pnested2.two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed.one = (float64) 10 "
        "structMixed.vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "notFoundPar.mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status);

    return (ok != skipUnlinked) && (status.internalSetupError == skipUnlinked);
}


bool SimulinkWrapperGAMTest::TestSetup_WithNestedSingleSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'modelComplexity', 5, 'hasInputs', false);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out3_VectorDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "Out4_VectorUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 8"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, false);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParametersFromExternalSource_Unlinked() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructParams', true, 'hasTunableParams', true);";

    StreamString skipUnlinkedParams = "0";

    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble  = {"
        "    DataSource = Drv1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = Drv1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";


    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = ""
        "structScalar.pnested1.one = (float64) 1 "
        "structScalar.pnested1.two = (float64) 1 "
        "structScalar.pnested2.one = (float64) 1 ";

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
                  "2",
                  "ByteArray",
                  "0",
                  skipUnlinkedParams.Buffer(),
                  inputSignals.Buffer(),
                  outputSignals.Buffer(),
                  parameters.Buffer()
                 );

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

    // The GAM external source of parameters is expected to be a
    // ReferenceContainer populated by AnyObject
    if (ok) {

        // Create the ReferenceContainer
        ReferenceT<ReferenceContainer> cfgParameterContainer("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cfgParameterContainer->SetName("ExtSource");

        ok = cfgParameterContainer.IsValid();

        if (ok) {
            ok = ObjectRegistryDatabase::Instance()->Insert(cfgParameterContainer);
        }

        // Populate with AnyObjects
        float64 param1 = 1.0;
        AnyType anyParam(param1);

        ReferenceT<AnyObject> objParam1("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());

        anyParam.SetStaticDeclared(false);
        objParam1->Serialise(anyParam);
        objParam1->SetName("structScalar.pnested2.two");
        cfgParameterContainer->Insert("structScalar.pnested2.two", objParam1);

        ReferenceT<AnyObject> objParam2("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        objParam2->Serialise(anyParam);
        objParam2->SetName("structScalar.one");
        cfgParameterContainer->Insert("structScalar.one", objParam2);


        // Verify that non-AnyObject references are ignored
        ReferenceT<ReferenceContainer> refContainer("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        refContainer->SetName("structScalar.pnested2.one");
        cfgParameterContainer->Insert("structScalar.pnested2.one", refContainer);

    }

    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = god->Find("Test.Functions.GAM1");
        if (gam.IsValid()) {
            status = gam->GetStatus();
        }
    }

    god->Purge();

    return ok && status;
}

#ifdef ENUM_FEATURE

bool SimulinkWrapperGAMTest::TestSetup_WithOutputEnumSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasEnums', true, 'hasInputs', false);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = int32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, false);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithOutputEnumSignals_FailedWrongType() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasEnums', true, 'hasInputs', false);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = int16"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, false);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_WithEnumSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasEnums', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "In2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = int32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = int32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = "";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, false);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithEnumParameters() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasEnums', true, 'hasInputs', false, 'hasTunableParams', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble = {"
        "    DataSource = DDB1"
        "    Type = float64"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "Out2_ScalarUint32  = {"
        "    DataSource = DDB1"
        "    Type = int32"
        "    NumberOfElements = 1"
        "    NumberOfDimensions = 0"
        "}"
        "}";

    StreamString parameters = ""
        "EnumParam = (int32) 1";

    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, NULL, false, false);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestExecute_WithEnumSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasEnums', true);";

    StreamString skipUnlinkedParams = "1";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble  = { "
        "        DataSource = Drv1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    In2_ScalarUint32  = { "
        "        DataSource = Drv1"
        "        Type = int32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = {"
        "        DataSource = DDB1"
        "        Type = float64"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "    Out2_ScalarUint32 = {"
        "        DataSource = DDB1"
        "        Type = int32"
        "        NumberOfElements = 1"
        "        NumberOfDimensions = 0"
        "    }"
        "}";

    StreamString inputValues = ""
        "In1_ScalarDouble = (float64) 3.141592653 "
        "In2_ScalarUint32 = (int32)   0";
        
    StreamString expectedOutputValues = ""
        "Out1_ScalarDouble = (float64) 3.141592653 "
        "Out2_ScalarUint32 = (int32)   0";

    StreamString parameters = "";

    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    // Test setup
    StreamString verbosity = " 2 ";

    bool ok = TestSetupWithTemplate(scriptCall, verbosity, skipUnlinkedParams, inputSignals, outputSignals, parameters, status, ord, false, false);

    ConfigurationDatabase cdb;
    if (ok) {
        inputValues.Seek(0u);
        StandardParser parser(inputValues, cdb);
        ok = parser.Parse();
    }
    
    ConfigurationDatabase outCdb;
    if (ok) {
        expectedOutputValues.Seek(0u);
        StandardParser parser(expectedOutputValues, outCdb);
        ok = parser.Parse();
    }

    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");

        ok = gam.IsValid();

        // Copy inputValues to the GAM input signal memory
        if (ok) {

            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {

                StreamString signalName;
                ok = gam->GetSignalName(InputSignals, signalIdx, signalName);


                AnyType arrayDescription = cdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);

                //
                uint32 memoryAllocationSize = 0u;
                switch (arrayDescription.GetNumberOfDimensions()) {

                    case 0u:
                        memoryAllocationSize = arrayDescription.GetByteSize();
                        break;

                    case 1u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u);
                        break;

                    case 2u:
                        memoryAllocationSize = arrayDescription.GetByteSize() * arrayDescription.GetNumberOfElements(0u) * arrayDescription.GetNumberOfElements(1u);
                        break;
                }
                if (ok) {
                    ok = MemoryOperationsHelper::Copy(gam->GetInputSignalMemoryTest(signalIdx), arrayDescription.GetDataPointer(), memoryAllocationSize);
                }
            }

            ok = gam->Execute();
        }

        if (ok) {
            SimulinkRootInterface *inputs = gam->GetInputs();
            SimulinkRootInterface* in1 = &inputs[0];
            SimulinkRootInterface* in2 = &inputs[1];

            AnyType adIn1 = cdb.GetType(in1->fullPath.Buffer());
            AnyType adIn2 = cdb.GetType(in2->fullPath.Buffer());

            ok =    adIn1.GetDataPointer() != NULL_PTR(void *) &&
                    adIn2.GetDataPointer() != NULL_PTR(void *);

            if(ok) {
                ok =    (SafeMath::IsEqual<float64>( *( (float64*) in1->dataAddr), *( (float64*) adIn1.GetDataPointer() ) ) ) &&
                        (SafeMath::IsEqual<int32>  ( *( (int32*)   in2->dataAddr), *( (int32*)   adIn2.GetDataPointer() ) ) );
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Wrong data pointers");
            }
        }
        
        if (ok) {
            SimulinkRootInterface *outputs = gam->GetOutputs();
            SimulinkRootInterface* out1 = &outputs[0];
            SimulinkRootInterface* out2 = &outputs[1];

            AnyType adOut1 = outCdb.GetType(out1->fullPath.Buffer());
            AnyType adOut2 = outCdb.GetType(out2->fullPath.Buffer());

            ok =    adOut1.GetDataPointer() != NULL_PTR(void *) &&
                    adOut2.GetDataPointer() != NULL_PTR(void *);

            if(ok) {
                ok =    (SafeMath::IsEqual<float64>( *( (float64*) out1->dataAddr), *( (float64*) adOut1.GetDataPointer() ) ) ) &&
                        (SafeMath::IsEqual<int32>  ( *( (int32*)   out2->dataAddr), *( (int32*)   adOut2.GetDataPointer() ) ) );
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok && status;
}

#endif
