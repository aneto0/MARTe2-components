/**
 * @file NI9157DeviceOperatorTITest.cpp
 * @brief Source file for class NI9157DeviceOperatorTITest.
 * @date 22/03/2021
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
 * the class NI9157DeviceOperatorTITest (public, protected, and private). Be 
 * aware that some methods, such as those inline could be defined on the header
 * file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157DeviceOperatorTITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
NI9157DeviceOperatorTITest::NI9157DeviceOperatorTITest() {
}

NI9157DeviceOperatorTITest::~NI9157DeviceOperatorTITest() {
}

bool NI9157DeviceOperatorTITest::TestConstructor() {

	NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    ReferenceT<NI9157Device> niDevice(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        niDeviceOperator = creator->Create(niDevice);
        ret = (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *));
    }
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }

	return true;
}

bool NI9157DeviceOperatorTITest::TestIsValid() {

	NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    ReferenceT<NI9157Device> niDevice(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        niDeviceOperator = creator->Create(niDevice);
        ret = niDeviceOperator->IsValid();
    }
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }

	return true;
}

bool NI9157DeviceOperatorTITest::TestGetByteSize() {

	NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    ReferenceT<NI9157Device> niDevice(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        niDeviceOperator = creator->Create(niDevice);
        ret = niDeviceOperator->IsValid();
        ret &= (niDeviceOperator->GetByteSize() == sizeof(UnsignedInteger8Bit));
    }
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }

	return true;
}


bool NI9157DeviceOperatorTITest::TestGetNI9157Device() {

	NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    ReferenceT<NI9157Device> niDevice(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        niDeviceOperator = creator->Create(niDevice);
        ret = niDeviceOperator->IsValid();
        ret &= (niDeviceOperator->GetByteSize() == sizeof(UnsignedInteger8Bit));
        ret &= (niDeviceOperator->GetNI9157Device() == niDevice);
    }
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }

	return true;
}

bool NI9157DeviceOperatorTITest::TestCompare() {

	NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    ReferenceT<NI9157Device> niDevice(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        niDeviceOperator = creator->Create(niDevice);
        ret = niDeviceOperator->IsValid();
        if (ret) {
            uint8 aValue = 0u;
            uint8 bValue = 8u;
            uint8 cValue = 8u;
            ret = (niDeviceOperator->Compare((uint8 *) &aValue, (uint8 *) &bValue) == -1);
            ret &= (niDeviceOperator->Compare((uint8 *) &bValue, (uint8 *) &aValue) == 1);
            ret &= (niDeviceOperator->Compare((uint8 *) &bValue, (uint8 *) &cValue) == 0);
        }
    }
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }

    return ret;
}

bool NI9157DeviceOperatorTITest::TestCopy() {
	
    NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger8Bit);
    ReferenceT<NI9157Device> niDevice(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
    if (ret) {
        niDeviceOperator = creator->Create(niDevice);
        ret = niDeviceOperator->IsValid();
        if (ret) {
            uint8 source = 8u;
            uint8 destination = 0u;
            niDeviceOperator->Copy((uint8 *) &destination, (uint8 *) &source);
            ret = (destination == source);
        }
    }
    if (niDeviceOperator != NULL_PTR(NI9157DeviceOperatorTI *)) {
        delete niDeviceOperator;
        niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    }

    return ret;
}

bool NI9157DeviceOperatorTITest::TestFindResource(uint32 model) {

    NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NULL_PTR(CreateNI9157DeviceOperatorI *);
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
    ReferenceT<NI9157Device> interface;
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
            creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger64Bit);
            ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
            if (ret) {
                niDeviceOperator = creator->Create(interface);
                const char8 * const resourceName = "FIFO1_U64_R";
                uint32 varDescriptor;
                ret = (niDeviceOperator->FindResource(resourceName, varDescriptor) == 0);
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceOperatorTITest::TestNiWrite(uint32 model) {

    NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NULL_PTR(CreateNI9157DeviceOperatorI *);
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
    ReferenceT<NI9157Device> interface;
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
        if (ret) {
            creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger64Bit);
            ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
            if (ret) {
                niDeviceOperator = creator->Create(interface);
                const char8 * const contrName = "ContU64";
                uint32 contrDescriptor;
                ret = (niDeviceOperator->FindResource(contrName, contrDescriptor) == 0);
                if (ret) {
                    uint64 writeVal = 1ull;
                    ret = (niDeviceOperator->NiWrite(contrDescriptor, &writeVal) == 0);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceOperatorTITest::TestNiRead(uint32 model) {

    NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NULL_PTR(CreateNI9157DeviceOperatorI *);
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
    ReferenceT<NI9157Device> interface;
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
        if (ret) {
            creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger64Bit);
            ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
            if (ret) {
                niDeviceOperator = creator->Create(interface);
                const char8 * const contrName = "ContU64";
                const char8 * const indName = "IndU64";
                uint32 contrDescriptor;
                uint32 indDescriptor;
                ret = (niDeviceOperator->FindResource(contrName, contrDescriptor) == 0);
                ret &= (niDeviceOperator->FindResource(indName, indDescriptor) == 0);
                if (ret) {
                    uint64 writeVal = 1ull;
                    ret = (niDeviceOperator->NiWrite(contrDescriptor, &writeVal) == 0);
                    if (ret) {
                        uint64 readVal = 0ull;
                        ret = (niDeviceOperator->NiRead(indDescriptor, &readVal) == 0);
                        ret &= (readVal == writeVal); 
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceOperatorTITest::TestNiWriteFifo(uint32 model) {

    NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NULL_PTR(CreateNI9157DeviceOperatorI *);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig2;
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
    ReferenceT<NI9157Device> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }
    if (ret) {
        creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger64Bit);
        ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
        if (ret) {
            niDeviceOperator = creator->Create(interface);
            const char8 * const contrName = "FIFO0_U64_W";
            uint32 contrDescriptor;
            ret = (niDeviceOperator->FindResource(contrName, contrDescriptor) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 emptyElementsRemaining = 0u;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(contrDescriptor, numberOfElements, oldSize) == 0);
                if (ret) {
                    uint64 writeVal[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        writeVal[i] = static_cast<uint64>(i);
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        ret = (niDeviceOperator->NiWriteFifo(contrDescriptor, writeVal, numberOfElements, 0xffffffffu, emptyElementsRemaining) == 0);
                    }
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

bool NI9157DeviceOperatorTITest::TestNiReadFifo(uint32 model) {

    NI9157DeviceOperatorTI *niDeviceOperator = NULL_PTR(NI9157DeviceOperatorTI *);
    CreateNI9157DeviceOperatorI *creator = NULL_PTR(CreateNI9157DeviceOperatorI *);
    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig2;
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
    ReferenceT<NI9157Device> interface;
    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }
    if (ret) {
        creator = NI9157DeviceOperatorDatabase::GetCreateNI9157DeviceOperator(UnsignedInteger64Bit);
        ret = (creator != NULL_PTR(CreateNI9157DeviceOperatorI *));
        if (ret) {
            niDeviceOperator = creator->Create(interface);
            const char8 * const contrName = "FIFO0_U64_W";
            const char8 * const indName = "FIFO1_U64_R";
            uint32 contrDescriptor;
            uint32 indDescriptor;
            ret = (niDeviceOperator->FindResource(contrName, contrDescriptor) == 0);
            ret &= (niDeviceOperator->FindResource(indName, indDescriptor) == 0);
            if (ret) {
                const uint32 numberOfElements = 1000;
                uint32 emptyElementsRemaining = 0u;
                uint32 oldSize = 0u;
                ret = (interface->NiConfigureFifo(contrDescriptor, numberOfElements, oldSize) == 0);
                ret &= (interface->NiConfigureFifo(indDescriptor, numberOfElements, oldSize) == 0);
                if (ret) {
                    uint64 writeVal[numberOfElements];
                    uint64 readVal[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        writeVal[i] = static_cast<uint64>(i);
                        readVal[i] = 0ull;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        ret = (niDeviceOperator->NiWriteFifo(contrDescriptor, writeVal, numberOfElements, 0xffffffffu, emptyElementsRemaining) == 0);
                        if (ret) {
                            uint32 elementsRemaining = 0u;
                            ret = (niDeviceOperator->NiReadFifo(indDescriptor, readVal, numberOfElements, 0xffffffffu, elementsRemaining) == 0);
                            for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                ret = (readVal[i] == static_cast<uint64>(i));
                                if (!ret) {
                                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_U64_R[%u]=%u != %u", i, static_cast<uint64>(readVal[i]), i);
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
