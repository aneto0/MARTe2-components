/**
 * @file TriggeredIOGAMTest.cpp
 * @brief Source file for class TriggeredIOGAMTest
 * @date 25/03/2022
 * @author Andre Neto
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
 * the class TriggeredIOGAMTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "TriggeredIOGAM.h"
#include "TriggeredIOGAMTest.h"
#include "DataSourceI.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * Gives access to the TriggeredIOGAM memory for the Execute test
 */
class TriggeredIOGAMHelper: public MARTe::TriggeredIOGAM {
public:
    CLASS_REGISTER_DECLARATION()TriggeredIOGAMHelper() : MARTe::TriggeredIOGAM() {
    }

    virtual ~TriggeredIOGAMHelper() {

    }

    void *GetInputSignalsMemory() {
        return TriggeredIOGAM::GetInputSignalsMemory();
    }

    void *GetOutputSignalsMemory() {
        return TriggeredIOGAM::GetOutputSignalsMemory();
    }

    bool AllOutputBrokersEnabled() {
        using namespace MARTe;
        bool allEnabled = true;
        for(uint32 n=0u; (n<outputBrokers.Size()) && (allEnabled); n++) {
            ReferenceT<ExecutableI> execBroker = outputBrokers.Get(n); 
            allEnabled = execBroker->IsEnabled();
        }
        return allEnabled;
    }
};
CLASS_REGISTER(TriggeredIOGAMHelper, "1.0");

/**
 * A dummy DataSource which can be used to test different configuration interfaces
 *  to the TriggeredIOGAM
 */
class TriggeredIOGAMDataSourceHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

TriggeredIOGAMDataSourceHelper    () : MARTe::DataSourceI() {
    }

    virtual ~TriggeredIOGAMDataSourceHelper() {

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

CLASS_REGISTER(TriggeredIOGAMDataSourceHelper, "1.0");

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
    else {
        ok = application->PrepareNextState("State1");
    }
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool TriggeredIOGAMTest::TestConstructor() {
    using namespace MARTe;
    TriggeredIOGAM gam;
    return (gam.GetNumberOfInputSignals() == 0u) && (gam.GetNumberOfOutputSignals() == 0u);
}

bool TriggeredIOGAMTest::TestSetup() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = TriggeredIOGAM"
            "            InputSignals = {"
            "               Trigger = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "               }"
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
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_ReadTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
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
            "            Class = TriggeredIOGAMDataSourceHelper"
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

bool TriggeredIOGAMTest::TestSetup_NoTrigger() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = TriggeredIOGAM"
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
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_ReadTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
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
            "            Class = TriggeredIOGAMDataSourceHelper"
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
    bool ok = !TestIntegratedInApplication(config1);
    return ok;
}
bool TriggeredIOGAMTest::TestSetup_LessInputs() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = TriggeredIOGAM"
            "            InputSignals = {"
            "               Trigger = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "               }"
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
            "               GAM1_ReadTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
            "               }"
            "               GAM1_WriteTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint16"
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
            "        +Drv1 = {"
            "            Class = TriggeredIOGAMDataSourceHelper"
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

bool TriggeredIOGAMTest::TestSetup_DifferentType() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = TriggeredIOGAM"
            "            InputSignals = {"
            "               Trigger = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "               }"
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
            "               GAM1_ReadTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint64"
            "               }"
            "           }"
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
            "            Class = TriggeredIOGAMDataSourceHelper"
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

bool TriggeredIOGAMTest::TestSetup_False_DifferentSize() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = TriggeredIOGAM"
            "            InputSignals = {"
            "               Trigger = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "               }"
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
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   NumberOfElements = 11"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               GAM1_ReadTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_ExecTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
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
            "            Class = TriggeredIOGAMDataSourceHelper"
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

bool TriggeredIOGAMTest::TestExecute() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = TriggeredIOGAMHelper"
            "            InputSignals = {"
            "               Trigger = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "               }"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
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
            "            Class = TriggeredIOGAMDataSourceHelper"
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
    ReferenceT<TriggeredIOGAMHelper> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    uint8 *trigger = static_cast<uint8 *>(gam->GetInputSignalsMemory());
    uint32 *inMem = reinterpret_cast<uint32 *>(trigger + 1);
    uint32 *outMem = static_cast<uint32 *>(gam->GetOutputSignalsMemory());
    uint32 n;
    if (ok) {
        *trigger = 1;
        for (n = 0; n < 10; n++) {
            inMem[n] = (n * n + 1);
        }
        for (n = 0; n < 10; n++) {
            outMem[n] = 0;
        }
    }
    if (ok) {
        ok = gam->Execute();
    }
    if (ok) {
        ok = gam->AllOutputBrokersEnabled();
    }
    if (ok) {
        for (n = 0; (n < 10) && (ok); n++) {
            ok = (outMem[n] == (n * n + 1));
        }
    }
    if (ok) {
        *trigger = 0;
        for (n = 0; n < 10; n++) {
            inMem[n] = (n * n + 1);
        }
        for (n = 0; n < 10; n++) {
            outMem[n] = 0;
        }
    }
    if (ok) {
        ok = gam->Execute();
    }
    if (ok) {
        ok = !gam->AllOutputBrokersEnabled();
    }
    if (ok) {
        uint32 n;
        for (n = 0; (n < 10) && (ok); n++) {
            ok = (outMem[n] == 0);
        }
    }

    god->Purge();
    return ok;

}

bool TriggeredIOGAMTest::TestExecute_Samples() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = TriggeredIOGAMHelper"
            "            InputSignals = {"
            "               Trigger = {"
            "                   DataSource = Drv1"
            "                   Type = uint8"
            "               }"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   NumberOfElements = 1"
            "                   Samples = 10"
            "               }"
            "            }"
            "            OutputSignals = {"
            "               Signal1 = {"
            "                   DataSource = DDB1"
            "                   Type = float32"
            "                   NumberOfElements = 10"
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
            "            Class = TriggeredIOGAMDataSourceHelper"
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
    ReferenceT<TriggeredIOGAMHelper> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    uint8 *trigger = static_cast<uint8 *>(gam->GetInputSignalsMemory());
    uint32 *inMem = reinterpret_cast<uint32 *>(trigger + 1);
    uint32 *outMem = static_cast<uint32 *>(gam->GetOutputSignalsMemory());
    uint32 n;

    if (ok) {
        *trigger = 1;
        for (n = 0; n < 10; n++) {
            inMem[n] = (n * n + 1);

        }
        for (n = 0; n < 10; n++) {
            outMem[n] = 0;
        }
    }
    if (ok) {
        ok = gam->Execute();
    }
    if (ok) {
        for (n = 0; (n < 10) && (ok); n++) {
            ok = (outMem[n] == (n * n + 1));
        }
    }
    god->Purge();
    return ok;

}
