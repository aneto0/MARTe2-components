/**
 * @file MARTePvTest.cpp
 * @brief Source file for class MARTePvTest
 * @date 26/08/2019
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
 * the class MARTePvTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTePvTest.h"
#include "MARTeIocServerTest.h"
#include "MARTeCommonPv.h"
#include "GAM.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "casCtx.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

template<>
uint32 DiscoveryType<int8>() {
    return aitEnumInt8;
}
template<>
uint32 DiscoveryType<uint8>() {
    return aitEnumUint8;
}
template<>
uint32 DiscoveryType<int16>() {
    return aitEnumInt16;
}
template<>
uint32 DiscoveryType<uint16>() {
    return aitEnumUint16;
}
template<>
uint32 DiscoveryType<int32>() {
    return aitEnumInt32;
}
template<>
uint32 DiscoveryType<uint32>() {
    return aitEnumUint32;
}
template<>
uint32 DiscoveryType<float32>() {
    return aitEnumFloat32;
}
template<>
uint32 DiscoveryType<float64>() {
    return aitEnumFloat64;
}

template<>
TypeDescriptor DiscoveryTypeDescriptor<int8>() {
    return SignedInteger8Bit;
}
template<>
TypeDescriptor DiscoveryTypeDescriptor<uint8>() {
    return UnsignedInteger8Bit;
}
template<>
TypeDescriptor DiscoveryTypeDescriptor<int16>() {
    return SignedInteger16Bit;
}
template<>
TypeDescriptor DiscoveryTypeDescriptor<uint16>() {
    return UnsignedInteger16Bit;
}
template<>
TypeDescriptor DiscoveryTypeDescriptor<int32>() {
    return SignedInteger32Bit;
}
template<>
TypeDescriptor DiscoveryTypeDescriptor<uint32>() {
    return UnsignedInteger32Bit;
}
template<>
TypeDescriptor DiscoveryTypeDescriptor<float32>() {
    return Float32Bit;
}
template<>
TypeDescriptor DiscoveryTypeDescriptor<float64>() {
    return Float64Bit;
}

template<>
const char8 * DiscoveryStringType<int8>() {
    return "aitEnumInt8";
}
template<>
const char8 * DiscoveryStringType<uint8>() {
    return "aitEnumUint8";
}
template<>
const char8 * DiscoveryStringType<int16>() {
    return "aitEnumInt16";
}
template<>
const char8 * DiscoveryStringType<uint16>() {
    return "aitEnumUint16";
}
template<>
const char8 * DiscoveryStringType<int32>() {
    return "aitEnumInt32";
}
template<>
const char8 * DiscoveryStringType<uint32>() {
    return "aitEnumUint32";
}
template<>
const char8 * DiscoveryStringType<float32>() {
    return "aitEnumFloat32";
}
template<>
const char8 * DiscoveryStringType<float64>() {
    return "aitEnumFloat64";
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MARTePvTest::MARTePvTest() {
    // Auto-generated constructor stub for MARTePvTest
    // TODO Verify if manual additions are needed
}

MARTePvTest::~MARTePvTest() {
    // Auto-generated destructor stub for MARTePvTest
    // TODO Verify if manual additions are needed
}

bool MARTePvTest::TestInitialise() {
    const char8 * config = "TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    pv.SetName("jane");
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        ok = (pv.maxBound(0u) == 3u);
    }
    if (ok) {
        ok = (pv.bestExternalType() == aitEnumFloat64);
    }

    return ok;
}

bool MARTePvTest::TestInitialise_DefaultLen() {
    const char8 * config = "TYPE = aitEnumFloat64"
            "            SYNC = excasIoSync";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    pv.SetName("jane");
    if (ok) {
        ok = pv.Initialise(cdb);
    }
    if (ok) {
        ok = (pv.maxBound(0u) == 1u);
    }
    if (ok) {
        ok = (pv.bestExternalType() == aitEnumFloat64);
    }

    return ok;
}

bool MARTePvTest::TestInitialise_FalseUndefinedType() {
    const char8 * config = "NELM = 3"
            "    SYNC = excasIoSync";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeCommonPv pv;
    pv.SetName("jane");
    if (ok) {
        ok = !pv.Initialise(cdb);
    }
    return ok;
}

bool MARTePvTest::TestinterestRegister() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "        }"
            "        +albert = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    ReferenceT<MARTePv> jane;
    ReferenceT<MARTePv> albert;
    if (ok) {
        jane = ioc.Get(0);
        albert = ioc.Get(1);
        ok = (jane.IsValid()) && (albert.IsValid());
    }
    if (ok) {
        jane->SetIocServer(ioc);
        albert->SetIocServer(ioc);
    }
    uint32 nanos1;
    uint32 secs1;
    uint32 nanos2;
    uint32 secs2;
    uint32 nanosA1;
    uint32 secsA1;
    uint32 nanosA2;
    uint32 secsA2;
    if (ok) {
        jane->scan();
        jane->GetTimeStamp(secs1, nanos1);
        ok = (jane->interestRegister() == S_casApp_success);
    }
    if (ok) {
        albert->scan();
        albert->GetTimeStamp(secs2, nanos2);
        ok = (albert->interestRegister() == S_casApp_success);
    }
    if (ok) {
        Sleep::Sec(1.2);
    }
    if (ok) {
        jane->GetTimeStamp(secsA1, nanosA1);
        ok = (nanosA1 != nanos1);
    }
    if (ok) {
        albert->GetTimeStamp(secsA2, nanosA2);
        ok = (nanosA2 != nanos2);
    }
    return ok;
}

bool MARTePvTest::TestinterestDelete() {

    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "        }"
            "        +albert = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    ReferenceT<MARTePv> jane;
    ReferenceT<MARTePv> albert;
    if (ok) {
        jane = ioc.Get(0);
        albert = ioc.Get(1);
        ok = (jane.IsValid()) && (albert.IsValid());
    }
    if (ok) {
        jane->SetIocServer(ioc);
        albert->SetIocServer(ioc);
    }
    uint32 nanos1;
    uint32 secs1;
    uint32 nanos2;
    uint32 secs2;
    uint32 nanosA1;
    uint32 secsA1;
    uint32 nanosA2;
    uint32 secsA2;
    uint32 nanosAA1;
    uint32 secsAA1;
    uint32 nanosAA2;
    uint32 secsAA2;

    if (ok) {
        jane->scan();
        jane->GetTimeStamp(secs1, nanos1);
        ok = (jane->interestRegister() == S_casApp_success);
    }
    if (ok) {
        albert->scan();
        albert->GetTimeStamp(secs2, nanos2);
        ok = (albert->interestRegister() == S_casApp_success);
    }
    if (ok) {
        Sleep::Sec(1.2);
    }
    if (ok) {
        jane->interestDelete();
        jane->GetTimeStamp(secsA1, nanosA1);
        ok = (nanosA1 != nanos1);

    }
    if (ok) {
        albert->interestDelete();
        albert->GetTimeStamp(secsA2, nanosA2);
        ok = (nanosA2 != nanos2);
    }
    if (ok) {
        Sleep::Sec(1.2);
    }
    if (ok) {
        jane->GetTimeStamp(secsAA1, nanosAA1);
        ok = (nanosAA1 == nanosA1);

    }
    if (ok) {
        albert->GetTimeStamp(secsAA2, nanosAA2);
        ok = (nanosAA2 == nanosA2);
    }

    return ok;
}

bool MARTePvTest::TestbestExternalType() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane1 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumInt8"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "        }"
            "        +jane2 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumUint8"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }"
            "        +jane3 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumInt16"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }"
            "        +jane4 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumUint16"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }"
            "        +jane5 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumInt32"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }"
            "        +jane6 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumUint32"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }"
            "        +jane7 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat32"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }"
            "        +jane8 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.1"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }

    aitEnum types[] = { aitEnumInt8, aitEnumUint8, aitEnumInt16, aitEnumUint16, aitEnumInt32, aitEnumUint32, aitEnumFloat32, aitEnumFloat64, aitEnumFixedString,
            aitEnumString, aitEnumContainer, (aitEnum) 0 };

    for (uint32 i = 0u; (i < 8u) && ok; i++) {
        ReferenceT<MARTePv> pv = ioc.Get(i);
        ok = pv.IsValid();
        if (ok) {
            ok = (pv->bestExternalType() == types[i]);
        }
    }
    return ok;
}

bool MARTePvTest::TestGetScanPeriod() {

    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.25"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    ReferenceT<MARTePv> pv;
    if (ok) {
        pv = ioc.Get(0);
        ok = pv.IsValid();
    }
    if (ok) {
        ok = (pv->GetScanPeriod() == 0.25);
    }
    return ok;
}

bool MARTePvTest::Testdestroy() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.25"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }

    ReferenceT<MARTePv> pv;
    if (ok) {
        pv = ioc.Get(0);
        ok = pv.IsValid();
    }
    if (ok) {
        pv->destroy();
        AnyType at = pv->GetAnyType();
        ok = !at.IsVoid();
    }
    return ok;
}

bool MARTePvTest::TestgetName() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.25"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }

    ReferenceT<MARTePv> pv;
    if (ok) {
        pv = ioc.Get(0);
        ok = pv.IsValid();
    }
    if (ok) {
        StreamString name = pv->getName();
        ok = (name == "jane");
    }
    return ok;
}

bool MARTePvTest::TestcreateChannel() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.25"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }

    ReferenceT<MARTePv> pv;
    if (ok) {
        pv = ioc.Get(0);
        ok = pv.IsValid();
    }
    if (ok) {
        casCtx *ctx = new casCtx;
        casChannel *channel = pv->createChannel(*ctx, "boh", "riboh");
        ok = (channel != NULL);
        delete channel;
        delete ctx;
    }
    return true;
}

bool MARTePvTest::TestSetIocServer() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumFloat64"
            "            NELM = 3"
            "            SYNC = excasIoSync"
            "            SCAN = 0.25"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }

    ReferenceT<MARTePv> pv;
    if (ok) {
        pv = ioc.Get(0);
        ok = pv.IsValid();
    }
    if (ok) {
        pv->SetIocServer(ioc);
    }
    return ok;
}
bool MARTePvTest::TestGetNumberOfElements() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane1 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumInt8"
            "            NELM = 1"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "        }"
            "        +jane2 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumUint8"
            "            NELM = 3"
            "            SYNC = excasIoAsync"
            "            SCAN = 0.1"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    ReferenceT<MARTePv> jane;
    ReferenceT<MARTePv> jane2;
    if (ok) {
        jane = ioc.Get(0);
        jane2 = ioc.Get(1);
        ok = (jane.IsValid() && jane2.IsValid());
    }
    if (ok) {
        ok = (jane->GetNumberOfElements() == 1);
    }
    if (ok) {
        ok = (jane2->GetNumberOfElements() == 3);
    }
    return ok;
}

bool MARTePvTest::TestmaxDimension() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane1 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumInt8"
            "            NELM = 1"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "        }"
            "        +jane2 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumUint8"
            "            NELM = 3"
            "            SYNC = excasIoAsync"
            "            SCAN = 0.1"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    ReferenceT<MARTePv> jane;
    ReferenceT<MARTePv> jane2;
    if (ok) {
        jane = ioc.Get(0);
        jane2 = ioc.Get(1);
        ok = (jane.IsValid() && jane2.IsValid());
    }
    if (ok) {
        ok = (jane->maxDimension() == 0);
    }
    if (ok) {
        ok = (jane2->maxDimension() == 1);
    }
    return ok;
}

bool MARTePvTest::TestmaxBound() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane1 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumInt8"
            "            NELM = 1"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "        }"
            "        +jane2 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumUint8"
            "            NELM = 3"
            "            SYNC = excasIoAsync"
            "            SCAN = 0.1"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    ReferenceT<MARTePv> jane;
    ReferenceT<MARTePv> jane2;
    if (ok) {
        jane = ioc.Get(0);
        jane2 = ioc.Get(1);
        ok = (jane.IsValid() && jane2.IsValid());
    }
    if (ok) {
        ok = (jane->maxBound(0) == 1u);
    }
    if (ok) {
        ok = (jane2->maxBound(0) == 3u);
    }
    if (ok) {
        ok = (jane2->maxBound(1) == 1u);
    }
    return ok;
}

bool MARTePvTest::TestSetUseLocalTimeStamp() {
    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane1 = {"
            "            Class = MARTeCommonPv"
            "            TYPE = aitEnumInt8"
            "            NELM = 1"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "            UseLocalTimeStamp = 0"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    ReferenceT<MARTePv> jane;
    if (ok) {
        jane = ioc.Get(0);
        ok = jane.IsValid();
    }
    if (ok) {
        ok = (!jane->UseLocalTimeStamp());
    }
    if (ok) {
        jane->SetUseLocalTimeStamp(true);
        ok = (jane->UseLocalTimeStamp());
    }
    uint32 currentSecs;
    uint32 currentNanos;
    uint32 secs = 1;
    uint32 nanos = 100;
    uint32 newSecs;
    uint32 newNanos;
    if (ok) {
        jane->GetTimeStamp(currentSecs, currentNanos);
        ok = ((secs != currentSecs) && (nanos != currentNanos));
    }
    if (ok) {
        jane->SetTimeStamp(secs, nanos);
        jane->GetTimeStamp(newSecs, newNanos);
        ok = ((newSecs == currentSecs) && (newNanos == currentNanos));
    }
    if (ok) {
        jane->SetUseLocalTimeStamp(false);
        ok = (!jane->UseLocalTimeStamp());
    }
    if (ok) {
        jane->SetTimeStamp(secs, nanos);
        jane->GetTimeStamp(newSecs, newNanos);
        ok = ((newSecs == secs) && (newNanos == nanos));
    }
    return ok;
}

bool MARTePvTest::TestUseLocalTimeStamp() {
    return TestSetUseLocalTimeStamp();
}

bool MARTePvTest::TestSetTimeStamp() {
    return TestSetUseLocalTimeStamp();
}

bool MARTePvTest::TestGetTimeStamp() {
    return TestSetUseLocalTimeStamp();
}

bool MARTePvTest::TestGddToAnyTypeConverter_created() {

    uint32 newSize = 1u;
    gdd* pDD = new gddAtomic(gddAppType_value, aitEnumUint8, 1u, newSize);
    pDD->unreference();

    uint8 input = 3u;
    pDD->put(input);

    AnyType at;
    bool ret = MARTePv::GddToAnyTypeConverter(at, *pDD, true);

    if (ret) {
        TypeDescriptor td = at.GetTypeDescriptor();
        ret = (td == UnsignedInteger8Bit);
    }

    if (ret) {
        ret = !at.IsVoid();
        if (ret) {
            uint8* ptr = (uint8*) at.GetDataPointer();
            ret = (*ptr == input);
        }
    }
    if (ret) {
        input = 5u;
        pDD->put(input);
        ret = MARTePv::GddToAnyTypeConverter(at, *pDD, false);
    }
    if (ret) {
        ret = (!at.IsVoid());
        if (ret) {
            uint8* ptr = (uint8*) at.GetDataPointer();
            ret = (*ptr == input);
            HeapManager::Free((void*&) ptr);
        }
    }


    return ret;
}

bool MARTePvTest::TestConvertToTypeDescriptor() {

    aitEnum gddType[] = { aitEnumInvalid, aitEnumInt8, aitEnumUint8, aitEnumInt16, aitEnumUint16, aitEnumEnum16, aitEnumInt32, aitEnumUint32, aitEnumFloat32,
            aitEnumFloat64, aitEnumFixedString, aitEnumString, aitEnumContainer, (aitEnum) 0 };

    TypeDescriptor typeDescriptors[] = { InvalidType, SignedInteger8Bit, UnsignedInteger8Bit, SignedInteger16Bit, UnsignedInteger16Bit, Character8Bit,
            SignedInteger32Bit, UnsignedInteger32Bit, Float32Bit, Float64Bit, Character8Bit, Character8Bit, StructuredDataInterfaceType, 0 };

    bool ret = true;
    for (uint32 i = 0u; (i < 13u) && (ret); i++) {
        TypeDescriptor td = MARTePv::ConvertToTypeDescriptor(gddType[i]);
        ret = (td == typeDescriptors[i]);
    }

    return ret;
}

