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

    MARTe::uint32 GetNumberOfParameters() {
        return modelNumOfParameters;
    }

    MARTe::uint32 GetNumberOfModelInputs() {
        return modelNumOfInputs;
    }


    MARTe::uint32 GetNumberOfModelOutputs() {
        return modelNumOfOutputs;
    }

    MARTe::uint32 GetNumberOfLoggingSignals() {
        return modelNumOfSignals;
    }
    
    MARTe::SimulinkNonVirtualBusMode GetNonVirtualBusMode() {
        return nonVirtualBusMode;
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
                                                   StreamString configOptions,
                                                   StreamString inputSignals,
                                                   StreamString outputSignals,
                                                   StreamString parameters,
                                                   ErrorManagement::ErrorType& status,
                                                   ObjectRegistryDatabase* objRegDatabase = NULL_PTR(ObjectRegistryDatabase*)
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
                  configOptions.Buffer(),
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

bool SimulinkWrapperGAMTest::TestExecuteGeneric(StreamString marteInputs,
                                                StreamString modelExpectedInputs,
                                                StreamString modelExpectedOutputs,
                                                StreamString marteExpectedOutputs,
                                                StreamString marteExpectedLoggingSignals,
                                                ErrorManagement::ErrorType& status,
                                                ObjectRegistryDatabase* objRegDatabase
                                                ) {

    bool ok = (objRegDatabase != NULL_PTR(ObjectRegistryDatabase*));

    ConfigurationDatabase marteInputCdb;
    if ( (marteInputs.Size() > 0u) && ok) {
        marteInputs.Seek(0u);
        StandardParser parser(marteInputs, marteInputCdb);
        ok = parser.Parse();
    }

    ConfigurationDatabase modelExpectedInputCdb;
    if ( (modelExpectedInputs.Size() > 0u) && ok) {
        modelExpectedInputs.Seek(0u);
        StandardParser parser(modelExpectedInputs, modelExpectedInputCdb);
        ok = parser.Parse();
    }

    ConfigurationDatabase modelExpectedOutputCdb;
    if ( (modelExpectedOutputs.Size() > 0u) && ok) {
        modelExpectedOutputs.Seek(0u);
        StandardParser parser(modelExpectedOutputs, modelExpectedOutputCdb);
        ok = parser.Parse();
    }

    ConfigurationDatabase marteExpectedOutputCdb;
    if ( (marteExpectedOutputs.Size() > 0u) && ok) {
        marteExpectedOutputs.Seek(0u);
        StandardParser parser(marteExpectedOutputs, marteExpectedOutputCdb);
        ok = parser.Parse();
    }

    ConfigurationDatabase marteExpectedLoggingCdb;
    if ( (marteExpectedLoggingSignals.Size() > 0u) && ok) {
        marteExpectedLoggingSignals.Seek(0u);
        StandardParser parser(marteExpectedLoggingSignals, marteExpectedLoggingCdb);
        ok = parser.Parse();
    }

    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = objRegDatabase->Find("Test.Functions.GAM1");

        ok = gam.IsValid();

        // Copy inputValues to the GAM input signal memory
        if (ok) {

            for (uint32 signalIdx = 0u; (signalIdx < gam->GetNumberOfInputSignals()) && ok ; signalIdx++) {

                StreamString signalName;
                ok = gam->GetSignalName(InputSignals, signalIdx, signalName);

                AnyType arrayDescription = marteInputCdb.GetType(signalName.Buffer());
                ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for providing input signal %s", signalName.Buffer());
                }

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
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed MemoryOperationsHelper::Copy() for signal %s.", signalName.Buffer());
                    }
                }
            }

            ok = gam->Execute();
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed Execute().");
            }
        }

        // Compare interfaces with expected values
        if (ok) {
            SimulinkRootInterface* inputs = gam->GetInputs();
            for (uint32 rootInputIdx = 0u; (rootInputIdx < gam->GetNumberOfModelInputs()) && ok; rootInputIdx++) {

                if (inputs[rootInputIdx].isStructured) {
                    for (uint32 subInputIdx = 0u; (subInputIdx < inputs[rootInputIdx].GetSize()) && ok; subInputIdx++) {
                        StreamString inputName = inputs[rootInputIdx][subInputIdx]->fullPath;
                        uint32       inputSize = inputs[rootInputIdx][subInputIdx]->byteSize;
                        void*        inputAddr = inputs[rootInputIdx][subInputIdx]->destPtr;

                        if (modelExpectedInputs.Size() > 0u) {
                            AnyType arrayDescription = modelExpectedInputCdb.GetType(inputName.Buffer());
                            ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s", inputName.Buffer());
                            }

                            if (ok) {
                                ok = (MemoryOperationsHelper::Compare(inputAddr, arrayDescription.GetDataPointer(), inputSize) == 0u);
                                if (!ok) {
                                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs model, comparison failed.", inputName.Buffer());
                                    if (inputName.Locate(StreamString("Double")) != -1) {
                                        for (uint32 byteIdx = 0u; byteIdx < inputSize/8u; byteIdx++) {
                                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: % 4.1f GAM: % 4.1f ", byteIdx, (uint8*)inputAddr + byteIdx, ((float64*)arrayDescription.GetDataPointer())[byteIdx], ((float64*)inputAddr)[byteIdx]);
                                        }
                                    }
                                    else {
                                        for (uint32 byteIdx = 0u; byteIdx < inputSize; byteIdx++) {
                                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: %3u GAM: %3u ", byteIdx, (uint8*)inputAddr + byteIdx, ((uint8*)arrayDescription.GetDataPointer())[byteIdx], ((uint8*)inputAddr)[byteIdx]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else { // non-structured
                    StreamString inputName = inputs[rootInputIdx].fullPath;
                    uint32       inputSize = inputs[rootInputIdx].byteSize;
                    void*        inputAddr = inputs[rootInputIdx].destPtr;

                    if (modelExpectedInputs.Size() > 0u) {
                        AnyType arrayDescription = modelExpectedInputCdb.GetType(inputName.Buffer());
                        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                        if (!ok) {
                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s", inputName.Buffer());
                        }

                        if (ok) {
                            ok = (MemoryOperationsHelper::Compare(inputAddr, arrayDescription.GetDataPointer(), inputSize) == 0u);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs model, comparison failed.", inputName.Buffer());
                                for (uint32 byteIdx = 0u; byteIdx < inputSize; byteIdx++) {
                                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "[%u - %p] ref: %3u GAM: %3u (%p)", byteIdx, (uint8*)inputAddr + byteIdx, ((uint8*)arrayDescription.GetDataPointer())[byteIdx], ((uint8*)inputAddr)[byteIdx]);
                                }
                            }
                        }
                    }
                }
            }

            SimulinkRootInterface* outputs = gam->GetOutputs();
            for (uint32 rootOutputIdx = 0u; (rootOutputIdx < gam->GetNumberOfModelOutputs()) && ok; rootOutputIdx++) {

                if (outputs[rootOutputIdx].isStructured) {
                    for (uint32 subOutputIdx = 0u; (subOutputIdx < outputs[rootOutputIdx].GetSize()) && ok; subOutputIdx++) {
                        StreamString outputName = outputs[rootOutputIdx][subOutputIdx]->fullPath;
                        uint32       outputSize = outputs[rootOutputIdx][subOutputIdx]->byteSize;
                        void*        modelAddr  = outputs[rootOutputIdx][subOutputIdx]->sourcePtr;
                        void*        GAMAddr    = outputs[rootOutputIdx][subOutputIdx]->destPtr;

                        // Check model output
                        if (modelExpectedOutputs.Size() > 0u) {
                            AnyType arrayDescription = modelExpectedOutputCdb.GetType(outputName.Buffer());
                            ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s", outputName.Buffer());
                            }

                            if (ok) {
                                ok = (MemoryOperationsHelper::Compare(modelAddr, arrayDescription.GetDataPointer(), outputSize) == 0u);
                                if (!ok) {
                                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs model, comparison failed.", outputName.Buffer());
                                    if (outputName.Locate(StreamString("Double")) != -1) {
                                        for (uint32 byteIdx = 0u; byteIdx < outputSize/8u; byteIdx++) {
                                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: % 4.1f model: % 4.1f ", byteIdx, (uint8*)modelAddr + byteIdx, ((float64*)arrayDescription.GetDataPointer())[byteIdx], ((float64*)modelAddr)[byteIdx]);
                                        }
                                    }
                                    else {
                                        for (uint32 byteIdx = 0u; byteIdx < outputSize; byteIdx++) {
                                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: %3u model: %3u ", byteIdx, (uint8*)modelAddr + byteIdx, ((uint8*)arrayDescription.GetDataPointer())[byteIdx], ((uint8*)modelAddr)[byteIdx]);
                                        }
                                    }
                                }
                            }
                        }

                        // Check GAM output
                        if ( (marteExpectedOutputs.Size() > 0u) && ok) {
                            AnyType arrayDescription = marteExpectedOutputCdb.GetType(outputName.Buffer());
                            ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s.", outputName.Buffer());
                            }

                            if (ok) {
                                ok = (MemoryOperationsHelper::Compare(GAMAddr, arrayDescription.GetDataPointer(), outputSize) == 0u);
                                if (!ok) {
                                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs GAM, comparison failed.", outputName.Buffer());
                                    if (outputName.Locate(StreamString("Double")) != -1) {
                                        for (uint32 byteIdx = 0u; byteIdx < outputSize/8u; byteIdx++) {
                                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: % 4.1f GAM: % 4.1f ", byteIdx, (uint8*)GAMAddr + byteIdx, ((float64*)arrayDescription.GetDataPointer())[byteIdx], ((float64*)GAMAddr)[byteIdx]);
                                        }
                                    }
                                    else {
                                        for (uint32 byteIdx = 0u; byteIdx < outputSize; byteIdx++) {
                                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: %3u GAM: %3u ", byteIdx, (uint8*)GAMAddr + byteIdx, ((uint8*)arrayDescription.GetDataPointer())[byteIdx], ((uint8*)GAMAddr)[byteIdx]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else { // non-structured

                    StreamString outputName = outputs[rootOutputIdx].fullPath;
                    uint32       outputSize = outputs[rootOutputIdx].byteSize;
                    void*        modelAddr  = outputs[rootOutputIdx].sourcePtr;
                    void*        GAMAddr    = outputs[rootOutputIdx].destPtr;

                    // Check model output
                    if (modelExpectedOutputs.Size() > 0u) {
                        AnyType arrayDescription = modelExpectedOutputCdb.GetType(outputName.Buffer());
                        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                        if (!ok) {
                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s", outputName.Buffer());
                        }

                        if (ok) {
                            ok = (MemoryOperationsHelper::Compare(modelAddr, arrayDescription.GetDataPointer(), outputSize) == 0u);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs model, comparison failed.", outputName.Buffer());
                                if (outputName.Locate(StreamString("Double")) != -1) {
                                    for (uint32 byteIdx = 0u; byteIdx < outputSize/8u; byteIdx++) {
                                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: % 4.1f model: % 4.1f ", byteIdx, (uint8*)modelAddr + byteIdx, ((float64*)arrayDescription.GetDataPointer())[byteIdx], ((float64*)modelAddr)[byteIdx]);
                                    }
                                }
                                else {
                                    for (uint32 byteIdx = 0u; byteIdx < outputSize; byteIdx++) {
                                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: %3u model: %3u ", byteIdx, (uint8*)modelAddr + byteIdx, ((uint8*)arrayDescription.GetDataPointer())[byteIdx], ((uint8*)modelAddr)[byteIdx]);
                                    }
                                }
                            }
                        }
                    }

                    // Check GAM output
                    if ((marteExpectedOutputs.Size() > 0u) && ok) {
                        AnyType arrayDescription = marteExpectedOutputCdb.GetType(outputName.Buffer());
                        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                        if (!ok) {
                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s", outputName.Buffer());
                        }

                        if (ok) {
                            ok = (MemoryOperationsHelper::Compare(GAMAddr, arrayDescription.GetDataPointer(), outputSize) == 0u);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs GAM, comparison failed.", outputName.Buffer());
                                if (outputName.Locate(StreamString("Double")) != -1) {
                                    for (uint32 byteIdx = 0u; byteIdx < outputSize/8u; byteIdx++) {
                                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: % 4.1f GAM: % 4.1f ", byteIdx, (uint8*)GAMAddr + byteIdx, ((float64*)arrayDescription.GetDataPointer())[byteIdx], ((float64*)GAMAddr)[byteIdx]);
                                    }
                                }
                                else {
                                    for (uint32 byteIdx = 0u; byteIdx < outputSize; byteIdx++) {
                                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "[% 2u - %p] ref: %3u GAM: %3u ", byteIdx, (uint8*)GAMAddr + byteIdx, ((uint8*)arrayDescription.GetDataPointer())[byteIdx], ((uint8*)GAMAddr)[byteIdx]);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            SimulinkRootInterface* signals = gam->GetSignals();
            for (uint32 logSignalIdx = 0u; (logSignalIdx < gam->GetNumberOfLoggingSignals()) && ok; logSignalIdx++) {

                if (signals[logSignalIdx].isStructured) {
                    for (uint32 subLogSignalIdx = 0u; (subLogSignalIdx < signals[logSignalIdx].GetSize()) && ok; subLogSignalIdx++) {
                        StreamString signalName = signals[logSignalIdx][subLogSignalIdx]->fullPath;
                        uint32       signalSize = signals[logSignalIdx][subLogSignalIdx]->byteSize;
                        void*        GAMAddr    = signals[logSignalIdx][subLogSignalIdx]->destPtr;

                        // Check GAM output
                        if ( (marteExpectedLoggingSignals.Size() > 0u) && ok) {
                            AnyType arrayDescription = marteExpectedLoggingCdb.GetType(signalName.Buffer());
                            ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s", signalName.Buffer());
                            }

                            if (ok) {
                                ok = (MemoryOperationsHelper::Compare(GAMAddr, arrayDescription.GetDataPointer(), signalSize) == 0u);
                                if (!ok) {
                                    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs GAM, comparison failed.", signalName.Buffer());
                                    for (uint32 byteIdx = 0u; byteIdx < signalSize; byteIdx++) {
                                        REPORT_ERROR_STATIC(ErrorManagement::Debug, "[%u - %p] ref: %3u GAM: %3u (%p)", byteIdx, (uint8*)GAMAddr + byteIdx, ((uint8*)arrayDescription.GetDataPointer())[byteIdx], ((uint8*)GAMAddr)[byteIdx]);
                                    }
                                }
                            }
                        }
                    }
                }
                else { // non-structured
                    StreamString signalName = signals[logSignalIdx].fullPath;
                    uint32       signalSize = signals[logSignalIdx].byteSize;
                    void*        GAMAddr    = signals[logSignalIdx].destPtr;

                    // Check GAM output
                    if ((marteExpectedLoggingSignals.Size() > 0u) && ok) {
                        AnyType arrayDescription = marteExpectedLoggingCdb.GetType(signalName.Buffer());
                        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
                        if (!ok) {
                            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Failed GetDataPointer() for signal %s", signalName.Buffer());
                        }

                        if (ok) {
                            ok = (MemoryOperationsHelper::Compare(GAMAddr, arrayDescription.GetDataPointer(), signalSize) == 0u);
                            if (!ok) {
                                REPORT_ERROR_STATIC(ErrorManagement::Debug, "Signal %s: reference vs GAM, comparison failed.", signalName.Buffer());
                            }
                        }
                    }
                }
            }
        }
    }

    if (ok) {
        objRegDatabase->Purge();
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
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_1() {
    //TODO Check if hasTunableParams has to be added

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructParams', true, 'hasTunableParams', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_2() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true', 'hasInputs', false);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParameters_3() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 4, 'hasTunableParams', true, 'hasStructParams', true', 'hasInputs', false);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
    StreamString inputSignals = "";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "Out1_ScalarDouble   = { DataSource = DDB1   Type = float64   NumberOfElements = 1   NumberOfDimensions = 0 } "
        "Out2_ScalarUint32   = { DataSource = DDB1   Type = uint32    NumberOfElements = 1   NumberOfDimensions = 0 } "
        "Out3_VectorDouble   = { DataSource = DDB1   Type = float64   NumberOfElements = 8   NumberOfDimensions = 1 } "
        "Out4_VectorUint32   = { DataSource = DDB1   Type = uint32    NumberOfElements = 8   NumberOfDimensions = 1 } "
        "Out5_MatrixDouble   = { DataSource = DDB1   Type = float64   NumberOfElements = 36  NumberOfDimensions = 2 } "
        "Out6_MatrixUint32   = { DataSource = DDB1   Type = uint32    NumberOfElements = 36  NumberOfDimensions = 2 } "
        "Out7_Matrix3DDouble = { DataSource = DDB1   Type = float64   NumberOfElements = 24  NumberOfDimensions = 3 } "
        "Out8_Matrix3DUint32 = { DataSource = DDB1   Type = uint32    NumberOfElements = 24  NumberOfDimensions = 3 } "
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
                  configOptions.Buffer(),
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
        uint32 param1[4u][2u][3u] = { { {1u,1u,1u}, {1u,1u,1u} },
                                      { {1u,1u,1u}, {1u,1u,1u} },
                                      { {1u,1u,1u}, {1u,1u,1u} },
                                      { {1u,1u,1u}, {1u,1u,1u} }
                                    };
                               
        TypeDescriptor type1 = TypeDescriptor::GetTypeDescriptorFromTypeName("uint32");
        AnyType anyParam1(type1, 0u, param1);
        anyParam1.SetNumberOfDimensions(3u);
        anyParam1.SetNumberOfElements(0u, 3u);
        anyParam1.SetNumberOfElements(1u, 2u);
        anyParam1.SetNumberOfElements(2u, 4u);
        
        ReferenceT<AnyObject> objParam1("AnyObject", GlobalObjectsDatabase::Instance()->GetStandardHeap());
        objParam1->Serialise(anyParam1);
        objParam1->SetName("structMixed.mat3d");
        cfgParameterContainer->Insert("structMixed.mat3d", objParam1);

        Reference foo = cfgParameterContainer->Find("structMixed.mat3d");

        float64 param2[4u][2u][3u] = { { {1.0,1.0,1.0}, {1.0,1.0,1.0} },
                                       { {1.0,1.0,1.0}, {1.0,1.0,1.0} },
                                       { {1.0,1.0,1.0}, {1.0,1.0,1.0} },
                                       { {1.0,1.0,1.0}, {1.0,1.0,1.0} }
                                     };
                               
        TypeDescriptor type2 = TypeDescriptor::GetTypeDescriptorFromTypeName("float64");
        AnyType anyParam2(type2, 0u, param2);
        anyParam2.SetNumberOfDimensions(3u);
        anyParam2.SetNumberOfElements(0u, 3u);
        anyParam2.SetNumberOfElements(1u, 2u);
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
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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
                  configOptions.Buffer(),
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
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}


bool SimulinkWrapperGAMTest::TestSetup_WithStructSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    
    StreamString scriptCall = "createTestModel('hasStructSignals', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured  = { DataSource = Drv1    Type = uint8    NumberOfElements = 16    NumberOfDimensions = 1 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out12_NonVirtualBus = { DataSource = DDB1    Type = uint8      NumberOfElements = 16   NumberOfDimensions = 1 }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_WithNestedStructSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'modelComplexity', 2, 'hasInputs', false);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
    StreamString inputSignals = "";
    
    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1     NumberOfDimensions = 0 } "
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1     NumberOfDimensions = 0 } "
        "    Out3_VectorDouble   = { DataSource = Drv1    Type = float64    NumberOfElements = 8     NumberOfDimensions = 1 } "
        "    Out4_VectorUint32   = { DataSource = Drv1    Type = uint32     NumberOfElements = 8     NumberOfDimensions = 1 } "
        "    Out12_NonVirtualBus = { DataSource = DDB1    Type = uint8      NumberOfElements = 16    NumberOfDimensions = 1 } "
        "    Out34_NonVirtualBus = { DataSource = DDB1    Type = uint8      NumberOfElements = 96    NumberOfDimensions = 1 } "
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_WithLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasLoggingSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured  = { DataSource = Drv1   Type = uint8   NumberOfElements = 16   NumberOfDimensions = 1 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble    = { DataSource = DDB1    Type = float64    NumberOfElements = 1   NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32    = { DataSource = DDB1    Type = uint32     NumberOfElements = 1   NumberOfDimensions = 0 }"
        "    Out12_NonVirtualBus  = { DataSource = DDB1    Type = uint8      NumberOfElements = 16  NumberOfDimensions = 1 }"
        "    Log1_ScalarDouble    = { DataSource = DDB1    Type = float64    NumberOfElements = 1   NumberOfDimensions = 0 }"
        "    Log2_ScalarUint32    = { DataSource = DDB1    Type = uint32     NumberOfElements = 1   NumberOfDimensions = 0 }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_SkipInvalidTunableParams() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasTunableParams',     true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status.ErrorsCleared());
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_DontSkipUnlinkedTunableParams() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    StreamString scriptCall = "createTestModel('hasTunableParams',     true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfInputs() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    
    StreamString scriptCall = "createTestModel();";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfOutputs() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasLoggingSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_EmptyInterfaceName() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasSignalNames', false);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.illegalOperation);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongInputName() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongOutputName() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}


bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongLoggingSignalName() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasLoggingSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongDatatype() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfElements() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfDimensions() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel();";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_LoggingSignalWrongDatatype() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasLoggingSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongNumberOfDimensions() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDimensions() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDimensions_Matrix() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasInputs', false);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_ParamWrongDatatype() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_NoInputs() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasInputs', false);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return ok && status.ErrorsCleared();
}


bool SimulinkWrapperGAMTest::TestSetup_NoOutputs() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasOutputs', false);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return ok && status.ErrorsCleared();
}


bool SimulinkWrapperGAMTest::TestSetup_LowVerbosity() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel();";

    StreamString configOptions = ""
        "Verbosity = 1 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return ok && status.ErrorsCleared();
}


bool SimulinkWrapperGAMTest::TestSetup_ZeroVerbosity() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel();";StreamString configOptions = ""
        "Verbosity = 0 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

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
    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return ok && status.ErrorsCleared();
}

bool SimulinkWrapperGAMTest::TestSetup_StructSignalBytesize() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 4, 'hasStructSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "   In12_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 16   NumberOfDimensions = 1 }"
        "   In34_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 96   NumberOfDimensions = 1 }"
        "   In56_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 432  NumberOfDimensions = 1 }"
        "   In78_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 288  NumberOfDimensions = 1 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble     = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32     = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out3_VectorDouble     = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out4_VectorUint32     = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out5_MatrixDouble     = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out6_MatrixUint32     = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out7_Matrix3DDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Out8_Matrix3DUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Out12_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 16   NumberOfDimensions = 1 }"
        "    Out34_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 96   NumberOfDimensions = 1 }"
        "    Out56_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 432  NumberOfDimensions = 1 }"
        "    Out78_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 288  NumberOfDimensions = 1 }"
        "    Out3456_NonVirtualBus = { DataSource = DDB1    Type = uint8      NumberOfElements = 528  NumberOfDimensions = 1 }"
        "}"
        ;

    StreamString parameters = "";

    StreamString expectedBytesizes = ""
        "Out1_ScalarDouble = (uint32) 8       "
        "Out2_ScalarUint32 = (uint32) 4       "
        "Out3_VectorDouble = (uint32) 64      "
        "Out4_VectorUint32 = (uint32) 32      "
        "Out5_MatrixDouble = (uint32) 288     "
        "Out6_MatrixUint32 = (uint32) 144     "
        "Out7_Matrix3DDouble = (uint32) 192   "
        "Out8_Matrix3DUint32 = (uint32) 96    "
        "Out12_NonVirtualBus = (uint32) 12    "
        "Out34_NonVirtualBus = (uint32) 96    "
        "Out56_NonVirtualBus = (uint32) 432   "
        "Out78_NonVirtualBus = (uint32) 288   "
        "Out3456_NonVirtualBus = (uint32) 528 "
        "Vector_Structured = (uint32) 96       "
        "Matrix_Structured = (uint32) 432      "
        ""
        ;

    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);

    ConfigurationDatabase cdb;
    if (ok) {
        expectedBytesizes.Seek(0u);
        StandardParser parser(expectedBytesizes, cdb);
        ok = parser.Parse();
    }

    // Check bytesizes
    if (ok) {
        ReferenceT<SimulinkWrapperGAMHelper> gam = ord->Find("Test.Functions.GAM1");

        ok = gam.IsValid();
        if (ok) {
            SimulinkRootInterface* outputs = gam->GetOutputs();
            for (uint32 rootOutputIdx = 0u; (rootOutputIdx < gam->GetNumberOfModelOutputs()) && ok; rootOutputIdx++) {
                ConfigurationDatabase sizeDb = outputs[rootOutputIdx].rootStructure;
                uint32 currNumOfChildren = sizeDb.GetNumberOfChildren();

                for (uint32 parIdx = 0u; parIdx < currNumOfChildren; parIdx++) {

                    uint32 byteSize = 0u;
                    StreamString signalName = "";

                    // struct
                    if (sizeDb.MoveToChild(parIdx)) {
                        signalName = sizeDb.GetName();
                        byteSize = outputs[rootOutputIdx].GetInterfaceBytesize(sizeDb);
                        uint32 expectedBytesize = *((uint32*) cdb.GetType(signalName.Buffer()).GetDataPointer());

                        ok = (byteSize == expectedBytesize);
                        for (uint32 subIdx = 0u; (subIdx < sizeDb.GetNumberOfChildren()) && ok; subIdx++) {

                            if (sizeDb.MoveToChild(subIdx)) {
                                signalName = sizeDb.GetName();
                                byteSize = outputs[rootOutputIdx].GetInterfaceBytesize(sizeDb);
                                uint32 expectedBytesize;
                                ok = cdb.Read(signalName.Buffer(), expectedBytesize);

                                if (ok) {
                                    ok = (byteSize == expectedBytesize);
                                }

                                sizeDb.MoveToAncestor(1u);
                            }
                        }

                        sizeDb.MoveToAncestor(1u);
                    }
                    // numeric
                    else {
                        signalName = sizeDb.GetChildName(parIdx);
                        byteSize = outputs[rootOutputIdx][parIdx]->byteSize;
                        uint32 expectedBytesize = *((uint32*) cdb.GetType(signalName.Buffer()).GetDataPointer());

                        ok = (byteSize == expectedBytesize);
                    }

                }
            }
        }
    }

    if (ok) {
        ord->Purge();
    }

    return ok && status.ErrorsCleared();
}


bool SimulinkWrapperGAMTest::TestSetup_With3DSignals() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 4, 'hasStructSignals', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "   In12_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 16   NumberOfDimensions = 1 }"
        "   In34_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 96   NumberOfDimensions = 1 }"
        "   In56_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 432  NumberOfDimensions = 1 }"
        "   In78_Structured = { DataSource = Drv1    Type = uint8     NumberOfElements = 288  NumberOfDimensions = 1 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble     = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32     = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out3_VectorDouble     = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out4_VectorUint32     = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out5_MatrixDouble     = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out6_MatrixUint32     = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out7_Matrix3DDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Out8_Matrix3DUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Out12_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 16   NumberOfDimensions = 1 }"
        "    Out34_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 96   NumberOfDimensions = 1 }"
        "    Out56_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 432  NumberOfDimensions = 1 }"
        "    Out78_NonVirtualBus   = { DataSource = DDB1    Type = uint8      NumberOfElements = 288  NumberOfDimensions = 1 }"
        "    Out3456_NonVirtualBus = { DataSource = DDB1    Type = uint8      NumberOfElements = 528  NumberOfDimensions = 1 }"
        "}"
        ;
    
    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return ok && status.ErrorsCleared();
}

bool SimulinkWrapperGAMTest::TestSetup_StructArraysAsParams() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true, 'hasStructArrayParams', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status);
}

bool SimulinkWrapperGAMTest::TestSetup_NestedStructArraysAsParams() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasTunableParams', true, 'hasStructParams', true, 'hasStructArrayParams', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (ok) && (status);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongNumberOfDimensionsWithStructSignals() {
    
    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals',     true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = {  DataSource = Drv1 Type = uint8 NumberOfElements = 16 NumberOfDimensions = 1 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1     NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1     NumberOfDimensions = 0 }"
        "    Out12_NonVirtualBus = { DataSource = DDB1    Type = uint8      NumberOfElements = 16    NumberOfDimensions = 2 }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);
    
    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_Failed_WrongDatatypeWithStructSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals',     true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "In12_Structured = { "
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
        "Out12_NonVirtualBus  = {"
        "    DataSource = DDB1"
        "    Type = uint32"
        "    NumberOfElements = 16"
        "    NumberOfDimensions = 1"
        "}"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

#ifdef ROW_MAJOR_ND_FEATURE

bool SimulinkWrapperGAMTest::TestParameterActualisation_RowMajorModel() {
    
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true', 'hasInputs', false, 'dataOrientation', 'Row-major');";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    
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
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    
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
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    
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
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    
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
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    
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

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
        "        ScalarDouble  = { "
        "            DataSource = Drv1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        ScalarUint32  = { "
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
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        ScalarDouble  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);


    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithStructuredLoggingSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true, 'hasLoggingSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
        "        ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    Log12_NonVirtualBus = { "
        "        ScalarUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);


    return ok && status;
}

bool SimulinkWrapperGAMTest::TestExecute() {

    StreamString scriptCall = " createTestModel('modelComplexity', 4, 'hasTunableParams', true);";
    
    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
    
    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    In2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    In3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    In4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    In5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    In6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    In7_Matrix3DDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 } "
        "    In8_Matrix3DUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 } "
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 10   NumberOfDimensions = 1 } "
        "    Out3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    Out4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    Out5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    Out6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    Out7_Matrix3DDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 } "
        "    Out8_Matrix3DUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 } "
        "}";

    StreamString parameters = ""
        "vectorConstant = (uint32) { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 }";
    
    // 3D matrices must be input as a sequence of uint8 raw bytes since configuration syntax does not yet support more than 2D
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
        "In7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "In8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
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
        "In7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "In8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   9,   0,   0,   0,   10,   0,   0,   0,   11,   0,   0,   0,   12,   0,   0,   0,   13,   0,   0,   0,   14,   0,   0,   0,   15,   0,   0,   0,   16,   0,   0,   0,   17,   0,   0,   0,   18,   0,   0,   0,   19,   0,   0,   0,   20,   0,   0,   0,   21,   0,   0,   0,   22,   0,   0,   0,   23,   0,   0,   0,   24,   0,   0,   0 } "
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
        "Out7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        ok = TestExecuteGeneric(inputValues, expectedInputValues, "", expectedOutputValues, "", status, ord);
    }
    
    return ok && status;
}

bool SimulinkWrapperGAMTest::TestExecute_WithLoggingSignals() {

    StreamString scriptCall = " createTestModel('modelComplexity', 4, 'hasTunableParams', true, 'hasLoggingSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    In3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    In4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    In5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    In6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    In7_Matrix3DDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    In8_Matrix3DUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 10   NumberOfDimensions = 1 }"
        "    Out3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out7_Matrix3DDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Out8_Matrix3DUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Log1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 10   NumberOfDimensions = 1 }"
        "    Log3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Log4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Log5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Log6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Log7_Matrix3DDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Log8_Matrix3DUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
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
        "In7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "In8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
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
        "In7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "In8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   9,   0,   0,   0,   10,   0,   0,   0,   11,   0,   0,   0,   12,   0,   0,   0,   13,   0,   0,   0,   14,   0,   0,   0,   15,   0,   0,   0,   16,   0,   0,   0,   17,   0,   0,   0,   18,   0,   0,   0,   19,   0,   0,   0,   20,   0,   0,   0,   21,   0,   0,   0,   22,   0,   0,   0,   23,   0,   0,   0,   24,   0,   0,   0 } "
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
        "Out7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
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
        "Log7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Log8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
       ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        ok = TestExecuteGeneric(inputValues, expectedInputValues, "", expectedOutputValues, expectedSignalValues, status, ord);
    }

    return ok && status;
}


bool SimulinkWrapperGAMTest::TestExecute_WithStructuredSignals() {

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 1 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
        "        ScalarDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 1     NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 1     NumberOfDimensions = 0 }"
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "}";

    StreamString inputValues = ""
        "In12_Structured.ScalarDouble = (float64) 3.141592653 "
        "In12_Structured.ScalarUint32 = (uint32)  2";
        
    StreamString expectedOutputValues = ""
        "Out1_ScalarDouble = (float64) 3.141592653 "
        "Out2_ScalarUint32 = (uint32)  2 "
        "Out12_NonVirtualBus.ScalarDouble = (float64) 3.141592653 "
        "Out12_NonVirtualBus.ScalarUint32 = (uint32)  2";

    StreamString parameters = "";


    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        ok = TestExecuteGeneric(inputValues, "", "", expectedOutputValues, "", status, ord);
    }
    
    return ok && status;
}


bool SimulinkWrapperGAMTest::TestExecute_WithStructuredLoggingSignals() {

    StreamString scriptCall = "createTestModel('modelComplexity', 4, 'hasStructSignals', true, 'hasLoggingSignals', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
        "        ScalarDouble    = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarUint32    = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    In34_Structured = { "
        "        VectorDouble    = { DataSource = Drv1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "        VectorUint32    = { DataSource = Drv1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    }"
        "    In56_Structured = { "
        "        MatrixDouble    = { DataSource = Drv1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "        MatrixUint32    = { DataSource = Drv1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    }"
        "    In78_Structured = { "
        "        Matrix3DDouble  = { DataSource = Drv1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "        Matrix3DUint32  = { DataSource = Drv1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out7_Matrix3DDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Out8_Matrix3DUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Log1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Log3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Log4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Log5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Log6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Log7_Matrix3DDouble = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Log8_Matrix3DUint32 = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    Out34_NonVirtualBus = { "
        "        VectorUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "        VectorDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    }"
        "    Out56_NonVirtualBus = { "
        "        MatrixUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "        MatrixDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    }"
        "    Out78_NonVirtualBus = { "
        "        Matrix3DUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "        Matrix3DDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    }"
        "    Out3456_NonVirtualBus = { "
        "        Vector_Structured = { "
        "            VectorUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "            VectorDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "        } "
        "        Matrix_Structured = { "
        "            MatrixUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "            MatrixDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "        } "
        "    }"
        "    Log12_NonVirtualBus = { "
        "        ScalarUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    Log34_NonVirtualBus = { "
        "        VectorUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "        VectorDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    }"
        "    Log56_NonVirtualBus = { "
        "        MatrixUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "        MatrixDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    }"
        "    Log78_NonVirtualBus = { "
        "        Matrix3DUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 24   NumberOfDimensions = 3 }"
        "        Matrix3DDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 24   NumberOfDimensions = 3 }"
        "    }"
        "    Log3456_NonVirtualBus = { "
        "        Vector_Structured = { "
        "            VectorUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "            VectorDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "        } "
        "        Matrix_Structured = { "
        "            MatrixUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "            MatrixDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "        } "
        "    }"
        "}";

    // 3D matrices must be input as a sequence of uint8 raw bytes since configuration syntax does not yet support more than 2D
    StreamString inputValues = ""
        "In12_Structured.ScalarDouble = (float64) 3.141592653 "
        "In12_Structured.ScalarUint32 = (uint32)  2 "
        "In34_Structured.VectorDouble = (float64) { 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "In34_Structured.VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7 } "
        "In56_Structured.MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12} }"
        "In56_Structured.MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12} }"
        "In78_Structured.Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "In78_Structured.Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        ;

    StreamString expectedLoggingSignalValues = ""
        "Log1_ScalarDouble = (float64) 3.141592653 "
        "Log2_ScalarUint32 = (uint32)  2 "
        "Log3_VectorDouble = (float64) { 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Log4_VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7 } "
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
        "Log7_Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Log8_Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        "Log12_NonVirtualBus.ScalarDouble = (float64) 3.141592653 "
        "Log12_NonVirtualBus.ScalarUint32 = (uint32)  2 "
        "Log34_NonVirtualBus.VectorDouble = (float64) { 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Log34_NonVirtualBus.VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7 } "
        "Log56_NonVirtualBus.MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12} }"
        "Log56_NonVirtualBus.MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12},"
        "                                               { 10, 11, 11, 11, 11, 12} }"
        "Log78_NonVirtualBus.Matrix3DDouble = (uint8) {  0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Log78_NonVirtualBus.Matrix3DUint32 = (uint8) {  1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        "Log3456_NonVirtualBus.Vector_Structured.VectorDouble = (float64) { 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Log3456_NonVirtualBus.Vector_Structured.VectorUint32 = (uint32)  { 0, 1, 2, 3, 4, 5, 6, 7 } "
        "Log3456_NonVirtualBus.Matrix_Structured.MatrixDouble = (float64) { { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12} }"
        "Log3456_NonVirtualBus.Matrix_Structured.MatrixUint32 = (uint32)  { { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12} } "
        ;

    StreamString parameters = "";

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        ok = TestExecuteGeneric(inputValues, "", "", "", expectedLoggingSignalValues, status, ord);
    }

    return ok && status;
}


bool SimulinkWrapperGAMTest::TestSetup_DisconnectedOutputSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
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
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        ScalarDouble  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestSetup_DisconnectedOutputStructuredSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 1 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
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
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestSetup_DisconnectedInputSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasInputs', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 1 "
        "NonVirtualBusMode = Structured ";

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestSetup_DisconnectedInputStructuredSignal_Failed() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 1 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
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
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32 = {"
        "            DataSource = DDB1"
        "            Type = uint32"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "        ScalarDouble  = {"
        "            DataSource = DDB1"
        "            Type = float64"
        "            NumberOfElements = 1"
        "            NumberOfDimensions = 0"
        "        }"
        "    }"
        "}";

    StreamString parameters = "";

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (!status);
}

bool SimulinkWrapperGAMTest::TestExecute_MultiMixedSignalsTranspose(bool transpose) {

    StreamString scriptCall;
    if(transpose) {
        scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true, 'modelComplexity', 3, 'dataOrientation', 'Column-major');";
    }
    else {
        scriptCall = "createTestModel('hasStructSignals', true, 'hasInputs', true, 'modelComplexity', 3, 'dataOrientation', 'Row-major');";
    }

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";

    StreamString inputSignals = ""
        "InputSignals = { "
        "    In12_Structured = { "
        "        ScalarDouble    = { DataSource = Drv1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarUint32    = { DataSource = Drv1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    In34_Structured = { "
        "        VectorDouble    = { DataSource = Drv1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "        VectorUint32    = { DataSource = Drv1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    }"
        "    In56_Structured = { "
        "        MatrixDouble    = { DataSource = Drv1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "        MatrixUint32    = { DataSource = Drv1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    }"
        "}";

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out2_ScalarUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    Out3_VectorDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out4_VectorUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    Out5_MatrixDouble   = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out6_MatrixUint32   = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    Out12_NonVirtualBus = { "
        "        ScalarUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 1    NumberOfDimensions = 0 }"
        "        ScalarDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 1    NumberOfDimensions = 0 }"
        "    }"
        "    Out34_NonVirtualBus = { "
        "        VectorUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 8    NumberOfDimensions = 1 }"
        "        VectorDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 8    NumberOfDimensions = 1 }"
        "    }"
        "    Out56_NonVirtualBus = { "
        "        MatrixUint32  = { DataSource = DDB1    Type = uint32     NumberOfElements = 36   NumberOfDimensions = 2 }"
        "        MatrixDouble  = { DataSource = DDB1    Type = float64    NumberOfElements = 36   NumberOfDimensions = 2 }"
        "    }"
        "}";

    StreamString inputValues = ""
            "In12_Structured.ScalarUint32 = (uint32)  2 "
            "In12_Structured.ScalarDouble = (float64) 3.141592653 "
            "In34_Structured.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
            "In34_Structured.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0} "
            "In56_Structured.MatrixUint32 = (uint32)  { { 9, 11, 11, 11, 11, 12},"
            "                                          { 10, 11, 11, 11, 11, 12},"
            "                                          { 11, 11, 11, 11, 11, 12},"
            "                                          { 12, 11, 11, 11, 11, 12},"
            "                                          { 13, 11, 11, 11, 11, 12},"
            "                                          { 14, 11, 11, 11, 11, 12} }"
            "In56_Structured.MatrixDouble = (float64) { { 1.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                          { 2.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                          { 3.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                          { 4.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                          { 5.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
            "                                          { 6.0, 11.0, 11.0, 11.0, 11.0, 12.0} }";

    StreamString checkValues = ""
            "In12_Structured.ScalarUint32 = (uint32)  2 "
            "In12_Structured.ScalarDouble = (float64) 3.141592653 "
            "In34_Structured.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
            "In34_Structured.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0} "
            "In56_Structured.MatrixUint32 = (uint32)  { {  9, 10, 11, 12, 13, 14 },"
            "                                          { 11, 11, 11, 11, 11, 11 },"
            "                                          { 11, 11, 11, 11, 11, 11 },"
            "                                          { 11, 11, 11, 11, 11, 11 },"
            "                                          { 11, 11, 11, 11, 11, 11 },"
            "                                          { 12, 12, 12, 12, 12, 12 } }"
            "In56_Structured.MatrixDouble = (float64) { { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0},"
            "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0},"
            "                                          { 12.0, 12.0, 12.0, 12.0, 12.0, 12.0} }";


    StreamString parameters = "";

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        if (transpose) {
            ok = TestExecuteGeneric(inputValues, checkValues, "", "", "", status, ord);
        }
        else {
            ok = TestExecuteGeneric(inputValues, inputValues, "", "", "", status, ord);
        }
    }

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithNotFoundParameter_Failed(bool skipUnlinked) {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('modelComplexity', 3, 'hasTunableParams', true, 'hasStructParams', true');";

    StreamString configOptions;
    configOptions.Printf(""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = %s "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ",
        skipUnlinked ? "1" : "0");

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (ok == skipUnlinked) && (status.internalSetupError != skipUnlinked);
}


bool SimulinkWrapperGAMTest::TestSetup_WithNestedSingleSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructSignals', true, 'modelComplexity', 2, 'hasInputs', false);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";
        ;

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, NULL);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_StructTunableParametersFromExternalSource_Unlinked() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasStructParams', true, 'hasTunableParams', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 0 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = ByteArray ";
        ;

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
                  configOptions.Buffer(),
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

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";
        ;

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithOutputEnumSignals_FailedWrongType() {

    ErrorManagement::ErrorType status = ErrorManagement::NoError;

    StreamString scriptCall = "createTestModel('hasEnums', true, 'hasInputs', false);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";
        ;

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return (!ok) && (status.internalSetupError);
}

bool SimulinkWrapperGAMTest::TestSetup_WithEnumSignals() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasEnums', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";
        ;

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestSetup_WithEnumParameters() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;

    StreamString scriptCall = "createTestModel('hasEnums', true, 'hasInputs', false, 'hasTunableParams', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";
        ;

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

    bool ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status);

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestExecute_WithEnumSignals() {

    StreamString scriptCall = "createTestModel('hasEnums', true);";

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 0 "
        "NonVirtualBusMode = Structured ";
        ;

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

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        ok = TestExecuteGeneric(inputValues, "", expectedOutputValues, expectedOutputValues, "", status, ord);
    }

    
    return ok && status;
}

#endif


bool SimulinkWrapperGAMTest::TestExecute_With3DSignals(bool transpose) {

    StreamString scriptCall;
    if (transpose) {
        scriptCall = "createTestModel('modelComplexity', 4, 'hasStructSignals', true, 'dataOrientation', 'Column-major');";
    }
    else {
        scriptCall = "createTestModel('modelComplexity', 4, 'hasStructSignals', true, 'dataOrientation', 'Row-major');";
    }

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 1 "
        "NonVirtualBusMode = Structured ";
        ;

    StreamString inputSignals = ""
        "InputSignals = { "
        "   In12_Structured = { DataSource = Drv1    Type = Scalar_Structured_t   NumberOfElements = 1    NumberOfDimensions = 0 } "
        "   In34_Structured = { DataSource = Drv1    Type = Vector_Structured_t   NumberOfElements = 1    NumberOfDimensions = 0 } "
        "   In56_Structured = { DataSource = Drv1    Type = Matrix_Structured_t   NumberOfElements = 1    NumberOfDimensions = 0 } "
        "   In78_Structured = { DataSource = Drv1    Type = Matrix3D_Structured_t NumberOfElements = 1    NumberOfDimensions = 0 } "
        "}"
        ;

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble     = { DataSource = DDB1    Type = float64                   NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out2_ScalarUint32     = { DataSource = DDB1    Type = uint32                    NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out3_VectorDouble     = { DataSource = DDB1    Type = float64                   NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    Out4_VectorUint32     = { DataSource = DDB1    Type = uint32                    NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    Out5_MatrixDouble     = { DataSource = DDB1    Type = float64                   NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    Out6_MatrixUint32     = { DataSource = DDB1    Type = uint32                    NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    Out7_Matrix3DDouble   = { DataSource = DDB1    Type = float64                   NumberOfElements = 24   NumberOfDimensions = 3 } "
        "    Out8_Matrix3DUint32   = { DataSource = DDB1    Type = uint32                    NumberOfElements = 24   NumberOfDimensions = 3 } "
        "    Out12_NonVirtualBus   = { DataSource = DDB1    Type = Scalar_Structured_t       NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out34_NonVirtualBus   = { DataSource = DDB1    Type = Vector_Structured_t       NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out56_NonVirtualBus   = { DataSource = DDB1    Type = Matrix_Structured_t       NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out78_NonVirtualBus   = { DataSource = DDB1    Type = Matrix3D_Structured_t     NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out3456_NonVirtualBus = { DataSource = DDB1    Type = VectorMatrix_Structured_t NumberOfElements = 1    NumberOfDimensions = 0 } "
        "}"
        ;

    StreamString inputValues = ""
        "In12_Structured.ScalarDouble = (float64) 3.141592653 "
        "In12_Structured.ScalarUint32 = (uint32)  2 "
        "In34_Structured.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0} "
        "In34_Structured.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "In56_Structured.MatrixDouble = (float64) { { 1.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                          { 2.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                          { 3.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                          { 4.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                          { 5.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                          { 6.0, 11.0, 11.0, 11.0, 11.0, 12.0} }"
        "In56_Structured.MatrixUint32 = (uint32)  { { 9, 11, 11, 11, 11, 12},"
        "                                          { 10, 11, 11, 11, 11, 12},"
        "                                          { 11, 11, 11, 11, 11, 12},"
        "                                          { 12, 11, 11, 11, 11, 12},"
        "                                          { 13, 11, 11, 11, 11, 12},"
        "                                          { 14, 11, 11, 11, 11, 12} }"
        " /* 3D matrices must be input as a sequence of uint8 raw byte since configuration syntax does not yet support more than 2D */ "
        "In78_Structured.Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        "In78_Structured.Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        ""
        ;

    StreamString expectedInputValuesColMajor = ""
        "In12_Structured.ScalarDouble = (float64) 3.141592653 "
        "In12_Structured.ScalarUint32 = (uint32)  2 "
        "In34_Structured.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0} "
        "In34_Structured.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "In56_Structured.MatrixDouble = (float64) { {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                          { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                          { 12.0, 12.0, 12.0, 12.0, 12.0, 12.0 } }"
        "In56_Structured.MatrixUint32 = (uint32)  { {  9, 10, 11, 12, 13, 14 },"
        "                                          { 11, 11, 11, 11, 11, 11 },"
        "                                          { 11, 11, 11, 11, 11, 11 },"
        "                                          { 11, 11, 11, 11, 11, 11 },"
        "                                          { 11, 11, 11, 11, 11, 11 },"
        "                                          { 12, 12, 12, 12, 12, 12 } }"
        " /* 3D matrices must be input as raw data since configuration syntax does not yet support more than 2D */ "
        "In78_Structured.Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   9,   0,   0,   0,   10,   0,   0,   0,   11,   0,   0,   0,   12,   0,   0,   0,   13,   0,   0,   0,   14,   0,   0,   0,   15,   0,   0,   0,   16,   0,   0,   0,   17,   0,   0,   0,   18,   0,   0,   0,   19,   0,   0,   0,   20,   0,   0,   0,   21,   0,   0,   0,   22,   0,   0,   0,   23,   0,   0,   0,   24,   0,   0,   0 } "
        "In78_Structured.Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        ""
        ;

    StreamString expectedOutputValuesRowMajor = ""
        "Out1_ScalarDouble     = (float64) 3.141592653 "
        "Out2_ScalarUint32     = (uint32)  2 "
        "Out3_VectorDouble     = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0 } "
        "Out4_VectorUint32     = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "Out5_MatrixDouble     = (float64) { { 1.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                    { 2.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                    { 3.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                    { 4.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                    { 5.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                    { 6.0, 11.0, 11.0, 11.0, 11.0, 12.0} } "
        "Out6_MatrixUint32     = (uint32)  { {  9, 11, 11, 11, 11, 12},"
        "                                    { 10, 11, 11, 11, 11, 12},"
        "                                    { 11, 11, 11, 11, 11, 12},"
        "                                    { 12, 11, 11, 11, 11, 12},"
        "                                    { 13, 11, 11, 11, 11, 12},"
        "                                    { 14, 11, 11, 11, 11, 12} } "
        "Out8_Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        "Out7_Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out12_NonVirtualBus.ScalarUint32 = (uint32)  2 "
        "Out12_NonVirtualBus.ScalarDouble = (float64) 3.141592653 "
        "Out34_NonVirtualBus.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "Out34_NonVirtualBus.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0 } "
        "Out56_NonVirtualBus.MatrixUint32 = (uint32) { {  9, 11, 11, 11, 11, 12},"
        "                                              { 10, 11, 11, 11, 11, 12},"
        "                                              { 11, 11, 11, 11, 11, 12},"
        "                                              { 12, 11, 11, 11, 11, 12},"
        "                                              { 13, 11, 11, 11, 11, 12},"
        "                                              { 14, 11, 11, 11, 11, 12} } "
        "Out56_NonVirtualBus.MatrixDouble = (float64) { { 1.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                               { 2.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                               { 3.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                               { 4.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                               { 5.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                               { 6.0, 11.0, 11.0, 11.0, 11.0, 12.0} } "
        " /* 3D matrices must be input as raw data since configuration syntax does not yet support more than 2D */ "
        "Out78_NonVirtualBus.Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        "Out78_NonVirtualBus.Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0 } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixUint32 = (uint32)  { {  9, 11, 11, 11, 11, 12},"
        "                                                                   { 10, 11, 11, 11, 11, 12},"
        "                                                                   { 11, 11, 11, 11, 11, 12},"
        "                                                                   { 12, 11, 11, 11, 11, 12},"
        "                                                                   { 13, 11, 11, 11, 11, 12},"
        "                                                                   { 14, 11, 11, 11, 11, 12} } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixDouble = (float64) { { 1.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                                                   { 2.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                                                   { 3.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                                                   { 4.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                                                   { 5.0, 11.0, 11.0, 11.0, 11.0, 12.0},"
        "                                                                   { 6.0, 11.0, 11.0, 11.0, 11.0, 12.0} } "
        ""
        ;

    StreamString expectedOutputValuesColMajor = ""
        "Out1_ScalarDouble     = (float64) 3.141592653 "
        "Out2_ScalarUint32     = (uint32)  2 "
        "Out3_VectorDouble     = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0 } "
        "Out4_VectorUint32     = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "Out5_MatrixDouble     = (float64) { {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                    { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                    { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                    { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                    { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                    { 12.0, 12.0, 12.0, 12.0, 12.0, 12.0 } } "
        "Out6_MatrixUint32     = (uint32)  { {  9, 10, 11, 12, 13, 14},"
        "                                    { 11, 11, 11, 11, 11, 11},"
        "                                    { 11, 11, 11, 11, 11, 11},"
        "                                    { 11, 11, 11, 11, 11, 11},"
        "                                    { 11, 11, 11, 11, 11, 11},"
        "                                    { 12, 12, 12, 12, 12, 12} } "
        "Out8_Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   9,   0,   0,   0,   10,   0,   0,   0,   11,   0,   0,   0,   12,   0,   0,   0,   13,   0,   0,   0,   14,   0,   0,   0,   15,   0,   0,   0,   16,   0,   0,   0,   17,   0,   0,   0,   18,   0,   0,   0,   19,   0,   0,   0,   20,   0,   0,   0,   21,   0,   0,   0,   22,   0,   0,   0,   23,   0,   0,   0,   24,   0,   0,   0 } "
        "Out7_Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out12_NonVirtualBus.ScalarUint32 = (uint32)  2 "
        "Out12_NonVirtualBus.ScalarDouble = (float64) 3.141592653 "
        "Out34_NonVirtualBus.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "Out34_NonVirtualBus.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0 } "
        "Out56_NonVirtualBus.MatrixUint32 = (uint32) { {  9, 10, 11, 12, 13, 14},"
        "                                              { 11, 11, 11, 11, 11, 11},"
        "                                              { 11, 11, 11, 11, 11, 11},"
        "                                              { 11, 11, 11, 11, 11, 11},"
        "                                              { 11, 11, 11, 11, 11, 11},"
        "                                              { 12, 12, 12, 12, 12, 12} } "
        "Out56_NonVirtualBus.MatrixDouble = (float64) { {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                               { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                               { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                               { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                               { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                               { 12.0, 12.0, 12.0, 12.0, 12.0, 12.0 } } "
        " /* 3D matrices must be input as raw data since configuration syntax does not yet support more than 2D */ "
        "Out78_NonVirtualBus.Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   9,   0,   0,   0,   10,   0,   0,   0,   11,   0,   0,   0,   12,   0,   0,   0,   13,   0,   0,   0,   14,   0,   0,   0,   15,   0,   0,   0,   16,   0,   0,   0,   17,   0,   0,   0,   18,   0,   0,   0,   19,   0,   0,   0,   20,   0,   0,   0,   21,   0,   0,   0,   22,   0,   0,   0,   23,   0,   0,   0,   24,   0,   0,   0 } "
        "Out78_NonVirtualBus.Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorUint32 = (uint32)  { 12, 1, 2, 3, 4, 5, 6, 7 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorDouble = (float64) { 2.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0 } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixUint32 = (uint32)  { {  9, 10, 11, 12, 13, 14},"
        "                                                                  { 11, 11, 11, 11, 11, 11},"
        "                                                                  { 11, 11, 11, 11, 11, 11},"
        "                                                                  { 11, 11, 11, 11, 11, 11},"
        "                                                                  { 11, 11, 11, 11, 11, 11},"
        "                                                                  { 12, 12, 12, 12, 12, 12} } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixDouble = (float64) { {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                                                   { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                                                   { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                                                   { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                                                   { 11.0, 11.0, 11.0, 11.0, 11.0, 11.0 },"
        "                                                                   { 12.0, 12.0, 12.0, 12.0, 12.0, 12.0 } } "
        ""
        ;

    StreamString parameters = "";

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        if (transpose) {
            ok = TestExecuteGeneric(inputValues, expectedInputValuesColMajor, expectedOutputValuesColMajor, expectedOutputValuesRowMajor, "", status, ord);
        }
        else {
            ok = TestExecuteGeneric(inputValues, inputValues, expectedOutputValuesRowMajor, expectedOutputValuesRowMajor, "", status, ord);
        }
    }

    return ok && status;
}

bool SimulinkWrapperGAMTest::TestExecute_With3DSignals_NoInputs(bool transpose) {

    StreamString scriptCall;
    if (transpose) {
        scriptCall = "createTestModel('modelComplexity', 4, 'hasStructSignals', true, 'hasInputs', false, 'dataOrientation', 'Column-major');";
    }
    else {
        scriptCall = "createTestModel('modelComplexity', 4, 'hasStructSignals', true, 'hasInputs', false, 'dataOrientation', 'Row-major');";
    }

    StreamString configOptions = ""
        "Verbosity = 2 "
        "SkipInvalidTunableParams = 1 "
        "EnforceModelSignalCoverage = 1 "
        "NonVirtualBusMode = Structured ";
        ;

    StreamString inputSignals = ""
        ;

    StreamString outputSignals = ""
        "OutputSignals = { "
        "    Out1_ScalarDouble     = { DataSource = DDB1    Type = float64                   NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out2_ScalarUint32     = { DataSource = DDB1    Type = uint32                    NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out3_VectorDouble     = { DataSource = DDB1    Type = float64                   NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    Out4_VectorUint32     = { DataSource = DDB1    Type = uint32                    NumberOfElements = 8    NumberOfDimensions = 1 } "
        "    Out5_MatrixDouble     = { DataSource = DDB1    Type = float64                   NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    Out6_MatrixUint32     = { DataSource = DDB1    Type = uint32                    NumberOfElements = 36   NumberOfDimensions = 2 } "
        "    Out7_Matrix3DDouble   = { DataSource = DDB1    Type = float64                   NumberOfElements = 24   NumberOfDimensions = 3 } "
        "    Out8_Matrix3DUint32   = { DataSource = DDB1    Type = uint32                    NumberOfElements = 24   NumberOfDimensions = 3 } "
        "    Out12_NonVirtualBus   = { DataSource = DDB1    Type = Scalar_Structured_t       NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out34_NonVirtualBus   = { DataSource = DDB1    Type = Vector_Structured_t       NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out56_NonVirtualBus   = { DataSource = DDB1    Type = Matrix_Structured_t       NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out78_NonVirtualBus   = { DataSource = DDB1    Type = Matrix3D_Structured_t     NumberOfElements = 1    NumberOfDimensions = 0 } "
        "    Out3456_NonVirtualBus = { DataSource = DDB1    Type = VectorMatrix_Structured_t NumberOfElements = 1    NumberOfDimensions = 0 } "
        "}"
        ;

        StreamString expectedOutputValuesRowMajor = ""
        "Out1_ScalarDouble     = (float64) 1 "
        "Out2_ScalarUint32     = (uint32)  1 "
        "Out3_VectorDouble     = (float64) { 7, 6, 5, 4, 3, 2, 1, 0 } "
        "Out4_VectorUint32     = (uint32)  { 1, 1, 1, 1, 1, 1, 1, 1 } "
        "Out5_MatrixDouble     = (float64) { { 1.0,  7.0, 13.0, 1.0,  7.0, 13.0 },"
        "                                    { 2.0,  8.0, 14.0, 2.0,  8.0, 14.0 },"
        "                                    { 3.0,  9.0, 15.0, 3.0,  9.0, 15.0 },"
        "                                    { 4.0, 10.0, 16.0, 4.0, 10.0, 16.0 },"
        "                                    { 5.0, 11.0, 17.0, 5.0, 11.0, 17.0 },"
        "                                    { 6.0, 12.0, 18.0, 6.0, 12.0, 18.0 } } "
        "Out6_MatrixUint32     = (uint32)  { { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 } } "
        "Out7_Matrix3DDouble   = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out8_Matrix3DUint32   = (uint8) { 1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        "Out12_NonVirtualBus.ScalarUint32 = (uint32)  1 "
        "Out12_NonVirtualBus.ScalarDouble = (float64) 1.000000000 "
        "Out34_NonVirtualBus.VectorUint32 = (uint32)  { 1, 1, 1, 1, 1, 1, 1, 1 } "
        "Out34_NonVirtualBus.VectorDouble = (float64) { 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Out56_NonVirtualBus.MatrixUint32 = (uint32) { { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 } } "
        "Out56_NonVirtualBus.MatrixDouble = (float64) { { 1.0,  7.0, 13.0, 1.0,  7.0, 13.0 },"
        "                                               { 2.0,  8.0, 14.0, 2.0,  8.0, 14.0 },"
        "                                               { 3.0,  9.0, 15.0, 3.0,  9.0, 15.0 },"
        "                                               { 4.0, 10.0, 16.0, 4.0, 10.0, 16.0 },"
        "                                               { 5.0, 11.0, 17.0, 5.0, 11.0, 17.0 },"
        "                                               { 6.0, 12.0, 18.0, 6.0, 12.0, 18.0 } } "
        " /* 3D matrices must be input as raw data since configuration syntax does not yet support more than 2D */ "
        "Out78_NonVirtualBus.Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   7,   0,   0,   0,   13,   0,   0,   0,   19,   0,   0,   0,   3,   0,   0,   0,   9,   0,   0,   0,   15,   0,   0,   0,   21,   0,   0,   0,   5,   0,   0,   0,   11,   0,   0,   0,   17,   0,   0,   0,   23,   0,   0,   0,   2,   0,   0,   0,   8,   0,   0,   0,   14,   0,   0,   0,   20,   0,   0,   0,   4,   0,   0,   0,   10,   0,   0,   0,   16,   0,   0,   0,   22,   0,   0,   0,   6,   0,   0,   0,   12,   0,   0,   0,   18,   0,   0,   0,   24,   0,   0,   0 } "
        "Out78_NonVirtualBus.Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorUint32 = (uint32)  { 1, 1, 1, 1, 1, 1, 1, 1 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorDouble = (float64) { 7, 6, 5, 4, 3, 2, 1, 0 } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixUint32 = (uint32)  { { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 } } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixDouble = (float64) { { 1.0,  7.0, 13.0, 1.0,  7.0, 13.0 },"
        "                                                                   { 2.0,  8.0, 14.0, 2.0,  8.0, 14.0 },"
        "                                                                   { 3.0,  9.0, 15.0, 3.0,  9.0, 15.0 },"
        "                                                                   { 4.0, 10.0, 16.0, 4.0, 10.0, 16.0 },"
        "                                                                   { 5.0, 11.0, 17.0, 5.0, 11.0, 17.0 },"
        "                                                                   { 6.0, 12.0, 18.0, 6.0, 12.0, 18.0 } } "
        ""
        ;

    StreamString expectedOutputValuesColMajor = ""
        "Out1_ScalarDouble     = (float64) 1 "
        "Out2_ScalarUint32     = (uint32)  1 "
        "Out3_VectorDouble     = (float64) { 7, 6, 5, 4, 3, 2, 1, 0 } "
        "Out4_VectorUint32     = (uint32)  { 1, 1, 1, 1, 1, 1, 1, 1 } "
        "Out5_MatrixDouble     = (float64) { {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                    {  7.0,  8.0,  9.0, 10.0, 11.0, 12.0 },"
        "                                    { 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 },"
        "                                    {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                    {  7.0,  8.0,  9.0, 10.0, 11.0, 12.0 },"
        "                                    { 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 } } "
        "Out6_MatrixUint32     = (uint32)  { { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 },"
        "                                    { 1, 1, 1, 1, 1, 1 } } "
        "Out7_Matrix3DDouble   = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out8_Matrix3DUint32   = (uint8) { 1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   9,   0,   0,   0,   10,   0,   0,   0,   11,   0,   0,   0,   12,   0,   0,   0,   13,   0,   0,   0,   14,   0,   0,   0,   15,   0,   0,   0,   16,   0,   0,   0,   17,   0,   0,   0,   18,   0,   0,   0,   19,   0,   0,   0,   20,   0,   0,   0,   21,   0,   0,   0,   22,   0,   0,   0,   23,   0,   0,   0,   24,   0,   0,   0 } "
        "Out12_NonVirtualBus.ScalarUint32 = (uint32)  1 "
        "Out12_NonVirtualBus.ScalarDouble = (float64) 1.000000000 "
        "Out34_NonVirtualBus.VectorUint32 = (uint32)  { 1, 1, 1, 1, 1, 1, 1, 1 } "
        "Out34_NonVirtualBus.VectorDouble = (float64) { 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0 } "
        "Out56_NonVirtualBus.MatrixUint32 = (uint32) { { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 },"
        "                                              { 1, 1, 1, 1, 1, 1 } } "
        "Out56_NonVirtualBus.MatrixDouble = (float64) { {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                               {  7.0,  8.0,  9.0, 10.0, 11.0, 12.0 },"
        "                                               { 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 },"
        "                                               {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                               {  7.0,  8.0,  9.0, 10.0, 11.0, 12.0 },"
        "                                               { 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 } } "
        " /* 3D matrices must be input as raw data since configuration syntax does not yet support more than 2D */ "
        "Out78_NonVirtualBus.Matrix3DUint32 = (uint8) { 1,   0,   0,   0,   2,   0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   6,   0,   0,   0,   7,   0,   0,   0,   8,   0,   0,   0,   9,   0,   0,   0,   10,   0,   0,   0,   11,   0,   0,   0,   12,   0,   0,   0,   13,   0,   0,   0,   14,   0,   0,   0,   15,   0,   0,   0,   16,   0,   0,   0,   17,   0,   0,   0,   18,   0,   0,   0,   19,   0,   0,   0,   20,   0,   0,   0,   21,   0,   0,   0,   22,   0,   0,   0,   23,   0,   0,   0,   24,   0,   0,   0 } "
        "Out78_NonVirtualBus.Matrix3DDouble = (uint8) { 0,   0,   0,   0,   0,   0, 240,  63,   0,   0,   0,   0,   0,   0,   0,  64,   0,   0,   0,   0,   0,   0,   8,  64,   0,   0,   0,   0,   0,   0,  16,  64,   0,   0,   0,   0,   0,   0,  20,  64,   0,   0,   0,   0,   0,   0,  24,  64,   0,   0,   0,   0,   0,   0,  28,  64,   0,   0,   0,   0,   0,   0,  32,  64,   0,   0,   0,   0,   0,   0,  34,  64,   0,   0,   0,   0,   0,   0,  36,  64,   0,   0,   0,   0,   0,   0,  38,  64,   0,   0,   0,   0,   0,   0,  40,  64,   0,   0,   0,   0,   0,   0,  42,  64,   0,   0,   0,   0,   0,   0,  44,  64,   0,   0,   0,   0,   0,   0,  46,  64,   0,   0,   0,   0,   0,   0,  48,  64,   0,   0,   0,   0,   0,   0,  49,  64,   0,   0,   0,   0,   0,   0,  50,  64,   0,   0,   0,   0,   0,   0,  51,  64,   0,   0,   0,   0,   0,   0,  52,  64,   0,   0,   0,   0,   0,   0,  53,  64,   0,   0,   0,   0,   0,   0,  54,  64,   0,   0,   0,   0,   0,   0,  55,  64,   0,   0,   0,   0,   0,   0,  56,  64 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorUint32 = (uint32)  { 1, 1, 1, 1, 1, 1, 1, 1 } "
        "Out3456_NonVirtualBus.Vector_Structured.VectorDouble = (float64) { 7, 6, 5, 4, 3, 2, 1, 0 } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixUint32 = (uint32)  { { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 },"
        "                                                                   { 1, 1, 1, 1, 1, 1 } } "
        "Out3456_NonVirtualBus.Matrix_Structured.MatrixDouble = (float64) { {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                                                   {  7.0,  8.0,  9.0, 10.0, 11.0, 12.0 },"
        "                                                                   { 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 },"
        "                                                                   {  1.0,  2.0,  3.0,  4.0,  5.0,  6.0 },"
        "                                                                   {  7.0,  8.0,  9.0, 10.0, 11.0, 12.0 },"
        "                                                                   { 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 } } "
        ""
        ;

    StreamString parameters = "";

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase* ord = ObjectRegistryDatabase::Instance();

    bool ok = (ord != NULL_PTR(ObjectRegistryDatabase*));

    if (ok) {
        ok = TestSetupWithTemplate(scriptCall, configOptions, inputSignals, outputSignals, parameters, status, ord);
    }

    if (ok) {
        if (transpose) {
            ok = TestExecuteGeneric("", "", expectedOutputValuesColMajor, expectedOutputValuesRowMajor, "", status, ord);
        }
        else {
            ok = TestExecuteGeneric("", "", expectedOutputValuesRowMajor, expectedOutputValuesRowMajor, "", status, ord);
        }
    }

    return ok && status;
}
