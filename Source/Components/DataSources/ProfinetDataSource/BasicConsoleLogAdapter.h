/**
 * @file BasicConsoleLogAdapter.h
 * @brief Header file for the Basic Console Log Adapter
 * @date 
 * @author
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

 * @details This header file contains the declaration of the class ProfinetDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_BASICCONSOLELOGADAPTER_H_
#define DATASOURCES_PROFINET_BASICCONSOLELOGADAPTER_H_

#include "ILoggerAdapter.h"

namespace MARTe {
        /**
        * @brief The basic console log adapter.
        * @details  Routes the log through the REPORT_ERROR_STATIC MARTe facility, to allow non-MARTe
        *           diagnostic output. Provides also a log level specification in output.
        */
        class BasicConsoleLogAdapter : public ProfinetDataSourceDriver::ILoggerAdapter {
        private:
            ProfinetDataSourceDriver::log_adapter_level_t minimumLevel;

        public:
            BasicConsoleLogAdapter(ProfinetDataSourceDriver::log_adapter_level_t minimumLevel = ProfinetDataSourceDriver::LogLevel_Debug);
            virtual void Log(ProfinetDataSourceDriver::log_adapter_level_t logLevel, std::string message);
    };
}

#endif /* DATASOURCES_PROFINET_BASICCONSOLELOGADAPTER_H_ */
