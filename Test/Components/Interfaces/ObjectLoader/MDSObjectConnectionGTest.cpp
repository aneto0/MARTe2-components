/**
 * @file MDSObjectConnectionGTest.cpp
 * @brief Source file for class MDSObjectConnectionGTest
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
 * the class MDSObjectConnectionGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MDSObjectConnectionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MDSObjectConnectionGTest,TestConstructor) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MDSObjectConnectionGTest,TestInitialise) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(MDSObjectConnectionGTest,TestInitialise_ThinClient) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_ThinClient());
}

TEST(MDSObjectConnectionGTest,TestInitialise_DistributedClient) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_DistributedClient());
}

TEST(MDSObjectConnectionGTest,TestInitialise_TargetDim) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_TargetDim());
}

TEST(MDSObjectConnectionGTest,TestInitialise_StartIdxStopIdx) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_StartIdxStopIdx());
}

TEST(MDSObjectConnectionGTest,TestInitialise_UnlinkedParameter) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_UnlinkedParameter());
}

TEST(MDSObjectConnectionGTest,TestInitialise_NestedParameters) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_NestedParameters());
}

TEST(MDSObjectConnectionGTest,TestInitialise_NestedParameters_WithDash) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_NestedParameters_WithDash());
}

TEST(MDSObjectConnectionGTest,TestInitialise_RowMajor) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_RowMajor());
}

TEST(MDSObjectConnectionGTest,TestInitialise_ColMajor) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_ColMajor());
}

TEST(MDSObjectConnectionGTest,TestInitialise_String_ThinClient) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_String("Thin"));
}

TEST(MDSObjectConnectionGTest,TestInitialise_String_DistributedClient) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_String("Distributed"));
}

TEST(MDSObjectConnectionGTest,TestInitialise_Struct_RowMajor_ThinClient) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_Struct_RowMajor("Thin"));
}

TEST(MDSObjectConnectionGTest,TestInitialise_Struct_RowMajor_DistributedClient) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_Struct_RowMajor("Distributed"));
}

TEST(MDSObjectConnectionGTest,TestInitialise_DictAsStruct_RowMajor) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_DictAsStruct_RowMajor());
}

TEST(MDSObjectConnectionGTest,TestInitialise_DictAsStruct_ColMajor) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_DictAsStruct_ColMajor());
}

TEST(MDSObjectConnectionGTest,TestInitialise_StructArray_RowMajor) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_StructArray_RowMajor());
}

TEST(MDSObjectConnectionGTest,TestInitialise_StructArray_ColMajor) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_StructArray_ColMajor());
}

TEST(MDSObjectConnectionGTest,TestInitialise_NoTree_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_NoTree_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_NoShot_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_NoShot_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_ThinClient_NoServer_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_ThinClient_NoServer_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_WrongClient_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_WrongClient_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_NoParameters_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_NoParameters_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_NoPath_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_NoPath_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_WrongOrientation_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_WrongOrientation_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_TargetDimAndStartIdx_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_TargetDimAndStartIdx_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_WrongPath_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_WrongPath_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_InvalidListItems_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_InvalidListItems_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_Invalid4DMatrix_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_Invalid4DMatrix_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_UnsupportedDataType_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_UnsupportedDataType_Failed());
}

TEST(MDSObjectConnectionGTest,TestInitialise_StringColMajor_Failed) {
    MDSObjectConnectionTest test;
    ASSERT_TRUE(test.TestInitialise_StringColMajor_Failed());
}

