/**
 * @file ObjectLoaderTest.cpp
 * @brief Source file for class ObjectLoaderTest
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
 * the class ObjectLoaderTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectLoaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

// static bool TestIntegratedInApplication(const MARTe::char8 * const config,
//                                         MARTe::ObjectRegistryDatabase* objRegDatabase,
//                                         MARTe::ErrorManagement::ErrorType& status,
//                                         bool destroy = true) {
//
//     using namespace MARTe;
//
//     ConfigurationDatabase cdb;
//     StreamString configStream = config;
//     configStream.Seek(0);
//     StandardParser parser(configStream, cdb);
//
//     bool ok = parser.Parse();
//     cdb.MoveToRoot();
//
//     objRegDatabase = ObjectRegistryDatabase::Instance();
//
//     if (ok) {
//         //objRegDatabase->Purge();
//         ok = objRegDatabase->Initialise(cdb);
//     }
//
// //     ReferenceT<RealTimeApplication> application;
// //     if (ok) {
// //         application = objRegDatabase->Find("Test");
// //         ok = application.IsValid();
// //     }
// //
// //     if (ok) {
// //         ok = application->ConfigureApplication();
// //         ReferenceT<SimulinkWrapperGAMHelper> gam = objRegDatabase->Find("Test.Functions.GAM1");
// //         if (gam.IsValid()) {
// //             status = gam->GetStatus();
// //         }
// //     }
//
//     if (destroy) {
//         objRegDatabase->Purge();
//     }
//     return ok;
//
// }

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ObjectLoaderTest::ObjectLoaderTest() {
}

ObjectLoaderTest::~ObjectLoaderTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool ObjectLoaderTest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut) {

    ConfigurationDatabase config;
    ObjectLoader loader;
    bool ok = TestInitialiseWithConfiguration(configStreamIn, statusOut, config, loader);

    return ok;
}

bool ObjectLoaderTest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut, ObjectLoader& loader) {

    configStreamIn.Seek(0u);
    StandardParser parser(configStreamIn, cdbOut);
    bool ok = parser.Parse();

    if (ok) {
        ok = loader.Initialise(cdbOut);
        statusOut = loader.GetStatus();
    }

    return ok;
}

bool ObjectLoaderTest::TestConstructor() {

    ObjectLoader loader;

    return loader.GetStatus().ErrorsCleared();
}

bool ObjectLoaderTest::TestInitialise() {

    StreamString configStream = ""
        "Class = ObjectLoader                        \n"
        "+CDBConnection2 = {                         \n"
        "    Class = ConfigurationDatabaseConnection \n"
        "    Parameters = {                          \n"
        "        gain4 = (float64) 1                 \n"
        "        gainStruct4 = {                     \n"
        "            gain_a = (uint32) 10            \n"
        "            gain_b = (float32) {1, 2, 3, 4} \n"
        "        }                                   \n"
        "    }                                       \n"
        "}                                           \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.ErrorsCleared() && ok);
}

bool ObjectLoaderTest::TestInitialise_ParametersCopy() {

    ConfigurationDatabase cdb;
    ObjectLoader loader;

    StreamString configStream = ""
        "Class = ObjectLoader           \n"
        "Param1 = Hello                 \n"
        "Param2 = World                 \n"
        "+SubNode1 = {                  \n"
        "    Class = ReferenceContainer \n"
        "}                              \n"
        "+DoNotCopyThis = {             \n"
        "    Class = ReferenceContainer \n"
        "    Param3 = Error             \n"
        "}                              \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status, cdb, loader);

    if (ok) {
        ok &= cdb.MoveRelative("+SubNode1");

        StreamString temp;
        ok &= cdb.Read("Param1", temp);
        ok &= (temp == "Hello");

        temp = "";
        ok &= cdb.Read("Param2", temp);
        ok &= (temp == "World");

        // these should not be copied
        ok &= !cdb.Read("+DoNotCopyThis", temp);

        temp = "";
        ok &= cdb.Read("Class", temp);
        ok &= (temp != "ObjectLoader");
    }

    return (status.ErrorsCleared() && ok);
}

bool ObjectLoaderTest::TestInitialise_ObjectValues() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase *ord = NULL_PTR(ObjectRegistryDatabase*);

    StreamString config = ""
        "+LD1 = {                                                            \n"
        "    Class = ObjectLoader                               \n"
        "    +CDBConnection = {                                              \n"
        "        Class = ConfigurationDatabaseConnection                     \n"
        "        Parameters = {                                              \n"
        "            scalarDouble = (float64) 1                              \n"
        "            struct1 = {                                             \n"
        "                scalarUint32 = (uint32)  10                         \n"
        "                vectorSingle = (float32) { 1, 2, 3, 4 }             \n"
        "                matrixDouble = (float64) { { 1, 2, 3 }, {4, 5, 6} } \n"
        "            }                                                       \n"
        "        }                                                           \n"
        "    }                                                               \n"
        "}                                                                   \n"
        ""
        ;



    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ord = ObjectRegistryDatabase::Instance();
    ReferenceT<ObjectLoader> loader;
    ReferenceT<ObjectConnectionI> connection;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        loader = ord->Find("LD1");
        ok = loader.IsValid();
        if (ok) {
            connection = ord->Find("LD1.CDBConnection");
            ok = connection.IsValid();
        }
    }
    if (ok) {
        status = loader->GetStatus();
        if (status.ErrorsCleared()) {
            status = connection->GetStatus();
        }
        ok = status.ErrorsCleared();
    }

    if (ok) {
        float64 scalarDouble = 1.;
        uint32  scalarUint32 = 10;
        float32 vectorSingle[4] = {1., 2., 3., 4.};
        float64 matrixDouble[2][3] = { { 1, 2, 3 }, {4, 5, 6} };

        ReferenceT<AnyObject> anyScalarDouble = ord->Find("LD1.scalarDouble");
        ReferenceT<AnyObject> anyScalarUint32 = ord->Find("LD1.struct1.scalarUint32");
        ReferenceT<AnyObject> anyVectorSingle = ord->Find("LD1.struct1.vectorSingle");
        ReferenceT<AnyObject> anyMatrixDouble = ord->Find("LD1.struct1.matrixDouble");

        ok &= (anyScalarDouble.IsValid()) &&
            (MemoryOperationsHelper::Compare(&scalarDouble, anyScalarDouble->GetType().GetDataPointer(), anyScalarDouble->GetType().GetByteSize()) == 0u);
        ok &= (anyScalarUint32.IsValid()) &&
            (MemoryOperationsHelper::Compare(&scalarUint32, anyScalarUint32->GetType().GetDataPointer(), anyScalarUint32->GetType().GetByteSize()) == 0u);
        ok &= (anyVectorSingle.IsValid()) &&
            (MemoryOperationsHelper::Compare(&vectorSingle, anyVectorSingle->GetType().GetDataPointer(), anyVectorSingle->GetType().GetByteSize()) == 0u);
        ok &= (anyMatrixDouble.IsValid()) &&
            (MemoryOperationsHelper::Compare(&matrixDouble, anyMatrixDouble->GetType().GetDataPointer(), anyMatrixDouble->GetType().GetByteSize()) == 0u);

    }

    return (status.ErrorsCleared() && ok);
}

bool ObjectLoaderTest::TestInitialise_Failed_InvalidType() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase *ord = NULL_PTR(ObjectRegistryDatabase*);

    StreamString config = ""
        "+LD1 = {                                         \n"
        "    Class = ObjectLoader                         \n"
        "    +CDBConnection = {                           \n"
        "        Class = ConfigurationDatabaseConnection  \n"
        "        Parameters = {                           \n"
        "            scalarDouble = (float64) 1           \n"
        "        }                                        \n"
        "    }                                            \n"
        "}                                                \n"
        ""
        ;

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ord = ObjectRegistryDatabase::Instance();
    ReferenceT<ObjectLoader> loader;
    ReferenceT<ObjectConnectionI> connection;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        loader = ord->Find("LD1");
        ok = loader.IsValid();
        if (ok) {
            connection = ord->Find("LD1.CDBConnection");
            ok = connection.IsValid();
        }
    }
    if (ok) {
        status = loader->GetStatus();
        if (status.ErrorsCleared()) {
            status = connection->GetStatus();
        }
        ok = status.ErrorsCleared();
    }

    // invalidate the object
    if (ok) {
        AnyType* invalidAnyType = new AnyType();
        invalidAnyType->SetTypeDescriptor(InvalidType);
        ok = connection->Set(0u, invalidAnyType);
        if (ok) {
            status = loader->UpdateObjects();
        }
    }

    return (status.exception && ok);
}

bool ObjectLoaderTest::TestInitialise_Failed_CannotSerialise() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase *ord = NULL_PTR(ObjectRegistryDatabase*);

    StreamString config = ""
        "+LD1 = {                                         \n"
        "    Class = ObjectLoader                         \n"
        "    +CDBConnection = {                           \n"
        "        Class = ConfigurationDatabaseConnection  \n"
        "        Parameters = {                           \n"
        "            scalarDouble = (float64) 1           \n"
        "        }                                        \n"
        "    }                                            \n"
        "}                                                \n"
        ""
        ;

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ord = ObjectRegistryDatabase::Instance();
    ReferenceT<ObjectLoader> loader;
    ReferenceT<ObjectConnectionI> connection;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        loader = ord->Find("LD1");
        ok = loader.IsValid();
        if (ok) {
            connection = ord->Find("LD1.CDBConnection");
            ok = connection.IsValid();
        }
    }
    if (ok) {
        status = loader->GetStatus();
        if (status.ErrorsCleared()) {
            status = connection->GetStatus();
        }
        ok = status.ErrorsCleared();
    }

    // invalidate the object
    if (ok) {
        AnyType* invalidAnyType = new AnyType();
        invalidAnyType->SetDataPointer(NULL_PTR(void*));
        ok = connection->Set(0u, invalidAnyType);
        if (ok) {
            status = loader->UpdateObjects();
        }
    }

    return (status.internalSetupError && ok);
}


bool ObjectLoaderTest::TestInitialise_Failed_CannotInsert() {

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    ObjectRegistryDatabase *ord = NULL_PTR(ObjectRegistryDatabase*);

    StreamString config = ""
        "+LD1 = {                                         \n"
        "    Class = ObjectLoader                         \n"
        "    +CDBConnection = {                           \n"
        "        Class = ConfigurationDatabaseConnection  \n"
        "        Parameters = {                           \n"
        "            scalarDouble = (float64) 1           \n"
        "        }                                        \n"
        "    }                                            \n"
        "}                                                \n"
        ""
        ;

    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb, NULL);
    bool ok = parser.Parse();
    cdb.MoveToRoot();
    ord = ObjectRegistryDatabase::Instance();
    ReferenceT<ObjectLoader> loader;
    ReferenceT<ObjectConnectionI> connection;
    if (ok) {
        ok = ord->Initialise(cdb);
    }
    if (ok) {
        loader = ord->Find("LD1");
        ok = loader.IsValid();
        if (ok) {
            connection = ord->Find("LD1.CDBConnection");
            ok = connection.IsValid();
        }
    }
    if (ok) {
        status = loader->GetStatus();
        if (status.ErrorsCleared()) {
            status = connection->GetStatus();
        }
        ok = status.ErrorsCleared();
    }

    // invalidate the object
    if (ok) {
        AnyType* invalidAnyType = new AnyType();
        ok = connection->Add(invalidAnyType);
        if (ok) {
            status = loader->UpdateObjects();
        }
    }

    return (status.illegalOperation && ok);
}

