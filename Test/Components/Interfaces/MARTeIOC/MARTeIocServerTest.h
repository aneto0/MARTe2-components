/**
 * @file MARTeIocServerTest.h
 * @brief Header file for class MARTeIocServerTest
 * @date 12/07/2019
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

 * @details This header file contains the declaration of the class MARTeIocServerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEIOCSERVERTEST_H_
#define TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEIOCSERVERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MARTeIocServer.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief MARTeIocServer tests
 */
class MARTeIocServerTest {
public:

    /**
     * @bief Constructor
     */
    MARTeIocServerTest();

    /**
     * @brief Destructor
     */
    virtual ~MARTeIocServerTest();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the createTimer method
     */
    bool TestCreateTimer();

    /**
     * @brief Tests the pvExistsTest method
     */
    bool TestPvExistTest();

    /**
     * @brief Tests the pvAttach method
     */
    bool TestPvAttach();

    /**
     * @brief Tests the Execute method
     */
    bool TestExecute();

    /**
     * @brief Tests the IocWrite method
     */
    template<typename T, typename T2>
    bool TestIocWrite(T input,
                      T2 expected);

    /**
     * @brief Tests the IocWrite method with vectors
     */
    template<typename T, typename T2>
    bool TestIocWriteVector(T* input,
                            T2* expected);

    /**
     * @brief Tests the IocRead method
     */
    template<typename T, typename T2>
    bool TestIocRead(T input,
                     T2 expected);

    /**
     * @brief Tests the IocRead method with vectors
     */
    template<typename T, typename T2>
    bool TestIocReadVector(T* input,
                           T2* expected);

    /**
     * @brief Tests the IocAddPv method
     */
    bool TestIocAddPv();

    /**
     * @brief Tests the IocRemovePv method
     */
    bool TestIocRemovePv();

    /**
     * @brief Tests the GetNumberOfPVs method
     */
    bool TestGetNumberOfPVs();

    /**
     * @brief Interactive test
     */
    bool TestInteractive();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T, typename T2>
bool MARTeIocServerTest::TestIocWrite(T input,
                                      T2 expected) {

    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0.1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
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

    if (ok) {
        ok = (ioc.IocWrite("jane", input) == 0);
        T2 readVal;
        if (ok) {
            ok = (ioc.IocRead("jane", readVal) == 0);
        }
        if (ok) {
            ok = (readVal = expected);
        }
    }

    return ok;
}

template<typename T, typename T2>
bool MARTeIocServerTest::TestIocRead(T input,
                                     T2 expected) {
    return TestIocWrite(input, expected);
}

template<typename T, typename T2>
bool MARTeIocServerTest::TestIocWriteVector(T* input,
                                            T2* expected) {

    const char8 *config = ""
            "    Class = MARTeIocServer"
            "    ScanOn = true"
            "    AsyncScan = true"
            "    AsyncDelay = 0.1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NAME = \"jane\""
            "            TYPE = aitEnumFloat32"
            "            NELM = 10"
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

    if (ok) {

        Vector < T > inputVec(input, 10u);
        Vector<T2> readVal(10u);
        Vector < T2 > expectedVec(expected, 10u);

        ok = (ioc.IocWrite("jane", inputVec) == 0);
        if (ok) {
            ok = (ioc.IocRead("jane", readVal) == 0);
        }
        if (ok) {
            for (uint32 i = 0u; (i < 10u) && ok; i++) {
                ok = (readVal[i] == expected[i]);
            }
        }
    }

    return ok;
}

template<typename T, typename T2>
bool MARTeIocServerTest::TestIocReadVector(T* input,
                       T2* expected) {
    return TestIocWriteVector(input, expected);
}

#endif /* TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEIOCSERVERTEST_H_ */

