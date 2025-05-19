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

class ObjectLoaderTestInterface: public ObjectLoader {
public:
    CLASS_REGISTER_DECLARATION();

    ObjectLoaderTestInterface() : MARTe::ObjectLoader() {}

    ~ObjectLoaderTestInterface() {}

    MARTe::ErrorManagement::ErrorType GetStatus() {
        return this->status;
    }

};

CLASS_REGISTER(ObjectLoaderTestInterface, "1.0")

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
    bool ok = TestInitialiseWithConfiguration(configStreamIn, statusOut, config);

    return ok;
}

bool ObjectLoaderTest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut) {

    ObjectLoaderTestInterface loader;

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

    ObjectLoaderTestInterface loader;

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
    bool ok = TestInitialiseWithConfiguration(configStream, status, cdb);

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

