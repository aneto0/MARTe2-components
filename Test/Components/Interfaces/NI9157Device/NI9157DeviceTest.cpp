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
 *
 * @details This source file contains the definition of all the methods for
 * the class NI9157DeviceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceTest.h"

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

bool NI9157DeviceTest::TestInitialise(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
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
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret &= StringHelper::Compare(interface->GetNiRioDeviceName(), multiIOFirmware[nParams*model + 0]) == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenFile(), pathAndFile.Buffer()) == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), multiIOFirmware[nParams*model + 2]) == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialiseIsOpened(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
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
        ret &= cdb.Write("Open", 1);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret &= StringHelper::Compare(interface->GetNiRioDeviceName(), multiIOFirmware[nParams*model + 0]) == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenFile(), pathAndFile.Buffer()) == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), multiIOFirmware[nParams*model + 2]) == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialiseRandomConfig(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
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
        ret &= cdb.MoveRelative("Configuration");
        ret &= cdb.Write("ControlBool_stop", 1);
        ret &= cdb.Write("ControlBool_use_dsfifo_data", 1);
        ret &= cdb.MoveToRoot();
    }

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
        ret &= StringHelper::Compare(interface->GetNiRioDeviceName(), multiIOFirmware[nParams*model + 0]) == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenFile(), pathAndFile.Buffer()) == 0;
        ret &= StringHelper::Compare(interface->GetNiRioGenSignature(), multiIOFirmware[nParams*model + 2]) == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoDeviceName(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Delete("NiRioDeviceName");
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoGenFile(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        ret &= cdb.Delete("NiRioGenFile");
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoGenSignature(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Delete("NiRioGenSignature");
		ret &= cdb.Delete("Open");
		ret &= cdb.Delete("Run");
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoType(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.Write("Open", 1);
        ret &= cdb.Write("Run", 1);
        ret &= cdb.MoveRelative("Configuration");
        ret &= cdb.Delete("ControlBool_stop");
        ret &= cdb.Write("Control_stop", 0);
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseVariableNotFound(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.Write("Open", 1);
        ret &= cdb.MoveRelative("Configuration");
        ret &= cdb.Delete("ControlBool_stop");
        ret &= cdb.Write("ControlBool_stop2", 0);

        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseOpenFakeDevice(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", "0123456789ABCDEF0123456789ABCDEF");
        ret &= cdb.Write("Open", "1");
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_OpenResetRun(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.Write("Open", 1);
        ret &= cdb.Write("Reset", 1);
        ret &= cdb.Write("Run", 1);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
        if (ret) {
            ret &= interface->Reset() == 0;
            ret &= interface->Close() == 0;
        }
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_NoOpenResetRun(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
		ret &= cdb.Delete("Open");
		ret &= cdb.Delete("Reset");
		ret &= cdb.Delete("Run");
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    ReferenceT<NI9157DeviceTestIF> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
        if (ret) {
            ret &= interface->Reset() != 0;
            ret &= interface->Close() != 0;
        }
    }

    return ret;
}

bool NI9157DeviceTest::TestInitialise_FalseNoConfig(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
		ret &= cdb.Write("Open", "1");
        ret &= cdb.Write("Reset", "1");
		ret &= cdb.Write("Run", "1");
        ret &= cdb.Delete("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = !god->Initialise(cdb);
    }

    return ret;
}

bool NI9157DeviceTest::TestOpen(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret = interface->Open() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestOpen_FalseParams(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", "0123456789ABCDEF0123456789ABCDEF");
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret = !interface->Open() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestIsOpened(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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

bool NI9157DeviceTest::TestRun(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret = interface->Open() == 0;
        if (ret) {
            ret = interface->Run() == 0;
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestRun_FalseParams(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", "0123456789ABCDEF0123456789ABCDEF");
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret = !interface->Open() == 0;
        if (ret) {
            ret = !interface->Run() == 0;
        }
        ret &= interface->Reset() != 0;
        ret &= interface->Close() != 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestIsRunning(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            ret = interface->Open() == 0;
        }
        if (ret) {
            ret = interface->Run() == 0;
        }
        if (ret) {
            ret = interface->IsRunning() == 1;
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestGetSession(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            ret = interface->Open() == 0;
        }
        if (ret) {
            NiFpga_Session session = interface->GetSession();
            NiFpga_Status status = 0;
            status = NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));
            ret = (status == 0);
        }
        ret &= interface->Reset() == 0;    
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestReset(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret = interface->Open() == 0;
        if (ret) {
            ret = interface->Reset() == 0;
        }
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestClose(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret = interface->Open() == 0;
        if (ret) {
            ret = interface->Reset() == 0;
        }
        if (ret) {
            ret = interface->Close() == 0;
        }
    }

    return ret;
}

bool NI9157DeviceTest::TestCrioStart(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
		ErrorManagement::ErrorType err;
        err = interface->CrioStart();
        ret = err.ErrorsCleared();
    }
    if (ret) {
        ret = interface->IsRunning() == 1;
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestCrioStop(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ret = interface->Open() == 0;
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
            ErrorManagement::ErrorType err;
            err = interface->CrioStop();
            ret = err.ErrorsCleared();
        }
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestFindResource(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            ret = interface->Open() == 0;
        }
        if (ret) {
            ret = interface->IsOpened() == 1;
        }
        uint32 varDescriptor;
        if (ret) {
            bool typeBool = 0;
            ret = (interface->FindResource("IndBool", typeBool, varDescriptor) == 0);
        }
        if (ret) {
            uint8 typeU8 = 0;
            ret = (interface->FindResource("IndU8", typeU8, varDescriptor) == 0);
        }
        if (ret) {
            uint16 typeU16 = 0;
            ret = (interface->FindResource("IndU16", typeU16, varDescriptor) == 0);
        }
        if (ret) {
            uint32 typeU32 = 0;
            ret = (interface->FindResource("IndU32", typeU32, varDescriptor) == 0);
        }
        if (ret) {
            uint64 typeU64 = 0;
            ret = (interface->FindResource("IndU64", typeU64, varDescriptor) == 0);
        }
        if (ret) {
            int8 typeI8 = 0;
            ret = (interface->FindResource("IndI8", typeI8, varDescriptor) == 0);
        }
        if (ret) {
            int16 typeI16 = 0;
            ret = (interface->FindResource("IndI16", typeI16, varDescriptor) == 0);
        }
        if (ret) {
            int32 typeI32 = 0;
            ret = (interface->FindResource("IndI32", typeI32, varDescriptor) == 0);
        }
        if (ret) {
            int64 typeI64 = 0;
            ret = (interface->FindResource("IndI64", typeI64, varDescriptor) == 0);
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWrite(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        uint32 contrDescriptor;
        uint32 varDescriptor;
        if (ret) {
            bool typeBool = false;
            ret = (interface->FindResource("ContBool", typeBool, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (bool) 1) == 0);
        }
        if (ret) {
            uint8 typeU8 = 0;
            ret = (interface->FindResource("ContU8", typeU8, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint8) 1) == 0);
        }
        if (ret) {
            uint16 typeU16 = 0;
            ret = (interface->FindResource("ContU16", typeU16, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint16) 2) == 0);
        }
        if (ret) {
            uint32 typeU32 = 0;
            ret = (interface->FindResource("ContU32", typeU32, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint32) 3) == 0);
        }
        if (ret) {
            uint64 typeU64 = 0;
            ret = (interface->FindResource("ContU64", typeU64, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint64) 4) == 0);
        }
        if (ret) {
            int8 typeI8 = 0;
            ret = (interface->FindResource("ContI8", typeI8, varDescriptor) == 0);
            ret &= (interface->NiWrite(varDescriptor, (int8) - 1) == 0);
        }
        if (ret) {
            int16 typeI16 = 0;
            ret = (interface->FindResource("ContI16", typeI16, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (int16) - 2) == 0);
        }
        if (ret) {
            int32 typeI32 = 0;
            ret = (interface->FindResource("ContI32", typeI32, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (int32) - 3) == 0);
        }
        if (ret) {
            int64 typeI64 = 0;
            ret = (interface->FindResource("ContI64", typeI64, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (int64) - 4) == 0);
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiRead(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        uint32 contrDescriptor;
        uint32 varDescriptor;
        if (ret) {
            bool typeBool = false;
            bool valueBool;
            ret = (interface->FindResource("IndBool", typeBool, varDescriptor) == 0);
            ret &= (interface->FindResource("ContBool", typeBool, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (bool) 1) == 0);
            ret &= (interface->NiRead(varDescriptor, valueBool) == 0);
            ret &= (valueBool == (bool) 1);
        }
        if (ret) {
            uint8 typeU8 = 0;
            uint8 valueU8;
            ret = (interface->FindResource("IndU8", typeU8, varDescriptor) == 0);
            ret &= (interface->FindResource("ContU8", typeU8, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint8) 1) == 0);
            ret &= (interface->NiRead(varDescriptor, valueU8) == 0);
            ret &= (valueU8 == (uint8) 1);
        }
        if (ret) {
            uint16 typeU16 = 0;
            uint16 valueU16;
            ret = (interface->FindResource("IndU16", typeU16, varDescriptor) == 0);
            ret &= (interface->FindResource("ContU16", typeU16, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint16) 2) == 0);
            ret &= (interface->NiRead(varDescriptor, valueU16) == 0);
            ret &= (valueU16 == (uint16) 2);
        }
        if (ret) {
            uint32 typeU32 = 0;
            uint32 valueU32;
            ret = (interface->FindResource("IndU32", typeU32, varDescriptor) == 0);
            ret &= (interface->FindResource("ContU32", typeU32, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint32) 3) == 0);
            ret &= (interface->NiRead(varDescriptor, valueU32) == 0);
            ret &= (valueU32 == (uint32) 3);
        }
        if (ret) {
            uint64 typeU64 = 0;
            uint64 valueU64;
            ret = (interface->FindResource("IndU64", typeU64, varDescriptor) == 0);
            ret &= (interface->FindResource("ContU64", typeU64, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (uint64) 4) == 0);
            ret &= (interface->NiRead(varDescriptor, valueU64) == 0);
            ret &= (valueU64 == (uint64) 4);
        }
        if (ret) {
            int8 typeI8 = 0;
            int8 valueI8;
            ret = (interface->FindResource("IndI8", typeI8, varDescriptor) == 0);
            ret &= (interface->FindResource("ContI8", typeI8, varDescriptor) == 0);
            ret &= (interface->NiWrite(varDescriptor, (int8) - 1) == 0);
            ret &= (interface->NiRead(varDescriptor, valueI8) == 0);
            ret &= (valueI8 == (int8) -1);
        }
        if (ret) {
            int16 typeI16 = 0;
            int16 valueI16;
            ret = (interface->FindResource("IndI16", typeI16, varDescriptor) == 0);
            ret &= (interface->FindResource("ContI16", typeI16, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (int16) - 2) == 0);
            ret &= (interface->NiRead(varDescriptor, valueI16) == 0);
            ret &= (valueI16 == (int16) -2);
        }
        if (ret) {
            int32 typeI32 = 0;
            int32 valueI32;
            ret = (interface->FindResource("IndI32", typeI32, varDescriptor) == 0);
            ret &= (interface->FindResource("ContI32", typeI32, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (int32) - 3) == 0);
            ret &= (interface->NiRead(varDescriptor, valueI32) == 0);
            ret &= (valueI32 == (int32) -3);
        }
        if (ret) {
            int64 typeI64 = 0;
            int64 valueI64;
            ret = (interface->FindResource("IndI64", typeI64, varDescriptor) == 0);
            ret &= (interface->FindResource("ContI64", typeI64, contrDescriptor) == 0);
            ret &= (interface->NiWrite(contrDescriptor, (int64) - 4) == 0);
            ret &= (interface->NiRead(varDescriptor, valueI64) == 0);
            ret &= (valueI64 == (int64) -4);
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }
    return ret;
}

bool NI9157DeviceTest::TestNiConfigureFifo(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            ret = interface->Open() == 0;
        }
        if (ret) {
            ret = interface->IsOpened() == 1;
        }
        if (ret) {
            uint64 typeU64 = 0;
            uint32 varDescriptor;
            ret = (interface->FindResource("FIFO1_U64_R", typeU64, varDescriptor) == 0);
            if (ret) {
                uint32 oldSize = 0;
                uint32 hostFifoSize = 20000;
                ret = (interface->NiConfigureFifo(varDescriptor, hostFifoSize, oldSize) == 0);
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiStartFifo(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            ret = interface->Open() == 0;
        }
        if (ret) {
            ret = interface->IsOpened() == 1;
        }
        if (ret) {
            uint64 typeU64 = 0;
            uint32 varDescriptor;
            ret = (interface->FindResource("FIFO1_U64_R", typeU64, varDescriptor) == 0);
            if (ret) {
                ret = (interface->NiStartFifo(varDescriptor) == 0);
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiStopFifo(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            ret = interface->Open() == 0;
        }
        if (ret) {
            ret = interface->IsOpened() == 1;
        }
        if (ret) {
            uint64 typeU64 = 0;
            uint32 varDescriptor;
            ret = (interface->FindResource("FIFO1_U64_R", typeU64, varDescriptor) == 0);
            if (ret) {
                ret = (interface->NiStartFifo(varDescriptor) == 0);
            }
            if (ret) {
                ret = (interface->NiStopFifo(varDescriptor) == 0);
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestWriteParam(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ErrorManagement::ErrorType err;
        StreamString paramName;
        StreamString paramType;
        uint64 writeValue;
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContBool";
            paramType = "bool";
            writeValue = 1;
            writeValue &= 1ull;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU8";
            paramType = "uint8";
            writeValue = 1;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU16";
            paramType = "uint16";
            writeValue = 2;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU32";
            paramType = "uint32";
            writeValue = 3;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU64";
            paramType = "uint64";
            writeValue = 4;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI8";
            paramType = "int8";
            writeValue = -1;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI16";
            paramType = "int16";
            writeValue = -2;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI32";
            paramType = "int32";
            writeValue = -3;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI64";
            paramType = "int64";
            writeValue = -4;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI64";
            paramType = "aType";
            writeValue = -4;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = !err.ErrorsCleared();
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }
    return ret;
}

bool NI9157DeviceTest::TestReadParam(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = multiIoConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", multiIOFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, multiIOFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", multiIOFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        ErrorManagement::ErrorType err;
        StreamString paramName;
        StreamString paramType;
        uint64 writeValue;
        uint64 readValue;
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContBool";
            paramType = "bool";
            writeValue = 1;
            readValue = 0;
            writeValue &= 1ull;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndBool";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((bool)(writeValue & 1ull) == (bool)(readValue & 1ull));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU8";
            paramType = "uint8";
            writeValue = 1;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndU8";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((uint8)(writeValue) == (uint8)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU16";
            paramType = "uint16";
            writeValue = 2;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndU16";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((uint16)(writeValue) == (uint16)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU32";
            paramType = "uint32";
            writeValue = 3;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndU32";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((uint32)(writeValue) == (uint32)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContU64";
            paramType = "uint64";
            writeValue = 4;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndU64";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((uint64)(writeValue) == (uint64)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI8";
            paramType = "int8";
            writeValue = -1;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndI8";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((int8)(writeValue) == (int8)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI16";
            paramType = "int16";
            writeValue = -2;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndI16";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((int16)(writeValue) == (int16)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI32";
            paramType = "int32";
            writeValue = -3;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndI32";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((int32)(writeValue) == (int32)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI64";
            paramType = "int64";
            writeValue = -4;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndI64";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = err.ErrorsCleared();
                ret &= ((int64)(writeValue) == (int64)(readValue));
            }
        }
        if (ret) {
            paramName.SetSize(0);
            paramType.SetSize(0);
            paramName = "ContI64";
            paramType = "aType";
            writeValue = -4;
            readValue = 0;
            err = interface->WriteParam(paramName, writeValue, paramType);
            ret = !err.ErrorsCleared();
            if (ret) {
                paramName.SetSize(0);
                paramName = "IndI64";
                err = interface->ReadParam(paramName, readValue, paramType);
                ret = !err.ErrorsCleared();
                ret &= !((int64)(writeValue) == (int64)(readValue));
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }
    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_Bool(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", boolFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, boolFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", boolFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            bool typeBool = 0;
            uint32 fifoBoolw;
            ret = (interface->FindResource("FIFO0_BOOL_W", typeBool, fifoBoolw) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                bool dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = (bool) (i & 1ull);
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoBoolw, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_Bool(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", boolFirmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, boolFirmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", boolFirmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            bool typeBool = 0;
            uint32 fifoBoolw;
            uint32 fifoBoolr;
            ret = (interface->FindResource("FIFO0_BOOL_W", typeBool, fifoBoolw) == 0);
            ret &= (interface->FindResource("FIFO1_BOOL_R", typeBool, fifoBoolr) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoBoolw, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoBoolr, numberOfElements, oldSize) == 0);
                if (ret) {
                    bool dataw[numberOfElements];
                    bool datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = (bool) (i & 1ull);
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoBoolw, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoBoolr, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (bool) (i & 1ull));
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_BOOL_R[%d]=%s != %s", i, datar[i] ? "true" : "false", (bool) (i & 1ull) ? "true" : "false");
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }
    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_U8(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u8Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u8Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u8Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint8 typeU8 = 0;
            uint32 fifoU8w;
            ret = (interface->FindResource("FIFO0_U8_W", typeU8, fifoU8w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint8 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = (i % 256);
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoU8w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_U8(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u8Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u8Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u8Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint8 typeU8 = 0;
            uint32 fifoU8w;
            uint32 fifoU8r;
            ret = (interface->FindResource("FIFO0_U8_W", typeU8, fifoU8w) == 0);
            ret &= (interface->FindResource("FIFO1_U8_R", typeU8, fifoU8r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoU8w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoU8r, numberOfElements, oldSize) == 0);
                if (ret) {
                    uint8 dataw[numberOfElements];
                    uint8 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = (i % 256);
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoU8w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoU8r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (i % 256));
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_U8_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_U16(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u16Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u16Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u16Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint16 typeU16 = 0;
            uint32 fifoU16w;
            ret = (interface->FindResource("FIFO0_U16_W", typeU16, fifoU16w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint16 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = i;
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoU16w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_U16(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u16Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u16Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u16Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint16 typeU16 = 0;
            uint32 fifoU16w;
            uint32 fifoU16r;
            ret = (interface->FindResource("FIFO0_U16_W", typeU16, fifoU16w) == 0);
            ret &= (interface->FindResource("FIFO1_U16_R", typeU16, fifoU16r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoU16w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoU16r, numberOfElements, oldSize) == 0);
                if (ret) {
                    uint16 dataw[numberOfElements];
                    uint16 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = i;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoU16w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoU16r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (uint16)i);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_U16_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_U32(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u32Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u32Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u32Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint32 typeU32 = 0;
            uint32 fifoU32w;
            ret = (interface->FindResource("FIFO0_U32_W", typeU32, fifoU32w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = i;
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoU32w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_U32(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u32Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u32Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u32Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint32 typeU32 = 0;
            uint32 fifoU32w;
            uint32 fifoU32r;
            ret = (interface->FindResource("FIFO0_U32_W", typeU32, fifoU32w) == 0);
            ret &= (interface->FindResource("FIFO1_U32_R", typeU32, fifoU32r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoU32w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoU32r, numberOfElements, oldSize) == 0);
                if (ret) {
                    uint32 dataw[numberOfElements];
                    uint32 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = i;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoU32w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoU32r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (uint32)i);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_U32_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_U64(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u64Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u64Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u64Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint64 typeU64 = 0;
            uint32 fifoU64w;
            ret = (interface->FindResource("FIFO0_U64_W", typeU64, fifoU64w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint64 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = i;
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoU64w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_U64(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", u64Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, u64Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", u64Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            uint64 typeU64 = 0;
            uint32 fifoU64w;
            uint32 fifoU64r;
            ret = (interface->FindResource("FIFO0_U64_W", typeU64, fifoU64w) == 0);
            ret &= (interface->FindResource("FIFO1_U64_R", typeU64, fifoU64r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoU64w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoU64r, numberOfElements, oldSize) == 0);
                if (ret) {
                    uint64 dataw[numberOfElements];
                    uint64 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = i;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoU64w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoU64r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (uint64)i);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_U64_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_I8(uint32 model) {
 
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i8Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i8Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i8Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int8 typeI8 = 0;
            uint32 fifoI8w;
            ret = (interface->FindResource("FIFO0_I8_W", typeI8, fifoI8w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                int8 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = (i % 256)-128;
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoI8w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_I8(uint32 model) {
 
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i8Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i8Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i8Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int8 typeI8 = 0;
            uint32 fifoI8w;
            uint32 fifoI8r;
            ret = (interface->FindResource("FIFO0_I8_W", typeI8, fifoI8w) == 0);
            ret &= (interface->FindResource("FIFO1_I8_R", typeI8, fifoI8r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoI8w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoI8r, numberOfElements, oldSize) == 0);
                if (ret) {
                    int8 dataw[numberOfElements];
                    int8 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = (i % 256)-128;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoI8w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoI8r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == ((int32)i % 256)-128);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_I8_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_I16(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i16Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i16Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i16Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int16 typeI16 = 0;
            uint32 fifoI16w;
            ret = (interface->FindResource("FIFO0_I16_W", typeI16, fifoI16w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                int16 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = i;
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoI16w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_I16(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i16Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i16Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i16Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int16 typeI16 = 0;
            uint32 fifoI16w;
            uint32 fifoI16r;
            ret = (interface->FindResource("FIFO0_I16_W", typeI16, fifoI16w) == 0);
            ret &= (interface->FindResource("FIFO1_I16_R", typeI16, fifoI16r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoI16w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoI16r, numberOfElements, oldSize) == 0);
                if (ret) {
                    int16 dataw[numberOfElements];
                    int16 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = i;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoI16w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoI16r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (int16)i);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_I16_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_I32(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i32Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i32Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i32Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int32 typeI32 = 0;
            uint32 fifoI32w;
            ret = (interface->FindResource("FIFO0_I32_W", typeI32, fifoI32w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                int32 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = i ;
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoI32w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_I32(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i32Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i32Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i32Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int32 typeI32 = 0;
            uint32 fifoI32w;
            uint32 fifoI32r;
            ret = (interface->FindResource("FIFO0_I32_W", typeI32, fifoI32w) == 0);
            ret &= (interface->FindResource("FIFO1_I32_R", typeI32, fifoI32r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoI32w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoI32r, numberOfElements, oldSize) == 0);
                if (ret) {
                    int32 dataw[numberOfElements];
                    int32 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = i ;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoI32w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoI32r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (int32)i);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_I32_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiWriteFifo_I64(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i64Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i64Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i64Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int64 typeI64 = 0;
            uint32 fifoI64w;
            ret = (interface->FindResource("FIFO0_I64_W", typeI64, fifoI64w) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                int64 dataw[numberOfElements];
                for (uint32 i = 0u; i < numberOfElements; i++) {
                    dataw[i] = i;
                }
                if (ret) {
                    ret = interface->Run() == 0;
                }
                if (ret) {
                    ret = interface->IsRunning() == 1;
                }
                if (ret) {
                    uint32 emptyElementsRemaining = 0u;
                    ret = (interface->NiWriteFifo(fifoI64w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceTest::TestNiReadFifo_I64(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        ret = cdb.MoveAbsolute("+NiDevice");
        ret &= cdb.Write("NiRioDeviceName", i64Firmware[nParams*model + 0]);
        StreamString pathAndFile = "";
        pathAndFile.Printf("%s/%s", firmwarePath, i64Firmware[nParams*model + 1]);
        ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
        ret &= cdb.Write("NiRioGenSignature", i64Firmware[nParams*model + 2]);
        ret &= cdb.MoveRelative("Configuration");
        // Change....
        ret &= cdb.MoveToRoot();
    }

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
        if (ret) {
            int64 typeI64 = 0;
            uint32 fifoI64w;
            uint32 fifoI64r;
            ret = (interface->FindResource("FIFO0_I64_W", typeI64, fifoI64w) == 0);
            ret &= (interface->FindResource("FIFO1_I64_R", typeI64, fifoI64r) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(fifoI64w, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(fifoI64r, numberOfElements, oldSize) == 0);
                if (ret) {
                    int64 dataw[numberOfElements];
                    int64 datar[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = i;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (interface->NiWriteFifo(fifoI64w, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (interface->NiReadFifo(fifoI64r, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (datar[i] == (int32)i);
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_I64_R[%d]=%u != %d", i, datar[i] ,i);
                                }
                            }
                        }
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}
