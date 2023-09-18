/**
 * @file OPCUAClientITest.cpp
 * @brief Source file for class OPCUAClientITest
 * @date 28/10/2019
 * @author Luca Porzio
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
 * the class OPCUAClientITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "ObjectRegistryDatabase.h"
#include "OPCUAClientI.h"
#include "OPCUAClientITest.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class TestHelperClass: public MARTe::OPCUAClientI {
public:
    TestHelperClass() : OPCUAClientI() {

    }

    ~TestHelperClass() {

    }

    virtual bool GetExtensionObjectByteString(const MARTe::TypeDescriptor *const&entryTypes,
                                              const MARTe::uint32 *const&entryArrayElements,
                                              const MARTe::uint32 *const&entryNumberOfMembers,
                                              const MARTe::uint32 entryArraySize,
                                              MARTe::uint32 &nodeCounter,
                                              MARTe::uint32 &index) {
        return true;
    }

    virtual bool SetServiceRequest(const MARTe::uint16 *const namespaceIndexes,
                                   MARTe::StreamString *const nodePaths,
                                   const MARTe::uint32 numberOfNodes) {
        return true;
    }

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool OPCUAClientITest::TestConstructor() {
    TestHelperClass thc;
    bool ok = false;
    if (thc.GetServerAddress() == "") {
        ok = true;
    }
    return ok;
}

bool OPCUAClientITest::Test_SetServerAddress() {
    TestHelperClass thc;
    bool ok = false;
    thc.SetServerAddress("test");
    if (thc.GetServerAddress() == "test") {
        ok = true;
    }
    return ok;
}

bool OPCUAClientITest::Test_Connect() {
    using namespace MARTe;
    StreamString config = ""
            "+ServerTest = {"
            "     Class = OPCUA::OPCUAServer"
            "     AddressSpace = {"
            "         MyNode = {"
            "             Type = uint8"
            "         }"
            "     }"
            "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    TestHelperClass thc;
    ok = false;
    thc.SetServerAddress("opc.tcp://localhost:4840");
    ok = thc.Connect();
    return ok;
}

bool OPCUAClientITest::Test_SetValueMemories() {
    TestHelperClass thc;
    bool ok = true;
    thc.SetValueMemories(32u);
    void **mem = thc.GetValueMemories();
    if (mem != NULL_PTR(void**)) {
        MARTe::uint32 n = thc.GetNumberOfNodes();
        for (MARTe::uint32 j = 0u; j < n; j++) {
            if (mem[j] != NULL_PTR(void*)) {
                ok = false;
            }
        }
    }
    else {
        ok = false;
    }
    return ok;
}

bool OPCUAClientITest::Test_SetDataPtr() {
    TestHelperClass thc;
    bool ok = true;
    MARTe::uint32 b = 5u;
    thc.SetDataPtr(b);
    void *ptr = thc.GetDataPtr();
    if (ptr == NULL_PTR(void*)) {
        ok = false;
    }
    return ok;
}

bool OPCUAClientITest::Test_GetSignalMemory() {
    TestHelperClass thc;
    bool ok = false;
    thc.SetValueMemories(32u);
    void * mem;
    MARTe::TypeDescriptor td(MARTe::SignedInteger8Bit);
    ok = thc.GetSignalMemory(mem,0u,td,1u);
    void **vm = thc.GetValueMemories();
    if (ok) {
        ok = (mem == vm[0]);
    }
    (void)MARTe::HeapManager::Free(vm[0u]);
    return ok;
}

