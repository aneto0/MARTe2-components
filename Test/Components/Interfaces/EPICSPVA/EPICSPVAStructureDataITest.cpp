/**
 * @file EPICSPVAStructureDataITest.cpp
 * @brief Source file for class EPICSPVAStructureDataITest
 * @date 13/06/2018
 * @author Andre Neto
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
 * the class EPICSPVAStructureDataITest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <pv/rpcClient.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include <EPICSPVAStructureDataI.h>
#include "File.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"
#include "EPICSPVAStructureDataITest.h"
#include "EPICSRPCClient.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool EPICSPVAStructureDataITest::TestConstructor() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    return test.IsStructureFinalised();
}

bool EPICSPVAStructureDataITest::TestRead_UInt8() {
    using namespace MARTe;
    uint8 wvalue = 16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt16() {
    using namespace MARTe;
    uint16 wvalue = 16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt32() {
    using namespace MARTe;
    uint32 wvalue = 16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt64() {
    using namespace MARTe;
    uint64 wvalue = 16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int8() {
    using namespace MARTe;
    int8 wvalue = -16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int16() {
    using namespace MARTe;
    int16 wvalue = -16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int32() {
    using namespace MARTe;
    int32 wvalue = -16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int64() {
    using namespace MARTe;
    int64 wvalue = -16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Float32() {
    using namespace MARTe;
    float32 wvalue = -16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Float64() {
    using namespace MARTe;
    float64 wvalue = -16;
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_StreamString() {
    using namespace MARTe;
    StreamString wvalue = "EPICSPVAStructureDataITest";
    return TestRead(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt8_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint8> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint16> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint64> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int8_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int8> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int16> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int64> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Float32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<float32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Float64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<float64> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_StreamString_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<StreamString> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i].Printf("%d", i + 1);
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint16 wvalue = 16;
    test.Write("Test", wvalue);
    uint16 rvalue = 0;
    bool ok = !test.Read("Test", rvalue);

    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_False_Char8String() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    StreamString wvalue = "AAA";
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    char8 rvalue[3u];
    bool ok = !test.Read("Test", rvalue);
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_False_InvalidType() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint16 wvalue = 33;
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    uint7 rvalue;
    bool ok = !test.Read("Test", rvalue);
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_False_Array_Dimension() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 vsize = 8u;
    Vector<uint8> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    MARTe::Vector<uint8> rvalue(2);
    bool ok = !test.Read("Test", rvalue);
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_False_Array_Char8String() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 vsize = 8u;
    Vector<StreamString> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i].Printf("%d", i + 1);
    }

    test.Write("Test", wvalue);
    test.FinaliseStructure();
    MARTe::Vector<char8 *> rvalue(8);
    bool ok = !test.Read("Test", rvalue);
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_False_Array_InvalidType() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 vsize = 8u;
    Vector<uint8> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    MARTe::Vector<uint7> rvalue(vsize);
    bool ok = !test.Read("Test", rvalue);
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_DifferentTypes() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    float32 wvalue = 32.3;
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    uint32 rvalue;
    bool ok = test.Read("Test", rvalue);
    if (ok) {
        ok = (rvalue == static_cast<uint32>(wvalue));
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_Numeric_ToString() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 wvalue = 32;
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    StreamString rvalue;
    bool ok = test.Read("Test", rvalue);
    if (ok) {
        ok = (rvalue == "32");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_String_ToNumeric() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    StreamString wvalue = "32";
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    uint32 rvalue;
    bool ok = test.Read("Test", rvalue);
    if (ok) {
        ok = (rvalue == 32);
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_DifferentTypes_Array() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 vsize = 8u;
    Vector<float32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = static_cast<float32>(i) + 0.4;
    }
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    MARTe::Vector<uint32> rvalue(vsize);
    bool ok = test.Read("Test", rvalue);
    if (ok) {
        for (i = 0u; (i < vsize) && (ok); i++) {
            ok = (rvalue[i] == static_cast<uint32>(wvalue[i]));
        }
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_Numeric_ToString_Array() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 vsize = 8u;
    Vector<uint32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = (i + 1);
    }
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    MARTe::Vector<StreamString> rvalue(vsize);
    bool ok = test.Read("Test", rvalue);
    if (ok) {
        for (i = 0u; (i < vsize) && (ok); i++) {
            StreamString toTest;
            toTest.Printf("%d", wvalue[i]);
            ok = (rvalue[i] == toTest);
        }
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestRead_String_ToNumeric_Array() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 vsize = 8u;
    Vector<StreamString> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i].Printf("%d", i + 1);
    }
    test.Write("Test", wvalue);
    test.FinaliseStructure();
    MARTe::Vector<uint32> rvalue(vsize);
    bool ok = test.Read("Test", rvalue);
    if (ok) {
        for (i = 0u; (i < vsize) && (ok); i++) {
            ok = (rvalue[i] == (i + 1));
        }
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetType_UInt8() {
    using namespace MARTe;
    uint8 wvalue = 16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_UInt16() {
    using namespace MARTe;
    uint16 wvalue = 16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_UInt32() {
    using namespace MARTe;
    uint32 wvalue = 16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_UInt64() {
    using namespace MARTe;
    uint64 wvalue = 16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int8() {
    using namespace MARTe;
    int8 wvalue = -16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int16() {
    using namespace MARTe;
    int16 wvalue = -16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int32() {
    using namespace MARTe;
    int32 wvalue = -16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int64() {
    using namespace MARTe;
    int64 wvalue = -16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Float32() {
    using namespace MARTe;
    float32 wvalue = -16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Float64() {
    using namespace MARTe;
    float64 wvalue = -16;
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_StreamString() {
    using namespace MARTe;
    StreamString wvalue = "EPICSPVAStructureDataITest";
    return TestGetType(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_UInt8_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint8> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_UInt16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint16> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_UInt32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_UInt64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint64> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int8_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int8> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int16> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Int64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int64> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Float32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<float32> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_Float64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<float64> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_StreamString_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<StreamString> wvalue(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue[i].Printf("%d", i + 1);
    }

    return TestGetTypeArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestGetType_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint16 wvalue = 16;
    test.Write("Test", wvalue);
    bool ok = (test.GetType("Test").GetTypeDescriptor() == voidAnyType.GetTypeDescriptor());

    return ok;
}

bool EPICSPVAStructureDataITest::TestGetType_False_Parameter_Does_Not_Exist() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.FinaliseStructure();
    bool ok = (test.GetType("Test").GetTypeDescriptor() == voidAnyType.GetTypeDescriptor());
    return ok;
}

bool EPICSPVAStructureDataITest::TestWrite_UInt8() {
    using namespace MARTe;
    uint8 wvalue1 = 16;
    uint8 wvalue2 = 9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_UInt16() {
    using namespace MARTe;
    uint16 wvalue1 = 16;
    uint16 wvalue2 = 9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_UInt32() {
    using namespace MARTe;
    uint32 wvalue1 = 16;
    uint32 wvalue2 = 9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_UInt64() {
    using namespace MARTe;
    uint64 wvalue1 = 16;
    uint64 wvalue2 = 9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int8() {
    using namespace MARTe;
    int8 wvalue1 = -16;
    int8 wvalue2 = -9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int16() {
    using namespace MARTe;
    int16 wvalue1 = -16;
    int16 wvalue2 = -9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int32() {
    using namespace MARTe;
    int32 wvalue1 = -16;
    int32 wvalue2 = -9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int64() {
    using namespace MARTe;
    int64 wvalue1 = -16;
    int64 wvalue2 = -9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Float32() {
    using namespace MARTe;
    uint8 wvalue1 = -16;
    uint8 wvalue2 = -9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Float64() {
    using namespace MARTe;
    uint8 wvalue1 = -16;
    uint8 wvalue2 = -9;
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_StreamString() {
    using namespace MARTe;
    StreamString wvalue1 = "EPICSPVAStructureDataITest";
    StreamString wvalue2 = "TestWrite_StreamString";
    return TestWrite(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Char8String() {
    using namespace MARTe;
    StreamString wvalue1 = "EPICSPVAStructureDataITest";
    const char8 *wvalue2 = "TestWrite_StreamString";
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue1);
    test.FinaliseStructure();
    test.Write("Test", wvalue2);
    StreamString rvalue;
    test.Read("Test", rvalue);

    return (rvalue == wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_UInt8_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint8> wvalue1(vsize);
    Vector<uint8> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_UInt16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint16> wvalue1(vsize);
    Vector<uint16> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_UInt32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint32> wvalue1(vsize);
    Vector<uint32> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_UInt64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint64> wvalue1(vsize);
    Vector<uint64> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int8_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int8> wvalue1(vsize);
    Vector<int8> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int16> wvalue1(vsize);
    Vector<int16> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int32> wvalue1(vsize);
    Vector<int32> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Int64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int64> wvalue1(vsize);
    Vector<int64> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Float32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<float32> wvalue1(vsize);
    Vector<float32> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Float64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint8> wvalue1(vsize);
    Vector<uint8> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i] = i + 1;
        wvalue2[i] = (i + 1) * 2;
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_StreamString_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<StreamString> wvalue1(vsize);
    Vector<StreamString> wvalue2(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i].Printf("%d", i + 1);
        wvalue2[i].Printf("%d", (i + 1) * 2);
    }

    return TestWriteArray(wvalue1, wvalue2);
}

bool EPICSPVAStructureDataITest::TestWrite_Char8String_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<StreamString> wvalue1(vsize);
    Vector<const char8 *> wvalue2(vsize);
    Vector<StreamString> wvaluet(vsize);
    uint32 i;
    for (i = 0u; i < vsize; i++) {
        wvalue1[i].Printf("%d", i + 1);
        wvaluet[i].Printf("%d", (i + 1) * 2);
        wvalue2[i] = wvaluet[i].Buffer();
    }
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue1);
    test.FinaliseStructure();
    test.Write("Test", wvalue2);
    MARTe::Vector<StreamString> rvalue(wvalue1.GetNumberOfElements());
    test.Read("Test", rvalue);
    bool ok = true;
    for (i = 0; (i < wvalue2.GetNumberOfElements()) && (ok); i++) {
        ok = (rvalue[i] == wvalue2[i]);
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestWrite_False_Scalar_Vector() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    Vector<uint32> wvalue1(5u);
    test.InitStructure();
    test.Write("Test", wvalue1);
    test.FinaliseStructure();
    uint32 wvalue2 = 0;
    bool ok = !test.Write("Test", wvalue2);
    return ok;
}

bool EPICSPVAStructureDataITest::TestWrite_False_Vector_Scalar() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    uint32 wvalue1 = 0;
    test.Write("Test", wvalue1);
    test.FinaliseStructure();
    Vector<uint32> wvalue2(5u);
    bool ok = !test.Write("Test", wvalue2);
    return ok;
}

bool EPICSPVAStructureDataITest::TestWrite_False_InvalidType() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    uint7 wvalue1 = 9;
    test.InitStructure();
    test.Write("Test", wvalue1);
    bool ok = !test.FinaliseStructure();
    return ok;
}

bool EPICSPVAStructureDataITest::TestWrite_False_Array_InvalidType() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint7> wvalue1(vsize);
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue1);
    bool ok = !test.FinaliseStructure();
    return ok;
}

bool EPICSPVAStructureDataITest::TestWrite_False_Array_Dimension() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint8> wvalue1(vsize);
    Vector<uint8> wvalue2(2 * vsize);
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.Write("Test", wvalue1);
    test.FinaliseStructure();
    bool ok = !test.Write("Test", wvalue2);
    return ok;
}

bool EPICSPVAStructureDataITest::TestCopy() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("A.D");
    test.CreateAbsolute("E.F");
    test.MoveAbsolute("A.B.C");
    test.Write("AString", "AAA");
    test.Write("BString", "BBB");
    Vector<StreamString> wvalue1(3);
    wvalue1[0].Printf("A0", voidAnyType);
    wvalue1[1].Printf("A1", voidAnyType);
    wvalue1[2].Printf("A2", voidAnyType);

    test.Write("CString", wvalue1);
    test.MoveAbsolute("A.D");
    test.Write("AUInt32", 30);
    test.MoveAbsolute("E.F");
    test.Write("BUInt32", 34);
    Vector<uint32> wvalue2(3);
    wvalue2[0] = 1;
    wvalue2[1] = 2;
    wvalue2[2] = 3;
    test.Write("CUInt32", wvalue2);

    test.FinaliseStructure();
    ConfigurationDatabase cdb;
    bool ok = test.Copy(cdb);
    if (ok) {
        ok = cdb.MoveAbsolute("A.B.C");
    }
    if (ok) {
        StreamString str1;
        ok = cdb.Read("AString", str1);
        if (ok) {
            ok = (str1 == "AAA");
        }
    }
    if (ok) {
        StreamString str1;
        ok = cdb.Read("BString", str1);
        if (ok) {
            ok = (str1 == "BBB");
        }
    }
    if (ok) {
        Vector<StreamString> str1(3);
        ok = cdb.Read("CString", str1);
        if (ok) {
            ok = (str1[0] == "A0");
        }
        if (ok) {
            ok = (str1[1] == "A1");
        }
        if (ok) {
            ok = (str1[2] == "A2");
        }
    }
    if (ok) {
        ok = cdb.MoveAbsolute("A.D");
    }
    if (ok) {
        uint32 val;
        ok = cdb.Read("AUInt32", val);
        if (ok) {
            ok = (val == 30);
        }
    }
    if (ok) {
        ok = cdb.MoveAbsolute("E.F");
    }
    if (ok) {
        uint32 val;
        ok = cdb.Read("BUInt32", val);
        if (ok) {
            ok = (val == 34);
        }
    }
    if (ok) {
        Vector<uint32> val(3);
        ok = cdb.Read("CUInt32", val);
        if (ok) {
            ok = (val[0] == 1);
        }
        if (ok) {
            ok = (val[1] == 2);
        }
        if (ok) {
            ok = (val[2] == 3);
        }
    }
    return ok;

}

bool EPICSPVAStructureDataITest::TestCopy_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    bool ok = !test.Copy(cdb);
    return ok;
}

bool EPICSPVAStructureDataITest::TestAddToCurrentNode() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.FinaliseStructure();
    Reference r;
    bool ok = !test.AddToCurrentNode(r);
    return ok;
}

bool EPICSPVAStructureDataITest::TestMoveToRoot() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("B.C");
    test.CreateAbsolute("C");
    test.CreateAbsolute("C.D");
    test.CreateAbsolute("C.D.E");
    bool ok = test.MoveToRoot();
    test.CreateRelative("E.F.G");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("E.F.G");
    }
    if (ok) {
        ok = test.MoveToRoot();
    }
    if (ok) {
        ok = test.MoveRelative("E.F");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestMoveToAncestor() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("B.C");
    test.CreateAbsolute("C");
    test.CreateAbsolute("C.D");
    test.CreateAbsolute("C.D.E");
    bool ok = test.MoveToAncestor(2u);
    test.CreateRelative("E.F.G");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("C.E.F.G");
    }
    if (ok) {
        ok = test.MoveToAncestor(3u);
    }
    if (ok) {
        ok = test.MoveRelative("D.E");
    }
    if (ok) {
        ok = !test.MoveToAncestor(5u);
    }
    if (ok) {
        ok = test.MoveToAncestor(3u);
    }
    if (ok) {
        ok = test.MoveRelative("A.B.C");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestMoveAbsolute() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("B.C");
    test.CreateAbsolute("C");
    test.CreateAbsolute("C.D");
    test.CreateAbsolute("C.D.E");
    bool ok = test.MoveAbsolute("A.B.C");
    test.CreateRelative("E.F.G");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.C.E.F.G");
    }
    if (ok) {
        ok = test.MoveToRoot();
    }
    if (ok) {
        ok = test.MoveAbsolute("B.C");
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.C");
    }
    if (ok) {
        ok = !test.MoveAbsolute("A.B.D.E.F.G");
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.C.E.F.G");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestMoveRelative() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("A.B.E");
    bool ok = test.MoveAbsolute("A.B.C");
    test.CreateRelative("E.F.G");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B");
    }
    if (ok) {
        ok = test.MoveRelative("E");
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.C");
    }
    if (ok) {
        ok = test.MoveRelative("E.F.G");
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.C");
    }
    if (ok) {
        ok = !test.MoveRelative("F.G");
    }
    if (ok) {
        ok = test.MoveRelative("E.F.G");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestMoveToChild() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("A.B.E");
    test.CreateAbsolute("A.B.D");
    test.CreateAbsolute("F.G.C");
    test.CreateAbsolute("F.G.E");
    test.CreateAbsolute("F.G.D");
    bool ok = test.MoveToRoot();
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveToChild(1u);
    }
    if (ok) {
        ok = test.MoveRelative("G.D");
    }
    if (ok) {
        ok = test.MoveToRoot();
    }
    if (ok) {
        ok = test.MoveToChild(0u);
    }
    if (ok) {
        ok = test.MoveRelative("B.E");
    }
    if (ok) {
        ok = test.MoveToRoot();
    }
    if (ok) {
        ok = test.MoveToChild(5u);
    }
    if (ok) {
        ok = test.MoveToChild(1u);
    }
    if (ok) {
        ok = test.MoveRelative("G.D");
    }
    if (ok) {
        ok = test.MoveToRoot();
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestCreateAbsolute() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("F.G.E");
    bool ok = test.MoveToRoot();
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("F.G.E");
    }
    if (ok) {
        ok = !test.MoveAbsolute("C.D");
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.C");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestCreateAbsolute_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    bool ok = test.MoveToRoot();
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = !test.CreateAbsolute("F.G.E");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestCreateRelative() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A");
    test.CreateRelative("B.E");
    test.CreateAbsolute("C");
    test.CreateRelative("F.G");
    bool ok = test.MoveToRoot();
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.E");
    }
    if (ok) {
        ok = test.MoveToRoot();
    }
    if (ok) {
        ok = test.MoveRelative("C.F.G");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestCreateRelative_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A");
    bool ok = test.MoveToRoot();
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("A");
    }
    if (ok) {
        ok = !test.CreateRelative("F.G.E");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestDelete() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("F.G.E");
    test.MoveToRoot();
    bool ok = test.Delete("A");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("F.G.E");
    }
    if (ok) {
        ok = !test.MoveAbsolute("A");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestDelete_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C");
    test.CreateAbsolute("F.G.E");
    bool ok = test.FinaliseStructure();
    if (ok) {
        ok = !test.Delete("A");
    }
    if (ok) {
        ok = test.MoveAbsolute("F.G.E");
    }
    if (ok) {
        ok = test.MoveAbsolute("A");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetName() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");

    bool ok = test.FinaliseStructure();
    if (ok) {
        ok = test.MoveAbsolute("A.B.C");
        StreamString tname = test.GetName();
        ok = (tname == "C");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetName_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.MoveAbsolute("A.B.C");
    StreamString tname = test.GetName();
    bool ok = (tname == "");
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetChildName() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.CreateAbsolute("A.B.C.E");
    test.CreateAbsolute("A.B.C.F");

    bool ok = test.FinaliseStructure();
    if (ok) {
        ok = test.MoveAbsolute("A.B.C");
        StreamString tname = test.GetChildName(1u);
        ok = (tname == "E");
    }
    if (ok) {
        StreamString tname = test.GetChildName(5u);
        ok = (tname == "");
    }
    if (ok) {
        StreamString tname = test.GetChildName(2u);
        ok = (tname == "F");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetChildName_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.CreateAbsolute("A.B.C.E");
    test.CreateAbsolute("A.B.C.F");
    test.MoveAbsolute("A.B.C");
    StreamString tname = test.GetChildName(1u);
    bool ok = (tname == "");
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetNumberOfChildren() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.CreateAbsolute("A.B.C.E");
    test.CreateAbsolute("A.B.C.F");
    test.FinaliseStructure();
    test.MoveAbsolute("A.B.C");
    bool ok = (test.GetNumberOfChildren() == 3u);
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetNumberOfChildren_False_FinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.CreateAbsolute("A.B.C.E");
    test.CreateAbsolute("A.B.C.F");
    test.MoveAbsolute("A.B.C");
    bool ok = (test.GetNumberOfChildren() == 0u);
    return ok;
}

bool EPICSPVAStructureDataITest::TestSetStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.CreateAbsolute("A.B.C.E");
    test.CreateAbsolute("A.B.C.F");
    test.FinaliseStructure();
    epics::pvData::PVStructurePtr pvStruct = test.GetRootStruct();
    epics::pvData::PVStructurePtr structPtr = epics::pvData::getPVDataCreate()->createPVStructure(pvStruct);
    EPICSPVAStructureDataI test2;
    test2.InitStructure();
    test2.SetStructure(structPtr);
    bool ok = test2.IsStructureFinalised();
    if (ok) {
        test2.MoveAbsolute("A.B.C.E");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestInitStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    bool ok = !test.IsStructureFinalised();
    test.CreateAbsolute("A");
    test.CreateRelative("B.E");
    test.CreateAbsolute("C");
    test.CreateRelative("F.G");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.E");
    }
    if (ok) {
        ok = test.MoveAbsolute("C.F.G");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestFinaliseStructure() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    bool ok = !test.IsStructureFinalised();
    test.CreateAbsolute("A");
    test.CreateRelative("B.E");
    test.CreateAbsolute("C");
    test.CreateRelative("F.G");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.IsStructureFinalised();
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.E");
    }
    if (ok) {
        ok = test.MoveAbsolute("C.F.G");
    }
    return ok;
}

bool EPICSPVAStructureDataITest::TestGetRootStruct() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    test.CreateAbsolute("A.B.C.D");
    test.CreateAbsolute("A.B.C.E");
    test.CreateAbsolute("A.B.C.F");
    test.FinaliseStructure();
    epics::pvData::PVStructurePtr pvStruct = test.GetRootStruct();
    bool ok = (pvStruct ? true : false);
    return ok;
}

bool EPICSPVAStructureDataITest::TestIsStructureFinalised() {
    using namespace MARTe;
    EPICSPVAStructureDataI test;
    ConfigurationDatabase cdb;
    test.InitStructure();
    bool ok = !test.IsStructureFinalised();
    test.CreateAbsolute("A");
    test.CreateRelative("B.E");
    test.CreateAbsolute("C");
    test.CreateRelative("F.G");
    if (ok) {
        ok = test.FinaliseStructure();
    }
    if (ok) {
        ok = test.IsStructureFinalised();
    }
    if (ok) {
        ok = test.MoveAbsolute("A.B.E");
    }
    if (ok) {
        ok = test.MoveAbsolute("C.F.G");
    }
    return ok;
}
