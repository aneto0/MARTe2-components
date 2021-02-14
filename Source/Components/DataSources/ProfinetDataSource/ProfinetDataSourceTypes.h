/**
 * @file ProfinetDataSourceTypes.h
 * @brief Header file for class ProfinetDataSourceTypes 
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

 * @details This header file contains the declaration of the ancillary classes 
 * which can be grouped under the name ProfinetDataSourceTypes
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_TYPES_H
#define DATASOURCES_PROFINET_TYPES_H


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define PNET_MAX_DIRECTORYPATH_LENGTH 4095
#define PNET_MAX_PORT 1
#define PNET_MAX_DIRECTORYPATH_SIZE 240

#include <pnet_api.h>
#include <string>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ProfinetDataStructure.h"

#ifndef BIT
    #define BIT(n) (1u << (n))
#endif

namespace ProfinetDataSourceDriver {

    /**
     * @brief Allowed directions for a sub-module data
     */
    typedef enum pnet_submodule_direction
    {
        NoInputOutput,
        Input,
        Output,
        InputOutput
    } pnet_submodule_direction_t;
    
    /**
     * @details     Set of nested / nestable containers, which represents the typical Profinet peripheral hierarchy.
     *              A peripheral is a module, which can contain submodules. Each module is hosted into a slot
     *              while each submodule in a subslot. A slot can contain 1:N subslots, which once declared, are
     *              plugged at runtime by the Profinet library.
     *              NOTE: As the DataSource can (potentially) emulate every Profinet peripheral, every possible configuration
     *              could be specified. Configuration from the peripheral side must match the one loaded with GSDML on the master side.
     *              This is why two "configuration endpoints" exists:
     *              - expectedConfigurationData = what is specified from the peripheral side in the configuration file (which should somehow match the GSDML)
     *              - masterConfigurationData = what the master says about the Profinet peripheral, as specified in the GSDML.
     *              When running, the two must match, otherwise no memory mapping can occur and no runtime plugging can happen.
    */
    typedef struct pnet_subslot {
        bool                isDeviceAccessPoint;
        bool                masterAdded;
        bool                configAdded;
        bool                alarmArmed;
        
        uint16_t            subslotNumber;
        uint32_t            submoduleIdentifier;
        std::string         submoduleName;
        
        uint8_t             *inputData;
        uint8_t             *outputData;

        pnet_data_cfg_t     masterConfigurationData;
        pnet_data_cfg_t     expectedConfigurationData;

        pnet_subslot() {
            isDeviceAccessPoint = masterAdded = configAdded = alarmArmed = false;
            submoduleIdentifier = 0u;
			subslotNumber = 0u;
            submoduleName = "";
            inputData = outputData = static_cast<uint8_t*>(0u);

            masterConfigurationData.data_dir = PNET_DIR_NO_IO;
            masterConfigurationData.insize = 0u;
            masterConfigurationData.outsize = 0u;

            expectedConfigurationData.data_dir = PNET_DIR_NO_IO;
            expectedConfigurationData.insize = 0u;
            expectedConfigurationData.outsize = 0u;
        }

        bool IsMatchingExpectedConfiguration() const { 
            return  (masterConfigurationData.data_dir ==  expectedConfigurationData.data_dir) &&
                    (masterConfigurationData.insize == expectedConfigurationData.insize) &&
                    (masterConfigurationData.outsize == expectedConfigurationData.outsize);
        }

        bool  IsDAP()     const { return isDeviceAccessPoint; }
        bool  IsInput()   const { return (masterConfigurationData.data_dir == PNET_DIR_INPUT) || (masterConfigurationData.data_dir == PNET_DIR_IO); }
        bool  IsOutput()  const { return (masterConfigurationData.data_dir == PNET_DIR_OUTPUT) || (masterConfigurationData.data_dir == PNET_DIR_IO); }
        bool  IsNoIO()    const { return (masterConfigurationData.data_dir == PNET_DIR_NO_IO); }
        bool  IsUsed()    const { return (masterAdded && configAdded);  }
        bool  IsAlarmed() const { return alarmArmed; }
   	    void  AckAlarm()  { alarmArmed = false; }

    }pnet_subslot_t;

    /**
     * @details     Set of nested / nestable containers, which represents the typical Profinet peripheral hierarchy.
     *              A peripheral is a module, which can contain submodules. Each module is hosted into a slot
     *              while each submodule in a subslot. A slot can contain 1:N subslots, which once declared, are
     *              plugged at runtime by the Profinet library.
     *              NOTE: As the DataSource can (potentially) emulate every Profinet peripheral, every possible configuration
     *              could be specified. Configuration from the peripheral side must match the one loaded with GSDML on the master side.
     *              This is why two "configuration endpoints" exists:
     *              - expectedConfigurationData = what is specified from the peripheral side in the configuration file (which should somehow match the GSDML)
     *              - masterConfigurationData = what the master says about the Profinet peripheral, as specified in the GSDML.
     *              When running, the two must match, otherwise no memory mapping can occur and no runtime plugging can happen.
    */
    typedef struct pnet_slot {
        bool                                masterAdded;
        bool                                configAdded;

        uint16_t                            slotNumber;
        uint32_t                            moduleIdentifier;

        SimpleLinkedList<pnet_subslot_t*>   subslots;

        pnet_slot() {
            masterAdded = configAdded = false;
            slotNumber = 0u;
			moduleIdentifier = 0u;
        }

        bool                                IsUsed()    const { return (masterAdded && configAdded);  }

    }pnet_slot_t;

    /**
     * @brief Describes for the specific API Identifier the AREP (application relationship identifier). Currently single AREP supported.
     */
    typedef struct pnet_api {
        uint32_t                            apiIdentifier;
        uint32_t                            appRelationshipIdentifier;

        SimpleLinkedList<pnet_slot_t*>      slots;
    }pnet_api_t;

    /**
     * @brief Defines the event flag that can occur during Profinet stack execution.
     */
    typedef enum pnet_event {
        ReadyForData    = 1u,
        Timer           = 2u,
        Alarm           = 4u,
        Abort           = 128u
    }pnet_event_t;

    /**
     * @brief Allowed values for the Software Revision Field inside the Identification and Maintainance Data (I&M Data)
     */
    typedef enum {
        SwRev_V = 'V',
        SwRev_R = 'R',
        SwRev_P = 'P',
        SwRev_U = 'U',
        SwRev_T = 'T'
    }softwarerevision_t;
}

#endif /* DATASOURCES_PROFINET_TYPES_H */
