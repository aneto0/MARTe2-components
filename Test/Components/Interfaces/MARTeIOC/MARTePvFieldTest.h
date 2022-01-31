/**
 * @file MARTePvFieldTest.h
 * @brief Header file for class MARTePvFieldTest
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

 * @details This header file contains the declaration of the class MARTePvFieldTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVFIELDTEST_H_
#define TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVFIELDTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "casCtx.h"
#include <stdio.h>
#include "dbMapper.h"
#include "db_access.h"
#include "gddApps.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MARTePvField.h"
#include "MARTeCommonPv.h"
#include "MARTeIocServer.h"
#include "GAMScheduler.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief MARTePvField Tests
 */
class MARTePvFieldTest {
public:

    /**
     * @brief Constructor
     */
    MARTePvFieldTest();
    virtual ~MARTePvFieldTest();

    /**
     * @brief Tests the read method
     */
    template<typename T>
    bool Testread(const char8* field,
                  T input);

    /**
     * @brief Tests the read method with a string
     */
    bool Testread_string(const char8* field,
                         const char8 * input);

    /**
     * @brief Tests the write method
     */
    template<typename T>
    bool Testwrite(const char8* field,
                   T input);

    /**
     * @brief Tests the write method with a string
     */
    bool Testwrite_string(const char8* field,
                          const char8 * input);

    /**
     * @brief Tests the ReadDirect method
     */
    template<typename T>
    bool TestReadDirect(const char8 *field,
                        T input);

    /**
     * @brief Tests the WriteDirect method
     */
    template<typename T>
    bool TestWriteDirect(const char8 *field,
                         T input);

    /**
     * @brief Tests the scan method
     */
    bool Testscan();

    /**
     * @brief Tests the writeNotify method
     */
    template<typename T>
    bool TestwriteNotify(const char8* field,
                         T input);

    /**
     * @brief Tests the writeNotify method with a string
     */
    bool TestwriteNotify_string(const char8* field,
                                const char8 * input);

    /**
     * @brief Tests the SetPvFather method
     */
    bool TestSetPvFather();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
bool MARTePvFieldTest::Testread(const char8* field,
                                T input) {
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
        T result = (T) (*gddRead);
        ok = (result == input);
    }
    delete ctx;

    return ok;
}

template<typename T>
bool MARTePvFieldTest::Testwrite(const char8* field,
                                 T input) {

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

        gddVal->put(input);
        test->interestRegister();

        ok = (test->write(*ctx, *gddVal) == S_casApp_success);
    }
    if (ok) {
        ok = (test->read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        T result = (T) (*gddRead);
        ok = (result == input);
    }
    delete ctx;

    return ok;
}

template<typename T>
bool MARTePvFieldTest::TestReadDirect(const char8 *field,
                                      T input) {

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

    MARTeIocServer server;
    if (ok) {
        pv.SetIocServer(server);
    }


    ReferenceT<MARTePvField> test;
    if (ok) {
        test = pv.Find(field);
        ok = (test.IsValid());
    }

    T result;
    if (ok) {
        test->interestRegister();
        ok = (test->ReadDirect(result) == S_casApp_success);
    }
    if (ok) {
        ok = (result == input);
    }
    return ok;
}

template<typename T>
bool MARTePvFieldTest::TestWriteDirect(const char8 *field,
                                       T input) {
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

    if (ok) {
        test->interestRegister();
        ok = (test->WriteDirect(input) == S_casApp_success);
    }
    T result;
    if (ok) {
        ok = (test->ReadDirect(result) == S_casApp_success);
    }
    if (ok) {
        ok = (result == input);
    }
    return ok;
}

template<typename T>
bool MARTePvFieldTest::TestwriteNotify(const char8* field,
                                       T input) {

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

        gddVal->put(input);
        test->interestRegister();

        ok = (test->writeNotify(*ctx, *gddVal) == S_casApp_success);
    }
    if (ok) {
        ok = (test->read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        T result = (T) (*gddRead);
        ok = (result == input);
    }
    delete ctx;

    return ok;
}

#endif /* TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVFIELDTEST_H_ */

