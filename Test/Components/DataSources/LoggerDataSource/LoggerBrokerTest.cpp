/**
 * @file LoggerBrokerTest.cpp
 * @brief Source file for class LoggerBrokerTest
 * @date 12/11/2016
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
 * the class LoggerDataSourceBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ErrorManagement.h"
#include "GAM.h"
#include "LoggerBroker.h"
#include "LoggerBrokerTest.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * GAM to "generate" signals for the LoggerDataSource
 */
class LoggerBrokerTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

LoggerBrokerTestGAM    () : GAM() {
    }

    ~LoggerBrokerTestGAM() {

    }

    bool Execute() {
        using namespace MARTe;
        uint32 numberOfSignals = GetNumberOfOutputSignals();
        uint32 n;
        for(n=0u; n<numberOfSignals; n++) {
            uint32 *signalPtr = reinterpret_cast<uint32 *>(GetOutputSignalMemory(n));
            uint32 nOfRanges = 0;
            GetSignalNumberOfRanges(OutputSignals, n, nOfRanges);
            uint32 e;
            if(nOfRanges == 0u) {
                uint32 nOfElements;
                GetSignalNumberOfElements(OutputSignals, n, nOfElements);
                for(e=0; e<nOfElements; e++) {
                    signalPtr[e] = (e + 1);
                }
            }
            else {
                uint32 c = 0;
                for(e=0; e<nOfRanges; e++) {
                    uint32 rangeStart;
                    uint32 rangeEnd;
                    GetSignalRangesInfo(OutputSignals, n, e, rangeStart, rangeEnd);
                    uint32 r;
                    for(r=rangeStart; r<(rangeEnd + 1); r++) {
                        signalPtr[c] = (r + 1);
                        c++;
                    }
                }
            }
        }

        return true;
    }

    bool Setup() {
        return true;
    }

};
CLASS_REGISTER(LoggerBrokerTestGAM, "1.0")

/**
 * Very basic scheduler to execute a single cycle
 */
class LoggerBrokerTestScheduler: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

LoggerBrokerTestScheduler    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    void ExecuteThreadCycle(MARTe::uint32 threadId);

    virtual bool ConfigureScheduler(MARTe::Reference realTimeApp);

    virtual void CustomPrepareNextState();

private:

    MARTe::ScheduledState * const * scheduledStates;
};

LoggerBrokerTestScheduler::LoggerBrokerTestScheduler() :
        MARTe::GAMSchedulerI() {
    scheduledStates = NULL_PTR(MARTe::ScheduledState * const *);
}
MARTe::ErrorManagement::ErrorType LoggerBrokerTestScheduler::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

bool LoggerBrokerTestScheduler::ConfigureScheduler(MARTe::Reference realTimeApp) {
    bool ret = GAMSchedulerI::ConfigureScheduler(realTimeApp);
    if (ret) {
        scheduledStates = GetSchedulableStates();
    }
    return ret;
}

void LoggerBrokerTestScheduler::ExecuteThreadCycle(MARTe::uint32 threadId) {
    MARTe::ReferenceT<MARTe::RealTimeApplication> realTimeAppT = realTimeApp;

    ExecuteSingleCycle(scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].executables,
                       scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].numberOfExecutables);

}
MARTe::ErrorManagement::ErrorType LoggerBrokerTestScheduler::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void LoggerBrokerTestScheduler::CustomPrepareNextState() {

}

CLASS_REGISTER(LoggerBrokerTestScheduler, "1.0")

static MARTe::StreamString lastError;
void LoggerBrokerTestErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                                          const char * const errorDescription) {
    if (lastError.Size() > 0) {
        lastError += " ";
    }
    lastError += errorDescription;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool LoggerBrokerTest::TestConstructor() {
    using namespace MARTe;
    LoggerBroker broker;
    return true;
}

bool LoggerBrokerTest::TestInit() {
    using namespace MARTe;
    const MARTe::char8 * const config1 = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = LoggerBrokerTestGAM"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                }"
            "                Signal2 = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                    NumberOfElements = 8"
            "                    Ranges = {{1 1}}"
            "                }"
            "                Signal3 = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                    NumberOfElements = 6"
            "                    Ranges = {{1 2} {3 5}}"
            "                }"
            "                Signal4 = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                    NumberOfElements = 4"
            "                    NumberOfDimensions = 2"
            "                }"
            "                Signal5 = {"
            "                    DataSource = LoggerDS"
            "                    Type = uint32"
            "                    NumberOfElements = 2"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +LoggerDS = {"
            "            Class = LoggerDataSource"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = LoggerBrokerTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = config1;
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
    ReferenceT<LoggerBrokerTestScheduler> scheduler = application->Find("Scheduler");
    ok = scheduler.IsValid();
    if (ok) {
        scheduler->PrepareNextState("", "State1");

        application->StartNextStateExecution();

        lastError = "";
        ErrorManagement::ErrorProcessFunctionType currentErrorMessageProcessFunction = MARTe::ErrorManagement::errorMessageProcessFunction;

        SetErrorProcessFunction(&LoggerBrokerTestErrorProcessFunction);

        scheduler->ExecuteThreadCycle(0);

        SetErrorProcessFunction(currentErrorMessageProcessFunction);
    }

    REPORT_ERROR_STATIC(ErrorManagement::Information, lastError.Buffer());
    god->Purge();
    if (ok) {
        ok = (lastError == "Signal1 [0:0]:1 Signal2 [1:1]:{ 2 }  Signal3 [1:2]:{ 2 3 }  Signal3 [3:5]:{ 4 5 6 }  Signal4 [0:3]:{ { 1 2 3 4 } }  Signal5 [0:1]:{ 1 2 } ");
    }
    return ok;
}

bool LoggerBrokerTest::TestExecute() {
    return TestInit();
}
