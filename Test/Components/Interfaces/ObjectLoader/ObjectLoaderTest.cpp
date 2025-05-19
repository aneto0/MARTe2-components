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

bool ObjectLoaderTest::TestConstructor() {

    ObjectLoaderTestInterface loader;

    return loader.GetStatus().ErrorsCleared();
}

bool ObjectLoaderTest::TestInitialise() {

    ObjectLoaderTestInterface loader;
    ConfigurationDatabase     config;

    StreamString configStream = ""
        "Class = ObjectLoader                       "
        "+CDBConnection2 = {                        "
        "    Class = ConfigurationDatabaseConnection"
        "    Parameters = {                         "
        "        gain4 = (float64) 1                "
        "        gainStruct4 = {                    "
        "            gain_a = (uint32) 10           "
        "            gain_b = (float32) {1, 2, 3, 4}"
        "        }                                  "
        "    }                                      "
        "}                                          "
        ""
        ;

    StandardParser parser(configStream, config);
    bool ok = parser.Parse();

    if (ok) {
        ok = loader.Initialise(config);
    }
    return ok;
}

