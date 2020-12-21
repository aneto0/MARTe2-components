/**
 * @file ILoggerAdapter.h
 * @brief Logger adaptation layer to bring reporting facilities to upper layers.
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

 * @details     Provides an abstraction to become able to provide logging / printing capabilities
 *              from inner classes, decoupling from MARTe specifics. This interface allows the usage of
 *              a generic Log() method from the inner classes which can be conveniently routed to the
 *              MARTe REPORT_ERROR() facility once implementing the DataSource
 */

#ifndef __PROFINET_DATASOURCE_ILOGGERADAPTER_H__
#define __PROFINET_DATASOURCE_ILOGGERADAPTER_H__

#include <string>

namespace ProfinetDataSourceDriver {

    typedef enum logadapter_level {
        LogLevel_Debug      = 0,
        LogLevel_Info       = 1,
        LogLevel_Warning    = 2,
        LogLevel_Error      = 3
    }log_adapter_level_t;

    class ILoggerAdapter {
        
        public:
            virtual ~ILoggerAdapter() {}
            virtual void Log(log_adapter_level_t logLevel, std::string message) = 0;
    };

}

#endif /* __PROFINET_DATASOURCE_ILOGGERADAPTER_H__ */
