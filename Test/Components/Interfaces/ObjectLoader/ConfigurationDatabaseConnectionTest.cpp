/**
 * @file ConfigurationDatabaseConnectionTest.cpp
 * @brief Source file for class ConfigurationDatabaseConnectionTest
 * @date 15/5/2025
 * @author nferron
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
 * the class ConfigurationDatabaseConnectionTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabaseConnectionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

// class TestObjectConnection : public MARTe::ObjectConnectionI {
// public:
//     CLASS_REGISTER_DECLARATION()
//
//     TestObjectConnection() : MARTe::ObjectConnectionI() {
//
//     }
//
//     ~TestObjectConnection() {
//         while (GetSize() > 0u) {
//             AnyType* toDelete;
//             if(Extract(0U, toDelete)) {
//                 delete toDelete;
//             }
//         }
//
//         while (paramNames.GetSize() > 0u) {
//             StreamString* toDelete;
//             if(paramNames.Extract(0U, toDelete)) {
//                 delete toDelete;
//             }
//         }
//     }
//
//     MARTe::StaticList<StreamString*>* GetParameterList() {
//         return &paramNames;
//     }
//
// };
//
// CLASS_REGISTER(TestObjectConnection, "1.0");

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ConfigurationDatabaseConnectionTest::ConfigurationDatabaseConnectionTest() {
}

ConfigurationDatabaseConnectionTest::~ConfigurationDatabaseConnectionTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool ConfigurationDatabaseConnectionTest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut) {

    ConfigurationDatabase config;
    ConfigurationDatabaseConnection loader;
    loader.SetName("CDBL1");
    bool ok = TestInitialiseWithConfiguration(configStreamIn, statusOut, config, loader);

    return ok;
}

bool ConfigurationDatabaseConnectionTest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut, ConfigurationDatabaseConnection& loader) {

    configStreamIn.Seek(0u);
    StandardParser parser(configStreamIn, cdbOut);
    bool ok = parser.Parse();

    if (ok) {
        ok = loader.Initialise(cdbOut);
        statusOut = loader.GetStatus();
    }

    return ok;
}

bool ConfigurationDatabaseConnectionTest::TestConstructor() {

    ConfigurationDatabaseConnection loader;

    return loader.GetStatus().ErrorsCleared();
}

bool ConfigurationDatabaseConnectionTest::TestInitialise() {

    StreamString configStream = ""
        "Class = ConfigurationDatabaseConnection \n"
        "Parameters = {                          \n"
        "    gain4 = (float64) 1                 \n"
        "    gainStruct4 = {                     \n"
        "        gain_a = (uint32) 10            \n"
        "        gain_b = (float32) {1, 2, 3, 4} \n"
        "    }                                   \n"
        "}                                       \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.ErrorsCleared() && ok);
}

bool ConfigurationDatabaseConnectionTest::TestInitialise_ParametersValues() {

    StreamString configStream = ""
        "Class = ConfigurationDatabaseConnection \n"
        "Parameters = {                          \n"
        "    gain4 = (float64) 1                 \n"
        "    gainStruct4 = {                     \n"
        "        gain_a = (uint32) 10            \n"
        "        gain_b = (float32) {1, 2, 3, 4} \n"
        "    }                                   \n"
        "}                                       \n"
        ""
        ;

    ConfigurationDatabaseConnection cdConnection;
    ConfigurationDatabase cdb;
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, cdb, cdConnection);

    float64 gain4_ref = 1;
    uint32  gain_a_ref = 10;
    float32 gain_b_ref[4] = {1, 2, 3, 4};
    ok &= (MemoryOperationsHelper::Compare(cdConnection[0u]->GetDataPointer(), &gain4_ref, cdConnection[0u]->GetDataSize()) == 0u);
    ok &= (MemoryOperationsHelper::Compare(cdConnection[1u]->GetDataPointer(), &gain_a_ref, cdConnection[1u]->GetDataSize()) == 0u);
    ok &= (MemoryOperationsHelper::Compare(cdConnection[2u]->GetDataPointer(), &gain_b_ref, cdConnection[2u]->GetDataSize()) == 0u);

    ok &= cdConnection.GetParameterName(0u) == StreamString("gain4");
    ok &= cdConnection.GetParameterName(1u) == StreamString("gainStruct4.gain_a");
    ok &= cdConnection.GetParameterName(2u) == StreamString("gainStruct4.gain_b");

    return (status.ErrorsCleared() && ok);
}

bool ConfigurationDatabaseConnectionTest::TestInitialise_DotNotation() {

    StreamString configStream = ""
        "Class = ConfigurationDatabaseConnection         \n"
        "Parameters = {                                  \n"
        "    gain4 = (float64) 1                         \n"
        "    gainStruct4.gain_a = (uint32) 10            \n"
        "    gainStruct4.gain_b = (float32) {1, 2, 3, 4} \n"
        "}                                               \n"
        ""
        ;

    ConfigurationDatabaseConnection cdConnection;
    ConfigurationDatabase cdb;
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, cdb, cdConnection);

    float64 gain4_ref = 1;
    uint32  gain_a_ref = 10;
    float32 gain_b_ref[4] = {1, 2, 3, 4};
    ok &= (MemoryOperationsHelper::Compare(cdConnection[0u]->GetDataPointer(), &gain4_ref, cdConnection[0u]->GetDataSize()) == 0u);
    ok &= (MemoryOperationsHelper::Compare(cdConnection[1u]->GetDataPointer(), &gain_a_ref, cdConnection[1u]->GetDataSize()) == 0u);
    ok &= (MemoryOperationsHelper::Compare(cdConnection[2u]->GetDataPointer(), &gain_b_ref, cdConnection[2u]->GetDataSize()) == 0u);

    ok &= cdConnection.GetParameterName(0u) == StreamString("gain4");
    ok &= cdConnection.GetParameterName(1u) == StreamString("gainStruct4.gain_a");
    ok &= cdConnection.GetParameterName(2u) == StreamString("gainStruct4.gain_b");

    return (status.ErrorsCleared() && ok);
}

bool ConfigurationDatabaseConnectionTest::TestInitialise_DashNotation() {

    StreamString configStream = ""
        "Class = ConfigurationDatabaseConnection         \n"
        "Parameters = {                                  \n"
        "    gain4 = (float64) 1                         \n"
        "    gainStruct4-gain_a = (uint32) 10            \n"
        "    gainStruct4-gain_b = (float32) {1, 2, 3, 4} \n"
        "}                                               \n"
        ""
        ;

    ConfigurationDatabaseConnection cdConnection;
    ConfigurationDatabase cdb;
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, cdb, cdConnection);

    float64 gain4_ref = 1;
    uint32  gain_a_ref = 10;
    float32 gain_b_ref[4] = {1, 2, 3, 4};
    ok &= (MemoryOperationsHelper::Compare(cdConnection[0u]->GetDataPointer(), &gain4_ref, cdConnection[0u]->GetDataSize()) == 0u);
    ok &= (MemoryOperationsHelper::Compare(cdConnection[1u]->GetDataPointer(), &gain_a_ref, cdConnection[1u]->GetDataSize()) == 0u);
    ok &= (MemoryOperationsHelper::Compare(cdConnection[2u]->GetDataPointer(), &gain_b_ref, cdConnection[2u]->GetDataSize()) == 0u);

    ok &= cdConnection.GetParameterName(0u) == StreamString("gain4");
    ok &= cdConnection.GetParameterName(1u) == StreamString("gainStruct4.gain_a");
    ok &= cdConnection.GetParameterName(2u) == StreamString("gainStruct4.gain_b");

    return (status.ErrorsCleared() && ok);
}

bool ConfigurationDatabaseConnectionTest::TestInitialise_Failed_ParametersNodeNotFound() {

    StreamString configStream = ""
        "Class = ConfigurationDatabaseConnection \n"
        "NotTheParameters = {                    \n"
        "    gain = (float64) 1                  \n"
        "}                                       \n"
        ""
        ;

    ConfigurationDatabaseConnection cdConnection;
    ConfigurationDatabase cdb;
    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    cdConnection.SetName("CDC1");
    bool ok = TestInitialiseWithConfiguration(configStream, status, cdb, cdConnection);

    return (status.parametersError && (!ok) );
}

