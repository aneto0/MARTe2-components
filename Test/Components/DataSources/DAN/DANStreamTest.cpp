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
#ifdef CCS_LT_60
#include <tcn.h>
#else
#include <common/TimeTools.h> // ccs::HelperTools::GetCurrentTime, etc.
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "DANSource.h"
#include "DANStream.h"
#include "DANStreamTest.h"
#include "DirectoryScanner.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Checks the PutData function against a list of know types.
 */
template<typename typeToCheck> static bool TestPutDataT(bool useAbsoluteTime = false, bool useRelativeTime = false, MARTe::uint32 numberOfRuns = 1u) {
    using namespace MARTe;

    //To discover the type
    typeToCheck tDiscover;
    AnyType typeDiscover(tDiscover);

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
    const uint32 numberOfWrites = 3u;
    const uint32 numberOfChannels = 3u;
    const float64 samplingFrequency = 1e3;
    const uint64 periodNanos = 1e9 / samplingFrequency;
    //The +1 is to cater for the extra write that is needed below in order to handle a bug in DAN
    uint64 externalTimeAbsoluteArr[numberOfWrites + 1];
    uint32 externalTimeRelativeArr[numberOfWrites + 1];

    TypeDescriptor typeDesc=typeDiscover.GetTypeDescriptor();
    DANStream ds(TypeDescriptor::GetTypeNameFromTypeDescriptor(typeDesc), "DANStreamTest", 4, samplingFrequency, numberOfSamples, true);
    ds.AddSignal(0u);
    ds.AddToStructure(0u, "Signal0", typeDesc, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(2u);
    ds.AddToStructure(2u, "Signal2", typeDesc, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(5u);
    ds.AddToStructure(5u, "Signal5", typeDesc, 1u, 0u, "Unknown", "Unknown");
    ds.Finalise();
    typeToCheck *signal0Ptr = NULL;
    ok &= ds.GetSignalMemoryBuffer(0u, reinterpret_cast<void *&>(signal0Ptr));
    ok &= (signal0Ptr != NULL);
    typeToCheck *signal2Ptr = NULL;
    ok &= ds.GetSignalMemoryBuffer(2u, reinterpret_cast<void *&>(signal2Ptr));
    ok &= (signal2Ptr != NULL);
    typeToCheck *signal5Ptr = NULL;
    ok &= ds.GetSignalMemoryBuffer(5u, reinterpret_cast<void *&>(signal5Ptr));
    ok &= (signal5Ptr != NULL);

    uint32 r;
    for (r = 0; (r < numberOfRuns) && (ok); r++) {
        //Delete any previous DAN test files
        DirectoryScanner hd5FilesToTest;
        hd5FilesToTest.Scan("/tmp/data/", "*DANStreamTest*");
        uint32 i = 0u;
        while (hd5FilesToTest.ListPeek(i)) {
            Directory *toDelete = static_cast<Directory *>(hd5FilesToTest.ListPeek(i));
            toDelete->Delete();
            i++;
        }

        ds.Reset();

        ok = ds.OpenStream();

#ifdef CCS_LT_60
        hpn_timestamp_t hpnTimeStamp;
        if (ok) {
            ok = (tcn_get_time(&hpnTimeStamp) == TCN_SUCCESS);
        }
#else
        uint64 hpnTimeStamp = ccs::HelperTools::GetCurrentTime();
#endif
        if (ok) {
            uint32 j;
            if (!useAbsoluteTime) {
                    ds.SetAbsoluteStartTime(hpnTimeStamp);
                    if (useRelativeTime) {
                        for (j = 0; (j < (numberOfWrites + 1)) && (ok); j++) {
                            externalTimeRelativeArr[j] = (j * periodNanos * numberOfSamples / 1000); //Relative time is in us
                        }
                    }
            }
            else {
                for (j = 0; (j < (numberOfWrites + 1)) && (ok); j++) {
                    externalTimeAbsoluteArr[j] = (hpnTimeStamp + j * periodNanos * numberOfSamples);
                }
            }
        }

        uint32 j;
        uint32 c;
        uint32 k = 1;
        typeToCheck *signalPtrs[numberOfChannels] = { signal0Ptr, signal2Ptr, signal5Ptr };
        for (k = 0; (k < numberOfWrites) && (ok); k++) {
            if (useAbsoluteTime) {
                ds.SetAbsoluteTimeSignal(&externalTimeAbsoluteArr[k]);
            }
            else if (useRelativeTime) {
                ds.SetRelativeTimeSignal(&externalTimeRelativeArr[k]);
            }
            for (j = 0; j < numberOfSamples; j++) {
                for (c = 0; c < numberOfChannels; c++) {
                    signalPtrs[c][j] = (k * numberOfSamples + c + j);
                }
            }
            if (ok) {
                ok = ds.PutData();
            }
        }
        //This second PutData is required to overcome a bug in the danStreamReader
        if (ok) {
            if (useAbsoluteTime) {
                ds.SetAbsoluteTimeSignal(&externalTimeAbsoluteArr[k]);
            }
            else if (useRelativeTime) {
                ds.SetRelativeTimeSignal(&externalTimeRelativeArr[k]);
            }
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
        if (ok) {
            ok = found;
        }
        if (ok) {
            ok = ds.CloseStream();
        }

        DanStreamReaderCpp danStreamReader;
        const uint32 expectedNSamples = 20u;
        if (ok) {
            found = false;
            timeoutCounter = 0;
            while ((ok) && (!found) && (timeoutCounter < maxTimeoutCounter)) {
                StreamString hd5FileName = static_cast<Directory *>(hd5FilesToTest.ListPeek(0u))->GetName();
                ok = (danStreamReader.openFile(hd5FileName.Buffer()) >= 0);
#ifdef NO_HDF5_SET_CHANNEL
                Sleep::Sec(2.0);
                found = true;
#else
                found = (danStreamReader.getCurSamples() == expectedNSamples);
#endif
                if (!found) {
                    danStreamReader.closeFile();
                    Sleep::Sec(0.1);
                }
                timeoutCounter++;
            }
            if (ok) {
                ok = found;
            }
        }

        const char8 *channelNames[] = { "Signal1", "Signal2", "Signal3" };
        for (c = 0; (c < numberOfChannels) && (ok); c++) {
            DanDataHolder *pDataChannel = NULL;
            if (ok) {
#ifdef NO_HDF5_SET_CHANNEL
		danStreamReader.openDataPath(channelNames[c]);
#else
                danStreamReader.setChannel(channelNames[c]);
#endif
                DataInterval interval = danStreamReader.getIntervalWhole();
                pDataChannel = danStreamReader.getRawValuesNative(&interval, -1);
                ok = (pDataChannel != NULL);
                if (ok) {
                    ok = (interval.getTimeFrom() == (long) hpnTimeStamp);
                }
                if (ok) {
                    ok = (interval.getTimeTo() == (long) (hpnTimeStamp + (expectedNSamples - 1) * periodNanos));
                }
            }
            if (ok) {
                const typeToCheck *channelDataStored = NULL;
                if (typeDiscover.GetTypeDescriptor() == UnsignedInteger16Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asUInt16();
                }
                else if (typeDiscover.GetTypeDescriptor() == SignedInteger16Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asInt16();
                }
                else if (typeDiscover.GetTypeDescriptor() == UnsignedInteger32Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asUInt32();
                }
                else if (typeDiscover.GetTypeDescriptor() == SignedInteger32Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asInt32();
                }
                else if (typeDiscover.GetTypeDescriptor() == UnsignedInteger64Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asUInt64();
                }
                else if (typeDiscover.GetTypeDescriptor() == SignedInteger64Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asInt64();
                }
                else if (typeDiscover.GetTypeDescriptor() == Float32Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asFloat();
                }
                else if (typeDiscover.GetTypeDescriptor() == Float64Bit) {
                    channelDataStored = (typeToCheck *) pDataChannel->asDouble();
                }
                for (k = 0; k < numberOfWrites; k++) {
                    for (j = 0; (j < numberOfSamples) && (ok); j++) {
                        ok &= ((typeToCheck) channelDataStored[(k * numberOfSamples) + j] == (typeToCheck) (k * numberOfSamples + c + j));
                        if (!ok) {
                            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "[%d, %d] %e != %e", k, j, (typeToCheck )channelDataStored[(k * numberOfSamples) + j], (typeToCheck )(k * numberOfSamples + c + j));
                        }
                    }
                }

                delete pDataChannel;
                danStreamReader.closeStream();
            }
        }
        if (ok) {
            danStreamReader.closeFile();
        }
    }
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool DANStreamTest::TestConstructor() {
    using namespace MARTe;
    DANStream ds("float32", "DANStreamTest", 4, 2e6, 8, true);
    StreamString typeOut;
    ds.GetType(typeOut);
    bool ok = ( typeOut == "float32");
    ok &= (ds.GetDANBufferMultiplier() == 4);
    ok &= (ds.GetPeriodNanos() == 500);
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

    DANStream ds("float32", "DANStreamTest", 4, 2e6, 8, true);
    float32 *signalPtr = NULL;

    ds.AddSignal(2u);
    ds.AddToStructure(2u, "Signal2", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(0u);
    ds.AddToStructure(0u, "Signal0", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(1u);
    ds.AddToStructure(1u, "Signal1", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ok &= !ds.GetSignalMemoryBuffer(0u, reinterpret_cast<void *&>(signalPtr));

    ds.Finalise();
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

bool DANStreamTest::TestAddToStructure() {
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

    DANStream ds("float32", "DANStreamTest", 4, 2e6, 8, true);
    float32 *signalPtr = NULL;

    ds.AddSignal(0u);
    ok &= ds.AddToStructure(0u, "Field0", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ok &= ds.AddToStructure(1u, "Field1", UnsignedInteger32Bit, 1u, 0u, "Unknown", "Unknown");
    ok &= ds.AddToStructure(2u, "Field2", SignedInteger8Bit, 1u, 0u, "Unknown", "Unknown");

    ds.AddSignal(3u);
    ok &= ds.AddToStructure(3u, "Field0", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ok &= !ds.AddToStructure(4u, "Field1", Float64Bit, 1u, 0u, "Unknown", "Unknown");

    return ok;
}


bool DANStreamTest::TestFinalise() {
    return TestAddSignal();
}

bool DANStreamTest::TestPutData_False() {
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
    danSource.Initialise(cdb);

    DANStream ds("float32", "DANStreamTest", 4, 2e6, 1000, true);
    ds.AddSignal(0u);
    ds.AddToStructure(0u, "Signal0", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(2u);
    ds.AddToStructure(2u, "Signal2", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(5u);
    ds.AddToStructure(5u, "Signal5", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    return !ds.PutData();
}

bool DANStreamTest::TestPutData_UInt16() {
    return TestPutDataT<MARTe::uint16>();
}

bool DANStreamTest::TestPutData_Int16() {
    return TestPutDataT<MARTe::int16>();
}

bool DANStreamTest::TestPutData_UInt32() {
    return TestPutDataT<MARTe::uint32>();
}

bool DANStreamTest::TestPutData_Int32() {
    return TestPutDataT<MARTe::int32>();
}

bool DANStreamTest::TestPutData_UInt64() {
    return TestPutDataT<MARTe::uint64>();
}

bool DANStreamTest::TestPutData_Int64() {
    return TestPutDataT<MARTe::int64>();
}

bool DANStreamTest::TestPutData_Float32() {
    return TestPutDataT<MARTe::float32>();
}

bool DANStreamTest::TestPutData_Float64() {
    return TestPutDataT<MARTe::float64>();
}

bool DANStreamTest::TestOpenStream() {
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

    DANStream ds("float32", "DANStreamTest", 4, 2e6, 8, true);
    ds.AddSignal(2u);
    ds.AddToStructure(2u, "Signal2", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(0u);
    ds.AddToStructure(0u, "Signal0", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(1u);
    ds.AddToStructure(1u, "Signal1", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.Finalise();

    ok &= ds.OpenStream();
    ds.CloseStream();

    return ok;
}

bool DANStreamTest::TestOpenStream_NoFinalise() {
    using namespace MARTe;
    DANStream ds("float32", "DANStreamTest", 4, 2e6, 8, true);

    return !ds.OpenStream();
}

bool DANStreamTest::TestCloseStream() {
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

    DANStream ds("float32", "DANStreamTest", 4, 2e6, 8, true);
    ds.AddSignal(2u);
    ds.AddToStructure(2u, "Signal2", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(0u);
    ds.AddToStructure(0u, "Signal0", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.AddSignal(1u);
    ds.AddToStructure(1u, "Signal1", Float32Bit, 1u, 0u, "Unknown", "Unknown");
    ds.Finalise();

    ds.OpenStream();
    ok &= ds.CloseStream();

    return ok;
}

bool DANStreamTest::TestGetSignalMemoryBuffer() {
    return TestAddSignal();
}

bool DANStreamTest::TestReset() {
    return TestPutDataT<MARTe::uint16>(false, false, 2);
}

bool DANStreamTest::TestSetAbsoluteTimeSignal() {
    bool ok = TestPutDataT<MARTe::int16>(true);
    if (ok) {
        ok = TestPutDataT<MARTe::int64>(true);
    }
    return ok;
}

bool DANStreamTest::TestSetRelativeTimeSignal() {
    bool ok = TestPutDataT<MARTe::int16>(false, true);
    if (ok) {
        ok = TestPutDataT<MARTe::int64>(false, true);
    }
    return ok;
}

bool DANStreamTest::TestSetAbsoluteStartTime() {
    return TestPutDataT<MARTe::float32>(false, false);
}
