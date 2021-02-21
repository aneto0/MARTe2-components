/**
 * @file ProfinetDataSourceConstants.h
 * @brief Header file for class ProfinetDataSourceConstants
 * @date 15/01/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the class ProfinetDataSourceConstants
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_CONSTANTS_H
#define DATASOURCES_PROFINET_CONSTANTS_H

    #ifndef PNET_MAX_PORT
	/**
 	* @brief Maximum number of Profinet ports (Application Relationship EndPoints) currently the DataSource consumes only 1
 	*/ 	
        #define PNET_MAX_PORT 1
    #endif

    #ifndef PNET_MAX_DIRECTORYPATH_SIZE
	/**
 	* @brief Maximum path length (needd by P-NET library) 
 	*/ 	
        #define PNET_MAX_DIRECTORYPATH_SIZE 240
    #endif

#endif /* DATASOURCES_PROFINET_CONSTANTS_H */
