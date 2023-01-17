/**
 * @file MDSReaderTest.cpp
 * @brief Source file for class MDSReaderTest
 * @date 22/09/2017
 * @author Llorenc Capella
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
 * the class MDSReaderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "stdio.h"
#include <iostream>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MDSReaderTest.h"
#include "MDSReaderTestHelper.h"
#include "CreateTree.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MDSReaderTest::MDSReaderTest() {
    treeName = "test_tree";
    char *home = getenv("HOME");
    fullPath = treeName.Buffer();
    fullPath += "_path=";
    fullPath += home;
    //Important detail: fullPath must exist in all places where the environment variables is needed.
    //In other words, the scope of the environment variables is defined by the scope of the string (char *)
    //given to putenv() function.
    putenv((char *) (fullPath.Buffer()));
    CreateTree myTreeCreated(treeName);
    numberOfValidNodes = 10u;
    numberOfNodes = 11u;
    allValidNodeNames = new StreamString[numberOfValidNodes];
    allValidNodeNames[0] = "S_uint8";
    allValidNodeNames[1] = "S_int8";
    allValidNodeNames[2] = "S_uint16";
    allValidNodeNames[3] = "S_int16";
    allValidNodeNames[4] = "S_uint32";
    allValidNodeNames[5] = "S_int32";
    allValidNodeNames[6] = "S_uint64";
    allValidNodeNames[7] = "S_int64";
    allValidNodeNames[8] = "S_float32";
    allValidNodeNames[9] = "S_float64";
    allNodeNames = new StreamString[11];
    for (uint32 i = 0; i < numberOfValidNodes; i++) {
        allNodeNames[i] = allValidNodeNames[i];
    }
    allNodeNames[10] = "Info";
}

MDSReaderTest::MDSReaderTest(MARTe::StreamString name = "test_tree",
                             MARTe::uint32 nElementsPerSeg = 20,
                             MARTe::uint32 nSegments = 100,
                             MARTe::float64 elapsetTimeSeg = 2.0) {
    treeName = name;
    char *home = getenv("HOME");
    fullPath = treeName.Buffer();
    fullPath += "_path=";
    fullPath += home;
    //Important detail: fullPath must exist in all places where the environment variables is needed.
    //In other words, the scope of the environment variables is defined by the scope of the string (char *)
    //given to putenv() function.
    putenv((char *) (fullPath.Buffer()));
    CreateTree myTreeCreated(treeName, nElementsPerSeg, nSegments, elapsetTimeSeg);
    numberOfValidNodes = 10u;
    numberOfNodes = 11u;
    allValidNodeNames = new StreamString[numberOfValidNodes];
    allValidNodeNames[0] = "S_uint8";
    allValidNodeNames[1] = "S_int8";
    allValidNodeNames[2] = "S_uint16";
    allValidNodeNames[3] = "S_int16";
    allValidNodeNames[4] = "S_uint32";
    allValidNodeNames[5] = "S_int32";
    allValidNodeNames[6] = "S_uint64";
    allValidNodeNames[7] = "S_int64";
    allValidNodeNames[8] = "S_float32";
    allValidNodeNames[9] = "S_float64";
    allNodeNames = new StreamString[11];
    for (uint32 i = 0; i < numberOfValidNodes; i++) {
        allNodeNames[i] = allValidNodeNames[i];
    }
    allNodeNames[10] = "Info";
}

MDSReaderTest::~MDSReaderTest() {
    if (allNodeNames != NULL_PTR(StreamString *)) {
        delete[] allNodeNames;
        allNodeNames = NULL_PTR(StreamString *);
    }
    if (allValidNodeNames != NULL_PTR(StreamString *)) {
        delete[] allValidNodeNames;
        allValidNodeNames = NULL_PTR(StreamString *);
    }
    MDSplus::Tree *tree = new MDSplus::Tree(treeName.Buffer(), -1);
    uint32 shotNumber = tree->getCurrent(treeName.Buffer());
    while (shotNumber > 0u) {
        tree->deletePulse(shotNumber);
        shotNumber--;
        tree->setCurrent(treeName.Buffer(), shotNumber);
    }
    delete tree;
    StreamString strChar = getenv("HOME");
    strChar += "/";
    strChar += treeName.Buffer();
    strChar += "_model.characteristics";
    if (0 != remove(strChar.Buffer())) {
        printf("Error while removing %s\n", strChar.Buffer());
    }

    StreamString strData = getenv("HOME");
    strData += "/";
    strData += treeName.Buffer();
    strData += "_model.datafile";
    if (0 != remove(strData.Buffer())) {
        printf("Error while removing %s\n", strData.Buffer());
    }

    StreamString strTree = getenv("HOME");
    strTree += "/";
    strTree += treeName.Buffer();
    strTree += "_model.tree";

    if (0 != remove(strTree.Buffer())) {
        printf("Error while removing %s\n", strTree.Buffer());
    }
    StreamString strShot = getenv("HOME");
    strShot += "/";
    strShot += "shotid.sys";
    if (remove(strShot.Buffer()) != 0) {
        printf("Error while removing %s\n", strShot.Buffer());
    }
}

bool MDSReaderTest::TestInitialiseNoTreeName() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseNoShotNumber() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseWrongShotNumber() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", -1); //Wrong ShotNumber
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseWrongTreeName() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", "test_tree1"); // wrong tree name
    config.Write("ShotNumber", 1);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseUnexistingShotNumber() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 666); //unexisting ShotNumber
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseNoFrequency() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialiseNoSignals() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1);
    ok = !dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestInitialise() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1);
    config.CreateRelative("Signals");
    config.MoveToRoot();
    ok = dS.Initialise(config);
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoSignals() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    /*
     //example how to print a ConfigurationDatabase
     StreamString a;
     a.Printf("%!", config);
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    config.CreateAbsolute(".Signals");
    config.MoveToRoot();
    ok = dS.Initialise(config);
    config.MoveToRoot();
    config.Delete("Signals");
    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase0Functions() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.MoveToRoot();

    ok = dS.Initialise(config);
    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase2Functions() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("QualifiedName", "1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToRoot();
    ok = dS.Initialise(config);
    config.MoveToRoot();
    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase0Signals() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("QualifiedName", "1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();
    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseWrongSamples() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("QualifiedName", "1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 2);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();
    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseDiffSignalsAndFunctions() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("QualifiedName", "1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();
    /*
     StreamString a;
     a.Printf("%!", config);
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     printf("%s\n", a.Buffer());
     printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
     */
    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoNodeName() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute("Signals");
    config.MoveToRoot();
    ok = dS.Initialise(config);
    config.CreateRelative("0");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("QualifiedName", "1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    /*
     config.CreateRelative("0");
     config.Write("Samples", 1);
     config.MoveToAncestor(1u);
     config.CreateRelative("1");
     config.MoveToAncestor(1u);
     */
    config.Write("ByteSize", 10);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoNodeName_2() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("QualifiedName", "1");
    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidNodeName() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "InvalidNodeName"); //invalid name
    config.Write("QualifiedName", "1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseEqualNodeName() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_uint8"); //repeated
    config.Write("Type", "uint32");
    config.Write("QualifiedName", "1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "uint32");
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidNodeType() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("NodeName", "Info");
    config.Write("QualifiedName", "2");
    config.MoveToAncestor(1u);
    config.CreateRelative("3");
    config.Write("QualifiedName", "3");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.CreateRelative("3");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseWrongType() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "WrongType");
    config.Write("QualifiedName", "1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInconsistentType() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "float32");
    config.Write("QualifiedName", "1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoNumberOfElements() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("QualifiedName", "0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("Type", "uint32");
    config.Write("QualifiedName", "1");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidNumberOfElements() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.Write("NumberOfElements", 100);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("QualifiedName", "1");
    config.Write("NumberOfElements", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "int64");
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoTimeNumberOfElements() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.Write("NumberOfElements", 3);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("QualifiedName", "1");
    config.Write("NumberOfElements", 3);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "uint32");
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidTimeNumberOfElements() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("QualifiedName", "1");
    config.Write("NumberOfElements", 5);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "int64");
    config.Write("QualifiedName", "2");
    config.Write("NumberOfElements", 3);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidBytesSize() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("QualifiedName", "1");
    config.Write("NumberOfElements", 100);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "int32");
    config.Write("NumberOfElements", 1);
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidTimeBytesSize() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("QualifiedName", "1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "int32");
    config.Write("NumberOfElements", 1);
    config.Write("QualifiedName", "2");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoDataManagement() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute("Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("HoleManagement", 0);

    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("QualifiedName", "1");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);

    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "uint32");
    config.Write("QualifiedName", "2");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 4);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidDataManagement() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("Type", "uint8");
    config.Write("QualifiedName", "0");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 4);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("Type", "int8");
    config.Write("QualifiedName", "1");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("NumberOfElements", 1);
    config.Write("QualifiedName", "2");
    config.Write("ByteSize", 4);
    config.Write("Type", "uint32");

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseDataManagement0() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 400);
    config.Write("ByteSize", 400);
    config.Write("DataManagement", 0);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 0);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");
    config.Write("Type", "uint64");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 8);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseNoHoleManagement() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);

    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);

    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");
    config.Write("Type", "int32");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 4);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidHoleManagement() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 2);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");
    config.Write("Type", "int32");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 4);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidTimeType() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 1);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Name", "Time");
    config.Write("QualifiedName", "2");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 4);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseInvalidSamplingTime() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 1);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");
    config.Write("Name", "Time");
    config.Write("Type", "int64");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 4);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = !dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabase() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 1);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 1);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("QualifiedName", "2");
    config.Write("Type", "int32");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 4);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestSetConfiguredDatabaseShotNumber() {
    bool ok;
    MDSReader dS;
    ConfigurationDatabase config;
    config.Write("TreeName", treeName.Buffer());
    config.Write("ShotNumber", 0);
    config.Write("Frequency", 1000.0);
    config.CreateAbsolute(".Signals");
    config.CreateRelative("0");
    config.Write("NodeName", "S_uint8");
    config.Write("QualifiedName", "0");
    config.Write("Type", "uint8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 1);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.Write("NodeName", "S_int8");
    config.Write("QualifiedName", "1");
    config.Write("Type", "int8");
    config.Write("NumberOfElements", 100);
    config.Write("ByteSize", 100);
    config.Write("DataManagement", 2);
    config.Write("HoleManagement", 0);
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.Write("Type", "int32");
    config.Write("QualifiedName", "2");
    config.Write("NumberOfElements", 1);
    config.Write("ByteSize", 4);

    config.CreateAbsolute("Functions");
    config.CreateRelative("0");
    config.CreateRelative("InputSignals");
    config.CreateRelative("0");
    config.Write("Samples", 1);
    config.MoveToAncestor(1u);
    config.CreateRelative("1");
    config.MoveToAncestor(1u);
    config.CreateRelative("2");
    config.MoveToAncestor(1u);
    config.Write("ByteSize", 10);
    config.MoveToRoot();

    ok = dS.Initialise(config);

    config.MoveToRoot();

    if (ok) {
        ok = dS.SetConfiguredDatabase(config);
    }
    return ok;
}

bool MDSReaderTest::TestPrepareNextState() {
    MDSReader dS;
    return dS.PrepareNextState("state1", "state2");
}

bool MDSReaderTest::TestAllocateMemory() {
    MDSReader dS;
    return dS.AllocateMemory();
}

bool MDSReaderTest::TestGetBrokerName() {
    MDSReader dS;
    MARTe::ConfigurationDatabase emptyCD;
    MARTe::StreamString auxStr = dS.GetBrokerName(emptyCD, MARTe::InputSignals);
    return (auxStr == "MemoryMapSynchronisedInputBroker");
}

bool MDSReaderTest::TestGetInputBroker() {
    MDSReaderTestHelper dS(treeName);
    dS.CreateConfigurationFile();
    dS.Initialise(dS.config);
    dS.SetConfiguredDatabase(dS.config);
    MARTe::ReferenceContainer rC;
    void * ptr = &rC;
    bool ok = dS.GetInputBrokers(rC, "GAMNameLL", ptr);
    return ok;
}

bool MDSReaderTest::TestGetOutputBroker() {
    MDSReaderTestHelper dS(treeName);
    dS.CreateConfigurationFile();
    dS.Initialise(dS.config);
    dS.SetConfiguredDatabase(dS.config);
    MARTe::ReferenceContainer rC;
    void * ptr = &rC;
    bool ok = !dS.GetOutputBrokers(rC, "GAMNameLL", ptr);
    return ok;
}

bool MDSReaderTest::TestGetNumberOfMemoryBuffers() {
    MDSReader dS;
    return dS.GetNumberOfMemoryBuffers();
}

bool MDSReaderTest::TestGetSignalMemoryBufferNoMemory() {
    MDSReader dS;
    void *ptr = NULL_PTR(void *);
    return (!dS.GetSignalMemoryBuffer(1, 0, ptr));
}

bool MDSReaderTest::TestGetSignalMemoryBuffer() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void *ptr = NULL_PTR(void *);
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr);
        if (ok) {
            ok = (ptr != NULL_PTR(void *));
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 i = 0u; (i < 20) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise1() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 20; //200000;//20
    ok = dS.CreateConfigurationFile(0.1);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    printf("ready to synchronise\n");
    if (ok) {
        //for (uint32 i = 0u; (i < 10) && ok; i++) {
        for (uint32 i = 0u; (i < 4) && ok; i++) {
            ok = dS.Synchronise();

            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                //ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise2() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 40;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise3() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 15;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise4() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.2, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.2, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.2, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise5() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise6() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.08, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.08, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.08, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise7() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    dS.elementsRead = 32;
    ok = dS.CreateConfigurationFile(0.08, 1, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 5) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 0.8, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 1.6, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 3.2, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 6.4, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.08, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.08, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise8() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.2, 2, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 16, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.2, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.2, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise9() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise10() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 32;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint interation = 0u;
    if (ok) { //last read not verified
        for (uint32 i = 0u; (i < 1) && ok; i++) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        interation++;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), interation, 1, 5);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), interation, 1, 5);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), interation, 2, 5);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), interation, 2, 5);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), interation, 4, 5);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), interation, 4, 5);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, 5);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), interation, 8, 5);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), interation, 0.1, 5);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), interation, 0.1, 5);
        }
        for (uint32 i = 6u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        interation++;
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), interation, 1, 5);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), interation, 1, 5);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), interation, 2, 5);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), interation, 2, 5);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), interation, 4, 5);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), interation, 4, 5);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), interation, 8, 5);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), interation, 8, 5);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), interation, 0.1, 5);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), interation, 0.1, 5);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_1() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0.0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_2() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0.0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_3() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0.0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_4() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise11_5() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 20;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 20u; (i < 40) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise12() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= (((float32 *) ptr[8])[i] == 0);
            ok &= (((float64 *) ptr[9])[i] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise13() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), m + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), m + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), m + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), m + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), m + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), m + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), m + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), m + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), m + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), m + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 0);
                    ok &= (((int8 *) ptr[1])[j] == 0);
                    ok &= (((uint16 *) ptr[2])[j] == 0);
                    ok &= (((int16 *) ptr[3])[j] == 0);
                    ok &= (((uint32 *) ptr[4])[j] == 0);
                    ok &= (((int32 *) ptr[5])[j] == 0);
                    ok &= (((uint64 *) ptr[6])[j] == 0);
                    ok &= (((int64 *) ptr[7])[j] == 0);
                    ok &= (((float32 *) ptr[8])[j] == 0);
                    ok &= (((float64 *) ptr[9])[j] == 0);
                }
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise14() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise15() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise16() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 20 * (m + 1) - 1);
                    int8 aux = int8(20 * (m + 1) - 1) - 20;
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 40 * (m + 1) - 2);
                    int16 aux16 = int16(40 * (m + 1) - 2) - 40;
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 80 * (m + 1) - 4);
                    int32 aux32 = int32(80 * (m + 1) - 4) - 80;
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 160 * (m + 1) - 8);
                    int64 aux64 = int64(160 * (m + 1) - 8) - 160;
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(2 * (m + 1) - 0.1));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 2 * (m + 1) - 0.1);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise17() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise18() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise19() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 0);
                    int8 aux = int8(0);
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 0);
                    int16 aux16 = int16(0);
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 0);
                    int32 aux32 = int32(0);
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 0);
                    int64 aux64 = int64(0);
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise20() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise21() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise22() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 20 * (m + 1) - 1);
                    int8 aux = int8(20 * (m + 1) - 1) - 20;
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 40 * (m + 1) - 2);
                    int16 aux16 = int16(40 * (m + 1) - 2) - 40;
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 80 * (m + 1) - 4);
                    int32 aux32 = int32(80 * (m + 1) - 4) - 80;
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 160 * (m + 1) - 8);
                    int64 aux64 = int64(160 * (m + 1) - 8) - 160;
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(2 * (m + 1) - 0.1));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 2 * (m + 1) - 0.1);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise23() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise24() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 0);
            ok &= (((int8 *) ptr[1])[i] == 0);
            ok &= (((uint16 *) ptr[2])[i] == 0);
            ok &= (((int16 *) ptr[3])[i] == 0);
            ok &= (((uint32 *) ptr[4])[i] == 0);
            ok &= (((int32 *) ptr[5])[i] == 0);
            ok &= (((uint64 *) ptr[6])[i] == 0);
            ok &= (((int64 *) ptr[7])[i] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise25() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 0);
                    int8 aux = int8(0);
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 0);
                    int16 aux16 = int16(0);
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 0);
                    int32 aux32 = int32(0);
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 0);
                    int64 aux64 = int64(0);
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(0));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 0);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise26() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 15;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 5;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 1, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 1, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 1, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 1, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 1, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 1, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 i = 0u; (i < 10) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        elementsToCheck = 5;
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[10]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[10]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[10]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[10]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[10]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[10]), 2, 0.1, elementsToCheck);
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise27() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {

        ok = dS.Synchronise();
        elementsToCheck = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), 0, 1, elementsToCheck);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), 0, 2, elementsToCheck);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), 0, 4, elementsToCheck);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), 0, 8, elementsToCheck);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), 0, 0.1, elementsToCheck);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), 0, 0.1, elementsToCheck);
        }
        for (uint32 i = elementsToCheck; (i < dS.elementsRead) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        elementsToCheck = 10;
        for (uint32 i = 0u; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[15]), 1, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[15]), 1, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[15]), 1, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[15]), 1, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[15]), 1, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[15]), 1, 0.1, elementsToCheck);

    }
    if (ok) {
        elementsToCheck = 10;
        ok = dS.Synchronise();
        ok &= dS.CompareS_uint8((&((uint8 *) ptr[0])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_int8((&((int8 *) ptr[1])[0]), 2, 1, elementsToCheck);
        ok &= dS.CompareS_uint16((&((uint16 *) ptr[2])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_int16((&((int16 *) ptr[3])[0]), 2, 2, elementsToCheck);
        ok &= dS.CompareS_uint32((&((uint32 *) ptr[4])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_int32((&((int32 *) ptr[5])[0]), 2, 4, elementsToCheck);
        ok &= dS.CompareS_uint64((&((uint64 *) ptr[6])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_int64((&((int64 *) ptr[7])[0]), 2, 8, elementsToCheck);
        ok &= dS.CompareS_float32((&((float32 *) ptr[8])[0]), 2, 0.1, elementsToCheck);
        ok &= dS.CompareS_float64((&((float64 *) ptr[9])[0]), 2, 0.1, elementsToCheck);

        for (uint32 i = 10; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 39);
            ok &= (((int8 *) ptr[1])[i] == 19);
            ok &= (((uint16 *) ptr[2])[i] == 78);
            ok &= (((int16 *) ptr[3])[i] == 38);
            ok &= (((uint32 *) ptr[4])[i] == 156);
            ok &= (((int32 *) ptr[5])[i] == 76);
            ok &= (((uint64 *) ptr[6])[i] == 312);
            ok &= (((int64 *) ptr[7])[i] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise28() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 5;
    uint32 elementsToCheck = 0;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 m = 0u; m < 4; m++) {

            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                elementsToCheck = 5;
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), (m * 4) + i, 1, elementsToCheck);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), (m * 4) + i, 2, elementsToCheck);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), (m * 4) + i, 4, elementsToCheck);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), (m * 4) + i, 8, elementsToCheck);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), (m * 4) + i, 0.1, elementsToCheck);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), (m * 4) + i, 0.1, elementsToCheck);
                }
            }
            for (uint32 i = 0u; i < 4; i++) {
                ok = dS.Synchronise();
                for (uint32 j = 0; (j < dS.elementsRead) && ok; j++) {
                    ok &= (((uint8 *) ptr[0])[j] == 20 * (m + 1) - 1);
                    int8 aux = int8(20 * (m + 1) - 1) - 20;
                    ok &= (((int8 *) ptr[1])[j] == aux);
                    ok &= (((uint16 *) ptr[2])[j] == 40 * (m + 1) - 2);
                    int16 aux16 = int16(40 * (m + 1) - 2) - 40;
                    ok &= (((int16 *) ptr[3])[j] == aux16);
                    ok &= (((uint32 *) ptr[4])[j] == 80 * (m + 1) - 4);
                    int32 aux32 = int32(80 * (m + 1) - 4) - 80;
                    ok &= (((int32 *) ptr[5])[j] == aux32);
                    ok &= (((uint64 *) ptr[6])[j] == 160 * (m + 1) - 8);
                    int64 aux64 = int64(160 * (m + 1) - 8) - 160;
                    ok &= (((int64 *) ptr[7])[j] == aux64);
                    ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(2 * (m + 1) - 0.1));
                    ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 2 * (m + 1) - 0.1);
                }
            }

        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise29() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise30() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        for (uint32 j = 15u; (j < 25) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;
        for (uint32 j = 0u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise31() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 i = samplesToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise32() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 15u; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;

        for (uint32 i = 0u; (i < from) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise33() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise34() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        for (uint32 j = 15u; (j < 25) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;
        for (uint32 j = 0u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise35() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 i = samplesToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise36() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 15u; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;

        for (uint32 i = 0u; (i < from) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise37() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise38() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        for (uint32 j = 15u; (j < 25) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;
        for (uint32 j = 0u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise39() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        for (uint32 j = 6u; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        for (uint32 j = 0u; (j < 5) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            samplesToCheck = 10;
            from = 5;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            samplesToCheck = 10;
            from = 0;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 i = samplesToCheck; (i < 15) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise40() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 25;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    uint32 iteration = 0;
    if (ok) {
        ok = dS.Synchronise();
        from = 20;
        samplesToCheck = 5;
        for (uint32 j = 6u; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }

        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], 1, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 0;
        samplesToCheck = 15;
        ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
        ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
        ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
        ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
        ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
        ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);

        for (uint32 i = 15u; (i < 25) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        from = 10;
        samplesToCheck = 15;

        for (uint32 i = 0u; (i < from) && ok; i++) {
            ok &= (((uint8 *) ptr[0])[i] == 19);
            ok &= (((int8 *) ptr[1])[i] == -1);
            ok &= (((uint16 *) ptr[2])[i] == 38);
            ok &= (((int16 *) ptr[3])[i] == -2);
            ok &= (((uint32 *) ptr[4])[i] == 76);
            ok &= (((int32 *) ptr[5])[i] == -4);
            ok &= (((uint64 *) ptr[6])[i] == 152);
            ok &= (((int64 *) ptr[7])[i] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[i], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[i], 1.9);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise41() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 15;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 4);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 15;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 15;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 10;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 0;
        from = 0;
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 0;
        from = 0;
        for (uint32 j = samplesToCheck; (j < 15) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
    }
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 10;
        from = 5;
        for (uint32 j = 0; (j < from) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise42() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise43() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise44() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= (((float32 *) ptr[8])[j] == 0);
            ok &= (((float64 *) ptr[9])[j] == 0);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise45() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise46() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise47() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = samplesToCheck; (j < (samplesToCheck + 20)) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise48() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        for (uint32 j = 0; j < 20 && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0.0);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise49() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        for (uint32 j = 0; j < 20 && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0.0);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise50() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        for (uint32 j = 0; j < 20 && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0.0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0.0);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise51() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        for (uint32 j = 0; (j < 20) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 20;

        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }

        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise52() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        for (uint32 j = 0; (j < 20) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 20;

        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }

        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 80; (j < 100) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 39);
            ok &= (((int8 *) ptr[1])[j] == 19);
            ok &= (((uint16 *) ptr[2])[j] == 78);
            ok &= (((int16 *) ptr[3])[j] == 38);
            ok &= (((uint32 *) ptr[4])[j] == 156);
            ok &= (((int32 *) ptr[5])[j] == 76);
            ok &= (((uint64 *) ptr[6])[j] == 312);
            ok &= (((int64 *) ptr[7])[j] == 152);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(3.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 3.9);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise53() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 1, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        for (uint32 j = 0; (j < 20) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 20;

        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 40; (j < 60) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 19);
            ok &= (((int8 *) ptr[1])[j] == -1);
            ok &= (((uint16 *) ptr[2])[j] == 38);
            ok &= (((int16 *) ptr[3])[j] == -2);
            ok &= (((uint32 *) ptr[4])[j] == 76);
            ok &= (((int32 *) ptr[5])[j] == -4);
            ok &= (((uint64 *) ptr[6])[j] == 152);
            ok &= (((int64 *) ptr[7])[j] == -8);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(1.9));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 1.9);
        }

        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        from = 80;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 39, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 19, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 78, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 38, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 156, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 76, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 312, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 152, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 3.9, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 3.9, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise54() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 100;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 2, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration = 0;
    uint32 samplesToCheck = 0u;
    uint32 from = 0;
    if (ok) {
        ok = dS.Synchronise();

        from = 0;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, samplesToCheck);
        }
        samplesToCheck = 20;
        from = 20;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        from = 40;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, samplesToCheck);
        }
        samplesToCheck = 20;
        from = 60;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        from = 80;
        samplesToCheck = 20;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, samplesToCheck);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, samplesToCheck);
        }
    }

    if (ok) {
        ok = dS.Synchronise();
        samplesToCheck = 20;
        from = 0;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }

        for (uint32 j = 20; (j < 40) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 40;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
        for (uint32 j = 60; (j < 80) && ok; j++) {
            ok &= (((uint8 *) ptr[0])[j] == 0);
            ok &= (((int8 *) ptr[1])[j] == 0);
            ok &= (((uint16 *) ptr[2])[j] == 0);
            ok &= (((int16 *) ptr[3])[j] == 0);
            ok &= (((uint32 *) ptr[4])[j] == 0);
            ok &= (((int32 *) ptr[5])[j] == 0);
            ok &= (((uint64 *) ptr[6])[j] == 0);
            ok &= (((int64 *) ptr[7])[j] == 0);
            ok &= dS.IsEqualLargeMargins(((float32 *) ptr[8])[j], static_cast<float32>(0));
            ok &= dS.IsEqualLargeMargins(((float64 *) ptr[9])[j], 0);
        }
        samplesToCheck = 20;
        from = 80;
        iteration++;
        if (ok) {
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, samplesToCheck);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, samplesToCheck);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, samplesToCheck);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, samplesToCheck);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, samplesToCheck);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, samplesToCheck);
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise55() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile();
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 i = 0u; (i < 200) && ok; i++) {
            ok = dS.Synchronise();
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Synchronise failed in reading = %u (last segment idx = %u)", i, 199);
            }
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }
//    uint32 iteration = 199;
//    if (ok) {
//        ok = !dS.Synchronise();
//        if (ok) {
//            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), iteration, 1, dS.elementsRead);
//            ok &= dS.CompareS_int8(((int8 *) ptr[1]), iteration, 1, dS.elementsRead);
//            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), iteration, 2, dS.elementsRead);
//            ok &= dS.CompareS_int16(((int16 *) ptr[3]), iteration, 2, dS.elementsRead);
//            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), iteration, 4, dS.elementsRead);
//            ok &= dS.CompareS_int32(((int32 *) ptr[5]), iteration, 4, dS.elementsRead);
//            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), iteration, 8, dS.elementsRead);
//            ok &= dS.CompareS_int64(((int64 *) ptr[7]), iteration, 8, dS.elementsRead);
//            ok &= dS.CompareS_float32(((float32 *) ptr[8]), iteration, 0.1, dS.elementsRead);
//            ok &= dS.CompareS_float64(((float64 *) ptr[9]), iteration, 0.1, dS.elementsRead);
//        }
//    }

    if (ok) {
        uint32 iteration = 200;
        iteration++;
        ok = !dS.Synchronise();
        uint32 from = 0;
        if (ok) {
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, dS.elementsRead);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, dS.elementsRead);
        }
    }
    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise56() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 iteration;
    uint32 from = 0;
    if (ok) {
        for (uint32 i = 0u; (i < 50) && ok; i += 1) {
            for (uint32 j = 0; (j < 2) && ok; j++) {
                ok = dS.Synchronise();
                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Synchronise failed in read i = %u, j = %u", i, j);
                }
                if (ok) {
                    ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i * 4 + j, 1, dS.elementsRead);
                    ok &= dS.CompareS_int8(((int8 *) ptr[1]), i * 4 + j, 1, dS.elementsRead);
                    ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i * 4 + j, 2, dS.elementsRead);
                    ok &= dS.CompareS_int16(((int16 *) ptr[3]), i * 4 + j, 2, dS.elementsRead);
                    ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i * 4 + j, 4, dS.elementsRead);
                    ok &= dS.CompareS_int32(((int32 *) ptr[5]), i * 4 + j, 4, dS.elementsRead);
                    ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i * 4 + j, 8, dS.elementsRead);
                    ok &= dS.CompareS_int64(((int64 *) ptr[7]), i * 4 + j, 8, dS.elementsRead);
                    ok &= dS.CompareS_float32(((float32 *) ptr[8]), i * 4 + j, 0.1, dS.elementsRead);
                    ok &= dS.CompareS_float64(((float64 *) ptr[9]), i * 4 + j, 0.1, dS.elementsRead);
                }
            }
            for (uint32 j = 0; (j < 2) && ok; j++) {
                if (i == 49) { // the last two reads (which is the last segment) is a hole of data..
                    ok = !dS.Synchronise();
                }
                else {
                    ok = dS.Synchronise();
                    if (!ok) {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Synchronise 2 failed in read i = %u, j = %u", i, j);
                    }
                    iteration = i * 4 + j + 2;
                    if (ok) {
                        ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, dS.elementsRead);
                        ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, dS.elementsRead);
                    }
                }
            }
        }
    }
    if (ok) { //read 200
        iteration = 200;
        ok = !dS.Synchronise();
    }
    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise57() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2, "int32");
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    uint32 *ptr = NULL_PTR(uint32 *);
    if (ok) {
        ok = dS.GetSignalMemoryBuffer(numberOfValidNodes, 0, reinterpret_cast<void *&>(ptr));
    }
    uint32 timeRef = 0;
    if (ok) {
        for (uint32 i = 0u; (i < 49) && ok; i += 1) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= (*ptr == timeRef);
                timeRef += 1000000;
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise58() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2, "int32");
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    int32 *ptr = NULL_PTR(int32 *);
    if (ok) {
        ok = dS.GetSignalMemoryBuffer(numberOfValidNodes, 0, reinterpret_cast<void *&>(ptr));
    }
    int32 timeRef = 0;
    if (ok) {
        for (uint32 i = 0u; (i < 49) && ok; i += 1) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= (*ptr == timeRef);
                timeRef += 1000000;
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise59() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2, "uint64");
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    /*uint64 *ptr = NULL_PTR(uint64 *);
     if (ok) {
     ok = dS.GetSignalMemoryBuffer(numberOfValidNodes, 0, reinterpret_cast<void *&>(ptr));
     }*/
    uint64 timeRef = 0;
    if (ok) {
        for (uint32 i = 0u; (i < 49) && ok; i += 1) {
            ok = dS.Synchronise();
            if (ok) {
                //ok &= (*ptr == timeRef);
                timeRef += 1000000;
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise60() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 1, 0, 2, "int64");
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    int64 *ptr = NULL_PTR(int64 *);
    if (ok) {
        ok = dS.GetSignalMemoryBuffer(numberOfValidNodes, 0, reinterpret_cast<void *&>(ptr));
    }
    int64 timeRef = 0;
    if (ok) {
        for (uint32 i = 0u; (i < 49) && ok; i += 1) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= (*ptr == timeRef);
                timeRef += 1000000;
            }
        }
    }
    return ok;
}

bool MDSReaderTest::TestSynchronise61() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(2, 0, 0, 1);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 i = 0u; (i < 10) && ok; i += 1) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }

    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise62() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(2, 1, 0, 1);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 i = 0u; (i < 10) && ok; i += 1) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }

    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise63() {
    MDSReaderTestHelper dS(treeName);
    bool ok;
    ok = dS.CreateConfigurationFile(2, 2, 0, 1);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    if (ok) {
        for (uint32 i = 0u; (i < 5) && ok; i += 1) {
            ok = dS.Synchronise();
            if (ok) {
                ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_int8(((int8 *) ptr[1]), i, 1, dS.elementsRead);
                ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_int16(((int16 *) ptr[3]), i, 2, dS.elementsRead);
                ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_int32(((int32 *) ptr[5]), i, 4, dS.elementsRead);
                ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_int64(((int64 *) ptr[7]), i, 8, dS.elementsRead);
                ok &= dS.CompareS_float32(((float32 *) ptr[8]), i, 0.1, dS.elementsRead);
                ok &= dS.CompareS_float64(((float64 *) ptr[9]), i, 0.1, dS.elementsRead);
            }
        }
    }

    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise64() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 42;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 2);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 from = 0;
    uint32 elementsToCompare = 0;
    uint32 iteration = 0u;
    if (ok) {
        ok = dS.Synchronise();
        if (ok) {
            from = 0u;
            elementsToCompare = 20;
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), iteration, 1, elementsToCompare);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), iteration, 1, elementsToCompare);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), iteration, 2, elementsToCompare);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), iteration, 2, elementsToCompare);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), iteration, 4, elementsToCompare);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), iteration, 4, elementsToCompare);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), iteration, 8, elementsToCompare);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), iteration, 8, elementsToCompare);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), iteration, 0.1, elementsToCompare);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), iteration, 0.1, elementsToCompare);
        }
        if (ok) {
            from = 20;
            elementsToCompare = 20;
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, elementsToCompare);
        }
        if (ok) {
            from = 40;
            elementsToCompare = 2;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, elementsToCompare);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, elementsToCompare);
        }

    }

    if (ok) {
        ok = dS.Synchronise();
        if (ok) {
            from = 0u;
            elementsToCompare = 18;
            ok &= dS.CompareS_uint8(((uint8 *) ptr[0]), iteration, 1, elementsToCompare);
            ok &= dS.CompareS_int8(((int8 *) ptr[1]), iteration, 1, elementsToCompare);
            ok &= dS.CompareS_uint16(((uint16 *) ptr[2]), iteration, 2, elementsToCompare);
            ok &= dS.CompareS_int16(((int16 *) ptr[3]), iteration, 2, elementsToCompare);
            ok &= dS.CompareS_uint32(((uint32 *) ptr[4]), iteration, 4, elementsToCompare);
            ok &= dS.CompareS_int32(((int32 *) ptr[5]), iteration, 4, elementsToCompare);
            ok &= dS.CompareS_uint64(((uint64 *) ptr[6]), iteration, 8, elementsToCompare);
            ok &= dS.CompareS_int64(((int64 *) ptr[7]), iteration, 8, elementsToCompare);
            ok &= dS.CompareS_float32(((float32 *) ptr[8]), iteration, 0.1, elementsToCompare);
            ok &= dS.CompareS_float64(((float64 *) ptr[9]), iteration, 0.1, elementsToCompare);
        }
        if (ok) {
            from = 18;
            elementsToCompare = 20;
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, elementsToCompare);
        }
        if (ok) {
            from = 38;
            elementsToCompare = 4;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, elementsToCompare);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, elementsToCompare);
        }

    }

    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise65() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 42;
    bool ok;
    ok = dS.CreateConfigurationFile(0.1, 0, 0, 3);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 from = 0;
    uint32 elementsToCompare = 0;
    uint32 iteration = 0u;
    if (ok) {
        ok = dS.Synchronise();
        if (ok) {
            from = 0;
            elementsToCompare = 20;
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, elementsToCompare);
        }
        if (ok) {
            from = 20;
            elementsToCompare = 20;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, elementsToCompare);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, elementsToCompare);
        }
        if (ok) {
            from = 40;
            elementsToCompare = 2;
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, elementsToCompare);
        }

    }

    if (ok) {
        ok = dS.Synchronise();
        iteration++;
        if (ok) {
            from = 0u;
            elementsToCompare = 18;
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, elementsToCompare);
        }
        if (ok) {
            from = 18;
            elementsToCompare = 20;
            ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 1, elementsToCompare);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2, elementsToCompare);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4, elementsToCompare);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8, elementsToCompare);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.1, elementsToCompare);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1, elementsToCompare);
        }
        if (ok) {
            from = 38;
            elementsToCompare = 4;
            ok &= dS.CompareIntegerValue<uint8>(&(((uint8 *) (ptr[0]))[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int8>(&(((int8 *) ptr[1])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint16>(&(((uint16 *) ptr[2])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int16>(&(((int16 *) ptr[3])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint32>(&(((uint32 *) ptr[4])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int32>(&(((int32 *) ptr[5])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<uint64>(&(((uint64 *) ptr[6])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareIntegerValue<int64>(&(((int64 *) ptr[7])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float32>(&(((float32 *) ptr[8])[from]), iteration, 0, elementsToCompare);
            ok &= dS.CompareFloatPointValue<float64>(&(((float64 *) ptr[9])[from]), iteration, 0, elementsToCompare);
        }

    }

    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise66() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.02, 2, 0, 1);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 from = 0;
    uint32 elementsToCompare = 0;
    uint32 iteration = 0u;
    for (uint32 i = 0u; (i < 250) && ok; i++) { //each read are 8 saved samples. there are 20*100 samples saved--> to consume all data 250 reads are needed
        ok = dS.Synchronise();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Synchronise failed in read = %u", i);
        }
        iteration = i;
        if (ok) {
            from = 0;
            if (i == 249) {
                elementsToCompare = 40 - 4; //The last interpolation is not check due boundary elements, but can be done in the future.
            }
            else {
                elementsToCompare = 40;
            }
            ok &= dS.CompareS_uint8Hold(&((uint8 *) ptr[0])[from], iteration, 1, 5, elementsToCompare);
            ok &= dS.CompareS_int8Hold(&((int8 *) ptr[1])[from], iteration, 1, 5, elementsToCompare);
            ok &= dS.CompareS_uint16Hold(&((uint16 *) ptr[2])[from], iteration, 2, 5, elementsToCompare);
            ok &= dS.CompareS_int16Hold(&((int16 *) ptr[3])[from], iteration, 2, 5, elementsToCompare);
            ok &= dS.CompareS_uint32Hold(&((uint32 *) ptr[4])[from], iteration, 4, 5, elementsToCompare);
            ok &= dS.CompareS_int32Hold(&((int32 *) ptr[5])[from], iteration, 4, 5, elementsToCompare);
            ok &= dS.CompareS_uint64Hold(&((uint64 *) ptr[6])[from], iteration, 8, 5, elementsToCompare);
            ok &= dS.CompareS_int64Hold(&((int64 *) ptr[7])[from], iteration, 8, 5, elementsToCompare);
            ok &= dS.CompareS_float32Hold(&((float32 *) ptr[8])[from], iteration, 0.1, 5, elementsToCompare);
            ok &= dS.CompareS_float64Hold(&((float64 *) ptr[9])[from], iteration, 0.1, 5, elementsToCompare);
        }
    }
    if (ok) {
        ok = !dS.Synchronise(); //end nodecreate
    }

    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise67() {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 40;
    bool ok;
    ok = dS.CreateConfigurationFile(0.02, 1, 0, 1);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    uint32 from = 0;
    uint32 elementsToCompare = 0;
    uint32 iteration = 0u;
    for (uint32 i = 0u; (i < 249) && ok; i++) {
        ok = dS.Synchronise();
        iteration = i;
        if (ok) {
            from = 0;
            elementsToCompare = 40;
            //ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 0.2, elementsToCompare);
            //ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 0.2, elementsToCompare);
            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 0.4, elementsToCompare);
            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2.0 / 5, elementsToCompare);
            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4.0 / 5, elementsToCompare);
            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4.0 / 5, elementsToCompare);
            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8.0 / 5, elementsToCompare);
            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8.0 / 5, elementsToCompare);
            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.02, elementsToCompare);
            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1 / 5, elementsToCompare);
        }
    }

    delete[] ptr;
    return ok;
}

bool MDSReaderTest::TestSynchronise68(uint32 nOfSegments) {
    MDSReaderTestHelper dS(treeName);
    dS.elementsRead = 128;
    bool ok;
    ok = dS.CreateConfigurationFile(1 / 2e6, 0, 0, 1);
    if (ok) {
        ok = dS.Initialise(dS.config);
    }
    if (ok) {
        ok = dS.SetConfiguredDatabase(dS.config);
    }
    void **ptr = new void *[numberOfValidNodes];
    for (uint32 i = 0u; (i < numberOfValidNodes) && ok; i++) {
        ok = dS.GetSignalMemoryBuffer(i, 0, ptr[i]);
    }
    for (uint32 i = 0u; (i < nOfSegments) && ok; i++) {
        ok = dS.Synchronise();
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Synchronise failed in segment = %u (last segment idx = %u)", i, nOfSegments - 1);
        }
        if (ok) {
            //ok &= dS.CompareS_uint8(&((uint8 *) ptr[0])[from], iteration, 0.2, elementsToCompare);
            //ok &= dS.CompareS_int8(&((int8 *) ptr[1])[from], iteration, 0.2, elementsToCompare);
//            ok &= dS.CompareS_uint16(&((uint16 *) ptr[2])[from], iteration, 0.4, elementsToCompare);
//            ok &= dS.CompareS_int16(&((int16 *) ptr[3])[from], iteration, 2.0 / 5, elementsToCompare);
//            ok &= dS.CompareS_uint32(&((uint32 *) ptr[4])[from], iteration, 4.0 / 5, elementsToCompare);
//            ok &= dS.CompareS_int32(&((int32 *) ptr[5])[from], iteration, 4.0 / 5, elementsToCompare);
//            ok &= dS.CompareS_uint64(&((uint64 *) ptr[6])[from], iteration, 8.0 / 5, elementsToCompare);
//            ok &= dS.CompareS_int64(&((int64 *) ptr[7])[from], iteration, 8.0 / 5, elementsToCompare);
//            ok &= dS.CompareS_float32(&((float32 *) ptr[8])[from], iteration, 0.02, elementsToCompare);
//            ok &= dS.CompareS_float64(&((float64 *) ptr[9])[from], iteration, 0.1 / 5, elementsToCompare);
        }
    }

    delete[] ptr;
    return ok;
}
