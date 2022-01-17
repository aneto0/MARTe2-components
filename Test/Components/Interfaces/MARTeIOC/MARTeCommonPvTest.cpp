/**
 * @file MARTeCommonPvTest.cpp
 * @brief Source file for class MARTeCommonPvTest
 * @date 27/08/2019
 * @author Giuseppe Ferro
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
 * the class MARTeCommonPvTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTeCommonPvTest.h"
#include "MARTeIocServerTest.h"
#include "MARTeCommonPv.h"
#include "GAM.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

template<>
uint32 DiscoveryGddType<int8>() {
    return aitEnumInt8;
}
template<>
uint32 DiscoveryGddType<uint8>() {
    return aitEnumUint8;
}
template<>
uint32 DiscoveryGddType<int16>() {
    return aitEnumInt16;
}
template<>
uint32 DiscoveryGddType<uint16>() {
    return aitEnumUint16;
}
template<>
uint32 DiscoveryGddType<int32>() {
    return aitEnumInt32;
}
template<>
uint32 DiscoveryGddType<uint32>() {
    return aitEnumUint32;
}
template<>
uint32 DiscoveryGddType<float32>() {
    return aitEnumFloat32;
}
template<>
uint32 DiscoveryGddType<float64>() {
    return aitEnumFloat64;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MARTeCommonPvTest::MARTeCommonPvTest() {
    // Auto-generated constructor stub for MARTeCommonPvTest
    // TODO Verify if manual additions are needed
}

MARTeCommonPvTest::~MARTeCommonPvTest() {
    // Auto-generated destructor stub for MARTeCommonPvTest
    // TODO Verify if manual additions are needed
}

bool MARTeCommonPvTest::TestInitialise() {

    const char8 *config = ""
            "            VAL = 0.5"
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        ok = (pv.maxBound(0u) == 1u);
    }
    if (ok) {
        AnyType at = pv.GetAnyType();
        ok = !at.IsVoid();
        if (ok) {
            float64 val = *(float64*) at.GetDataPointer();
            ok = IsEqual(val, 0.5);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestInitialise_InvalidHHSV() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"CICCIO\""
            "            HSV = \"MAJOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }

    if (ok) {
        StreamString hhsv;
        ok = (pv.ReadDirect(hhsv, gddAppType_hhsv) == S_cas_success);
        if (ok) {
            ok = (hhsv == "INVALID");
        }
    }
    if (ok) {
        StreamString hsv;
        ok = (pv.ReadDirect(hsv, gddAppType_hsv) == S_cas_success);
        if (ok) {
            ok = (hsv == "MAJOR");
        }
    }
    if (ok) {
        StreamString lsv;
        ok = (pv.ReadDirect(lsv, gddAppType_lsv) == S_cas_success);
        if (ok) {
            ok = (lsv == "MINOR");
        }
    }
    if (ok) {
        StreamString llsv;
        ok = (pv.ReadDirect(llsv, gddAppType_llsv) == S_cas_success);
        if (ok) {
            ok = (llsv == "MAJOR");
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestInitialise_InvalidHSV() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"CICCIO\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }

    if (ok) {
        StreamString hhsv;
        ok = (pv.ReadDirect(hhsv, gddAppType_hhsv) == S_cas_success);
        if (ok) {
            ok = (hhsv == "MAJOR");
        }
    }
    if (ok) {
        StreamString hsv;
        ok = (pv.ReadDirect(hsv, gddAppType_hsv) == S_cas_success);
        if (ok) {
            ok = (hsv == "INVALID");
        }
    }
    if (ok) {
        StreamString lsv;
        ok = (pv.ReadDirect(lsv, gddAppType_lsv) == S_cas_success);
        if (ok) {
            ok = (lsv == "MINOR");
        }
    }
    if (ok) {
        StreamString llsv;
        ok = (pv.ReadDirect(llsv, gddAppType_llsv) == S_cas_success);
        if (ok) {
            ok = (llsv == "MAJOR");
        }
    }
    return ok;

    return ok;
}

bool MARTeCommonPvTest::TestInitialise_InvalidLSV() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"CICCIo\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }

    if (ok) {
        StreamString hhsv;
        ok = (pv.ReadDirect(hhsv, gddAppType_hhsv) == S_cas_success);
        if (ok) {
            ok = (hhsv == "MAJOR");
        }
    }
    if (ok) {
        StreamString hsv;
        ok = (pv.ReadDirect(hsv, gddAppType_hsv) == S_cas_success);
        if (ok) {
            ok = (hsv == "MINOR");
        }
    }
    if (ok) {
        StreamString lsv;
        ok = (pv.ReadDirect(lsv, gddAppType_lsv) == S_cas_success);
        if (ok) {
            ok = (lsv == "INVALID");
        }
    }
    if (ok) {
        StreamString llsv;
        ok = (pv.ReadDirect(llsv, gddAppType_llsv) == S_cas_success);
        if (ok) {
            ok = (llsv == "MAJOR");
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestInitialise_InvalidLLSV() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"CICCIO\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }

    if (ok) {
        StreamString hhsv;
        ok = (pv.ReadDirect(hhsv, gddAppType_hhsv) == S_cas_success);
        if (ok) {
            ok = (hhsv == "MAJOR");
        }
    }
    if (ok) {
        StreamString hsv;
        ok = (pv.ReadDirect(hsv, gddAppType_hsv) == S_cas_success);
        if (ok) {
            ok = (hsv == "MINOR");
        }
    }
    if (ok) {
        StreamString lsv;
        ok = (pv.ReadDirect(lsv, gddAppType_lsv) == S_cas_success);
        if (ok) {
            ok = (lsv == "NO_ALARM");
        }
    }
    if (ok) {
        StreamString llsv;
        ok = (pv.ReadDirect(llsv, gddAppType_llsv) == S_cas_success);
        if (ok) {
            ok = (llsv == "INVALID");
        }
    }
    return ok;
}


bool MARTeCommonPvTest::Testread_Precision() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_precision);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            uint32 test = (uint32)(*gddRead);
            ok = (test == 4u);
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_HighLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_controlHigh);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float32 test = (float32)(*gddRead);
            ok = (IsEqual(test, 10.));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_LowLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_controlLow);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float32 test = (float32)(*gddRead);
            ok = (IsEqual(test, 1.));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_HighAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_alarmHigh);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float32 test = (float32)(*gddRead);
            ok = (IsEqual(test, 9.));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_LowAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_alarmLow);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float32 test = (float32)(*gddRead);
            ok = (IsEqual(test, 0.1));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_HighWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_alarmHighWarning);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float32 test = (float32)(*gddRead);
            ok = (test == 8.);
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_LowWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_alarmLowWarning);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float32 test = (float32)(*gddRead);
            ok = (IsEqual(test, 0.5));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Units() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_units);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            aitString test((aitString&) (*gddRead));
            StreamString t = (const char8*) test;
            ok = (t == "volt");
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Enums() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_enums);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Ackt() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_ackt);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            uint32 test = (uint32)(*gddRead);
            ok = (test == 0u);
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Acks() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_acks);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            uint32 test = (uint32)(*gddRead);
            ok = (test == 0u);
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_NumberOfElements() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 5"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_maxElements);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            uint32 test = (uint32)(*gddRead);
            ok = (test == 5u);
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Timestamp() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;
    epicsTimeStamp currentTimestamp;
    if (ok) {
        float64 val = 10.;
        currentTimestamp = epicsTime::getCurrent();
        //write something there
        ok = (pv.WriteDirect(val) == S_casApp_success);
    }

    if (ok) {
        epicsTimeStamp newTimestamp;

        smartGDDPointer gddRead = new gdd(gddAppType_timeStamp);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            gddRead->getTimeStamp(&newTimestamp);
            ok = ((currentTimestamp.secPastEpoch - newTimestamp.secPastEpoch) < 1);
            if (ok) {
                ok = (currentTimestamp.nsec != newTimestamp.nsec);
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Hyst() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_hyst);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float64 test = (float64)(*gddRead);
            ok = (IsEqual(test, 0.0002));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Mdel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_mdel);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float64 test = (float64)(*gddRead);
            ok = (IsEqual(test, 0.01));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Adel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_adel);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            float64 test = (float64)(*gddRead);
            ok = (IsEqual(test, 0.0001));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Hhsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_hhsv);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            aitString temp = (aitString&) (*gddRead);
            StreamString test = (const char8 *) (temp);
            ok = (test == "MAJOR");
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Hsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_hsv);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            aitString temp = (aitString&) (*gddRead);
            StreamString test = (const char8 *) (temp);
            ok = (test == "MINOR");
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Lsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_lsv);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            aitString temp = (aitString&) (*gddRead);
            StreamString test = (const char8 *) (temp);
            ok = (test == "NO_ALARM");
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Llsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddRead = new gdd(gddAppType_llsv);
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            aitString temp = (aitString&) (*gddRead);
            StreamString test = (const char8 *) (temp);
            ok = (test == "MINOR");
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Sevr() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 0.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"INVALID\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;
    smartGDDPointer gddRead = new gdd(gddAppType_sevr);

    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            aitString temp = (aitString&) (*gddRead);
            StreamString test = (const char8 *) (temp);
            ok = (test == "NO_ALARM");
        }
    }
    smartGDDPointer gddWrite = new gdd(gddAppType_value);
    if (ok) {
        float64 sat = 0.4;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "NO_ALARM");
            }
        }
    }
    if (ok) {
        float64 sat = 0.05;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "MINOR");
            }
        }
    }
    if (ok) {
        float64 sat = 8.5;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "INVALID");
            }
        }
    }
    if (ok) {
        float64 sat = 88.5;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "INVALID");
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_Stat() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 0.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"INVALID\""
            "            HSV = \"MAJOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0002"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    casCtx *ctx = new casCtx;
    smartGDDPointer gddRead = new gdd(gddAppType_stat);

    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_cas_success);
        if (ok) {
            aitString temp = (aitString&) (*gddRead);
            StreamString test = (const char8 *) (temp);
            ok = (test == "NO_ALARM");
        }
    }
    smartGDDPointer gddWrite = new gdd(gddAppType_value);
    if (ok) {
        float64 sat = 0.4;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "NO_ALARM");
            }
        }
    }
    if (ok) {
        float64 sat = 0.05;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "LOLO_ALARM");
            }
        }
    }
    if (ok) {
        float64 sat = 8.5;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "HIGH_ALARM");
            }
        }
    }
    if (ok) {
        float64 sat = 88.5;
        gddWrite->put(sat);
        ok = (pv.write(*ctx, *gddWrite) == S_casApp_success);
        if (ok) {
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);
                StreamString test = (const char8 *) (temp);
                ok = (test == "HIHI_ALARM");
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Ackt() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_ackt);
        gddWrite->put(aitEnumUint32);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_ackt);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                uint32 test = (uint32)(*gddRead);
                ok = (test == aitEnumUint32);
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Acks() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_acks);
        gddWrite->put(aitEnumUint16);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_acks);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                uint32 test = (uint32)(*gddRead);
                ok = (test == aitEnumUint16);
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Precision() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_precision);
        uint32 precision = 6;
        gddWrite->put(precision);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_precision);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                uint32 test = (uint32)(*gddRead);
                ok = (test == 6);
            }
        }
    }
    delete ctx;
    return ok;

}

bool MARTeCommonPvTest::Testwrite_HighLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_controlHigh);
        uint32 hopr = 20;
        gddWrite->put(hopr);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_controlHigh);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, 20.));
            }
        }
    }
    delete ctx;
    return ok;

}

bool MARTeCommonPvTest::Testwrite_LowLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_controlLow);
        int32 lopr = -50;
        gddWrite->put(lopr);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_controlLow);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, -50.));
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_HighAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_alarmHigh);
        uint32 hihi = 10;
        gddWrite->put(hihi);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_alarmHigh);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            float64 test = (float64)(*gddRead);
            ok = (IsEqual(test, 10.));
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_LowAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_alarmLow);
        float32 lolo = -1.5;
        gddWrite->put(lolo);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_alarmLow);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, -1.5));
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_HighWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_alarmHighWarning);
        float32 high = 5.25;
        gddWrite->put(high);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_alarmHighWarning);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, 5.25));
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_LowWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_alarmLowWarning);
        float32 high = -5.25;
        gddWrite->put(high);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_alarmLowWarning);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, -5.25));
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Units() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_units);
        StreamString units = "ampere";
        gddWrite->put(units.Buffer());
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_units);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);

                StreamString test = (const char8*) (temp);
                ok = (test == "ampere");
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Enums() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_enums);
        uint32 boh = 1;
        gddWrite->put(boh);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Timestamp() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        epicsTimeStamp timestamp;
        timestamp.secPastEpoch = 10;
        timestamp.nsec = 11;

        smartGDDPointer gddWrite = new gdd(gddAppType_timeStamp);
        aitTimeStamp gddts(timestamp);
        gddWrite->setTimeStamp(&gddts);

        uint32 boh = 1;
        gddWrite->put(boh);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_timeStamp);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                epicsTimeStamp newTimestamp;
                gddRead->getTimeStamp(&newTimestamp);
                ok = (newTimestamp.secPastEpoch == 10);
                if (ok) {
                    ok = (newTimestamp.nsec == 11);
                }
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Hyst() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.00005"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_hyst);
        float64 hyst = 0.0001;
        gddWrite->put(hyst);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_hyst);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, 0.0001));
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Mdel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.00005"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_mdel);
        float64 hyst = 0.02;
        gddWrite->put(hyst);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_mdel);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, 0.02));
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Adel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.00005"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_adel);
        float64 adel = 0.0003;
        gddWrite->put(adel);
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_adel);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                float64 test = (float64)(*gddRead);
                ok = (IsEqual(test, 0.0003));
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Hhsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.00005"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_hhsv);
        StreamString hhsv = "MINOR";
        gddWrite->put(hhsv.Buffer());
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_hhsv);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);

                StreamString test = (const char8*) (temp);
                ok = (test == "MINOR");
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Hsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.00005"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_hsv);
        StreamString hsv = "MINOR";
        gddWrite->put(hsv.Buffer());
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_hsv);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);

                StreamString test = (const char8*) (temp);
                ok = (test == "MINOR");
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Lsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.00005"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_lsv);
        StreamString lsv = "MINOR";
        gddWrite->put(lsv.Buffer());
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_lsv);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);

                StreamString test = (const char8*) (temp);
                ok = (test == "MINOR");
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_Llsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.00005"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_llsv);
        StreamString llsv = "NO_ALARM";
        gddWrite->put(llsv.Buffer());
        ok = (pv.write(*ctx, *gddWrite) == S_cas_success);
        if (ok) {
            smartGDDPointer gddRead = new gdd(gddAppType_llsv);
            ok = (pv.read(*ctx, *gddRead) == S_cas_success);
            if (ok) {
                aitString temp = (aitString&) (*gddRead);

                StreamString test = (const char8*) (temp);
                ok = (test == "NO_ALARM");
            }
        }
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_False1() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddWrite = new gdd(gddAppType_value);
        gddWrite->setDimension(3);

        ok = (pv.write(*ctx, *gddWrite) == S_cas_noConvert);
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_False2() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"NO_ALARM\""
            "            LLSV = \"MINOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddVal = new gddScalar(gddAppType_value, aitEnumString);
        gddVal->put("ciao");
        ok = (pv.write(*ctx, *gddVal) != S_cas_success);
    }
    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testwrite_saturated() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MAJOR\""
            "            LSV = \"MAJOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    if (ok) {
        pv.interestRegister();
    }

    casCtx *ctx = new casCtx;

    if (ok) {
        smartGDDPointer gddVal = new gddScalar(gddAppType_value, aitEnumUint8);
        uint8 x = 11;
        gddVal->put(x);
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }

    if (ok) {
        smartGDDPointer gddRead = new gddScalar(gddAppType_value);
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
        if (ok) {
            ok = ((uint8)(*gddRead) == 10);
        }
    }

    if (ok) {
        smartGDDPointer gddVal = new gddScalar(gddAppType_value, aitEnumInt8);
        int8 x = -11;
        gddVal->put(x);
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }

    if (ok) {
        smartGDDPointer gddRead = new gddScalar(gddAppType_value);
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
        if (ok) {
            ok = ((int8)(*gddRead) == -1);
        }
    }

    if (ok) {
        smartGDDPointer gddVal = new gddScalar(gddAppType_value, aitEnumInt8);
        float32 x = 8.5;
        gddVal->put(x);
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }

    if (ok) {
        smartGDDPointer gddVal = new gddScalar(gddAppType_value, aitEnumInt8);
        float32 x = 0.25;
        gddVal->put(x);
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }

    delete ctx;
    return ok;
}

bool MARTeCommonPvTest::Testread_string() {

    const char8 *config = ""
            "            TYPE = aitEnumString"
            "            NELM = 1"
            "            SYNC = excasIoSync";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    smartGDDPointer gddVal;
    casCtx *ctx = new casCtx;
    if (ok) {
        gddVal = new gddScalar(gddAppType_value, aitEnumString);
    }
    if (ok) {
        gddVal->put("ciao");
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }
    gddVal->unreference();

    smartGDDPointer gddRead = new gddScalar(gddAppType_value, aitEnumString);
    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        aitString stringR = (const aitString &) (*gddRead);
        StreamString result = (const char8*) stringR;
        printf("result=%s\n", result.Buffer());
        ok = result == "ciao";
    }

    return ok;
}

bool MARTeCommonPvTest::Testwrite_string() {
    return Testread_string();
}

bool MARTeCommonPvTest::Testread_vector_string() {
    const char8 *config = ""
            "            TYPE = aitEnumString"
            "            NELM = 5"
            "            SYNC = excasIoSync";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    smartGDDPointer gddVal;
    casCtx *ctx = new casCtx;
    const char8* test[] = { "ciao1", "ciao2", "ciao3", "ciao4", "ciao5" };
    const uint32 numberOfElements = 5u;
    if (ok) {
        aitString *ptr = new aitString[numberOfElements];
        for (uint32 i = 0u; i < numberOfElements; i++) {
            ptr[i] = test[i];
        }
        gddVal = new gddAtomic(gddAppType_value, aitEnumString, 1u, numberOfElements);
        gddVal->putRef(ptr, aitEnumString, new gddDestructor);
    }
    if (ok) {
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }
    //gddVal->unreference();

    smartGDDPointer gddRead = new gdd(gddAppType_value);
    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        aitString *stringR = (aitString *) (*gddRead);
        //gddRead->getRef(stringR);

        for (uint32 i = 0u; (i < numberOfElements) && ok; i++) {
            StreamString result = (const char8*) (stringR[i]);
            ok = (result == test[i]);
            printf("result=%s\n", result.Buffer());
        }
    }

    return ok;
}

bool MARTeCommonPvTest::Testwrite_vector_string() {
    return Testread_vector_string();
}

bool MARTeCommonPvTest::TestWriteDirect_string() {

    const char8 *config = ""
            "            TYPE = aitEnumString"
            "            NELM = 1"
            "            SYNC = excasIoSync";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    if (ok) {
        ok = (pv.WriteDirect("ciao") == S_casApp_success);
    }
    StreamString result;
    if (ok) {
        ok = (pv.ReadDirect(result) == S_casApp_success);
    }
    if (ok) {
        printf("result=%s\n", result.Buffer());
        ok = (result == "ciao");
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirect_string() {
    return TestWriteDirect_string();
}

bool MARTeCommonPvTest::TestWriteDirect_vector_string() {
    const uint32 numberOfElements = 5u;
    const char8 *config = ""
            "            TYPE = aitEnumString"
            "            NELM = 5"
            "            SYNC = excasIoSync";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    Vector < StreamString > test(numberOfElements);
    if (ok) {
        test[0] = "ciao0";
        test[1] = "ciao1";
        test[2] = "ciao2";
        test[3] = "ciao3";
        test[4] = "ciao4";
        ok = (pv.WriteDirect(test) == S_casApp_success);
    }
    if (ok) {
        Vector < StreamString > result(numberOfElements);
        ok = (pv.ReadDirect(result) == S_casApp_success);
        for (uint32 i = 0u; (i < numberOfElements) && ok; i++) {
            ok = (test[i] == result[i]);
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestReadDirect_vector_string() {
    return TestWriteDirect_vector_string();
}

bool MARTeCommonPvTest::TestWriteDirect_saturated() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    if (ok) {
        float32 in = 11;
        ok = (pv.WriteDirect(in) == S_casApp_success);
    }
    if (ok) {
        float32 out;
        ok = (pv.ReadDirect(out) == S_casApp_success);
        if (ok) {
            ok = (out == 10.0);
        }
    }

    if (ok) {
        float32 in = -11;
        ok = (pv.WriteDirect(in) == S_casApp_success);
    }
    if (ok) {
        float32 out;
        ok = (pv.ReadDirect(out) == S_casApp_success);
        if (ok) {
            ok = (out == -10.0);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirect_saturated() {
    return TestWriteDirect_saturated();
}

bool MARTeCommonPvTest::TestWriteDirect_False() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    if (ok) {
        StreamString test = "ciao";
        ok = (pv.WriteDirect(test) != S_casApp_success);
    }
    return ok;
}

bool MARTeCommonPvTest::TestWriteDirect_vector_False() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 5"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    if (ok) {
        Vector < StreamString > test(5);
        test[0] = "ciao0";
        test[1] = "ciao1";
        test[2] = "ciao2";
        test[3] = "ciao3";
        test[4] = "ciao4";

        ok = (pv.WriteDirect(test) != S_casApp_success);
    }
    return ok;
}

bool MARTeCommonPvTest::TestReadDirect_False() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer iocServ;
    if (ok) {
        pv.SetIocServer(iocServ);
    }

    if (ok) {
        float32 test = 5.0;
        ok = (pv.WriteDirect(test) == S_casApp_success);
    }
    if (ok) {
        Vector < float32 > result(2);
        ok = (pv.ReadDirect(result) != S_casApp_success);
    }
    return ok;
}

bool MARTeCommonPvTest::Testscan() {

    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = 1.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01"
            "            SCAN = 1.0";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    MARTeIocServer server;
    if (ok) {
        pv.SetIocServer(server);
    }
    uint32 nanos1;
    uint32 secs1;
    uint32 nanos2;
    uint32 secs2;
    if (ok) {
        pv.scan();
        pv.GetTimeStamp(secs1, nanos1);
        Sleep::Sec(0.5);
        pv.GetTimeStamp(secs2, nanos2);
        ok = (nanos1 == nanos2);
    }
    if (ok) {
        Sleep::Sec(0.5);
        pv.scan();
        pv.GetTimeStamp(secs1, nanos1);
        ok = (nanos1 != nanos2);

    }
    return ok;
}

bool MARTeCommonPvTest::TestReadDirectPrecision() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        uint32 precision;
        ok = (pv.ReadDirect(precision, gddAppType_precision) == S_casApp_success);
        if (ok) {
            ok = (precision == 4u);
        }
    }

    return ok;

}

bool MARTeCommonPvTest::TestReadDirectHighLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 hopr;
        ok = (pv.ReadDirect(hopr, gddAppType_controlHigh) == S_casApp_success);
        if (ok) {
            ok = IsEqual(hopr, 10.);
        }
    }

    return ok;

}

bool MARTeCommonPvTest::TestReadDirectLowLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 lopr;
        ok = (pv.ReadDirect(lopr, gddAppType_controlLow) == S_casApp_success);
        if (ok) {
            ok = IsEqual(lopr, -10.);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectHighAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 hihi;
        ok = (pv.ReadDirect(hihi, gddAppType_alarmHigh) == S_casApp_success);
        if (ok) {
            ok = IsEqual(hihi, 9.);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectLowAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 test;
        ok = (pv.ReadDirect(test, gddAppType_alarmLow) == S_casApp_success);
        if (ok) {
            ok = IsEqual(test, 0.1);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectHighWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 test;
        ok = (pv.ReadDirect(test, gddAppType_alarmHighWarning) == S_casApp_success);
        if (ok) {
            ok = IsEqual(test, 8.);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectLowWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 test;
        ok = (pv.ReadDirect(test, gddAppType_alarmLowWarning) == S_casApp_success);
        if (ok) {
            ok = IsEqual(test, 0.5);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectUnits() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_units) == S_casApp_success);
        if (ok) {
            ok = (test == "volt");
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectEnums() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_enums) == S_casApp_success);
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectAckt() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        int32 test;
        ok = (pv.ReadDirect(test, gddAppType_ackt) == S_casApp_success);
        if (ok) {
            ok = (test == (int32) aitEnumInvalid);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectAcks() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        int32 test;
        ok = (pv.ReadDirect(test, gddAppType_acks) == S_casApp_success);
        if (ok) {
            ok = (test == (int32) aitEnumInvalid);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectElementCount() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 5"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        uint32 test;
        ok = (pv.ReadDirect(test, gddAppType_maxElements) == S_casApp_success);
        if (ok) {
            ok = (test == 5u);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectTimeStamp() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        epicsTimeStamp currentTimestamp = epicsTime::getCurrent();

        uint32 temp = 1u;
        ok = (pv.WriteDirect(temp) == S_casApp_success);
        if (ok) {
            uint64 test;
            ok = (pv.ReadDirect(test, gddAppType_timeStamp) == S_casApp_success);
            epicsTimeStamp *newTimeStamp = (epicsTimeStamp *) (&test);
            ok = ((currentTimestamp.secPastEpoch - newTimeStamp->secPastEpoch) < 1);
            if (ok) {
                ok = (currentTimestamp.nsec != newTimeStamp->nsec);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectHyst() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 test;
        ok = (pv.ReadDirect(test, gddAppType_hyst) == S_casApp_success);
        if (ok) {
            ok = IsEqual(test, 0.0001);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectMdel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 test;
        ok = (pv.ReadDirect(test, gddAppType_mdel) == S_casApp_success);
        if (ok) {
            ok = IsEqual(test, 0.01);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectAdel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0002"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 test;
        ok = (pv.ReadDirect(test, gddAppType_adel) == S_casApp_success);
        if (ok) {
            ok = IsEqual(test, 0.0002);
        }
    }

    return ok;

}

bool MARTeCommonPvTest::TestReadDirectHhsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0002"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_hhsv) == S_casApp_success);
        if (ok) {
            ok = (test == "MAJOR");
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestReadDirectHsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0002"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_hsv) == S_casApp_success);
        if (ok) {
            ok = (test == "MINOR");
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestReadDirectLsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0002"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_lsv) == S_casApp_success);
        if (ok) {
            ok = (test == "MINOR");
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestReadDirectLlsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0002"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_llsv) == S_casApp_success);
        if (ok) {
            ok = (test == "MAJOR");
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestReadDirectStat() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0002"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_stat) == S_casApp_success);
        if (ok) {
            ok = (test == "NO_ALARM");
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestReadDirectSevr() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0002"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString test;
        ok = (pv.ReadDirect(test, gddAppType_sevr) == S_casApp_success);
        if (ok) {
            ok = (test == "NO_ALARM");
        }
    }
    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectPrecision() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        uint32 precision = 6u;
        ok = (pv.WriteDirect(precision, gddAppType_precision) == S_casApp_success);
        if (ok) {
            uint32 test;
            ok = (pv.ReadDirect(test, gddAppType_precision) == S_casApp_success);
            if (ok) {
                ok = (test == 6u);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectHighLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = 20.;
        ok = (pv.WriteDirect(input, gddAppType_controlHigh) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_controlHigh) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, 20.);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectLowLimit() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = -20.;
        ok = (pv.WriteDirect(input, gddAppType_controlLow) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_controlLow) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, -20.);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectHighAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = 20.;
        ok = (pv.WriteDirect(input, gddAppType_alarmHigh) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_alarmHigh) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, 20.);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectLowAlarm() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = 10.;
        ok = (pv.WriteDirect(input, gddAppType_alarmLow) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_alarmLow) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, 10.);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectHighWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = -10.;
        ok = (pv.WriteDirect(input, gddAppType_alarmHighWarning) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_alarmHighWarning) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, -10.);
            }
        }
    }

    return ok;

}

bool MARTeCommonPvTest::TestWriteDirectLowWarning() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = -10.;
        ok = (pv.WriteDirect(input, gddAppType_alarmLowWarning) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_alarmLowWarning) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, -10.);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectUnits() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString input = "ampere";
        ok = (pv.WriteDirect(input, gddAppType_units) == S_casApp_success);
        if (ok) {
            StreamString test;
            ok = (pv.ReadDirect(test, gddAppType_units) == S_casApp_success);
            if (ok) {
                ok = (test == "ampere");
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectEnums() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        uint32 in;
        ok = (pv.WriteDirect(in, gddAppType_enums) == S_casApp_success);
        if (ok) {
            uint32 test;
            ok = (pv.ReadDirect(test, gddAppType_enums) == S_casApp_success);
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectAckt() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        int32 input = 1;
        ok = (pv.WriteDirect(input, gddAppType_ackt) == S_casApp_success);
        if (ok) {
            int32 test;
            ok = (pv.ReadDirect(test, gddAppType_ackt) == S_casApp_success);
            if (ok) {
                ok = (test == 1);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectAcks() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        int32 input = 1;
        ok = (pv.WriteDirect(input, gddAppType_ackt) == S_casApp_success);
        if (ok) {
            int32 test;
            ok = (pv.ReadDirect(test, gddAppType_ackt) == S_casApp_success);
            if (ok) {
                ok = (test == 1);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectTimeStamp() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        epicsTimeStamp currentTimeStamp;
        currentTimeStamp.secPastEpoch = 10;
        currentTimeStamp.nsec = 11;

        ok = (pv.WriteDirect(*(uint64*) (&currentTimeStamp), gddAppType_timeStamp) == S_casApp_success);
        if (ok) {
            uint64 test;
            ok = (pv.ReadDirect(test, gddAppType_timeStamp) == S_casApp_success);

            epicsTimeStamp *newTimeStamp = (epicsTimeStamp *) (&test);
            ok = (newTimeStamp->secPastEpoch == 10);
            if (ok) {
                ok = (newTimeStamp->nsec == 11);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectHyst() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = 0.002;
        ok = (pv.WriteDirect(input, gddAppType_hyst) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_hyst) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, 0.002);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectMdel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = 0.1;
        ok = (pv.WriteDirect(input, gddAppType_mdel) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_mdel) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, 0.1);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectAdel() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        float64 input = 0.3;
        ok = (pv.WriteDirect(input, gddAppType_adel) == S_casApp_success);
        if (ok) {
            float64 test;
            ok = (pv.ReadDirect(test, gddAppType_adel) == S_casApp_success);
            if (ok) {
                ok = IsEqual(test, 0.3);
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectHhsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString input = "NO_ALARM";
        ok = (pv.WriteDirect(input, gddAppType_hhsv) == S_casApp_success);
        if (ok) {
            StreamString test;
            ok = (pv.ReadDirect(test, gddAppType_hhsv) == S_casApp_success);
            if (ok) {
                ok = (test == "NO_ALARM");
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectHsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString input = "INVALID";
        ok = (pv.WriteDirect(input, gddAppType_hsv) == S_casApp_success);
        if (ok) {
            StreamString test;
            ok = (pv.ReadDirect(test, gddAppType_hsv) == S_casApp_success);
            if (ok) {
                ok = (test == "INVALID");
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectLsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString input = "MAJOR";
        ok = (pv.WriteDirect(input, gddAppType_lsv) == S_casApp_success);
        if (ok) {
            StreamString test;
            ok = (pv.ReadDirect(test, gddAppType_lsv) == S_casApp_success);
            if (ok) {
                ok = (test == "MAJOR");
            }
        }
    }

    return ok;
}

bool MARTeCommonPvTest::TestWriteDirectLlsv() {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            NELM = 1"
            "            PREC = 4"
            "            SYNC = excasIoSync"
            "            EGU = \"volt\""
            "            HOPR = 10.0"
            "            LOPR = -10.0"
            "            HIHI = 9.0"
            "            HIGH = 8.0"
            "            LOW = 0.5"
            "            LOLO = 0.1"
            "            HHSV =  \"MAJOR\""
            "            HSV = \"MINOR\""
            "            LSV = \"MINOR\""
            "            LLSV = \"MAJOR\""
            "            HYST = 0.0001"
            "            ADEL = 0.0001"
            "            MDEL = 0.01";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        StreamString input = "MINOR";
        ok = (pv.WriteDirect(input, gddAppType_llsv) == S_casApp_success);
        if (ok) {
            StreamString test;
            ok = (pv.ReadDirect(test, gddAppType_llsv) == S_casApp_success);
            if (ok) {
                ok = (test == "MINOR");
            }
        }
    }

    return ok;
}

