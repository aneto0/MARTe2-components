/**
 * @file ObjectConnectionITest.cpp
 * @brief Source file for class ObjectConnectionITest
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
 * the class ObjectConnectionITest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectConnectionITest.h"

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

ObjectConnectionITest::ObjectConnectionITest() {
}

ObjectConnectionITest::~ObjectConnectionITest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool ObjectConnectionITest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut) {

    ConfigurationDatabase config;
    ObjectConnectionI loader;
    bool ok = TestInitialiseWithConfiguration(configStreamIn, statusOut, config, loader);

    return ok;
}

bool ObjectConnectionITest::TestInitialiseWithConfiguration(StreamString configStreamIn, ErrorManagement::ErrorType& statusOut, ConfigurationDatabase& cdbOut, ObjectConnectionI& loader) {

    configStreamIn.Seek(0u);
    StandardParser parser(configStreamIn, cdbOut);
    bool ok = parser.Parse();

    if (ok) {
        ok = loader.Initialise(cdbOut);
        statusOut = loader.GetStatus();
    }

    return ok;
}

bool ObjectConnectionITest::TestConstructor() {

    ObjectConnectionI loader;

    return loader.GetStatus().ErrorsCleared();
}

bool ObjectConnectionITest::TestInitialise() {

    StreamString configStream = ""
        "Class = ObjectConnectionI \n"
        ""
        ;

    ErrorManagement::ErrorType status = ErrorManagement::FatalError;
    bool ok = TestInitialiseWithConfiguration(configStream, status);

    return (status.ErrorsCleared() && ok);
}

