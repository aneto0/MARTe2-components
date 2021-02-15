/**
 * @file ILoggerAdapter.h
 * @brief Header file for class ILoggerAdapter
 * @date 22/01/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class (interface) ILoggerAdapter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_ILOGGERADAPTER_H
#define DATASOURCES_PROFINET_ILOGGERADAPTER_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <string>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace ProfinetDataSourceDriver {
/**
 * @brief Implementation of the ILoggerAdapter for the ProfinetDataSourceDriver
 * 
 * @details Implements a generic double-linked list with the Sentinel Node approach.
 * Provides also a convenient iterator (forward and reverse) for easy traversal.
 */

/**
 * @brief Log levels for the ILogAdapter class.
 * @details Allowed message output levels for the ILogAdapter class.
 */
//lint ++flb "ILoggerAdapter" Avoids considering enum items unused due to linter skipping underlying layers
typedef enum logadapter_level {
    LogLevel_Debug      = 0,
    LogLevel_Info       = 1,
    LogLevel_Warning    = 2,
    LogLevel_Error      = 3
}log_adapter_level_t;
//lint --flb

/**
 * @brief Interface for a simple generic log adaptation.
 * @details Abstraction to allow logging / printing capabilities from inner classes,
 * decoupling from MARTe specifics. This interface allows the usage of a generic Log
 * method from the inner classes which can be conveniently routed to the MARTe2 
 * REPORT_ERROR_STATIC facility once implementing the DataSource.
 */
class ILoggerAdapter {
public:

    /**
     * @brief Destructor.
     * @details The destructor for ILoggerAdapter.
     */
    virtual ~ILoggerAdapter() {}

    /**
     * @brief The Log method.
     * @details The Log method used at ProfinetToMARTeLogAdapter.
     */
    //lint -esym(1716, ProfinetDataSourceDriver::*) Function is called by underlying libraries which are not seen by the Linter
    virtual void Log(log_adapter_level_t logLevel, std::string message) = 0;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_ILOGGERADAPTER_H */
