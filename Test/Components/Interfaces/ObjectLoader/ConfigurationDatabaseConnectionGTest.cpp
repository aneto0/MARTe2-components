/**
 * @file ConfigurationDatabaseConnectionGTest.cpp
 * @brief Source file for class ConfigurationDatabaseConnectionGTest
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
 * the class ConfigurationDatabaseConnectionGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabaseConnectionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ConfigurationDatabaseConnectionGTest,TestConstructor) {
    ConfigurationDatabaseConnectionTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(ConfigurationDatabaseConnectionGTest,TestInitialise) {
    ConfigurationDatabaseConnectionTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(ConfigurationDatabaseConnectionGTest,TestInitialise_ParametersValues) {
    ConfigurationDatabaseConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_ParametersValues());
}

TEST(ConfigurationDatabaseConnectionGTest,TestInitialise_DotNotation) {
    ConfigurationDatabaseConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_DotNotation());
}

TEST(ConfigurationDatabaseConnectionGTest,TestInitialise_DashNotation) {
    ConfigurationDatabaseConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_DashNotation());
}

TEST(ConfigurationDatabaseConnectionGTest,TestInitialise_Failed_ParametersNodeNotFound) {
    ConfigurationDatabaseConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_ParametersNodeNotFound());
}

