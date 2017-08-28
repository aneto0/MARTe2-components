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
                printf("%d %d: %x vs %x\n", s, z, (uint32)signalToVerify[s]->type ## Signal[z], (uint32)gam->type ## Signal[z]);\
            }\
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

    void Printf(MARTe::File &f, MARTe::TypeDescriptor t, MARTe::uint32 e) {
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

static void GenerateFile(const MARTe::char8 * const filename, const MARTe::char8 * const fileContent) {
    using namespace MARTe;
    File f;
    f.Open(filename, BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    f.Printf("%s", fileContent);
    f.Flush();
    f.Close();
}

static void GenerateFile(const MARTe::char8 * const filename) {
    using namespace MARTe;
    const char8 * fileContent =
            ""
                    "#Trigger (uint8)[1];Time (uint32)[1];SignalUInt8 (uint8)[1];SignalUInt16 (uint16)[1];SignalUInt32 (uint32)[1];SignalUInt64 (uint64)[1];SignalInt8 (int8)[1];SignalInt16 (int16)[1];SignalInt32 (int32)[1];SignalInt64 (int64)[1];SignalFloat32 (float32)[1];SignalFloat64WhichIsAlsoAVeryLongSignalNameSoThatItHasMoreThan32CharsAndIsHopefullyTruncated (float64)[1]\n"
                    "1;2000000;2;2;2;2;-2;-2;-2;-2;-2.000000;-2.000000\n"
                    "1;6000000;4;4;4;4;-4;-4;-4;-4;-4.000000;-4.000000\n"
                    "1;8000000;5;5;5;5;5;5;5;5;5.000000;5.000000\n";
    GenerateFile(filename, fileContent);
}

static void DeleteTestFile(const MARTe::char8 * const filename) {
    using namespace MARTe;
    Directory toDelete(filename);
    toDelete.Delete();
}

static bool TestIntegratedExecution(const MARTe::char8 * const config, const MARTe::char8 * const filename, FRTSignalToVerify **signalToVerify,
                                    MARTe::uint32 *signalToVerifyNumberOfElements, MARTe::uint32 signalToVerifyNumberOfSamples, bool csv,
                                    MARTe::uint32 interpolationPeriod) {
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
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }
    god->Purge();
    return ok;
}

static void GenerateCSVFile(const MARTe::char8 * const filename, const MARTe::char8 * const csvSeparator, FRTSignalToVerify **signalToVerify,
                            MARTe::uint32 *signalToVerifyNumberOfElements, MARTe::uint32 signalToVerifyNumberOfSamples) {
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

static void GenerateBinaryFile(const MARTe::char8 * const filename, FRTSignalToVerify **signalToVerify, MARTe::uint32 *signalToVerifyNumberOfElements,
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
                f.Write(reinterpret_cast<const char8 *>(signalToVerify[s]->signalPtrs[n]), writeSize);
            }
        }
    }
    f.Flush();
    f.Close();
}

static bool TestIntegratedExecution(const MARTe::char8 * const config, bool csv, MARTe::uint32 *numberOfElements) {
    using namespace MARTe;
    const char8 * filename = "";
    bool ok = true;
    const uint32 signalToVerifyNumberOfSamples = 3u;
    FRTSignalToVerify **signals = new FRTSignalToVerify*[signalToVerifyNumberOfSamples];
    uint32 i;
    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        signals[i] = new FRTSignalToVerify(numberOfElements, i + 1);
    }

    if (csv) {
        filename = "TestIntegratedExecution.csv";
        GenerateCSVFile(filename, ";", signals, numberOfElements, signalToVerifyNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, true, 0);
        }
    }
    else {
        filename = "TestIntegratedExecution.bin";
        GenerateBinaryFile(filename, signals, numberOfElements, signalToVerifyNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signals, numberOfElements, signalToVerifyNumberOfSamples, false, 0);
        }
    }

    for (i = 0; i < signalToVerifyNumberOfSamples; i++) {
        delete signals[i];
    }
    delete signals;
    DeleteTestFile(filename);

    return ok;
}

static bool TestIntegratedExecutionInterpolation(const MARTe::char8 * const config, bool csv, MARTe::uint32 *numberOfElements) {
    using namespace MARTe;
    const char8 * filename = "";
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
            ok = TestIntegratedExecution(config, filename, signalsToVerify, numberOfElements, signalToVerifyNumberOfSamples, true, interpolationPeriod);
        }
    }
    else {
        filename = "TestIntegratedExecutionInterpolation.bin";
        GenerateBinaryFile(filename, signalsFile, numberOfElements, signalToGenerateNumberOfSamples);
        if (ok) {
            ok = TestIntegratedExecution(config, filename, signalsToVerify, numberOfElements, signalToVerifyNumberOfSamples, false, interpolationPeriod);
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
        "            XAxisSignal = SignalUInt32"
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

//Wrong configuration with samples
static const MARTe::char8 * const config2 = ""
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
        "            XAxisSignal = SignalUInt32"
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
static const MARTe::char8 * const config3 = ""
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

#if 0
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
        uint32 numberOfElements[] = { 1, 1, 1, 3, 1, 1, 1, 1, 10, 1 };
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0]);
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
    const char8 * const filename = "FileReaderTest_Test.csv";
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
    const char8 * const filename = "FileReaderTest_Test.csv";
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
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0]);
    }
    return ok;
}

bool FileReaderTest::TestGetOutputBrokers() {
    using namespace MARTe;
    FileReader test;
    ReferenceContainer rc;
    return !test.GetInputBrokers(rc, "", NULL);
}

bool FileReaderTest::TestSynchronise() {
    using namespace MARTe;
    bool ok = true;
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecution(config1, true, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 3, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecution(config1, false, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 3, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecution(config1, true, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecutionInterpolation(config1, false, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecutionInterpolation(config1, true, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        ok = TestIntegratedExecutionInterpolation(config1, false, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecutionInterpolation(config1, false, &numberOfElements[0]);
    }
    if (ok) {
        uint32 numberOfElements[] = { 2, 4, 5, 2, 1, 4, 3, 2, 4, 2 };
        ok = TestIntegratedExecutionInterpolation(config1, true, &numberOfElements[0]);
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
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
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
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
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
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
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
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
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
    const char8 * const filename = "FileReaderTest_TestInitialise.bin";
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
    const char8 * const filename = "doesnotexist.csv";
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
    const char8 * const filename = "corrupted.csv";
    const char8 * fileContent =
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
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
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
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
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
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
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

bool FileReaderTest::TestInitialise_False_XAxisSignal() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "yes");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("FileFormat", "csv");
    cdb.Write("InterpolationPeriod", 100);
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestInitialise_False_InterpolationPeriod() {
    using namespace MARTe;
    FileReader test;
    ConfigurationDatabase cdb;
    const char8 * const filename = "FileReaderTest_TestInitialise.csv";
    GenerateFile(filename);
    cdb.Write("Filename", filename);
    cdb.Write("Interpolate", "yes");
    cdb.Write("CSVSeparator", ";");
    cdb.Write("FileFormat", "csv");
    cdb.Write("XAxisSignal", "SignalUint8");
    cdb.MoveToRoot();
    bool ok = test.Initialise(cdb);
    DeleteTestFile(filename);
    return !ok;
}

bool FileReaderTest::TestSetConfiguredDatabase() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1, false, &numberOfElements[0]);
}

bool FileReaderTest::TestSetConfiguredDatabase_False_NumberOfSamples() {
    using namespace MARTe;
    const char8 * const filename = "config2.csv";
    GenerateFile(filename);
    bool ok = !TestIntegratedInApplication(config2, true);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestSetConfiguredDatabase_False_MoreThanOneFunction() {
    using namespace MARTe;
    const char8 * const filename = "config3.csv";
    GenerateFile(filename);
    bool ok = !TestIntegratedInApplication(config3, true);
    DeleteTestFile(filename);
    return ok;
}

bool FileReaderTest::TestOpenFile() {
    using namespace MARTe;
    uint32 numberOfElements[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return TestIntegratedExecution(config1, false, &numberOfElements[0]);
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

#if 0
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
