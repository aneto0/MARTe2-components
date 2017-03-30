/**
 * @file EpicsDataSourceSupport.h
 * @brief Header file for class EpicsDataSourceSupport
 * @date 30/01/2017
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class EpicsDataSourceSupport
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EPICSDATASOURCESUPPORT_H_
#define EPICSDATASOURCESUPPORT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Builds a configuration database as needed by DataSourceI::
 * SetConfiguredDatabase.
 * @details The configuration database will contain two root nodes, one called
 * "Signals" and another called "Functions" as specified in DataSourceI::
 * SetConfiguredDatabase. The "Signals" node will contain numberOfSignals
 * signals of type uint32, while "Functions" will contain numberOfFunctions
 * functions.
 */
bool BuildConfigurationDatabase(MARTe::ConfigurationDatabase& cdb,
                                const MARTe::uint32 numberOfSignals,
                                const MARTe::uint32 numberOfFunctions = 0);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EPICSDATASOURCESUPPORT_H_ */
