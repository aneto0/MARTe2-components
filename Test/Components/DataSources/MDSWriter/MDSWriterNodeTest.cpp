/**
 * @file MDSWriterNodeTest.cpp
 * @brief Source file for class MDSWriterNodeTest
 * @date 16/02/2017
 * @author Andre Neto
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
 * the class MDSWriterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "mdsobjects.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "MDSWriterNode.h"
#include "MDSWriterNodeTest.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
MDSWriterNodeTest::MDSWriterNodeTest() {
    treeTestHelper.Create("mds_m2test");
}

MDSWriterNodeTest::~MDSWriterNodeTest() {
    treeTestHelper.Destroy();
}

bool MDSWriterNodeTest::TestConstructor() {
    using namespace MARTe;
    MDSWriterNode test;
    bool ok = !test.IsDecimatedMinMax();
    ok &= (test.GetDecimatedNodeName() == "");
    ok &= (test.GetExecutePeriodMicroSecond() == 0);
    ok &= (test.GetMakeSegmentAfterNWrites() == 0);
    ok &= (test.GetMinMaxResampleFactor() == 0);
    ok &= (test.GetNodeName() == "");
    ok &= (test.GetNodeType() == 0);
    ok &= (test.GetNumberOfElements() == 0);
    ok &= (test.GetPeriod() == 0.F);
    ok &= (test.GetPhaseShift() == 0);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == 0);
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_NoDecimatedNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 100);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= !test.IsDecimatedMinMax();
    ok &= (test.GetDecimatedNodeName() == "");
    ok &= (test.GetExecutePeriodMicroSecond() == (100 * (5e-7 * 1e6)));
    ok &= (test.GetMakeSegmentAfterNWrites() == 4);
    ok &= (test.GetMinMaxResampleFactor() == 0);
    ok &= (test.GetNodeName() == "AAA");
    ok &= (test.GetNodeType() == DTYPE_WU);
    ok &= (test.GetNumberOfElements() == 1);
    ok &= (test.GetPeriod() == 5e-7);
    ok &= (test.GetPhaseShift() == 0);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == sizeof(uint16));
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_DecimatedNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 100);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);

    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.IsDecimatedMinMax());
    ok &= (test.GetDecimatedNodeName() == "BBB");
    ok &= (test.GetExecutePeriodMicroSecond() == (100 * (5e-7 * 1e6)));
    ok &= (test.GetMakeSegmentAfterNWrites() == 4);
    ok &= (test.GetMinMaxResampleFactor() == 5);
    ok &= (test.GetNodeName() == "AAA");
    ok &= (test.GetNodeType() == DTYPE_WU);
    ok &= (test.GetNumberOfElements() == 1);
    ok &= (test.GetPeriod() == 5e-7);
    ok &= (test.GetPhaseShift() == 0);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == sizeof(uint16));
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_SamplePhase() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 100);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("SamplePhase", 3);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= !test.IsDecimatedMinMax();
    ok &= (test.GetDecimatedNodeName() == "");
    ok &= (test.GetExecutePeriodMicroSecond() == (100 * (5e-7 * 1e6)));
    ok &= (test.GetMakeSegmentAfterNWrites() == 4);
    ok &= (test.GetMinMaxResampleFactor() == 0);
    ok &= (test.GetNodeName() == "AAA");
    ok &= (test.GetNodeType() == DTYPE_WU);
    ok &= (test.GetNumberOfElements() == 1);
    ok &= (test.GetPeriod() == 5e-7);
    ok &= (test.GetPhaseShift() == 3);
    ok &= (test.GetStart() == 0);
    ok &= (test.GetTypeMultiplier() == sizeof(uint16));
    ok &= (test.IsUseTimeVector() == false);
    ok &= (test.GetNumberOfExecuteCalls() == 0);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_False_NoNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint22");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoPeriod() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadPeriod() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 0.0F);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoNumberOfElements() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadNumberOfElements() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 0);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoMakeSegmentAfterNWrites() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 4);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("Period", 5e-7);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadMakeSegmentAfterNWrites() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 10);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 0);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoMinMaxResampleFactor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadMinMaxResampleFactor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 0);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoAutomaticSegmentation() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadAutomaticSegmentation() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("AutomaticSegmentation", 2);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoSamples() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("AutomaticSegmentation", 0);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_NoNumberOfDimensions() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("Samples", 1);
    cdb.Write("AutomaticSegmentation", 0);

    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_False_BadNumberOfDimensions() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("NumberOfDimensions", 3);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);


    MDSWriterNode test;
    return !test.Initialise(cdb);
}

bool MDSWriterNodeTest::TestInitialise_Type_UInt16() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_WU);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Int16() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "int16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_W);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_UInt32() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint32");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_LU);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Int32() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "int32");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_L);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_UInt64() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint64");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_QU);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Int64() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "int64");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_Q);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Float32() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "float32");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_FLOAT);
    return ok;
}

bool MDSWriterNodeTest::TestInitialise_Type_Float64() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "float64");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNodeType() == DTYPE_DOUBLE);
    return ok;
}

bool MDSWriterNodeTest::TestExecute() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }

    uint16 signal;
    if (ok) {
        ok = test.AllocateTreeNode(tree);
    }
    if (ok) {
        test.SetSignalMemory(&signal);
    }
    if (ok) {
        ok = test.Execute();
    }

    if (ok) {
        const uint64 maxTimeoutSeconds = 2;
        uint64 maxTimeout = HighResolutionTimer::Counter() + maxTimeoutSeconds * HighResolutionTimer::Frequency();
        while ((sigUInt16F->getNumSegments() != 1) && (ok)) {
            Sleep::MSec(10);
            ok = (HighResolutionTimer::Counter() < maxTimeout);
        }
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;

}

bool MDSWriterNodeTest::TestExecute_Decimated() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("DecimatedNodeName", "SIGUINT16D");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    cdb.Write("MinMaxResampleFactor", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }

    uint16 signal;
    if (ok) {
        ok = test.AllocateTreeNode(tree);
    }
    if (ok) {
        test.SetSignalMemory(&signal);
    }
    if (ok) {
        ok = test.Execute();
    }

    if (ok) {
        const uint64 maxTimeoutSeconds = 2;
        uint64 maxTimeout = HighResolutionTimer::Counter() + maxTimeoutSeconds * HighResolutionTimer::Frequency();
        while ((sigUInt16F->getNumSegments() != 1) && (ok)) {
            Sleep::MSec(10);
            ok = (HighResolutionTimer::Counter() < maxTimeout);
        }
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;

}

bool MDSWriterNodeTest::TestExecute_False_NoTree() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }

    uint16 signal;
    if (ok) {
        test.SetSignalMemory(&signal);
    }
    if (ok) {
        ok = !test.Execute();
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;

}

bool MDSWriterNodeTest::TestExecute_False_NoInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    bool ok = true;
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }

    uint16 signal;
    if (ok) {
        test.SetSignalMemory(&signal);
    }
    if (ok) {
        ok = !test.Execute();
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;
}

bool MDSWriterNodeTest::TestExecute_False_NoSignalSet() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("DecimatedNodeName", "SIGUINT16D");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    cdb.Write("MinMaxResampleFactor", 1);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }

    if (ok) {
        ok = test.AllocateTreeNode(tree);
    }
    if (ok) {
        ok = !test.Execute();
    }

    if (tree != NULL) {
        delete tree;
    }

    return ok;
}

bool MDSWriterNodeTest::TestAllocateTreeNode() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    if (ok) {
        ok = test.AllocateTreeNode(tree);
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;

}

bool MDSWriterNodeTest::TestAllocateTreeNode_BadNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "ABC");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    if (ok) {
        ok = !test.AllocateTreeNode(tree);
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;

}

bool MDSWriterNodeTest::TestFlush() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 2);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }

    uint16 signal;
    if (ok) {
        ok = test.AllocateTreeNode(tree);
    }
    if (ok) {
        test.SetSignalMemory(&signal);
    }
    if (ok) {
        ok = test.Execute();
    }
    if (ok) {
        ok = test.Flush();
    }

    if (ok) {
        const uint64 maxTimeoutSeconds = 2;
        uint64 maxTimeout = HighResolutionTimer::Counter() + maxTimeoutSeconds * HighResolutionTimer::Frequency();
        while ((sigUInt16F->getNumSegments() != 1) && (ok)) {
            Sleep::MSec(10);
            ok = (HighResolutionTimer::Counter() < maxTimeout);
        }
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;

}

bool MDSWriterNodeTest::TestSetSignalMemory() {
    return TestExecute();
}

bool MDSWriterNodeTest::TestSetTimeSignalMemory() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 1);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 1);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    MDSWriterNode test;
    bool ok = test.Initialise(cdb);
    StreamString treeName = "mds_m2test";

    MDSplus::Tree *tree = NULL;
    int32 lastPulseNumber = -1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }

    MDSplus::TreeNode *sigUInt16F;
    if (ok) {
        try {
            sigUInt16F = tree->getNode("SIGUINT16F");
            sigUInt16F->deleteData();

        }
        catch (MDSplus::MdsException &exc) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed opening node");
            ok = false;
        }
    }

    uint16 signal;
    uint32 timeSignal;

    if (ok) {
        ok = test.AllocateTreeNode(tree);
    }
    if (ok) {
        test.SetSignalMemory(&signal);
    }
    if (ok) {
        test.SetTimeSignalMemory(&timeSignal);
    }
    if (ok) {
        ok = test.Execute();
    }

    if (ok) {
        const uint64 maxTimeoutSeconds = 2;
        uint64 maxTimeout = HighResolutionTimer::Counter() + maxTimeoutSeconds * HighResolutionTimer::Frequency();
        while ((sigUInt16F->getNumSegments() != 1) && (ok)) {
            Sleep::MSec(10);
            ok = (HighResolutionTimer::Counter() < maxTimeout);
        }
    }

    if (tree != NULL) {
        delete tree;
    }
    return ok;
}

bool MDSWriterNodeTest::TestIsDecimatedMinMax() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);

    MDSWriterNode test;
    test.Initialise(cdb);
    bool ok = (!test.IsDecimatedMinMax());
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("MinMaxResampleFactor", 5);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 0);
    cdb.Write("NumberOfElements", 1);
    test.Initialise(cdb);
    ok &= (test.IsDecimatedMinMax());
    return ok;
}

bool MDSWriterNodeTest::TestGetDecimatedNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetDecimatedNodeName() == "BBB");
}

bool MDSWriterNodeTest::TestGetExecutePeriodMicroSecond() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("NodeName", "AAA");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    MDSWriterNode test;
    return (test.GetExecutePeriodMicroSecond() == (100 * (5e-7 * 1e6)));
}

bool MDSWriterNodeTest::TestGetMakeSegmentAfterNWrites() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetMakeSegmentAfterNWrites() == 4);
}

bool MDSWriterNodeTest::TestGetMinMaxResampleFactor() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetMinMaxResampleFactor() == 5);
}

bool MDSWriterNodeTest::TestGetNodeName() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetNodeName() == "AAA");
}

bool MDSWriterNodeTest::TestGetNodeType() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetNodeType() == DTYPE_WU);
}

bool MDSWriterNodeTest::TestGetNumberOfElements() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 5e-7);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetNumberOfElements() == 100);
}

bool MDSWriterNodeTest::TestGetPeriod() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 2);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetPeriod() == 2);
}

bool MDSWriterNodeTest::TestGetPhaseShift() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 2);
    cdb.Write("SamplePhase", 20);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetPhaseShift() == 20);
}

bool MDSWriterNodeTest::TestGetStart() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "SIGUINT16F");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 2);
    cdb.Write("SamplePhase", 20);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    test.Initialise(cdb);

    MDSplus::Tree *tree = NULL;
    StreamString treeName = "mds_m2test";
    int32 lastPulseNumber = -1;
    bool ok = true;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), lastPulseNumber);
        lastPulseNumber = tree->getCurrent(treeName.Buffer());
    }
    catch (MDSplus::MdsException &exc) {
        ok = false;
    }
    delete tree;
    tree = NULL_PTR(MDSplus::Tree *);
    int32 currentPulseNumber = lastPulseNumber + 1;
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1);
        tree->setCurrent(treeName.Buffer(), currentPulseNumber);
        tree->createPulse(currentPulseNumber);
    }
    catch (MDSplus::MdsException &exc) {
        delete tree;
        tree = NULL_PTR(MDSplus::Tree *);
        ok = false;
    }
    if (ok) {
        ok = test.AllocateTreeNode(tree);
    }
    if (ok) {
        ok = (test.GetStart() == 40);
    }
    if (ok) {
        delete tree;
    }

    return ok;
}

bool MDSWriterNodeTest::TestGetTypeMultiplier() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 2);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetTypeMultiplier() == sizeof(uint16));
}

bool MDSWriterNodeTest::TestIsUseTimeVector() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 2);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    bool ok = !test.IsUseTimeVector();
    uint32 signalTime;
    test.SetTimeSignalMemory(&signalTime);
    ok &= test.IsUseTimeVector();
    return ok;
}

bool MDSWriterNodeTest::TestGetNumberOfExecuteCalls() {
    using namespace MARTe;
    ConfigurationDatabase cdb;

    MDSWriterNode test;
    cdb.Write("NodeName", "AAA");
    cdb.Write("DecimatedNodeName", "BBB");
    cdb.Write("Type", "uint16");
    cdb.Write("NumberOfElements", 100);
    cdb.Write("Period", 2);
    cdb.Write("MakeSegmentAfterNWrites", 4);
    cdb.Write("AutomaticSegmentation", 0);
    cdb.Write("Samples", 1);
    cdb.Write("NumberOfDimensions", 1);
    cdb.Write("MinMaxResampleFactor", 5);
    test.Initialise(cdb);
    return (test.GetNumberOfExecuteCalls() == 0);
}
