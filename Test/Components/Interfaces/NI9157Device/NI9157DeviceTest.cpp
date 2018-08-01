/**
 * @file NI9157DeviceTest.cpp
 * @brief Source file for class NI9157DeviceTest
 * @date 22/05/2018
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
 * the class NI9157DeviceTest (public, protected, and private). Be aware that some 
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
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "NI9157DeviceTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class NI9157DeviceTestIF: public NI9157Device {

public:
    CLASS_REGISTER_DECLARATION()

    NiFpga_Status GetStatus();

    const char8 *GetNiRioGenFile();
    const char8 *GetNiRioGenSignature();

    const char8 *GetNiRioDeviceName();
};

NiFpga_Status NI9157DeviceTestIF::GetStatus() {
    return status;
}

const char8 *NI9157DeviceTestIF::GetNiRioGenFile() {
    return niRioGenFile.Buffer();
}

const char8 *NI9157DeviceTestIF::GetNiRioGenSignature() {
    return niRioGenSignature.Buffer();
}

const char8 *NI9157DeviceTestIF::GetNiRioDeviceName() {
    return niRioDeviceName.Buffer();
}

CLASS_REGISTER(NI9157DeviceTestIF, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

NI9157DeviceTest::NI9157DeviceTest() {
}

NI9157DeviceTest::~NI9157DeviceTest() {
}

bool NI9157DeviceTest::TestConstructor() {
    NI9157DeviceTestIF interface;
    bool ret = interface.IsOpened() == 0;
    ret &= interface.IsRunning() == 0;
    ret &= StringHelper::Compare(interface.GetNiRioGenFile(), "") == 0;
    ret &= StringHelper::Compare(interface.GetNiRioGenSignature(), "") == 0;
    ret &= StringHelper::Compare(interface.GetNiRioDeviceName(), "") == 0;
    return ret;
}

bool NI9157DeviceTest::TestInitialise() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsOpened() == 0;
        ret &= interface->IsRunning() == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenFile(), "Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx") == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), "6275BF517CD273AA4FDA8E4E3B0D7378") == 0;
        ret &= StringHelper::Compare(interface->GetNiRioDeviceName(), "RIO0") == 0;
    }
    return ret;

}

bool NI9157DeviceTest::TestInitialiseIsOpened() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsOpened() == 1;
        ret &= interface->IsRunning() == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenFile(), "Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx") == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), "6275BF517CD273AA4FDA8E4E3B0D7378") == 0;
        ret &= StringHelper::Compare(interface->GetNiRioDeviceName(), "RIO0") == 0;
    }
    return ret;

}

bool NI9157DeviceTest::TestInitialiseRandomConfig() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU64_packet_size = 1"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
            "        NiFpga_TestGTD0001_ControlI32_Timeout = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
            "        NiFpga_TestGTD0001_ControlU32_tcn_cycle_phase = 10000"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU32_tcn_period_ticks = 40000"
            "        NiFpga_TestGTD0001_ControlU64_end_frame = 0xFFFFFFFFFFFFFFFF"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsOpened() == 0;
        ret &= interface->IsRunning() == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenFile(), "Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx") == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), "6275BF517CD273AA4FDA8E4E3B0D7378") == 0;
        ret &= StringHelper::Compare(interface->GetNiRioDeviceName(), "RIO0") == 0;
    }
    return ret;

}

bool NI9157DeviceTest::TestInitialise_FalseNoDeviceName() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU64_packet_size = 1"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
            "        NiFpga_TestGTD0001_ControlI32_Timeout = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
            "        NiFpga_TestGTD0001_ControlU32_tcn_cycle_phase = 10000"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU32_tcn_period_ticks = 40000"
            "        NiFpga_TestGTD0001_ControlU64_end_frame = 0xFFFFFFFFFFFFFFFF"
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
    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoGenFile() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU64_packet_size = 1"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
            "        NiFpga_TestGTD0001_ControlI32_Timeout = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
            "        NiFpga_TestGTD0001_ControlU32_tcn_cycle_phase = 10000"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU32_tcn_period_ticks = 40000"
            "        NiFpga_TestGTD0001_ControlU64_end_frame = 0xFFFFFFFFFFFFFFFF"
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
    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoGenSignature() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU16_maxV = 5"
            "        NiFpga_TestGTD0001_ControlU32_cycleTimeDAC_ticks = 1"
            "        NiFpga_TestGTD0001_ControlU64_packet_size = 1"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
            "        NiFpga_TestGTD0001_ControlI32_Timeout = 0"
            "        NiFpga_TestGTD0001_ControlBool_use_RT_MXI = 1"
            "        NiFpga_TestGTD0001_ControlU16_DacResolution = 16383"
            "        NiFpga_TestGTD0001_ControlU32_cycle_ticks = 200"
            "        NiFpga_TestGTD0001_ControlU32_tcn_cycle_phase = 10000"
            "        NiFpga_TestGTD0001_ControlBool_use_counter = 1"
            "        NiFpga_TestGTD0001_ControlU32_tcn_period_ticks = 40000"
            "        NiFpga_TestGTD0001_ControlU64_end_frame = 0xFFFFFFFFFFFFFFFF"
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
    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoType() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    Open = 1"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_Control_options = 2"
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
    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseVariableNotFound() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    Open = 1"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options_boh = 2"
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
    return ret;
}

bool NI9157DeviceTest::TestOpen() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsOpened() == 0;
    }

    if (ret) {
        ret = interface->Open() == 0;
    }
    if (ret) {
        ret = interface->IsOpened() == 1;
    }

    return ret;
}
bool NI9157DeviceTest::TestRun() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsRunning() == 0;
    }

    if (ret) {
        ret = interface->Open() == 0;
    }
    if (ret) {
        ret = interface->Run() == 0;
    }
    if (ret) {
        ret = interface->IsRunning() == 1;
    }

    return ret;
}

bool NI9157DeviceTest::TestIsOpened() {
    return TestOpen();
}
bool NI9157DeviceTest::TestIsRunning() {
    return TestRun();
}

bool NI9157DeviceTest::TestGetSession() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsOpened() == 0;
    }

    if (ret) {
        ret = interface->Open() == 0;
    }
    if (ret) {
        NiFpga_Session session = interface->GetSession();
        NiFpga_Status status;
        NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));

        ret = status == 0;
    }
    if (ret) {
        ret = interface->Run() != 0;
    }
    if (ret) {
        ret = interface->Reset() == 0;
    }
    if (ret) {
        ret = interface->Close() == 0;
    }
    return ret;
}

bool NI9157DeviceTest::TestReset() {
    return TestGetSession();
}

bool NI9157DeviceTest::TestClose() {
    return TestGetSession();
}

bool NI9157DeviceTest::TestFindResource() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsOpened() == 0;
    }

    if (ret) {
        ret = interface->Open() == 0;
    }
    if (ret) {
        ret = interface->IsOpened() == 1;
    }

    if (ret) {

        //uint8 typeU8 = 0;
        uint32 varDescriptor;
        //ret &= (interface->FindResource("IndU8IndU8", typeU8, varDescriptor) == 0);

        uint16 typeU16 = 0;
        ret &= (interface->FindResource("IndU16", typeU16, varDescriptor) == 0);

        uint32 typeU32 = 0;
        ret &= (interface->FindResource("IndU32", typeU32, varDescriptor) == 0);

        uint64 typeU64 = 0;
        ret &= (interface->FindResource("IndU64", typeU64, varDescriptor) == 0);

        int8 typeI8 = 0;
        ret &= (interface->FindResource("IndI8", typeI8, varDescriptor) == 0);

        int16 typeI16 = 0;
        ret &= (interface->FindResource("IndI16", typeI16, varDescriptor) == 0);

        int32 typeI32 = 0;
        ret &= (interface->FindResource("IndI32", typeI32, varDescriptor) == 0);

        int64 typeI64 = 0;
        ret &= (interface->FindResource("IndI64", typeI64, varDescriptor) == 0);

    }

    return ret;
}

bool NI9157DeviceTest::TestNiRead() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"6275BF517CD273AA4FDA8E4E3B0D7378\""
            "    Configuration = {"
            "        NiFpga_TestGTD0001_ControlU8_options = 2"
            "        NiFpga_TestGTD0001_ControlU8_options2 = 2"
            "        NiFpga_TestGTD0001_ControlBool_stop = 0"
            "        NiFpga_TestGTD0001_ControlBool_stop2 = 0"
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

    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }

    if (ret) {
        ret = interface->IsOpened() == 0;
    }

    if (ret) {
        ret = interface->Open() == 0;
    }
    if (ret) {
        ret = interface->IsOpened() == 1;
    }

    if (ret) {
        ret = interface->Run() == 0;
    }
    if (ret) {
        ret = interface->IsRunning() == 1;
    }

    if (ret) {

        //uint8 typeU8 = 0;

        uint32 contrDescriptor;
        uint32 varDescriptor;
        //ret &= (interface->FindResource("IndU8IndU8", typeU8, varDescriptor) == 0);

        uint16 typeU16 = 0;
        ret &= (interface->FindResource("IndU16", typeU16, varDescriptor) == 0);
        ret &= (interface->FindResource("ContU16", typeU16, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, 2) == 0);
        uint16 valueU16;
        ret &= (interface->NiRead(varDescriptor, valueU16) == 0);
        ret &= (valueU16 == 2);

        uint32 typeU32 = 0;
        ret &= (interface->FindResource("IndU32", typeU32, contrDescriptor) == 0);
        ret &= (interface->FindResource("ContU32", typeU32, varDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, 3) == 0);
        uint32 valueU32;
        ret &= (interface->NiRead(varDescriptor, valueU32) == 0);
        ret &= valueU32 == 3;

        uint64 typeU64 = 0;
        ret &= (interface->FindResource("IndU64", typeU64, varDescriptor) == 0);
        ret &= (interface->FindResource("ContU64", typeU64, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, 4) == 0);
        uint64 valueU64;
        ret &= (interface->NiRead(varDescriptor, valueU64) == 0);
        ret &= valueU64 == 4;

        /*
         int8 typeI8 = 0;
         ret &= (interface->FindResource("IndI8", typeI8, varDescriptor) == 0);
         ret &= (interface->FindResource("ContrI8", typeU16, varDescriptor) == 0);

         ret&=(interface->NiWrite(varDescriptor, -1)==0);
         int8 valueI8;
         ret&=(interface->NiRead(varDescriptor, valueI8)==0);
         ret&=valueI8==-1;
         */

        int16 typeI16 = 0;
        ret &= (interface->FindResource("IndI16", typeI16, varDescriptor) == 0);
        ret &= (interface->FindResource("ContI16", typeU16, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, -2) == 0);
        int8 valueI16;
        ret &= (interface->NiRead(varDescriptor, valueI16) == 0);
        ret &= valueI16 == -2;

        int32 typeI32 = 0;
        ret &= (interface->FindResource("IndI32", typeI32, varDescriptor) == 0);
        ret &= (interface->FindResource("ContI32", typeI32, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, -3) == 0);
        int8 valueI32;
        ret &= (interface->NiRead(varDescriptor, valueI32) == 0);
        ret &= valueI32 == -3;

        int64 typeI64 = 0;
        ret &= (interface->FindResource("IndI64", typeI64, varDescriptor) == 0);
        ret &= (interface->FindResource("ContU64", typeI64, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, -4) == 0);
        int8 valueI64;
        ret &= (interface->NiRead(varDescriptor, valueI64) == 0);
        ret &= valueI64 == -4;

    }

    ret &= interface->Reset() == 0;
    ret &= interface->Close() == 0;

    return ret;

}

bool NI9157DeviceTest::TestNiWrite() {
    return TestNiRead();
}

