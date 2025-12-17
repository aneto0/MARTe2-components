/**
 * @file DANStreamByReferenceTest.cpp
 * @brief Additional source file for class DANStreamTest
 * @date 18/11/2025
 * @author Kevin Meyer
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

// DAN
#include "DANAPI.h"

// System headers
#include <sys/mman.h> // For mmap, munmap, shm_open, shm_unlink, PROT_* and MAP_*
#include <fcntl.h>    // For O_CREAT, O_RDWR
#include <unistd.h>   // For close
#include <string.h>   // For memset
#include <errno.h>    // For errno

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "DANStream.h"
#include "DANStreamByReferenceTest.h"
#include "DirectoryScanner.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

// Helper data structures and data
typedef struct {
    PUBLISHER_DATA_BLOCK_HEADER std_header;
    uint64_t lost_count;
} __attribute__((packed)) DAN_DATA_BLOCK_HEADER_TYPE_LDAN;
DAN_DATA_BLOCK_HEADER_TYPE_LDAN headerBlock;

// Helper methods
void flushData(void); //Delete any previous DAN test files

#define SHM_FS "/dev/shm"

static u_int32_t get_file_size(const char *file_name) {
    struct stat buf;
    if (stat(file_name, &buf) != 0)
        return (0);
    return (uint32_t) buf.st_size;
}

static MARTe::char8* allocateSharedMemoryBuffer(const char * name,
                                                int64_t size) {
    using namespace MARTe;
    int newf = 1;
    char shmfile[PATH_MAX];
    sprintf(shmfile, SHM_FS"/%s", name);
    int64_t currsize = get_file_size(shmfile);
    if (currsize > 0) {
        newf = 0;
    }

    int fd = shm_open(name, O_CREAT | O_RDWR, S_IRWXU | S_IRWXO | S_IRWXG);
    if (fd < 0) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to open shared memory '%s'.", shmfile);
        return 0;
    }

    if ((size > 0) && newf) {
        int res = ftruncate(fd, size + 64);
        if (res < 0) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to resize shared memory '%s' to %u.", shmfile, uint32(size + 64));
            shm_unlink(name);
            return 0;
        }

    }
    else {
        if (currsize != (size + 64)) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Existing shared memory '%s' is wrong size (actual %u, expected %u). Delete it to be recreated.",
                                shmfile, uint32(currsize), uint32(size + 64));
            shm_unlink(name);
            return 0;
        }
    }

    MARTe::char8 *buf = (MARTe::char8*) mmap(0, size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, 0);
    if ((void*) buf == (void*) -1) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to memory map shared memory '%s'.", shmfile);
        shm_unlink(name);
        return 0;
    }
    return buf;
}

static void freeSharedMemoryBuffer(const char * name,
                                   MARTe::char8 * buf,
                                   int64_t size) {
    munmap(buf, size);
    shm_unlink(name);
}

static MARTe::char8* setUp(MARTe::DANStream & ds,
                           bool & result,
                           const char * shmName,
                           int64_t shmSize) {
    using namespace MARTe;
    result = true;

    flushData();

    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Calling dan_initLibrary");
    if (!DANAPI::InitLibrary()) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to initialise DAN API.");
        result = false;
        return NULL_PTR(char8 *);
    }
    MARTe::char8 *buffer = allocateSharedMemoryBuffer(shmName, shmSize);
    if (buffer == NULL_PTR(char8 *)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to allocate shared memory buffer.");
        result = false;
        return NULL_PTR(char8 *);
    }

    // initialize standard header fields
    headerBlock.std_header.dim1 = 1;
    headerBlock.std_header.dim2 = 1;
    headerBlock.std_header.operational_mode = 0;
    headerBlock.std_header.sampling_rate = 0;
    headerBlock.std_header.reserve = 0;

    return buffer;
}

static void tearDown(bool deleteAll,
                     const char * streamName,
                     MARTe::char8 * buffer,
                     int64_t shmSize) {
    using namespace MARTe;

    freeSharedMemoryBuffer(streamName, buffer, shmSize);

    DANAPI::CloseLibrary();

    // Teardown should also remove h5 files from /tmp/data
    if (deleteAll) {
        flushData();
    }
}

//Delete any previous DAN test files
void flushData(void) {
    using namespace MARTe;
    DirectoryScanner hd5FilesToDelete;
    hd5FilesToDelete.Scan("/tmp/data/", "*DANStreamByReferenceTest*");
    uint32 i = 0u;
    while (hd5FilesToDelete.ListPeek(i)) {
        Directory *toDelete = static_cast<Directory*>(hd5FilesToDelete.ListPeek(i));
        toDelete->Delete();
        i++;
    }
}

enum TimingMode {
    Internal,
    ExternalAbsolute,
    ExternalRelative
};

MARTe::int64 absStart = 1234567890;
MARTe::uint64 absSignal = 9876543210;
MARTe::uint32 relSignal = 5555;

static bool streamCreatorAndWriter(TimingMode mode,
                                   MARTe::float32 scalingFactor,
                                   MARTe::uint32 actualNumberOfSamples,
                                   bool flush) {
    using namespace MARTe;
    bool ok = true;
    const float64 samplingFrequencyIn = 2e6;
    uint32 numberOfSamplesIn = 1024;

    const char streamName[] = "DANStreamByReferenceTest";
    const char shmName[] = "topic_mem";
    int64_t shmSize = 1024 * sizeof(float32);

    // Wait a little, to let the DAN writer settle down.
    Sleep::Sec(1.5);

    // Create stream 
    DANStream ds(Float32Bit, "DANSource0", samplingFrequencyIn, numberOfSamplesIn);

    MARTe::char8 *buffer = setUp(ds, ok, shmName, shmSize);
    if (buffer == NULL_PTR(char8 *)) {
        return false;
    }

    if (mode == Internal) {
        ds.SetAbsoluteStartTime(absStart);
    }
    else if (mode == ExternalAbsolute) {
        ds.SetAbsoluteStartTime(absStart); // still needed for base
        ds.SetAbsoluteTimeSignal(&absSignal);
    }
    else if (mode == ExternalRelative) {
        ds.SetAbsoluteStartTime(absStart);
        ds.SetRelativeTimeSignal(&relSignal);
    }

    // Write stream data
    StreamString shmemName;
    (void) shmemName.Printf("%s/%s", SHM_FS, shmName);
    (void) shmemName.Seek(0LLU);
    if (!ds.InitializePublishSource(streamName, shmemName.Buffer(), shmSize)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to initialise DAN API.");
        tearDown(true, streamName, buffer, shmSize);
        return false;
    }

    if (ok && buffer != NULL_PTR(char8 *)) {
        ok = ds.OpenStream();
    }

    headerBlock.std_header.sampling_rate = samplingFrequencyIn;
    headerBlock.lost_count = 123;
    // Write a pattern of float data to the shared memory buffer
    float32 *floatBuffer = reinterpret_cast<float32*>(buffer);
    for (uint32 i = 0; i < actualNumberOfSamples; i++) {
        floatBuffer[i] = static_cast<float32>(i * scalingFactor);
    }

    char8 *header = reinterpret_cast<char8*>(&headerBlock);
    uint64 data_offset = 0;
    ok &= ds.PutData(header, actualNumberOfSamples, data_offset, flush);

    // Close Stream
    ds.CloseStream();

    // Teardown
    if (buffer != NULL_PTR(char8 *)) {
        tearDown(false, streamName, buffer, shmSize);
    }

    return ok;
}

// Helper to check the DAN file pattern and data

static bool CheckDANFilePattern(TimingMode mode,
                                MARTe::uint32 actualNumberOfSamples,
                                MARTe::float32 patternMultiplier) {
    using namespace MARTe;
    DirectoryScanner hd5FilesToTest;
    bool found = false;
    uint32 timeoutCounter = 0u;
    const uint32 maxTimeoutCounter = 30u;
    while ((!found) && (timeoutCounter < maxTimeoutCounter)) {
        hd5FilesToTest.Scan("/tmp/data/", "*DANStreamByReferenceTest*");
        found = (static_cast<Directory*>(hd5FilesToTest.ListPeek(0u)) != NULL);
        if (!found) {
            Sleep::Sec(0.1);
        }
        timeoutCounter++;
    }
    if (!found) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to find test files in /tmp/data/ (timeout).");
        return false;
    }

    DanStreamReaderCpp danStreamReader;
    const uint32 expectedNSamples = actualNumberOfSamples;
    found = false;
    timeoutCounter = 0;
    bool ok = true;
    while ((ok) && (!found) && (timeoutCounter < maxTimeoutCounter)) {
        StreamString hd5FileName = static_cast<Directory*>(hd5FilesToTest.ListPeek(0u))->GetName();
        ok = (danStreamReader.openFile(hd5FileName.Buffer()) >= 0);
#ifdef NO_HDF5_SET_CHANNEL
        Sleep::Sec(1.0);
        found = true;
#else
        uint64 samplesRead = danStreamReader.getCurSamples();
        found = (samplesRead == expectedNSamples);
#endif
        if (!found) {
            danStreamReader.closeFile();
            Sleep::Sec(0.1);
        }
        timeoutCounter++;
    }
    if (!ok || !found) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to read sample data from /tmp/data/.");
        return false;
    }

    const char8 *channelName = "Channel0";
#ifdef NO_HDF5_SET_CHANNEL
    ok = (danStreamReader.openDataPath(channelName) == 0);
#else
    danStreamReader.setChannel(channelName);
#endif

    if (ok) {
        DataInterval interval = danStreamReader.getIntervalWhole();
        const float64 samplingFrequencyIn = 2e6;

        // Do timestamp checking
        const int64 expectedDurationNanos = (1e9 * static_cast<float64>(actualNumberOfSamples - 1)) / samplingFrequencyIn;
        int64_t diff = interval.getTimeTo() - interval.getTimeFrom();
        ok &= (diff == expectedDurationNanos);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning, "Timestamp duration check failed!");
        }

        ok &= (interval.getPosTo() == actualNumberOfSamples - 1);
        if (mode == Internal) {
            ok &= (interval.getTimeFrom() == absStart);
            ok &= (interval.getTimeTo() == absStart + expectedDurationNanos);
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Internal time check failed!");
            }
        }
        else if (mode == ExternalAbsolute) {
            ok &= (interval.getTimeFrom() == static_cast<int64_t>(absSignal));
            ok &= (interval.getTimeTo() == static_cast<int64_t>(absSignal + expectedDurationNanos));
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "ExternalAbsolute time check failed!");
            }
        }
        else if (mode == ExternalRelative) {
            // Note: the relative time is provided in micro-seconds, so multiply by 1e3 to convert ro nanoseconds.
            ok &= (interval.getTimeFrom() == absStart + relSignal * 1e3);
            ok &= (interval.getTimeTo() == absStart + relSignal * 1e3 + expectedDurationNanos);
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "ExternalRelative time check failed!");
            }
        }

        DanDataHolder *pDataChannel = danStreamReader.getRawValuesNative(&interval, -1);
        if (pDataChannel == NULL) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to retrieve data from stream.");
            ok = false;
        }
        else {
            const int64 length = pDataChannel->length();
            ok &= (length == (int64) actualNumberOfSamples);
            const float32 *channelDataStored = pDataChannel->asFloat();
            for (uint32 i = 0; (i < actualNumberOfSamples) && ok; i++) {
                ok &= (channelDataStored[i] == static_cast<float32>(i * patternMultiplier));
            }
            delete pDataChannel;
            if (!ok) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to validate data from file in /tmp/data/.");
            }
        }
#ifdef NO_HDF5_SET_CHANNEL
        danStreamReader.closeStream();
#endif
    }
    danStreamReader.closeFile();
    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool DANStreamByReferenceTest::TestConstructor() {
    using namespace MARTe;
    DANStream ds(Float32Bit, "DANSource0", 2e6, 1);
    StreamString typeOut;
    ds.GetType(typeOut);
    bool ok = (typeOut == "float32");
    ok &= (ds.GetDANBufferMultiplier() == 1);
    ok &= (ds.GetPeriodNanos() == 500);
    ok &= (ds.GetNumberOfSamples() == 1);
    return ok;
}

bool DANStreamByReferenceTest::TestInitializePublishSource() {
    using namespace MARTe;
    bool ok = true;
    int64_t shmSize = 1024 * sizeof(float32);
    const float64 samplingFrequencyIn = 2e6;
    const char streamName[] = "DANStreamByReferenceTest";
    const char shmName[] = "topic_mem";
    uint32 numberOfSamplesIn = 1024;

    DANStream ds(Float32Bit, "DANSource0", samplingFrequencyIn, numberOfSamplesIn);

    MARTe::char8 *buffer = setUp(ds, ok, shmName, shmSize);
    if (buffer == NULL_PTR(char8 *)) {
        return false;
    }

    StreamString shmemName;
    (void) shmemName.Printf("%s/%s", SHM_FS, shmName);
    (void) shmemName.Seek(0LLU);
    // Function under test
    if (!ds.InitializePublishSource(streamName, shmemName.Buffer(), shmSize)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to initialise DAN API.");
        tearDown(true, streamName, buffer, shmSize);
        return false;
    }
    // Also test changing stream name
    const char altStreamName[] = "DANStreamTest";
    // Function under test
    if (!ds.InitializePublishSource(altStreamName, shmemName.Buffer(), shmSize)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to initialise DAN API.");
        tearDown(true, altStreamName, buffer, shmSize);
        return false;
    }
    if (buffer != NULL_PTR(char8 *)) {
        tearDown(true, altStreamName, buffer, shmSize);
    }

    return ok;
}

bool DANStreamByReferenceTest::TestOpenStream() {
    using namespace MARTe;
    bool ok = true;
    int64_t shmSize = 1024 * sizeof(float32);
    const float64 samplingFrequencyIn = 2e6;
    const char streamName[] = "DANStreamByReferenceTest";
    const char shmName[] = "topic_mem";
    uint32 numberOfSamplesIn = 1024;

    DANStream ds(Float32Bit, "DANSource0", samplingFrequencyIn, numberOfSamplesIn);

    MARTe::char8 *buffer = setUp(ds, ok, shmName, shmSize);
    if (buffer == NULL_PTR(char8 *)) {
        return false;
    }

    StreamString shmemName;
    (void) shmemName.Printf("%s/%s", SHM_FS, shmName);
    (void) shmemName.Seek(0LLU);
    if (!ds.InitializePublishSource(streamName, shmemName.Buffer(), shmSize)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to initialise DAN API.");
        tearDown(true, streamName, buffer, shmSize);
        return false;
    }

    if (ok && buffer != NULL_PTR(char8 *)) {
        // Function under test
        ok = ds.OpenStream();
        ds.CloseStream();
    }

    if (buffer != NULL_PTR(char8 *)) {
        tearDown(true, streamName, buffer, shmSize);
    }

    return ok;
}

bool DANStreamByReferenceTest::TestCloseStream() {
    using namespace MARTe;
    bool ok = true;
    int64_t shmSize = 1024 * sizeof(float32);
    const float64 samplingFrequencyIn = 2e6;
    const char streamName[] = "DANStreamByReferenceTest";
    const char shmName[] = "topic_mem";
    uint32 numberOfSamplesIn = 1024;

    DANStream ds(Float32Bit, "DANSource0", samplingFrequencyIn, numberOfSamplesIn);

    MARTe::char8 *buffer = setUp(ds, ok, shmName, shmSize);
    if (buffer == NULL_PTR(char8 *)) {
        return false;
    }

    StreamString shmemName;
    (void) shmemName.Printf("%s/%s", SHM_FS, shmName);
    (void) shmemName.Seek(0LLU);
    if (!ds.InitializePublishSource(streamName, shmemName.Buffer(), shmSize)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to initialise DAN API.");
        tearDown(true, streamName, buffer, shmSize);
        return false;
    }

    if (ok && buffer != NULL_PTR(char8 *)) {
        ok = ds.OpenStream();
        // Function under test
        ok &= ds.CloseStream();
    }

    if (buffer != NULL_PTR(char8 *)) {
        tearDown(true, streamName, buffer, shmSize);
    }

    return ok;
}

bool DANStreamByReferenceTest::TestPutData_Float32() {
    using namespace MARTe;
    bool ok = true;
    float scalingFactor = 1.5f;
    uint32 actualNumberOfSamples = 1000;

    ok &= streamCreatorAndWriter(Internal, scalingFactor, actualNumberOfSamples, false);
    // Let the DANWriter settle down after close
    Sleep::Sec(1.5);
    // Use common helper for file and pattern check
    ok &= CheckDANFilePattern(Internal, actualNumberOfSamples, scalingFactor);

    // Manually delete tmp file
    flushData();

    return ok;
}

bool DANStreamByReferenceTest::TestPutData_Float32Flush() {
    using namespace MARTe;
    bool ok = true;
    float scalingFactor = 2.0f;
    uint32 actualNumberOfSamples = 1001;

    ok &= streamCreatorAndWriter(ExternalAbsolute, scalingFactor, actualNumberOfSamples, true);
    // Let the DANWriter settle down after flush
    Sleep::Sec(1.5);

    // Check that two data files exist
    DirectoryScanner hd5FilesToTest;
    bool found = false;
    uint32 timeoutCounter = 0u;
    const uint32 maxTimeoutCounter = 30u;
    while ((!found) && (timeoutCounter < maxTimeoutCounter)) {
        hd5FilesToTest.Scan("/tmp/data/", "*DANStreamByReferenceTest*");
        found = (static_cast<Directory*>(hd5FilesToTest.ListPeek(0u)) != NULL);
        if (!found) {
            Sleep::Sec(0.1);
        }
        timeoutCounter++;
    }

    ok &= (hd5FilesToTest.ListSize() == 2);

    // Manually delete tmp file
    flushData();

    return ok;
}

bool DANStreamByReferenceTest::TestSetAbsoluteTimeSignal() {
    using namespace MARTe;
    bool ok = true;
    float scalingFactor = 2.0f;
    uint32 actualNumberOfSamples = 1001;

    ok &= streamCreatorAndWriter(ExternalAbsolute, scalingFactor, actualNumberOfSamples, false);
    ok &= CheckDANFilePattern(ExternalAbsolute, actualNumberOfSamples, scalingFactor);

    // Manually delete tmp file
    flushData();

    return ok;
}

bool DANStreamByReferenceTest::TestSetRelativeTimeSignal() {
    using namespace MARTe;
    bool ok = true;
    float scalingFactor = 4.2f;
    uint32 actualNumberOfSamples = 1002;

    ok &= streamCreatorAndWriter(ExternalRelative, scalingFactor, actualNumberOfSamples, false);
    ok &= CheckDANFilePattern(ExternalRelative, actualNumberOfSamples, scalingFactor);

    // Manually delete tmp file
    flushData();

    return ok;
}

bool DANStreamByReferenceTest::TestPutWithNoOpenOK() {
    using namespace MARTe;
    bool ok = true;
    const float64 samplingFrequencyIn = 2e6;
    uint32 numberOfSamplesIn = 1024;
    char8 *header = reinterpret_cast<char8*>(&headerBlock);
    uint64 data_offset = 0;
    bool flush = false;

    DANStream ds(Float32Bit, "DANSource0", samplingFrequencyIn, numberOfSamplesIn);
    // Function under test
    ok &= ds.PutData(header, numberOfSamplesIn, data_offset, flush);

    // Manually delete tmp file
    flushData();

    return ok;
}

bool DANStreamByReferenceTest::TestDoubleCloseOK() {
    using namespace MARTe;
    bool ok = true;
    int64_t shmSize = 1024 * sizeof(float32);
    const float64 samplingFrequencyIn = 2e6;
    const char streamName[] = "DANStreamByReferenceTest";
    const char shmName[] = "topic_mem";
    uint32 numberOfSamplesIn = 1024;

    // Wait a little, to let the DAN writer settle down.
    // Sleep::Sec(1.5);

    DANStream ds(Float32Bit, "DANSource0", samplingFrequencyIn, numberOfSamplesIn);

    MARTe::char8 *buffer = setUp(ds, ok, shmName, shmSize);
    if (buffer == NULL_PTR(char8 *)) {
        return false;
    }

    StreamString shmemName;
    (void) shmemName.Printf("%s/%s", SHM_FS, shmName);
    (void) shmemName.Seek(0LLU);
    if (!ds.InitializePublishSource(streamName, shmemName.Buffer(), shmSize)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to initialise DAN API.");
        tearDown(true, streamName, buffer, shmSize);
        return false;
    }

    if (ok && buffer != NULL_PTR(char8 *)) {
        ok = ds.OpenStream();
        ds.CloseStream();
        // Function under test
        ok &= ds.CloseStream();

        // Let the DANWriter settle down after flush
        Sleep::Sec(1.5);
    }

    if (buffer != NULL_PTR(char8 *)) {
        tearDown(true, streamName, buffer, shmSize);
    }

    return ok;
}
