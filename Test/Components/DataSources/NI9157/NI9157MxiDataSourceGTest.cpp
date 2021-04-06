/**
 * @file NI9157MxiDataSourceGTest.cpp
 * @brief Source file for class NI9157MxiDataSourceGTest.
 * @date 11/02/2021
 * @author Giuseppe Ferro
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class NI9157MxiDataSourceGTest (public, protected, and private). Be
 * aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "NI9157MxiDataSourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(NI9157MxiDataSourceGTest,TestConstructor) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI9157MxiDataSourceGTest,TestInitialise) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise(idx);
        }
        else {
            ret = test.TestInitialise(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestInitialise_DefaultRunNi) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_DefaultRunNi(idx);
        }
        else {
            ret = test.TestInitialise_DefaultRunNi(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestInitialise_False_NoNiDev) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_NoNiDev(idx);
        }
        else {
            ret = test.TestInitialise_False_NoNiDev(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestInitialise_False_InvalidNiDevPath) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestInitialise_False_InvalidNiDevPath(idx);
        }
        else {
            ret = test.TestInitialise_False_InvalidNiDevPath(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSetConfiguredDatabase) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSetConfiguredDatabase(idx);
        }
        else {
            ret = test.TestSetConfiguredDatabase(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSetConfiguredDatabase_False_InvalidLabviewVar) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSetConfiguredDatabase_False_InvalidLabviewVar(idx);
        }
        else {
            ret = test.TestSetConfiguredDatabase_False_InvalidLabviewVar(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSetConfiguredDatabase_False_InvalidType) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSetConfiguredDatabase_False_InvalidType(idx);
        }
        else {
            ret = test.TestSetConfiguredDatabase_False_InvalidType(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestGetBrokerName) {
    NI9157MxiDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(NI9157MxiDataSourceGTest,TestPrepareNextState) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestPrepareNextState(idx);
        }
        else {
            ret = test.TestPrepareNextState(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSynchronise(idx);
        }
        else {
            ret = test.TestSynchronise(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise_Variables) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSynchronise_Variables(idx);
        }
        else {
            ret = test.TestSynchronise_Variables(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise_FIFOs) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSynchronise_FIFOs(idx);
        }
        else {
            ret = test.TestSynchronise_FIFOs(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise_InitialPatterns) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSynchronise_InitialPatterns(idx);
        }
        else {
            ret = test.TestSynchronise_InitialPatterns(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestSynchronise_BlockIfNotRunning) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestSynchronise_BlockIfNotRunning(idx);
        }
        else {
            ret = test.TestSynchronise_BlockIfNotRunning(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestAsyncRead) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestAsyncRead(idx);
        }
        else {
            ret = test.TestAsyncRead(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestAsyncWrite) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestAsyncWrite(idx);
        }
        else {
            ret = test.TestAsyncWrite(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}

TEST(NI9157MxiDataSourceGTest,TestReset) {
    NI9157MxiDataSourceTest test;
    bool ret = true;
    for (uint32 idx = 0; idx < nDevices; idx++) {
        if(testAllRetTrue) {
            ret &= test.TestReset(idx);
        }
        else {
            ret = test.TestReset(idx);
            if (ret) {
                break;
            }
        }
    }
    ASSERT_TRUE(ret);
}
