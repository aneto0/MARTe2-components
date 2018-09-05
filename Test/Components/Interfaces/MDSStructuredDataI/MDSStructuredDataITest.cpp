/**
 * @file MDSStructuredDataITest.cpp
 * @brief Source file for class MDSStructuredDataITest
 * @date 04/09/2018
 * @author Andre Neto
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
 * the class MDSStructuredDataITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "MDSStructuredDataI.h"
#include "MDSStructuredDataITest.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool MDSStructuredDataITest::TestConstructor() {
    using namespace MARTe;
    MDSplus::Tree *tree = NULL;
    StreamString treeName = "mds_sdi";
    try {
        tree = new MDSplus::Tree(treeName.Buffer(), -1, "EDIT");
    }
    catch (const MDSplus::MdsException &exc) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Warning, "Error opening tree %s. Error: %s", treeName.Buffer(), exc.what());
    }
    MDSStructuredDataI mdsStructuredDataI;
    mdsStructuredDataI.SetTree(tree);
    mdsStructuredDataI.SetEditMode(true);

     mdsStructuredDataI.CreateAbsolute("A");
     mdsStructuredDataI.CreateAbsolute("A.B");
     mdsStructuredDataI.CreateAbsolute("A.B.C");
     float32 f = 9;
     mdsStructuredDataI.Write("AFloat", f);

     mdsStructuredDataI.CreateAbsolute("A.B.D");
     StreamString b = "HELLO!";
     mdsStructuredDataI.Write("AString", b.Buffer());
    /*
    const char* config = "Test = {"
            "    Class = RealTimeApplication"
            "    Functions = {"
            "        Class = ReferenceContainer"
            "        GAM1 = {"
            "            Class = MDSWriterGAMTriggerTestHelper"
            "            Signal =   {8 1 2 3 4 5 6 7 }"
            "            Trigger =  {0 1 0 1 0 1 0 1 }"
            "            OutputSignals = {"
            "                Trigger = {"
            "                    Type = uint8"
            "                    DataSource = Drv1"
            "                }"
            "                Time = {"
            "                    Type = float64"
            "                    DataSource = Drv1"
            "                }"
            "                SignalUInt16F = {"
            "                    Type = uint16"
            "                    DataSource = Drv1"
            "                }"
            "                SignalInt16F = {"
            "                    Type = int16"
            "                    DataSource = Drv1"
            "                }"
            "            }"
            "        }"
            "    }"
            "    Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        Drv1 = {"
            "            Class = MDSWriter"
            "            NumberOfBuffers = 10"
            "            CPUMask = 15"
            "            StackSize = 10000000"
            "            TreeName = \"mds_m2test\""
            "            StoreOnTrigger = 1"
            "            EventName = \"updatejScope\""
            "            TimeRefresh = 5"
            "            NumberOfPreTriggers = 1"
            "            NumberOfPostTriggers = 1"
            "            Signals = {"
            "                Trigger = {"
            "                    Type = uint8"
            "                }"
            "                Time = {"
            "                    Type = float64"
            "                    TimeSignal = 1"
            "                }"
            "                SignalUInt16F = {"
            "                    NodeName = \"SIGUINT16F\""
            "                    Period = 2"
            "                    MakeSegmentAfterNWrites = 4"
            "                    DecimatedNodeName = \"SIGUINT16D\""
            "                    MinMaxResampleFactor = 4"
            "                    AutomaticSegmentation = 0"
            "                }"
            "                SignalInt16F = {"
            "                    NodeName = \"SIGINT16F\""
            "                    Period = 2"
            "                    MakeSegmentAfterNWrites = 4"
            "                    DecimatedNodeName = \"SIGINT16D\""
            "                    MinMaxResampleFactor = 4"
            "                    AutomaticSegmentation = 0"
            "                }"
            "            }"
            "        }"
            "    }"
            "    States = {"
            "        Class = ReferenceContainer"
            "        State1 = {"
            "            Class = RealTimeState"
            "            Threads = {"
            "                Class = ReferenceContainer"
            "                Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAM1}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    Scheduler = {"
            "        Class = MDSWriterSchedulerTestHelper"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }
    cdb.MoveToRoot();
    cdb.Copy(mdsStructuredDataI);*/

    tree->write();
    delete tree;
    return true;
}

