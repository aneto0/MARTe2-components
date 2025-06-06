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

TestObjectConnectionI::TestObjectConnectionI() {}

TestObjectConnectionI::~TestObjectConnectionI() {}

ErrorManagement::ErrorType TestObjectConnectionI::TestTransposeAndCopy(void *const destination, const void *const source, const TypeDescriptor typeDesc,
    const uint32 numberOfRows, const uint32 numberOfColumns, const uint32 numberOfPages) {

    return TransposeAndCopy(destination, source, typeDesc, numberOfRows, numberOfColumns, numberOfPages);
}

CLASS_REGISTER(TestObjectConnectionI, "1.0");

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

bool ObjectConnectionITest::TestTransposeAndCopy_Failed_InvalidType() {

    TestObjectConnectionI obConnection;
    obConnection.SetName("OBC1");
    ErrorManagement::ErrorType status = ErrorManagement::NoError;
    bool ok = true;

    uint32 rowMajorArray[2][3] = { {1, 2, 3}, {4, 5, 6} };
    uint32 colMajorArray[3][2] = { {1, 4}, {2, 5}, {3, 6} };
    uint32 outputArray[3][2]   = { {0, 0}, {0, 0}, {0, 0} };

    TypeDescriptor typeDesc = InvalidType;

    status = obConnection.TestTransposeAndCopy(outputArray, rowMajorArray, typeDesc, 2, 3, 1);
    ok = (MemoryOperationsHelper::Compare(&outputArray[0][0], &colMajorArray[0][0], 6u*sizeof(uint32)) == 0u);

    return (status.exception && !ok);
}

