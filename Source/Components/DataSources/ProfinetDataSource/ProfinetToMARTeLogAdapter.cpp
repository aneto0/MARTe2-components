/**
 * @file ProfinetToMARTeLogAdapter.cpp
 * @brief Source file for class ProfinetToMARTeLogAdapter
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ProfinetToMARTeLogAdapter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

    ProfinetToMARTeLogAdapter::ProfinetToMARTeLogAdapter(ProfinetDataSourceDriver::log_adapter_level_t minimumLevel) {
        this->minimumLevel = minimumLevel;
    }

    void ProfinetToMARTeLogAdapter::Log(ProfinetDataSourceDriver::log_adapter_level_t logLevel, std::string message) {
        if(logLevel >= minimumLevel) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, message.c_str());
        }
    }

    ProfinetDataSourceDriver::log_adapter_level_t ProfinetToMARTeLogAdapter::GetMinimumLoggerLevel() {
        return minimumLevel;
    }
}
