/**
 * @file SDNLoggerCallback.cpp
 * @brief MARTe2 plug-in for SDN Core library logs
 * @date 30/01/2017
 * @author Bertrand Bauvir
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
 * the class LinuxTimer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ErrorManagement.h"
#include "ErrorInformation.h"

#include "sdn-api.h" /* SDN core library - API definition (sdn::core) */

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

void vMessage2MARTe (Severity_t severity, const char* source, const char* message, va_list args) 
{ 
  /* Filter out traces and debug statements */
  if (severity > LOG_INFO) return;

  using namespace MARTe;

  ErrorManagement::ErrorType code;

  switch (severity) 
    {
      default: 
      case LOG_INFO:
      case LOG_NOTICE:
	code = ErrorManagement::Information; 
	break;
      case LOG_WARNING: 
	code = ErrorManagement::Warning; 
	break;
      case LOG_ERR:
	code = ErrorManagement::RecoverableError; 
	break;
      case LOG_CRIT:    
      case LOG_ALERT:
      case LOG_EMERG:
	code = ErrorManagement::FatalError; 
	break;
    }

  char buffer [1024] = STRING_UNDEFINED;
  char* p_buf = (char*) buffer;
  uint_t size = 1024;

  if (IsUndefined(source) != true) snprintf(p_buf, size, "[%s] ", source);

  /* Re-align pointer */
  size -= strlen(p_buf); p_buf += strlen(p_buf); 

  vsnprintf(p_buf, size, message, args); size -= strlen(p_buf); p_buf += strlen(p_buf); /* Re-align pointer */

  /* MARTe2 logging */
  REPORT_ERROR(code, buffer);

  return; 

};

static ccs::log::Func_t org_cb = ccs::log::SetCallback(&vMessage2MARTe);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
