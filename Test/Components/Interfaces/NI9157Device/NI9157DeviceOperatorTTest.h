/**
 * @file NI9157DeviceOperatorTTest.h
 * @brief Header file for class NI9157DeviceOperatorTTest
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
 * @details This header file contains the declaration of the class NI9157DeviceOperatorTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef NI9157DEVICEOPERATORTTEST_H_
#define NI9157DEVICEOPERATORTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "GAMSchedulerI.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
#include "NI9157DeviceOperatorT.h"
#include "NI9157DeviceTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the NI9157DeviceOperatorT methods
 */
template<typename T>
class NI9157DeviceOperatorTTest {
public:

    /**
     * @brief Constructor
     */
    NI9157DeviceOperatorTTest();

    /**
     * @brief Destructor
     */
    virtual ~NI9157DeviceOperatorTTest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestGetByteSize method
     */
    bool TestGetByteSize();

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestCompare method
     */
    bool TestCompare(T a,
                     T b);

    /**
     * @brief Tests the NI9157DeviceOperatorT::TestCopy method
     */
    bool TestCopy(T source);

    /**
     * @brief Tests the NI9157DeviceOperatorT::FindResource method
     */
    bool TestFindResource(const char8* resourceName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiRead method
     */
    bool TestNiRead(const char8 *readVarName,
                    const char8 *writeVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiWrite method
     */
    bool TestNiWrite(const char8 *readVarName,
                     const char8 *writeVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiReadFifo method
     */
    bool TestNiReadFifo(const char8 *fileName,
                        const char8* signature,
                        const char8 *readVarName,
                        const char8 *writeVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::NiWriteFifo method
     */
    bool TestNiWriteFifo(const char8 *fileName,
                         const char8* signature,
                         const char8 *readVarName,
                         const char8 *writeVarName);

    /**
     * @brief Tests the NI9157DeviceOperatorT::GetNI9157Device method
     */
    bool TestGetNI9157Device();

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
bool NI9157DeviceOperatorTTest<T>::TestFindResource(const char8* resourceName) {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
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

    ReferenceT<NI9157Device> interface;

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
        NI9157DeviceOperatorT<T> niOperator(interface);
        uint32 varDescriptor;
        ret &= (niOperator.FindResource(resourceName, varDescriptor) == 0);
    }

    ret &= interface->Close() == 0;
    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiRead(const char8 *readVarName,
                                              const char8 *writeVarName) {
                                                
    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
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

    ReferenceT<NI9157Device> interface;

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
        NI9157DeviceOperatorT<T> niOperator(interface);
        uint32 contrDescriptor;
        uint32 varDescriptor;

        ret &= (niOperator.FindResource(readVarName, varDescriptor) == 0);
        ret &= (niOperator.FindResource(writeVarName, contrDescriptor) == 0);

        uint64 rU64 = 1;
        ret &= (niOperator.NiWrite(contrDescriptor, &rU64) == 0);
        uint64 valueU64 = 0;
        ret &= (niOperator.NiRead(varDescriptor, &valueU64) == 0);
        ret &= (valueU64 == rU64);

    }

    ret &= interface->Reset() == 0;
    ret &= interface->Close() == 0;
    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiWrite(const char8 *readVarName,
                                               const char8 *writeVarName) {
    return TestNiRead(readVarName, writeVarName);
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestGetNI9157Device() {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
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

    ReferenceT<NI9157Device> interface;

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
        NI9157DeviceOperatorT<T> niOperator(interface);
        ret = (niOperator.GetNI9157Device() == interface);
    }

    ret &= interface->Close() == 0;
    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiReadFifo(const char8 *fileName,
                                                  const char8* signature,
                                                  const char8 *readVarName,
                                                  const char8 *writeVarName) {

    static const char8 * const config = ""
            "+NiDevice = {"
            "    Class = NI9157Device"
            "    NiRioDeviceName = RIO0"
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

    ret&=cdb.MoveAbsolute("+NiDevice");
    ret&=cdb.Write("NiRioGenFile", fileName);
    ret&=cdb.Write("NiRioGenSignature", signature);
    ret&=cdb.MoveToRoot();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ret) {
        god->Purge();
        ret = god->Initialise(cdb);
    }

    ReferenceT < NI9157Device > interface;

    if (ret) {
        interface = ObjectRegistryDatabase::Instance()->Find("NiDevice");
        ret = interface.IsValid();
    }
    if (ret) {
        ret = interface->IsOpened() == 1;
    }
    if (ret) {
        NI9157DeviceOperatorT<T> niOperator(interface);
        uint32 fifow;
        uint32 fifor;
        ret &= (niOperator.FindResource(writeVarName, fifow) == 0);
        ret &= (niOperator.FindResource(readVarName, fifor) == 0);

        const uint32 numberOfElements = 1000;

        uint32 oldSize = 0u;
        ret &= (interface->NiConfigureFifo(fifow, numberOfElements, oldSize) == 0);
        ret &= (interface->NiConfigureFifo(fifor, numberOfElements, oldSize) == 0);
        T dataw[numberOfElements];
        T datar[numberOfElements];

        for (uint32 i = 0u; i < numberOfElements; i++) {
            dataw[i] = (T) i;
        }
        if (ret) {
            if (ret) {
                ret = interface->Run() == 0;
            }
            if (ret) {
                ret = interface->IsRunning() == 1;
            }

            uint32 emptyElementsRemaining = 0u;
            ret = (niOperator.NiWriteFifo(fifow, dataw, numberOfElements, 0xffffffff, emptyElementsRemaining) == 0);

            if (ret) {
                uint32 elementsRemaining = 0u;
                ret = (niOperator.NiReadFifo(fifor, datar, numberOfElements, 0xffffffff, elementsRemaining) == 0);

                for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
                    ret = datar[i] == (T) i;
                    //printf("datar[%d]=%d\n", i, datar[i]);
                }
            }
        }
    }

    return ret;
}

template<typename T>
bool NI9157DeviceOperatorTTest<T>::TestNiWriteFifo(const char8 *fileName,
                                                   const char8* signature,
                                                   const char8 *readVarName,
                                                   const char8 *writeVarName) {
    return TestNiReadFifo(fileName, signature, readVarName, writeVarName);
}

#endif /* NI9157DEVICEOPERATORTTEST_H_ */
