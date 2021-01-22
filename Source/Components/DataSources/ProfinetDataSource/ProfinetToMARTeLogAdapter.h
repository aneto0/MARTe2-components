/**
 * @file ProfinetToMARTeLogAdapter.h
 * @brief Header file for class ProfinetToMARTeLogAdapter
 * @date 21/01/2021
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
 *
 * @details This header file contains the declaration of the class ProfinetToMARTeLogAdapter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_PROFINETTOMARTELOGADAPTER_H_
#define DATASOURCES_PROFINET_PROFINETTOMARTELOGADAPTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ILoggerAdapter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
    
/**
* @brief The basic console log adapter.
* @details  Routes the log through the REPORT_ERROR_STATIC MARTe facility, to allow non-MARTe
* diagnostic output. Provides also a log level specification in output.
*/
class ProfinetToMARTeLogAdapter : public ProfinetDataSourceDriver::ILoggerAdapter {
public:

    /**
     * @brief Parametrized constructor.
     * @details The Log method uses the MARTe2 REPORT_ERROR_STATIC facility to ouput messages 
     * with level logLevel above the minimum level set through the ProfinetToMARTeLogAdapter method.
     * @param[in] minimumLevel Defines the minimum log level needed for the logger to produce
     * an output. Defaults to log level Debug.
     */
    ProfinetToMARTeLogAdapter(ProfinetDataSourceDriver::log_adapter_level_t minimumLevel = ProfinetDataSourceDriver::LogLevel_Debug);

    /**
     * @brief The Log method used to send output messages.
     * @details The Log method uses the MARTe2 REPORT_ERROR_STATIC facility to ouput messages with
     * level logLevel above the minimum level set through the ProfinetToMARTeLogAdapter method. 
     * @param[in] logLevel The level of the message to be logged.
     * @param[in] message The message to be logged.
     */
    virtual void Log(ProfinetDataSourceDriver::log_adapter_level_t logLevel, std::string message);

    /**
     * @brief The GetMinimumLoggerLevel method.
     * @details The GetMinimumLoggerLevel method retuns the minimumLevel set for the logger. 
     * @return The minimumLevel set for the logger.
     */
    ProfinetDataSourceDriver::log_adapter_level_t GetMinimumLoggerLevel();

private:

    /**
     * Holds the minimum logging level.
     */
    ProfinetDataSourceDriver::log_adapter_level_t minimumLevel;
};
}

#endif /* DATASOURCES_PROFINET_PROFINETTOMARTELOGADAPTER_H_ */
