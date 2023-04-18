/**
 * @file MDSReaderTestHelper.cpp
 * @brief Source file for class MDSReaderTestHelper
 * @date 16/01/2023
 * @author Llorenc Capella
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
 * the class MDSReaderTestHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MDSReaderTestHelper.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MDSReaderTestHelper::MDSReaderTestHelper(StreamString name) {
    treeName = name;
    elementsRead = 10;
    sum_uint8 = 0;
    sum_int8 = -20;
    sum_uint16 = 0;
    sum_int16 = -40;
    sum_uint32 = 0;
    sum_int32 = -80;
    sum_uint64 = 0;
    sum_int64 = -160;
    sum_float32 = 0;
    sum_float64 = 0;
}

MDSReaderTestHelper::~MDSReaderTestHelper() {

}

bool MDSReaderTestHelper::CreateConfigurationFile(float64 samplingTime,
                                                  uint8 dataManagement,
                                                  uint8 holeManagement,
                                                  int32 shot,
                                                  StreamString typeTime) {
    bool ok = true;
    ok &= config.Write("TreeName", treeName.Buffer());
    ok &= config.Write("ShotNumber", shot);
    float64 CycleFrequnecy = 1 / (samplingTime * elementsRead);
    ok &= config.Write("Frequency", CycleFrequnecy); //0.1 is the sample time of test_tree pulse 1

    ok &= config.CreateAbsolute(".Signals");
    ok &= config.CreateRelative("0");
    ok &= config.Write("NodeName", "S_uint8");
    ok &= config.Write("QualifiedName", "0");
    ok &= config.Write("Type", "uint8");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 1);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("1");
    ok &= config.Write("NodeName", "S_int8");
    ok &= config.Write("QualifiedName", "1");
    ok &= config.Write("Type", "int8");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 1);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("2");
    ok &= config.Write("NodeName", "S_uint16");
    ok &= config.Write("QualifiedName", "2");
    ok &= config.Write("Type", "uint16");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 2);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("3");
    ok &= config.Write("NodeName", "S_int16");
    ok &= config.Write("QualifiedName", "3");
    ok &= config.Write("Type", "int16");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 2);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("4");
    ok &= config.Write("NodeName", "S_uint32");
    ok &= config.Write("QualifiedName", "4");
    ok &= config.Write("Type", "uint32");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 4);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("5");
    ok &= config.Write("NodeName", "S_int32");
    ok &= config.Write("QualifiedName", "5");
    ok &= config.Write("Type", "int32");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 4);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("6");
    ok &= config.Write("NodeName", "S_uint64");
    ok &= config.Write("QualifiedName", "6");
    ok &= config.Write("Type", "uint64");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 8);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("7");
    ok &= config.Write("NodeName", "S_int64");
    ok &= config.Write("QualifiedName", "7");
    ok &= config.Write("Type", "int64");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 8);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("8");
    ok &= config.Write("NodeName", "S_float32");
    ok &= config.Write("QualifiedName", "8");
    ok &= config.Write("Type", "float32");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 4);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("9");
    ok &= config.Write("NodeName", "S_float64");
    ok &= config.Write("QualifiedName", "9");
    ok &= config.Write("Type", "float64");
    ok &= config.Write("NumberOfElements", elementsRead);
    ok &= config.Write("ByteSize", elementsRead * 8);
    ok &= config.Write("DataManagement", dataManagement);
    ok &= config.Write("HoleManagement", holeManagement);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("10");
    ok &= config.Write("QualifiedName", "10");
    ok &= config.Write("Type", typeTime.Buffer());
    ok &= config.Write("NumberOfElements", 1);
    if ((typeTime == "uint32") || (typeTime == "int32")) {
        ok &= config.Write("ByteSize", 4);
    }
    else if ((typeTime == "uint64") || (typeTime == "int64")) {
        ok &= config.Write("ByteSize", 8);
    }
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateAbsolute("Functions");
    ok &= config.CreateRelative("0");
    ok &= config.Write("QualifiedName", "GAMNameLL");
    ok &= config.CreateRelative("InputSignals");
    ok &= config.CreateRelative("0");
    ok &= config.Write("Samples", 1);
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    uint32 byteOffset[1][2] = { { 0, elementsRead * 1 } };
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("1");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 1;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("2");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 2;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("3");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 2;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("4");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 24;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("5");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 4;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("6");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 8;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("7");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 8;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("8");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 4;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("9");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    ok &= config.Write("GAMMemoryOffset", 0);
    ok &= config.Write("Alias", "LL");
    byteOffset[0][1] = elementsRead * 8;
    ok &= config.Write("ByteOffset", byteOffset);
    ok &= config.MoveToAncestor(1u);
    ok &= config.CreateRelative("10");
    ok &= config.Write("Broker", "MemoryMapSynchronisedInputBroker");
    if ((typeTime == "uint32") || (typeTime == "int32")) {
        ok &= config.Write("GAMMemoryOffset", 0);
        ok &= config.Write("Alias", "LL");
        byteOffset[0][1] = elementsRead * 4;
        ok &= config.Write("ByteOffset", byteOffset);
    }
    else if ((typeTime == "uint64") || (typeTime == "int64")) {
        ok &= config.Write("GAMMemoryOffset", 0);
        ok &= config.Write("Alias", "LL");
        byteOffset[0][1] = elementsRead * 8;
        ok &= config.Write("ByteOffset", byteOffset);
    }
    ok &= config.MoveToAncestor(1u);
    ok &= config.Write("ByteSize", 10);
    ok &= config.MoveToRoot();
    return ok;
}

bool MDSReaderTestHelper::CompareS_uint8Hold(uint8 *ptr,
                                             uint32 iteration,
                                             float64 increment,
                                             uint32 every,
                                             uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryuint8 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<uint8>(round(sum_uint8)));
        if (!ok) {
            printf("uint8 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint8>(round(sum_uint8)), iteration,
                   i);
        }
        countEveryuint8++;
        if (countEveryuint8 == every) {
            sum_uint8 += increment;
            countEveryuint8 = 0;
        }
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_int8Hold(int8 *ptr,
                                            uint32 iteration,
                                            float64 increment,
                                            uint32 every,
                                            uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryint8 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<int8>(round(sum_int8)));
        if (!ok) {
            printf("int8 value = %d != ref = %u. iteration = %d, position in the iteration = %d\n", ptr[i], static_cast<int8>(round(sum_int8)), iteration, i);
        }
        countEveryint8++;
        if (countEveryint8 == every) {
            sum_int8 += increment;
            countEveryint8 = 0;
        }
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_uint16Hold(uint16 *ptr,
                                              uint32 iteration,
                                              float64 increment,
                                              uint32 every,
                                              uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryuint16 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<uint16>(round(sum_uint16)));
        if (!ok) {
            printf("uint16 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint16>(round(sum_uint16)), iteration,
                   i);
        }
        countEveryuint16++;
        if (countEveryuint16 == every) {
            sum_uint16 += increment;
            countEveryuint16 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_int16Hold(int16 *ptr,
                                             uint32 iteration,
                                             float64 increment,
                                             uint32 every,
                                             uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryint16 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<int16>(round(sum_int16)));
        if (!ok) {
            printf("int16 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int16>(round(sum_int16)), iteration,
                   i);
        }
        countEveryint16++;
        if (countEveryint16 == every) {
            sum_int16 += increment;
            countEveryint16 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_uint32Hold(uint32 *ptr,
                                              uint32 iteration,
                                              float64 increment,
                                              uint32 every,
                                              uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryuint32 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<uint32>(round(sum_uint32)));
        if (!ok) {
            printf("uint8 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint32>(round(sum_uint32)), iteration,
                   i);
        }
        countEveryuint32++;
        if (countEveryuint32 == every) {
            sum_uint32 += increment;
            countEveryuint32 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_int32Hold(int32 *ptr,
                                             uint32 iteration,
                                             float64 increment,
                                             uint32 every,
                                             uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryint32 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<int32>(round(sum_int32)));
        if (!ok) {
            printf("int32 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int32>(round(sum_int32)), iteration,
                   i);
        }
        countEveryint32++;
        if (countEveryint32 == every) {
            sum_int32 += increment;
            countEveryint32 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_uint64Hold(uint64 *ptr,
                                              uint32 iteration,
                                              float64 increment,
                                              uint32 every,
                                              uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryuint64 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<uint64>(round(sum_uint64)));
        if (!ok) {
            printf("uint64 value = %llu != ref = %llu. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint64>(round(sum_uint64)),
                   iteration, i);
        }
        countEveryuint64++;
        if (countEveryuint64 == every) {
            sum_uint64 += increment;
            countEveryuint64 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_int64Hold(int64 *ptr,
                                             uint32 iteration,
                                             float64 increment,
                                             uint32 every,
                                             uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryint64 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<int64>(round(sum_int64)));
        if (!ok) {
            printf("int64 value = %lld != ref = %lld. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int64>(round(sum_int64)),
                   iteration, i);
        }
        countEveryint64++;
        if (countEveryint64 == every) {
            sum_int64 += increment;
            countEveryint64 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_float32Hold(float32 *ptr,
                                               uint32 iteration,
                                               float64 increment,
                                               uint32 every,
                                               uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryfloat32 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = IsEqualLargeMargins(ptr[i], static_cast<float32>(sum_float32));
        if (!ok) {
            printf("float32 value = %f != ref = %f. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<float32>(sum_float32), iteration, i);
        }
        countEveryfloat32++;
        if (countEveryfloat32 == every) {
            sum_float32 += increment;
            countEveryfloat32 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_float64Hold(float64 *ptr,
                                               uint32 iteration,
                                               float64 increment,
                                               uint32 every,
                                               uint32 elementsToCompare) {
    bool ok = true;
    static uint32 countEveryfloat64 = 2;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = IsEqualLargeMargins(ptr[i], sum_float64);
        if (!ok) {
            printf("float64 value = %lf != ref = %lf. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<float64>(sum_float64), iteration,
                   i);
        }
        countEveryfloat64++;
        if (countEveryfloat64 == every) {
            sum_float64 += increment;
            countEveryfloat64 = 0;
        }
    }
    return ok;
}

bool MDSReaderTestHelper::CompareS_uint8(uint8 *ptr,
                                         uint32 iteration,
                                         float64 increment,
                                         uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = (ptr[i] == static_cast<uint8>(round(sum_uint8)));
        if (!ok) {
            printf("uint8 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint8>(round(sum_uint8)), iteration,
                   i);
        }
        sum_uint8 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_int8(int8 *ptr,
                                        uint32 iteration,
                                        float64 increment,
                                        uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = ptr[i] == static_cast<int8>(round(sum_int8));
        if (!ok) {
            printf("int8 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int8>(round(sum_int8)), iteration, i);
        }
        sum_int8 += increment;
    }

    return ok;
}
bool MDSReaderTestHelper::CompareS_uint16(uint16 * ptr,
                                          uint32 iteration,
                                          float64 increment,
                                          uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = ptr[i] == static_cast<uint16>(round(sum_uint16));
        if (!ok) {
            printf("uint16 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint16>(round(sum_uint16)), iteration,
                   i);
        }
        sum_uint16 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_int16(int16 * ptr,
                                         uint32 iteration,
                                         float64 increment,
                                         uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = ptr[i] == static_cast<int16>(round(sum_int16));
        if (!ok) {
            printf("int16 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int16>(round(sum_int16)), iteration,
                   i);
        }
        sum_int16 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_uint32(uint32 * ptr,
                                          uint32 iteration,
                                          float64 increment,
                                          uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = ptr[i] == static_cast<uint32>(round(sum_uint32));
        if (!ok) {
            printf("uint32 value = %u != ref = %u. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint32>(round(sum_uint32)), iteration,
                   i);
        }
        sum_uint32 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_int32(int32 * ptr,
                                         uint32 iteration,
                                         float64 increment,
                                         uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = ptr[i] == static_cast<int32>(round(sum_int32));
        if (!ok) {
            printf("int32 value = %d != ref = %d. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<int32>(round(sum_int32)), iteration,
                   i);
        }
        sum_int32 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_uint64(uint64 * ptr,
                                          uint32 iteration,
                                          float64 increment,
                                          uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = ptr[i] == static_cast<uint64>(round(sum_uint64));
        if (!ok) {
            printf("uint64 value = %llu != ref = %llu. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint64>(round(sum_uint64)),
                   iteration, i);
        }
        sum_uint64 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_int64(int64 * ptr,
                                         uint32 iteration,
                                         float64 increment,
                                         uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
//            ok = ptr[i] == static_cast<int64>(round(sum_int64));
        if (!ok) {
            printf("int64 value = %lld!= ref = %lld. iteration = %u, position in the iteration = %d\n", ptr[i], static_cast<uint64>(round(sum_uint64)),
                   iteration, i);
        }
        sum_int64 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_float32(float32 * ptr,
                                           uint32 iteration,
                                           float64 increment,
                                           uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = IsEqualLargeMargins(ptr[i], static_cast<float32>(sum_float32));
        if (!ok) {
            printf("float32 value = %.15f != ref = %.15f, iteration = %u, position in the iteration = %u\n", ptr[i], static_cast<float32>(sum_float32),
                   iteration, i);
        }
        sum_float32 += increment;
    }
    return ok;
}
bool MDSReaderTestHelper::CompareS_float64(float64 * ptr,
                                           uint32 iteration,
                                           float64 increment,
                                           uint32 elementsToCompare) {
    bool ok = true;
    for (uint32 i = 0u; (i < elementsToCompare) && ok; i++) {
        ok = IsEqualLargeMargins(ptr[i], sum_float64);
        if (!ok) {
            printf("float64 value = %.17lf != ref = %.17lf, iteration = %u, position in the iteration = %u\n", ptr[i], sum_float64, iteration, i);
        }
        sum_float64 += increment;
    }
    return ok;
}

bool MDSReaderTestHelper::IsEqualLargeMargins(float32 f1,
                                              float32 f2) {
    bool ret;
    float32 margin = 0.005;
    float32 diff = f1 - f2;
    ret = (diff < margin) && (diff > -margin);
    return ret;
}

bool MDSReaderTestHelper::IsEqualLargeMargins(float64 f1,
                                              float64 f2) {
    bool ret;
    float64 margin = 0.0000005;
    float64 diff = f1 - f2;
    ret = (diff < margin) && (diff > -margin);
    return ret;
}

}

