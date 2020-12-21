/**
 * @file pnetdstypes.h
 * @brief   Data structure for Profinet adaptation.
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

 * @details     Set of nested containers, which represents the typical Profinet peripheral hierarchy.
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

#ifndef __PNET_DATASOURCE_TYPES_H__
#define __PNET_DATASOURCE_TYPES_H__

#include <string>
#include <stdint.h>

#define PNET_MAX_DIRECTORYPATH_LENGTH 4095

#include <pnet_api.h>
#include "pnetdatastructure.h"

#ifndef BIT
    #define BIT(n) (1U << (n))
#endif

namespace ProfinetDataSourceDriver {

    typedef enum pnet_submodule_direction
    {
        NoInputOutput,
        Input,
        Output,
        InputOutput
    } pnet_submodule_direction_t;

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
            submoduleIdentifier = subslotNumber = 0;
            submoduleName = "";
            inputData = outputData = NULL;

            masterConfigurationData.data_dir = PNET_DIR_NO_IO;
            masterConfigurationData.insize = 0;
            masterConfigurationData.outsize = 0;

            expectedConfigurationData.data_dir = PNET_DIR_NO_IO;
            expectedConfigurationData.insize = 0;
            expectedConfigurationData.outsize = 0;
        }

        bool IsMatchingExpectedConfiguration() { 
            return  (masterConfigurationData.data_dir ==  expectedConfigurationData.data_dir) &&
                    (masterConfigurationData.insize == expectedConfigurationData.insize) &&
                    (masterConfigurationData.outsize == expectedConfigurationData.outsize);
        }

        bool                IsDAP()     { return isDeviceAccessPoint; }
        bool                IsInput()   { return (masterConfigurationData.data_dir == PNET_DIR_INPUT) || (masterConfigurationData.data_dir == PNET_DIR_IO); }
        bool                IsOutput()  { return (masterConfigurationData.data_dir == PNET_DIR_OUTPUT) || (masterConfigurationData.data_dir == PNET_DIR_IO); }
        bool                IsNoIO()    { return (masterConfigurationData.data_dir == PNET_DIR_NO_IO); }
        bool                IsUsed()    { return (masterAdded && configAdded);  }
        bool                IsAlarmed() { return alarmArmed; }
        void                AckAlarm()  { alarmArmed = false; }

    }pnet_subslot_t;

    typedef struct pnet_slot {
        bool                                masterAdded;
        bool                                configAdded;

        uint16_t                            slotNumber;
        uint32_t                            moduleIdentifier;

        SimpleLinkedList<pnet_subslot_t*>   subslots;

        pnet_slot() {
            masterAdded = configAdded = false;
            slotNumber = moduleIdentifier = 0;
        }

        bool                                IsUsed()    { return (masterAdded && configAdded);  }

    }pnet_slot_t;

    typedef struct pnet_api {
        uint32_t                            apiIdentifier;
        uint32_t                            appRelationshipIdentifier;

        SimpleLinkedList<pnet_slot_t*>      slots;
    }pnet_api_t;

    typedef enum pnet_event {
        ReadyForData    = BIT (0),
        Timer           = BIT (1),
        Alarm           = BIT (2),
        Abort           = BIT (15)
    }pnet_event_t;

    typedef enum {
        SwRev_V = 'V',
        SwRev_R = 'R',
        SwRev_P = 'P',
        SwRev_U = 'U',
        SwRev_T = 'T'
    }softwarerevision_t;
}

#endif /* __PNET_DATASOURCE_TYPES_H__ */