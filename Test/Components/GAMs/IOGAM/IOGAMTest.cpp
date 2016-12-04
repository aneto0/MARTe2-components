/**
 * @file IOGAMTest.cpp
 * @brief Source file for class IOGAMTest
 * @date 31/10/2016
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
#include "IOGAM.h"
#include "IOGAMTest.h"
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
 * Gives access to the IOGAM memory for the Execute test
 */
class IOGAMHelper: public MARTe::IOGAM {
public:
    CLASS_REGISTER_DECLARATION()IOGAMHelper() : MARTe::IOGAM() {
    }

    virtual ~IOGAMHelper() {

    }

    void *GetInputSignalsMemory() {
        return IOGAM::GetInputSignalsMemory();
    }

    void *GetOutputSignalsMemory() {
        return IOGAM::GetOutputSignalsMemory();
    }
};
CLASS_REGISTER(IOGAMHelper, "1.0");

/**
 * A dummy DataSource which can be used to test different configuration interfaces
 *  to the IOGAM
 */
class IOGAMDataSourceHelper: public MARTe::DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

IOGAMDataSourceHelper    () : MARTe::DataSourceI() {
    }

    virtual ~IOGAMDataSourceHelper() {

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

CLASS_REGISTER(IOGAMDataSourceHelper, "1.0");

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
bool IOGAMTest::TestConstructor() {
    using namespace MARTe;
    IOGAM gam;
    return (gam.GetNumberOfInputSignals() == 0u) && (gam.GetNumberOfOutputSignals() == 0u);
}

bool IOGAMTest::TestSetup() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAM"
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
    return ok;
}

bool IOGAMTest::TestSetup_False_LessInputs() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAM"
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

bool IOGAMTest::TestSetup_False_LessOutputs() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAM"
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
            "               GAM1_ReadTime_out = {"
            "                   DataSource = DDB1"
            "                   Type = uint32"
            "               }"
            "               GAM1_WriteTime_out = {"
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

bool IOGAMTest::TestSetup_False_DifferentType_Input() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAM"
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
            "                   Type = uint32"
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

bool IOGAMTest::TestSetup_False_DifferentType_Output() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAM"
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
            "                   Type = int32"
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

bool IOGAMTest::TestSetup_False_DifferentSize_Input() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAM"
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

bool IOGAMTest::TestSetup_False_DifferentSize_Output() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAM"
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
            "                   NumberOfElements = 11"
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

bool IOGAMTest::TestExecute() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAMHelper"
            "            InputSignals = {"
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
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<IOGAMHelper> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    if (ok) {
        uint32 *inMem = static_cast<uint32 *>(gam->GetInputSignalsMemory());
        uint32 *outMem = static_cast<uint32 *>(gam->GetOutputSignalsMemory());
        uint32 n;
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
        uint32 *outMem = static_cast<uint32 *>(gam->GetOutputSignalsMemory());
        uint32 n;
        for (n = 0; (n < 10) && (ok); n++) {
            ok = (outMem[n] == (n * n + 1));
        }
    }
    god->Purge();
    return ok;

}

bool IOGAMTest::TestExecute_Samples() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAM1 = {"
            "            Class = IOGAMHelper"
            "            InputSignals = {"
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
    bool ok = TestIntegratedInApplication(config1, false);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<IOGAMHelper> gam = god->Find("Test.Functions.GAM1");
    if (ok) {
        ok = gam.IsValid();
    }
    if (ok) {
        uint32 *inMem = static_cast<uint32 *>(gam->GetInputSignalsMemory());
        uint32 *outMem = static_cast<uint32 *>(gam->GetOutputSignalsMemory());
        uint32 n;
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
        uint32 *outMem = static_cast<uint32 *>(gam->GetOutputSignalsMemory());
        uint32 n;
        for (n = 0; (n < 10) && (ok); n++) {
            ok = (outMem[n] == (n * n + 1));
        }
    }
    god->Purge();
    return ok;

}
