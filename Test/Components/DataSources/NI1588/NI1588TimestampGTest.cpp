/**
 * @file NI1588TimestampGTest.cpp
 * @brief Source file for class NI1588TimestampGTest
 * @date 15/05/2018
 * @author Giuseppe Ferrò
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
 * the class NI1588TimestampGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

#include "NI1588TimestampTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(NI1588TimestampGTest,TestConstructor) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(NI1588TimestampGTest,TestInitialise) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(NI1588TimestampGTest,TestInitialise_DefaultPollTimeout) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestInitialise_DefaultPollTimeout());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_AllSignals) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_AllSignals());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI0_Type) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI0_Type());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI1_Type) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI1_Type());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI2_Type) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidTcnTimestamp_PFI2_Type());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidEventType_PFI0_Type) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidEventType_PFI0_Type());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidEventType_PFI1_Type) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidEventType_PFI1_Type());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidEventType_PFI2_Type) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidEventType_PFI2_Type());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidTcnTimestamp_Nelements) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidTcnTimestamp_Nelements());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidEventType_Nelements) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidEventType_Nelements());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_InvalidSignal) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_InvalidSignal());
}

TEST(NI1588TimestampGTest,TestSetConfiguredDatabase_False_CaptureEvent) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_CaptureEvent());
}

TEST(NI1588TimestampGTest,TestDriverRead) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestDriverRead());
}

TEST(NI1588TimestampGTest,TestDriverRead_PFI0) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestDriverRead_PFI(500000000, 6000, 0));
}

TEST(NI1588TimestampGTest,TestDriverRead_PFI1) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestDriverRead_PFI(500000000, 6000, 1));
}

TEST(NI1588TimestampGTest,TestDriverRead_PFI2) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestDriverRead_PFI(500000000, 6000, 2));
}

TEST(NI1588TimestampGTest,TestDriverRead_MoreSamples) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestDriverRead_MoreSamples(250000000, 4000, 0));
}

TEST(NI1588TimestampGTest,TestDriverRead_TimeoutExpired) {
    NI1588TimestampTest test;


	static const char8 * const config =
			""
					"$Application1 = {"
					"    Class = RealTimeApplication"
					"    +Functions = {"
					"        Class = ReferenceContainer"
					"        +GAMA = {"
					"            Class = NI1588TimestampTestGAM1"
					"             InputSignals = {"
					"               InternalTimeStamp = {"
					"                   DataSource = Drv1"
					"                   Type = uint64"
					"                   Samples = 1"
					"               }"
					"               ErrorCheck = {"
					"                   DataSource = Drv1"
					"                   Type = uint32"
					"                   Samples = 5"
					"               }"
					"               TerminalPFI0 = {"
					"                   DataSource = Drv1"
					"                   Type = uint64"
					"                   Samples = 1"
					"                   Frequency = 0"
					"               }"
					"            }"
					"        }"
					"    }"
					"    +Data = {"
					"        Class = ReferenceContainer"
					"        +Drv1 = {"
					"            Class = NI1588TimestampTestDS"
					"            NumberOfBuffers = 100"
					"            CpuMask = 1"
					"            ReceiverThreadPriority = 31"
					"            PollMsecTimeout = 10"
					"            NiSyncDevicePath = NiSyncDevice"
                                        "            Signals = {"
                                        "               TerminalPFI0 = {"
                                        "                   Type = uint64"
                                        "                   CaptureEvent = NISYNC_EDGE_RISING"
                                        "               }"
                                        "            }"
					"        }"
					"        +Timings = {"
					"            Class = TimingDataSource"
					"        }"
					"    }"
					"    +States = {"
					"        Class = ReferenceContainer"
					"        +State1 = {"
					"            Class = RealTimeState"
					"            +Threads = {"
					"                Class = ReferenceContainer"
					"                +Thread1 = {"
					"                    Class = RealTimeThread"
					"                    Functions = {GAMA}"
					"                }"
					"            }"
					"        }"
					"    }"
					"    +Scheduler = {"
					"        Class = GAMScheduler"
					"        TimingDataSource = Timings"
					"    }"
					"}";

    ASSERT_TRUE(test.TestDriverRead_TimeoutExpired(config, 500000000, 6000, 0, 5));
}

TEST(NI1588TimestampGTest,TestDriverRead_NoSync) {
    NI1588TimestampTest test;
    ASSERT_TRUE(test.TestDriverRead_NoSync(500000, 2500));
}

TEST(NI1588TimestampGTest,TestDriverRead_Pattern_Terminal0_2Hz_rising) {
	const char8 * const config =
			""
					"$Application1 = {"
					"    Class = RealTimeApplication"
					"    +Functions = {"
					"        Class = ReferenceContainer"
					"        +GAMA = {"
					"            Class = NI1588TimestampTestGAM1"
					"             InputSignals = {"
					"               TerminalPFI0 = {"
					"                   DataSource = Drv1"
					"                   Type = uint64"
					"                   Samples = 1"
					"                   Frequency = 0"
					"               }"
					"            }"
					"        }"
					"    }"
					"    +Data = {"
					"        Class = ReferenceContainer"
					"        +Drv1 = {"
					"            Class = NI1588TimestampTestDS"
					"            NumberOfBuffers = 10"
					"            CpuMask = 1"
					"            ReceiverThreadPriority = 31"
					"            PollMsecTimeout = -1"
					"            NiSyncDevicePath = NiSyncDevice"
                                        "            Signals = {"
                                        "               TerminalPFI0 = {"
                                        "                   Type = uint64"
                                        "                   CaptureEvent = NISYNC_EDGE_RISING"
                                        "               }"
                                        "            }"
					"        }"
					"        +Timings = {"
					"            Class = TimingDataSource"
					"        }"
					"    }"
					"    +States = {"
					"        Class = ReferenceContainer"
					"        +State1 = {"
					"            Class = RealTimeState"
					"            +Threads = {"
					"                Class = ReferenceContainer"
					"                +Thread1 = {"
					"                    Class = RealTimeThread"
					"                    Functions = {GAMA}"
					"                }"
					"            }"
					"        }"
					"    }"
					"    +Scheduler = {"
					"        Class = GAMScheduler"
					"        TimingDataSource = Timings"
					"    }"
					"}";


    NI1588TimestampTest test;
    uint64 expectedDeltas[]={500000000, 500000000, 500000000, 500000000, 500000000};

    ASSERT_TRUE(test.TestDriverRead_Pattern(config, expectedDeltas, 5000, 5));
}



TEST(NI1588TimestampGTest,TestDriverRead_Pattern_Terminal0_2Hz_falling) {
	const char8 * const config =
			""
					"$Application1 = {"
					"    Class = RealTimeApplication"
					"    +Functions = {"
					"        Class = ReferenceContainer"
					"        +GAMA = {"
					"            Class = NI1588TimestampTestGAM1"
					"             InputSignals = {"
					"               TerminalPFI0 = {"
					"                   DataSource = Drv1"
					"                   Type = uint64"
					"                   Samples = 1"
					"                   Frequency = 0"
					"               }"
					"            }"
					"        }"
					"    }"
					"    +Data = {"
					"        Class = ReferenceContainer"
					"        +Drv1 = {"
					"            Class = NI1588TimestampTestDS"
					"            NumberOfBuffers = 10"
					"            CpuMask = 1"
					"            ReceiverThreadPriority = 31"
					"            PollMsecTimeout = -1"
					"            NiSyncDevicePath = NiSyncDevice"
                                        "            Signals = {"
                                        "               TerminalPFI0 = {"
                                        "                   Type = uint64"
                                        "                   CaptureEvent = NISYNC_EDGE_FALLING"
                                        "               }"
                                        "            }"
					"        }"
					"        +Timings = {"
					"            Class = TimingDataSource"
					"        }"
					"    }"
					"    +States = {"
					"        Class = ReferenceContainer"
					"        +State1 = {"
					"            Class = RealTimeState"
					"            +Threads = {"
					"                Class = ReferenceContainer"
					"                +Thread1 = {"
					"                    Class = RealTimeThread"
					"                    Functions = {GAMA}"
					"                }"
					"            }"
					"        }"
					"    }"
					"    +Scheduler = {"
					"        Class = GAMScheduler"
					"        TimingDataSource = Timings"
					"    }"
					"}";


    NI1588TimestampTest test;
    uint64 expectedDeltas[]={500000000, 500000000, 500000000, 500000000, 500000000};

    ASSERT_TRUE(test.TestDriverRead_Pattern(config, expectedDeltas, 5000, 5));
}




TEST(NI1588TimestampGTest,TestDriverRead_Pattern_Terminal0_2Hz_any) {
	const char8 * const config =
			""
					"$Application1 = {"
					"    Class = RealTimeApplication"
					"    +Functions = {"
					"        Class = ReferenceContainer"
					"        +GAMA = {"
					"            Class = NI1588TimestampTestGAM1"
					"             InputSignals = {"
					"               TerminalPFI0 = {"
					"                   DataSource = Drv1"
					"                   Type = uint64"
					"                   Samples = 1"
					"                   Frequency = 0"
					"               }"
					"            }"
					"        }"
					"    }"
					"    +Data = {"
					"        Class = ReferenceContainer"
					"        +Drv1 = {"
					"            Class = NI1588TimestampTestDS"
					"            NumberOfBuffers = 10"
					"            CpuMask = 1"
					"            ReceiverThreadPriority = 31"
					"            PollMsecTimeout = -1"
					"            NiSyncDevicePath = NiSyncDevice"
                                        "            Signals = {"
                                        "               TerminalPFI0 = {"
                                        "                   Type = uint64"
                                        "                   CaptureEvent = NISYNC_EDGE_ANY"
                                        "               }"
                                        "            }"
					"        }"
					"        +Timings = {"
					"            Class = TimingDataSource"
					"        }"
					"    }"
					"    +States = {"
					"        Class = ReferenceContainer"
					"        +State1 = {"
					"            Class = RealTimeState"
					"            +Threads = {"
					"                Class = ReferenceContainer"
					"                +Thread1 = {"
					"                    Class = RealTimeThread"
					"                    Functions = {GAMA}"
					"                }"
					"            }"
					"        }"
					"    }"
					"    +Scheduler = {"
					"        Class = GAMScheduler"
					"        TimingDataSource = Timings"
					"    }"
					"}";


    NI1588TimestampTest test;
    uint64 expectedDeltas[]={250000000, 250000000, 250000000, 250000000, 250000000};

    ASSERT_TRUE(test.TestDriverRead_Pattern(config, expectedDeltas, 5000, 5));
}
