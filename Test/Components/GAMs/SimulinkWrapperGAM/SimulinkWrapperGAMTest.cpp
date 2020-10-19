/**
 * @file SimulinkWrapperGAMTest.cpp
 * @brief Source file for class SimulinkWrapperGAMTest
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
#include "SimulinkWrapperGAMTest.h"
#include "SimulinkWrapperGAM.h"
#include "MatlabDataArray.hpp"
#include "MatlabEngine.hpp"

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
    
    MARTe::uint16 GetNumOfPars() {
        return modelParameters.GetSize();
    }
    
    MARTe::SimulinkParameter* GetParameter(MARTe::uint32 index) {
        return modelParameters[index];
    }

    MARTe::SimulinkPort* GetPort(MARTe::uint32 index) {
        return modelPorts[index];
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
                                        bool destroy = true) {
    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    return TestIntegratedInApplication(config, god, destroy);
}

/**
 * @brief Class that initialises a test environment for SimulinkWrapperGAM.
 */
class SimulinkGAMGTestEnvironment {
public:

    SimulinkGAMGTestEnvironment() {
        
        // Start MATLAB engine synchronously
        matlabPtr = matlab::engine::startMATLAB();
        //matlabPtr = matlab::engine::connectMATLAB(u"MATLAB_1198");
        
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
        compiledModels.Add(currentModelName);
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

bool SimulinkWrapperGAMTest::TestInitialiseWithConfiguration(ConfigurationDatabase configIn) {
    
    SimulinkWrapperGAM gam;
    bool ok = gam.Initialise(configIn);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetupWithTemplate(StreamString scriptCall,
                                                   StreamString skipUnlinkedParams,
                                                   StreamString inputSignals,
                                                   StreamString outputSignals,
                                                   StreamString parameters,
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
                  structuredSignalsAsByteArrays?"1":"0",
                  enforceModelSignalCoverage?"1":"0",
                  skipUnlinkedParams.Buffer(),
                  inputSignals.Buffer(),
                  outputSignals.Buffer(),
                  parameters.Buffer()
                 );
    
    REPORT_ERROR_STATIC(ErrorManagement::Information, "%s", config);

    // Test setup
    bool ok = false;
    
    if (objRegDatabase == NULL) {
        ok = TestIntegratedInApplication(config.Buffer());
    }
    else {
        ok = TestIntegratedInApplication(config.Buffer(), objRegDatabase, false);
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
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
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
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
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
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
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
    modelName   = testEnvironment.CreateTestModel("createTestModel();");
    
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
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LoadSymbols() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
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
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LibraryMissingGetMmiFunction() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
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
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_LibraryMissingAllocFunction() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
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
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_MissingParametersLeaf() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
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
    config.Write("SkipUnlinkedTunableParams",  1);
    
    return TestInitialiseWithConfiguration(config);
    
}


bool SimulinkWrapperGAMTest::TestSetup() {
    
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_1() {
    
    StreamString scriptCall = "createTestModel('hasStructParams', true);";
    
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
        "structScalar-one = (float64) 1 "
        "structScalar-nested1-one = (float64) 1 "
        "structScalar-nested1-two = (float64) 1 "
        "structScalar-nested2-one = (float64) 1 "
        "structScalar-nested2-two = (float64) 1 ";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_2() {

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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_3() {

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
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint32) { {10, 10, 10, 10, 10, 10},"
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
                  "1",
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
        objParam1->SetName("structMixed-mat3d");
        cfgParameterContainer->Insert(objParam1);
        
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
        
    }
    
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    god->Purge();
    
    return ok;
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParametersFromExternalSource() {
    
    StreamString scriptCall = "createTestModel('hasStructParams', true);";
    
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
        "structScalar-nested1-one = (float64) 1 "
        "structScalar-nested1-two = (float64) 1 "
        "structScalar-nested2-one = (float64) 1 ";
        
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
                  "1",
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
        objParam1->SetName("structScalar-nested2-two");
        cfgParameterContainer->Insert(objParam1);
        
        ReferenceT<AnyObject> objParam2("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        objParam2->Serialise(anyParam);
        objParam2->SetName("structScalar-one");
        cfgParameterContainer->Insert(objParam2);
        
        // Verify that non-AnyObject references are ignored
        ReferenceT<ReferenceContainer> refContainer("ReferenceContainer", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        refContainer->SetName("structScalar-nested2-one");
        cfgParameterContainer->Insert(refContainer);
    }
    
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    god->Purge();
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_NoTunableParameters() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_WithStructSignals() {
    
    StreamString scriptCall = "createTestModel('hasStructSignals',     true);";
    
    StreamString skipUnlinkedParams = "1";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "In1_Structured = {"
        "    DataSource = Drv1"
        "    Type = uint8"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 1"
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_WithNestedStructSignals() {
    
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
        "    NumberOfElements = 24"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_SkipUnlinkedTunableParams() {

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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_DontSkipUnlinkedTunableParams() {
    
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
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfInputs() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfOutputs() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongInputName() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongOutputName() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongDatatype() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfElements() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfDimensions() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongNumberOfDimensions() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDimensions() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDimensions_Matrix() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDatatype() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_NoInputs() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}


bool SimulinkWrapperGAMTest::TestSetup_NoOutputs() {
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_StructArraysAsParams() {
    
    StreamString scriptCall = "createTestModel('hasTunableParams', true, 'hasStructArrayParams', true);";
    
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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_NestedStructArraysAsParams() {
    
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
        "structScalar-one = (float64) 1 "
        "structScalar-nested1-one = (float64) 1 "
        "structScalar-nested1-two = (float64) 1 "
        "structScalar-nested2-one = (float64) 1 "
        "structScalar-nested2-two = (float64) 1 ";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfDimensionsWithStructSignals() {
    
    StreamString scriptCall = "createTestModel('hasStructSignals',     true);";
    
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
        "Out20_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint8"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 2"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongDatatypeWithStructSignals() {
    
    StreamString scriptCall = "createTestModel('hasStructSignals',     true);";
    
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
        "Out20_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";
    
    // Test setup
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters);
    
    return !ok;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_RowMajorModel() {
    
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, ord);
    
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
            
            for (uint32 paramIdx = 0u; (paramIdx < gam->GetNumOfPars()) && ok ; paramIdx++) {
                
                SimulinkParameter* par = gam->GetParameter(paramIdx);
                
                StreamString paramName = par->fullName;
                uint32       paramSize = par->byteSize;
                void*        paramAddr = par->address;
                
                AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_ColumnMajorModel() {
    
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint32) { {10, 10, 10, 10, 10, 10},"
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint32) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, ord);
    
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
            
            for (uint32 paramIdx = 0u; (paramIdx < gam->GetNumOfPars()) && ok ; paramIdx++) {
                
                SimulinkParameter* par = gam->GetParameter(paramIdx);
                
                StreamString paramName = par->fullName;
                uint32       paramSize = par->byteSize;
                void*        paramAddr = par->address;
                
                AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_Uint() {
    
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (uint8) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint16)  { {10, 10, 10, 10, 10, 10},"
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (uint8) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint16) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, ord);
    
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
            
            for (uint32 paramIdx = 0u; (paramIdx < gam->GetNumOfPars()) && ok ; paramIdx++) {
                
                SimulinkParameter* par = gam->GetParameter(paramIdx);
                
                StreamString paramName = par->fullName;
                uint32       paramSize = par->byteSize;
                void*        paramAddr = par->address;
                
                AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_Int() {
    
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int8) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (int16)  { {10, 10, 10, 10, 10, 10},"
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int8) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (int16) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, ord);
    
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
            
            for (uint32 paramIdx = 0u; (paramIdx < gam->GetNumOfPars()) && ok ; paramIdx++) {
                
                SimulinkParameter* par = gam->GetParameter(paramIdx);
                
                StreamString paramName = par->fullName;
                uint32       paramSize = par->byteSize;
                void*        paramAddr = par->address;
                
                AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestParameterActualisation_Float() {
    
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (float32)  { {10, 10, 10, 10, 10, 10},"
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
        "structScalar-one         = (float64) 3.141592653 "
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (int32) { {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12},"
        "                              {10, 11, 11, 11, 11, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (float32) { {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12},"
        "                             {10, 11, 11, 11, 11, 12}}";
    
    // Test setup
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, ord);
    
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
            
            for (uint32 paramIdx = 0u; (paramIdx < gam->GetNumOfPars()) && ok ; paramIdx++) {
                
                SimulinkParameter* par = gam->GetParameter(paramIdx);
                
                StreamString paramName = par->fullName;
                uint32       paramSize = par->byteSize;
                void*        paramAddr = par->address;
                
                AnyType arrayDescription = cdb.GetType(paramName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(paramAddr, arrayDescription.GetDataPointer(), paramSize) == 0u);
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestExecute() {
    
    StreamString scriptCall = " createTestModel('modelComplexity', 4);";
    
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
        "structScalar-nested1-one = (float64) 2.718281828 "
        "structScalar-nested1-two = (float64) 2.718281828 "
        "structScalar-nested2-one = (float64) 1.414213562 "
        "structScalar-nested2-two = (float64) 1.414213562 "
        "vectorConstant2 = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "matrixConstant2 = (float64) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}"
        "structMixed-one = (float64) 10 "
        "structMixed-vec = (float64) { 0, 1, 2, 3, 4, 5, 6, 7 }"
        "structMixed-mat = (uint32) { {10, 10, 10, 10, 10, 10},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {11, 11, 11, 11, 11, 11},"
        "                              {12, 12, 12, 12, 12, 12}}";
    
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
    StreamString expectedValues = ""
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
    
    // Setup GAM
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();
    
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, ord);
    
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
        if (ok) {
            expectedValues.Seek(0u);
            StandardParser parser(expectedValues, cdb);
            ok = parser.Parse();
        }
        
        if (ok) {
                
            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {
                
                SimulinkPort* port = gam->GetPort(signalIdx);
                
                StreamString portName = port->fullName;
                uint32       portSize = port->byteSize;
                void*        portAddr = port->address;
                
                AnyType arrayDescription = cdb.GetType(portName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (ok) {
                    ok = (MemoryOperationsHelper::Compare(portAddr, arrayDescription.GetDataPointer(), portSize) == 0u);
                }
            }
        }
    }
    
    if (ok) {
        ord->Purge();
    }
    
    return ok;
}

bool SimulinkWrapperGAMTest::TestPrintAlgoInfo() {
    
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
    
    return ok;
    
}

bool SimulinkWrapperGAMTest::Test_StructuredSignals() {

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
    bool ok = TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, NULL, false, false);


    return ok;
}

bool SimulinkWrapperGAMTest::Test_StructuredSignals_Failed() {
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
    bool ok = !TestSetupWithTemplate(scriptCall, skipUnlinkedParams, inputSignals, outputSignals, parameters, NULL, false, true);

    return ok;
}
