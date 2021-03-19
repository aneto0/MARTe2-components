/**
 * @file FileReaderTest.cpp
 * @brief Source file for class FileReaderTest
 * @date 21/08/2017
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
 * the class FileReaderTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Directory.h"
#include "FileReader.h"
#include "FileReaderTest.h"
#include "GAM.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Helper class that reacts to messages received from the FileReader class
 */
class FileReaderTestHelper: public MARTe::Object, public MARTe::MessageI {
public:CLASS_REGISTER_DECLARATION()
    FileReaderTestHelper() :
            Object(),
            MessageI() {
        using namespace MARTe;
        openFailFunctionCalled = false;
        openOKFunctionCalled = false;
        closeFunctionCalled = false;
        ReferenceT<RegisteredMethodsMessageFilter> filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
        if (!ret.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to install message filters");
        }
    }

    MARTe::ErrorManagement::ErrorType HandleOpenFail() {
        openFailFunctionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleOpenOK() {
        openOKFunctionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleClose() {
        closeFunctionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType HandleRuntimeError() {
        runtimeErrorFunctionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    bool openOKFunctionCalled;
    bool openFailFunctionCalled;
    bool closeFunctionCalled;
    bool runtimeErrorFunctionCalled;
};
CLASS_REGISTER(FileReaderTestHelper, "1.0")
CLASS_METHOD_REGISTER(FileReaderTestHelper, HandleOpenOK)
CLASS_METHOD_REGISTER(FileReaderTestHelper, HandleOpenFail)
CLASS_METHOD_REGISTER(FileReaderTestHelper, HandleClose)
CLASS_METHOD_REGISTER(FileReaderTestHelper, HandleRuntimeError)

/**
 * @brief GAM which generates a given signal trigger, time and signal pattern which is then sinked to the FileReader
 */
class FileReaderGAMTriggerTestHelper: public MARTe::GAM {
public:CLASS_REGISTER_DECLARATION()
    FileReaderGAMTriggerTestHelper() {
        char8Signal = NULL;
        stringSignal = NULL;
        uint8Signal = NULL;
        int8Signal = NULL;
        uint16Signal = NULL;
        int16Signal = NULL;
        uint32Signal = NULL;
        int32Signal = NULL;
        uint64Signal = NULL;
        int64Signal = NULL;
        float32Signal = NULL;
        float64Signal = NULL;
        numberOfElements = NULL;
    }

    virtual ~FileReaderGAMTriggerTestHelper() {
        if (numberOfElements != NULL) {
            delete[] numberOfElements;
        }
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        numberOfElements = new uint32[GetNumberOfInputSignals()];
        uint32 n;
        for (n = 0; n < GetNumberOfInputSignals(); n++) {
            uint32 nElements;
            GetSignalNumberOfElements(InputSignals, 0, nElements);
            numberOfElements[n] = nElements;
            if (GetSignalType(InputSignals, n) == Character8Bit) {
                char8Signal = reinterpret_cast<char8*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == CharString) {
                stringSignal = reinterpret_cast<char8*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger8Bit) {
                uint8Signal = reinterpret_cast<uint8*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger8Bit) {
                int8Signal = reinterpret_cast<int8*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger16Bit) {
                int16Signal = reinterpret_cast<int16*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger64Bit) {
                int64Signal = reinterpret_cast<int64*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == Float32Bit) {
                float32Signal = reinterpret_cast<float32*>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == Float64Bit) {
                float64Signal = reinterpret_cast<float64*>(GetInputSignalMemory(n));
            }

        }
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
    }

    MARTe::char8 *char8Signal;
    MARTe::char8 *stringSignal;
    MARTe::uint8 *uint8Signal;
    MARTe::int8 *int8Signal;
    MARTe::uint16 *uint16Signal;
    MARTe::int16 *int16Signal;
    MARTe::uint32 *uint32Signal;
    MARTe::int32 *int32Signal;
    MARTe::uint64 *uint64Signal;
    MARTe::int64 *int64Signal;
    MARTe::float32 *float32Signal;
    MARTe::float64 *float64Signal;
    MARTe::uint32 *numberOfElements;
};
CLASS_REGISTER(FileReaderGAMTriggerTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the FileReader and the GAMs
 */
class FileReaderSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

    FileReaderSchedulerTestHelper() :
            MARTe::GAMSchedulerI() {
        scheduledStates = NULL;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    bool ExecuteThreadCycle(MARTe::uint32 threadId) {
        using namespace MARTe;
        ReferenceT<RealTimeApplication> realTimeAppT = realTimeApp;
        return ExecuteSingleCycle(scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].executables,
                                  scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler(MARTe::Reference realTimeApp) {

        bool ret = GAMSchedulerI::ConfigureScheduler(realTimeApp);
        if (ret) {
            scheduledStates = GetSchedulableStates();
        }
        return ret;
    }

    virtual void CustomPrepareNextState() {
    }

private:

    MARTe::ScheduledState *const*scheduledStates;
};

CLASS_REGISTER(FileReaderSchedulerTestHelper, "1.0")

static bool TestIntegratedInApplication(const MARTe::char8 *const config,
                                        bool destroy) {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StreamString err;
    StandardParser parser(configStream, cdb, &err);

    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "%s", err.Buffer());
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }

    if (destroy) {
        god->Purge();
    }
    return ok;
}

#define FRT_ADD_SIGNAL_TO_VERIFY(idx,type,nElements,value)\
        if (nElements[idx] > 0) {\
            type ## Signal = new type[nElements[idx]];\
            uint32 zz;\
            for(zz=0;zz<nElements[idx];zz++){\
                type ## Signal[zz]=static_cast<type>(value);\
            }\
        }\
        else {\
            type ## Signal = NULL;\
        }\
        signalPtrs[idx] = type ## Signal;

#define FRT_FREE_SIGNAL(idx,type)\
        if (type ## Signal != NULL) {\
            delete [] type ## Signal;\
        }

#define FRT_VERIFY_SIGNAL(idx,type)\
        if (signalToVerify[s]->type ## Signal != NULL) {\
            for(z=0; (z<signalToVerifyNumberOfElements[idx]) && (ok); z++) {\
                ok = (signalToVerify[s]->type ## Signal[z] == gam->type ## Signal[z]);\
            }\
        }

class FRTSignalToVerify {
public:
    FRTSignalToVerify(MARTe::uint32 *nElements,
                      MARTe::uint32 value) {
        using namespace MARTe;
        signalPtrs = new void*[10];
        FRT_ADD_SIGNAL_TO_VERIFY(0, uint8, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(1, int8, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(2, uint16, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(3, int16, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(4, uint32, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(5, int32, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(6, uint64, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(7, int64, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(8, float32, nElements, value)
        FRT_ADD_SIGNAL_TO_VERIFY(9, float64, nElements, value)

    }

    ~FRTSignalToVerify() {
        using namespace MARTe;
        delete[] signalPtrs;
        FRT_FREE_SIGNAL(0, uint8)
        FRT_FREE_SIGNAL(1, int8)
        FRT_FREE_SIGNAL(2, uint16)
        FRT_FREE_SIGNAL(3, int16)
        FRT_FREE_SIGNAL(4, uint32)
        FRT_FREE_SIGNAL(5, int32)
        FRT_FREE_SIGNAL(6, uint64)
        FRT_FREE_SIGNAL(7, int64)
        FRT_FREE_SIGNAL(8, float32)
        FRT_FREE_SIGNAL(9, float64)
    }

    void Printf(MARTe::File &f,
                MARTe::TypeDescriptor t,
                MARTe::uint32 e) {
        using namespace MARTe;
        if (t == UnsignedInteger8Bit) {
            f.Printf("%u", uint8Signal[e]);
        }
        else if (t == SignedInteger8Bit) {
            f.Printf("%d", int8Signal[e]);
        }
        else if (t == UnsignedInteger16Bit) {
            f.Printf("%u", uint16Signal[e]);
        }
        else if (t == SignedInteger16Bit) {
            f.Printf("%d", int16Signal[e]);
        }
        else if (t == UnsignedInteger32Bit) {
            f.Printf("%u", uint32Signal[e]);
        }
        else if (t == SignedInteger32Bit) {
            f.Printf("%d", int32Signal[e]);
        }
        else if (t == UnsignedInteger64Bit) {
            f.Printf("%u", uint64Signal[e]);
        }
        else if (t == SignedInteger64Bit) {
            f.Printf("%d", int64Signal[e]);
        }
        else if (t == Float32Bit) {
            f.Printf("%f", float32Signal[e]);
        }
        else if (t == Float64Bit) {
            f.Printf("%f", float64Signal[e]);
        }
    }

    MARTe::uint8 *uint8Signal;
    MARTe::int8 *int8Signal;
    MARTe::uint16 *uint16Signal;
    MARTe::int16 *int16Signal;
    MARTe::uint32 *uint32Signal;
    MARTe::int32 *int32Signal;
    MARTe::uint64 *uint64Signal;
    MARTe::int64 *int64Signal;
    MARTe::float32 *float32Signal;
    MARTe::float64 *float64Signal;
    void **signalPtrs;
};

static void GenerateFile(const MARTe::char8 *const filename,
                         const MARTe::char8 *const fileContent) {
    using namespace MARTe;
    File f;
    f.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    f.Printf("%s", fileContent);
    f.Flush();
    f.Close();
}

static void GenerateFile(const MARTe::char8 *const filename) {
    using namespace MARTe;
    const char8 *fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    GenerateFile(filename, fileContent);
}

static void GenerateFile2(const MARTe::char8 *const filename) {
    using namespace MARTe;
    const char8 *fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000\n";
    GenerateFile(filename, fileContent);
}

static void GenerateFile3(const MARTe::char8 *const filename) {
    using namespace MARTe;
    uint32 signalToVerifyNumberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    const uint32 N_OF_SIGNALS = 10;

    const char8 *signalNames[N_OF_SIGNALS] = { "SignalUInt8", "SignalInt8", "SignalUInt16", "SignalInt16", "SignalUInt32", "SignalInt32", "SignalUInt64",
            "SignalInt64", "SignalFloat32", "SignalFloat64WhichIsAlsoAVeryLon" };
    const TypeDescriptor signalTypes[N_OF_SIGNALS] = { UnsignedInteger8Bit, SignedInteger8Bit, UnsignedInteger16Bit, SignedInteger16Bit, UnsignedInteger32Bit,
            SignedInteger32Bit, UnsignedInteger64Bit, SignedInteger64Bit, Float32Bit, Float64Bit };

    uint32 signalBinarySize = 0u;
    uint32 signalToVerifyNumberOfSamples = 3u;
    uint32 i;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(signalToVerifyNumberOfElements, i + 1);
    }
    for (i = 0u; i < N_OF_SIGNALS; i++) {
        signalBinarySize += signalTypes[i].numberOfBits * signalToVerifyNumberOfElements[i] / 8u;
    }
    const uint32 SIGNAL_NAME_SIZE = 32;
    File f;
    bool ok = f.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);

    if (ok) {
        uint32 writeSize = sizeof(uint32);
        f.Write(reinterpret_cast<const char8*>(&N_OF_SIGNALS), writeSize);
        uint32 n;
        //Write the header
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            writeSize = sizeof(uint16);
            f.Write(reinterpret_cast<const char8*>(&signalTypes[n].all), writeSize);
            char8 signalName32[SIGNAL_NAME_SIZE];
            MemoryOperationsHelper::Set(&signalName32[0], '\0', SIGNAL_NAME_SIZE);
            MemoryOperationsHelper::Copy(&signalName32[0], signalNames[n], StringHelper::Length(signalNames[n]));
            writeSize = SIGNAL_NAME_SIZE;
            f.Write(reinterpret_cast<const char8*>(&signalName32[0]), writeSize);

            writeSize = sizeof(uint32);
            f.Write(reinterpret_cast<const char8*>(&signalToVerifyNumberOfElements[n]), writeSize);
        }
        uint32 s;
        for (s = 0; s < signalToVerifyNumberOfSamples - 1; s++) { //Last loop done differently
            for (n = 0u; n < N_OF_SIGNALS; n++) {
                writeSize = signalToVerifyNumberOfElements[n] * signalTypes[n].numberOfBits / 8;
                f.Write(reinterpret_cast<const char8*>(signals[s]->signalPtrs[n]), writeSize);
            }
        }
        for (n = 0u; n < N_OF_SIGNALS - 1; n++) { //Last element not written
            writeSize = signalToVerifyNumberOfElements[n] * signalTypes[n].numberOfBits / 8;
            f.Write(reinterpret_cast<const char8*>(signals[s]->signalPtrs[n]), writeSize);
        }
    }
    f.Flush();
    f.Close();
    if (signals != NULL_PTR(FRTSignalToVerify**)) {
        for (uint32 i = 0; i < signalToVerifyNumberOfSamples; i++) {
            if (signals[i] != NULL_PTR(FRTSignalToVerify*)) {
                delete signals[i];
                signals[i] = NULL_PTR(FRTSignalToVerify*);
            }
        }
        delete[] signals;
        signals = NULL_PTR(FRTSignalToVerify**);
    }
}

static void GenerateJumboFile(const MARTe::char8 *const filename) {
    using namespace MARTe;
    uint32 signalToVerifyNumberOfElements[] = { 1 };

    const uint32 N_OF_SIGNALS = 1;

    const char8 *signalNames[N_OF_SIGNALS] = { "SignalUInt8" };
    const TypeDescriptor signalTypes[N_OF_SIGNALS] = { UnsignedInteger8Bit };

    uint32 signalToVerifyNumberOfSamples = 3u;
    uint32 i;

    const uint32 SIGNAL_NAME_SIZE = 32;
    File f;
    bool ok = f.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);

    if (ok) {
        uint32 writeSize = sizeof(uint32);
        f.Write(reinterpret_cast<const char8*>(&N_OF_SIGNALS), writeSize);
        uint32 n;
//Write the header
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            writeSize = sizeof(uint16);
            f.Write(reinterpret_cast<const char8*>(&signalTypes[n].all), writeSize);
            char8 signalName32[SIGNAL_NAME_SIZE];
            MemoryOperationsHelper::Set(&signalName32[0], '\0', SIGNAL_NAME_SIZE);
            MemoryOperationsHelper::Copy(&signalName32[0], signalNames[n], StringHelper::Length(signalNames[n]));
            writeSize = SIGNAL_NAME_SIZE;
            f.Write(reinterpret_cast<const char8*>(&signalName32[0]), writeSize);

            writeSize = sizeof(uint32);
            f.Write(reinterpret_cast<const char8*>(&signalToVerifyNumberOfElements[n]), writeSize);
        }
    }
//write data
    uint32 size = 510000000;
    char8 *data = new char8[size]; //0.51 GB
    MemoryOperationsHelper::Set(data, 1, size);
    f.Write(data, size);
    f.Flush();
    f.Close();
    delete[] data;
}

static void DeleteTestFile(const MARTe::char8 *const filename) {
    using namespace MARTe;
    Directory toDelete(filename);
    toDelete.Delete();
}

static bool TestIntegratedExecution(const MARTe::char8 *const config,
                                    const MARTe::char8 *const filename,
                                    FRTSignalToVerify **signalToVerify,
                                    MARTe::uint32 *signalToVerifyNumberOfElements,
                                    MARTe::uint32 signalToVerifyNumberOfSamples,
                                    bool csv,
                                    MARTe::uint32 interpolationPeriod,
                                    const MARTe::char8 *const xAxisSignalName = "SignalUInt32",
                                    bool purge = true,
                                    bool ignoreFailure = false,
                                    const MARTe::char8 *const csvSeparator = ";",
                                    bool forceEOFRewind = false,
                                    bool forceEOFLast = false,
                                    bool forceEOFError = false) {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    cdb.MoveAbsolute("$Test.+Data.+Drv1");
    cdb.Delete("Filename");
    cdb.Write("Filename", filename);
    cdb.Delete("Interpolate");
    if (interpolationPeriod > 0) {
        cdb.Write("Interpolate", "yes");
        cdb.Write("InterpolationPeriod", interpolationPeriod);
        cdb.Delete("XAxisSignal");
        cdb.Write("XAxisSignal", xAxisSignalName);
    }
    else {
        cdb.Write("Interpolate", "no");
    }
    cdb.Delete("FileFormat");
    if (csv) {
        cdb.Write("FileFormat", "csv");
        cdb.Delete("CSVSeparator");
        cdb.Write("CSVSeparator", csvSeparator);
    }
    else {
        cdb.Write("FileFormat", "binary");
    }

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        cdb.MoveToRoot();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    ReferenceT<FileReaderSchedulerTestHelper> scheduler;
    ReferenceT<FileReaderGAMTriggerTestHelper> gam;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    ReferenceT<FileReader> fileReader;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }
    if (ok) {
        uint32 z;
        uint32 s;
        for (s = 0; (s < signalToVerifyNumberOfSamples) && (ok); s++) {
            ok = scheduler->ExecuteThreadCycle(0);
            if (ok) {
                FRT_VERIFY_SIGNAL(0, uint8);
                FRT_VERIFY_SIGNAL(1, int8);
                FRT_VERIFY_SIGNAL(2, uint16);
                FRT_VERIFY_SIGNAL(3, int16);
                FRT_VERIFY_SIGNAL(4, uint32);
                FRT_VERIFY_SIGNAL(5, int32);
                FRT_VERIFY_SIGNAL(6, uint64);
                FRT_VERIFY_SIGNAL(7, int64);
                FRT_VERIFY_SIGNAL(8, float32);
                FRT_VERIFY_SIGNAL(9, float64);
            }
            if (ignoreFailure) {
                ok = true;
            }
        }
        if (forceEOFError) {
            ok = !scheduler->ExecuteThreadCycle(0);
        }
        else if (forceEOFRewind) {
            for (s = 0; (s < signalToVerifyNumberOfSamples) && (ok); s++) {
                ok = scheduler->ExecuteThreadCycle(0);
                if (ok) {
                    FRT_VERIFY_SIGNAL(0, uint8);
                    FRT_VERIFY_SIGNAL(1, int8);
                    FRT_VERIFY_SIGNAL(2, uint16);
                    FRT_VERIFY_SIGNAL(3, int16);
                    FRT_VERIFY_SIGNAL(4, uint32);
                    FRT_VERIFY_SIGNAL(5, int32);
                    FRT_VERIFY_SIGNAL(6, uint64);
                    FRT_VERIFY_SIGNAL(7, int64);
                    FRT_VERIFY_SIGNAL(8, float32);
                    FRT_VERIFY_SIGNAL(9, float64);
                }
            }
        }
        else if (forceEOFLast) {
            uint32 k = 0;
            s = (signalToVerifyNumberOfSamples - 1);
            for (k = 0; (k < signalToVerifyNumberOfSamples) && (ok); k++) {
                ok = scheduler->ExecuteThreadCycle(0);
                if (ok) {
                    FRT_VERIFY_SIGNAL(0, uint8);
                    FRT_VERIFY_SIGNAL(1, int8);
                    FRT_VERIFY_SIGNAL(2, uint16);
                    FRT_VERIFY_SIGNAL(3, int16);
                    FRT_VERIFY_SIGNAL(4, uint32);
                    FRT_VERIFY_SIGNAL(5, int32);
                    FRT_VERIFY_SIGNAL(6, uint64);
                    FRT_VERIFY_SIGNAL(7, int64);
                    FRT_VERIFY_SIGNAL(8, float32);
                    FRT_VERIFY_SIGNAL(9, float64);
                }
            }
        }
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    if (purge) {
        god->Purge();
    }
    return ok;
}

static void GenerateCSVFile(const MARTe::char8 *const filename,
                            const MARTe::char8 *const csvSeparator,
                            FRTSignalToVerify **signalToVerify,
                            MARTe::uint32 *signalToVerifyNumberOfElements,
                            MARTe::uint32 signalToVerifyNumberOfSamples) {
    using namespace MARTe;
    const uint32 N_OF_SIGNALS = 10;
    const char8 *signalNames[N_OF_SIGNALS] = { "SignalUInt8", "SignalInt8", "SignalUInt16", "SignalInt16", "SignalUInt32", "SignalInt32", "SignalUInt64",
            "SignalInt64", "SignalFloat32", "SignalFloat64WhichIsAlsoAVeryLon" };
    const TypeDescriptor signalTypes[N_OF_SIGNALS] = { UnsignedInteger8Bit, SignedInteger8Bit, UnsignedInteger16Bit, SignedInteger16Bit, UnsignedInteger32Bit,
            SignedInteger32Bit, UnsignedInteger64Bit, SignedInteger64Bit, Float32Bit, Float64Bit };
    File f;
    bool ok = f.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);

    if (ok) {
        uint32 n;
        f.Printf("%s", "#");
//Write the header
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            if (n != 0u) {
                f.Printf("%s", csvSeparator);
            }
            f.Printf("%s (%s)[%d]", signalNames[n], TypeDescriptor::GetTypeNameFromTypeDescriptor(signalTypes[n]), signalToVerifyNumberOfElements[n]);
        }
        f.Printf("%s", "\n");
        uint32 s;
        for (s = 0; s < signalToVerifyNumberOfSamples; s++) {
            for (n = 0u; n < N_OF_SIGNALS; n++) {
                if (n != 0u) {
                    f.Printf("%s", csvSeparator);
                }
                if (signalToVerifyNumberOfElements[n] > 1) {
                    uint32 i;
                    f.Printf("%s", "{");
                    for (i = 0u; i < signalToVerifyNumberOfElements[n]; i++) {
                        if (i != 0u) {
                            f.Printf("%s", ",");
                        }
                        signalToVerify[s]->Printf(f, signalTypes[n], i);
                    }
                    f.Printf("%s", "}");
                }
                else {
                    signalToVerify[s]->Printf(f, signalTypes[n], 0);
                }
            }
            f.Printf("%s", "\n");
        }
    }
    f.Flush();
    f.Close();
}

static void GenerateBinaryFile(const MARTe::char8 *const filename,
                               FRTSignalToVerify **signalToVerify,
                               MARTe::uint32 *signalToVerifyNumberOfElements,
                               MARTe::uint32 signalToVerifyNumberOfSamples) {
    using namespace MARTe;
    const uint32 N_OF_SIGNALS = 10;
    const char8 *signalNames[N_OF_SIGNALS] = { "SignalUInt8", "SignalInt8", "SignalUInt16", "SignalInt16", "SignalUInt32", "SignalInt32", "SignalUInt64",
            "SignalInt64", "SignalFloat32", "SignalFloat64WhichIsAlsoAVeryLon" };
    const TypeDescriptor signalTypes[N_OF_SIGNALS] = { UnsignedInteger8Bit, SignedInteger8Bit, UnsignedInteger16Bit, SignedInteger16Bit, UnsignedInteger32Bit,
            SignedInteger32Bit, UnsignedInteger64Bit, SignedInteger64Bit, Float32Bit, Float64Bit };

    uint32 signalBinarySize = 0u;
    uint32 i;
    for (i = 0u; i < N_OF_SIGNALS; i++) {
        signalBinarySize += signalTypes[i].numberOfBits * signalToVerifyNumberOfElements[i] / 8u;
    }
    const uint32 SIGNAL_NAME_SIZE = 32;
    File f;
    bool ok = f.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);

    if (ok) {
        uint32 writeSize = sizeof(uint32);
        f.Write(reinterpret_cast<const char8*>(&N_OF_SIGNALS), writeSize);
        uint32 n;
//Write the header
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            writeSize = sizeof(uint16);
            f.Write(reinterpret_cast<const char8*>(&signalTypes[n].all), writeSize);
            char8 signalName32[SIGNAL_NAME_SIZE];
            MemoryOperationsHelper::Set(&signalName32[0], '\0', SIGNAL_NAME_SIZE);
            MemoryOperationsHelper::Copy(&signalName32[0], signalNames[n], StringHelper::Length(signalNames[n]));
            writeSize = SIGNAL_NAME_SIZE;
            f.Write(reinterpret_cast<const char8*>(&signalName32[0]), writeSize);

            writeSize = sizeof(uint32);
            f.Write(reinterpret_cast<const char8*>(&signalToVerifyNumberOfElements[n]), writeSize);
        }
        uint32 s;
        for (s = 0; s < signalToVerifyNumberOfSamples; s++) {
            for (n = 0u; n < N_OF_SIGNALS; n++) {
                writeSize = signalToVerifyNumberOfElements[n] * signalTypes[n].numberOfBits / 8;
                f.Write(reinterpret_cast<const char8*>(signalToVerify[s]->signalPtrs[n]), writeSize);
            }
        }
    }
    f.Flush();
    f.Close();
}

static bool TestIntegratedExecution(const MARTe::char8 *const config,
                                    bool csv,
                                    MARTe::uint32 *numberOfElements,
                                    const MARTe::char8 *const csvSeparator,
                                    bool forceEOFRewind = false,
                                    bool forceEOFLast = false,
                                    bool forceEOFError = false) {
    using namespace MARTe;
    const char8 *filename = "";
    bool ok = true;
    const uint32 signalToVerifyNumberOfSamples = 3u;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    uint32 i;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }

    if (csv) {
        filename = "TestIntegratedExecution.csv";
        GenerateCSVFile(filename, csvSeparator, signals, numberOfElements, signalToVerifyNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, true, 0, "", true, false, csvSeparator,
                                         forceEOFRewind, forceEOFLast, forceEOFError);
        }
    }
    else {
        filename = "TestIntegratedExecution.bin";
        GenerateBinaryFile(filename, signals, numberOfElements, signalToVerifyNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, false, 0, "", true, false, "",
                                         forceEOFRewind, forceEOFLast, forceEOFError);
        }
    }

    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete signals;
    DeleteTestFile(filename);

    return ok;
}

static bool TestIntegratedExecutionInterpolation(const MARTe::char8 *const config,
                                                 bool csv,
                                                 MARTe::uint32 *numberOfElements,
                                                 const MARTe::char8 *const xAxisSignal = "SignalUInt32") {
    using namespace MARTe;
    const char8 *filename = "";
    bool ok = true;
    const uint32 initialValue = 4;
    const uint32 signalToGenerateNumberOfSamples = 6;
    const uint32 signalToVerifyNumberOfSamples = signalToGenerateNumberOfSamples * 2 - 1;

    FRTSignalToVerify **signalsFile = new FRTSignalToVerify*[signalToGenerateNumberOfSamples];
    FRTSignalToVerify **signalsToVerify = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    const uint32 interpolationPeriod = 2;
    uint32 i;
    for (i = 0; i < signalToGenerateNumberOfSamples; i++) {
        signalsFile[i] = new FRTSignalToVerify(numberOfElements, 2 * interpolationPeriod * i + initialValue);
    }
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signalsToVerify[i] = new FRTSignalToVerify(numberOfElements, i * interpolationPeriod + initialValue);
    }

    if (csv) {
        filename = "TestIntegratedExecutionInterpolation.csv";
        GenerateCSVFile(filename, ";", signalsFile, numberOfElements, signalToGenerateNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signalsToVerify, numberOfElements, signalToVerifyNumberOfSamples, true, interpolationPeriod,
                                         xAxisSignal);
        }
    }
    else {
        filename = "TestIntegratedExecutionInterpolation.bin";
        GenerateBinaryFile(filename, signalsFile, numberOfElements, signalToGenerateNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signalsToVerify, numberOfElements, signalToVerifyNumberOfSamples, false, interpolationPeriod,
                                         xAxisSignal);
        }
    }
    for (i = 0; i < signalToGenerateNumberOfSamples; i++) {
        delete signalsFile[i];
    }
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signalsToVerify[i];
    }
    delete signalsFile;
    delete signalsToVerify;
    DeleteTestFile(filename);

    return ok;
}

//Standard configuration to be patched
static const MARTe::char8 *const config1 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Standard configuration to be patched
static const MARTe::char8 *const config1P = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            Preload = yes"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Standard configuration to be patched
static const MARTe::char8 *const config1P_smallSize = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            Preload = yes"
        "            MaxFileByteSize = 4"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Standard configuration to be patched
static const MARTe::char8 *const config1P_B = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.bin\""
        "            FileFormat = binary"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            Preload = yes"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Wrong configuration with samples
static const MARTe::char8 *const config2 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                    Samples = 2"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"config2.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Wrong configuration with more than one GAM interacting with the DataSource
static const MARTe::char8 *const config3 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"config3.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            XAxisSignal = SignalUInt32"
        "            Interpolate = no"
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
        "                    Functions = {GAM1 GAM2}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Invalid message destination for run time error
static const MARTe::char8 *const config4 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelperError"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Invalid message destination for run time error
static const MARTe::char8 *const config4P = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Preload = yes"
        "            EOF = Error"
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelperError"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Invalid runtime error message name
static const MARTe::char8 *const config5 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeErrorNew = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Invalid runtime error message type
static const MARTe::char8 *const config6 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = ReferenceContainer"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//Invalid runtime error message type
static const MARTe::char8 *const config6P = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            Preload = yes"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = ReferenceContainer"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 but with Frequency set
static const MARTe::char8 *const config7 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                    Frequency = 500000000"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = yes"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 with strings
static const MARTe::char8 *const config8 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalChar8 = {"
        "                    Type = char8"
        "                    DataSource = Drv1"
        "                    NumberOfElements = 5"
        "                }"
        "                SignalString = {"
        "                    Type = string"
        "                    DataSource = Drv1"
        "                    NumberOfElements = 7"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 but with EOF set to Rewind
static const MARTe::char8 *const config9 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            EOF = Rewind"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 but with EOF set to Rewind
static const MARTe::char8 *const config9P = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            Preload = yes"
        "            XAxisSignal = SignalUInt32"
        "            EOF = Rewind"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 but with EOF set to Last
static const MARTe::char8 *const config10 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            EOF = Last"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 but with EOF set to Last
static const MARTe::char8 *const config10P = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Preload = yes"
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            EOF = Last"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 but with EOF set to Error
static const MARTe::char8 *const config11 = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            EOF = Error"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

//As config1 but with EOF set to Error
static const MARTe::char8 *const config11P = ""
        "$Test = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = FileReaderGAMTriggerTestHelper"
        "            InputSignals = {"
        "                SignalUInt8 = {"
        "                    Type = uint8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt8 = {"
        "                    Type = int8"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt16 = {"
        "                    Type = uint16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt16 = {"
        "                    Type = int16"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt32 = {"
        "                    Type = uint32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt32 = {"
        "                    Type = int32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalUInt64 = {"
        "                    Type = uint64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalInt64 = {"
        "                    Type = int64"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat32 = {"
        "                    Type = float32"
        "                    DataSource = Drv1"
        "                }"
        "                SignalFloat64WhichIsAlsoAVeryLon = {"
        "                    Type = float64"
        "                    DataSource = Drv1"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "        +Drv1 = {"
        "            Class = FileReader"
        "            Filename = \"filereader_test.csv\""
        "            FileFormat = csv"
        "            Preload = yes"
        "            CSVSeparator = \";\""
        "            Interpolate = no"
        "            XAxisSignal = SignalUInt32"
        "            EOF = Error"
        "            +Messages = {"
        "                Class = ReferenceContainer"
        "                +FileRuntimeError = {"
        "                    Class = Message"
        "                    Destination = FileReaderTestHelper"
        "                    Function = HandleRuntimeError"
        "                    Mode = ExpectsReply"
        "                }"
        "            }"
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
        "                    Functions = {GAM1}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FileReaderSchedulerTestHelper"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+FileReaderTestHelper = {"
        "    Class = FileReaderTestHelper"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +MessageFlush = {"
        "        Class = Message"
        "        Destination = \"Test.Data.Drv1\""
        "        Function = FlushFile"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool FileReaderTest::TestConstructor() {
    using namespace MARTe;
    FileReader test;
    return true;
}

bool FileReaderTest::TestAllocateMemory() {
    using namespace MARTe;
    FileReader test;
    return test.AllocateMemory();
}

bool FileReaderTest::TestGetNumberOfMemoryBuffers() {
    using namespace MARTe;
    FileReader test;
    return (test.GetNumberOfMemoryBuffers() == 1u);
}

bool FileReaderTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    void *ptr;
    FileReader test;
    bool ok = !test.GetSignalMemoryBuffer(0, 0, ptr);
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 3, 1, 1, 1, 1, 10, 1 };
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0], ";");
    }
    return ok;
}

bool FileReaderTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, OutputSignals), "") == 0);

    return ok;
}

bool FileReaderTest::TestGetBrokerName_MemoryMapInterpolatedInputBroker() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_Test.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint32");
    cdb.Write("InterpolationPeriod", 2);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapInterpolatedInputBroker") == 0);
    }
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestGetBrokerName_MemoryMapInputBroker() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_Test.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "no");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapSynchronisedInputBroker") == 0);
    }
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestGetInputBrokers() {
    using namespace MARTe;
    void *ptr;
    FileReader test;
    bool ok = !test.GetSignalMemoryBuffer(0, 0, ptr);
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 3, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0], ";");
    }
    return ok;
}

bool FileReaderTest::TestGetOutputBrokers() {
    using namespace MARTe;
    FileReader test;
    ReferenceContainer rc;
    return !test.GetOutputBrokers(rc, "", NULL);
}

bool FileReaderTest::TestSynchronise_CSV() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecution(config1, true, &numberOfElements[0], ";");
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 3, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecution(config1, true, &numberOfElements[0], ";");
    }
    return ok;
}

bool FileReaderTest::TestSynchronise_CSV_Strings() {
    using namespace MARTe;
    const char8 *filename = "FileReaderTest_Test.csv";
    const char8 *fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1];SignalChar8 (char8)[5];SignalString (string)[7]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000;AAAA;aaaaaa\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000;BBBB;bbbbbb\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000;C;cc\n";
    GenerateFile(filename, fileContent);
    ConfigurationDatabase cdb;
    StreamString configStream = config8;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    cdb.MoveAbsolute("$Test.+Data.+Drv1");
    cdb.Delete("Filename");
    cdb.Write("Filename", filename);
    cdb.Delete("Interpolate");
    cdb.Write("Interpolate", "no");
    cdb.Delete("FileFormat");
    cdb.Write("FileFormat", "csv");
    cdb.Delete("CSVSeparator");
    cdb.Write("CSVSeparator", ";");

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        cdb.MoveToRoot();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    ReferenceT<FileReaderSchedulerTestHelper> scheduler;
    ReferenceT<FileReaderGAMTriggerTestHelper> gam;
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    if (ok) {
        application = god->Find("Test");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        scheduler = application->Find("Scheduler");
        ok = scheduler.IsValid();
    }
    if (ok) {
        gam = application->Find("Functions.GAM1");
        ok = gam.IsValid();
    }
    ReferenceT<FileReader> fileReader;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        ok = application->StartNextStateExecution();
    }
    if (ok) {
        uint32 s;
        uint32 signalToVerifyNumberOfSamples = 3u;
        StreamString *char8ToVerify = new StreamString[signalToVerifyNumberOfSamples];
        StreamString *stringToVerify = new StreamString[signalToVerifyNumberOfSamples];
        char8ToVerify[0] = "AAAA";
        char8ToVerify[1] = "BBBB";
        char8ToVerify[2] = "C";
        stringToVerify[0] = "aaaaaa";
        stringToVerify[1] = "bbbbbb";
        stringToVerify[2] = "cc";

        for (s = 0; (s < signalToVerifyNumberOfSamples) && (ok); s++) {
            scheduler->ExecuteThreadCycle(0);
            ok = (char8ToVerify[s] == gam->char8Signal);
            ok &= (stringToVerify[s] == gam->stringSignal);
        }
        delete[] char8ToVerify;
        delete[] stringToVerify;
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    god->Purge();

    DeleteTestFile(filename);

    return ok;
}

bool FileReaderTest::TestSynchronise_CSV_Comma() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecution(config1, true, &numberOfElements[0], ",");
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 3, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecution(config1, true, &numberOfElements[0], ",");
    }
    return ok;
}

bool FileReaderTest::TestSynchronise_CSV_Interpolation() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecutionInterpolation(config1, true, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecutionInterpolation(config1, true, &numberOfElements[0]);
    }
    return ok;
}

bool FileReaderTest::TestSynchronise_CSV_Interpolation_Preload() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecutionInterpolation(config1P, true, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecutionInterpolation(config1P, true, &numberOfElements[0]);
    }
    return ok;
}

bool FileReaderTest::TestSynchronise_Binary() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0], ";");
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 3, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0], ";");
    }
    return ok;
}

bool FileReaderTest::TestSynchronise_Binary_Interpolation() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecutionInterpolation(config1, false, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecutionInterpolation(config1, false, &numberOfElements[0]);
    }
    return ok;
}

bool FileReaderTest::TestSynchronise_Binary_Interpolation_Preload() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecutionInterpolation(config1P, false, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecutionInterpolation(config1P, false, &numberOfElements[0]);
    }
    return ok;
}

bool FileReaderTest::TestSynchronise_Frequency() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecutionInterpolation(config7, true, &numberOfElements[0]);
    }

    return ok;
}

bool FileReaderTest::TestSynchronise_XAxisSignals() {
    using namespace MARTe;
    bool ok = true;
    const uint32 N_OF_SIGNALS = 10;
    const char8 *signalNames[N_OF_SIGNALS] = { "SignalUInt8", "SignalInt8", "SignalUInt16", "SignalInt16", "SignalUInt32", "SignalInt32", "SignalUInt64",
            "SignalInt64", "SignalFloat32", "SignalFloat64WhichIsAlsoAVeryLon" };
    uint32 i;
    for (i = 0u; (i < N_OF_SIGNALS) && (ok); i++) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        numberOfElements[i] = 1;
        ok = TestIntegratedExecutionInterpolation(config1, true, &numberOfElements[0], signalNames[i]);
    }

    return ok;
}

bool FileReaderTest::TestPrepareNextState() {
    using namespace MARTe;
    FileReader test;
    return test.PrepareNextState(NULL, NULL);
}

bool FileReaderTest::TestInitialise() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetFilename() == filename);
    ok &= (test.GetFileFormat() == "csv");
    ok &= (test.GetCSVSeparator() == ";");
    ok &= (test.GetXAxisSignal() == "SignalUint8");
    ok &= (test.GetInterpolationPeriod() == 100);
    ok &= (test.IsInterpolate());
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestInitialise_1() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "no");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetFilename() == filename);
    ok &= (test.GetFileFormat() == "csv");
    ok &= (test.GetCSVSeparator() == ";");
    ok &= (!test.IsInterpolate());
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestInitialise_False_Interpolate() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_Interpolate_Invalid() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "true");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestInitialise_Binary() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.bin";
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    const uint32 signalToVerifyNumberOfSamples = 3u;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    uint32 i;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }
    GenerateBinaryFile(filename, signals, numberOfElements, signalToVerifyNumberOfSamples);
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete signals;
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "binary");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetFilename() == filename);
    ok &= (test.GetFileFormat() == "binary");
    ok &= (test.GetXAxisSignal() == "SignalUint8");
    ok &= (test.GetInterpolationPeriod() == 100);
    ok &= (test.IsInterpolate());
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestInitialise_False_Filename() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_Filename_1() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "doesnotexist.csv";
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_CorruptedFile() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "corrupted.csv";
    const char8 *fileContent =
            ""
                    "#Trigger (uint822)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLongSignalNameSoThatItHasMoreThan32CharsAndIsHopefullyTruncated (float64)[1]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    GenerateFile(filename, fileContent);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_FileFormat() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_FileFormat_Invalid() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.Write("FileFormat", "xls");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_CSVSeparator() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "yes");
    cdb.Write("FileFormat", "csv");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_EOF() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.bin";
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "binary");
    cdb.Write("Interpolate", "yes");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.Write("InterpolationPeriod", 100);
    cdb.Write("EOF", "Ignore");
    cdb.MoveToRoot();
    bool ok = !test.Initialise(cdb);
    return ok;
}

bool FileReaderTest::TestInitialise_Warning_XAxisSignal() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "yes");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("FileFormat", "csv");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestInitialise_Warning_InterpolationPeriod() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "yes");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("FileFormat", "csv");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestInitialise_Preload_no() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "yes");
    cdb.Write("Preload", "no");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("FileFormat", "csv");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestInitialise_Preload_yes_NoMaxSize() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "yes");
    cdb.Write("Preload", "yes");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("FileFormat", "csv");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestInitialise_Preload_yes_MaxSizeToLarge() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 *const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "no");
    cdb.Write("Preload", "yes");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("FileFormat", "csv");
    uint64 maxSize = 5e9;
    cdb.Write("MaxFileByteSize", maxSize);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1, false, &numberOfElements[0], ";");
}

bool FileReaderTest::TestSetConfiguradDatabase_InputSignalSize0() {
    using namespace MARTe;
    FileReader ds;
    ConfigurationDatabase cfgInitialise;
    ConfigurationDatabase cfgSet;
    StreamString fileName = "test.bin";
    cfgInitialise.Write("FileFormat", "binary");
    cfgInitialise.Write("Filename", fileName.Buffer());
    cfgInitialise.Write("Interpolate", "no");
    cfgInitialise.Write("EOF", "Rewind");
    GenerateFile3(fileName.Buffer());
    bool ok = ds.Initialise(cfgInitialise);
    cfgSet.CreateAbsolute("Signals");
    cfgSet.CreateRelative("0");
    cfgSet.Write("NodeName", "Header1");
    cfgSet.Write("QualifiedName", "Header1LL");
    cfgSet.Write("ByteSize", 0);
    cfgSet.MoveToAncestor(1);
    cfgSet.MoveToRoot();
    if (ok) {
        ok = !ds.SetConfiguredDatabase(cfgSet);
    }
    DeleteTestFile(fileName.Buffer());
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_False_NumberOfSamples() {
    using namespace MARTe;
    const char8 *const filename = "config2.csv";
    GenerateFile(filename);
    bool ok = !TestIntegratedInApplication(config2, true);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_WrongFileSize_CSV() {
    using namespace MARTe;
    const char8 *const filename = "filereader_test.csv";
    GenerateFile2(filename);
    bool ok = !TestIntegratedInApplication(config1P, true);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_WrongFileSize_Bin() {
    using namespace MARTe;
    const char8 *const filename = "filereader_test.bin";
    GenerateFile3(filename);
    bool ok = !TestIntegratedInApplication(config1P_B, true);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_MaxSizeSmallerThanFileSize() {
    using namespace MARTe;
    const char8 *const filename = "filereader_test.csv";
    GenerateFile2(filename);
    bool ok = !TestIntegratedInApplication(config1P_smallSize, true);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_False_MoreThanOneFunction() {
    using namespace MARTe;
    const char8 *const filename = "config3.csv";
    GenerateFile(filename);
    bool ok = !TestIntegratedInApplication(config3, true);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_False_XAxisSignal() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = !TestIntegratedExecutionInterpolation(config1, true, &numberOfElements[0], "DoesNotExist");
    }

    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_False_XAxisSignal_Dimensions() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 10, 4, 3, 2, 4, 2 };
        ok = !TestIntegratedExecutionInterpolation(config1, true, &numberOfElements[0]);
    }

    return ok;
}

bool FileReaderTest::TestOpenFile() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1, false, &numberOfElements[0], ";");
}

bool FileReaderTest::TestCloseFile() {
    return TestOpenFile();
}

bool FileReaderTest::TestGetFilename() {
    return TestInitialise();
}

bool FileReaderTest::TestGetFileFormat() {
    return TestInitialise();
}

bool FileReaderTest::TestGetCSVSeparator() {
    return TestInitialise();
}

bool FileReaderTest::TestIsInterpolate() {
    return TestInitialise();
}

bool FileReaderTest::TestGetXAxisSignal() {
    return TestInitialise();
}

bool FileReaderTest::TestGetInterpolationPeriod() {
    return TestInitialise();
}

bool FileReaderTest::TestRuntimeErrorMessage() {
    using namespace MARTe;
    const char8 *fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    const char8 *const filename = "filereader_test.csv";
    GenerateFile(filename, fileContent);
    const uint32 signalToVerifyNumberOfSamples = 3u;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 i;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }
    TestIntegratedExecution(config1, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, true, 0, "SignalUint32", false, true);
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete signals;
    DeleteTestFile(filename);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReaderTestHelper> helper = godb->Find("FileReaderTestHelper");
    bool ok = helper.IsValid();

    if (ok) {
        ok = (helper->runtimeErrorFunctionCalled);
    }
    if (ok) {
        Directory toDelete(filename);
        toDelete.Delete();
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestRuntimeErrorMessage_2() {
    using namespace MARTe;
    const char8 *fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[2];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1]\n"
                    "1;2000000;{2,2};2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;{4};4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;{5,5};5;5;5;5;5;5;5;5.000000;5.000000\n";
    const char8 *const filename = "filereader_test.csv";
    GenerateFile(filename, fileContent);
    const uint32 signalToVerifyNumberOfSamples = 3u;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    uint32 numberOfElements[] = { 1, 1, 2, 1, 1, 1, 1, 1, 1, 1 };
    uint32 i;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }
    TestIntegratedExecution(config1, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, true, 0, "SignalUint32", false, true);
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete signals;
    DeleteTestFile(filename);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReaderTestHelper> helper = godb->Find("FileReaderTestHelper");
    bool ok = helper.IsValid();

    if (ok) {
        ok = (helper->runtimeErrorFunctionCalled);
    }
    if (ok) {
        Directory toDelete(filename);
        toDelete.Delete();
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestRuntimeErrorMessage_Fail() {
    using namespace MARTe;
    const char8 *fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1]\n"
                    "1;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    const char8 *const filename = "filereader_test.csv";
    GenerateFile(filename, fileContent);
    const uint32 signalToVerifyNumberOfSamples = 3u;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 i;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }
    TestIntegratedExecution(config4, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, true, 0, "SignalUint32", false, true);
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete[] signals;
    DeleteTestFile(filename);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReaderTestHelper> helper = godb->Find("FileReaderTestHelper");
    bool ok = helper.IsValid();

    if (ok) {
        ok = (!helper->runtimeErrorFunctionCalled);
    }
    if (ok) {
        Directory toDelete(filename);
        toDelete.Delete();
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestRuntimeErrorMessage_Fail_Preload() {
    using namespace MARTe;
    const char8 *fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1]\n"
                    "1;2;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    const char8 *const filename = "filereader_test.csv";
    GenerateFile(filename, fileContent);
    const uint32 signalToVerifyNumberOfSamples = 4u;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    uint32 i;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }
    TestIntegratedExecution(config4P, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, true, 0, "SignalUint32", false, true);
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete signals;
    DeleteTestFile(filename);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReaderTestHelper> helper = godb->Find("FileReaderTestHelper");
    bool ok = helper.IsValid();

    if (ok) {
        ok = (!helper->runtimeErrorFunctionCalled);
    }
    if (ok) {
        Directory toDelete(filename);
        toDelete.Delete();
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestInvalidMessageName() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config5, true);
    return !ok;
}

bool FileReaderTest::TestInvalidMessageType() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config6, true);
    return !ok;
}

bool FileReaderTest::TestInvalidMessageType_Preload() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config6P, true);
    return !ok;
}

bool FileReaderTest::TestEOF_CSV() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1, true, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_CSV_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1P, true, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_Binary() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1, false, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_Binary_JumboFile() {
    using namespace MARTe;
    FileReader ds;
    ConfigurationDatabase cfgInitialise;
    ConfigurationDatabase cfgSet;
    StreamString fileName = "test.bin";
    cfgInitialise.Write("FileFormat", "binary");
    cfgInitialise.Write("Preload", "yes");
    uint64 auxSize = 1000000000;
    cfgInitialise.Write("MaxFileByteSize", auxSize);
    cfgInitialise.Write("Filename", fileName.Buffer());
    cfgInitialise.Write("Interpolate", "no");
    cfgInitialise.Write("EOF", "Rewind");
    GenerateJumboFile(fileName.Buffer());
    bool ok = ds.Initialise(cfgInitialise);
    cfgSet.CreateAbsolute("Signals");
    cfgSet.CreateRelative("0");
    cfgSet.Write("NodeName", "SignalUInt8");
    cfgSet.Write("QualifiedName", "SignalUInt8");
    cfgSet.Write("ByteSize", 510000000);
///////////////
// Functions //
///////////////
    cfgSet.CreateAbsolute("Functions");
    cfgSet.CreateRelative("0");
    cfgSet.Write("QualifiedName", "SignalUInt8");
    cfgSet.CreateRelative("InputSignals");
    cfgSet.CreateRelative("0");
    cfgSet.Write("Samples", 1);
    cfgSet.Write("Broker", "MemoryMapSynchronisedInputBroker");
    cfgSet.Write("GAMMemoryOffset", 0);
//    uint32 byteOffset[1][2] = { { 0, 48 * 1 } };
//    ok &= cfgSet.Write("ByteOffset", byteOffset);
    cfgSet.MoveToAncestor(1u);
    cfgSet.Write("ByteOffset", 48 * 3 + 10 + 5 + 4);
    cfgSet.MoveToRoot();
    if (ok) {
        ok = ds.SetConfiguredDatabase(cfgSet);
    }
    DeleteTestFile(fileName.Buffer());
    return ok;
}

bool FileReaderTest::TestEOF_Binary_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1P, false, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_Rewind_CSV() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config9, true, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_Rewind_CSV_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config9P, true, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_Rewind_Binary() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config9, false, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_Rewind_Binary_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config9P, false, &numberOfElements[0], ";", true, false, false);
}

bool FileReaderTest::TestEOF_Last_CSV() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config10, true, &numberOfElements[0], ";", false, true, false);
}

bool FileReaderTest::TestEOF_Last_CSV_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config10P, true, &numberOfElements[0], ";", false, true, false);
}

bool FileReaderTest::TestEOF_Last_Binary() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config10, false, &numberOfElements[0], ";", false, true, false);
}

bool FileReaderTest::TestEOF_Last_Binary_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config10P, false, &numberOfElements[0], ";", false, true, false);
}

bool FileReaderTest::TestEOF_Error_CSV() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config11, true, &numberOfElements[0], ";", false, false, true);
}

bool FileReaderTest::TestEOF_Error_CSV_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config11P, true, &numberOfElements[0], ";", false, false, true);
}

bool FileReaderTest::TestEOF_Error_Binary() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config11, false, &numberOfElements[0], ";", false, false, true);
}

bool FileReaderTest::TestEOF_Error_Binary_Preload() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config11P, false, &numberOfElements[0], ";", false, false, true);
}
