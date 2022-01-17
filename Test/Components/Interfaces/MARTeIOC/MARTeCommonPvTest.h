/**
 * @file MARTeCommonPvTest.h
 * @brief Header file for class MARTeCommonPvTest
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

 * @details This header file contains the declaration of the class MARTeCommonPvTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEAIPVTEST_H_
#define TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEAIPVTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "casCtx.h"
#include <stdio.h>
#include "dbMapper.h"
#include "db_access.h"
#include "gddApps.h"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
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
 * @brief MARTeCommonPv Tests
 */
class MARTeCommonPvTest {
public:

    /**
     * @brief Constructor
     */
    MARTeCommonPvTest();

    /**
     * @brief Destructor
     */
    virtual ~MARTeCommonPvTest();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with invalid HHSV parameter
     */
    bool TestInitialise_InvalidHHSV();

    /**
     * @brief Tests the Initialise method with invalid HSV parameter
     */
    bool TestInitialise_InvalidHSV();

    /**
     * @brief Tests the Initialise method with invalid LSV parameter
     */
    bool TestInitialise_InvalidLSV();

    /**
     * @brief Tests the Initialise method with invalid LLSV parameter
     */
    bool TestInitialise_InvalidLLSV();

    /**
     * @brief Tests the read method to read the precision
     */
    bool Testread_Precision();

    /**
     * @brief Tests the read method to read the high limit
     */
    bool Testread_HighLimit();

    /**
     * @brief Tests the read method to read the low limit
     */
    bool Testread_LowLimit();

    /**
     * @brief Tests the read method to read the high alarm
     */
    bool Testread_HighAlarm();

    /**
     * @brief Tests the read method to read the low alarm
     */
    bool Testread_LowAlarm();

    /**
     * @brief Tests the read method to read the high warning
     */
    bool Testread_HighWarning();

    /**
     * @brief Tests the read method to read the low warning
     */
    bool Testread_LowWarning();

    /**
     * @brief Tests the read method to read the units
     */
    bool Testread_Units();

    /**
     * @brief Tests the read method to read the enums
     */
    bool Testread_Enums();

    /**
     * @brief Tests the read method to read the ackt
     */
    bool Testread_Ackt();

    /**
     * @brief Tests the read method to read the acks
     */
    bool Testread_Acks();

    /**
     * @brief Tests the read method to read the number of elements
     */
    bool Testread_NumberOfElements();

    /**
     * @brief Tests the read method to read the timestamp
     */
    bool Testread_Timestamp();

    /**
     * @brief Tests the read method to read the hysteresis
     */
    bool Testread_Hyst();

    /**
     * @brief Tests the read method to read the mdel
     */
    bool Testread_Mdel();

    /**
     * @brief Tests the read method to read the adel
     */
    bool Testread_Adel();

    /**
     * @brief Tests the read method to read the high alarm severity
     */
    bool Testread_Hhsv();

    /**
     * @brief Tests the read method to read the high warning severity
     */
    bool Testread_Hsv();

    /**
     * @brief Tests the read method to read the low warning severity
     */
    bool Testread_Lsv();

    /**
     * @brief Tests the read method to read the low alarm severity
     */
    bool Testread_Llsv();

    /**
     * @brief Tests the read method to read the current severity
     */
    bool Testread_Sevr();

    /**
     * @brief Tests the read method to read the current status
     */
    bool Testread_Stat();

    /**
     * @brief Tests the write method to write the ackt
     */
    bool Testwrite_Ackt();

    /**
     * @brief Tests the write method to write the acks
     */
    bool Testwrite_Acks();

    /**
     * @brief Tests the write method to write the precision
     */
    bool Testwrite_Precision();

    /**
     * @brief Tests the write method to write the high limit
     */
    bool Testwrite_HighLimit();

    /**
     * @brief Tests the write method to write the low limit
     */
    bool Testwrite_LowLimit();

    /**
     * @brief Tests the write method to write the high alarm
     */
    bool Testwrite_HighAlarm();

    /**
     * @brief Tests the write method to write the low alarm
     */
    bool Testwrite_LowAlarm();

    /**
     * @brief Tests the write method to write the high alarm
     */
    bool Testwrite_HighWarning();

    /**
     * @brief Tests the write method to write the low warning
     */
    bool Testwrite_LowWarning();

    /**
     * @brief Tests the write method to write the units
     */
    bool Testwrite_Units();

    /**
     * @brief Tests the write method to write the enums
     */
    bool Testwrite_Enums();

    /**
     * @brief Tests the write method to write the timestamps
     */
    bool Testwrite_Timestamp();

    /**
     * @brief Tests the write method to write the hysteresis
     */
    bool Testwrite_Hyst();

    /**
     * @brief Tests the write method to write the mdel
     */
    bool Testwrite_Mdel();

    /**
     * @brief Tests the write method to write the adel
     */
    bool Testwrite_Adel();

    /**
     * @brief Tests the write method to write the high alarm severity
     */
    bool Testwrite_Hhsv();

    /**
     * @brief Tests the write method to write the high warning severity
     */
    bool Testwrite_Hsv();

    /**
     * @brief Tests the write method to write the low warning severity
     */
    bool Testwrite_Lsv();

    /**
     * @brief Tests the write method to write the low alarm severity
     */
    bool Testwrite_Llsv();

    /**
     * @brief Tests the write method that returns false if there is
     * a difference in dimensions
     */
    bool Testwrite_False1();

    /**
     * @brief Tests the write method that returns false if the conversion
     * fails (for example from string to numeric)
     */
    bool Testwrite_False2();

    /**
     * @brief Tests the write method that saturates the value if beyond the limits
     */
    bool Testwrite_saturated();

    /**
     * @brief Tests the read method
     */
    template<typename T>
    bool Testread(T input);

    /**
     * @brief Tests the write method
     */
    template<typename T>
    bool Testwrite(T input);

    /**
     * @brief Tests the read method with strings
     */
    bool Testread_string();

    /**
     * @brief Tests the write method with strings
     */
    bool Testwrite_string();

    /**
     * @brief Tests the read method with vectors
     */
    template<typename T>
    bool Testread_vector(T* input);

    /**
     * @brief Tests the write method with vectors
     */
    template<typename T>
    bool Testwrite_vector(T* input);

    /**
     * @brief Tests the read method with vectors of strings
     */
    bool Testread_vector_string();

    /**
     * @brief Tests the write method with vectors of strings
     */
    bool Testwrite_vector_string();

    /**
     * @brief Tests the WriteDirect method
     */
    template<typename T>
    bool TestWriteDirect(T input);

    /**
     * @brief Tests the ReadDirect method
     */
    template<typename T>
    bool TestReadDirect(T input);

    /**
     * @brief Tests the WriteDirect method with vectors
     */
    template<typename T>
    bool TestWriteDirect_vector(T* input);

    /**
     * @brief Tests the ReadDirect method with vectors
     */
    template<typename T>
    bool TestReadDirect_vector(T* input);

    /**
     * @brief Tests the WriteDirect method with strings
     */
    bool TestWriteDirect_string();

    /**
     * @brief Tests the ReadDirect method with strings
     */
    bool TestReadDirect_string();

    /**
     * @brief Tests the WriteDirect method with vectors of strings
     */
    bool TestWriteDirect_vector_string();

    /**
     * @brief Tests the ReadDirect method with vectors of strings
     */
    bool TestReadDirect_vector_string();

    /**
     * @brief Tests the WriteDirect method that saturates if the value is beyond the limits
     */
    bool TestWriteDirect_saturated();

    /**
     * @brief Tests the ReadDirect method that saturates if the value is beyond the limits
     */
    bool TestReadDirect_saturated();

    /**
     * @brief Tests the WriteDirect method that fails if it is not possible to
     * convert the input (i.e. from string to numeric)
     */
    bool TestWriteDirect_False();

    /**
     * @brief Tests the WriteDirect method that fails if it is not possible to
     * convert the input (i.e. from string to numeric)
     */
    bool TestWriteDirect_vector_False();

    /**
     * @brief Tests the ReadDirect method that fails in case of dimensions mismatch
     */
    bool TestReadDirect_False();

    /**
     * @brief Tests the ReadDirect method to read the precisio*
     * @brief Tests the writeNotify method
     */
    bool TestReadDirectPrecision();

    /**
     * @brief Tests the ReadDirect method to read the high limit
     */
    bool TestReadDirectHighLimit();

    /**
     * @brief Tests the ReadDirect method to read the low limit
     */
    bool TestReadDirectLowLimit();

    /**
     * @brief Tests the ReadDirect method to read the high alarm
     */
    bool TestReadDirectHighAlarm();

    /**
     * @brief Tests the ReadDirect method to read the low alarm
     */
    bool TestReadDirectLowAlarm();

    /**
     * @brief Tests the ReadDirect method to read the high warning
     */
    bool TestReadDirectHighWarning();

    /**
     * @brief Tests the ReadDirect method to read the low warning
     */
    bool TestReadDirectLowWarning();

    /**
     * @brief Tests the ReadDirect method to read the units
     */
    bool TestReadDirectUnits();

    /**
     * @brief Tests the ReadDirect method to read the enums
     */
    bool TestReadDirectEnums();

    /**
     * @brief Tests the ReadDirect method to read the ackt
     */
    bool TestReadDirectAckt();

    /**
     * @brief Tests the ReadDirect method to read the acks
     */
    bool TestReadDirectAcks();

    /**
     * @brief Tests the ReadDirect method to read the number of elements
     */
    bool TestReadDirectElementCount();

    /**
     * @brief Tests the ReadDirect method to read the timestamp
     */
    bool TestReadDirectTimeStamp();

    /**
     * @brief Tests the ReadDirect method to read the hysteresis
     */
    bool TestReadDirectHyst();

    /**
     * @brief Tests the ReadDirect method to read the mdel
     */
    bool TestReadDirectMdel();

    /**
     * @brief Tests the ReadDirect method to read the adel
     */
    bool TestReadDirectAdel();

    /**
     * @brief Tests the ReadDirect method to read the high alarm severity
     */
    bool TestReadDirectHhsv();

    /**
     * @brief Tests the ReadDirect method to read the high warning severity
     */
    bool TestReadDirectHsv();

    /**
     * @brief Tests the ReadDirect method to read the low warning severity
     */
    bool TestReadDirectLsv();

    /**
     * @brief Tests the ReadDirect method to read the low alarm severity
     */
    bool TestReadDirectLlsv();

    /**
     * @brief Tests the ReadDirect method to read the current status
     */
    bool TestReadDirectStat();

    /**
     * @brief Tests the ReadDirect method to read the current severity
     */
    bool TestReadDirectSevr();

    /**
     * @brief Tests the WriteDirect method to write the precision
     */
    bool TestWriteDirectPrecision();

    /**
     * @brief Tests the WriteDirect method to write the high limit
     */
    bool TestWriteDirectHighLimit();

    /**
     * @brief Tests the WriteDirect method to write the low limit
     */
    bool TestWriteDirectLowLimit();

    /**
     * @brief Tests the WriteDirect method to write the high alarm
     */
    bool TestWriteDirectHighAlarm();

    /**
     * @brief Tests the WriteDirect method to write the low alarm
     */
    bool TestWriteDirectLowAlarm();

    /**
     * @brief Tests the WriteDirect method to write the high warning
     */
    bool TestWriteDirectHighWarning();

    /**
     * @brief Tests the WriteDirect method to write the low warning
     */
    bool TestWriteDirectLowWarning();

    /**
     * @brief Tests the WriteDirect method to write the units
     */
    bool TestWriteDirectUnits();

    /**
     * @brief Tests the WriteDirect method to write the enums
     */
    bool TestWriteDirectEnums();

    /**
     * @brief Tests the WriteDirect method to write the ackt
     */
    bool TestWriteDirectAckt();

    /**
     * @brief Tests the WriteDirect method to write the acks
     */
    bool TestWriteDirectAcks();

    /**
     * @brief Tests the WriteDirect method to write the timestamp
     */
    bool TestWriteDirectTimeStamp();

    /**
     * @brief Tests the WriteDirect method to write the hysteresis
     */
    bool TestWriteDirectHyst();

    /**
     * @brief Tests the WriteDirect method to write the mdel
     */
    bool TestWriteDirectMdel();

    /**
     * @brief Tests the WriteDirect method to write the adel
     */
    bool TestWriteDirectAdel();

    /**
     * @brief Tests the WriteDirect method to write the high alarm severity
     */
    bool TestWriteDirectHhsv();

    /**
     * @brief Tests the WriteDirect method to write the high warning severity
     */
    bool TestWriteDirectHsv();

    /**
     * @brief Tests the WriteDirect method to write the low warning severity
     */
    bool TestWriteDirectLsv();

    /**
     * @brief Tests the WriteDirect method to write the low alarm severity
     */
    bool TestWriteDirectLlsv();

    /**
     * @brief Tests the scan method
     */
    bool Testscan();

    /**
     * @brief Tests the writeNotify method
     */
    template<typename T>
    bool TestwriteNotify(T input);

    /**
     * @brief Tests the writeNotify method with vectors
     */
    template<typename T>
    bool TestwriteNotify_vector(T *input);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
uint32 DiscoveryGddType();

template<typename T>
bool MARTeCommonPvTest::Testread(T input) {
    smartGDDPointer gddVal = new gddScalar(gddAppType_value, (aitEnum) DiscoveryGddType<T>());
    casCtx *ctx = new casCtx;

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
        gddVal->put(input);
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }
    //gddVal->unreference();

    smartGDDPointer gddRead = new gddScalar(gddAppType_value, aitEnumFloat64);
    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        if (input > T(10.)) {
            input = T(10.);
        }
        if (input < T(-10.)) {
            input = T(-10.);
        }

        ok = ((T) (*gddRead) == input);
    }
    delete ctx;
    //gddRead->unreference();
    return ok;

}

template<typename T>
bool MARTeCommonPvTest::Testwrite(T input) {
    return Testread(input);
}

template<typename T>
bool MARTeCommonPvTest::Testread_vector(T* input) {
    const uint32 numberOfElements = 5u;
    uint32 size = sizeof(T) * numberOfElements;
    void *ptr = HeapManager::Malloc(size);

    MemoryOperationsHelper::Copy(ptr, input, size);
    smartGDDPointer gddVal = new gddAtomic(gddAppType_value, (aitEnum) DiscoveryGddType<T>(), 1u, numberOfElements);

    gddVal->putRef(ptr, (aitEnum) DiscoveryGddType<T>(), new gddDestructor);
    casCtx *ctx = new casCtx;

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
        ok = (pv.write(*ctx, *gddVal) == S_casApp_success);
    }
    //gddVal->unreference();

//    float64 *ptr2=(float64*)HeapManager::Malloc(size);

    smartGDDPointer gddRead = new gdd(gddAppType_value);
    //Atomic(gddAppType_value, aitEnumFloat64, 1u, numberOfElements);
    //gddRead->putRef(ptr2, aitEnumFloat64, new gddDestructor);

    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        float64* ref;
        gddRead->getRef(ref);
        for (uint32 i = 0u; (i < numberOfElements) && ok; i++) {
            ok = ((T) (ref[i]) == input[i]);
        }
    }
    delete ctx;

    //gddRead->unreference();
    return ok;

}

template<typename T>
bool MARTeCommonPvTest::Testwrite_vector(T* input) {
    return Testread_vector(input);
}

template<typename T>
bool MARTeCommonPvTest::TestWriteDirect(T input) {
    const char8 *config = ""
            "            TYPE = aitEnumFloat64"
            "            VAL = 0.5"
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
        pv.interestRegister();
        ok = (pv.WriteDirect(input) == S_casApp_success);
    }
    T output;
    if (ok) {
        ok = (pv.ReadDirect(output) == S_casApp_success);
    }
    if (ok) {
        ok = (input == output);
    }
    return ok;
}

template<typename T>
bool MARTeCommonPvTest::TestReadDirect(T input) {
    return TestWriteDirect(input);
}

template<typename T>
bool MARTeCommonPvTest::TestWriteDirect_vector(T* input) {
    return true;
}

template<typename T>
bool MARTeCommonPvTest::TestReadDirect_vector(T* input) {
    return TestWriteDirect_vector(input);
}

template<typename T>
bool MARTeCommonPvTest::TestwriteNotify(T input) {
    smartGDDPointer gddVal = new gddScalar(gddAppType_value, (aitEnum) DiscoveryGddType<T>());
    casCtx *ctx = new casCtx;

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
        gddVal->put(input);
        ok = (pv.writeNotify(*ctx, *gddVal) == S_casApp_success);
    }
    //gddVal->unreference();

    smartGDDPointer gddRead = new gddScalar(gddAppType_value, aitEnumFloat64);
    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        if (input > T(10.)) {
            input = T(10.);
        }
        if (input < T(-10.)) {
            input = T(-10.);
        }

        ok = ((T) (*gddRead) == input);
    }
    delete ctx;

    //gddRead->unreference();
    return ok;

}

template<typename T>
bool MARTeCommonPvTest::TestwriteNotify_vector(T *input) {
    const uint32 numberOfElements = 5u;
    uint32 size = sizeof(T) * numberOfElements;
    void *ptr = HeapManager::Malloc(size);

    MemoryOperationsHelper::Copy(ptr, input, size);
    smartGDDPointer gddVal = new gddAtomic(gddAppType_value, (aitEnum) DiscoveryGddType<T>(), 1u, numberOfElements);

    gddVal->putRef(ptr, (aitEnum) DiscoveryGddType<T>(), new gddDestructor);
    casCtx *ctx = new casCtx;

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
        ok = (pv.writeNotify(*ctx, *gddVal) == S_casApp_success);
    }
    //gddVal->unreference();

    //    float64 *ptr2=(float64*)HeapManager::Malloc(size);

    smartGDDPointer gddRead = new gdd(gddAppType_value);
    //Atomic(gddAppType_value, aitEnumFloat64, 1u, numberOfElements);
    //gddRead->putRef(ptr2, aitEnumFloat64, new gddDestructor);

    if (ok) {
        ok = (pv.read(*ctx, *gddRead) == S_casApp_success);
    }
    if (ok) {
        float64* ref;
        gddRead->getRef(ref);
        for (uint32 i = 0u; (i < numberOfElements) && ok; i++) {
            ok = ((T) (ref[i]) == input[i]);
        }
    }
    delete ctx;

    //gddRead->unreference();
    return ok;
}

#endif /* TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEAIPVTEST_H_ */

