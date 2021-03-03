/**
 * @file NI9157MxiDataSourceTest.cpp
 * @brief Source file for class NI9157MxiDataSourceTest
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 * the class NI9157MxiDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "AdvancedErrorManagement.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "IOGAM.h"
#include "Interleaved2FlatGAM.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "NI9157MxiDataSourceTest.h"
#include "ObjectRegistryDatabase.h" 
#include "RealTimeApplication.h" 
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI9157MxiDataSourceTestDS: public NI9157MxiDataSource {
public:
    CLASS_REGISTER_DECLARATION()

    NI9157MxiDataSourceTestDS();
    virtual ~NI9157MxiDataSourceTestDS();
    ReferenceT<NI9157Device> GetNiDeviceBoard();
    NI9157DeviceOperatorTI ** GetNiDevice();
    uint32 *GetVarId();
    uint8 GetRunNi();
    uint8 *GetSignalFlag();
    uint32 *GetNumberOfElements();
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

};

NI9157MxiDataSourceTestDS::NI9157MxiDataSourceTestDS() {
}

NI9157MxiDataSourceTestDS::~NI9157MxiDataSourceTestDS() {
}

ReferenceT<NI9157Device> NI9157MxiDataSourceTestDS::GetNiDeviceBoard() {
    return niDeviceBoard;
}

NI9157DeviceOperatorTI ** NI9157MxiDataSourceTestDS::GetNiDevice() {
    return niDevice;
}

uint32 *NI9157MxiDataSourceTestDS::GetVarId() {
    return varId;
}

uint8 NI9157MxiDataSourceTestDS::GetRunNi() {
    return runNi;
}

uint8 *NI9157MxiDataSourceTestDS::GetSignalFlag() {
    return signalFlag;
}

uint32 *NI9157MxiDataSourceTestDS::GetNumberOfElements() {
    return numberOfElements;
}

bool NI9157MxiDataSourceTestDS::PrepareNextState(const char8 * const currentStateName,
                                                   const char8 * const nextStateName) {

    bool ret = false;
    ret = NI9157MxiDataSource::PrepareNextState(currentStateName, nextStateName);

    return ret;
}

CLASS_REGISTER(NI9157MxiDataSourceTestDS, "1.0")

class NI9157MxiDataSourceTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    NI9157MxiDataSourceTestGAM1 ();
    virtual bool Setup();
    virtual bool Execute();
    void *GetInputMemoryBuffer();
    void *GetOutputMemoryBuffer();

private:

    uint32 totalSize;

};

NI9157MxiDataSourceTestGAM1::NI9157MxiDataSourceTestGAM1() :
        GAM() {

    totalSize = 0u;

}

bool NI9157MxiDataSourceTestGAM1::Setup() {
    return true;
}

bool NI9157MxiDataSourceTestGAM1::Execute() {
    return true;
}

void *NI9157MxiDataSourceTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *NI9157MxiDataSourceTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(NI9157MxiDataSourceTestGAM1, "1.0")

class NI9157MxiDataSourceTestGAM2: public Interleaved2FlatGAM {
public:
    CLASS_REGISTER_DECLARATION()

    NI9157MxiDataSourceTestGAM2 ();
    void *GetInputMemoryBuffer();
    void *GetOutputMemoryBuffer();

};

NI9157MxiDataSourceTestGAM2::NI9157MxiDataSourceTestGAM2() :
        Interleaved2FlatGAM() {
}

void *NI9157MxiDataSourceTestGAM2::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *NI9157MxiDataSourceTestGAM2::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(NI9157MxiDataSourceTestGAM2, "1.0")

static uint32 model = 0u;

static const uint32 nParams                 = 3u;
static const char8 * const firmwarePath     = "Test/Components/Interfaces/NI9157Device/TestLabviewFiles";
static const char8 * const multiIOFirmware[]= {"RIO0", "NiFpga_NI9159_MultiIO.lvbitx", "03AB279CA6C34216C3ABAADB90262282"};
static const char8 * const multiIOConfig0 = ""
    "+NiDevice = {"
    "    Class = NI9157Device"
    "    NiRioDeviceName = XptoDevice"
    "    NiRioGenFile = XptoPath"
    "    NiRioGenSignature = XptoSignature"
    "    Open = 1"
    "    Configuration = {"
    "        ControlBool_stop = 0"
    "        ControlBool_use_dsfifo_data = 0"
    "        ControlBool_use_counter = 1"
    "        ControlU32_cycle_ticks = 200"
    "        ControlU64_packet_size = 1"
    "    }"
    "}"
    "$Application1 = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +GAMA = {"
    "            Class = NI9157MxiDataSourceTestGAM1"
    "            InputSignals = {"
    "                FIFO1_U64_R = {"
    "                    DataSource = Drv1"
    "                    Type = uint64"
    "                    Frequency = 0"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 10000"
    "                }"
    "                ticks_counter = {"
    "                    DataSource = Drv1"
    "                    Type = uint32"
    "                }"
    "            }"
    "            OutputSignals = {"
    "                FIFO0_U64_W = {"
    "                    DataSource = Drv1"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 10000"
    "                }"
    "                sample_value = {"
    "                    DataSource = Drv1"
    "                    Type = uint16"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Data = {"
    "        Class = ReferenceContainer"
    "        +Drv1 = {"
    "            Class = NI9157MxiDataSourceTestDS"
    "            NI9157DevicePath = NiDevice"
    "            RunNi = 1"
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
    "        Class = GAMScheduler"
    "        TimingDataSource = Timings"
    "    }"
    "}";

static const char8 * const multiIOConfig1 = ""
    "+NiDevice = {"
    "    Class = NI9157Device"
    "    NiRioDeviceName = XptoDevice"
    "    NiRioGenFile = XptoPath"
    "    NiRioGenSignature = XptoSignature"
    "    Open = 1"
    "    Configuration = {"
    "        ControlBool_stop = 0"
    "        ControlBool_use_dsfifo_data = 0"
    "        ControlBool_use_counter = 1"
    "        ControlU32_cycle_ticks = 200"
    "        ControlU64_packet_size = 1"
    "    }"
    "}"
    "$Application1 = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +GAMA = {"
    "            Class = NI9157MxiDataSourceTestGAM1"
    "            InputSignals = {"
    "                FIFO1_U64_R = {"
    "                    DataSource = Drv1"
    "                    Type = uint64"
    "                    Frequency = 0"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 10000"
    "                }"
    "                ticks_counter = {"
    "                    DataSource = Drv1"
    "                    Type = uint32"
    "                }"
    "            }"
    "            OutputSignals = {"
    "                FIFO0_U64_W = {"
    "                    DataSource = Drv1"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 10000"
    "                }"
    "                sample_value = {"
    "                    DataSource = Drv1"
    "                    Type = uint16"
    "                }"
    "            }"
    "        }"
    "        +GAMB = {"
    "            Class = NI9157MxiDataSourceTestGAM2"
    "            InputSignals = {"
    "               FIFO1_U64_R = {"
    "                   DataSource = Drv1"
    "                   Type = uint64"
    "                   Samples = 1"
    "                   NumberOfDimensions = 1"
    "                   NumberOfElements = 10000"
    "                   PacketMemberSizes = {8, 8, 4, 4, 4, 4, 2, 2, 2, 1, 1}"
    "               }"
    "            }"
    "            OutputSignals = {"
    "                Signal0_U64_PacketCounter = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal1_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal2_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal3_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal4_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal5_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal6_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal7_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal8_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal9_U8 = {"
    "                    Type = uint8"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal10_U8 = {"
    "                    Type = uint8"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Data = {"
    "        Class = ReferenceContainer"
    "        +DDB = {"
    "            Class = GAMDataSource"
    "        }"
    "        +Drv1 = {"
    "            Class = NI9157MxiDataSourceTestDS"
    "            NI9157DevicePath = NiDevice"
    "            RunNi = 1"
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
    "                    Functions = {GAMA GAMB}"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Scheduler = {"
    "        Class = GAMScheduler"
    "        TimingDataSource = Timings"
    "    }"
    "}";

static const char8 * const multiIOConfig2 = ""
    "+NiDevice = {"
    "    Class = NI9157Device"
    "    NiRioDeviceName = XptoDevice"
    "    NiRioGenFile = XptoPath"
    "    NiRioGenSignature = XptoSignature"
    "    Open = 1"
    "    Configuration = {"
    "        ControlBool_stop = 0"
    "        ControlBool_use_dsfifo_data = 0"
    "        ControlBool_use_counter = 1"
    "        ControlU32_cycle_ticks = 200"
    "        ControlU64_packet_size = 1"
    "    }"
    "}"
    "$Application1 = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +GAMA = {"
    "            Class = NI9157MxiDataSourceTestGAM2"
    "            InputSignals = {"
    "               FIFO1_U64_R = {"
    "                   DataSource = Drv1"
    "                   Type = uint64"
    "                   Samples = 1"
    "                   Frequency = 1"
    "                   NumberOfDimensions = 1"
    "                   NumberOfElements = 10000"
    "                   PacketMemberSizes = {8, 8, 4, 4, 4, 4, 2, 2, 2, 1, 1}"
    "               }"
    "            }"
    "            OutputSignals = {"
    "                Signal0_U64_PacketCounter = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal1_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal2_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal3_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal4_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal5_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal6_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal7_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal8_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal9_U8 = {"
    "                    Type = uint8"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal10_U8 = {"
    "                    Type = uint8"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "            }"
    "        }"
    "        +GAMB = {"
    "            Class = IOGAM"
    "            InputSignals = {"
    "                Signal6_U16 = {"
    "                    DataSource = DDB"
    "                    Type = uint16"
    "                    Ranges = {{1000, 1000}}"
    "                }"
    "            }"
    "            OutputSignals = {"
    "                sample_value = {"
    "                    DataSource = Drv1"
    "                    Type = uint16"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Data = {"
    "        Class = ReferenceContainer"
    "        +DDB = {"
    "            Class = GAMDataSource"
    "        }"
    "        +Drv1 = {"
    "            Class = NI9157MxiDataSourceTestDS"
    "            RunNi = 1"
    "            NI9157DevicePath = NiDevice"
    "            Signals = {"
    "                ticks_counter = {"
    "                    Type = uint32"
    "                }"
    "                sample_value = {"
    "                    Type = uint16"
    "                }"
    "            }"
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
    "                    Functions = {GAMA GAMB}"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Scheduler = {"
    "        Class = GAMScheduler"
    "        TimingDataSource = Timings"
    "    }"
    "}";

static const char8 * const multiIOConfig3 = ""
    "+NiDevice = {"
    "    Class = NI9157Device"
    "    NiRioDeviceName = XptoDevice"
    "    NiRioGenFile = XptoPath"
    "    NiRioGenSignature = XptoSignature"
    "    Open = 1"
    "    Configuration = {"
    "        ControlBool_stop = 0"
    "        ControlBool_use_dsfifo_data = 1"
    "        ControlBool_use_counter = 1"
    "        ControlU32_cycle_ticks = 200"
    "        ControlU64_packet_size = 1"
    "    }"
    "}"
    "$Application1 = {"
    "    Class = RealTimeApplication"
    "    +Functions = {"
    "        Class = ReferenceContainer"
    "        +GAMA = {"
    "            Class = NI9157MxiDataSourceTestGAM2"
    "            InputSignals = {"
    "               FIFO1_U64_R = {"
    "                   DataSource = Drv1"
    "                   Type = uint64"
    "                   Samples = 1"
    "                   Frequency = 1"
    "                   NumberOfDimensions = 1"
    "                   NumberOfElements = 10000"
    "                   PacketMemberSizes = {8, 8, 4, 4, 4, 4, 2, 2, 2, 1, 1}"
    "               }"
    "            }"
    "            OutputSignals = {"
    "                Signal0_U64_PacketCounter = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal1_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal2_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal3_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal4_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal5_U32 = {"
    "                    Type = uint32"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal6_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal7_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal8_U16 = {"
    "                    Type = uint16"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal9_U8 = {"
    "                    Type = uint8"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal10_U8 = {"
    "                    Type = uint8"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "            }"
    "        }"
    "        +GAMB = {"
    "            Class = IOGAM"
    "            InputSignals = {"
    "                Signal0_U64_PacketCounter = {"
    "                    DataSource = DDB"
    "                    Type = uint64"
    "                    Ranges = {{0, 1999}}"
    "                }"
    "            }"
    "            OutputSignals = {"
    "                FIFO0_U64_W = {"
    "                    DataSource = Drv1"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    Type = uint64"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Data = {"
    "        Class = ReferenceContainer"
    "        +DDB = {"
    "            Class = GAMDataSource"
    "        }"
    "        +Drv1 = {"
    "            Class = NI9157MxiDataSourceTestDS"
    "            RunNi = 1"
    "            NI9157DevicePath = NiDevice"
    "            Signals = {"
    "                ticks_counter = {"
    "                    Type = uint32"
    "                }"
    "                sample_value = {"
    "                    Type = uint16"
    "                }"
    "            }"
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
    "                    Functions = {GAMA GAMB}"
    "                }"
    "            }"
    "        }"
    "    }"
    "    +Scheduler = {"
    "        Class = GAMScheduler"
    "        TimingDataSource = Timings"
    "    }"
    "}";


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * Helper function to setup a MARTe execution environment
 */
// static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

//     HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
//     ConfigurationDatabase cdb;
//     StreamString configStream = config;
//     configStream.Seek(0);
//     StandardParser parser(configStream, cdb);
//     bool ok = parser.Parse();
//     ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

//     if (ok) {
//         god->Purge();
//         ok = god->Initialise(cdb);
//     }

//     ReferenceT<RealTimeApplication> application;

//     if (ok) {
//         application = god->Find("Application1");
//         ok = application.IsValid();
//     }
//     if (ok) {
//         ok = application->ConfigureApplication();
//     }

//     return ok;
// }

NI9157MxiDataSourceTest::NI9157MxiDataSourceTest() {
}

NI9157MxiDataSourceTest::~NI9157MxiDataSourceTest() {
}

bool NI9157MxiDataSourceTest::TestConstructor() {

    NI9157MxiDataSourceTestDS dataSource;
    bool ret = false;
    
    ret = dataSource.GetNiDevice() == NULL;
    ret &= dataSource.GetVarId() == NULL;
    ret &= dataSource.GetRunNi() == 0;
    ret &= dataSource.GetSignalFlag() == NULL;
    ret &= dataSource.GetNumberOfElements() == NULL;
    ReferenceT<NI9157Device> x = dataSource.GetNiDeviceBoard();
    ret &= !x.IsValid();

    return ret;
}

bool NI9157MxiDataSourceTest::TestInitialise() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
	        "    Configuration = {"
	        "        ControlBool_stop = 0"
	        "        ControlBool_use_dsfifo_data = 0"
	        "        ControlBool_use_counter = 1"
	        "        ControlU32_cycle_ticks = 200"
	        "        ControlU64_packet_size = 1"
	        "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            NI9157DevicePath = NiDevice"
            "            RunNi = 1"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret &= dataSource->GetRunNi() == 1;
        ReferenceT<NI9157Device> x = dataSource->GetNiDeviceBoard();
        ret &= x.IsValid();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestInitialise_DefaultRunNi() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            NI9157DevicePath = NiDevice"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }
    if (ret) {
        ret = cdb.MoveAbsolute("Application1.Data.Drv1");
        ret &= cdb.Delete("RunNi");
        ret &= cdb.MoveToRoot();       
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret &= dataSource->GetRunNi() == 0;
        ReferenceT<NI9157Device> x = dataSource->GetNiDeviceBoard();
        ret &= x.IsValid();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestInitialise_False_NoNiDev() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }   
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.Data.Drv1");
        ret &= cdb.Delete("NI9157DevicePath");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestInitialise_False_InvalidNiDevPath(){

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            NI9157DevicePath = somePath"
            "            RunNi = 1"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }    
    if (ret) {
        ret = cdb.MoveAbsolute("Application1.Data.Drv1");
        ret &= cdb.Write("NI9157DevicePath", "SomePath");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestGetBrokerName() {

    const char8 *config = "Frequency = -1\n"
            "Trigger = 1";

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    NI9157MxiDataSource dataSource;
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb, InputSignals), "MemoryMapSynchronisedInputBroker") == 0;
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb, OutputSignals), "MemoryMapSynchronisedOutputBroker") == 0;

    const char8 * config1 = "Frequency = 1\n"
            "Trigger = 0";
    ConfigurationDatabase cdb1;

    StreamString configStream1 = config1;
    configStream1.Seek(0);
    StandardParser parser1(configStream1, cdb1);

    ret &= parser1.Parse();
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb1, InputSignals), "MemoryMapSynchronisedInputBroker") == 0;
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb1, OutputSignals), "MemoryMapSynchronisedOutputBroker") == 0;

    const char8 * config2 = "Frequency = -1\n"
            "Trigger = 0";
    ConfigurationDatabase cdb2;

    StreamString configStream2 = config2;
    configStream2.Seek(0);
    StandardParser parser2(configStream2, cdb2);

    ret &= parser2.Parse();
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb2, InputSignals), "MemoryMapInputBroker") == 0;
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb2, OutputSignals), "MemoryMapOutputBroker") == 0;

    const char8 * config3 = "Trigger = 1";
    ConfigurationDatabase cdb3;

    StreamString configStream3 = config3;
    configStream3.Seek(0);
    StandardParser parser3(configStream3, cdb3);

    ret &= parser3.Parse();
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb3, InputSignals), "MemoryMapSynchronisedInputBroker") == 0;
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb3, OutputSignals), "MemoryMapSynchronisedOutputBroker") == 0;

    const char8 * config4 = "Frequency = -1.0\n";
    ConfigurationDatabase cdb4;

    StreamString configStream4 = config4;
    configStream4.Seek(0);
    StandardParser parser4(configStream4, cdb4);

    ret &= parser4.Parse();
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb4, InputSignals), "MemoryMapInputBroker") == 0;
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb4, OutputSignals), "MemoryMapOutputBroker") == 0;

    const char8 * config5 = "ciaone = 0.0\n";
    ConfigurationDatabase cdb5;

    StreamString configStream5 = config5;
    configStream5.Seek(0);
    StandardParser parser5(configStream5, cdb5);

    ret &= parser5.Parse();
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb5, InputSignals), "MemoryMapInputBroker") == 0;
    ret &= StringHelper::Compare(dataSource.GetBrokerName(cdb5, OutputSignals), "MemoryMapOutputBroker") == 0;

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSetConfiguredDatabase() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    //bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }  

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ReferenceT<NI9157Device> x = dataSource->GetNiDeviceBoard();
        ret = x.IsValid();
    }
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->Reset();
        ret = err.ErrorsCleared();
    }
    if (ret) {
        NI9157DeviceOperatorTI ** operators = dataSource->GetNiDevice();
        NI9157DeviceOperatorT<uint64>* x0 = dynamic_cast<NI9157DeviceOperatorT<uint64>*>(operators[0]);
        ret &= x0 != NULL;
        NI9157DeviceOperatorT<uint32>* x1 = dynamic_cast<NI9157DeviceOperatorT<uint32>*>(operators[1]);
        ret &= x1 != NULL;
        NI9157DeviceOperatorT<uint64>* x2 = dynamic_cast<NI9157DeviceOperatorT<uint64>*>(operators[2]);
        ret &= x2 != NULL;
        NI9157DeviceOperatorT<uint16>* x3 = dynamic_cast<NI9157DeviceOperatorT<uint16>*>(operators[3]);
        ret &= x3 != NULL;
    }
    if (ret) {
        uint32 descriptors[] = { 0, 0x8130, 1, 0x8136 };
        uint32 nElementsTest[] = { 10000, 1, 10000, 1 };
        uint32 * ids = dataSource->GetVarId();
        uint32 * nElements = dataSource->GetNumberOfElements();
        uint8 * signalFlags = dataSource->GetSignalFlag();
        uint32 numberOfSignals = dataSource->GetNumberOfSignals();
        ret = (numberOfSignals == 4);
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = ids[i] == descriptors[i];
            if (ret) {
                ret = (nElements[i] == nElementsTest[i]);
                if(!ret){
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at Element[%u] with %u != %u", i,  nElements[i], nElementsTest[i]);
                }
            }
            if (ret) {
                ret = (signalFlags[i] == 0);
                if(!ret){
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at SignalFlag[%u] with %u != 0", i, static_cast<uint32> (signalFlags[i]));
                }
            }
        }
    }
    if (ret) {
        ret = dataSource->GetRunNi() == 1;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSetConfiguredDatabase_False_InvalidLabviewVar() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                SomeFIFO = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }
    if (ret) {
        ret = cdb.MoveAbsolute("Application1.Functions.GAMA.InputSignals");
        ret &= cdb.CreateRelative("SomeFIFO");
        ret &= ConfigurationDatabase tempNode = cdb;
        ret &= cdb.MoveToAncestor(1u);
        ret &= cdb.MoveRelative("FIFO1_U64_R");
        ret &= cdb.Copy(tempNode);
        ret &= cdb.MoveToAncestor(1u);
        ret &= cdb.Delete("FIFO1_U64_R");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->!Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSetConfiguredDatabase_False_InvalidType() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = float32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    //bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }
    if (ret) {
        ret = cdb.MoveAbsolute("Application1.Functions.GAMA.InputSignals.ticks_counter");
        ret &= cdb.Write("Type", "float32");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->!Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestPrepareNextState() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->Reset();
        ret = err.ErrorsCleared();
    }
    if (ret) {
        ReferenceT<NI9157Device> x = dataSource->GetNiDeviceBoard();
        ret = x.IsValid();
    }
    if (ret) {
        NI9157DeviceOperatorTI ** operators = dataSource->GetNiDevice();
        NI9157DeviceOperatorT<uint64>* x0 = dynamic_cast<NI9157DeviceOperatorT<uint64>*>(operators[0]);
        ret &= x0 != NULL;
        NI9157DeviceOperatorT<uint32>* x1 = dynamic_cast<NI9157DeviceOperatorT<uint32>*>(operators[1]);
        ret &= x1 != NULL;
        NI9157DeviceOperatorT<uint64>* x2 = dynamic_cast<NI9157DeviceOperatorT<uint64>*>(operators[2]);
        ret &= x2 != NULL;
        NI9157DeviceOperatorT<uint16>* x3 = dynamic_cast<NI9157DeviceOperatorT<uint16>*>(operators[3]);
        ret &= x3 != NULL;
    }
    if (ret) {
        uint32 descriptors[] = { 0, 0x8130, 1, 0x8136 };
        uint32 nElementsTest[] = { 10000, 1, 10000, 1 };
        uint32 * ids = dataSource->GetVarId();
        uint32 * nElements = dataSource->GetNumberOfElements();
        uint8 * signalFlags = dataSource->GetSignalFlag();
        uint32 numberOfSignals = dataSource->GetNumberOfSignals();
        ret = numberOfSignals == 4;
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = ids[i] == descriptors[i];
            if (ret) {
                ret = (nElements[i] == nElementsTest[i]);
                if(!ret){
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at Element[%u] with %u != %u", i,  nElements[i], nElementsTest[i]);
                }
            }
            if (ret) {
                ret = (signalFlags[i] == 0);
                if(!ret){
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at SignalFlag[%u] with %u != 0", i, static_cast<uint32> (signalFlags[i]));
                }
            }
        }
    }
    if (ret) {
        ret = dataSource->GetRunNi() == 1;
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    if (ret) {
        uint8 * signalFlags = dataSource->GetSignalFlag();
        ret &= signalFlags[0] == 5;
        ret &= signalFlags[1] == 1;
        ret &= signalFlags[2] == 6;
        ret &= signalFlags[3] == 2;
        ret &= dataSource->GetRunNi() == 1;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSynchronise() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = NI9157MxiDataSourceTestGAM2"
            "            InputSignals = {"
            "               FIFO1_U64_R = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "                PacketCounter = {"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                MHVPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                AI = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Alasm = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                InternalErrors = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                D0 = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                State = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Reserved = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
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
            "                    Functions = {GAMA GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig1;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->Reset();
        ret = err.ErrorsCleared();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    ReferenceT < NI9157MxiDataSourceTestGAM1 > gam1;
    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam1.IsValid();
    }
    ReferenceT < NI9157MxiDataSourceTestGAM2 > gam2;
    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam2.IsValid();
    }
    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapInputBroker > brokerIn1;
    ReferenceT < MemoryMapOutputBroker > brokerOut1;
    ReferenceT < MemoryMapInputBroker > brokerIn2;
    if (ret) {
        ReferenceContainer inputBrokers1, outputBrokers1, inputBrokers2;
        ret = gam1->GetInputBrokers(inputBrokers1);
        if (ret) {
            ret = gam1->GetOutputBrokers(outputBrokers1);
        }
        if (ret) {
            ret = gam2->GetInputBrokers(inputBrokers2);
        }
        if (ret) {
            brokerSync = inputBrokers1.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            brokerIn1 = inputBrokers1.Get(1);
            ret = brokerIn1.IsValid();
        }
        if (ret) {
            brokerOut1 = outputBrokers1.Get(0);
            ret = brokerOut1.IsValid();
        }
        if (ret) {
            brokerIn2 = inputBrokers2.Get(0);
            ret = brokerIn2.IsValid();
        }
    }
    if (ret) {
        uint64 *mem = (uint64 *) gam2->GetOutputMemoryBuffer();
        uint32 *mem1 = (((uint32 *) gam1->GetInputMemoryBuffer()) + 20000);
        uint32 storeTick = 0u;
        uint32 nReads = 100;
        uint32 beginMxiIndex = 20;
        //8000 tolerance ticks (cycle_ticks = 20 ticks is for 2MHz)
        //80000 tolerance ticks (cycle_ticks = 200 ticks is for 200kHz)
        uint32 tol = 8000;
        //40000 = 20 ticks * 2000 packets (cycle_ticks = 20 ticks is for 2MHz)
        //400000 = 200 ticks * 2000 packets (cycle_ticks = 200 ticks if for 200kHz)
        uint32 expectedDeltaTick = 400000;
        for (uint32 i = 0u; (i < nReads) && (ret); i++) {
            brokerSync->Execute();
            brokerIn1->Execute();
            gam1->Execute();
            brokerOut1->Execute();
            brokerIn2->Execute();
            gam2->Execute();
            if (i > beginMxiIndex) {
                uint32 deltaTick = (*mem1 - storeTick);
                ret = ((deltaTick - expectedDeltaTick) < tol) || ((deltaTick - expectedDeltaTick) > -tol);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error deltaTicks[%u]=%u != %u+/-%u", i, static_cast<uint32> (deltaTick), expectedDeltaTick, tol);
                }
            }
            storeTick = *mem1;
            if (ret) {
                ret = (mem[0] == ((2000 * i) + 1));
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error mem[%u]=%u != %u", i, static_cast<uint32> (mem[0]), ((2000 * i) + 1));
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSynchronise_Oscilloscope() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM2"
            "            InputSignals = {"
            "               FIFO1_U64_R = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "                PacketCounter = {"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                MHVPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                AI = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Alasm = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                InternalErrors = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                D0 = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                State = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Reserved = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = IOGAM"
            "            InputSignals = {"
            "                GY1_MHVPS_Current = {"
            "                    DataSource = DDB"
            "                    Type = uint16"
            "                    Ranges = {{1000, 1000}}"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
            "            Signals = {"
            "                ticks_counter = {"
            "                    Type = uint32"
            "                }"
            "                sample_value = {"
            "                    Type = uint16"
            "                }"
            "            }"
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
            "                    Functions = {GAMA GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig2;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    ReferenceT < NI9157MxiDataSourceTestGAM2 > gam1;
    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam1.IsValid();
    }
    ReferenceT<IOGAM> gam2;
    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam2.IsValid();
    }
    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > brokerOut1;
    ReferenceT < MemoryMapInputBroker > brokerIn2;
    ReferenceT < MemoryMapOutputBroker > brokerOut2;
    if (ret) {
        ReferenceContainer inputBrokers1, outputBrokers1, inputBrokers2, outputBrokers2;
        ret = gam1->GetInputBrokers(inputBrokers1);
        if (ret) {
            ret = gam1->GetOutputBrokers(outputBrokers1);
        }
        if (ret) {
            ret = gam2->GetInputBrokers(inputBrokers2);
        }
        if (ret) {
            ret = gam2->GetOutputBrokers(outputBrokers2);
        }
        if (ret) {
            brokerSync = inputBrokers1.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            brokerOut1 = outputBrokers1.Get(0);
            ret = brokerOut1.IsValid();
        }
        if (ret) {
            brokerIn2 = inputBrokers2.Get(0);
            ret = brokerIn2.IsValid();
        }
        if (ret) {
            brokerOut2 = outputBrokers2.Get(0);
            ret = brokerOut2.IsValid();
        }
    }
    if (ret) {
        uint64 *mem = (uint64 *) gam1->GetOutputMemoryBuffer();
        uint32 nReads = 1000;
        for (uint32 i = 0u; (i < nReads) && (ret); i++) {
            brokerSync->Execute();
            gam1->Execute();
            brokerOut1->Execute();
            brokerIn2->Execute();
            gam2->Execute();
            brokerOut2->Execute();
            if (ret) {
                ret = (mem[0] == ((2000 * i) + 1));
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error at mem[%u]=%u != %u", i, static_cast<uint32>(mem[0]), ((2000 * i) + 1));
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSynchronise_Oscilloscope_OutputFIFO() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
    "    Configuration = {"
    "        ControlBool_stop = 0"
    "        ControlBool_use_dsfifo_data = 1"
    "        ControlBool_use_counter = 1"
    "        ControlU32_cycle_ticks = 200"
    "        ControlU64_packet_size = 1"
    "    }"
            //"    Configuration = {"
            //"        NiFpga_TestGTD0001_ControlBool_stop = 0"
            //"        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 0"
            //"        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            //"        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            //"        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            //"        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 196"
            //"        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
            //"        NiFpga_TestGTD0001_ControlU32_tcn_cycle_phase = 10000"
            //"        NiFpga_TestGTD0001_ControlU32_tcn_period_ticks = 40000"
            //"        NiFpga_TestGTD0001_ControlI32_Timeout = 0"
            //"        NiFpga_TestGTD0001_ControlU64_packet_size = 1"
            //"        NiFpga_TestGTD0001_ControlU64_end_frame = 0xFFFFFFFFFFFFFFFF"
            //"    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM2"
            "            InputSignals = {"
            "               FIFO1_U64_R = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "                PacketCounter = {"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                MHVPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                AI = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Alasm = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                InternalErrors = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                D0 = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                State = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Reserved = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = IOGAM"
            "            InputSignals = {"
            "                PacketCounter = {"
            //"                GY1_MHVPS_Current = {"
            "                   DataSource = DDB"
            //"                   Type = uint16"
            //"                   Ranges = {{0, 1999}}"
            "                   Type = uint64"
            "                   Ranges = {{0, 1999}}"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               FIFO0_U64_W = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 2000"
            //"                   Type = uint16"
            "                   Type = uint64"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
            "            Signals = {"
            "               ticks_counter = {"
            "                   Type = uint32"
            "               }"
            "               sample_value = {"
            "                    Type = uint16"
            "                }"
            "            }"
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
            "                    Functions = {GAMA GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // Sleep::Sec(1);
    // bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  multiIOConfig3;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    ReferenceT < NI9157MxiDataSourceTestGAM2 > gam1;
    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam1.IsValid();
    }
    ReferenceT<IOGAM> gam2;
    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam2.IsValid();
    }
    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > brokerOut1;
    ReferenceT < MemoryMapInputBroker > brokerIn2;
    ReferenceT < MemoryMapOutputBroker > brokerOut2;
    if (ret) {
        ReferenceContainer inputBrokers1, outputBrokers1, inputBrokers2, outputBrokers2;
        ret = gam1->GetInputBrokers(inputBrokers1);
        if (ret) {
            ret = gam1->GetOutputBrokers(outputBrokers1);
        }
        if (ret) {
            ret = gam2->GetInputBrokers(inputBrokers2);
        }
        if (ret) {
            ret = gam2->GetOutputBrokers(outputBrokers2);
        }
        if (ret) {
            brokerSync = inputBrokers1.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            brokerOut1 = outputBrokers1.Get(0);
            ret = brokerOut1.IsValid();
        }
        if (ret) {
            brokerIn2 = inputBrokers2.Get(0);
            ret = brokerIn2.IsValid();
        }
        if (ret) {
            brokerOut2 = outputBrokers2.Get(0);
            ret = brokerOut2.IsValid();
        }
    }
    if (ret) {
        uint64 *mem = (uint64 *) gam1->GetOutputMemoryBuffer();
        uint32 nReads = 1000;
        for (uint32 i = 0u; (i < nReads) && (ret); i++) {
            brokerSync->Execute();
            gam1->Execute();
            brokerOut1->Execute();
            brokerIn2->Execute();
            gam2->Execute();
            brokerOut2->Execute();
            if (ret) {
                ret = (mem[0] == ((2000 * i) + 1));
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error at mem[%u]=%u != %u", i, static_cast<uint32>(mem[0]), ((2000 * i) + 1));
                if (!ret) {
                    //REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error at mem[%d]=%lld\n", i, mem[0]);
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestAsyncRead() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
            "            Signals = {"
            "                ticks_counter = {"
            "                    Type = uint32"
            "                }"
            "                sample_value = {"
            "                    Type = uint16"
            "                }"
            "            }"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }
    if (ret) {
        ret = cdb.MoveAbsolute("Application1.Data.Drv1");
        ret &= cdb.CreateRelative("Signals");
        ret &= cdb.MoveRelative("Signals");
        ret &= cdb.CreateRelative("ticks_counter");
        ret &= cdb.MoveRelative("ticks_counter");
        ret &= cdb.Write("Type", "uint32");
        ret &= cdb.MoveToAncestor(1u);
        ret &= cdb.CreateRelative("sample_value");
        ret &= cdb.MoveRelative("sample_value");
        ret &= cdb.Write("Type", "uint16");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->Reset();
        ret = err.ErrorsCleared();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    uint32 nReads = 30;
    uint32 deltaExpected = 40000;
    uint32 tol = 5000;
    uint32 store;
    StreamString varName = "ticks_counter";
    uint64 varValue = 0u;
    for (uint32 i = 0u; (i < nReads) && (ret); i++) {
        ret = dataSource->AsyncRead(varName, varValue);
        if (i > 0u) {
            uint32 delta = (varValue - store);
            ret = ((delta - deltaExpected) < tol) || ((delta - deltaExpected) > -tol);
        }
        Sleep::Sec(1e-3);
        store = (uint32) varValue;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return true;
}

bool NI9157MxiDataSourceTest::TestAsyncWrite() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
            "            Signals = {"
            "                ticks_counter = {"
            "                    Type = uint32"
            "                }"
            "                sample_value = {"
            "                    Type = uint16"
            "                }"
            "            }"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }
    if (ret) {
        ret = cdb.MoveAbsolute("Application1.Data.Drv1");
        ret &= cdb.CreateRelative("Signals");
        ret &= cdb.MoveRelative("Signals");
        ret &= cdb.CreateRelative("ticks_counter");
        ret &= cdb.MoveRelative("ticks_counter");
        ret &= cdb.Write("Type", "uint32");
        ret &= cdb.MoveToAncestor(1u);
        ret &= cdb.CreateRelative("sample_value");
        ret &= cdb.MoveRelative("sample_value");
        ret &= cdb.Write("Type", "uint16");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->Reset();
        ret = err.ErrorsCleared();
    }
    const uint32 nPoints = 1000;
    float32 mem[nPoints];
    for (uint32 i = 0u; i < nPoints; i++) {
        mem[i] = sin(i * (2 * M_PI / nPoints));
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    if (ret) {
        uint32 nWrites = 10;
        for (uint32 n = 0u; (n < nWrites) && ret; n++) {
            for (uint32 i = 0u; (i < nPoints) && ret; i++) {
                StreamString varName = "sample_value";
                uint32 varValue = (uint32)((mem[i] + 2.5) * (16383 / 5));
                ret = dataSource->AsyncWrite(varName, varValue);
                Sleep::Sec(1e-3);
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestReset() {

    /*static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        ControlBool_stop = 0"
            "        ControlBool_use_dsfifo_data = 0"
            "        ControlBool_use_counter = 1"
            "        ControlU32_cycle_ticks = 200"
            "        ControlU64_packet_size = 1"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM1"
            "            InputSignals = {"
            "                FIFO1_U64_R = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    Frequency = 0"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                ticks_counter = {"
            "                    DataSource = Drv1"
            "                    Type = uint32"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                FIFO0_U64_W = {"
            "                    DataSource = Drv1"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 10000"
            "                }"
            "                sample_value = {"
            "                    DataSource = Drv1"
            "                    Type = uint16"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
            "            NI9157DevicePath = NiDevice"
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
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";*/

    // bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIOConfig0;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    StreamString pathAndFile = "";
    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = application->ConfigureApplication();
    }
    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = dataSource->GetRunNi() == 1;
    }
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->Reset();
        ret = err.ErrorsCleared();
    }
    if (ret) {
        NI9157DeviceOperatorTI ** operators = dataSource->GetNiDevice();
        NI9157DeviceOperatorT<uint64>* x0 = dynamic_cast<NI9157DeviceOperatorT<uint64>*>(operators[0]);
        ret &= x0 != NULL;
        NI9157DeviceOperatorT<uint32>* x1 = dynamic_cast<NI9157DeviceOperatorT<uint32>*>(operators[1]);
        ret &= x1 != NULL;
        NI9157DeviceOperatorT<uint64>* x2 = dynamic_cast<NI9157DeviceOperatorT<uint64>*>(operators[2]);
        ret &= x2 != NULL;
        NI9157DeviceOperatorT<uint16>* x3 = dynamic_cast<NI9157DeviceOperatorT<uint16>*>(operators[3]);
        ret &= x3 != NULL;
    }
    if (ret) {
        uint32 descriptors[] = { 0, 0x8130, 1, 0x8136 };
        uint32 nElementsTest[] = { 10000, 1, 10000, 1 };
        uint32 * ids = dataSource->GetVarId();
        uint32 * nElements = dataSource->GetNumberOfElements();
        uint8 * signalFlags = dataSource->GetSignalFlag();
        uint32 numberOfSignals = dataSource->GetNumberOfSignals();
        ret = numberOfSignals == 4;
        for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
            ret = ids[i] == descriptors[i];
            if (ret) {
                ret = (nElements[i] == nElementsTest[i]);
                if(!ret){
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at Element[%u] with %u != %u", i,  nElements[i], nElementsTest[i]);
                }
            }
        }
    }
    if (ret) {
        ret = dataSource->GetRunNi() == 1;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

/*bool NI9157MxiDataSourceTest::TestSynchronise_Oscilloscope_2MHz() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 20"
            "        NiFpga_TestGTD0001_ControlU32_tcn_cycle_phase = 10000"
            "        NiFpga_TestGTD0001_ControlU32_tcn_period_ticks = 40000"
            "        NiFpga_TestGTD0001_ControlI32_Timeout = 0"
            "        NiFpga_TestGTD0001_ControlU64_packet_size = 1"
            "        NiFpga_TestGTD0001_ControlU64_end_frame = 0xFFFFFFFFFFFFFFFF"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM2"
            "            InputSignals = {"
            "               FIFO = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "                PacketCounter = {"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                MHVPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                AI = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Alasm = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                InternalErrors = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                D0 = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                State = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Reserved = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = IOGAM"
            "            InputSignals = {"
            "                GY1_MHVPS_Current = {"
            "                   DataSource = DDB"
            "                   Type = uint16"
            "                   Ranges = {{1000, 1000}}"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               DAC_value = {"
            "                   DataSource = Drv1"
            "                   Type = uint16"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
    		"            NumberOfPacketsInFifo = 2"
            "            NI9157DevicePath = NiDevice"
            "            Signals = {"
            "                options = {"
            "                    Type = uint8"
            "                }"
            "                options2 = {"
            "                    Type = uint8"
            "                }"
            "            }"
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
            "                    Functions = {GAMA GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    Sleep::Sec(1);

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT < NI9157MxiDataSourceTestGAM2 > gam1;

    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam1.IsValid();
    }

    ReferenceT<IOGAM> gam2;

    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam2.IsValid();
    }

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > brokerOut1;
    ReferenceT < MemoryMapInputBroker > brokerIn2;
    ReferenceT < MemoryMapOutputBroker > brokerOut2;

    if (ret) {
        ReferenceContainer inputBrokers1, outputBrokers1, inputBrokers2, outputBrokers2;
        ret = gam1->GetInputBrokers(inputBrokers1);
        if (ret) {
            ret = gam1->GetOutputBrokers(outputBrokers1);
        }
        if (ret) {
            ret = gam2->GetInputBrokers(inputBrokers2);
        }
        if (ret) {
            ret = gam2->GetOutputBrokers(outputBrokers2);
        }
        if (ret) {
            brokerSync = inputBrokers1.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            brokerOut1 = outputBrokers1.Get(0);
            ret = brokerOut1.IsValid();
        }
        if (ret) {
            brokerIn2 = inputBrokers2.Get(0);
            ret = brokerIn2.IsValid();
        }
        if (ret) {
            brokerOut2 = outputBrokers2.Get(0);
            ret = brokerOut2.IsValid();
        }
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    if (ret) {
        uint64 *mem = (uint64 *) gam1->GetOutputMemoryBuffer();
        uint32 nReads = 10000;
        for (uint32 i = 0u; (i < nReads) && (ret); i++) {

            brokerSync->Execute();

            gam1->Execute();
            brokerOut1->Execute();

            brokerIn2->Execute();
            gam2->Execute();
            brokerOut2->Execute();

            if (ret) {
                ret = (mem[0] == ((2000 * i) + 1));
                if (!ret) {
                    printf("mem[%d]=%lld\n", i, mem[0]);
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}*/

/*bool NI9157MxiDataSourceTest::TestSynchronise_Oscilloscope_OutputFIFO_2MHz() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_NI9159_MultiIO.lvbitx\""
            "    NiRioGenSignature = \"03AB279CA6C34216C3ABAADB90262282\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 20"
            "        NiFpga_TestGTD0001_ControlU32_tcn_cycle_phase = 10000"
            "        NiFpga_TestGTD0001_ControlU32_tcn_period_ticks = 40000"
            "        NiFpga_TestGTD0001_ControlI32_Timeout = 0"
            "        NiFpga_TestGTD0001_ControlU64_packet_size = 1"
            "        NiFpga_TestGTD0001_ControlU64_end_frame = 0xFFFFFFFFFFFFFFFF"
            "    }"
            "}"
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = NI9157MxiDataSourceTestGAM2"
            "            InputSignals = {"
            "               FIFO = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "            }"
            "            OutputSignals = {"
            "                PacketCounter = {"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                MHVPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_MHVPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Voltage = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_BPS_Current = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY1_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                GY2_RF_Detector = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                AI = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Alasm = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                InternalErrors = {"
            "                    Type = uint32"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                D0 = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                State = {"
            "                    Type = uint8"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Reserved = {"
            "                    Type = uint16"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = IOGAM"
            "            InputSignals = {"
            "                GY1_MHVPS_Current = {"
            "                   DataSource = DDB"
            "                   Type = uint16"
            "                   Ranges = {{0, 99} {1900, 1999}}"
            "                }"
            "            }"
            "            OutputSignals = {"
            "               FIFO2 = {"
            "                   DataSource = Drv1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 200"
            "                   Type = uint16"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +DDB = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Drv1 = {"
            "            Class = NI9157MxiDataSourceTestDS"
            "            RunNi = 1"
	    "            NumberOfPacketsInFifo = 2"
            "            NI9157DevicePath = NiDevice"
            "            Signals = {"
            "                options = {"
            "                    Type = uint8"
            "                }"
            "                options2 = {"
            "                    Type = uint8"
            "                }"
            "            }"
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
            "                    Functions = {GAMA GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157MxiDataSourceTestDS> dataSource;

    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    ReferenceT < NI9157MxiDataSourceTestGAM2 > gam1;

    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam1.IsValid();
    }

    ReferenceT<IOGAM> gam2;

    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam2.IsValid();
    }

    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > brokerOut1;
    ReferenceT < MemoryMapInputBroker > brokerIn2;
    ReferenceT < MemoryMapOutputBroker > brokerOut2;

    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    if (ret) {
        ReferenceContainer inputBrokers1, outputBrokers1, inputBrokers2, outputBrokers2;
        ret = gam1->GetInputBrokers(inputBrokers1);
        if (ret) {
            ret = gam1->GetOutputBrokers(outputBrokers1);
        }
        if (ret) {
            ret = gam2->GetInputBrokers(inputBrokers2);
        }
        if (ret) {
            ret = gam2->GetOutputBrokers(outputBrokers2);
        }
        if (ret) {
            brokerSync = inputBrokers1.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            brokerOut1 = outputBrokers1.Get(0);
            ret = brokerOut1.IsValid();
        }
        if (ret) {
            brokerIn2 = inputBrokers2.Get(0);
            ret = brokerIn2.IsValid();
        }
        if (ret) {
            brokerOut2 = outputBrokers2.Get(0);
            ret = brokerOut2.IsValid();
        }
    }
    if (ret) {
        uint64 *mem = (uint64 *) gam1->GetOutputMemoryBuffer();
        uint32 nReads = 10000;
        for (uint32 i = 0u; (i < nReads) && (ret); i++) {

            brokerSync->Execute();

            gam1->Execute();
            brokerOut1->Execute();

            brokerIn2->Execute();
            gam2->Execute();
            brokerOut2->Execute();

            if (ret) {
                ret = (mem[0] == ((2000 * i) + 1));
                if (!ret) {
                    printf("mem[%d]=%lld\n", i, mem[0]);
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}*/
