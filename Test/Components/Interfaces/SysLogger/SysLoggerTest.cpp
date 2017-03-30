/**
 * @file SysLoggerTest.cpp
 * @brief Source file for class SysLoggerTest
 * @date 17/03/2017
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
 * the class SysLoggerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "LoggerService.h"
#include "ReferenceT.h"
#include "SysLogger.h"
#include "SysLoggerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SysLoggerTest::TestConstructor() {
    using namespace MARTe;
    SysLogger logger;
    return true;
}

bool SysLoggerTest::TestInitialise() {
    using namespace MARTe;
    SysLogger test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtOofFRmC");
    cdb.Write("Ident", "MARTe2SysLoggerTest");
    return test.Initialise(cdb);
}

bool SysLoggerTest::TestInitialise_False_Ident() {
    using namespace MARTe;
    SysLogger test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtOofFRmC");
    return !test.Initialise(cdb);
}
	
bool SysLoggerTest::TestInitialise_False_Format() {
    using namespace MARTe;
    SysLogger test;
    ConfigurationDatabase cdb;
    cdb.Write("Ident", "MARTe2SysLoggerTest");
    return !test.Initialise(cdb);
}

bool SysLoggerTest::TestConsumeLogMessage() {
    using namespace MARTe;
    LoggerService test;
    ReferenceT<SysLogger> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;
    cdb2.Write("Format", "EtOofFRmC");
    cdb2.Write("Ident", "MARTe2SysLoggerTest");
    cdb2.Write("PrintKeys", 1);
    bool ok = consumer->Initialise(cdb2);
    ConfigurationDatabase cdb;
    test.Insert(consumer);
    cdb.Write("CPUs", 0x2);
    test.Initialise(cdb);
    REPORT_ERROR_STATIC(ErrorManagement::CommunicationError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::Completed, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::ErrorAccessDenied, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::ErrorSharing, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::Exception, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::IllegalOperation, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::Information, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::InternalSetupError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::NoError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::NotCompleted, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::OSError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::RecoverableError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::SyntaxError, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::Timeout, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::UnsupportedFeature, "TestConsumeLogMessage");
    REPORT_ERROR_STATIC(ErrorManagement::Warning, "TestConsumeLogMessage");

    return ok;
}
