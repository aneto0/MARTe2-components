/**
 * @file SimulinkWrapperGAMGTest.cpp
 * @brief Source file for class SimulinkWrapperGAMGTest
 * @date 22/03/2018
 * @author Bertrand Bauvir
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
 * the class SimulinkWrapperGAMGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SimulinkWrapperGAM.h"
#include "SimulinkWrapperGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

TEST(SimulinkWrapperGAMGTest, TestConstructor) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_MissingOptionalConfigurationSettings) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingOptionalConfigurationSettings());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_MissingLibrary) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_MissingLibrary());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_MissingSymbolPrefix) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_MissingSymbolPrefix());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_MissingTunableParamExternalSource) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_MissingTunableParamExternalSource());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LoadLibrary) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LoadLibrary());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LoadSymbols) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LoadSymbols());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LibraryMissingGetMmiFunction) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LibraryMissingGetMmiFunction());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_Failed_LibraryMissingAllocFunction) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_Failed_LibraryMissingAllocFunction());
}

TEST(SimulinkWrapperGAMGTest, TestInitialise_MissingParametersLeaf) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestInitialise_MissingParametersLeaf());
}

TEST(SimulinkWrapperGAMGTest, TestSetup) {
    SimulinkWrapperGAMTest test;
    ASSERT_TRUE(test.TestSetup());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

