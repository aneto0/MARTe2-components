/**
 * @file NI9157CircularFifoReaderTest.cpp
 * @brief Source file for class NI9157CircularFifoReaderTest
 * @date 17/05/2018
 * @author Giuseppe Ferrò
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
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

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

    uint32 GetPacketCounter();

    uint32 GetAcquireFromCounter();

    uint32 GetNextPacketCheck();

    uint32 GetCheckCounterAfterNPackets();

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
    return checkPacketCounter;

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

uint32 NI9157CircularFifoReaderTestDS::GetPacketCounter() {
    return packetCounter;
}
uint32 NI9157CircularFifoReaderTestDS::GetAcquireFromCounter() {
    return acquireFromCounter;
}

uint32 NI9157CircularFifoReaderTestDS::GetNextPacketCheck() {
    return nextPacketCheck;
}

uint32 NI9157CircularFifoReaderTestDS::GetCheckCounterAfterNPackets() {
    return checkCounterAfterNSteps;
}

uint32 NI9157CircularFifoReaderTestDS::GetCounterStep() {
    return counterStep;
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

    bool ret = dataSource.GetCheckPacketCounter() == 0u;
    ret &= dataSource.GetNFrameForSync() == 1u;
    ret &= dataSource.GetSampleByteSize() == 0u;
    ret &= dataSource.GetNiDeviceOperator() == NULL;
    ret &= dataSource.GetFifoDev() == 0u;
    ret &= dataSource.GetTotalReadSize() == 0u;
    ret &= dataSource.GetMiddleBuffer() == NULL;
    ret &= dataSource.GetRunNi() == 0u;
    ret &= dataSource.GetPacketCounter() == 1u;
    ret &= dataSource.GetAcquireFromCounter() == 0u;
    ret &= dataSource.GetNextPacketCheck() == 1u;
    ret &= dataSource.GetCheckCounterAfterNPackets() == 0u;
    ret &= dataSource.GetCounterStep() == 1u;
    ret &= dataSource.GetNumberOfPacketsInFIFO() == 10u;

    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 3;
        ret &= dataSource->GetRunNi() == 1;
        ret &= dataSource->GetAcquireFromCounter() == 2000u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 2000u;
        ret &= dataSource->GetCounterStep() == 2000u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 20u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultRunNi() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 3;
        ret &= dataSource->GetRunNi() == 0;
        ret &= dataSource->GetAcquireFromCounter() == 2000u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 2000u;
        ret &= dataSource->GetCounterStep() == 2000u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 20u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultNumberOfPacketsInFIFO() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 3;
        ret &= dataSource->GetRunNi() == 1;
        ret &= dataSource->GetAcquireFromCounter() == 2000u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 2000u;
        ret &= dataSource->GetCounterStep() == 2000u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 10u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultAcquiredFromPacket() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 3;
        ret &= dataSource->GetRunNi() == 1;
        ret &= dataSource->GetAcquireFromCounter() == 0u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 2000u;
        ret &= dataSource->GetCounterStep() == 2000u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 20u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultCheckPacketCounter() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 1;
        ret &= dataSource->GetRunNi() == 1;
        ret &= dataSource->GetAcquireFromCounter() == 0u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 0u;
        ret &= dataSource->GetCounterStep() == 1u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 20u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultCheckCounterAfterNPackets() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 1"
            "            AcquireFromCounter = 0"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 3;
        ret &= dataSource->GetRunNi() == 1;
        ret &= dataSource->GetAcquireFromCounter() == 0u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 1u;
        ret &= dataSource->GetCounterStep() == 1u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 20u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultCounterStep() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 0"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 3;
        ret &= dataSource->GetRunNi() == 1;
        ret &= dataSource->GetAcquireFromCounter() == 0u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 2000u;
        ret &= dataSource->GetCounterStep() == 1u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 20u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_DefaultNFrameForSync() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret &= dataSource->GetNFrameForSync() == 2;
        ret &= dataSource->GetRunNi() == 1;
        ret &= dataSource->GetAcquireFromCounter() == 2000u;
        ret &= dataSource->GetCheckCounterAfterNPackets() == 2000u;
        ret &= dataSource->GetCounterStep() == 2000u;
        ret &= dataSource->GetNumberOfPacketsInFIFO() == 20u;

    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_False_CounterStepZero() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 0"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_False_NFrameForSyncEqualTo1() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 1"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_False_CheckAfterNotDivideCounterStep() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CheckCounterAfterNSteps = 1"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            AcquireFromCounter = 2000"
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
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_False_CounterStepNotDivideAcquireFromCounterDifference() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 3"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_False_NoFifoName() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestInitialise_False_NoNI9157Device() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool NI9157CircularFifoReaderTest::TestInitialise_False_Invalidi9157Device() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase_False_MoreThanOneChannel() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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
    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase_False_NiDevOperatorNotFound() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool NI9157CircularFifoReaderTest::TestSetConfiguredDatabase_False_InvalidFifoName() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"Invalid\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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

    bool ret = !InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157CircularFifoReaderTest::TestPrepareNextState() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
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
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }

    if (ret) {
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
        Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }

        uint32 numberOfReads = 2;

        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();

        uint64 counterStore = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            gam->Execute();
            //printf("counter[%d]=%llu\n", 0, mem[0]);

            for (uint32 j = 0u; (j < 2000) && (ret); j++) {
                //printf("counter[%d]=%llu\n", j, mem[j]);
                if (i * 2000 + j > 0) {
                    ret = ((mem[j] - counterStore) == 1);
                }
                counterStore = mem[j];
            }

        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_AllSignals() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapMultiBufferInputBroker > broker;

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
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

        uint32 numberOfReads = 5;

        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();

        uint64* tsMem = &mem[10000];
        uint32 *errCheckMem = (uint32 *) (&tsMem[1]);

        uint64 counterStore = 0ull;

        uint64 tsStore = HighResolutionTimer::Counter();

        const uint32 expectedDeltaTs = 10000;
        const int32 tol = 100;

        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            //printf("counter[%d]=%llu\n", 0, mem[0]);
            if (i > 0) {
                uint32 delta = (uint32)((*tsMem - tsStore) * HighResolutionTimer::Period() * 1e6);
                //printf("HRT[%d]=%d\n", i, delta);
                ret = (((int32)(delta - expectedDeltaTs)) < tol) || (((int32)(delta - expectedDeltaTs)) > -tol);
            }

            for (uint32 j = 0u; (j < 3u) && (ret); j++) {
                //printf("ErrorCheck[%d, %d]=%d\n", i, j, errCheckMem[j]);
                ret = (errCheckMem[j] == 0);
            }

            tsStore = *tsMem;
            for (uint32 j = 0u; (j < 2000) && (ret); j++) {
                //printf("counter[%d]=%llu\n", j, mem[j]);

                if ((i * 2000 + j) > 0) {
                    ret = ((mem[j] - counterStore) == 1);
                }
                counterStore = mem[j];
            }

        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_AcquiredFromCounter() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 8001"
            "            FirstPacketCounter = 1"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapMultiBufferInputBroker > broker;

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
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

        uint32 numberOfReads = 2;

        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();

        uint64* tsMem = &mem[10000];
        uint32 *errCheckMem = (uint32 *) (&tsMem[1]);

        uint64 counterStore = 0ull;

        uint64 tsStore = HighResolutionTimer::Counter();

        const uint32 expectedDeltaTs = 10000;
        const int32 tol = 100;

        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            //printf("counter[%d]=%llu\n", 0, mem[0]);
            if (i > 0) {
                uint32 delta = (uint32)((*tsMem - tsStore) * HighResolutionTimer::Period() * 1e6);
                //printf("HRT[%d]=%d\n", i, delta);
                ret = (((int32)(delta - expectedDeltaTs)) < tol) || (((int32)(delta - expectedDeltaTs)) > -tol);
            }

            for (uint32 j = 0u; (j < 3u) && (ret); j++) {
                //printf("ErrorCheck[%d, %d]=%d\n", i, j, errCheckMem[j]);
                ret = (errCheckMem[j] == 0);
            }

            tsStore = *tsMem;
            for (uint32 j = 0u; (j < 2000) && (ret); j++) {
                //printf("counter[%d]=%llu\n", j, mem[j]);

                if ((i * 2000 + j) > 0) {
                    ret = ((mem[j] - counterStore) == 1);
                    ret = (mem[j] > 8000);
                }
                counterStore = mem[j];
            }

        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverReadCompleteCycle() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 2"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
            "            sleepInMutexSec = 1e-9"
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
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > outBroker;

    if (ret) {
        ReferenceContainer inputBrokers, outputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
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

        uint32 numberOfReads = 200;

        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();

        uint64 storeCounter = 0ull;
        //uint64 tic=0;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            //tic=HighResolutionTimer::Counter();

            brokerSync->Execute();
            gam->Execute();
            outBroker->Execute();
            if (i > 1) {
                ret = (mem[0] - storeCounter == 2000);
                if (!ret) {
                    printf("mem[%d]=%lld %lld\n", i, mem[0], storeCounter);
                }
            }
            storeCounter = mem[0];
        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_Resync() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;
    ReferenceT < MemoryMapMultiBufferInputBroker > broker;

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            broker = inputBrokers.Get(1);
            ret = broker.IsValid();
        }
        if (ret) {
            dataSource->Anticipate(1);
            ret = dataSource->PrepareNextState("State1", "State1");
        }

        uint32 numberOfReads = 2;
        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();

        uint64* tsMem = &mem[10000];
        uint32 *errCheckMem = (uint32 *) (&tsMem[1]);

        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            broker->Execute();
            gam->Execute();
            if (i == 1u) {
                printf("ErrorCheck[%d]=%d\n", i, errCheckMem[0]);
                ret = (errCheckMem[0] == 8);
            }
            else {
                ret = (errCheckMem[0] == 0);
            }
            //ret = (errCheckMem[j] == 0);
        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_NoCheckCounter() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            FifoName = \"FIFO\""
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }

        uint32 numberOfReads = 200;

        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();

        uint64 counterStore = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            gam->Execute();
            if (i > 0) {
                ret = ((mem[0] - counterStore) == 2000);
                if (!ret) {
                    printf("counter[%d]=%llu %llu\n", i, mem[0], counterStore);
                }
            }
            counterStore = mem[0];
        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_CheckAfterNPackets() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 20"
            "            CpuMask = 8"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 16000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            ret = dataSource->PrepareNextState("State1", "State1");
        }

        uint32 numberOfReads = 200;

        uint64* mem = (uint64*) gam->GetOutputMemoryBuffer();

        uint64 counterStore = 0ull;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            brokerSync->Execute();
            gam->Execute();
            //printf("counter[%d]=%llu\n", 0, mem[0]);

            for (uint32 j = 0u; (j < 2000) && (ret); j++) {
                //printf("counter[%d]=%llu\n", j, mem[j]);
                if (i * 2000 + j > 0) {
                    ret = ((mem[j] - counterStore) == 1);
                }
                counterStore = mem[j];
            }

        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;
}

bool NI9157CircularFifoReaderTest::TestDriverRead_InternalInterleaved() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/DataSources/NI9157/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
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
            "            CheckPacketCounter = 1"
            "            NumberOfPacketsInFIFO = 5"
            "            CpuMask = 16"
            "            FifoName = \"FIFO\""
            "            NumOfFrameForSync = 2"
            "            RunNi = 1"
            "            CounterStep = 2000"
            "            CheckCounterAfterNSteps = 2000"
            "            AcquireFromCounter = 1"
            "            FirstPacketCounter = 1"
            "            SleepInMutexSec = 1e-9"
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
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);

    ReferenceT<NI9157CircularFifoReaderTestDS> dataSource;
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
        ret = dataSource->GetNiDeviceOperator() != NULL;
    }

    if (ret) {
        ret = dataSource->PrepareNextState("State1", "State1");
    }
    ReferenceT < MemoryMapSynchronisedMultiBufferInputBroker > brokerSync;

    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam->GetInputBrokers(inputBrokers);
        if (ret) {
            brokerSync = inputBrokers.Get(0);
            ret = brokerSync.IsValid();
        }

        uint32 numberOfReads = 2000;

        uint64* mem = (uint64*) gam->GetInputMemoryBuffer();

        uint64 storeCounter = 0ull;
        //uint32 max = 0;
        for (uint32 i = 0u; (i < numberOfReads) && (ret); i++) {
            //uint64 tic = HighResolutionTimer::Counter();
            brokerSync->Execute();
            if (i > 1) {
                ret = ((mem[0] - storeCounter) == 2000);
                if (ret) {
                    ret = ((mem[1999] - mem[0]) == 1999);
                }
                if (!ret) {
                    printf("mem[0]=%lld mem[1999]=%lld, storeCounter=%lld, %d\n", mem[0], mem[1999], storeCounter, i);
                    printf("(mem[0] - storeCounter) = %lld\n", (mem[0] - storeCounter));
                    printf("(mem[1999] - mem[0]) = %lld\n", ((mem[1999] - mem[0])));
                }

            }

            /*for(uint32 j=0u; j<2000; j++){
             printf("mem[%d]=%lld\n", j, mem[j]);

             }*/
            storeCounter = mem[0];
        }

        Sleep::MSec(100);
        //Sleep::MSec(100);
        ObjectRegistryDatabase::Instance()->Purge();
    }
    return ret;

}

