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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "ErrorInformation.h"
#include "ErrorManagement.h"
#include "FastMath.h"
#include "GAM.h"
#include "GlobalObjectsDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "ConstantGAM.h"
#include "ConstantGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class ConstantGAMHelper: public MARTe::ConstantGAM {
public:
    CLASS_REGISTER_DECLARATION()

    ConstantGAMHelper() : MARTe::ConstantGAM() {};
    virtual ~ConstantGAMHelper() {};
    template <typename Type> bool GetOutput (MARTe::uint32 index, Type& value, MARTe::uint32 offset = 0u);
    MARTe::ErrorManagement::ErrorType SetOutput(MARTe::ReferenceContainer& message);
};

template<typename Type> bool ConstantGAMHelper::GetOutput(MARTe::uint32 signalIndex, Type& value, MARTe::uint32 index) {

    using namespace MARTe;

    bool ret = (GetNumberOfOutputSignals() > signalIndex);

    if (!ret) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "GetNumberOfOutputSignals() <= %u", signalIndex);
    }

    StreamString signalName;

    if (ret) {
        ret = GetSignalName(OutputSignals, signalIndex, signalName);
    }

    TypeDescriptor signalType = InvalidType;

    if (ret) {
        signalType = GetSignalType(OutputSignals, signalIndex);
        ret = (signalType != InvalidType);
    }

    /* Should test the signal type */

    if (ret) {
        Type* ref = static_cast<Type*>(GetOutputSignalMemory(signalIndex));
        value = ref[index];
        //ret = MemoryOperationsHelper::Copy(&value, static_cast<void*>(static_cast<Type*>(GetOutputSignalMemory(signalIndex)) + offset), sizeof(Type));
    }

    if (ret) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "ConstantGAMHelper::GetOutput - '%s' at offset '%u' '%!'", signalName.Buffer(), index, value);
    }

    return ret;
}

MARTe::ErrorManagement::ErrorType ConstantGAMHelper::SetOutput(MARTe::ReferenceContainer& message) {
    return ConstantGAM::SetOutput(message);
}

CLASS_REGISTER(ConstantGAMHelper, "1.0")
CLASS_METHOD_REGISTER(ConstantGAMHelper, SetOutput)

/**
 * Starts a MARTe application that uses this driver instance.
 */

namespace ConstantGAMTestHelper {

bool ConfigureApplication(const MARTe::char8 * const config) {

    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    StreamString err;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb, &err);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    bool ok = parser.Parse();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "StandardParser::Parse failed");
    }

    ReferenceT<RealTimeApplication> application;

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    }
    else {
        ok = application->ConfigureApplication();
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::ConfigureApplication failed");
    }

    return ok;

}

static inline bool StartApplication(const MARTe::char8 * const state = "Running") {

    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");

    bool ok = application.IsValid();

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    }
    else {
        ok = application->PrepareNextState(state);
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::PrepareNextState failed");
    }
    else {
        ok = application->StartNextStateExecution();
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::StartNextStateExecution failed");
    }

    return ok;

}

static inline bool StopApplication() {

    using namespace MARTe;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");

    bool ok = application.IsValid();

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::IsValid failed");
    }
    else {
        ok = application->StopCurrentStateExecution();
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "RealTimeApplication::StopCurrentStateExecution failed");
    }

    god->Purge();

    return ok;

}

} /* namespace ConstantGAMTestHelper */

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ConstantGAMTest::ConstantGAMTest() {
}

ConstantGAMTest::~ConstantGAMTest() {
}

bool ConstantGAMTest::TestConstructor() {
    using namespace MARTe;
    ConstantGAM gam;

    bool ok = (gam.GetNumberOfInputSignals() == 0u) && (gam.GetNumberOfOutputSignals() == 0u);

    return ok;
}

bool ConstantGAMTest::TestExecute() {
    using namespace MARTe;
    ConstantGAM gam;

    bool ok = gam.Execute();

    return ok;
}

bool ConstantGAMTest::TestSetup() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAM"
            "            OutputSignals = {"
            "                Constant_float32 = {"
            "                    DataSource = DDB"
            "                    Type = float32"
            "                    Default = 0.0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ReferenceT<ConstantGAM> gam = application->Find("Functions.Constants");

    if (ok) {
        ok = gam.IsValid();
    }

    return ok;
}

bool ConstantGAMTest::TestSetup_OutputSignal_Scalar() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = -100"
            "                }"
            "                Constant_2 = {"
            "                    DataSource = DDB"
            "                    Type = uint8"
            "                    Default = 100"
            "                }"
            "                Constant_3 = {"
            "                    DataSource = DDB"
            "                    Type = int16"
            "                    Default = -10000"
            "                }"
            "                Constant_4 = {"
            "                    DataSource = DDB"
            "                    Type = uint16"
            "                    Default = 10000"
            "                }"
            "                Constant_5 = {"
            "                    DataSource = DDB"
            "                    Type = int32"
            "                    Default = -1000000000"
            "                }"
            "                Constant_6 = {"
            "                    DataSource = DDB"
            "                    Type = uint32"
            "                    Default = 1000000000"
            "                }"
            "                Constant_7 = {"
            "                    DataSource = DDB"
            "                    Type = int64"
            "                    Default = -10000000000"
            "                }"
            "                Constant_8 = {"
            "                    DataSource = DDB"
            "                    Type = uint64"
            "                    Default = 10000000000"
            "                }"
            "                Constant_9 = {"
            "                    DataSource = DDB"
            "                    Type = float32"
            "                    Default = 0.001"
            "                }"
            "                Constant_10 = {"
            "                    DataSource = DDB"
            "                    Type = float64"
            "                    Default = 0.001"
            "                }"
            "                Constant_11 = {"
            "                    DataSource = DDB"
            "                    Type = string"
            "                    Default = \"undefined\""
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ReferenceT<ConstantGAMHelper> gam = application->Find("Functions.Constants");

    if (ok) {
        ok = gam.IsValid();
    }

    if (ok) {
        int8 value = 0;
        ok = (gam->GetOutput(0u, value) && (value == -100));
    }

    if (ok) {
        uint8 value = 0;
        ok = (gam->GetOutput(1u, value) && (value == 100));
    }

    if (ok) {
        int16 value = 0;
        ok = (gam->GetOutput(2u, value) && (value == -10000));
    }

    if (ok) {
        uint16 value = 0;
        ok = (gam->GetOutput(3u, value) && (value == 10000));
    }

    if (ok) {
        int32 value = 0;
        ok = (gam->GetOutput(4u, value) && (value == -1000000000));
    }

    if (ok) {
        uint32 value = 0u;
        ok = (gam->GetOutput(5u, value) && (value == 1000000000u));
    }

    if (ok) {
        int64 value = 0l;
        ok = (gam->GetOutput(6u, value) && (value == -10000000000l));
    }

    if (ok) {
        uint64 value = 0ul;
        ok = (gam->GetOutput(7u, value) && (value == 10000000000ul));
    }

    if (ok) {
        float32 value = 0.0;
        ok = (gam->GetOutput(8u, value) && ((value > 0.000999) && (value < 0.001001)));
    }

    if (ok) {
        float64 value = 0.0;
        ok = (gam->GetOutput(9u, value) && ((value > 0.000999) && (value < 0.001001)));
    }

    god->Purge();

    return ok;
}

bool ConstantGAMTest::TestSetup_OutputSignal_int8() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "                Constant_2 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = -10"
            "                }"
            "                Constant_3 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 127"
            "                }"
            "                Constant_4 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 4"
            "                    Default = {0 -10 127 -1}"
            "                }"
            "                Constant_5 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 2"
            "                    NumberOfElements = 8"
            "                    Default = {{0 -10 127 -1} {-1 127 -10 0}}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ReferenceT<ConstantGAMHelper> gam = application->Find("Functions.Constants");

    if (ok) {
        ok = gam.IsValid();
    }

    int8 value = 0;

    if (ok) {
        ok = (gam->GetOutput(0u, value) && (value == 0));
    }

    if (ok) {
        ok = (gam->GetOutput(1u, value) && (value == -10));
    }

    if (ok) {
        ok = (gam->GetOutput(2u, value) && (value == 127));
    }

    if (ok) {
        ok = (gam->GetOutput(3u, value, 0u) && (value == 0));
    }

    if (ok) {
        ok = (gam->GetOutput(3u, value, 1u) && (value == -10));
    }

    if (ok) {
        ok = (gam->GetOutput(3u, value, 2u) && (value == 127));
    }

    if (ok) {
        ok = (gam->GetOutput(3u, value, 3u) && (value == -1));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 0u) && (value == 0));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 1u) && (value == -10));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 2u) && (value == 127));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 3u) && (value == -1));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 4u) && (value == -1));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 5u) && (value == 127));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 6u) && (value == -10));
    }

    if (ok) {
        ok = (gam->GetOutput(4u, value, 7u) && (value == 0));
    }

    god->Purge();

    return ok;
}

bool ConstantGAMTest::TestSetup_Error_InputSignal() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAM"
            "            InputSignals = {"
            "                Constant_float32 = {"
            "                    DataSource = DDB"
            "                    Type = float32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Constant_float32 = {"
            "                    DataSource = DDB"
            "                    Type = float32"
            "                    Default = 0.0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    return !ok; // Expect failure 
}

bool ConstantGAMTest::TestSetup_Error_NoOutputSignal() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAM"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    return !ok; // Expect failure 
}

bool ConstantGAMTest::TestSetup_Error_NoDefault() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAM"
            "            OutputSignals = {"
            "                Constant_float32 = {"
            "                    DataSource = DDB"
            "                    Type = float32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    return !ok; // Expect failure
}

bool ConstantGAMTest::TestSetOutput_int8() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "                Constant_2 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = -10"
            "                }"
            "                Constant_3 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 127"
            "                }"
            "                Constant_4 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 4"
            "                    Default = {0 -10 127 -1}"
            "                }"
            "                Constant_5 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 2"
            "                    NumberOfElements = 8"
            "                    Default = {{0 -10 127 -1} {-1 127 -10 0}}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalIndex", 0);
    }            

    if (ok) {                
        ok = cdb.Write("SignalName", "Constant_1");
    }            

    if (ok) {                
        ok = cdb.Write("SignalValue", "-1");            
    }

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ReferenceT<ConstantGAMHelper> gam = application->Find("Functions.Constants");

    if (ok) {
        ok = gam.IsValid();
    }

    int8 value = 0;

    if (ok) {
        ok = (gam->GetOutput(0u, value) && (value == -1));
    }

    god->Purge();

    return ok;
}

bool ConstantGAMTest::TestSetOutput_1Darray() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "                Constant_2 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = -10"
            "                }"
            "                Constant_3 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 127"
            "                }"
            "                Constant_4 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 4"
            "                    Default = {0 -10 127 -1}"
            "                }"
            "                Constant_5 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 2"
            "                    NumberOfElements = 8"
            "                    Default = {{0 -10 127 -1} {-1 127 -10 0}}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalName", "Constant_4");
    }            

    if (ok) {
        /* WARNING - One can not really do this, rather pass an AnyType with the right content ... */                
        //ok = cdb.Write("Value", "{1 2 3 4}"); 
        int8 array [] = { 1, 2, 3, 4 };
	AnyType Value (array);      
        ok = cdb.Write("SignalValue", Value);            
    }

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ReferenceT<ConstantGAMHelper> gam = application->Find("Functions.Constants");

    if (ok) {
        ok = gam.IsValid();
    }

    int8 value = 0;

    if (ok) {
      ok = (gam->GetOutput(3u, value, 0) && (value == 1));
    }

    if (ok) {
      ok = (gam->GetOutput(3u, value, 1) && (value == 2));
    }

    if (ok) {
      ok = (gam->GetOutput(3u, value, 2) && (value == 3));
    }

    if (ok) {
      ok = (gam->GetOutput(3u, value, 3) && (value == 4));
    }

    god->Purge();

    return ok;
}

bool ConstantGAMTest::TestSetOutput_2Darray() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "                Constant_2 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = -10"
            "                }"
            "                Constant_3 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 127"
            "                }"
            "                Constant_4 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 4"
            "                    Default = {0 -10 127 -1}"
            "                }"
            "                Constant_5 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    NumberOfDimensions = 2"
            "                    NumberOfElements = 8"
            "                    Default = {{0 -10 127 -1} {-1 127 -10 0}}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalName", "Constant_5");
    }            

    if (ok) {
        int8 array [2][4] = {{ 1, 2, 3, 4 },
			     { 9, 8, 7, 6 }};
	AnyType Value (array);      
        ok = cdb.Write("SignalValue", Value);            
    }

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ReferenceT<ConstantGAMHelper> gam = application->Find("Functions.Constants");

    if (ok) {
        ok = gam.IsValid();
    }

    int8 value = 0;

    if (ok) {
      ok = (gam->GetOutput(4u, value, 0) && (value == 1));
    }

    if (ok) {
      ok = (gam->GetOutput(4u, value, 1) && (value == 2));
    }

    if (ok) {
      ok = (gam->GetOutput(4u, value, 2) && (value == 3));
    }

    if (ok) {
      ok = (gam->GetOutput(4u, value, 3) && (value == 4));
    }

    if (ok) {
      ok = (gam->GetOutput(4u, value, 4) && (value == 9));
    }

    if (ok) {
      ok = (gam->GetOutput(4u, value, 5) && (value == 8));
    }

    if (ok) {
      ok = (gam->GetOutput(4u, value, 6) && (value == 7));
    }

    if (ok) {
      ok = (gam->GetOutput(4u, value, 7) && (value == 6));
    }

    god->Purge();

    return ok;
}

bool ConstantGAMTest::TestSetOutput_WithIndex() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalIndex", 0);
    }            

    if (ok) {                
        ok = cdb.Write("SignalValue", "-1");            
    }

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Test");
    ReferenceT<ConstantGAMHelper> gam = application->Find("Functions.Constants");

    if (ok) {
        ok = gam.IsValid();
    }

    int8 value = 0;

    if (ok) {
        ok = (gam->GetOutput(0u, value) && (value == -1));
    }

    god->Purge();

    return ok;
}

bool ConstantGAMTest::TestSetOutput_Error_NoStructuredDataI() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "Message");            
    }            

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    return !ok; // Expect failure
}

bool ConstantGAMTest::TestSetOutput_Error_NoName() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalValue", "-1");
    }            

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    return !ok; // Expect failure
}

bool ConstantGAMTest::TestSetOutput_Error_InvalidName() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalIndex", 0);
    }            

    if (ok) {                
        ok = cdb.Write("SignalName", "Undefined");
    }            

    if (ok) {                
        ok = cdb.Write("SignalValue", "-1");
    }            

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    return !ok; // Expect failure
}

bool ConstantGAMTest::TestSetOutput_Error_NoValue() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalIndex", 0);
    }            

    if (ok) {                
        ok = cdb.Write("SignalName", "Constant_1");
    }            

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    return !ok; // Expect failure
}

bool ConstantGAMTest::TestSetOutput_Error_InvalidValue() {
    const MARTe::char8 * const config = ""
            "$Test = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +Constants = {"
            "            Class = ConstantGAMHelper"
            "            OutputSignals = {"
            "                Constant_1 = {"
            "                    DataSource = DDB"
            "                    Type = int8"
            "                    Default = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Running = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread = {"
            "                    Class = RealTimeThread"
            "                    Functions = {Constants}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ok = ConstantGAMTestHelper::ConfigureApplication(config);

    using namespace MARTe;
    
    ReferenceT<Message> message = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;

    if (ok) {
        ok = cdb.Write("Destination", "Test.Functions.Constants");
    }

    if (ok) {
        cdb.Write("Function", "SetOutput");
    }
    
    if (ok) {                
        ok = cdb.CreateAbsolute("+Parameters");            
    }

    if (ok) {                
        ok = cdb.Write("Class", "ConfigurationDatabase");            
    }            

    if (ok) {                
        ok = cdb.Write("SignalIndex", 0);
    }            

    if (ok) {                
        ok = cdb.Write("SignalName", "Constant_1");
    }            

    if (ok) {                
        ok = cdb.Write("SignalValue", "3.14");
    }            

    if (ok) {                
        ok = cdb.MoveToAncestor(1u);            
    }    

    if (ok) {
        ok = message->Initialise(cdb);
    }

    if (ok) {
        ErrorManagement::ErrorType status = MessageI::SendMessage(message, NULL);
	ok = (status == ErrorManagement::NoError);
    }

    return !ok; // Expect failure
}

