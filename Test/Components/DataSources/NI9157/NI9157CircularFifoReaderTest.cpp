/**
 * @file NI9157CircularFifoReaderTest.cpp
 * @brief Source file for class NI9157CircularFifoReaderTest
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
 * the class NI9157CircularFifoReaderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "Interleaved2FlatGAM.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "NI9157CircularFifoReaderTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class NI9157CircularFifoReaderTestDS: public NI9157CircularFifoReader {
public:
    CLASS_REGISTER_DECLARATION()

    NI9157CircularFifoReaderTestDS();
    virtual ~NI9157CircularFifoReaderTestDS();

    uint8 GetCheckPacketCounter();

    uint32 GetNFrameForSync();

    uint32 GetSampleByteSize();

    NI9157DeviceOperatorTI *GetNiDeviceOperator();

    uint32 GetFifoDev();

    uint32 GetTotalReadSize();

    uint8 *GetMiddleBuffer();

    uint8 GetRunNi();

    uint32 GetCounterStep();

    uint32 GetNumberOfPacketsInFIFO();

    void Anticipate(uint8 anticipate);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

protected:
    uint8 anticipateFpgaRun;

};

NI9157CircularFifoReaderTestDS::NI9157CircularFifoReaderTestDS() {
    anticipateFpgaRun = 0u;
}

NI9157CircularFifoReaderTestDS::~NI9157CircularFifoReaderTestDS() {
}

void NI9157CircularFifoReaderTestDS::Anticipate(uint8 anticipate) {
    anticipateFpgaRun = anticipate;
}

bool NI9157CircularFifoReaderTestDS::PrepareNextState(const char8 * const currentStateName,
                                                      const char8 * const nextStateName) {
    bool ret = true;
    if (ret) {
        if (anticipateFpgaRun) {
            if (runNi == 1u) {
                NiFpga_Status status = niDeviceBoard->Run();
                ret = (status == 0);
                runNi = 0u;
                Sleep::Sec(1);
            }
        }
    }
    if (ret) {
        ret = NI9157CircularFifoReader::PrepareNextState(currentStateName, nextStateName);
    }
    return ret;
}

uint8 NI9157CircularFifoReaderTestDS::GetCheckPacketCounter() {
    return checkFrame;
}

uint32 NI9157CircularFifoReaderTestDS::GetNFrameForSync() {
    return nFrameForSync;
}

uint32 NI9157CircularFifoReaderTestDS::GetSampleByteSize() {
    return sampleByteSize;
}

NI9157DeviceOperatorTI *NI9157CircularFifoReaderTestDS::GetNiDeviceOperator() {
    return niDeviceOperator;
}

uint32 NI9157CircularFifoReaderTestDS::GetFifoDev() {
    return fifoDev;
}

uint32 NI9157CircularFifoReaderTestDS::GetTotalReadSize() {
    return totalReadSize;
}

uint8 *NI9157CircularFifoReaderTestDS::GetMiddleBuffer() {
    return middleBuffer;
}

uint8 NI9157CircularFifoReaderTestDS::GetRunNi() {
    return runNi;
}

uint32 NI9157CircularFifoReaderTestDS::GetNumberOfPacketsInFIFO() {
    return numberOfPacketsInFIFO;
}
CLASS_REGISTER(NI9157CircularFifoReaderTestDS, "1.0")

class NI9157CircularFifoReaderTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    NI9157CircularFifoReaderTestGAM1 ();

    virtual bool Setup();

    virtual bool Execute();
    void *GetInputMemoryBuffer();

    void *GetOutputMemoryBuffer();
private:
    uint32 totalSize;
};

NI9157CircularFifoReaderTestGAM1::NI9157CircularFifoReaderTestGAM1() :
        GAM() {
    totalSize = 0u;
}

bool NI9157CircularFifoReaderTestGAM1::Setup() {

    return true;
}

bool NI9157CircularFifoReaderTestGAM1::Execute() {
    return true;
}

void *NI9157CircularFifoReaderTestGAM1::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *NI9157CircularFifoReaderTestGAM1::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(NI9157CircularFifoReaderTestGAM1, "1.0")

class NI9157CircularFifoReaderTestGAM2: public Interleaved2FlatGAM {
public:
    CLASS_REGISTER_DECLARATION()

    NI9157CircularFifoReaderTestGAM2 ();

    void *GetInputMemoryBuffer();

    void *GetOutputMemoryBuffer();
private:
};

NI9157CircularFifoReaderTestGAM2::NI9157CircularFifoReaderTestGAM2() :
        Interleaved2FlatGAM() {
}

void *NI9157CircularFifoReaderTestGAM2::GetInputMemoryBuffer() {
    return GAM::GetInputSignalsMemory();
}

void *NI9157CircularFifoReaderTestGAM2::GetOutputMemoryBuffer() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(NI9157CircularFifoReaderTestGAM2, "1.0")

static const uint32 nParams                 = 3u;
static const char8 * const firmwarePath     = "Test/Components/Interfaces/NI9157Device/TestLabviewFiles";
static const char8 * const multiIOFirmware[]= {"RIO0", "NiFpga_NI9159_MultiIO.lvbitx", "03AB279CA6C34216C3ABAADB90262282"};

static const char8 * const multiIOConfig0 = "";
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
    "            Class = NI9157CircularFifoReaderTestGAM1"
    // "            TcnTimeStamp = {"
    "            InputSignals = {"
    "               Signal1 = {"
    "                   DataSource = Drv1"
    "                   Type = uint64"
    "                   Samples = 1"
    "                   Frequency = 0"
    "               }"
    "            }"
    "        }"
    "    }"
    "    +Data = {"
    "        Class = ReferenceContainer"
    "        +Drv1 = {"
    "            Class = NI9157CircularFifoReaderTestDS"
    "            NI9157DevicePath = NiDevice"
    "            NumberOfBuffers = 80"
    "            NumberOfPacketsInFIFO = 20"
    "            CheckFrame = 1"
    "            RunNi = 1"
    "            CpuMask = 8"
    "            FifoName = \"FIFO1_U64_R\""
    "            +Checker = {"
    "                Class = NI9157::CounterChecker"
    "                NumOfFrameForSync = 3"
    "                CounterStep = 2000"
    "                CheckCounterAfterNSteps = 2000"
    "                AcquireFromCounter = 2001"
    "                SampleSize = 8"
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
    "}";

static const char8 * const multiIOConfig1 = ""
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
    "            Class = NI9157CircularFifoReaderTestGAM2"
    "            InputSignals = {"
    "               FIFO1_U64_R = {"
    "                   DataSource = Drv1"
    "                   Type = uint64"
    "                   Samples = 1"
    "                   NumberOfDimensions = 1"
    "                   NumberOfElements = 10000"
    "                   Frequency = 1000"
    "                   PacketMemberSizes = {8, 8, 8, 8, 8}"
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
    "                Signal2_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal3_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal4_U64 = {"
    "                    Type = uint64"
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
    "            Class = NI9157CircularFifoReaderTestDS"
    "            NI9157DevicePath = NiDevice"
    "            NumberOfBuffers = 80"
    "            NumberOfPacketsInFIFO = 20"
    "            CheckFrame = 1"
    "            RunNi = 1"
    "            CpuMask = 8"
    "            FifoName = \"FIFO1_U64_R\""
    "            +Checker = {"
    "                Class = NI9157::CounterChecker"
    "                NumOfFrameForSync = 2"
    "                CounterStep = 2000"
    "                CheckCounterAfterNSteps = 2000"
    "                AcquireFromCounter = 1"
    "                FirstPacketCounter = 1"
    "                SampleSize = 8"
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
    "}";

static const char8 * const multiIOConfig2 = ""
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
    "            Class = NI9157CircularFifoReaderTestGAM2"
    "            InputSignals = {"
    "               FIFO1_U64_R = {"
    "                   DataSource = Drv1"
    "                   Type = uint64"
    "                   Samples = 1"
    "                   NumberOfDimensions = 1"
    "                   NumberOfElements = 10000"
    "                   Frequency = 1000"
    "                   PacketMemberSizes = {8, 8, 8, 8, 8}"
    "               }"
    "               InternalTimeStamp = {"
    "                   DataSource = Drv1"
    "                   Type = uint64"
    "                   Samples = 1"
    "               }"
    "               ErrorCheck = {"
    "                   DataSource = Drv1"
    "                   Type = uint32"
    "                   Samples = 3"
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
    "                Signal2_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal3_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                Signal4_U64 = {"
    "                    Type = uint64"
    "                    NumberOfDimensions = 1"
    "                    NumberOfElements = 2000"
    "                    DataSource = DDB"
    "                }"
    "                InternalTimeStampDDB = {"
    "                   DataSource = DDB"
    "                   Type = uint64"
    "                }"
    "                ErrorCheckDDB = {"
    "                   DataSource = DDB"
    "                   Type = uint32"
    "                   NumberOfDimensions = 1"
    "                   NumberOfElements = 3"
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
    "            Class = NI9157CircularFifoReaderTestDS"
    "            NI9157DevicePath = NiDevice"
    "            NumberOfBuffers = 80"
    "            NumberOfPacketsInFIFO = 20"
    "            CheckFrame = 1"
    "            RunNi = 1"
    "            CpuMask = 8"
    "            FifoName = \"FIFO1_U64_R\""
    "            +Checker = {"
    "                Class = NI9157::CounterChecker"
    "                NumOfFrameForSync = 2"
    "                CounterStep = 2000"
    "                CheckCounterAfterNSteps = 2000"
    "                AcquireFromCounter = 1"
    "                FirstPacketCounter = 1"
    "                SampleSize = 8"
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
    "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
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
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    return ok;
}

NI9157CircularFifoReaderTest::NI9157CircularFifoReaderTest() {
}

NI9157CircularFifoReaderTest::~NI9157CircularFifoReaderTest() {
}

bool NI9157CircularFifoReaderTest::TestConstructor() {

    NI9157CircularFifoReaderTestDS dataSource;

    bool ret = (dataSource.GetCheckPacketCounter() == 0u);
    ret &= (dataSource.GetNFrameForSync() == 1u);
    ret &= (dataSource.GetSampleByteSize() == 0u);
    ret &= (dataSource.GetNiDeviceOperator() == NULL_PTR(NI9157CircularFifoReaderTestDS *));
    ret &= (dataSource.GetFifoDev() == 0u);
    ret &= (dataSource.GetTotalReadSize() == 0u);
    ret &= (dataSource.GetMiddleBuffer() == NULL_PTR(NI9157CircularFifoReaderTestDS *));
    ret &= (dataSource.GetRunNi() == 0u);
    ret &= (dataSource.GetNumberOfPacketsInFIFO() == 10u);

    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise(uint32 model) {

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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CheckFrame = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2001"
            "                SampleSize = 8"
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 3);
        ret &= (dataSource->GetRunNi() == 1);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 20u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultRunNi(uint32 model) {

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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2001"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Delete("RunNi");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 3);
        ret &= (dataSource->GetRunNi() == 0);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 20u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultNumberOfPacketsInFIFO(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2001"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Delete("NumberOfPacketsInFIFO");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 3u);
        ret &= (dataSource->GetRunNi() == 1u);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 10u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultAcquiredFromPacket(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Delete("AcquireFromCounter");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 3u);
        ret &= (dataSource->GetRunNi() == 1u);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 20u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultCheckPacketCounter(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2001"
            "                SampleSize = 8"
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
            "}"; */

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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Delete("CheckFrame");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 1u);
        ret &= (dataSource->GetRunNi() == 1u);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 20u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultCheckCounterAfterNPackets(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            RunNi = 1"
            "            FifoName = \"FIFO1_U64_R\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 1"
            "                AcquireFromCounter = 0"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Delete("CheckFrame");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 3u);
        ret &= (dataSource->GetRunNi() == 1u);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 20u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultCounterStep(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 0"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Delete("CounterStep");
        ret &= cdb.Write("AcquireFromCounter", 0);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 3u);
        ret &= (dataSource->GetRunNi() == 1u);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 20u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultNFrameForSync(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2001"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Delete("NumOfFrameForSync");
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNFrameForSync() == 1u);
        ret &= (dataSource->GetRunNi() == 1u);
        ret &= (dataSource->GetNumberOfPacketsInFIFO() == 20u);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_False_CounterStepZero(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 0"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("CounterStep", 0);
        ret &= cdb.Write("AcquireFromCounter", 2000);
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

bool NI9157CircularFifoReaderTest::TestInitialise_False_NFrameForSyncEqualTo1(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 1"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2000"
            "                SampleSize = 8"
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
            "}"; */

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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("NumOfFrameForSync", 1);
        ret &= cdb.Write("AcquireFromCounter", 2000);
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

bool NI9157CircularFifoReaderTest::TestInitialise_False_CheckAfterNotDivideCounterStep(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CheckCounterAfterNSteps = 1"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 3"
            "                CounterStep = 2000"
            "                AcquireFromCounter = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("CheckCounterAfterNSteps", 1);
        ret &= cdb.Write("AcquireFromCounter", 2000);
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

bool NI9157CircularFifoReaderTest::TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 2"
            "                NumOfFrameForSync = 3"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("AcquireFromCounter", 2);
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

bool NI9157CircularFifoReaderTest::TestInitialise_False_NoFifoName(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            RunNi = 1"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Delete("FifoName");
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

bool NI9157CircularFifoReaderTest::TestInitialise_False_NoNI9157Device(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            RunNi = 1"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            NumOfFrameForSync = 2"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
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

bool NI9157CircularFifoReaderTest::TestInitialise_False_Invalidi9157Device(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = Boh"
            "            NumberOfBuffers = 80"
            "            RunNi = 1"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Write("NI9157DevicePath", "SomeDevicePath");
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

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            RunNi = 1"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Write("NumOfFrameForSync", 2);
        ret &= cdb.Delete("AcquireFromCounter");
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase_False_MoreThanOneChannel(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            RunNi = 1"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals");
        ret &= cdb.CreateRelative("Signal2");
        ret &= cdb.MoveRelative("Signal2");
        ret &= cdb.Write("DataSource", "Drv1");
        ret &= cdb.Write("Type", "uint64");
        ret &= cdb.Write("Samples", 1);
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

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase_False_NiDevOperatorNotFound(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = float32"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            RunNi = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals.Signal1");
        ret &= cdb.Write("Type", "float32");
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

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase_False_InvalidFifoName(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            TcnTimeStamp = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   Frequency = 0"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            RunNi = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"Invalid\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Write("FifoName", "\"SomeFIFO\"");
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

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestPrepareNextState(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            RunNi = 1"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                SampleSize = 8"
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
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals.Signal1");
        ret &= cdb.Write("NumberOfDimensions", 1);
        ret &= cdb.Write("NumberOfElements", 10000);
        ret &= cdb.Write("Frequency", 1000);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("NumOfFrameForSync", 2);
        ret &= cdb.Delete("AcquireFromCounter");
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
        Sleep::MSec(100);
    }
    
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestStopAcquisition(uint32 model) {

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
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals.Signal1");
        ret &= cdb.Write("NumberOfDimensions", 1);
        ret &= cdb.Write("NumberOfElements", 10000);
        ret &= cdb.Write("Frequency", 1000);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("NumOfFrameForSync", 2);
        ret &= cdb.Delete("AcquireFromCounter");
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestStartAcquisition(uint32 model) {

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
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals.Signal1");
        ret &= cdb.Write("NumberOfDimensions", 1);
        ret &= cdb.Write("NumberOfElements", 10000);
        ret &= cdb.Write("Frequency", 1000);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("NumOfFrameForSync", 2);
        ret &= cdb.Delete("AcquireFromCounter");
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestSynchronise(uint32 model) {

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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        brokerSync->Execute();
        gam->Execute();
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM2"
            "            InputSignals = {"
            "               FIFO1_U64_R = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
            "                   PacketMemberSizes = {8, 8, 8, 8, 8}"
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
            "                Signal2_U64 = {"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Signal3_U64 = {"
            "                    Type = uint64"
            "                    NumberOfDimensions = 1"
            "                    NumberOfElements = 2000"
            "                    DataSource = DDB"
            "                }"
            "                Signal4_U64 = {"
            "                    Type = uint64"
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
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 0x1"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 1"
            "                FirstPacketCounter = 1"
            "                SampleSize = 8"
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 2u;
        uint32 numberOfValues = 2000u;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();
        uint64 counterStore = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            gam->Execute();
            for (uint32 j = 0u; (j < numberOfValues) && (ret); j++) {
                if (j > 0) { 
                    ret = ((mem[j] - counterStore) == 1u);
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at mem[%u]=%u != mem[%u]=%u + 1", j , static_cast<uint32>(mem[j]), j-1, static_cast<uint32>(counterStore));
                    }
                }
                counterStore = mem[j];
            }
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret &= err.ErrorsCleared();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_AllSignals(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM2"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
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
            "                InternalTimeStampDDB = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "                }"
            "                ErrorCheckDDB = {"
            "                   DataSource = DDB"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
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
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 1"
            "                FirstPacketCounter = 1"
            "                SampleSize = 8"
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapMultiBufferInputBroker > broker;
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        broker = inputBrokers.Get(1);
        ret = broker.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 5u;
        uint32 numberOfErrorChecks = 3u;
        uint32 numberOfValues = 2000u;
        const uint32 expectedDeltaTs = 10000;
        const int32 tol = 100;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();
        uint64* tsMem = &mem[10000];
        uint32 *errCheckMem = (uint32 *) (&tsMem[1]);
        uint64 counterStore = 0ull;
        uint64 tsStore = HighResolutionTimer::Counter();
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            if (i > 0) {
                uint32 delta = (uint32)((*tsMem - tsStore) * HighResolutionTimer::Period() * 1e6);
                ret = (((int32)(delta - expectedDeltaTs)) < tol) || (((int32)(delta - expectedDeltaTs)) > -tol);
            }
            for (uint32 j = 0u; (j < numberOfErrorChecks) && (ret); j++) {
                ret = (errCheckMem[j] == 0);
            }
            tsStore = *tsMem;
            for (uint32 j = 0u; (j < numberOfValues) && (ret); j++) {
                if ((i * 2000 + j) > 0) {
                    ret = ((mem[j] - counterStore) == 1);
                }
                counterStore = mem[j];
            }
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret &= err.ErrorsCleared();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_AcquiredFromCounter(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM2"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
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
            "                InternalTimeStampDDB = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "                }"
            "                ErrorCheckDDB = {"
            "                   DataSource = DDB"
            "                   Type = uint32"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 3"
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
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            //"            RunNi = 1"
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 1"    //8001
            "                FirstPacketCounter = 1"
            "                SampleSize = 8"
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapMultiBufferInputBroker > broker;
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        broker = inputBrokers.Get(1);
        ret = broker.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 2u;
        uint32 numberOfErrorChecks = 3u;
        uint32 numberOfValues = 2000u;
        const uint32 expectedDeltaTs = 10000;
        const int32 tol = 100;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();
        uint64* tsMem = &mem[10000];
        uint32 *errCheckMem = (uint32 *) (&tsMem[1]);
        uint64 counterStore = 0ull;
        uint64 tsStore = HighResolutionTimer::Counter();
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            if (i > 0) {
                uint32 delta = (uint32)((*tsMem - tsStore) * HighResolutionTimer::Period() * 1e6);
                ret = (((int32)(delta - expectedDeltaTs)) < tol) || (((int32)(delta - expectedDeltaTs)) > -tol);
            }
            for (uint32 j = 0u; (j < numberOfErrorChecks) && (ret); j++) {
                ret = (errCheckMem[j] == 0);
            }
            tsStore = *tsMem;
            for (uint32 j = 0u; (j < numberOfValues) && (ret); j++) {
                //if ((i * 2000 + j) > 0) {
                if (j > 0) {
                    ret = ((mem[j] - counterStore) == 1);
                    //ret = (mem[j] > 8000);
                }
                counterStore = mem[j];
            }
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret &= err.ErrorsCleared();
        Sleep::MSec(100);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverReadCompleteCycle(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM2"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
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
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 2"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            sleepInMutexSec = 1e-9"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 1"
            "                FirstPacketCounter = 1"
            "                SampleSize = 8"
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
            "                    RunOnCpu = 1"
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
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Write("sleepInMutexSec", 1e-9);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
        ret &= cdb.Write("RunOnCpu", 1);
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > outBroker;
    ReferenceContainer inputBrokers, outputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    if (ret) {
        ret = gam->GetOutputBrokers(outputBrokers);
    }
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        outBroker = outputBrokers.Get(0);
        ret = outBroker.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 200;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();
        uint64 storeCounter = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            gam->Execute();
            outBroker->Execute();
            if (i > 1) {
                ret = (mem[0] - storeCounter == 2000);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at mem[%u]=%u with storeCounter %u", i, static_cast<uint32>(mem[0]), static_cast<uint32>(storeCounter));
                }
            }
            storeCounter = mem[0];
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret &= err.ErrorsCleared();
    }
    
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_Resync(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM2"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
            "               InternalTimeStamp = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "               }"
            "               ErrorCheck = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
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
            "                InternalTimeStampDDB = {"
            "                   DataSource = DDB"
            "                   Type = uint64"
            "                }"
            "                ErrorCheckDDB = {"
            "                   DataSource = DDB"
            "                   Type = uint32"
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
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 1"
            "                FirstPacketCounter = 1"
            "                SampleSize = 8"
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
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals.ErrorCheck");
        ret &= cdb.Delete("Samples");
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Functions.+GAMA.OutputSignals.ErrorCheckDDB");
        ret &= cdb.Delete("NumberOfDimensions");
        ret &= cdb.Delete("NumberOfElements");
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapMultiBufferInputBroker > broker;
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        broker = inputBrokers.Get(1);
        ret = broker.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 2;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();
        uint64* tsMem = &mem[10000];
        uint32 *errCheckMem = (uint32 *) (&tsMem[1]);
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            if (i == 1u) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at ErrorCheck[%u]=%u", i, static_cast<uint32>(errCheckMem[0]));
                //ret = (errCheckMem[0] == 8);
                ret = (errCheckMem[0] == 0);
            }
            else {
                ret = (errCheckMem[0] == 0);
            }
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret &= err.ErrorsCleared();
    }
    
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_NoCheckCounter(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM2"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
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
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            RunNi = 1"
            "            FifoName = \"FIFO1_U64_R\""
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
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Delete("CheckFrame");
        ret &= cdb.Delete("+Checker");
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 200;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();
        uint64 counterStore = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            gam->Execute();
            if (i > 0) {
                ret = ((mem[0] - counterStore) == 2000);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at counter[%u]=%u with counterStore=%u", i, static_cast<uint32>(mem[0]), static_cast<uint32>(counterStore));
                }
            }
            counterStore = mem[0];
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_CheckAfterNPackets(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM2"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
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
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 16000"
            "                AcquireFromCounter = 1"
            "                FirstPacketCounter = 1"
            "                SampleSize = 8"
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
    if (ret) {
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.+Checker");
        ret &= cdb.Write("CheckCounterAfterNSteps", 16000);
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM2 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 200;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();
        uint64 counterStore = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            gam->Execute();
            for (uint32 j = 0u; (j < 2000) && (ret); j++) {
                if (i * 2000 + j > 0) {
                    ret = ((mem[j] - counterStore) == 1);
                }
                counterStore = mem[j];
            }
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_InternalInterleaved(uint32 model) {

/*    static const char8 * const config = ""
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
            "            Class = NI9157CircularFifoReaderTestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   Samples = 1"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   Frequency = 1000"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = NI9157CircularFifoReaderTestDS"
            "            NI9157DevicePath = NiDevice"
            "            NumberOfBuffers = 80"
            "            CheckFrame = 1"
            "            NumberOfPacketsInFIFO = 5"
            "            CpuMask = 16"
            "            FifoName = \"FIFO1_U64_R\""
            "            RunNi = 1"
            "            SleepInMutexSec = 1e-9"
            "            +Checker = {"
            "                Class = NI9157::CounterChecker"
            "                NumOfFrameForSync = 2"
            "                CounterStep = 2000"
            "                CheckCounterAfterNSteps = 2000"
            "                AcquireFromCounter = 1"
            "                FirstPacketCounter = 1"
            "                SampleSize = 8"
            "            }"
            "            Signals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint64"
            "                   NumberOfDimensions = 1"
            "                   NumberOfElements = 10000"
            "                   PacketMemberSizes = {8, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4, 1, 1, 2}"
            "               }"
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
            "                    RunOnCpu = 1"
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
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals.Signal1");
        ret &= cdb.Write("NumberOfDimensions", 1);
        ret &= cdb.Write("NumberOfElements", 10000);
        ret &= cdb.Write("Frequency", 1000);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Write("NumberOfPacketsInFIFO", 5);
        ret &= cdb.Write("SleepInMutexSec", 1e-9);
        ret &= cdb.MoveRelative("+Checker");
        ret &= cdb.Write("AcquireFromCounter", 1);
        ret &= cdb.Delete("FirstPacketCounter");
        ret &= cdb.MoveToRoot();
        ret &= cdb.CreateRelative("Signals");
        ret &= cdb.MoveRelative("Signals");
        ret &= cdb.CreateRelative("Signal1");
        ret &= cdb.MoveRelative("Signal1");
        ret &= cdb.Write("DataSource", "Drv1");
        ret &= cdb.Write("Type", "uint64");
        ret &= cdb.Write("NumberOfDimensions", 1);
        ret &= cdb.Write("NumberOfElements", 10000);
        ret &= cdb.Write("PacketMemberSizes", "{8, 8, 8, 8, 8}");
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+States.+State1.+Threads.+Thread1");
        ret &= cdb.Write("RunOnCpu", 1);
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
    ReferenceT < NI9157CircularFifoReaderTestDS > dataSource;
    ReferenceT < NI9157CircularFifoReaderTestGAM1 > gam;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNiDeviceOperator() != NULL_PTR(NI9157CircularFifoReaderTestDS *));
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceContainer inputBrokers;
    if (ret) {
        ret = gam->GetInputBrokers(inputBrokers);
    }
    if (ret) {
        brokerSync = inputBrokers.Get(0);
        ret = brokerSync.IsValid();
    }
    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        ErrorManagement::ErrorType err;
        err = dataSource->StartAcquisition();
        ret = err.ErrorsCleared();
    }
    Sleep::MSec(100);
    if (ret) {
        uint32 numberOfReads = 2000;
        uint64* mem = (uint64*) gam->GetInputMemoryBuffer();
        uint64 storeCounter = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            if (i > 1) {
                ret = ((mem[0] - storeCounter) == 2000);
                if (ret) {
                    ret = ((mem[1999] - mem[0]) == 1999);
                }
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at mem[0]=%u, mem[1999]=%u, storeCounter=%u, %u", static_cast<uint32>(mem[0]), static_cast<uint32>(mem[1999]), static_cast<uint32>(storeCounter), i );
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at (mem[0] - storeCounter)=%d", static_cast<int32>( (mem[0] - storeCounter) ));
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed at (mem[1999]-mem[0])=%d", static_cast<int32>( (mem[1999] - mem[0]) ));
                }
            }
            storeCounter = mem[0];
        }
        Sleep::MSec(100);
        ErrorManagement::ErrorType err;
        err = dataSource->StopAcquisition();
        ret &= err.ErrorsCleared();
    }
    
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}
