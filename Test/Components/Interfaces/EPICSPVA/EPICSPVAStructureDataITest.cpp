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
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint16> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint32> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_UInt64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<uint64> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int8_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int8> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int16_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int16> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int32> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Int64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<int64> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Float32_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<float32> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_Float64_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<float64> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i] = i + 1;
    }

    return TestReadArray(wvalue);
}

bool EPICSPVAStructureDataITest::TestRead_StreamString_Array() {
    using namespace MARTe;
    uint32 vsize = 8u;
    Vector<StreamString> wvalue(vsize);
    uint32 i;
    for (i=0u; i<vsize; i++) {
        wvalue[i].Printf("%d", i + 1);
    }

    return TestReadArray(wvalue);
}
