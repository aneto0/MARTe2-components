/**
 * @file NI9157DeviceOperatorTTest.h
 * @brief Header file for class NI9157DeviceOperatorTTest.
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
 * @details This header file contains the declaration of the class
 * NI9157DeviceOperatorTTest with all of its public, protected and private
 * members. It may also include definitions for inline methods which need to
 * be visible to the compiler.
 */

#ifndef NI9157DEVICEOPERATORTTEST_H_
#define NI9157DEVICEOPERATORTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "NI9157DeviceOperatorT.h"
#include "NI9157DeviceTest.h"
#include "NI9157DeviceTestsAndFwSettings.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157DeviceOperatorT methods.
 */
template<typename T>
class NI9157DeviceOperatorTTest {
public:

    /**
     * @brief Constructor.
     */
    NI9157DeviceOperatorTTest();

    /**
     * @brief Destructor.
     */
    virtual ~NI9157DeviceOperatorTTest();

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the IsValid method.
     */
    bool TestIsValid();

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestGetByteSize method.
     */
    bool TestGetByteSize();

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestCompare method.
     */
    bool TestCompare(T a,
                     T b);

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestCopy method.
     */
    bool TestCopy(T source);

    /**
     * @brief Tests the NI9157DeviceOperatorT::FindResource method.
     */
    bool TestFindResource(uint32 model,
                    const char8* resourceName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiWrite method.
     */
    bool TestNiWrite(uint32 model,
                    const char8 *writeVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiRead method.
     */
    bool TestNiRead(uint32 model,
                    const char8 *writeVarName,
                    const char8 *readVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiWriteFifo method.
     */
    bool TestNiWriteFifo(uint32 model,
                        const char8 *writeVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiReadFifo methods.
     */
    bool TestNiReadFifo(uint32 model,
                        const char8 *writeVarName,
                        const char8 *readVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::GetNI9157Device method.
     */
    bool TestGetNI9157Device(uint32 model);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
NI9157DeviceOperatorTTest<T>::NI9157DeviceOperatorTTest() {
}

template<typename T>
NI9157DeviceOperatorTTest<T>::~NI9157DeviceOperatorTTest() {
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestConstructor() {
    NI9157DeviceOperatorT<T> test;
    return !test.IsValid();
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestIsValid() {
    NI9157DeviceOperatorT<T> test;
    return !test.IsValid();
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestGetByteSize() {
    NI9157DeviceOperatorT<T> test;
    return test.GetByteSize() == sizeof(T);
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestCompare(T a,
                                               T b) {
    int32 isEqual = 0;
    if (a < b) {
        isEqual = -1;
    }
    if (a > b) {
        isEqual = 1;
    }
    NI9157DeviceOperatorT<T> test;
    return test.Compare((uint8 *) &a, (uint8 *) &b) == isEqual;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestCopy(T source) {
    T dest;
    NI9157DeviceOperatorT<T> test;
    test.Copy((uint8 *) &dest, (uint8 *) &source);
    return dest == source;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestFindResource(uint32 model,
                                            const char8* resourceName) {

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
            NI9157DeviceOperatorT<T> niOperator(interface);
            uint32 varDescriptor;
            ret = (niOperator.FindResource(resourceName, varDescriptor) == 0);
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiWrite(uint32 model,
                                            const char8 *writeVarName) {

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
            NI9157DeviceOperatorT<T> niOperator(interface);
            uint32 contrDescriptor;
            ret = (niOperator.FindResource(writeVarName, contrDescriptor) == 0);
            if (ret) {
                T writeVal = 1;
                ret = (niOperator.NiWrite(contrDescriptor, &writeVal) == 0);
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiRead(uint32 model,
                                            const char8 *writeVarName,
                                            const char8 *readVarName) {

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
            NI9157DeviceOperatorT<T> niOperator(interface);
            uint32 contrDescriptor;
            uint32 varDescriptor;
            ret = (niOperator.FindResource(writeVarName, contrDescriptor) == 0);
            ret &= (niOperator.FindResource(readVarName, varDescriptor) == 0);
            if (ret) {
                T writeVal = 1;
                ret = (niOperator.NiWrite(contrDescriptor, &writeVal) == 0);
                if (ret) {
                    T readVal = 0;
                    ret &= (niOperator.NiRead(varDescriptor, &readVal) == 0);
                    ret &= (readVal == writeVal);
                }
            }
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiWriteFifo(uint32 model,
                                                  const char8 *writeVarName) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig2;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        StreamString name = "";
        StreamString pathAndFile = "";
        StreamString signature = "";
        ret = cdb.MoveAbsolute("+NiDevice");
        if (ret) { 
            if (typeid(T) == typeid(bool)) {
                name.Printf("%s", boolFirmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, boolFirmware[nParams*model + 1]);
                signature.Printf("%s", boolFirmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(uint8)) {
                name.Printf("%s", u8Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u8Firmware[nParams*model + 1]);
                signature.Printf("%s", u8Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int8)) {
                name.Printf("%s", i8Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i8Firmware[nParams*model + 1]);
                signature.Printf("%s", i8Firmware[nParams*model + 2]);
                ret = true;
 	    }
            else if (typeid(T) == typeid(uint16)) {
                name.Printf("%s", u16Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u16Firmware[nParams*model + 1]);
                signature.Printf("%s", u16Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int16)) {
                name.Printf("%s", i16Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i16Firmware[nParams*model + 1]);
                signature.Printf("%s", i16Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(uint32)) {
                name.Printf("%s", u32Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u32Firmware[nParams*model + 1]);
                signature.Printf("%s", u32Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int32)) {
                name.Printf("%s", i32Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i32Firmware[nParams*model + 1]);
                signature.Printf("%s", i32Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(uint64)) {
                name.Printf("%s", u64Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u64Firmware[nParams*model + 1]);
                signature.Printf("%s", u64Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int64)) {
                name.Printf("%s", i64Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i64Firmware[nParams*model + 1]);
                signature.Printf("%s", i64Firmware[nParams*model + 2]);
                ret = true;
            }
            else {            
                ret = false;
            }
        }
        if (ret) {
            ret = cdb.Write("NiRioDeviceName", name.Buffer());
            ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
            ret &= cdb.Write("NiRioGenSignature", signature.Buffer());
            ret &= cdb.MoveRelative("Configuration");
            // Change....
            ret &= cdb.MoveToRoot();
        }
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    if (ret) {
        ReferenceT < NI9157Device > interface;
        if (ret) {
            interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
            ret = interface.IsValid();
        }
        if (ret) {
            ret = interface->IsOpened() == 1;
            if (ret) {
                NI9157DeviceOperatorT<T> niOperator(interface);
                uint32 fifow;
                ret = (niOperator.FindResource(writeVarName, fifow) == 0);
                if (ret) {
                    const uint32 numberOfElements = 1000;
                    T dataw[numberOfElements];
                    for (uint32 i = 0u; i < numberOfElements; i++) {
                        dataw[i] = (T) i;
                    }
                    if (ret) {
                        ret = interface->Run() == 0;
                    }
                    if (ret) {
                        ret = interface->IsRunning() == 1;
                    }
                    if (ret) {
                        uint32 emptyElementsRemaining = 0u;
                        ret = (niOperator.NiWriteFifo(fifow, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                    }
                }
            }
            ret &= interface->Reset() == 0;
            ret &= interface->Close() == 0;
        }
    }
    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiReadFifo(uint32 model,
                                                  const char8 *writeVarName,
                                                  const char8 *readVarName) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = fifoLoopConfig2;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);
    bool ret = parser.Parse();

    if (ret) {
        StreamString name = "";
        StreamString pathAndFile = "";
        StreamString signature = "";
        ret = cdb.MoveAbsolute("+NiDevice");
        if (ret) {
            if (typeid(T) == typeid(bool)) {
                name.Printf("%s", boolFirmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, boolFirmware[nParams*model + 1]);
                signature.Printf("%s", boolFirmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(uint8)) {
                name.Printf("%s", u8Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u8Firmware[nParams*model + 1]);
                signature.Printf("%s", u8Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int8)) {
                name.Printf("%s", i8Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i8Firmware[nParams*model + 1]);
                signature.Printf("%s", i8Firmware[nParams*model + 2]);
                ret = true;
 	    }
            else if (typeid(T) == typeid(uint16)) {
                name.Printf("%s", u16Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u16Firmware[nParams*model + 1]);
                signature.Printf("%s", u16Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int16)) {
                name.Printf("%s", i16Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i16Firmware[nParams*model + 1]);
                signature.Printf("%s", i16Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(uint32)) {
                name.Printf("%s", u32Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u32Firmware[nParams*model + 1]);
                signature.Printf("%s", u32Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int32)) {
                name.Printf("%s", i32Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i32Firmware[nParams*model + 1]);
                signature.Printf("%s", i32Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(uint64)) {
                name.Printf("%s", u64Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, u64Firmware[nParams*model + 1]);
                signature.Printf("%s", u64Firmware[nParams*model + 2]);
                ret = true;
            }
            else if (typeid(T) == typeid(int64)) {
                name.Printf("%s", i64Firmware[nParams*model + 0]);
                pathAndFile.Printf("%s/%s", firmwarePath, i64Firmware[nParams*model + 1]);
                signature.Printf("%s", i64Firmware[nParams*model + 2]);
                ret = true;
            }
            else {            
                ret = false;
            }
        }
        if (ret) {
            ret = cdb.Write("NiRioDeviceName", name.Buffer());
            ret &= cdb.Write("NiRioGenFile", pathAndFile.Buffer());
            ret &= cdb.Write("NiRioGenSignature", signature.Buffer());
            ret &= cdb.MoveRelative("Configuration");
            // Change....
            ret &= cdb.MoveToRoot();
        }
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }
    if (ret) {
        ReferenceT < NI9157Device > interface;
        if (ret) {
            interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
            ret = interface.IsValid();
        }
        if (ret) {
            ret = interface->IsOpened() == 1;
            if (ret) {
                NI9157DeviceOperatorT<T> niOperator(interface);
                uint32 fifow;
                uint32 fifor;
                ret = (niOperator.FindResource(writeVarName, fifow) == 0);
                ret &= (niOperator.FindResource(readVarName, fifor) == 0);
                if (ret) {
                    const uint32 numberOfElements = 1000;
                    uint32 oldSize = 0u;
                    ret = (interface->NiConfigureFifo(fifow, numberOfElements, oldSize) == 0);
                    ret &= (interface->NiConfigureFifo(fifor, numberOfElements, oldSize) == 0);
                    if (ret) {
                        T dataw[numberOfElements];
                        T datar[numberOfElements];
                        for (uint32 i = 0u; i < numberOfElements; i++) {
                            dataw[i] = (T) i;
                        }
                        if (ret) {
                            ret = interface->Run() == 0;
                        }
                        if (ret) {
                            ret = interface->IsRunning() == 1;
                        }
                        if (ret) {
                            uint32 emptyElementsRemaining = 0u;
                            ret = (niOperator.NiWriteFifo(fifow, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);
                            if (ret) {
                                uint32 elementsRemaining = 0u;
                                ret = (niOperator.NiReadFifo(fifor, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);
                                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                                    ret = (datar[i] == (T) i);
                                    if(!ret){
                                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Reading FIFO1_U8_R[%!]=%! != %!", (T)i, datar[i], (T)i);
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
    }

    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestGetNI9157Device(uint32 model) {

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
            NI9157DeviceOperatorT<T> niOperator(interface);
            ret = (niOperator.GetNI9157Device() == interface);
        }
        ret &= interface->Reset() == 0;
        ret &= interface->Close() == 0;
    }

    return ret;
}

#endif /* NI9157DEVICEOPERATORTTEST_H_ */
