/**
 * @file MARTePvTest.h
 * @brief Header file for class MARTePvTest
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

 * @details This header file contains the declaration of the class MARTePvTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVTEST_H_
#define TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include "dbMapper.h"
#include "db_access.h"
#include "gddApps.h"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MARTePv.h"
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
 * @brief MARTePv tests.
 */
class MARTePvTest {
public:

    /**
     * @brief Constructor
     */
    MARTePvTest();

    /**
     * @brief Destructor
     */
    virtual ~MARTePvTest();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with the default NELM parameter
     */
    bool TestInitialise_DefaultLen();

    /**
     * @brief Tests the Initialise method that returns false if TYPE is not
     * specified
     */
    bool TestInitialise_FalseUndefinedType();

    /**
     * @brief Tests the interestRegister method
     */
    bool TestinterestRegister();

    /**
     * @brief Tests the interestDelete method
     */
    bool TestinterestDelete();

    /**
     * @brief Tests the bestExternalType method
     */
    bool TestbestExternalType();

    /**
     * @brief Tests the TestGetScanPeriod method
     */
    bool TestGetScanPeriod();

    /**
     * @brief Tests the Testdestroy method
     */
    bool Testdestroy();

    /**
     * @brief Tests the TestgetName method
     */
    bool TestgetName();

    /**
     * @brief Tests the TestcreateChannel method
     */
    bool TestcreateChannel();

    /**
     * @brief Tests the TestSetIocServer method
     */
    bool TestSetIocServer();

    /**
     * @brief Tests the TestGetNumberOfElements method
     */
    bool TestGetNumberOfElements();

    /**
     * @brief Tests the TestmaxDimension method
     */
    bool TestmaxDimension();

    /**
     * @brief Tests the TestmaxBound method
     */
    bool TestmaxBound();

    /**
     * @brief Tests the TestSetUseLocalTimeStamp method
     */
    bool TestSetUseLocalTimeStamp();

    /**
     * @brief Tests the TestUseLocalTimeStamp method
     */
    bool TestUseLocalTimeStamp();

    /**
     * @brief Tests the TestSetTimeStamp method
     */
    bool TestSetTimeStamp();

    /**
     * @brief Tests the TestGetTimeStamp method
     */
    bool TestGetTimeStamp();

    /**
     * @brief Tests the TestGddToAnyTypeConverter method
     */
    template<typename T>
    bool TestGddToAnyTypeConverter(T input);

    /**
     * @brief Tests the TestGddToAnyTypeConverter method with the
     * AnyType memory already created
     */
    bool TestGddToAnyTypeConverter_created();

    /**
     * @brief Tests the ConvertToTypeDescriptor method
     */
    bool TestConvertToTypeDescriptor();

    /**
     * @brief Tests the GetAnyType method
     */
    template<typename T>
    bool TestGetAnyType(T input);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
uint32 DiscoveryType();

template<typename T>
TypeDescriptor DiscoveryTypeDescriptor();

template<typename T>
const char8 * DiscoveryStringType();

template<typename T>
bool MARTePvTest::TestGddToAnyTypeConverter(T input) {

    uint32 newSize = 1u;
    gdd* pDD = new gddAtomic(gddAppType_value, (aitEnum) DiscoveryType<T>(), 1u, newSize);
    pDD->unreference();

    pDD->put(input);

    AnyType at;
    bool ret = MARTePv::GddToAnyTypeConverter(at, *pDD, true);

    if (ret) {
        TypeDescriptor td = at.GetTypeDescriptor();
        ret = (td == DiscoveryTypeDescriptor<T>());
    }

    if (ret) {
        if (!at.IsVoid()) {
            T* ptr = (T*) at.GetDataPointer();
            ret = (*ptr == input);
            HeapManager::Free((void*&) ptr);
        }
    }
    return ret;
}

template<typename T>
bool MARTePvTest::TestGetAnyType(T input) {

    const char8 * config = "    Class = MARTeIocServer"
            "    AsyncScan = 1"
            "    AsyncDelay = 1"
            "    MaxSimultAsyncIO = 100"
            "        +jane = {"
            "            Class = MARTeCommonPv"
            "            NELM = 1"
            "            SYNC = excasIoSync"
            "            SCAN = 0.8"
            "        }";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();
    cdb.MoveAbsolute("+jane");
    cdb.Write("TYPE", DiscoveryStringType<T>());
    cdb.MoveToRoot();

    MARTeIocServer ioc;
    if (ok) {
        ok = ioc.Initialise(cdb);
    }
    if (ok) {
        ok = (ioc.IocWrite("jane", input) == S_casApp_success);
    }
    ReferenceT<MARTePv> jane;
    if (ok) {
        jane = ioc.Get(0);
        ok = jane.IsValid();
    }
    AnyType at;
    if (ok) {
        at = jane->GetAnyType();
        ok = !at.IsVoid();
    }
    if (ok) {
        T* ptr = (T*) at.GetDataPointer();
        ok = ((*ptr) == input);
    }

    return ok;
}

#endif /* TEST_COMPONENTS_INTERFACES_MARTEIOC_MARTEPVTEST_H_ */

