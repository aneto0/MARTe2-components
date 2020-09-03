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
 * @brief      Builds a model that can be used to test the SimulinkWrapperGAM from a script.
 * @param[out] modelName   name of the generated model.
 * @param[out] modelFolder folder in which the model was created.
 * @return     `true` if the model was succesfully built.
 */
static bool BuildTestModel(MARTe::StreamString& modelName, MARTe::StreamString& modelFolder) {
    
    using namespace MARTe;
    using namespace matlab::engine;
    
    bool ok = false;
    
    StreamString modelScriptFolder = getenv("MARTe2_Components_DIR");
    modelScriptFolder += "/Test/Components/GAMs/SimulinkWrapperGAM/";
    
    modelFolder = modelScriptFolder;
    
    StreamString addpathCommand = "addpath(\"";
    addpathCommand += modelScriptFolder.Buffer();
    addpathCommand += "\")";
    
    StreamString modelScriptPath = modelScriptFolder;
    modelScriptPath += "testModelScript.m";
    
    // Open and read the model source code
    File codeFile;
    ok = codeFile.Open(modelScriptPath.Buffer(), BasicFile::ACCESS_MODE_R);
    
    uint32 fileSize = (uint32) codeFile.Size();
    char8 codeBuffer[fileSize];
    
    printf("code size: %u\n", fileSize);
    
    codeFile.Seek(0u);
    ok = codeFile.Read(&codeBuffer[0], fileSize);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed to read input code.");
    }
    
printf("--- code ---\n\n%s\n", codeBuffer);
    if (false) { // TODO DEBUG substitute with ok
        
        // Start MATLAB engine synchronously
        std::unique_ptr<MATLABEngine> matlabPtr = startMATLAB();
        
        // Execute the code read from the file
        //matlabPtr->eval(convertUTF8StringToUTF16String(codeBuffer));
        matlabPtr->eval(convertUTF8StringToUTF16String(addpathCommand.Buffer()));
        matlabPtr->eval(u"global model_name model_compiled");
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
        matlab::data::TypedArray<bool> modelCompiled = matlabPtr->getVariable(u"model_compiled");
        
        ok = modelCompiled[0];
        if (ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Model succesfully compiled.");
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Model compilation failed.");
        }
        
        matlabPtr->feval<bool>(u"createTestModel", true,  false);
        matlabPtr->feval<bool>(u"createTestModel", false, true );
        
    }
    
    if (ok) {
        ok = codeFile.Close();
    }
    
    return ok;
    
}

/**
 * @brief Removes all files created by BuildTestModel. 
 */
static bool DeleteTestModel(MARTe::StreamString modelName, MARTe::StreamString modelFolder) {
    
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

/**
 * @brief Class that initialises a test environment for SimulinkWrapperGAM.
 */
class SimulinkGAMGTestEnvironment {
public:

    SimulinkGAMGTestEnvironment() {
        
        BuildTestModel(modelName, modelFolder);

        //modelName   = "test_model";
        //modelFolder = getenv("MARTe2_Components_DIR");
    }

    ~SimulinkGAMGTestEnvironment() {
        DeleteTestModel(modelName, modelFolder);
printf("DELETE ENDED SUCCESFULLY\n");
    }

    MARTe::StreamString modelName;
    MARTe::StreamString modelFolder;
};

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

bool SimulinkWrapperGAMTest::TestInitialise() {
    
    bool ok = true;
    
    StreamString modelName, modelFolder, modelFullPath;
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
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

bool SimulinkWrapperGAMTest::TestInitialiseWithConfiguration(ConfigurationDatabase configIn) {
    
    SimulinkWrapperGAM gam;
    bool ok = gam.Initialise(configIn);
    
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
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",                    modelFullPath);
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_Failed_MissingTunableParamExternalSource() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    MARTe::ConfigurationDatabase config;
    
    config.Write("Library",      modelFullPath);
    config.Write("SymbolPrefix", modelName);
    
    return !TestInitialiseWithConfiguration(config);
    
}

bool SimulinkWrapperGAMTest::TestInitialise_MissingOptionalConfigurationSettings() {
    
    MARTe::StreamString modelName, modelFolder, modelFullPath;
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
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
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
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
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
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
    
    modelName   = "test_model10";
    modelFolder = testEnvironment.modelFolder;
    
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
    
    modelName   = "test_model01";
    modelFolder = testEnvironment.modelFolder;
    
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
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
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

    StreamString configPart1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "       +Constants = {"
            "           Class = ConstantGAM"
            "            OutputSignals = {"
            "                RootIn1  = {"
            "                    DataSource = DDB1"
            "                    Type = float64"
            "                    NumberOfElements = 1"
            "                    NumberOfDimensions=1"
            "                    Default = {1}"
            "                }"
            "            }"
            "        }"
            "        +GAM1 = {"
            "            Class = SimulinkWrapperGAM"
            "            Library = \"";      // model full path will go here
            
    StreamString configPart2 = "\""
            "            SymbolPrefix = \""; // model name will go here
            
    StreamString configPart3 = "\""
            "            Verbosity = 2"
            "            TunableParamExternalSource = MDSParameters"
            "            SkipUnlinkedTunableParams = 1"
            "            InputSignals = {"
            "               RootIn1 = {"
            "                   DataSource = DDB1"
            "                   Type = float64"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               RootOut1 = {"
            "                   DataSource = DDB1"
            "                   Type = float64"
            "               }"
            "            }"
            "            Parameters = {"
            "               gain = 10"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
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
            "                    Functions = {Constants GAM1}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = false;
    
    StreamString modelName, modelFolder, modelFullPath;
    
    modelName   = testEnvironment.modelName;
    modelFolder = testEnvironment.modelFolder;
    
    modelFullPath  = modelFolder;
    modelFullPath += "/";
    modelFullPath += modelName;
    modelFullPath += ".so";
    
    // Insert model name and build folder in the configuration buffer
    StreamString config;
    
    config  = configPart1;
    config += modelFullPath;
    config += configPart2;
    config += modelName;
    config += configPart3;
    
    // Test setup
    ok = TestIntegratedInApplication(config.Buffer());
    
    return ok;
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
