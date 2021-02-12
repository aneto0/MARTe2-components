/**
 * @file NI9157DeviceTest.cpp
 * @brief Source file for class NI9157DeviceTest
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), "056FA65581781B17399E48BA851E9F28") == 0;
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), "056FA65581781B17399E48BA851E9F28") == 0;
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), "056FA65581781B17399E48BA851E9F28") == 0;
        ret &= StringHelper::Compare(interface->GetNiRioDeviceName(), "RIO0") == 0;
    }
    return ret;

}

bool NI9157DeviceTest::TestInitialise_FalseNoDeviceName() {
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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

        uint8 typeU8 = 0;
        uint32 varDescriptor;
        ret &= (interface->FindResource("IndU8", typeU8, varDescriptor) == 0);

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
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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

        uint32 contrDescriptor;
        uint32 varDescriptor;

        uint8 typeU8 = 0;
        ret &= (interface->FindResource("IndU8", typeU8, varDescriptor) == 0);
        ret &= (interface->FindResource("ContU8", typeU8, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, (uint8) 1) == 0);
        uint8 valueU8;
        ret &= (interface->NiRead(varDescriptor, valueU8) == 0);
        ret &= (valueU8 == 1);

        uint16 typeU16 = 0;
        ret &= (interface->FindResource("IndU16", typeU16, varDescriptor) == 0);
        ret &= (interface->FindResource("ContU16", typeU16, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, (uint16) 2) == 0);
        uint16 valueU16;
        ret &= (interface->NiRead(varDescriptor, valueU16) == 0);
        ret &= (valueU16 == 2);

        uint32 typeU32 = 0;
        ret &= (interface->FindResource("IndU32", typeU32, varDescriptor) == 0);
        ret &= (interface->FindResource("ContU32", typeU32, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, (uint32) 3) == 0);
        uint32 valueU32;
        ret &= (interface->NiRead(varDescriptor, valueU32) == 0);
        ret &= valueU32 == 3;

        uint64 typeU64 = 0;
        ret &= (interface->FindResource("IndU64", typeU64, varDescriptor) == 0);
        ret &= (interface->FindResource("ContU64", typeU64, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, (uint64) 4) == 0);
        uint64 valueU64;
        ret &= (interface->NiRead(varDescriptor, valueU64) == 0);
        ret &= valueU64 == 4;

        int8 typeI8 = 0;
        ret &= (interface->FindResource("IndI8", typeI8, varDescriptor) == 0);
        ret &= (interface->FindResource("ContI8", typeI8, varDescriptor) == 0);

        ret &= (interface->NiWrite(varDescriptor, (int8) - 1) == 0);
        int8 valueI8;
        ret &= (interface->NiRead(varDescriptor, valueI8) == 0);
        ret &= valueI8 == -1;

        int16 typeI16 = 0;
        ret &= (interface->FindResource("IndI16", typeI16, varDescriptor) == 0);
        ret &= (interface->FindResource("ContI16", typeI16, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, (int16) - 2) == 0);
        int8 valueI16;
        ret &= (interface->NiRead(varDescriptor, valueI16) == 0);
        ret &= valueI16 == -2;

        int32 typeI32 = 0;
        ret &= (interface->FindResource("IndI32", typeI32, varDescriptor) == 0);
        ret &= (interface->FindResource("ContI32", typeI32, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, (int32) - 3) == 0);
        int32 valueI32;
        ret &= (interface->NiRead(varDescriptor, valueI32) == 0);
        ret &= valueI32 == -3;

        int64 typeI64 = 0;
        ret &= (interface->FindResource("IndI64", typeI64, varDescriptor) == 0);
        ret &= (interface->FindResource("ContI64", typeI64, contrDescriptor) == 0);

        ret &= (interface->NiWrite(contrDescriptor, (int64) - 4) == 0);
        int64 valueI64;
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

bool NI9157DeviceTest::TestNiConfigureFifo() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
        uint64 typeU64 = 0;
        uint32 varDescriptor;
        ret &= (interface->FindResource("FIFO", typeU64, varDescriptor) == 0);
        uint32 oldSize = 0;
        uint32 hostFifoSize = 20000;
        ret = (interface->NiConfigureFifo(varDescriptor, hostFifoSize, oldSize) == 0);
    }

    return ret;

}

bool NI9157DeviceTest::TestNiStartFifo() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_TestGTD0001.lvbitx\""
            "    NiRioGenSignature = \"056FA65581781B17399E48BA851E9F28\""
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
        uint64 typeU64 = 0;
        uint32 varDescriptor;
        ret &= (interface->FindResource("FIFO", typeU64, varDescriptor) == 0);
        ret &= (interface->NiStartFifo(varDescriptor) == 0);
        if (ret) {
            ret = (interface->NiStopFifo(varDescriptor) == 0);
        }

    }

    return ret;
}

bool NI9157DeviceTest::TestNiStopFifo() {
    return TestNiStartFifo();
}

bool NI9157DeviceTest::TestNiReadFifo_U8() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled1.lvbitx\""
            "    NiRioGenSignature = \"163FA42D62F2477BF29AE83EC1BFAD97\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        uint8 typeU8 = 0;
        uint32 fifoU8w;
        uint32 fifoU8r;
        ret &= (interface->FindResource("FIFOU8", typeU8, fifoU8w) == 0);
        ret &= (interface->FindResource("FIFOU8r", typeU8, fifoU8r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoU8w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoU8r, numberOfElements, oldSize) == 0);
        uint8 dataw[numberOfElements];
        uint8 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = (i % 256);
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoU8w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoU8r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == (i % 256);
                    printf("datar[%d]=%d\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_U8() {
    return TestNiReadFifo_U8();
}

bool NI9157DeviceTest::TestNiReadFifo_U16() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled2.lvbitx\""
            "    NiRioGenSignature = \"25AFBDC09C72D8241C9EF82DD05F7615\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        uint16 typeU16 = 0;
        uint32 fifoU16w;
        uint32 fifoU16r;
        ret &= (interface->FindResource("FIFOU16", typeU16, fifoU16w) == 0);
        ret &= (interface->FindResource("FIFOU16r", typeU16, fifoU16r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoU16w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoU16r, numberOfElements, oldSize) == 0);
        uint16 dataw[numberOfElements];
        uint16 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = i;
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoU16w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoU16r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == i ;
                    printf("datar[%d]=%d\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_U16() {
    return TestNiReadFifo_U8();

}

bool NI9157DeviceTest::TestNiReadFifo_U32() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled5.lvbitx\""
            "    NiRioGenSignature = \"A10D94348BA197A47B9DFAD8F96D9407\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        uint32 typeU32 = 0;
        uint32 fifoU32w;
        uint32 fifoU32r;
        ret &= (interface->FindResource("FIFOU32", typeU32, fifoU32w) == 0);
        ret &= (interface->FindResource("FIFOU32r", typeU32, fifoU32r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoU32w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoU32r, numberOfElements, oldSize) == 0);
        uint32 dataw[numberOfElements];
        uint32 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = i;
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoU32w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoU32r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == i;
                    printf("datar[%d]=%d\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_U32() {
    return TestNiReadFifo_U8();

}

bool NI9157DeviceTest::TestNiReadFifo_U64() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled7.lvbitx\""
            "    NiRioGenSignature = \"42F3FE182496C099A79BB886A7E8C9CE\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        uint64 typeU64 = 0;
        uint32 fifoU64w;
        uint32 fifoU64r;
        ret &= (interface->FindResource("FIFOU64", typeU64, fifoU64w) == 0);
        ret &= (interface->FindResource("FIFOU64r", typeU64, fifoU64r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoU64w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoU64r, numberOfElements, oldSize) == 0);
        uint64 dataw[numberOfElements];
        uint64 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = i;
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoU64w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoU64r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == i ;
                    printf("datar[%d]=%lld\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_U64() {
    return TestNiReadFifo_U8();

}

bool NI9157DeviceTest::TestNiReadFifo_I8() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled3.lvbitx\""
            "    NiRioGenSignature = \"02D4F908CF0AAC78918C46663966129D\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        int8 typeI8 = 0;
        uint32 fifoI8w;
        uint32 fifoI8r;
        ret &= (interface->FindResource("FIFOI8", typeI8, fifoI8w) == 0);
        ret &= (interface->FindResource("FIFOI8r", typeI8, fifoI8r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoI8w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoI8r, numberOfElements, oldSize) == 0);
        int8 dataw[numberOfElements];
        int8 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = (i % 256)-128;
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoI8w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoI8r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == ((int32)i % 256)-128;
                    printf("datar[%d]=%d\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_I8() {
    return TestNiReadFifo_U8();

}

bool NI9157DeviceTest::TestNiReadFifo_I16() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled4.lvbitx\""
            "    NiRioGenSignature = \"06B03A0C60FA1ADF8039AC6E7412CDEF\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        int16 typeI16 = 0;
        uint32 fifoI16w;
        uint32 fifoI16r;
        ret &= (interface->FindResource("FIFOI16", typeI16, fifoI16w) == 0);
        ret &= (interface->FindResource("FIFOI16r", typeI16, fifoI16r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoI16w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoI16r, numberOfElements, oldSize) == 0);
        int16 dataw[numberOfElements];
        int16 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = i;
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoI16w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoI16r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == (int16)i;
                    printf("datar[%d]=%d\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_I16() {
    return TestNiReadFifo_U8();

}

bool NI9157DeviceTest::TestNiReadFifo_I32() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled6.lvbitx\""
            "    NiRioGenSignature = \"AB9D6324810F78B956217CEDC7DDDC19\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        int32 typeI32 = 0;
        uint32 fifoI32w;
        uint32 fifoI32r;
        ret &= (interface->FindResource("FIFOI32", typeI32, fifoI32w) == 0);
        ret &= (interface->FindResource("FIFOI32r", typeI32, fifoI32r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoI32w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoI32r, numberOfElements, oldSize) == 0);
        int32 dataw[numberOfElements];
        int32 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = i ;
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoI32w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoI32r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == (int32)i;
                    printf("datar[%d]=%d\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_I32() {
    return TestNiReadFifo_U8();

}

bool NI9157DeviceTest::TestNiReadFifo_I64() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157DeviceTestIF"
            "    NiRioDeviceName = RIO0"
            "    NiRioGenFile = \"Test/Components/Interfaces/NI9157Device/TestLabviewFiles/NiFpga_Untitled8.lvbitx\""
            "    NiRioGenSignature = \"1D84C3D47F5F61DC9C9BFB4778048DBF\""
            "    Open = 1"
            "    Configuration = {"
            "        NiFpga_Untitled1_ControlU32_period = 40000"
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
    }

    if (ret) {

        int64 typeI64 = 0;
        uint32 fifoI64w;
        uint32 fifoI64r;
        ret &= (interface->FindResource("FIFOI64", typeI64, fifoI64w) == 0);
        ret &= (interface->FindResource("FIFOI64r", typeI64, fifoI64r) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifoI64w, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifoI64r, numberOfElements, oldSize) == 0);
        int64 dataw[numberOfElements];
        int64 datar[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = i;
        }

        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }
            uint32 emptyElementsRemaining = 0u;
            ret = (interface->NiWriteFifo(fifoI64w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (interface->NiReadFifo(fifoI64r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == (int32)i;
                    printf("datar[%d]=%lld\n", i, datar[i]);
                }
            }
        }

    }

    return ret;

}

bool NI9157DeviceTest::TestNiWriteFifo_I64() {
    return TestNiReadFifo_U8();
}
