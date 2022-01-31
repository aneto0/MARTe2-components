/**
 * @file MARTePvFieldTest.cpp
 * @brief Source file for class MARTePvFieldTest
 * @date 09/09/2019
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
 * the class MARTePvFieldTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTePvFieldTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MARTePvFieldTest::MARTePvFieldTest() {
    // Auto-generated constructor stub for MARTePvFieldTest
    // TODO Verify if manual additions are needed
}

MARTePvFieldTest::~MARTePvFieldTest() {
    // Auto-generated destructor stub for MARTePvFieldTest
    // TODO Verify if manual additions are needed
}

bool MARTePvFieldTest::Testread_string(const char8* field,
                                       const char8 * input) {

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
    MARTeIocServer server;
    if (ok) {
        pv.SetIocServer(server);
    }
    ReferenceT<MARTePvField> test;
    if (ok) {
        test = pv.Find(field);
        ok = (test.IsValid());
    }

    smartGDDPointer gddRead = new gdd(gddAppType_value);
    casCtx *ctx = new casCtx;

    if (ok) {
        test->interestRegister();
        ok = (test->read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        aitString temp = (aitString &) (*gddRead);
        StreamString result = (const char8 *) (temp);
        ok = (result == input);
    }
    delete ctx;

    return ok;
}

bool MARTePvFieldTest::Testwrite_string(const char8* field,
                                        const char8 * input) {
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
    MARTeIocServer server;
    if (ok) {
        pv.SetIocServer(server);
    }

    ReferenceT<MARTePvField> test;
    if (ok) {
        test = pv.Find(field);
        ok = (test.IsValid());
    }

    smartGDDPointer gddVal = new gdd(gddAppType_value);
    smartGDDPointer gddRead = new gdd(gddAppType_value);
    casCtx *ctx = new casCtx;

    if (ok) {
        aitString in = input;
        gddVal->put(in);
        test->interestRegister();

        ok = (test->write(*ctx, *gddVal) == S_casApp_success);
    }
    if (ok) {
        ok = (test->read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        aitString temp = (aitString &) (*gddRead);
        StreamString result = (const char8 *) (temp);
        ok = (result == input);
    }
    delete ctx;

    return ok;
}

bool MARTePvFieldTest::Testscan() {

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
    MARTeIocServer server;
    if (ok) {
        pv.SetIocServer(server);
    }

    ReferenceT<MARTePvField> test;
    if (ok) {
        test = pv.Find("PREC");
        ok = (test.IsValid());
    }

    uint32 sec1;
    uint32 nSec1;
    uint32 sec2;
    uint32 nSec2;

    if (ok) {
        uint32 newPrec = 8;
        ok = (test->WriteDirect(newPrec) == S_casApp_success);
        if (ok) {
            test->GetTimeStamp(sec1, nSec1);
        }
    }

    if (ok) {
        test->scan();
        if (ok) {
            test->GetTimeStamp(sec2, nSec2);
        }
        ok = (((sec2 - sec1) < 1) && (nSec2 != nSec1));
    }

    return ok;
}

bool MARTePvFieldTest::TestwriteNotify_string(const char8* field,
                                              const char8 * input) {
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
    MARTeIocServer server;
    if (ok) {
        pv.SetIocServer(server);
    }

    ReferenceT<MARTePvField> test;
    if (ok) {
        test = pv.Find(field);
        ok = (test.IsValid());
    }

    smartGDDPointer gddVal = new gdd(gddAppType_value);
    smartGDDPointer gddRead = new gdd(gddAppType_value);
    casCtx *ctx = new casCtx;

    if (ok) {
        aitString in = input;
        gddVal->put(in);
        ok = (test->writeNotify(*ctx, *gddVal) == S_casApp_success);
    }
    if (ok) {
        ok = (test->read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        aitString temp = (aitString &) (*gddRead);
        StreamString result = (const char8 *) (temp);
        ok = (result == input);
    }
    delete ctx;

    return ok;
}

bool MARTePvFieldTest::TestSetPvFather() {

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
    MARTeIocServer server;
    if (ok) {
        pv.SetIocServer(server);
    }

    MARTePvField test;

    if (ok) {
        test.SetPvFather(pv, gddAppType_precision, aitEnumUint32, true);
        uint32 precRead = 0u;
        test.ReadDirect(precRead);
        ok = (precRead == 4u);
    }

    return ok;
}
