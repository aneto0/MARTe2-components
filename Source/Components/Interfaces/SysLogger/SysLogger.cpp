/**
 * @file SysLogger.cpp
 * @brief Source file for class SysLogger
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
 * the class SysLogger (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <syslog.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ErrorInformation.h"
#include "ErrorType.h"
#include "SysLogger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
SysLogger::SysLogger() : Object(), LoggerConsumerI() {
    ident = "";
}

SysLogger::~SysLogger() {
}

void SysLogger::ConsumeLogMessage(LoggerPage * const logPage) {
    if (logPage != NULL_PTR(LoggerPage *)) {
        /*lint -e{9130} -e{9117} the LOG_NDELAY and LOG_USER constants are defined by <syslog.h>*/
        openlog(ident.Buffer(), LOG_NDELAY, LOG_USER);

        StreamString err;
        PrintToStream(logPage, err);
        ErrorManagement::ErrorType errorType = logPage->errorInfo.header.errorType;

        int32 syslogErrorCode;
        if (errorType == ErrorManagement::Information) {
            syslogErrorCode = LOG_INFO;
        }
        else if (errorType == ErrorManagement::Warning) {
            syslogErrorCode = LOG_WARNING;
        }
        else if (errorType == ErrorManagement::FatalError) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::RecoverableError) {
            syslogErrorCode = LOG_ERR;
        }
        else if (errorType == ErrorManagement::Debug) {
            syslogErrorCode = LOG_DEBUG;
        }
        else if (errorType == ErrorManagement::Timeout) {
            syslogErrorCode = LOG_ERR;
        }
        else if (errorType == ErrorManagement::ParametersError) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::CommunicationError) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::NoError) {
            syslogErrorCode = LOG_INFO;
        }
        else if (errorType == ErrorManagement::Completed) {
            syslogErrorCode = LOG_WARNING;
        }
        else if (errorType == ErrorManagement::NotCompleted) {
            syslogErrorCode = LOG_WARNING;
        }
        else if (errorType == ErrorManagement::ErrorAccessDenied) {
            syslogErrorCode = LOG_ERR;
        }
        else if (errorType == ErrorManagement::InitialisationError) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::InternalSetupError) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::OSError) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::IllegalOperation) {
            syslogErrorCode = LOG_ERR;
        }
        else if (errorType == ErrorManagement::ErrorSharing) {
            syslogErrorCode = LOG_ERR;
        }
        else if (errorType == ErrorManagement::Exception) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::UnsupportedFeature) {
            syslogErrorCode = LOG_CRIT;
        }
        else if (errorType == ErrorManagement::SyntaxError) {
            syslogErrorCode = LOG_CRIT;
        }
        else {
            syslogErrorCode = LOG_CRIT;
        }
        syslog(syslogErrorCode, "%s", err.Buffer());
    }
}

bool SysLogger::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        ok = LoadPrintPreferences(data);
    }
    if (ok) {
        ok = data.Read("Ident", ident);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The Ident shall be specified");
        }
    }
    return ok;
}

CLASS_REGISTER(SysLogger, "1.0")

}

