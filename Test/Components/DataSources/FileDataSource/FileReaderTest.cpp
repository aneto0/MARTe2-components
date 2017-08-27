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
#include <stdio.h>

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
public:
    CLASS_REGISTER_DECLARATION()FileReaderTestHelper() : Object(), MessageI() {
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
public:
    CLASS_REGISTER_DECLARATION()FileReaderGAMTriggerTestHelper() {
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
            delete []numberOfElements;
        }
        using namespace MARTe;
    }

    virtual bool Initialise(MARTe::StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Setup() {
        using namespace MARTe;
        numberOfElements = new uint32[GetNumberOfInputSignals()];
        uint32 n;
        for (n=0; n<GetNumberOfInputSignals(); n++) {
            uint32 nElements;
            GetSignalNumberOfElements(InputSignals, 0, nElements);
            numberOfElements[n] = nElements;
            if (GetSignalType(InputSignals, n) == UnsignedInteger8Bit) {
                uint8Signal = reinterpret_cast<uint8 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger8Bit) {
                int8Signal = reinterpret_cast<int8 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger16Bit) {
                uint16Signal = reinterpret_cast<uint16 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger16Bit) {
                int16Signal = reinterpret_cast<int16 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger32Bit) {
                uint32Signal = reinterpret_cast<uint32 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger32Bit) {
                int32Signal = reinterpret_cast<int32 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == UnsignedInteger64Bit) {
                uint64Signal = reinterpret_cast<uint64 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == SignedInteger64Bit) {
                int64Signal = reinterpret_cast<int64 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == Float32Bit) {
                float32Signal = reinterpret_cast<float32 *>(GetInputSignalMemory(n));
            }
            else if (GetSignalType(InputSignals, n) == Float64Bit) {
                float64Signal = reinterpret_cast<float64 *>(GetInputSignalMemory(n));
            }

        }
        return true;
    }

    virtual bool Execute() {
        using namespace MARTe;
        return true;
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
    MARTe::uint32 *numberOfElements;
};
CLASS_REGISTER(FileReaderGAMTriggerTestHelper, "1.0")

/**
 * @brief Manual scheduler to test the correct interface between the FileReader and the GAMs
 */
class FileReaderSchedulerTestHelper: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

FileReaderSchedulerTestHelper    () : MARTe::GAMSchedulerI() {
        scheduledStates = NULL;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        return MARTe::ErrorManagement::NoError;
    }

    void ExecuteThreadCycle(MARTe::uint32 threadId) {
        using namespace MARTe;
        ExecuteSingleCycle(scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].executables,
                scheduledStates[RealTimeApplication::GetIndex()]->threads[threadId].numberOfExecutables);
    }

    virtual bool ConfigureScheduler() {

        bool ret = GAMSchedulerI::ConfigureScheduler();
        if (ret) {
            scheduledStates = GetSchedulableStates();
        }
        return ret;
    }

    virtual void CustomPrepareNextState() {
    }

private:

    MARTe::ScheduledState * const * scheduledStates;
};

CLASS_REGISTER(FileReaderSchedulerTestHelper, "1.0")

#if 0
static bool TestIntegratedInApplication(const MARTe::char8 * const config, bool destroy) {
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
#endif

#define FRT_ADD_SIGNAL_TO_VERIFY(idx,type,nElements,value)\
        if (nElements[idx] > 0) {\
            type ## Signal = new type[nElements[idx]];\
            uint32 zz;\
            for(zz=0;zz<nElements[idx];zz++){\
                type ## Signal[zz]=value;\
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

class FRTSignalToVerify {
public:
    FRTSignalToVerify(MARTe::uint32 *nElements, MARTe::uint32 value) {
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

#define FRT_VERIFY_SIGNAL(type)\
        for(z=0; (z<signalToVerifyNumberOfElements[s]) && (ok); z++) {\
            if (signalToVerify[s]->type ## Signal != NULL) {\
                ok = (signalToVerify[s]->type ## Signal[z] == gam->type ## Signal[z]);\
            }\
        }

static void GenerateCSVFile(const MARTe::char8 * const filename, const MARTe::char8 * const fileContent) {
    using namespace MARTe;
    File f;
    f.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    f.Printf("%s", fileContent);
    f.Flush();
    f.Close();
}

static void DeleteTestFile(const MARTe::char8 * const filename) {
    using namespace MARTe;
    Directory toDelete(filename);
    toDelete.Delete();
}

static bool TestIntegratedExecution(const MARTe::char8 * const config, const MARTe::char8 * const filename, FRTSignalToVerify **signalToVerify, MARTe::uint32 *signalToVerifyNumberOfElements, MARTe::uint32 signalToVerifyNumberOfSamples, bool csv, bool interpolated) {
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
    if (interpolated) {
        cdb.Write("Interpolate", "yes");
    }
    else {
        cdb.Write("Interpolate", "no");
    }
    cdb.Delete("FileFormat");
    if (csv) {
        cdb.Write("FileFormat", "csv");
        cdb.Delete("CSVSeparator");
        cdb.Write("CSVSeparator", ";");
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
            scheduler->ExecuteThreadCycle(0);
            FRT_VERIFY_SIGNAL(uint8);
            FRT_VERIFY_SIGNAL(int8);
            FRT_VERIFY_SIGNAL(uint16);
            FRT_VERIFY_SIGNAL(int16);
            FRT_VERIFY_SIGNAL(uint32);
            FRT_VERIFY_SIGNAL(int32);
            FRT_VERIFY_SIGNAL(uint64);
            FRT_VERIFY_SIGNAL(int64);
            FRT_VERIFY_SIGNAL(float32);
            FRT_VERIFY_SIGNAL(float64);

        }
    }
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    god->Purge();
    return ok;
}

static void GenerateBinaryFile(const MARTe::char8 * const filename, FRTSignalToVerify **signalToVerify, MARTe::uint32 *signalToVerifyNumberOfElements, MARTe::uint32 signalToVerifyNumberOfSamples) {
    using namespace MARTe;
    const uint32 N_OF_SIGNALS = 10;
    const char8 *signalNames[N_OF_SIGNALS] = { "SignalUInt8", "SignalInt8", "SignalUInt16", "SignalInt16", "SignalUInt32", "SignalInt32", "SignalUInt64", "SignalInt64", "SignalFloat32",
            "SignalFloat64WhichIsAlsoAVeryLon" };
    const TypeDescriptor signalTypes[N_OF_SIGNALS] = { UnsignedInteger8Bit, SignedInteger8Bit, UnsignedInteger16Bit, SignedInteger16Bit, UnsignedInteger32Bit, SignedInteger32Bit, UnsignedInteger64Bit,
            SignedInteger64Bit, Float32Bit, Float64Bit };

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
        f.Write(reinterpret_cast<const char8 *>(&N_OF_SIGNALS), writeSize);
        uint32 n;
        //Write the header
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            writeSize = sizeof(uint16);
            f.Write(reinterpret_cast<const char8 *>(&signalTypes[n].all), writeSize);
            char8 signalName32[SIGNAL_NAME_SIZE];
            MemoryOperationsHelper::Set(&signalName32[0], '\0', SIGNAL_NAME_SIZE);
            MemoryOperationsHelper::Copy(&signalName32[0], signalNames[n], StringHelper::Length(signalNames[n]));
            writeSize = SIGNAL_NAME_SIZE;
            f.Write(reinterpret_cast<const char8 *>(&signalName32[0]), writeSize);

            writeSize = sizeof(uint32);
            f.Write(reinterpret_cast<const char8 *>(&signalToVerifyNumberOfElements[n]), writeSize);
        }
        uint32 s;
        for (s = 0; s < signalToVerifyNumberOfSamples; s++) {
            for (n = 0u; n < N_OF_SIGNALS; n++) {
                writeSize = signalToVerifyNumberOfElements[n] * signalTypes[n].numberOfBits / 8;
                f.Write(reinterpret_cast<const char8 *>(&signalToVerify[s]->signalPtrs[n]), writeSize);
            }
        }
    }
    f.Close();
}

static bool TestIntegratedExecution(const MARTe::char8 * const config, bool csv) {
    using namespace MARTe;
    const char8 * filename = "";
    const char8 * fileContent;
    bool ok = true;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    FRTSignalToVerify **signals = new FRTSignalToVerify*[3];
    uint32 i;
    const uint32 signalToVerifyNumberOfSamples = 3u;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }

    if (csv) {
        filename = "TestIntegratedExecution.csv";
        fileContent =
                ""
                        "#SignalUInt8 (uint8)[1];SignalInt8 (int8)[1];SignalUInt16 (uint16)[1];SignalInt16 (int16)[1];SignalUInt32 (uint32)[1];SignalInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLon (float64)[1]\n"
                        "1;1;1;1;1;1;1;1;1;1\n"
                        "2;2;2;2;2;2;2;2;2;2\n"
                        "3;3;3;3;3;3;3;3;3;3\n";
        GenerateCSVFile(filename, fileContent);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, true, false);
        }
    }
    else {
        filename = "TestIntegratedExecution.bin";
        GenerateBinaryFile(filename, signals, numberOfElements, signalToVerifyNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, false, false);
        }
    }

    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete signals;

    DeleteTestFile(filename);

    return ok;
}

//Standard configuration to be patched
static const MARTe::char8 * const config1 = ""
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
        "            Filename = \"filewriter_test.bin\""
        "            FileFormat = csv"
        "            CSVSeparator = \";\""
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

#if 0
//Wrong configuration with samples
static const MARTe::char8 * const config2 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                    Samples = 2"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = \"filewriter_test.bin\""
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
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

//Wrong configuration with no signals
static const MARTe::char8 * const config3 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"        +DrvE = {"
"            Class = FileReader"
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = \"filewriter_test_empty.bin\""
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"        }"
"        +Drv1 = {"
"            Class = FileReader"
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = \"filewriter_test.bin\""
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
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

//Wrong configuration with more than one signal interacting with the DataSource
static const MARTe::char8 * const config4 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"            }"
"        }"
"        +GAM2 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = \"filewriter_test.bin\""
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
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

//Standard configuration without specifying the filename (to be set with a message)
static const MARTe::char8 * const config5 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"    +MessageCloseFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = CloseFile"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration with filename to test sending of messages
static const MARTe::char8 * const config6 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = Test_MessageOpenTree_2.bin"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration with a wrong filename to test the sending of the invalid message
static const MARTe::char8 * const config7 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = /tmpzzadsads/asdas/adsa/ss"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration with filename to test sending of messages
static const MARTe::char8 * const config8 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = Test_MessageOpenTree_2.bin"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelperWrong"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration with filename to test sending of messages with a wrong destination
static const MARTe::char8 * const config9 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
//"            Filename = Test_MessageOpenTree_2.bin"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelperWrong"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
"                +FileRuntimeError = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelperWrong"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration with filename to test sending of messages with a wrong destination
static const MARTe::char8 * const config10 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = /does/not/exist"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelperWrong"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration with filename to test sending of messages with an invalid message name
static const MARTe::char8 * const config11 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = Test_MessageOpenTree_2.bin"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpened = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration with filename to test sending of messages with an invalid message type
static const MARTe::char8 * const config12 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            Filename = Test_MessageOpenTree_2.bin"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = yes"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = ReferenceContainer"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";

//Standard configuration without specifying the filename (to be set with a message)
static const MARTe::char8 * const config13 = ""
"$Test = {"
"    Class = RealTimeApplication"
"    +Functions = {"
"        Class = ReferenceContainer"
"        +GAM1 = {"
"            Class = FileReaderGAMTriggerTestHelper"
"            Signal =  {0 1 2 3 4 5 6 7 8 9 8 7 6 5}"
"            InvertSigned = 1"
"            OutputSignals = {"
"                Trigger = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                Time = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                    DataSource = Drv1"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                    DataSource = Drv1"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                    DataSource = Drv1"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                    DataSource = Drv1"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                    DataSource = Drv1"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                    DataSource = Drv1"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
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
"                SignalFloat64 = {"
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
"            NumberOfBuffers = 10"
"            CPUMask = 15"
"            StackSize = 10000000"
"            FileFormat = csv"
"            CSVSeparator = \";\""
"            Overwrite = no"
"            StoreOnTrigger = 0"
"            Signals = {"
"                Trigger = {"
"                    Type = uint8"
"                }"
"                Time = {"
"                    Type = uint32"
"                }"
"                SignalUInt8 = {"
"                    Type = uint8"
"                }"
"                SignalUInt16 = {"
"                    Type = uint16"
"                }"
"                SignalUInt32 = {"
"                    Type = uint32"
"                }"
"                SignalUInt64 = {"
"                    Type = uint64"
"                }"
"                SignalInt8 = {"
"                    Type = int8"
"                }"
"                SignalInt16 = {"
"                    Type = int16"
"                }"
"                SignalInt32 = {"
"                    Type = int32"
"                }"
"                SignalInt64 = {"
"                    Type = int64"
"                }"
"                SignalFloat32 = {"
"                    Type = float32"
"                }"
"                SignalFloat64 = {"
"                    Type = float64"
"                }"
"            }"
"            +Messages = {"
"                Class = ReferenceContainer"
"                +FileOpenedOK = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenOK"
"                    Mode = ExpectsReply"
"                }"
"                +FileOpenedFail = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleOpenFail"
"                    Mode = ExpectsReply"
"                }"
"                +FileClosed = {"
"                    Class = Message"
"                    Destination = FileReaderTestHelper"
"                    Function = HandleClose"
"                    Mode = ExpectsReply"
"                }"
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
"    +MessageOpenFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = OpenFile"
"        +Parameters = {"
"            Class = ConfigurationDatabase"
"            param1 = Test_MessageOpenTree.bin"
"        }"
"    }"
"    +MessageCloseFile = {"
"        Class = Message"
"        Destination = \"Test.Data.Drv1\""
"        Function = CloseFile"
"    }"
"}"
"+FileReaderTestHelper = {"
"    Class = FileReaderTestHelper"
"}";
#endif

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
        ok = TestIntegratedExecution(config1, false);
    }
    return ok;
}

bool FileReaderTest::TestGetBrokerName_OutputSignals() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    bool ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "") == 0);

    return ok;
}

bool FileReaderTest::TestGetBrokerName_MemoryMapInterpolatedInputBroker() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 * const filename = "FileReaderTest_Test.csv";
    const char8 * fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLongSignalNameSoThatItHasMoreThan32CharsAndIsHopefullyTruncated (float64)[1]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    GenerateCSVFile(filename, fileContent);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "yes");
    cdb.CreateRelative("Signals");
    cdb.CreateRelative("InterpolatedTime");
    cdb.Write("Type", "uint64");
    cdb.Write("NumberOfElements", "1");
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
    const char8 * const filename = "FileReaderTest_Test.csv";
    const char8 * fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLongSignalNameSoThatItHasMoreThan32CharsAndIsHopefullyTruncated (float64)[1]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    GenerateCSVFile(filename, fileContent);
    cdb.Write("Filename", filename);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("Interpolate", "no");
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (StringHelper::Compare(test.GetBrokerName(cdb, InputSignals), "MemoryMapInputBroker") == 0);
    }
    DeleteTestFile(filename);
    return ok;
}

#if 0
bool FileReaderTest::TestGetInputBrokers() {
    using namespace MARTe;
    FileReader test;
    ReferenceContainer rc;
    return !test.GetInputBrokers(rc, "", NULL);
}

bool FileReaderTest::TestGetOutputBrokers() {
    bool ok = TestIntegratedInApplication_NoTrigger("FileReaderTest_TestGetOutputBrokers", true);
    return ok;
}

bool FileReaderTest::TestSynchronise() {
    bool ok = true;
    if (ok) {
        ok = TestIntegratedInApplication_NoTrigger("FileReaderTest_TestSynchronise_NoT_CSV", true);
    }
    if (ok) {
        ok = TestIntegratedInApplication_NoTrigger("FileReaderTest_TestSynchronise_NoT_BIN", false);
    }
    if (ok) {
        ok = TestIntegratedInApplication_NoTrigger_Array("FileReaderTest_TestSynchronise_NoT_Arr_CSV", true);
    }
    if (ok) {
        ok = TestIntegratedInApplication_NoTrigger_Array("FileReaderTest_TestSynchronise_NoT_Arr_BIN", false);
    }
    if (ok) {
        ok = TestIntegratedInApplication_Trigger("FileReaderTest_TestSynchronise_T_CSV", true);
    }
    if (ok) {
        ok = TestIntegratedInApplication_Trigger("FileReaderTest_TestSynchronise_T_BIN", false);
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
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNumberOfBuffers() == 10);
    ok &= (test.GetCPUMask() == 15);
    ok &= (test.GetStackSize() == 10000000);
    ok &= (test.GetFilename() == "FileReaderTest_TestInitialise");
    ok &= (test.GetFileFormat() == "csv");
    ok &= (test.GetCSVSeparator() == ",");
    ok &= (test.IsOverwrite());
    ok &= (test.IsStoreOnTrigger());
    ok &= (test.GetNumberOfPreTriggers() == 2);
    ok &= (test.GetNumberOfPostTriggers() == 3);
    return ok;
}
#endif

#if 0
bool FileReaderTest::TestInitialise_False_Overwrite() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_Overwrite_Invalid() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "true");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_Binary() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise_Binary");
    cdb.Write("FileFormat", "binary");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    ok &= (test.GetNumberOfBuffers() == 10);
    ok &= (test.GetCPUMask() == 15);
    ok &= (test.GetStackSize() == 10000000);
    ok &= (test.GetFilename() == "FileReaderTest_TestInitialise_Binary");
    ok &= (test.GetFileFormat() == "binary");
    ok &= (test.IsOverwrite());
    ok &= (test.IsStoreOnTrigger());
    ok &= (test.GetNumberOfPreTriggers() == 2);
    ok &= (test.GetNumberOfPostTriggers() == 3);
    return ok;
}

bool FileReaderTest::TestInitialise_False_NumberOfBuffers() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();

    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_NumberOfBuffers_GT_0() {
    using namespace MARTe;
    FileReader test2;
    ConfigurationDatabase cdb2;
    cdb2.Write("NumberOfBuffers", 0);
    cdb2.Write("CPUMask", 15);
    cdb2.Write("StackSize", 10000000);
    cdb2.Write("Filename", "FileReaderTest_TestInitialise");
    cdb2.Write("FileFormat", "csv");
    cdb2.Write("CSVSeparator", ",");
    cdb2.Write("Overwrite", "yes");
    cdb2.Write("StoreOnTrigger", 1);
    cdb2.Write("NumberOfPreTriggers", 2);
    cdb2.Write("NumberOfPostTriggers", 3);
    cdb2.CreateRelative("Signals");
    cdb2.MoveToRoot();
    return !test2.Initialise(cdb2);
}

bool FileReaderTest::TestInitialise_False_CPUMask() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_StackSize() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_StackSize_GT_0() {
    using namespace MARTe;
    FileReader test2;
    ConfigurationDatabase cdb2;
    cdb2.Write("NumberOfBuffers", 10);
    cdb2.Write("CPUMask", 15);
    cdb2.Write("StackSize", 0);
    cdb2.Write("Filename", "FileReaderTest_TestInitialise");
    cdb2.Write("FileFormat", "csv");
    cdb2.Write("CSVSeparator", ",");
    cdb2.Write("Overwrite", "yes");
    cdb2.Write("StoreOnTrigger", 1);
    cdb2.Write("NumberOfPreTriggers", 2);
    cdb2.Write("NumberOfPostTriggers", 3);
    cdb2.CreateRelative("Signals");
    cdb2.MoveToRoot();
    return !test2.Initialise(cdb2);
}

bool FileReaderTest::TestInitialise_Warning_Filename() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("FileFormat", "csv");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_FileFormat() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_FileFormat_Invalid() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "xls");
    cdb.Write("CSVSeparator", ",");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_CSVSeparator() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "csv");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_StoreOnTrigger() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "binary");
    cdb.Write("Overwrite", "yes");
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_NumberOfPreTriggers() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "binary");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_NumberOfPostTriggers() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "binary");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.CreateRelative("Signals");
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestInitialise_False_Signals() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 10);
    cdb.Write("CPUMask", 15);
    cdb.Write("StackSize", 10000000);
    cdb.Write("Filename", "FileReaderTest_TestInitialise");
    cdb.Write("FileFormat", "binary");
    cdb.Write("Overwrite", "yes");
    cdb.Write("StoreOnTrigger", 1);
    cdb.Write("NumberOfPreTriggers", 2);
    cdb.Write("NumberOfPostTriggers", 3);
    cdb.MoveToRoot();
    return !test.Initialise(cdb);
}

bool FileReaderTest::TestSetConfiguredDatabase() {
    return TestIntegratedInApplication_NoTrigger("FileReaderTest_TestSetConfiguredDatabase", true);
}

bool FileReaderTest::TestSetConfiguredDatabase_False_NumberOfSamples() {
    return !TestIntegratedInApplication(config2, true);
}

bool FileReaderTest::TestSetConfiguredDatabase_False_NoFileSignals() {
    return !TestIntegratedInApplication(config3, true);
}

bool FileReaderTest::TestSetConfiguredDatabase_False_TimeSignal_MoreThanOneFunction() {
    return !TestIntegratedInApplication(config4, true);
}

bool FileReaderTest::TestIntegratedInApplication_NoTrigger(const MARTe::char8 *filename, bool csv) {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    const uint32 N_OF_SIGNALS = 12;
    const char8 *signalNames[N_OF_SIGNALS] = {"Trigger", "Time", "SignalUInt8", "SignalUInt16", "SignalUInt32", "SignalUInt64", "SignalInt8", "SignalInt16",
        "SignalInt32", "SignalInt64", "SignalFloat32", "SignalFloat64WhichIsAlsoAVeryLon"};
    const uint16 signalTypes[N_OF_SIGNALS] = {UnsignedInteger8Bit.all, UnsignedInteger32Bit.all, UnsignedInteger8Bit.all, UnsignedInteger16Bit.all,
        UnsignedInteger32Bit.all, UnsignedInteger64Bit.all, SignedInteger8Bit.all, SignedInteger16Bit.all, SignedInteger32Bit.all, SignedInteger64Bit.all,
        Float32Bit.all, Float64Bit.all};
    const uint32 signalElements[N_OF_SIGNALS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const uint32 numberOfBuffers = 16;
    const float32 period = 2;
    const char8 * expectedFileContent = NULL;
    if (csv) {
        expectedFileContent =
        ""
        "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLongSignalNameSoThatItHasMoreThan32CharsAndIsHopefullyTruncated (float64)[1]\n"
        "0;0;1;1;1;1;1;1;1;1;1.000000;1.000000\n"
        "0;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
        "0;4000000;3;3;3;3;3;3;3;3;3.000000;3.000000\n"
        "0;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
        "0;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    }
    else {
        uint32 writeSize = sizeof(uint8); //trigger
        writeSize += sizeof(uint32);//time
        writeSize += sizeof(uint8);//signalUInt8
        writeSize += sizeof(uint16);//signalUInt16
        writeSize += sizeof(uint32);//signalUInt32
        writeSize += sizeof(uint64);//signalUInt64
        writeSize += sizeof(int8);//signalInt8
        writeSize += sizeof(int16);//signalInt16
        writeSize += sizeof(int32);//signalInt32
        writeSize += sizeof(int64);//signalInt64
        writeSize += sizeof(float32);//signalFloat32
        writeSize += sizeof(float64);//signalFloat64

        const uint32 SIGNAL_NAME_SIZE = 32;
        uint32 headerSize = sizeof(uint32) + N_OF_SIGNALS * (sizeof(uint16) + SIGNAL_NAME_SIZE + sizeof(uint32));
        uint32 memorySize = headerSize + (numberOfElements * writeSize);//5 writes
        expectedFileContent = static_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(memorySize));

        uint32 n;
        //Header
        char8 *header = const_cast<char8 *>(&expectedFileContent[0]);
        MemoryOperationsHelper::Copy(header, reinterpret_cast<const char8 *>(&N_OF_SIGNALS), sizeof(uint32));
        header += sizeof(uint32);
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            MemoryOperationsHelper::Copy(header, &signalTypes[n], sizeof(uint16));
            header += sizeof(uint16);
            MemoryOperationsHelper::Set(header, '\0', SIGNAL_NAME_SIZE);
            MemoryOperationsHelper::Copy(header, signalNames[n], StringHelper::Length(signalNames[n]));
            header += SIGNAL_NAME_SIZE;
            MemoryOperationsHelper::Copy(header, &signalElements[n], sizeof(uint32));
            header += sizeof(uint32);
        }

        for (n = 0u; n < numberOfElements; n++) {
            uint8 *triggerPointer = const_cast<uint8 *>(reinterpret_cast<const uint8 *>(&expectedFileContent[headerSize + (n * writeSize)]));
            uint32 *timerPointer = reinterpret_cast<uint32 *>(triggerPointer + 1);
            uint8 *signalUInt8Pointer = reinterpret_cast<uint8 *>(timerPointer + 1);
            uint16 *signalUInt16Pointer = reinterpret_cast<uint16 *>(signalUInt8Pointer + 1);
            uint32 *signalUInt32Pointer = reinterpret_cast<uint32 *>(signalUInt16Pointer + 1);
            uint64 *signalUInt64Pointer = reinterpret_cast<uint64 *>(signalUInt32Pointer + 1);
            int8 *signalInt8Pointer = reinterpret_cast<int8 *>(signalUInt64Pointer + 1);
            int16 *signalInt16Pointer = reinterpret_cast<int16 *>(signalInt8Pointer + 1);
            int32 *signalInt32Pointer = reinterpret_cast<int32 *>(signalInt16Pointer + 1);
            int64 *signalInt64Pointer = reinterpret_cast<int64 *>(signalInt32Pointer + 1);
            float32 *signalFloat32Pointer = reinterpret_cast<float32 *>(signalInt64Pointer + 1);
            float64 *signalFloat64Pointer = reinterpret_cast<float64 *>(signalFloat32Pointer + 1);

            *triggerPointer = 0u;
            *timerPointer = static_cast<uint32>(period * 1e6) * n;
            *signalUInt8Pointer = signalToGenerate[n];
            *signalUInt16Pointer = signalToGenerate[n];
            *signalUInt32Pointer = signalToGenerate[n];
            *signalUInt64Pointer = signalToGenerate[n];
            int32 multiplier = -1;
            if ((n % 2) == 0) {
                multiplier = 1;
            }
            *signalInt8Pointer = multiplier * signalToGenerate[n];
            *signalInt16Pointer = multiplier * signalToGenerate[n];
            *signalInt32Pointer = multiplier * signalToGenerate[n];
            *signalInt64Pointer = static_cast<int64>(multiplier) * signalToGenerate[n];
            *signalFloat32Pointer = static_cast<float32>(multiplier) * signalToGenerate[n];
            *signalFloat64Pointer = static_cast<float64>(multiplier) * signalToGenerate[n];
        }
    }

    bool ok = TestIntegratedExecution(config1, signalToGenerate, numberOfElements, NULL, 1u, numberOfBuffers, 0, 0, period, filename, expectedFileContent, csv);
    if (!csv) {
        if (expectedFileContent != NULL) {
            char8 *mem = const_cast<char8 *>(&expectedFileContent[0]);
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(mem));
        }
    }
    return ok;
}

bool FileReaderTest::TestIntegratedInApplication_NoTrigger_Array(const MARTe::char8 *filename, bool csv) {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    const uint32 N_OF_SIGNALS = 12;
    const char8 *signalNames[N_OF_SIGNALS] = {"Trigger", "Time", "SignalUInt8", "SignalUInt16", "SignalUInt32", "SignalUInt64", "SignalInt8", "SignalInt16",
        "SignalInt32", "SignalInt64", "SignalFloat32", "SignalFloat64WhichIsAlsoAVeryLon"};
    const uint16 signalTypes[N_OF_SIGNALS] = {UnsignedInteger8Bit.all, UnsignedInteger32Bit.all, UnsignedInteger8Bit.all, UnsignedInteger16Bit.all,
        UnsignedInteger32Bit.all, UnsignedInteger64Bit.all, SignedInteger8Bit.all, SignedInteger16Bit.all, SignedInteger32Bit.all, SignedInteger64Bit.all,
        Float32Bit.all, Float64Bit.all};
    const uint32 signalElements[N_OF_SIGNALS] = {1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
    const uint32 numberOfBuffers = 16;
    const float32 period = 2;
    const char8 * expectedFileContent = NULL;
    const uint32 ARRAY_SIZE = 2u;

    if (csv) {
        expectedFileContent =
        ""
        "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[2];SignalUInt16 (uint16)[2];SignalUInt32 (uint32)[2];SignalUInt64 (uint64)[2];SignalInt8 (int8)[2];SignalInt16 (int16)[2];SignalInt32 (int32)[2];SignalInt64 (int64)[2];SignalFloat32 (float32)[2];SignalFloat64WhichIsAlsoAVeryLongSignalNameSoThatItHasMoreThan32CharsAndIsHopefullyTruncated (float64)[2]\n"
        "0;0;{ 1 1 } ;{ 1 1 } ;{ 1 1 } ;{ 1 1 } ;{ 1 1 } ;{ 1 1 } ;{ 1 1 } ;{ 1 1 } ;{ 1.000000 1.000000 } ;{ 1.000000 1.000000 } \n"
        "0;4000000;{ 2 2 } ;{ 2 2 } ;{ 2 2 } ;{ 2 2 } ;{ -2 -2 } ;{ -2 -2 } ;{ -2 -2 } ;{ -2 -2 } ;{ -2.000000 -2.000000 } ;{ -2.000000 -2.000000 } \n"
        "0;8000000;{ 3 3 } ;{ 3 3 } ;{ 3 3 } ;{ 3 3 } ;{ 3 3 } ;{ 3 3 } ;{ 3 3 } ;{ 3 3 } ;{ 3.000000 3.000000 } ;{ 3.000000 3.000000 } \n"
        "0;12000000;{ 4 4 } ;{ 4 4 } ;{ 4 4 } ;{ 4 4 } ;{ -4 -4 } ;{ -4 -4 } ;{ -4 -4 } ;{ -4 -4 } ;{ -4.000000 -4.000000 } ;{ -4.000000 -4.000000 } \n"
        "0;16000000;{ 5 5 } ;{ 5 5 } ;{ 5 5 } ;{ 5 5 } ;{ 5 5 } ;{ 5 5 } ;{ 5 5 } ;{ 5 5 } ;{ 5.000000 5.000000 } ;{ 5.000000 5.000000 } \n";
    }
    else {
        uint32 writeSize = sizeof(uint8); //trigger
        writeSize += sizeof(uint32);//time
        writeSize += sizeof(uint8) * ARRAY_SIZE;//signalUInt8
        writeSize += sizeof(uint16) * ARRAY_SIZE;//signalUInt16
        writeSize += sizeof(uint32) * ARRAY_SIZE;//signalUInt32
        writeSize += sizeof(uint64) * ARRAY_SIZE;//signalUInt64
        writeSize += sizeof(int8) * ARRAY_SIZE;//signalInt8
        writeSize += sizeof(int16) * ARRAY_SIZE;//signalInt16
        writeSize += sizeof(int32) * ARRAY_SIZE;//signalInt32
        writeSize += sizeof(int64) * ARRAY_SIZE;//signalInt64
        writeSize += sizeof(float32) * ARRAY_SIZE;//signalFloat32
        writeSize += sizeof(float64) * ARRAY_SIZE;//signalFloat64

        const uint32 SIGNAL_NAME_SIZE = 32;
        uint32 headerSize = sizeof(uint32) + N_OF_SIGNALS * (sizeof(uint16) + SIGNAL_NAME_SIZE + sizeof(uint32));
        uint32 memorySize = headerSize + (numberOfElements * writeSize);//5 writes
        expectedFileContent = static_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(memorySize));

        uint32 n;
        //Header
        char8 *header = const_cast<char8 *>(&expectedFileContent[0]);
        MemoryOperationsHelper::Copy(header, reinterpret_cast<const char8 *>(&N_OF_SIGNALS), sizeof(uint32));
        header += sizeof(uint32);
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            MemoryOperationsHelper::Copy(header, &signalTypes[n], sizeof(uint16));
            header += sizeof(uint16);
            MemoryOperationsHelper::Set(header, '\0', SIGNAL_NAME_SIZE);
            MemoryOperationsHelper::Copy(header, signalNames[n], StringHelper::Length(signalNames[n]));
            header += SIGNAL_NAME_SIZE;
            MemoryOperationsHelper::Copy(header, &signalElements[n], sizeof(uint32));
            header += sizeof(uint32);
        }

        for (n = 0u; n < numberOfElements; n++) {
            uint8 *triggerPointer = const_cast<uint8 *>(reinterpret_cast<const uint8 *>(&expectedFileContent[headerSize + (n * writeSize)]));
            uint32 *timerPointer = reinterpret_cast<uint32 *>(triggerPointer + 1);
            uint8 *signalUInt8Pointer = reinterpret_cast<uint8 *>(timerPointer + 1);
            uint16 *signalUInt16Pointer = reinterpret_cast<uint16 *>(signalUInt8Pointer + ARRAY_SIZE);
            uint32 *signalUInt32Pointer = reinterpret_cast<uint32 *>(signalUInt16Pointer + ARRAY_SIZE);
            uint64 *signalUInt64Pointer = reinterpret_cast<uint64 *>(signalUInt32Pointer + ARRAY_SIZE);
            int8 *signalInt8Pointer = reinterpret_cast<int8 *>(signalUInt64Pointer + ARRAY_SIZE);
            int16 *signalInt16Pointer = reinterpret_cast<int16 *>(signalInt8Pointer + ARRAY_SIZE);
            int32 *signalInt32Pointer = reinterpret_cast<int32 *>(signalInt16Pointer + ARRAY_SIZE);
            int64 *signalInt64Pointer = reinterpret_cast<int64 *>(signalInt32Pointer + ARRAY_SIZE);
            float32 *signalFloat32Pointer = reinterpret_cast<float32 *>(signalInt64Pointer + ARRAY_SIZE);
            float64 *signalFloat64Pointer = reinterpret_cast<float64 *>(signalFloat32Pointer + ARRAY_SIZE);

            *triggerPointer = 0u;
            *timerPointer = static_cast<uint32>(period * 1e6) * n * ARRAY_SIZE;

            int32 multiplier = -1;
            if ((n % 2) == 0) {
                multiplier = 1;
            }

            uint32 i;
            for (i = 0; i < ARRAY_SIZE; i++) {
                *(signalUInt8Pointer + i) = signalToGenerate[n];
                *(signalUInt16Pointer + i) = signalToGenerate[n];
                *(signalUInt32Pointer + i) = signalToGenerate[n];
                *(signalUInt64Pointer + i) = signalToGenerate[n];
                *(signalInt8Pointer + i) = static_cast<int8>(multiplier) * signalToGenerate[n];
                *(signalInt16Pointer + i) = static_cast<int16>(multiplier) * signalToGenerate[n];
                *(signalInt32Pointer + i) = static_cast<int32>(multiplier) * signalToGenerate[n];
                *(signalInt64Pointer + i) = static_cast<int64>(multiplier) * signalToGenerate[n];
                *(signalFloat32Pointer + i) = static_cast<float32>(multiplier) * signalToGenerate[n];
                *(signalFloat64Pointer + i) = static_cast<float64>(multiplier) * signalToGenerate[n];
            }
        }
    }

    bool ok = TestIntegratedExecution(config1, signalToGenerate, numberOfElements, NULL, ARRAY_SIZE, numberOfBuffers, 0, 0, period, filename,
            expectedFileContent, csv);
    if (!csv) {
        if (expectedFileContent != NULL) {
            char8 *mem = const_cast<char8 *>(&expectedFileContent[0]);
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(mem));
        }
    }
    return ok;
}

bool FileReaderTest::TestIntegratedInApplication_Trigger(const MARTe::char8 *filename, bool csv) {
    using namespace MARTe;
    uint32 signalToGenerate[] = {1, 2, 3, 4, 5};
    uint8 triggerToGenerate[] = {0, 1, 0, 1, 1};
    uint32 numberOfElements = sizeof(signalToGenerate) / sizeof(uint32);
    const uint32 N_OF_SIGNALS = 12;
    const char8 *signalNames[N_OF_SIGNALS] = {"Trigger", "Time", "SignalUInt8", "SignalUInt16", "SignalUInt32", "SignalUInt64", "SignalInt8", "SignalInt16",
        "SignalInt32", "SignalInt64", "SignalFloat32", "SignalFloat64WhichIsAlsoAVeryLon"};
    const uint16 signalTypes[N_OF_SIGNALS] = {UnsignedInteger8Bit.all, UnsignedInteger32Bit.all, UnsignedInteger8Bit.all, UnsignedInteger16Bit.all,
        UnsignedInteger32Bit.all, UnsignedInteger64Bit.all, SignedInteger8Bit.all, SignedInteger16Bit.all, SignedInteger32Bit.all, SignedInteger64Bit.all,
        Float32Bit.all, Float64Bit.all};
    const uint32 signalElements[N_OF_SIGNALS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    const uint32 numberOfBuffers = 16;
    const float32 period = 2;
    const char8 * expectedFileContent = NULL;
    if (csv) {
        expectedFileContent =
        ""
        "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLongSignalNameSoThatItHasMoreThan32CharsAndIsHopefullyTruncated (float64)[1]\n"
        "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
        "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
        "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    }
    else {
        uint32 writeSize = sizeof(uint8); //trigger
        writeSize += sizeof(uint32);//time
        writeSize += sizeof(uint8);//signalUInt8
        writeSize += sizeof(uint16);//signalUInt16
        writeSize += sizeof(uint32);//signalUInt32
        writeSize += sizeof(uint64);//signalUInt64
        writeSize += sizeof(int8);//signalInt8
        writeSize += sizeof(int16);//signalInt16
        writeSize += sizeof(int32);//signalInt32
        writeSize += sizeof(int64);//signalInt64
        writeSize += sizeof(float32);//signalFloat32
        writeSize += sizeof(float64);//signalFloat64

        const uint32 SIGNAL_NAME_SIZE = 32;
        uint32 headerSize = sizeof(uint32) + N_OF_SIGNALS * (sizeof(uint16) + SIGNAL_NAME_SIZE + sizeof(uint32));
        uint32 memorySize = headerSize + (numberOfElements * writeSize);//3 writes (as per trigger)
        expectedFileContent = static_cast<char8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(memorySize));

        uint32 n;
        //Header
        char8 *header = const_cast<char8 *>(&expectedFileContent[0]);
        MemoryOperationsHelper::Copy(header, reinterpret_cast<const char8 *>(&N_OF_SIGNALS), sizeof(uint32));
        header += sizeof(uint32);
        for (n = 0u; n < N_OF_SIGNALS; n++) {
            MemoryOperationsHelper::Copy(header, &signalTypes[n], sizeof(uint16));
            header += sizeof(uint16);
            MemoryOperationsHelper::Set(header, '\0', SIGNAL_NAME_SIZE);
            MemoryOperationsHelper::Copy(header, signalNames[n], StringHelper::Length(signalNames[n]));
            header += SIGNAL_NAME_SIZE;
            MemoryOperationsHelper::Copy(header, &signalElements[n], sizeof(uint32));
            header += sizeof(uint32);
        }

        uint32 z = 0u;
        for (n = 0u; n < numberOfElements; n++) {
            if (triggerToGenerate[n] != 1) {
                continue;
            }
            uint8 *triggerPointer = const_cast<uint8 *>(reinterpret_cast<const uint8 *>(&expectedFileContent[headerSize + (z * writeSize)]));
            z++;

            uint32 *timerPointer = reinterpret_cast<uint32 *>(triggerPointer + 1);
            uint8 *signalUInt8Pointer = reinterpret_cast<uint8 *>(timerPointer + 1);
            uint16 *signalUInt16Pointer = reinterpret_cast<uint16 *>(signalUInt8Pointer + 1);
            uint32 *signalUInt32Pointer = reinterpret_cast<uint32 *>(signalUInt16Pointer + 1);
            uint64 *signalUInt64Pointer = reinterpret_cast<uint64 *>(signalUInt32Pointer + 1);
            int8 *signalInt8Pointer = reinterpret_cast<int8 *>(signalUInt64Pointer + 1);
            int16 *signalInt16Pointer = reinterpret_cast<int16 *>(signalInt8Pointer + 1);
            int32 *signalInt32Pointer = reinterpret_cast<int32 *>(signalInt16Pointer + 1);
            int64 *signalInt64Pointer = reinterpret_cast<int64 *>(signalInt32Pointer + 1);
            float32 *signalFloat32Pointer = reinterpret_cast<float32 *>(signalInt64Pointer + 1);
            float64 *signalFloat64Pointer = reinterpret_cast<float64 *>(signalFloat32Pointer + 1);

            *triggerPointer = 1u;
            *timerPointer = static_cast<uint32>(period * 1e6) * n;
            *signalUInt8Pointer = signalToGenerate[n];
            *signalUInt16Pointer = signalToGenerate[n];
            *signalUInt32Pointer = signalToGenerate[n];
            *signalUInt64Pointer = signalToGenerate[n];
            int32 multiplier = -1;
            if ((n % 2) == 0) {
                multiplier = 1;
            }
            *signalInt8Pointer = multiplier * signalToGenerate[n];
            *signalInt16Pointer = multiplier * signalToGenerate[n];
            *signalInt32Pointer = multiplier * signalToGenerate[n];
            *signalInt64Pointer = static_cast<int64>(multiplier) * signalToGenerate[n];
            *signalFloat32Pointer = static_cast<float32>(multiplier) * signalToGenerate[n];
            *signalFloat64Pointer = static_cast<float64>(multiplier) * signalToGenerate[n];
        }
    }

    bool ok = TestIntegratedExecution(config1, signalToGenerate, numberOfElements, triggerToGenerate, 1u, numberOfBuffers, 0, 0, period, filename,
            expectedFileContent, csv);
    if (!csv) {
        if (expectedFileContent != NULL) {
            char8 *mem = const_cast<char8 *>(&expectedFileContent[0]);
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(mem));
        }
    }
    return ok;
}

bool FileReaderTest::TestOpenFile() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config5, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

//Get the current pulse number
    ReferenceT<FileReader> fileReader;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    ReferenceT<Message> messageOpenFile = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageOpenFile");
    if (ok) {
        ok = messageOpenFile.IsValid();
    }
    if (ok) {
        MessageI::SendMessage(messageOpenFile, NULL);
    }
    ReferenceT<Message> messageFlushFile = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageFlushFile");
    if (ok) {
        ok = messageOpenFile.IsValid();
    }
    if (ok) {
        MessageI::SendMessage(messageFlushFile, NULL);
    }
    ReferenceT<Message> messageCloseFile = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageCloseFile");
    if (ok) {
        ok = messageCloseFile.IsValid();
    }
    if (ok) {
        MessageI::SendMessage(messageCloseFile, NULL);
    }
    StreamString testFilename = "Test_MessageOpenTree.bin";
    if (ok) {
        ok = (fileReader->GetFilename() == testFilename);
    }
    if (ok) {
        Directory toDelete(testFilename.Buffer());
        toDelete.Delete();
    }
    godb->Purge();
    return ok;
}

bool FileReaderTest::TestOpenFile_Overwrite() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config13, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

//Get the current pulse number
    ReferenceT<FileReader> fileReader;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    ReferenceT<Message> messageOpenFile = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageOpenFile");
    if (ok) {
        ok = messageOpenFile.IsValid();
    }
    if (ok) {
        MessageI::SendMessage(messageOpenFile, NULL);
    }
    ReferenceT<Message> messageFlushFile = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageFlushFile");
    if (ok) {
        ok = messageOpenFile.IsValid();
    }
    if (ok) {
        MessageI::SendMessage(messageFlushFile, NULL);
    }
    ReferenceT<Message> messageCloseFile = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageCloseFile");
    if (ok) {
        ok = messageCloseFile.IsValid();
    }
    if (ok) {
        MessageI::SendMessage(messageCloseFile, NULL);
    }

    StreamString testFilename = "Test_MessageOpenTree.bin";
    if (ok) {
        ok = !(fileReader->OpenFile(testFilename));
    }
    if (ok) {
        Directory toDelete(testFilename.Buffer());
        toDelete.Delete();
    }
    godb->Purge();
    return ok;
}

bool FileReaderTest::TestCloseFile() {
    return TestOpenFile();
}

bool FileReaderTest::TestFlushFile() {
    return TestOpenFile();
}

bool FileReaderTest::TestGetCPUMask() {
    return TestInitialise();
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

bool FileReaderTest::TestGetNumberOfBuffers() {
    return TestInitialise();
}

bool FileReaderTest::TestGetNumberOfPostTriggers() {
    return TestInitialise();
}

bool FileReaderTest::TestGetNumberOfPreTriggers() {
    return TestInitialise();
}

bool FileReaderTest::TestGetStackSize() {
    return TestInitialise();
}

bool FileReaderTest::TestIsStoreOnTrigger() {
    return TestInitialise();
}

bool FileReaderTest::TestOpenFileOKMessage() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config6, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }
    if (ok) {
        ok = (helper->openOKFunctionCalled);
    }
    if (ok) {
        StreamString testFilename = "Test_MessageOpenTree_2.bin";
        if (ok) {
            Directory toDelete(testFilename.Buffer());
            toDelete.Delete();
        }
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestCloseFileMessage() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config6, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }
    if (ok) {
        fileReader->CloseFile();
    }
    if (ok) {
        ok = (helper->closeFunctionCalled);
    }
    if (ok) {
        StreamString testFilename = "Test_MessageOpenTree_2.bin";
        if (ok) {
            Directory toDelete(testFilename.Buffer());
            toDelete.Delete();
        }
    }
    godb->Purge();
    return ok;
}

bool FileReaderTest::TestRuntimeErrorMessage() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config5, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }
    if (ok) {
        fileReader->Synchronise();
    }
    if (ok) {
        ok = (helper->runtimeErrorFunctionCalled);
    }
    if (ok) {
        StreamString testFilename = "Test_MessageOpenTree_2.bin";
        if (ok) {
            Directory toDelete(testFilename.Buffer());
            toDelete.Delete();
        }
    }
    godb->Purge();
    return ok;
}

bool FileReaderTest::TestOpenFileFailMessage() {
    using namespace MARTe;
    TestIntegratedInApplication(config7, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    fileReader = godb->Find("Test.Data.Drv1");
    bool ok = fileReader.IsValid();

    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }
    if (ok) {
        ok = (helper->openFailFunctionCalled);
    }
    godb->Purge();
    return ok;
}

bool FileReaderTest::TestOpenFileOKMessage_Fail() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config8, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestCloseFileMessage_Fail() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config9, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }
    if (ok) {
        fileReader->CloseFile();
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestRuntimeErrorMessage_Fail() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config9, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    if (ok) {
        fileReader = godb->Find("Test.Data.Drv1");
        ok = fileReader.IsValid();
    }
    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }
    if (ok) {
        fileReader->Synchronise();
        fileReader->CloseFile();
    }
    if (ok) {
        StreamString testFilename = "Test_MessageOpenTree_2.bin";
        if (ok) {
            Directory toDelete(testFilename.Buffer());
            toDelete.Delete();
        }
    }
    godb->Purge();
    return ok;
}

bool FileReaderTest::TestOpenFileFailMessage_Fail() {
    using namespace MARTe;
    TestIntegratedInApplication(config10, false);
    ObjectRegistryDatabase *godb = ObjectRegistryDatabase::Instance();

    ReferenceT<FileReader> fileReader;
    ReferenceT<FileReaderTestHelper> helper;
    fileReader = godb->Find("Test.Data.Drv1");
    bool ok = fileReader.IsValid();

    if (ok) {
        helper = godb->Find("FileReaderTestHelper");
        ok = helper.IsValid();
    }
    if (ok) {
        fileReader->CloseFile();
    }

    godb->Purge();
    return ok;
}

bool FileReaderTest::TestInvalidMessageName() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config11, true);
    return !ok;
}

bool FileReaderTest::TestInvalidMessageType() {
    using namespace MARTe;
    bool ok = TestIntegratedInApplication(config12, true);
    return !ok;
}
#endif
