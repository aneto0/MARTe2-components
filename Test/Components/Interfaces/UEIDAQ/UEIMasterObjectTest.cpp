/**
 * @file UEIMasterObjectTest.cpp
 * @brief Source file for class UEIMasterObjectTest
 * @date 22/03/2023
 * @author Xavier Ruche
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
 * the class UEIMasterObjectTest (public, protected, and private). Be aware that some 
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
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StreamString.h"
#include "TypeDescriptor.h"
#include "UEIMasterObjectTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class UEIMasterObjectHL : public UEIMasterObject {
public:
    CLASS_REGISTER_DECLARATION()
    UEIMasterObjectHL();
    ~UEIMasterObjectHL();
    int32 GetDAQ_handleHL()            {return DAQ_handle;}
    uint8 GetNDevicesHL()              {return nDevices;}
    uint16* GetIpHL()                  {return ip;}
    char8* GetIp_stringHL()            {return ip_string;}
    uint16 GetPortHL()                 {return port;}
    StreamString GetNameHL()           {return name;}
    ReferenceT<UEIDevice>* GetDevicesHL()                      {return devices;}
    ReferenceT<UEIMapContainer>* GetMapsHL()                   {return maps;}
    uint32 GetNMapsHL()                {return nMaps;}
    uint32 GetIOMTimeOutHL()           {return IOMTimeOut;}
    void SetDAQ_handleHL(int32 handle) {DAQ_handle = handle;}
};
UEIMasterObjectHL::UEIMasterObjectHL() : UEIMasterObject(){

}
UEIMasterObjectHL::~UEIMasterObjectHL(){

}
CLASS_REGISTER(UEIMasterObjectHL, "1.0")

class UEIBlankTestDevice : public UEIDevice {
public:
    CLASS_REGISTER_DECLARATION()
    UEIBlankTestDevice();
    ~UEIBlankTestDevice();
    int32 GetModel() {return 0x01;}
    bool ConfigureDevice(int32 DAQ_handle) {return false;}
    int32 configurableMode;
};
UEIBlankTestDevice::UEIBlankTestDevice() : UEIDevice(){
}
UEIBlankTestDevice::~UEIBlankTestDevice(){

}
CLASS_REGISTER(UEIBlankTestDevice, "1.0")

class UEIBlankTestMap : public UEIMapContainer {
public:
    CLASS_REGISTER_DECLARATION()
    UEIBlankTestMap();
    ~UEIBlankTestMap();
};
UEIBlankTestMap::UEIBlankTestMap() : UEIMapContainer(){
}
UEIBlankTestMap::~UEIBlankTestMap(){

}
CLASS_REGISTER(UEIBlankTestMap, "1.0")

class UEINoDeviceMap : public UEIMapContainer {
public:
    CLASS_REGISTER_DECLARATION()
    UEINoDeviceMap();
    ~UEINoDeviceMap();
    bool SetDevices(ReferenceT<UEIDevice>* referenceList) {return false;}
};
UEINoDeviceMap::UEINoDeviceMap() : UEIMapContainer(){
}
UEINoDeviceMap::~UEINoDeviceMap(){

}
CLASS_REGISTER(UEINoDeviceMap, "1.0")

class UEINoCoherencyMap : public UEIMapContainer {
public:
    CLASS_REGISTER_DECLARATION()
    UEINoCoherencyMap();
    ~UEINoCoherencyMap();
    bool CheckMapCoherency() {
        return false;
    }
};
UEINoCoherencyMap::UEINoCoherencyMap() : UEIMapContainer(){
}
UEINoCoherencyMap::~UEINoCoherencyMap(){

}
CLASS_REGISTER(UEINoCoherencyMap, "1.0")

const MARTe::char8 * const simpleConfig = ""
    "+UEIDAQ = {"
    "     Class    = UEIMasterObjectHL"
    "     Ip       = {127, 0, 0, 1}"
    "     Port     =  6334"
    "     ConnectionTimeOut = 100"
    "     +Devices = {"
    "       Class=ReferenceContainer"
    "       +dev0={"
    "          Class               = UEIAI217_803"
    "          Devn                = 0"
    "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "          SamplingFrequency  = 1000.0"
    "       }"
    "       +dev1={"
    "          Class               = UEIAI217_803"
    "          Devn                = 1"
    "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "          SamplingFrequency  = 1000.0"
    "       }"
    "    } "
    " }";

const MARTe::char8 * const mapsConfig = ""
    "+UEIDAQ = {"
    "    Class    = UEIMasterObjectHL"
    "    Ip       = {127, 0, 0, 1}"
    "    Port     =  6334"
    "    ConnectionTimeOut = 100"
    "    +Devices = {"
    "        Class=ReferenceContainer"
    "        +dev0={"
    "            Class               = UEIAI217_803"
    "            Devn                = 0"
    "            Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "            SamplingFrequency  = 1000.0"
    "        }"
    "        +dev1={"
    "            Class               = UEIAI217_803"
    "            Devn                = 1"
    "            Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
    "            SamplingFrequency  = 1000.0"
    "        }"
    "    }"
    "    +Maps = {"
    "        Class=ReferenceContainer"
    "        +Map1 = {"
    "            Class           = UEIBlankTestMap"
    "            ExecutionMode   = RealTimeThread"
    "            Inputs = {"
    "                dev0 = {"
    "                    Devn        = 0"
    "                    Channels    = {0, 1}"
    "                }"
    "            }"
    "        }"
    "        +Map2 = {"
    "            Class           = UEIBlankTestMap"
    "            ExecutionMode   = RealTimeThread"
    "            Inputs = {"
    "                dev1 = {"
    "                    Devn        = 1"
    "                    Channels    = {0, 1}"
    "                }"
    "            }"
    "        } "
    "    }"
    "}";

UEIMasterObjectTest::UEIMasterObjectTest(){
}

UEIMasterObjectTest::~UEIMasterObjectTest(){
}

bool UEIMasterObjectTest::TestConstructor() {
    bool ok = true;
    UEIMasterObjectHL testObject;
    ok &= SafeMath::IsEqual(testObject.GetDAQ_handleHL(), (int32) 0);
    ok &= SafeMath::IsEqual(testObject.GetPortHL(), (uint16) 0);
    ok &= SafeMath::IsEqual(testObject.GetIp_stringHL(), NULL_PTR(char8*));
    ok &= SafeMath::IsEqual(testObject.GetNDevicesHL(), (uint8) 0);
    ok &= SafeMath::IsEqual(testObject.GetNMapsHL(), (uint32) 0);
    ok &= SafeMath::IsEqual(testObject.GetIOMTimeOutHL(), (uint32) 0);
    //Test also the destructor when the IOM closing fails
    testObject.SetDAQ_handleHL(1);
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->SetErrorCode(DqCloseIOM_Error);
    return ok;
}

bool UEIMasterObjectTest::TestInitialise() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    manager->SetErrorCode(NO_ERROR);
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMasterObjectHL> myObject;
    if (ok) {
        myObject = ObjectRegistryDatabase::Instance()->Find("UEIDAQ");
        ok &= myObject.IsValid();
    }
    if (ok){
    ok &= SafeMath::IsEqual(myObject->GetDAQ_handleHL(), (int32) 1);
    ok &= SafeMath::IsEqual(myObject->GetPortHL(), (uint16) 0); //Set to local Ip
    uint16* thisIp = myObject->GetIpHL();
    ok &= SafeMath::IsEqual(thisIp[0], (uint16) 127);
    ok &= SafeMath::IsEqual(thisIp[1], (uint16) 0);
    ok &= SafeMath::IsEqual(thisIp[2], (uint16) 0);
    ok &= SafeMath::IsEqual(thisIp[3], (uint16) 1);
    ok &= SafeMath::IsEqual(myObject->GetNDevicesHL(), (uint8) 2);
    ok &= SafeMath::IsEqual(myObject->GetNMapsHL(), (uint32) 0);
    ok &= SafeMath::IsEqual(myObject->GetIOMTimeOutHL(), (uint32) 100);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_TrueClass() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ");
        ok &= cdb.Write("Class", "UEIMasterObject");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMasterObject> myObject;
    if (ok) {
        myObject = ObjectRegistryDatabase::Instance()->Find("UEIDAQ");
        ok &= myObject.IsValid();
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_NoConnectionTimeout() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ");
        ok &= cdb.Delete("ConnectionTimeOut");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMasterObjectHL> myObject;
    if (ok) {
        myObject = ObjectRegistryDatabase::Instance()->Find("UEIDAQ");
        ok &= myObject.IsValid();
    }
    if (ok){
    ok &= SafeMath::IsEqual(myObject->GetDAQ_handleHL(), (int32) 1);
    ok &= SafeMath::IsEqual(myObject->GetPortHL(), (uint16) 0); //Set to local Ip
    uint16* thisIp = myObject->GetIpHL();
    ok &= SafeMath::IsEqual(thisIp[0], (uint16) 127);
    ok &= SafeMath::IsEqual(thisIp[1], (uint16) 0);
    ok &= SafeMath::IsEqual(thisIp[2], (uint16) 0);
    ok &= SafeMath::IsEqual(thisIp[3], (uint16) 1);
    ok &= SafeMath::IsEqual(myObject->GetNDevicesHL(), (uint8) 2);
    ok &= SafeMath::IsEqual(myObject->GetNMapsHL(), (uint32) 0);
    ok &= SafeMath::IsEqual(myObject->GetIOMTimeOutHL(), (uint32) 200);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_NoIp() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ");
        ok &= cdb.Delete("Ip");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_BadIp() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ");
        uint16 newIp[4] = {256, 300, 451, 0};
        ok &= cdb.Write("Ip", newIp);
        ok &= cdb.MoveToRoot();
    }  
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_RemoteIpNoPort() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ");
        uint16 newIp[4] = {192, 168, 100, 1};
        ok &= cdb.Write("Ip", newIp);
        ok &= cdb.Delete("Port");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_RemoteIp() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ");
        uint16 newIp[4] = {192, 168, 100, 1};
        ok &= cdb.Write("Ip", newIp);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    ReferenceT<UEIMasterObjectHL> myObject;
    if (ok) {
        myObject = ObjectRegistryDatabase::Instance()->Find("UEIDAQ");
        ok &= myObject.IsValid();
    }
    if (ok){
    ok &= SafeMath::IsEqual(myObject->GetDAQ_handleHL(), (int32) 1);
    ok &= SafeMath::IsEqual(myObject->GetPortHL(), (uint16) 6334); //Set to local Ip
    uint16* thisIp = myObject->GetIpHL();
    ok &= SafeMath::IsEqual(thisIp[0], (uint16) 192);
    ok &= SafeMath::IsEqual(thisIp[1], (uint16) 168);
    ok &= SafeMath::IsEqual(thisIp[2], (uint16) 100);
    ok &= SafeMath::IsEqual(thisIp[3], (uint16) 1);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_NoDevices() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ");
        ok &= cdb.Delete("+Devices");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_ZeroDevices() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ.+Devices");
        ok &= cdb.Delete("+dev0");
        ok &= cdb.Delete("+dev1");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_TooManyDevices() {
    bool ok = true;
    //Local Config
    const MARTe::char8 * const failConfig = ""
        "+UEIDAQ = {"
        "     Class    = UEIMasterObjectHL"
        "     Ip       = {127, 0, 0, 1}"
        "     Port     =  6334"
        "     ConnectionTimeOut = 100"
        "     +Devices = {"
        "       Class=ReferenceContainer"
        "       +dev0={"
        "          Class               = UEIAI217_803"
        "          Devn                = 0"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev1={"
        "          Class               = UEIAI217_803"
        "          Devn                = 1"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev2={"
        "          Class               = UEIAI217_803"
        "          Devn                = 2"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev3={"
        "          Class               = UEIAI217_803"
        "          Devn                = 3"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev4={"
        "          Class               = UEIAI217_803"
        "          Devn                = 4"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev5={"
        "          Class               = UEIAI217_803"
        "          Devn                = 5"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev6={"
        "          Class               = UEIAI217_803"
        "          Devn                = 6"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev7={"
        "          Class               = UEIAI217_803"
        "          Devn                = 7"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev8={"
        "          Class               = UEIAI217_803"
        "          Devn                = 8"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev9={"
        "          Class               = UEIAI217_803"
        "          Devn                = 9"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev10={"
        "          Class               = UEIAI217_803"
        "          Devn                = 10"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev11={"
        "          Class               = UEIAI217_803"
        "          Devn                = 11"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev12={"
        "          Class               = UEIAI217_803"
        "          Devn                = 10"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "    } "
        " }";

    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = failConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_RepeatedDevices() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ.+Devices.+dev1");
        ok &= cdb.Write("Devn", 0);
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_DaqLibError() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    manager->SetErrorCode(DqInitDAQLib_ERROR);
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_UnableToConnectToIOM() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    manager->SetErrorCode(DqOpenIOM_Error);
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_BadHandle() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    manager->SetErrorCode(DqOpenIOM_ZeroHandle);
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_BadDevnHandle() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    manager->SetErrorCode(DqGetDevnBySlot_ERROR);
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_DevnModelMismatch() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x500); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_DeviceConfiguredNotInstalled() {
    bool ok = true;
    const MARTe::char8 * const failConfig = ""
        "+UEIDAQ = {"
        "     Class    = UEIMasterObjectHL"
        "     Ip       = {127, 0, 0, 1}"
        "     Port     =  6334"
        "     ConnectionTimeOut = 100"
        "     +Devices = {"
        "       Class=ReferenceContainer"
        "       +dev0={"
        "          Class               = UEIAI217_803"
        "          Devn                = 0"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev1={"
        "          Class               = UEIAI217_803"
        "          Devn                = 1"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "       +dev2={"
        "          Class               = UEIAI217_803"
        "          Devn                = 2"
        "          Gains               = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}"
        "          SamplingFrequency  = 1000.0"
        "       }"
        "    } "
        " }";


    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = failConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_DeviceUnableToConfigure() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x01); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = simpleConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ.+Devices.+dev1");
        ok &= cdb.Write("Class","UEIBlankTestDevice");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_MapsDefined() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = mapsConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok) {
        god->Purge();
        //Initialisation must succeed with the default initialisation configuration
        ok = god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_InconsistentMaps() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = mapsConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ.+Maps.+Map1");
        ok &= cdb.Write("Class", "UEINoCoherencyMap");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}

bool UEIMasterObjectTest::TestInitialise_InvalidSetDevices() {
    bool ok = true;
    UEIDAQMockupManager* manager = UEIDAQMockupManager::getInstance();
    manager->resetManager();
    //Set the devices configured in the IOM
    IOM* myIOM;
    ok &= (manager->GetIOM(1, &myIOM));
    if (ok){
        ok &= myIOM->SetDevice(0u, 0u, 0u, 0u, (uint16) 0x217); //Slot 0, devn 0, model 217
        ok &= myIOM->SetDevice(1u, 1u, 0u, 0u, (uint16) 0x217); //Slot 1, devn 1, model 217
    }
    GlobalObjectsDatabase::Instance()->GetStandardHeap();
    ConfigurationDatabase cdb;
    StreamString configStream = mapsConfig;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    ok &= parser.Parse();
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ok){
        ok &= cdb.MoveAbsolute("+UEIDAQ.+Maps.+Map1");
        ok &= cdb.Write("Class", "UEINoDeviceMap");
        ok &= cdb.MoveToRoot();
    }
    if (ok) {
        god->Purge();
        //Initialisation must not succeed with the default initialisation configuration
        ok = !god->Initialise(cdb);
    }
    if (ok){
        god->Purge();
    }
    return ok;
}
