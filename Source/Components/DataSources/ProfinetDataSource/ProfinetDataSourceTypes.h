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
/**
 * @brief Maximum size of the path (needed by the P-NET library
*/
#define PNET_MAX_DIRECTORYPATH_LENGTH 4095
/**
 * @brief Maximum number of ports (Application Relatioship Endpoints) currently the DataSource consumes 1 port
*/
#define PNET_MAX_PORT 1
/**
 * @brief Maximum size of the path (needed by the P-NET library)
*/
#define PNET_MAX_DIRECTORYPATH_SIZE 240

#include <pnet_api.h>
#include <string>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
//lint ++flb "Data Structure Lib"
#include "ProfinetDataStructure.h"
//lint --flb

#ifndef BIT
    /**
     * @brief Computes the bit shift for flag elements creation
     */ 
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
     * @brief 	    Nested containers to represent typical peripheral structure 
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
	/**
 	* @brief Indicates if the subslot belongs to the special DAP slots (see DataSource documentation)
 	*/ 
        bool                isDeviceAccessPoint;
	
	/**
 	* @brief Indicates if the subslot was added by the master, by advertising it
 	*/ 	
        bool                masterAdded;
	
	/**
 	* @brief Indicates if the subslot was added by the user, by configuring it in the MARTe2 configuration file
 	*/ 
        bool                configAdded;
	
	/**
 	* @brief Indicates if the alarm for the subslot is armed (currently not supported)
 	*/ 	
        bool                alarmArmed;
        
	/**
 	* @brief Identifier number for the subslot
 	*/  
        uint16_t            subslotNumber;
	
	/**
 	* @brief Identifier number for the submodule
 	*/ 	
        uint32_t            submoduleIdentifier;

	/**
 	* @brief Identifier name for the submodule (unused)
 	*/
        std::string         submoduleName;

        /**
 	* @brief Memory bank for input data. This heap block is shared with the underlying p-net lib.
 	*/	
        uint8_t             *inputData;

	/**
 	* @brief Memory bank for output data. This heap block is shared with the underlying p-net lib.
 	*/ 
        uint8_t             *outputData;

	/**
 	* @brief Master advertised configuration data (must match with the expected to enable the slot for the plugging.
 	*/ 
        pnet_data_cfg_t     masterConfigurationData;

	/**
 	* @brief MARTe2 configuration acquired data (must match with master to enable the slot for the plugging.
 	*/
        pnet_data_cfg_t     expectedConfigurationData;

	/**
 	* @brief Constructs and initializes the subslot
 	*/ 
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

	/**
 	* @brief Checks if expected and master advertised configuration is matching
 	*/ 
        bool IsMatchingExpectedConfiguration() const { 
            return  (masterConfigurationData.data_dir ==  expectedConfigurationData.data_dir) &&
                    (masterConfigurationData.insize == expectedConfigurationData.insize) &&
                    (masterConfigurationData.outsize == expectedConfigurationData.outsize);
        }

	/**
 	* @brief Convenient acceleration method to tell if the subslot is a DAP
 	*/ 
        bool  IsDAP()     const { return isDeviceAccessPoint; }

        /**
	* @brief Convenient acceleration method to tell if the subslot is an input
	*/	 
	bool  IsInput()   const { return (masterConfigurationData.data_dir == PNET_DIR_INPUT) || (masterConfigurationData.data_dir == PNET_DIR_IO); }

        /**
 	* @brief Convenient acceleration method to tell if the subslot is an output
 	*/ 
	bool  IsOutput()  const { return (masterConfigurationData.data_dir == PNET_DIR_OUTPUT) || (masterConfigurationData.data_dir == PNET_DIR_IO); }

        /**
 	* @brief Convenient acceleration method to tell if the subslot is a NO-I/O (See DataSource Documentation)
 	*/ 
	bool  IsNoIO()    const { return (masterConfigurationData.data_dir == PNET_DIR_NO_IO); }

        /**
 	* @brief Convenient acceleration method to tell if the subslot is master advertised and present in the MARTe2 cfg file
 	* which means that is used in the adapter loop
 	*/ 
	bool  IsUsed()    const { return (masterAdded && configAdded);  }

        /**
 	* @brief Convenient acceleration method to tell if an alarm is active on the subslot
 	*/
	bool  IsAlarmed() const { return alarmArmed; }

        /**
 	* @brief Convenient acceleration method to clear (ack) the alarm on the subslot
 	*/ 
	//lint -e{1960} Alarm acknowledgement sets the armed variable back, which status can be retrieved with the IsAlarmed function
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
        /**
 	* @brief Indicates whether the slot was advertised by the master
 	*/ 
	bool                                masterAdded;
	/**
 	* @brief Indicates whether the slot was added from the MARTe2 configuration file
 	*/  	
        bool                                configAdded;
	/**
 	* @brief Slot number idenifier
 	*/ 	
        uint16_t                            slotNumber;
        /**
 	* @brief Module identifier
 	*/ 	
	uint32_t                            moduleIdentifier;

	/**
 	* @brief Structure containing the subslot for the represented slot
 	*/
        SimpleLinkedList<pnet_subslot_t*>   subslots;

	/**
 	* @brief Constructs and initializes an empty slot structure
 	*/ 
        pnet_slot() {
            masterAdded = configAdded = false;
            slotNumber = 0u;
	    moduleIdentifier = 0u;
        }
	
	/**
 	* @brief Indicates if the slot was advertised by the master and added in the MARTe2 configuration file
 	* which essentially means that the slot is used
 	*/ 
        bool                                IsUsed()    const { return (masterAdded && configAdded);  }

    }pnet_slot_t;

    /**
     * @brief Describes for the specific API Identifier the AREP (application relationship identifier). Currently single AREP supported.
     */
    typedef struct pnet_api {
	/**
 	* @brief Identifies the API (Currently not supported)
 	*/ 	
        uint32_t                            apiIdentifier;

	/**
 	* @brief Identifies the application relatioship
 	*/ 
        uint32_t                            appRelationshipIdentifier;

	/**
 	* @brief The structure which models the slot and subslot hierarchy of the module
 	*/ 
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
