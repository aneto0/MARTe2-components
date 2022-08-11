/**
 * @file SysLogger.h
 * @brief Header file for class SysLogger
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

 * @details This header file contains the declaration of the class SysLogger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SYSLOGGER_H_
#define SYSLOGGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LoggerConsumerI.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief A LoggerConsumerI which outputs the log messages to a syslog.
 * @details The configuration syntax is (names are only given as an example):
 *
 * <pre>
 * +SysLogger = {
 *     Class = SysLogger
 *     Format = EtOoFm //Compulsory. As described in LoggerConsumerI::LoadPrintPreferences
 *     PrintKeys = 1 //Optional. As described in LoggerConsumerI::LoadPrintPreferences
 *     Ident = myapp //Compulsory. Name of the syslog ident.
 * }
 * </pre>
 */
class SysLogger: public Object, public LoggerConsumerI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    SysLogger();

    /**
     * @brief Destructor. Closes the console.NOOP.
     */
    virtual ~SysLogger();

    /**
     * @brief Prints the logPage in the console output.
     * @param logPage the log message to be printed.
     */
    virtual void ConsumeLogMessage(LoggerPage *logPage);

    /**
     * @brief Calls Object::Initialise and reads the Format, Ident and PrintKeys parameters (see class description) .
     * @param[in] data see Object::Initialise.
     * @return true if Object::Initialise returns true and if the compulsory are correctly set..
     */
    virtual bool Initialise(StructuredDataI &data);
private:
    /**
     * The syslog ident
     */
    StreamString ident;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SYSLOGGER_H_ */

