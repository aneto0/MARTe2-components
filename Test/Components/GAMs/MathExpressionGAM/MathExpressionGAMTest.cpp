/**
 * @file MathExpressionGAMTest.cpp
 * @brief Source file for class MathExpressionGAMTest
 * @date 03/06/2020
 * @author Nicolo Ferron
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
 * the class IOGAMTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "GAMScheduler.h"
#include "MathExpressionGAM.h"
#include "MathExpressionGAMTest.h"
#include "MathExpressionParser.h"
#include "RuntimeEvaluator.h"
#include "MemoryOperationsHelper.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Gives access to the MathExpressionGAM memory for the TestMemory test
 */
class MathExpressionGAMHelper: public MARTe::MathExpressionGAM {
public:
    CLASS_REGISTER_DECLARATION()
    
    MathExpressionGAMHelper() : MARTe::MathExpressionGAM() {
    
    }

    ~MathExpressionGAMHelper() {

    }
    
    void* GetInputSignalMemory(uint32 index) {
        return MathExpressionGAM::GetInputSignalMemory(index);
    }
    
    void* GetOutputSignalMemory(uint32 index) {
        return MathExpressionGAM::GetOutputSignalMemory(index);
    }
    
    MARTe::RuntimeEvaluator* GetEvaluator() {
        return evaluator;
    } 
    
};
CLASS_REGISTER(MathExpressionGAMHelper, "1.0");

/**
 * A dummy DataSource which can be used to test different configuration interfaces
 *  to the MathExpressionGAM
 */
class MathExpressionGAMDataSourceHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MathExpressionGAMDataSourceHelper    () : MARTe::DataSourceI() {
    }

    virtual ~MathExpressionGAMDataSourceHelper() {

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

CLASS_REGISTER(MathExpressionGAMDataSourceHelper, "1.0");

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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool MathExpressionGAMTest::TestConstructor() {
    
    MathExpressionGAM gam;
    return (gam.GetNumberOfInputSignals() == 0u) && (gam.GetNumberOfOutputSignals() == 0u);
}

bool MathExpressionGAMTest::TestInitialise(const char8* expression) {
    
    MathExpressionGAM gam;
    ConfigurationDatabase config;

    bool ok = true;
    
    if ((expression != NULL) && (expression[0] != '\0')) {
        ok = config.Write("Expression", expression);
    }

    if (ok) {
        ok = gam.Initialise(config);
    }
    return ok;
}

bool MathExpressionGAMTest::TestSetup() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \"GAM1_TotalTime = GAM1_ReadTime + GAM1_WriteTime + GAM1_ExecTime;\""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
    bool ok = TestIntegratedInApplication(config1);
    return ok;
}

bool MathExpressionGAMTest::TestSetup_Failed_InputSignalMissingVariable() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \"GAM1_TotalTime = GAM1_WriteTime + GAM1_ExecTime;\""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
    bool ok = TestIntegratedInApplication(config1);
    return !ok;
}

bool MathExpressionGAMTest::TestSetup_Failed_OutputSignalMissingVariable() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \""
            "                           GAM1_TotalTime = GAM1_ReadTime + GAM1_WriteTime + GAM1_ExecTime;"
            "                         \""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
    bool ok = TestIntegratedInApplication(config1);
    return !ok;
}

bool MathExpressionGAMTest::TestSetup_Failed_VariableMissingInputSignal() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \"GAM1_TotalTime = GAM1_ReadTime + GAM1_WriteTime + GAM1_ExecTime;\""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
    bool ok = TestIntegratedInApplication(config1);
    return !ok;
}


bool MathExpressionGAMTest::TestSetup_Succeed_VariableMissingOutputSignal() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \""
            "                           GAM1_ReadWriteTime = GAM1_ReadTime + GAM1_WriteTime;"
            "                           GAM1_TotalTime = GAM1_ReadWriteTime + GAM1_ExecTime;"
            "                         \""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
    bool ok = TestIntegratedInApplication(config1);
    return ok;
}

bool MathExpressionGAMTest::TestSetup_Failed_NonScalarInput() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \""
            "                           GAM1_ReadWriteTime = GAM1_ReadTime + GAM1_WriteTime;"
            "                           GAM1_TotalTime = GAM1_ReadWriteTime + GAM1_ExecTime + GAM1_NonScalarInput;"
            "                         \""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_NonScalarInput = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
            "        +Drv1 = {"
            "            Class = IOGAMDataSourceHelper"
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
    bool ok = TestIntegratedInApplication(config1);
    return !ok;
}

bool MathExpressionGAMTest::TestSetup_Failed_NonScalarOutput() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \""
            "                           GAM1_ReadWriteTime = GAM1_ReadTime + GAM1_WriteTime;"
            "                           GAM1_TotalTime = GAM1_ReadWriteTime + GAM1_ExecTime;"
            "                           GAM1_NonScalarOutput = GAM1_ReadWriteTime + GAM1_ExecTime;"
            "                         \""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_NonScalarOutput = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2"
            "               }"
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
            "        +Drv1 = {"
            "            Class = MathExpressionGAMDataSourceHelper"
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
    bool ok = TestIntegratedInApplication(config1);
    return !ok;
}

bool MathExpressionGAMTest::TestMemory() {
    
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAMHelper"
            "            Expression = \"GAM1_TotalTime = GAM1_ReadTime + GAM1_WriteTime + GAM1_ExecTime;\""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
            
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<MathExpressionGAMHelper> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    RuntimeEvaluator* evalPtr = NULL_PTR(RuntimeEvaluator *);
    if (ok) {
        evalPtr = gam->GetEvaluator();
    }
    
    StreamString signalName;
    if (evalPtr != NULL_PTR(RuntimeEvaluator *)) {
        for (uint32 index = 0u; (index < gam->GetNumberOfInputSignals()) && ok; index++) {
            ok = gam->GetSignalName(InputSignals, index, signalName);
            if (ok) {
                ok = (gam->GetInputSignalMemory(index) == evalPtr->GetInputVariableMemory(signalName));
            }
            signalName = "";
        }
        for (uint32 index = 0u; (index < gam->GetNumberOfOutputSignals()) && ok; index++) {
            ok = gam->GetSignalName(OutputSignals, index, signalName);
            if (ok) {
                ok = (gam->GetOutputSignalMemory(index) == evalPtr->GetOutputVariableMemory(signalName));
            }
            signalName = "";
        }
    }
    god->Purge();
    return ok;
}

bool MathExpressionGAMTest::TestTypes() {
    
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAMHelper"
            "            Expression = \""
            "                           GAM1_ReadWriteTime = GAM1_ReadTime + GAM1_WriteTime;"
            "                           GAM1_TotalTime = GAM1_ReadWriteTime + GAM1_ExecTime;"
            "                           GAM1_A_out = GAM1_A;"
            "                           GAM1_B_out = GAM1_B;"
            "                           GAM1_C_out = GAM1_C;"
            "                         \""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_A = {"
            "                   DataSource = Drv1"
            "                   Type = float64"
            "                   NumberOfDimensions = 0"
            "                   NumberOfElements = 1"
            "               }"
            "               GAM1_B = {"
            "                   DataSource = Drv1"
            "                   Type = int16"
            "                   NumberOfDimensions = 0"
            "                   NumberOfElements = 1"
            "               }"
            "               GAM1_C = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   NumberOfDimensions = 0"
            "                   NumberOfElements = 1"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_A_out = {"
            "                   DataSource = Drv1"
            "                   Type = float64"
            "                   NumberOfDimensions = 0"
            "                   NumberOfElements = 1"
            "               }"
            "               GAM1_B_out = {"
            "                   DataSource = Drv1"
            "                   Type = int16"
            "                   NumberOfDimensions = 0"
            "                   NumberOfElements = 1"
            "               }"
            "               GAM1_C_out = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   NumberOfDimensions = 0"
            "                   NumberOfElements = 1"
            "               }"
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
            "        +Drv1 = {"
            "            Class = MathExpressionGAMDataSourceHelper"
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
            
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<MathExpressionGAMHelper> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    RuntimeEvaluator* evalPtr = NULL_PTR(RuntimeEvaluator *);
    if (ok) {
        evalPtr = gam->GetEvaluator();
    }
    
    StreamString signalName;
    VariableInformation* var;
    if (evalPtr != NULL_PTR(RuntimeEvaluator *)) {
        uint32 varIdx = 0u;
        while (evalPtr->BrowseInputVariable(varIdx, var) && ok) {
            for (uint32 index = 0u; (index < gam->GetNumberOfInputSignals()) && ok; index++) {
                ok = gam->GetSignalName(InputSignals, index, signalName);
                if ((signalName == var->name) && ok) {
                    ok = (gam->GetSignalType(InputSignals, index) == var->type);
                }
                signalName = "";
            }
            varIdx++;
        }
        
        varIdx = 0u;
        while (evalPtr->BrowseOutputVariable(varIdx, var) && ok) {
            for (uint32 index = 0u; (index < gam->GetNumberOfOutputSignals()) && ok; index++) {
                ok = gam->GetSignalName(OutputSignals, index, signalName);
                if ((signalName == var->name) && ok) {
                    ok = (gam->GetSignalType(OutputSignals, index) == var->type);
                }
                signalName = "";
            }
            varIdx++;
        }
    }
    
    god->Purge();
    
    return ok;
}

bool MathExpressionGAMTest::TestExecute_SingleExpression() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \"GAM1_TotalTime = GAM1_ReadTime + GAM1_WriteTime + GAM1_ExecTime;\""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
            
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<MathExpressionGAM> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    if (ok) {
        ok = gam->Execute();
    }
    god->Purge();
    return ok;
}

bool MathExpressionGAMTest::TestExecute_MultipleExpressions() {
    
    const char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = MathExpressionGAM"
            "            Expression = \""
            "                           GAM1_ReadWriteTime = GAM1_ReadTime + GAM1_WriteTime;"
            "                           GAM1_TotalTime = GAM1_ReadWriteTime + GAM1_ExecTime;"
            "                         \""
            "            InputSignals = {"
            "               GAM1_ReadTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime = {"
            "                   DataSource = Timings"
            "                   Type = uint32"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_TotalTime = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
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
            
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<MathExpressionGAM> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    if (ok) {
        ok = gam->Execute();
    }
    god->Purge();
    return ok;
}
