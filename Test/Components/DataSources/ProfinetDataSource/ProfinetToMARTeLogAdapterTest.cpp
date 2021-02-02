/**
 * @file ProfinetToMARTeLogAdapterTest.cpp
 * @brief Source file for class  ProfinetToMARTeLogAdapterTest.
 * @date 21/01/2021
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class ProfinetToMARTeLogAdapterTest (public, protected, and private). 
 * Be aware that some methods, such as those inline could be defined on the
 * header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ReferenceT.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ProfinetToMARTeLogAdapter.h"
#include "ProfinetToMARTeLogAdapterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * Maximum Logger message size.
 */
static const MARTe::uint64 PROFINET_TEST_MAX_LOGGER_SIZE = 200llu;

/**
 * Current error message received on the Logger.
 */
static MARTe::StreamString currentLoggerMessage = MARTe::StreamString();

/**
 * Callback used to get the error message received on the Logger.
 */
static void TestGetProfinetLoggerOutput(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    currentLoggerMessage = errorDescription;
}

class ProfinetToMARTeLogAdapterTestHelper: public MARTe::ProfinetToMARTeLogAdapter, public MARTe::Object  {
public:
    CLASS_REGISTER_DECLARATION()ProfinetToMARTeLogAdapterTestHelper() {
        SetName("ProfinetToMARTeLogAdapterTestHelper.0");
    }

    bool TestLoggerMinimumLevel(ProfinetToMARTeLogAdapter *aTestLogger, ProfinetDataSourceDriver::log_adapter_level_t aMinimumLevel = ProfinetDataSourceDriver::LogLevel_Debug) {
        bool ok = true;
        aTestLogger = new ProfinetToMARTeLogAdapter(aMinimumLevel);
        ok &= (aTestLogger != NULL_PTR(ProfinetToMARTeLogAdapter *));
        ok &= (aTestLogger->GetMinimumLoggerLevel() == aMinimumLevel);
        if (aTestLogger != NULL_PTR(ProfinetToMARTeLogAdapter *)){
            delete aTestLogger;
            aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
        }
        return ok;
    }

    bool TestLoggerPerLevel(ProfinetToMARTeLogAdapter *aTestLogger,
                            ProfinetDataSourceDriver::log_adapter_level_t aMinimumLevel = ProfinetDataSourceDriver::LogLevel_Debug,
                            ProfinetDataSourceDriver::log_adapter_level_t aGivenLevel = ProfinetDataSourceDriver::LogLevel_Debug,
                            MARTe::StreamString aLoggerMessage = "ProfinetToMARTeLogAdapterTest Default Test Message") {
        bool ok = true;
        MARTe::StreamString tempLoggerMessage = aLoggerMessage;
        MARTe::StreamString readLoggerMessage = MARTe::StreamString();
        MARTe::uint64 messageSize = 0llu;
        messageSize = aLoggerMessage.Size();
        MARTe::uint32 readSize = 0u;
        MARTe::char8 readBuffer[PROFINET_TEST_MAX_LOGGER_SIZE + 1] = {};
        aTestLogger = new ProfinetToMARTeLogAdapter(aMinimumLevel);
        ok &= (messageSize > 0u);
        if (ok){
            if (messageSize > PROFINET_TEST_MAX_LOGGER_SIZE) {
                messageSize = PROFINET_TEST_MAX_LOGGER_SIZE;
            }
            readSize = MARTe::uint32(messageSize);
        }
        ok &= tempLoggerMessage.SetSize(messageSize);
        ok &= currentLoggerMessage.SetSize(0);
        if (ok) {
            aTestLogger->Log(aGivenLevel, aLoggerMessage.Buffer());
        }
        ok &= currentLoggerMessage.Seek(0);
        ok &= currentLoggerMessage.Read(readBuffer, readSize);
        if (ok) {
            readBuffer[PROFINET_TEST_MAX_LOGGER_SIZE] = '\0';
            readLoggerMessage = readBuffer; 
        }
        ok &= readLoggerMessage.SetSize(messageSize);
        ok &= (readLoggerMessage == tempLoggerMessage);
        if(aTestLogger != NULL_PTR(ProfinetToMARTeLogAdapter *)) {
            delete aTestLogger;
            aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
        }
        return ok;
    }
};
CLASS_REGISTER(ProfinetToMARTeLogAdapterTestHelper, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ProfinetToMARTeLogAdapterTest::TestConstructor() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    ok &= testHelper->TestLoggerMinimumLevel(aTestLogger);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMinimumLevels() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    ok &= testHelper->TestLoggerMinimumLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Debug);
    ok &= testHelper->TestLoggerMinimumLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Info);
    ok &= testHelper->TestLoggerMinimumLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Warning);
    ok &= testHelper->TestLoggerMinimumLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Error);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageZeroSize() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    MARTe::StreamString variableLoggerMessage;
    ok &= variableLoggerMessage.SetSize(0);
    ok &= !testHelper->TestLoggerPerLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Debug, ProfinetDataSourceDriver::LogLevel_Debug, variableLoggerMessage);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageAverageSize() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    MARTe::StreamString variableLoggerMessage;
    ok &= variableLoggerMessage.SetSize(0);
    variableLoggerMessage.Printf("%s", "ProfinetToMARTeLogAdapterTest::TestLoggerMessage LogLevel_Debug testing message with 100 characters.");
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Debug, ProfinetDataSourceDriver::LogLevel_Debug, variableLoggerMessage);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageMaximumSize() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    MARTe::StreamString variableLoggerMessage;
    ok &= variableLoggerMessage.SetSize(0);
    variableLoggerMessage.Printf("%s", "ProfinetToMARTeLogAdapterTest::TestLoggerMessage LogLevel_Debug testing message with 100 characters.");
    variableLoggerMessage.Printf("%s", "ProfinetToMARTeLogAdapterTest::TestLoggerMessage LogLevel_Debug testing message with 100 characters.");
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Debug, ProfinetDataSourceDriver::LogLevel_Debug, variableLoggerMessage);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageOverrunSize() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    MARTe::StreamString variableLoggerMessage;
    ok &= variableLoggerMessage.SetSize(0);
    variableLoggerMessage.Printf("%s", "ProfinetToMARTeLogAdapterTest::TestLoggerMessage LogLevel_Debug testing message with 100 characters.");
    variableLoggerMessage.Printf("%s", "ProfinetToMARTeLogAdapterTest::TestLoggerMessage LogLevel_Debug testing message with 100 characters.");
    variableLoggerMessage.Printf("%s", "ProfinetToMARTeLogAdapterTest::TestLoggerMessage LogLevel_Debug testing message with 100 characters.");
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, ProfinetDataSourceDriver::LogLevel_Debug, ProfinetDataSourceDriver::LogLevel_Debug, variableLoggerMessage);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageDebugLevel() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    ProfinetDataSourceDriver::log_adapter_level_t aMinimumLevel = ProfinetDataSourceDriver::LogLevel_Debug;
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Debug);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Info);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Warning);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Error);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageInformationLevel() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    ProfinetDataSourceDriver::log_adapter_level_t aMinimumLevel = ProfinetDataSourceDriver::LogLevel_Info;
    ok &= !testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Debug);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Info);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Warning);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Error);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageWarningLevel() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    ProfinetDataSourceDriver::log_adapter_level_t aMinimumLevel = ProfinetDataSourceDriver::LogLevel_Warning;
    ok &= !testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Debug);
    ok &= !testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Info);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Warning);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Error);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}

bool ProfinetToMARTeLogAdapterTest::TestLoggerMessageErrorLevel() {
    using namespace MARTe;
    bool ok = true;
    MARTe::ReferenceT<ProfinetToMARTeLogAdapterTestHelper> testHelper(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ErrorManagement::ErrorProcessFunctionType currentMessageProcessFunction = ErrorManagement::errorMessageProcessFunction;
    SetErrorProcessFunction(&TestGetProfinetLoggerOutput);
    ProfinetToMARTeLogAdapter *aTestLogger = NULL_PTR(ProfinetToMARTeLogAdapter *);
    ProfinetDataSourceDriver::log_adapter_level_t aMinimumLevel = ProfinetDataSourceDriver::LogLevel_Error;
    ok &= !testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Debug);
    ok &= !testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Info);
    ok &= !testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Warning);
    ok &= testHelper->TestLoggerPerLevel(aTestLogger, aMinimumLevel, ProfinetDataSourceDriver::LogLevel_Error);
    SetErrorProcessFunction(currentMessageProcessFunction);
    return ok;
}
