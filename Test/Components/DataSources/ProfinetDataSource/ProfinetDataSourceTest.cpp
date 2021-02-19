/**
 * @file ProfinetDataSourceTest.cpp
 * @brief Source file for the ProfinetDataSourceTest
 * @date 
 * @author
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

 * @details This header file contains the declaration of the class ProfinetDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "ErrorManagement.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "ProfinetDataSource.h"
#include "ProfinetDataSourceTest.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static const MARTe::char8 * const configurationFile = ""
"$TestApp = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"		+ConstantSignal = {"
"			Class = ConstantGAM"
"			OutputSignals = {"
"				InputBit = {"
"					Type = uint8"
"					DataSource = DDB1"
"					Default = 0xFF"
"				}"
"				InputSINT = {"
"					Type = int8"
"					DataSource = DDB1"
"					Default = 0xFF"
"				}"
"				InputUSINT = {"
"					Type = uint8"
"					DataSource = DDB1"
"					Default = 0xFF"
"				}"
"				InputINT = {"
"					Type = int16"
"					DataSource = DDB1"
"					Default = 0xFFFF"
"				}"
"				InputUINT = {"
"					Type = uint16"
"					DataSource = DDB1"
"					Default = 255"
"				}"
"				InputDINT = {"
"					Type = int32"
"					DataSource = DDB1"
"					Default = 0xFFFFFFFF"
"				}"
"				InputUDINT = {"
"					Type = int32"
"					DataSource = DDB1"
"					Default = 0xFFFFFFFF"
"				}"
"				InputLINT = {"
"					Type = int64"
"					DataSource = DDB1"
"					Default = 0xFFFFFFFFFFFFFFFF"
"				}"
"				InputULINT = {"
"					Type = uint64"
"					DataSource = DDB1"
"					Default = 0xFFFFFFFFFFFFFFFF"
"				}"
"				InputREAL = {"
"					Type = float32"
"					DataSource = DDB1"
"					Default = 123.45"
"				}"
"				InputLREAL = {"
"					Type = float64"
"					DataSource = DDB1"
"					Default = 123.45"
"				}"
"			}"
"		}"
"        +GAMTimer = {"
"            Class = IOGAM"
"            InputSignals = {"
"				OutputBit = {"
"					Type = uint8"
"					DataSource = DDB1"
"				}"
"				OutputSINT = {"
"					Type = int8"
"					DataSource = DDB1"
"				}"
"				OutputUSINT = {"
"					Type = uint8"
"					DataSource = DDB1"
"				}"
"				OutputINT = {"
"					Type = int16"
"					DataSource = DDB1"
"				}"
"				OutputUINT = {"
"					Type = uint16"
"					DataSource = DDB1"
"				}"
"				OutputDINT = {"
"					Type = int32"
"					DataSource = DDB1"
"				}"
"				OutputUDINT = {"
"					Type = int32"
"					DataSource = DDB1"
"				}"
"				OutputLINT = {"
"					Type = int64"
"					DataSource = DDB1"
"				}"
"				OutputULINT = {"
"					Type = uint64"
"					DataSource = DDB1"
"				}"
"				OutputREAL = {"
"					Type = float32"
"					DataSource = DDB1"
"				}"
"				OutputLREAL = {"
"					Type = float64"
"					DataSource = DDB1"
"				}"
"				Time = {"
"					Frequency = 5"
"					DataSource = Timer"
"					Type = uint32"
"				}"
"				ProfinetDeviceLed = {"
"					DataSource = DDB1"
"					Type = uint8"
"				}"
"				ProfinetDeviceReady = {"
"					DataSource = DDB1"
"					Type = uint8"
"				}"
"            }"
"            OutputSignals = {"
"				OutputBit = {"
"					Type = uint8"
"					DataSource = LoggerDataSource"
"				}"
"				OutputSINT = {"
"					Type = int8"
"					DataSource = LoggerDataSource"
"				}"
"				OutputUSINT = {"
"					Type = uint8"
"					DataSource = LoggerDataSource"
"				}"
"				OutputINT = {"
"					Type = int16"
"					DataSource = LoggerDataSource"
"				}"
"				OutputUINT = {"
"					Type = uint16"
"					DataSource = LoggerDataSource"
"				}"
"				OutputDINT = {"
"					Type = int32"
"					DataSource = LoggerDataSource"
"				}"
"				OutputUDINT = {"
"					Type = int32"
"					DataSource = LoggerDataSource"
"				}"
"				OutputLINT = {"
"					Type = int64"
"					DataSource = LoggerDataSource"
"				}"
"				OutputULINT = {"
"					Type = uint64"
"					DataSource = LoggerDataSource"
"				}"
"				OutputREAL = {"
"					Type = float32"
"					DataSource = LoggerDataSource"
"				}"
"				OutputLREAL = {"
"					Type = float64"
"					DataSource = LoggerDataSource"
"				}"
"				Time = {"
"					DataSource = LoggerDataSource"
"					Type = uint32"
"				}"
"				ProfinetDeviceLed = {"
"					DataSource = LoggerDataSource"
"					Type = uint8"
"				}"
"				ProfinetDeviceReady = {"
"					DataSource = LoggerDataSource"
"					Type = uint8"
"				}"
"            }"
"        }"
"    }"
"    +Data = {"
"        Class = ReferenceContainer"
"        DefaultDataSource = DDB1"
"        +Timings = {"
"            Class = TimingDataSource"
"        }"
"        +DDB1 = {"
"			Class = ProfinetDataSource"
"			NetworkInterface = \"dummyeth\""
"			StationName = \"rt-labs-dev\""
"			PeriodicInterval = 1000"
"			ReductionRatio = 10"
"			VendorIdentifier = 0xFEED"
"			DeviceIdentifier = 0xBEEF"
"			OEMVendorIdentifier = 0xC0FF"
"			OEMDeviceIdentifier = 0xEE01"
"			DeviceVendor = \"rt-labs\""
"			ManufacturerSpecificString = \"PNET demo\""
"			IMVendor = 0xFEED"
"			IMHardwareRevision = 1"
"			IMSoftwareRevision = 'V'"
"			IMFunctionalEnhancement = 1"
"			IMBugFix = 0"
"			IMInternalChange = 0"
"			IMProfileIdentifier = 0x1234"
"			IMProfileSpecificType = 0x5678"
"			IMVersionMajor = 0"
"			IMVersionMinor = 0"
"           IMOrderId = \"MARTe2\""
"           IMSerialNumber = \"0001\""
"			IMFunction = \"\""
"			IMLocation = \"\""
"			IMDate = \"\""
"			IMDescriptor = \"\""
"			IMSignature = \"\""
"			RTClass2Status = 0"
"			RTClass3Status = 0"
"			+MainThreadHelper = {"
"				Class = ProfinetMainThreadHelper"
"				Timeout = 1000"
"				CPUMask = 0xFF"
"			}"
"			+TimerHelper = {"
"				Class = ProfinetTimerHelper"
"                               Timeout = 0"
"				CPUMask = 0xFF"
"			}"
"			Slots = {"
"				Slot0 = {"
"					SlotNumber = 0"
"					Subslots = {"
"						Subslot1 = {"
"						SubslotNumber = 1"
"						DeviceAccessPoint = 1"
"						ExpectedDataDirection = 1"
"						ExpectedInputSize = 0"
"						ExpectedOutputSize = 0"
"					}"
"					Subslot32768 = {"
"						SubslotNumber = 32768"
"						DeviceAccessPoint = 1"
"						ExpectedDataDirection = 1"
"						ExpectedInputSize = 0"
"						ExpectedOutputSize = 0"
"					}"
"					Subslot32769 = {"
"						SubslotNumber = 32769"
"						DeviceAccessPoint = 1"
"						ExpectedDataDirection = 1"
"						ExpectedInputSize = 0"
"						ExpectedOutputSize = 0"
"					}"
"					}"
"				}"
"				Slot1 = {"
"					SlotNumber = 1"
"					Subslots = {"
"						Subslot1 = {"
"							SubslotNumber = 1"
"							DeviceAccessPoint = 0"
"							ExpectedDataDirection = 2"
"							ExpectedInputSize = 0"
"							ExpectedOutputSize = 43"
"						}"
"					}"
"				}"
"				Slot2 = {"
"					SlotNumber = 2"
"					Subslots = {"
"						Subslot1 = {"
"							SubslotNumber = 1"
"							DeviceAccessPoint = 0"
"							ExpectedDataDirection = 1"
"							ExpectedInputSize = 43"
"							ExpectedOutputSize = 0"
"						}"
"					}"
"				}"
"			}"
"			Signals = {"
"				OutputBit = {"
"					Type = uint8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 0"
"					Direction = 1"
"				}"
"				OutputSINT = {"
"					Type = int8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 1"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputUSINT = {"
"					Type = uint8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 2"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputINT = {"
"					Type = int16"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 3"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputUINT = {"
"					Type = uint16"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 5"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputDINT = {"
"					Type = int32"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 7"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputUDINT = {"
"					Type = int32"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 11"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputLINT = {"
"					Type = int64"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 15"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputULINT = {"
"					Type = uint64"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 23"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputREAL = {"
"					Type = float32"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 31"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				OutputLREAL = {"
"					Type = float64"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 1"
"					Subslot = 1"
"					Offset = 35"
"					Direction = 1"
"					NeedsSwapping = 1"
"				}"
"				InputBit = {"
"					Type = uint8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 0"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputSINT = {"
"					Type = int8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 1"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputUSINT = {"
"					Type = uint8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 2"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputINT = {"
"					Type = int16"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 3"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputUINT = {"
"					Type = uint16"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 5"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputDINT = {"
"					Type = int32"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 7"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputUDINT = {"
"					Type = int32"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 11"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputLINT = {"
"					Type = int64"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 15"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputULINT = {"
"					Type = uint64"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 23"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputREAL = {"
"					Type = float32"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 31"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				InputLREAL = {"
"					Type = float64"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"					Slot = 2"
"					Subslot = 1"
"					Offset = 35"
"					Direction = 0"
"					NeedsSwapping = 1"
"				}"
"				ProfinetDeviceLed = {"
"					Type = uint8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"				}"
"				ProfinetDeviceReady = {"
"					Type = uint8"
"					NumberOfElements = 1"
"					NumberOfDimensions = 0"
"				}"
"			}"
"        }"
"        +LoggerDataSource = {"
"            Class = LoggerDataSource"
"        }"
"		+Timer = {"
"            Class = LinuxTimer"
"            SleepNature = \"Default\""
"            Signals = {"
"				Counter = {"
"					Type = uint32"
"				}"
"                Time = {"
"                    Type = uint32"
"                }"
"            }"
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
"                    Functions = { ConstantSignal GAMTimer }"
"                }"
"            }"
"        }"
"    }"
"	+Scheduler = {"
"        Class = GAMScheduler"
"        TimingDataSource = Timings"
"    }"
"}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ProfinetDataSourceTest::ProfinetDataSourceTest() {

}

MARTe::StreamString ProfinetDataSourceTest::GetEthInterfaceName(bool getIfNameFromEnv) {
    
    if(getIfNameFromEnv) {
        char* envEth = getenv("PROFINET_ETH_INTERFACE");
        MARTe::StreamString mEthInterface(envEth);
        return mEthInterface;
    }
    else {
        MARTe::StreamString mEthInterface = "dummy-eth";
        return mEthInterface;    
    }
}

bool ProfinetDataSourceTest::PatchConfigurationDatabase(
    MARTe::ConfigurationDatabase& inputCdb,
    const MARTe::char8 * const absoluteParameterPath,
    const MARTe::char8 * const parameterToPatch,
    const MARTe::char8 * const replacementValue) {

    using namespace MARTe;

    bool ok = inputCdb.MoveAbsolute(absoluteParameterPath);
    if(ok) {
        ok = inputCdb.Delete(parameterToPatch);
        if(ok) {
            if(replacementValue != NULL) {
                StreamString repVal(replacementValue);
                inputCdb.Write(parameterToPatch, repVal);
            }
        }
    }
    if(ok) {
        ok = inputCdb.MoveToRoot();
    }

    return ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialise() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "NetworkInterface", ethInterface.Buffer());

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseFailBaseNet() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "NetworkInterface", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "StationName", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "PeriodicInterval", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "ReductionRatio", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseFailBaseId() {
    using namespace MARTe;
    
    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "VendorIdentifier", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "DeviceIdentifier", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "OEMVendorIdentifier", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "OEMDeviceIdentifier", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "DeviceVendor", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "ManufacturerSpecificString", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseFailInM() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMVendor", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMHardwareRevision", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMSoftwareRevision", NULL);

    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMFunctionalEnhancement", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMBugFix", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMInternalChange", NULL);

    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMProfileIdentifier", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMProfileSpecificType", NULL);

    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMVersionMajor", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMVersionMinor", NULL);

    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMFunction", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMLocation", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMDate", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMDescriptor", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMSignature", NULL);


    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseNoSlots() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots", "Slot0", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots", "Slot1", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots", "Slot2", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseWrongSlots() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots.Slot1.Subslots.Subslot1", "DeviceAccessPoint", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots.Slot1.Subslots.Subslot1", "ExpectedDataDirection", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots.Slot1.Subslots.Subslot1", "ExpectedInputSize", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots.Slot1.Subslots.Subslot1", "ExpectedOutputSize", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseNoSubslots() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots.Slot1", "Subslots", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseWrongDirection() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Signals.OutputBank", "Direction", "9");

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseWrongCoordinates() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Signals.OutputBit", "Slot", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Signals.OutputBit", "Subslot", NULL);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Signals.OutputBit", "Offset", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseFailSlotsSection() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "Slots", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseFailSignalsSection() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "Signals", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseFailTimerHelperSection() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "+TimerHelper", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneInitialiseFailMainHelperSection() {
    using namespace MARTe;

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "+MainThreadHelper", NULL);

    cdb.MoveAbsolute("$TestApp.+Data.+DDB1");

    ProfinetDataSource testDataSource;

    if(ok) {
        ok = testDataSource.Initialise(cdb);
    }

    return !ok;
}

bool ProfinetDataSourceTest::TestStandaloneSweepSoftwareRevision() {
    using namespace MARTe;

    StreamString allowedRevisions[6] = { 
        StreamString("V"), 
        StreamString("R"), 
        StreamString("P"), 
        StreamString("U"), 
        StreamString("T"),
        StreamString("Z") //We also sweep an invalid value in configuration
    };

    StreamString configStream = configurationFile;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    StreamString ethInterface = GetEthInterfaceName(true);
    
    ok = PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "NetworkInterface", ethInterface.Buffer());

    for(int i = 0; (i < 5) && ok; i++) {
        ProfinetDataSource *testDataSource = new ProfinetDataSource();    
        ok = (testDataSource != NULL);
        if(ok) {
            std::cout << "------------ " << allowedRevisions[i].Buffer() << std::endl;

            PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "IMSoftwareRevision", allowedRevisions[i].Buffer());
            cdb.MoveAbsolute("$TestApp.+Data.+DDB1");
            ok = testDataSource->Initialise(cdb);

            delete testDataSource;
        }
    }

    return ok;
}

bool ProfinetDataSourceTest::RunIntegrated(MARTe::ConfigurationDatabase& inputCdb, bool withSignalValueCheck) {
    using namespace MARTe;

    bool ok = true;

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if(ok) {
        god->Purge();
        ok = god->Initialise(inputCdb);
    }

    ReferenceT<RealTimeApplication> application;

    if(ok) {
        application = god->Find("TestApp");
        ok = application.IsValid();
    }

    if(ok) {
        ok = application->ConfigureApplication();
    }

    if(ok) {
        application->PrepareNextState("State1");
    }

    if(ok) {
        ok = application->StartNextStateExecution();
        Sleep::Sec(30);
        if(ok) {
            ok = application->StopCurrentStateExecution();
        }
        if(withSignalValueCheck) {
            ReferenceT<ProfinetDataSource> profinetDS;

            if(ok) {
                profinetDS = application->Find("Data.DDB1");
                ok = profinetDS.IsValid();
            }

            uint8* testInBIT = NULL_PTR(uint8*);
            uint8* testInSINT = NULL_PTR(uint8*);
            int8* testInUSINT = NULL_PTR(int8*);
            int16* testInINT = NULL_PTR(int16*);
            uint16* testInUINT = NULL_PTR(uint16*);
            int32* testInDINT = NULL_PTR(int32*);
            uint32* testInUDINT = NULL_PTR(uint32*);
            int64* testInLINT = NULL_PTR(int64*);
            uint64* testInULINT = NULL_PTR(uint64*);
            float32* testInREAL = NULL_PTR(float32*);
            float64* testInLREAL = NULL_PTR(float64*);

            uint8* testOutBIT = NULL_PTR(uint8*);
            uint8* testOutSINT = NULL_PTR(uint8*);
            int8* testOutUSINT = NULL_PTR(int8*);
            int16* testOutINT = NULL_PTR(int16*);
            uint16* testOutUINT = NULL_PTR(uint16*);
            int32* testOutDINT = NULL_PTR(int32*);
            uint32* testOutUDINT = NULL_PTR(uint32*);
            int64* testOutLINT = NULL_PTR(int64*);
            uint64* testOutULINT = NULL_PTR(uint64*);
            float32* testOutREAL = NULL_PTR(float32*);
            float64* testOutLREAL = NULL_PTR(float64*);

            if(ok) {
                profinetDS->GetSignalMemoryBuffer(11, 0, (void* &)testInBIT);
                profinetDS->GetSignalMemoryBuffer(12, 0, (void* &)testInSINT);
                profinetDS->GetSignalMemoryBuffer(13, 0, (void* &)testInUSINT);
                profinetDS->GetSignalMemoryBuffer(14, 0, (void* &)testInINT);
                profinetDS->GetSignalMemoryBuffer(15, 0, (void* &)testInUINT);
                profinetDS->GetSignalMemoryBuffer(16, 0, (void* &)testInDINT);
                profinetDS->GetSignalMemoryBuffer(17, 0, (void* &)testInUDINT);
                profinetDS->GetSignalMemoryBuffer(18, 0, (void* &)testInLINT);
                profinetDS->GetSignalMemoryBuffer(19, 0, (void* &)testInULINT);
                profinetDS->GetSignalMemoryBuffer(20, 0, (void* &)testInREAL);
                profinetDS->GetSignalMemoryBuffer(21, 0, (void* &)testInLREAL);
            }

            if(ok) {
                profinetDS->GetSignalMemoryBuffer(0, 0, (void* &)testOutBIT);
                profinetDS->GetSignalMemoryBuffer(1, 0, (void* &)testOutSINT);
                profinetDS->GetSignalMemoryBuffer(2, 0, (void* &)testOutUSINT);
                profinetDS->GetSignalMemoryBuffer(3, 0, (void* &)testOutINT);
                profinetDS->GetSignalMemoryBuffer(4, 0, (void* &)testOutUINT);
                profinetDS->GetSignalMemoryBuffer(5, 0, (void* &)testOutDINT);
                profinetDS->GetSignalMemoryBuffer(6, 0, (void* &)testOutUDINT);
                profinetDS->GetSignalMemoryBuffer(7, 0, (void* &)testOutLINT);
                profinetDS->GetSignalMemoryBuffer(8, 0, (void* &)testOutULINT);
                profinetDS->GetSignalMemoryBuffer(9, 0, (void* &)testOutREAL);
                profinetDS->GetSignalMemoryBuffer(10, 0, (void* &)testOutLREAL);

                ok =    (*testInBIT == *testOutBIT) &&
                        (*testInSINT == *testOutSINT) &&
                        (*testInUSINT == *testOutUSINT) &&
                        (*testInINT == *testOutINT) &&
                        (*testInUINT == *testOutUINT) &&
                        (*testInDINT == *testOutDINT) &&
                        (*testInUDINT == *testOutUDINT) &&
                        (*testInLINT == *testOutLINT) &&
                        (*testInULINT == *testOutULINT) &&
                        (*testInREAL == *testOutREAL) &&
                        (*testInLREAL == *testOutLREAL);
            }
        }

        god->Purge();
        god = NULL;
    }

    return ok;
}

bool ProfinetDataSourceTest::TestIntegratedRunProfinetSlave() {

    using namespace MARTe;

    bool testValid = false;
    
    ConfigurationDatabase cdb;
    StreamString configStream = configurationFile;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    testValid = parser.Parse();

    if(testValid) {
        StreamString ethInterface = GetEthInterfaceName(true);
        testValid = PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "NetworkInterface", ethInterface.Buffer());
    }

    if(testValid) {
        testValid = RunIntegrated(cdb, false);
    }

    return testValid;
}

bool ProfinetDataSourceTest::TestIntegratedRunFailModulePlugging() {

    using namespace MARTe;

    bool testValid = false;
    
    ConfigurationDatabase cdb;
    StreamString configStream = configurationFile;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    testValid = parser.Parse();

    if(testValid) {
        StreamString ethInterface = GetEthInterfaceName(true);
        testValid = PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "NetworkInterface", ethInterface.Buffer());
    }

    if(testValid) {
        testValid = PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots.Slot1.Subslots.Subslot1", "ExpectedDataDirection", "0");
        testValid = testValid && PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1.Slots.Slot2.Subslots.Subslot1", "ExpectedDataDirection", "3");
    }

    if(testValid) {
        testValid = RunIntegrated(cdb, false);
    }

    return testValid;
}

bool ProfinetDataSourceTest::TestIntegratedRunPLCLoopback() {
    using namespace MARTe;

    bool testValid = false;

    ConfigurationDatabase cdb;
    StreamString configStream = configurationFile;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    testValid = parser.Parse();

    if(testValid) {
        //Patching the ETH interface name
        StreamString ethInterface = GetEthInterfaceName(true);
        testValid = PatchConfigurationDatabase(cdb, "$TestApp.+Data.+DDB1", "NetworkInterface", ethInterface.Buffer());
    }

    if(testValid) {
        testValid = RunIntegrated(cdb, true);
    }

    return testValid;
}
