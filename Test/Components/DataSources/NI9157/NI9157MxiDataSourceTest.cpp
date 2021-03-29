/**
 * @file NI9157MxiDataSourceTest.cpp
 * @brief Source file for class NI9157MxiDataSourceTest.
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
 * the class NI9157MxiDataSourceTest (public, protected, and private). Be
 * aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157MxiDataSourceTest.h"

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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
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

bool NI9157MxiDataSourceTest::TestInitialise(uint32 model) {

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

bool NI9157MxiDataSourceTest::TestInitialise_DefaultRunNi(uint32 model) {

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

bool NI9157MxiDataSourceTest::TestInitialise_False_NoNiDev(uint32 model) {

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

bool NI9157MxiDataSourceTest::TestInitialise_False_InvalidNiDevPath(uint32 model){

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

bool NI9157MxiDataSourceTest::TestSetConfiguredDatabase(uint32 model) {

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

bool NI9157MxiDataSourceTest::TestSetConfiguredDatabase_False_InvalidLabviewVar(uint32 model) {

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
    if (ret){
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals");
        ret &= cdb.CreateRelative("SomeFIFO");
        ConfigurationDatabase tempNode = cdb;
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
        ret = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ret) {
        application = god->Find("Application1");
        ret = application.IsValid();
    }
    if (ret) {
        ret = !application->ConfigureApplication();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSetConfiguredDatabase_False_InvalidType(uint32 model) {

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
        ret = cdb.MoveAbsolute("$Application1.+Functions.+GAMA.InputSignals.ticks_counter");
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
        ret = !application->ConfigureApplication();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestPrepareNextState(uint32 model) {

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

bool NI9157MxiDataSourceTest::TestSynchronise(uint32 model) {

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

bool NI9157MxiDataSourceTest::TestSynchronise_Variables(uint32 model) {

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

bool NI9157MxiDataSourceTest::TestSynchronise_FIFOs(uint32 model) {

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
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Error at mem[%u]=%u != %u", i, static_cast<uint32>(mem[0]), ((2000 * i) + 1));
                }
            }
        }
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestSynchronise_InitialPatterns(uint32 model) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream =  multiIOConfig4;
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
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.Signals.FIFO1_U64_R");
        ret &= cdb.Write("InitialPattern", 1);
        ret &= cdb.MoveToRoot();
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1.Signals.IndU32");
        ret &= cdb.Write("InitialPattern", 0);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.Signals.FIFO0_U64_W");
        ret &= cdb.Write("InitialPattern", 1);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.Signals.ContU32");
        ret &= cdb.Write("InitialPattern", 1);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.Signals.ControlBool_IndBool");
        ret &= cdb.Write("InitialPattern", 0);
        ret &= cdb.MoveToRoot();
        ret &= cdb.MoveAbsolute("$Application1.+Data.+Drv1.Signals.ControlBool_ContBool");
        ret &= cdb.Write("InitialPattern", 0);
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
    ReferenceT < NI9157MxiDataSourceTestGAM2 > gam1;
    if (ret) {
        gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam1.IsValid();
    }
    ReferenceT< IOGAM > gam2;
    if (ret) {
        gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gam2.IsValid();
    }
    ReferenceT< IOGAM > gam3;
    if (ret) {
        gam3 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMC");
        ret = gam3.IsValid();
    }
    ReferenceT< IOGAM > gam4;
    if (ret) {
        gam4 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMD");
        ret = gam4.IsValid();
    }
    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > brokerOut1;
    ReferenceT < MemoryMapInputBroker > brokerIn2;
    ReferenceT < MemoryMapOutputBroker > brokerOut2;
    ReferenceT < MemoryMapInputBroker > brokerIn3;
    ReferenceT < MemoryMapOutputBroker > brokerOut3;
    ReferenceT < MemoryMapInputBroker > brokerIn4;
    ReferenceT < MemoryMapOutputBroker > brokerOut4;
    ReferenceContainer inputBrokers1, outputBrokers1;
    ReferenceContainer inputBrokers2, outputBrokers2;
    ReferenceContainer inputBrokers3, outputBrokers3;
    ReferenceContainer inputBrokers4, outputBrokers4;
    if (ret) {
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
            ret = gam3->GetInputBrokers(inputBrokers3);
        }
        if (ret) {
            ret = gam3->GetOutputBrokers(outputBrokers3);
        }
        if (ret) {
            ret = gam4->GetInputBrokers(inputBrokers4);
        }
        if (ret) {
            ret = gam4->GetOutputBrokers(outputBrokers4);
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
        if (ret) {
            brokerIn3 = inputBrokers3.Get(0);
            ret = brokerIn3.IsValid();
        }
        if (ret) {
            brokerOut3 = outputBrokers3.Get(0);
            ret = brokerOut3.IsValid();
        }
        if (ret) {
            brokerIn4 = inputBrokers4.Get(0);
            ret = brokerIn4.IsValid();
        }
        if (ret) {
            brokerOut4 = outputBrokers4.Get(0);
            ret = brokerOut4.IsValid();
        }
    }

    if (ret) {
        brokerSync->Execute();
    }
    
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

static void NI9157DeviceRunCallback(const void * const params) {
    
    ReferenceT<NI9157Device> interface;
    bool ret = false;

    interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
    ret = interface.IsValid();
    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "NI9157DeviceRunCallback: Waiting 2s to call Run()...");
        Sleep::MSec(2000);
        ret = (interface->Run() == 0);
        if (ret) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "NI9157DeviceRunCallback: NiDevice->Run()");
        }
    }
}

bool NI9157MxiDataSourceTest::TestSynchronise_BlockIfNotRunning (uint32 model) {

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
        ret = cdb.MoveAbsolute("$Application1.+Data.+Drv1");
        ret &= cdb.Write("RunNi", 0);
        ret &= cdb.Write("BlockIfNotRunning", 1);
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
    ReferenceT < MemoryMapSynchronisedInputBroker > brokerSync;
    ReferenceT < MemoryMapOutputBroker > brokerOut1;
    ReferenceContainer inputBrokers1, outputBrokers1;
    if (ret) {
        ret = gam1->GetInputBrokers(inputBrokers1);
        if (ret) {
            ret = gam1->GetOutputBrokers(outputBrokers1);
        }
        if (ret) {
            brokerSync = inputBrokers1.Get(0);
            ret = brokerSync.IsValid();
        }
        if (ret) {
            brokerOut1 = outputBrokers1.Get(0);
            ret = brokerOut1.IsValid();
        }
    }
    if (ret) {
        ThreadIdentifier tid = Threads::BeginThread(NI9157DeviceRunCallback);
        brokerSync->Execute();
        Threads::Kill(tid);
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestAsyncRead(uint32 model) {

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
    int32 delta = 0u;
    int32 deltaExpected = 40000u;
    int32 tol = 5000u;
    int32 store = 0u;
    StreamString varName = "ticks_counter";
    uint64 varValue = 0u;
    for (uint32 i = 0u; (i < nReads) && (ret); i++) {
        ret = dataSource->AsyncRead(varName, varValue);
        if (i > 0u) {
            delta = (static_cast<int32>(varValue) - store);
            ret = ((delta - deltaExpected) < tol) || ((delta - deltaExpected) > -tol);
        }
        Sleep::Sec(1e-3);
        store = static_cast<int32>(varValue);
    }
    ret &= !dataSource->AsyncRead("someVariable", varValue);

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestAsyncWrite(uint32 model) {

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
    uint32 varValue = 0u;
    if (ret) {
        uint32 nWrites = 10;
        for (uint32 n = 0u; (n < nWrites) && ret; n++) {
            for (uint32 i = 0u; (i < nPoints) && ret; i++) {
                StreamString varName = "sample_value";
                varValue = (uint32)((mem[i] + 2.5) * (16383 / 5));
                ret = dataSource->AsyncWrite(varName, varValue);
                Sleep::Sec(1e-3);
            }
        }
    }
    ret &= !dataSource->AsyncWrite("someVariable", varValue);

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool NI9157MxiDataSourceTest::TestReset(uint32 model) {

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
