/**
 * @file DANStreamTest.cpp
 * @brief Source file for class DANStreamTest
 * @date 11/04/2017
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
 * the class DANStreamTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "dan/dan_DataCore.h"
#include "dan/dan_Source.h"
#include "dan/reader/dan_stream_reader_cpp.h"
#include "tcn.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "DANSource.h"
#include "DANStream.h"
#include "DANStreamTest.h"
#include "DirectoryScanner.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool DANStreamTest::TestConstructor() {
    using namespace MARTe;
    DANStream ds(Float32Bit, "DANStreamTest", 4, 2e6, 8);
    bool ok = (ds.GetType() == Float32Bit);
    ok &= (ds.GetDANBufferMultiplier() == 4);
    ok &= (ds.GetSamplingFrequency() == 2e6);
    ok &= (ds.GetNumberOfSamples() == 8);

    return true;
}

bool DANStreamTest::TestGetType() {
    return TestConstructor();
}

bool DANStreamTest::TestGetSamplingFrequency() {
    return TestConstructor();
}

bool DANStreamTest::TestGetDANBufferMultiplier() {
    return TestConstructor();
}

bool DANStreamTest::TestGetNumberOfSamples() {
    return TestConstructor();
}

bool DANStreamTest::TestAddSignal() {
    using namespace MARTe;
    //This is required in order to create the dan_initLibrary
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 1);
    cdb.Write("StackSize", 1048576);
    cdb.Write("DanBufferMultiplier", 4);
    cdb.Write("StoreOnTrigger", 0);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    DANSource danSource;
    bool ok = danSource.Initialise(cdb);

    DANStream ds(Float32Bit, "DANStreamTest", 4, 2e6, 8);
    ds.AddSignal(2u);
    ds.AddSignal(0u);
    ds.AddSignal(1u);
    ds.Finalise();
    float32 *signalPtr = NULL;
    ok &= ds.GetSignalMemoryBuffer(0u, reinterpret_cast<void *&>(signalPtr));
    ok &= (signalPtr != NULL);
    signalPtr = NULL;
    ok &= ds.GetSignalMemoryBuffer(1u, reinterpret_cast<void *&>(signalPtr));
    ok &= (signalPtr != NULL);
    signalPtr = NULL;
    ok &= ds.GetSignalMemoryBuffer(2u, reinterpret_cast<void *&>(signalPtr));
    ok &= (signalPtr != NULL);
    signalPtr = NULL;
    ok &= !ds.GetSignalMemoryBuffer(3u, reinterpret_cast<void *&>(signalPtr));

    return ok;
}

bool DANStreamTest::TestFinalise() {
    return TestAddSignal();
}

bool DANStreamTest::TestPutData() {
    using namespace MARTe;

    //Delete any previous DAN test files
    DirectoryScanner hd5FilesToTest;
    hd5FilesToTest.Scan("/tmp/data/", "*DANStreamTest*");
    uint32 i = 0u;
    while (hd5FilesToTest.ListPeek(i)) {
        Directory *toDelete = static_cast<Directory *>(hd5FilesToTest.ListPeek(i));
        toDelete->Delete();
        i++;
    }

    //This is required in order to create the dan_initLibrary
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 1);
    cdb.Write("StackSize", 1048576);
    cdb.Write("DanBufferMultiplier", 4);
    cdb.Write("StoreOnTrigger", 0);
    cdb.CreateAbsolute("Signals");
    cdb.MoveToRoot();
    DANSource danSource;
    bool ok = danSource.Initialise(cdb);

    const uint32 numberOfSamples = 5;

    DANStream ds(Float32Bit, "DANStreamTest", 4, 1e3, numberOfSamples);
    ds.AddSignal(0u);
    ds.AddSignal(2u);
    ds.AddSignal(5u);
    ds.Finalise();
    float32 *signal0Ptr = NULL;
    ok &= ds.GetSignalMemoryBuffer(0u, reinterpret_cast<void *&>(signal0Ptr));
    ok &= (signal0Ptr != NULL);
    float32 *signal2Ptr = NULL;
    ok &= ds.GetSignalMemoryBuffer(2u, reinterpret_cast<void *&>(signal2Ptr));
    ok &= (signal2Ptr != NULL);
    float32 *signal5Ptr = NULL;
    ok &= ds.GetSignalMemoryBuffer(5u, reinterpret_cast<void *&>(signal5Ptr));
    ok &= (signal5Ptr != NULL);

    TimeStamp now;
    HighResolutionTimer::GetTimeStamp(now);
    ds.OpenStream();
    if (ok) {
        hpn_timestamp_t hpnTimeStamp;
        ok = (tcn_get_time(&hpnTimeStamp) == TCN_SUCCESS);
        ds.SetAbsoluteStartTime(hpnTimeStamp);
    }

    uint32 j;
    uint32 c;
    uint32 k = 1;
    const uint32 numberOfWrites = 3u;
    const uint32 numberOfChannels = 3u;
    float32 *signalPtrs[numberOfChannels] = { signal0Ptr, signal2Ptr, signal5Ptr };
    for (k = 0; k < numberOfWrites; k++) {
        for (j = 0; j < numberOfSamples; j++) {
            for (c = 0; c < numberOfChannels; c++) {
                signalPtrs[c][j] = (k * numberOfSamples + i + c + j);
            }
        }
        if (ok) {
            ok = ds.PutData();
        }
    }
    //This second PutData is required to overcome a bug in the danStreamReader
    if (ok) {
        ok = ds.PutData();
    }

    //Wait for the file to be created...
    bool found = false;
    uint32 timeoutCounter = 0u;
    const uint32 maxTimeoutCounter = 30u;
    while ((!found) && (timeoutCounter < maxTimeoutCounter)) {
        hd5FilesToTest.Scan("/tmp/data/", "*DANStreamTest*");
        found = (static_cast<Directory *>(hd5FilesToTest.ListPeek(0u)) != NULL);
        if (!found) {
            Sleep::Sec(0.1);
        }
        timeoutCounter++;
    }
    ok = found;
    ds.CloseStream();

    DanStreamReaderCpp danStreamReader;
    if (ok) {
        StreamString hd5FileName = static_cast<Directory *>(hd5FilesToTest.ListPeek(0u))->GetName();
        ok = (danStreamReader.openFile(hd5FileName.Buffer()) == 0);
    }

    const char8 *channelNames[] = { "Signal1", "Signal2", "Signal3" };
    for (c = 0; c < numberOfChannels; c++) {
        DanDataHolder *pDataChannel = NULL;
        if (ok) {
            DataInterval interval = danStreamReader.getIntervalWhole();
            danStreamReader.setChannel(channelNames[c]);
            pDataChannel = danStreamReader.getRawValuesNative(&interval, -1);
            ok = (pDataChannel != NULL);
        }
        if (ok) {
            const float32 *channelDataStored = pDataChannel->asFloat();
            for (k = 0; k < numberOfWrites; k++) {
                for (j = 0; (j < numberOfSamples) && ok; j++) {
                    ok &= (channelDataStored[(k * numberOfSamples) + j] == (k * numberOfSamples + i + c + j));
                }
            }

            delete pDataChannel;
        }
    }
    if (ok) {
        danStreamReader.closeFile();
    }

    return ok;
}

bool DANStreamTest::TestOpenStream() {
    return true;
}

bool DANStreamTest::TestCloseStream() {
    return true;
}

bool DANStreamTest::TestGetSignalMemoryBuffer() {
    return true;
}

bool DANStreamTest::TestReset() {
    return true;
}

bool DANStreamTest::TestSetAbsoluteTimeSignal() {
    return true;
}

bool DANStreamTest::TestSetRelativeTimeSignal() {
    return true;
}

bool DANStreamTest::TestSetAbsoluteStartTime() {
    return true;
}
