/**
 * @file ProfinetDataSourceAdapter.h
 * @brief Header file for class ProfinetDataSourceAdapter
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

 * @details This header file contains the declaration of the class ProfinetDataSourceAdapter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_ADAPTER_H
#define DATASOURCES_PROFINET_ADAPTER_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "ProfinetDataSourceConstants.h"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <pnal.h>
#include <pnet_api.h>
#undef MAX
#undef MIN
#include <sstream>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "ICyclicNotifiable.h"
#include "ILoggerAdapter.h"
#include "IOperationalSignalsEntryPoint.h"
#include "IProfinetEventNotifiable.h"
#include "MutexSem.h"
#include "ProfinetDataSourceTypes.h"
#include "ProfinetEventType.h"
#include "StreamString.h"

/**
 * @brief Maximum ethernet interface name size 
 */
#define PNDS_MAX_INTERFACE_NAME_SIZE    16
/**
 * @brief Maximum Profinet station name size
 */
#define PNDS_MAX_STATION_NAME_SIZE      241
/**
 * @brief Maximum allowed subslots
 */
#define PNDS_MAX_SUPPORTED_SUBSLOTS     5
/**
 * @brief Maximum Profinet product name size
 */
#define PNDS_MAX_PRODUCTNAME_SIZE       26
/**
 * @brief Maximum I&M Order identification size
 */
#define PNDS_MAX_IM0_ORDERID_SIZE       21
/**
 * @brief Maximum I&M Serial Number size
 */
#define PNDS_MAX_IM0_SERNUM_SIZE        17
/**
 * @brief Maximum I&M Tag Function size
 */
#define PNDS_MAX_IM1_TAGFUNC_SIZE       33
/**
 * @brief Maximum I&M Tag Location size
 */
#define PNDS_MAX_IM1_TAGLOC_SIZE        23
/**
 * @brief Maximum I&M Tag Date size
 */
#define PNDS_MAX_IM2_DATE_SIZE          17
/**
 * @brief Maximum I&M Tag Description size
 */
#define PNDS_MAX_IM3_DESC_SIZE          55
/**
 * @brief Maximum I&M Tag Signature size
 */
#define PNDS_MAX_IM4_SIGN_SIZE          54

/**
 * @brief Literal for error return
 */
#define PNDS_RETURN_VALUE_ERROR         -1
/**
 * @brief Literal for success return
 */
#define PNDS_RETURN_VALUE_SUCCESS       0

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace ProfinetDataSourceDriver
{
    /**
     * @brief   Implements an intermediate adapter between the MARTe DataSource and the Profinet library.
     *          As the p-net library is implemented in C, the library is first 1:1 mapped into C++.
     *          Convenient function for library and bus management are provided to upper layers, with
     *          the MARTe DataSource approach in mind.
     */
    class ProfinetDataSourceAdapter {

        private:

            /**
             * @brief Default private empty constructor
             */
            ProfinetDataSourceAdapter() {;}
            

            /**
             * @brief Configures the internals for network parameters, which are further used in deep configuration processes
            */
            bool ConfigureNetworkData();

            /**
             * @brief Configures the Profinet handle network parameters, previously filled up
             */
            bool ConfigureProfinetPort();

            /**
             * @brief Holds the Logger Adapter instance.
             */
            ILoggerAdapter  *loggerAdapter;

            /**
             * @brief Holds the P-NET library Profinet Handle, which is the main structure for the whole stack implementation.
            */
            pnet_t          *profinetHandle;

            /**
             * @brief Holds the P-NET library Configuration Handle
            */
            pnet_cfg_t      *profinetConfigurationHandle;

            /**
             * @brief Holds a structure which keeps Slots/Subslots hierarchy and info.
            */
            pnet_api_t      apiStructure;

            /**
             * @brief Periodic interval in microseconds
            */
            uint32_t        periodicInterval;

            /**
             * @brief Essentially an identifier, which serves as handle, to identify the relationship (see Profinet AREP)
            */
            uint32_t        appRelationshipEndPoint;

            /**
             * @brief Network interface MAC Address
            */
            pnal_ethaddr_t    macAddress;

            /**
             * @brief Network interface IP Address
            */
            pnal_ipaddr_t     ipAddress;

            /**
             * @brief Network interface Netmask
            */
            pnal_ipaddr_t     netmask;

            /**
             * @brief Network interface gateway
            */
            pnal_ipaddr_t     gateway;

            
            //os_thread_t     *mainThread;
            //os_timer_t      *taskTimer;
            //os_timer_t      *ledTimer;
            //os_event_t      *events;

            //uint32_t        eventMask;
            //uint32_t        eventFlags;

	    /**
 	    * @brief Reduction between loop cycles and update cycles (1 update every gearRatio)
 	    */ 	
            uint32_t        gearRatio;

            /**
             * @brief Indicates whether the callbacks are assigned.
             */
            bool            callbacksUp;

            /**
             * @brief Indicates whether the base configuration data is setup.
             */
            bool            baseDataUp;

            /**
             * @brief Indicates whether the I&M data is setup.
             */
            bool            idMaintDataUp;

            /**
             * @brief Indicates whether the Link Layer Discovery Protocol data is setup.
             */
            bool            lldpDataUp;

            /**
             * @brief Indicates whether the whole P-NET stack is up.
             */
            bool            profinetUp;

            /**
             * @brief The network interface name, as seen by the OS.
             */
            char            ethInterface[PNET_INTERFACE_NAME_MAX_SIZE];

            /**
             * @brief The Profinet station name, which matches the GSDML configured one.
             */
            char            stationName[PNDS_MAX_STATION_NAME_SIZE];

            /**
             * @brief The status of the LED signal.
             */
            bool            signalLEDStatus;
            
            /**
             * @brief Holds a total scan cycles occurred, used to compute the reduction ratio for updates.
             */
            uint32_t cycles;

            //Region - Callbacks
            /**
             * @brief Initializes the callbacks from the P-NET to the Adapter.
             */
            void BaseInitializeCallbacks();

            /**
             * @brief Indication to the application that a connect request was received from the controller.
             */
            int ConnectIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result);

            /**
             * @brief Indication to the application that a release request was received from the controller.
             */
            int ReleaseIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result);

            /**
             * @brief Indication to the application that a DControl request was received from the controller.
             */
            int DControlIndication(pnet_t * net, uint32_t arep, pnet_control_command_t control_command, pnet_result_t * p_result);

            /**
             * @brief Indication to the application that a CControler confirmation was received from the controller.
             */
            int CControlIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result);

            /**
             * @brief Indication to the application that a state transition has occurred within the Profinet stack.
             */
            int StateIndication(pnet_t * net, uint32_t arep, pnet_event_values_t state); 

            /**
             * @brief Indication to the application that an IODRead request was received from the controller.
             */
            int ReadIndication(pnet_t * net, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t ** pp_read_data, uint16_t * p_read_length, pnet_result_t * p_result);

            /**
             * @brief Indication to the application that an IODWrite request was received from the controller
             */
            int WriteIndication(pnet_t * net, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t * p_write_data, pnet_result_t * p_result);

            /**
             * @brief Indication to the application that a module is requested by the controller in a specific slot.
             */
            int ExpModuleIndication(pnet_t * net, uint32_t api, uint16_t slot, uint32_t module_ident);

            /**
             * @brief Indication to the application that a sub-module is requested by the controller in a specific sub-slot.
             */
            int ExpSubmoduleIndication(pnet_t * net, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t * p_exp_data); 

            /**
             * @brief Indication to the application that the data status received from the controller has changed.
             */
            int NewDataStatusIndication(pnet_t * net, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status);

            /**
             * @brief The IO Controller has sent an alarm to the device. Used for alarms triggered by the IO Controller.
             */
            int AlarmIndication(pnet_t * net, uint32_t arep, const pnet_alarm_argument_t * p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t * p_data);

            /**
             * @brief The controller acknowledges the alarm sent previously, to allow sending other alarms.
             */
            int AlarmConfirmation(pnet_t * net, uint32_t arep, const pnet_pnio_status_t * p_pnio_status);

            /**
             * @brief The controller is acknowledging the alarm ACK sent previously. Used for alarms triggered by the IO Controller.
             */
            int AlarmAcknowledgementConfirmation(pnet_t * net, uint32_t arep, int res);

            /**
             * @brief Indication to the application that a reset request was received from the IO Controller.
             */
            int ResetIndication(pnet_t * net, bool should_reset_application, uint16_t reset_mode);

            /**
             * @brief Indication to the application that the Profinet signal LED should change state. Optional to implement but a 
             * Profinet compliant device must have a signal LED.
             */
            int SignalLedIndication(pnet_t * net, bool led_state);
            
            //Endregion - Callbacks

            //Region - Signal Led management
            /**
             * @brief Internal method to set the LED status
             */
            void SetLedStatus(bool ledStatus);
            //Endregion - Signal Led management

            //Region - Callback mappers
            /**
             * @brief Remaps P-NET callback to the Connection indication method.
             */
            friend int pnetds_connect_ind(pnet_t * net, void * arg, uint32_t arep, pnet_result_t * p_result);

            /**
             * @brief Remaps P-NET callback to the Release indication method.
             */
            friend int pnetds_release_ind(pnet_t * net, void * arg, uint32_t arep, pnet_result_t * p_result);

            /**
             * @brief Remaps P-NET callback to the DControl indication method.
             */
            friend int pnetds_dcontrol_ind(pnet_t * net, void * arg, uint32_t arep, pnet_control_command_t control_command, pnet_result_t * p_result);          

            /**
             * @brief Remaps P-NET callback to the CControl indication method.
             */
            friend int pnetds_ccontrol_cnf(pnet_t * net, void * arg, uint32_t arep, pnet_result_t * p_result);

            /**
             * @brief Remaps P-NET callback to the State Indication method.
             */
            friend int pnetds_state_ind(pnet_t * net, void * arg, uint32_t arep, pnet_event_values_t state); 

            /**
             * @brief Remaps P-NET callback to the Read Indication method.
             */
            friend int pnetds_read_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t ** pp_read_data, uint16_t * p_read_length, pnet_result_t * p_result);

            /**
             * @brief Remaps P-NET callback to the Write Indication method.
             */
            friend int pnetds_write_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t * p_write_data, pnet_result_t * p_result);

            /**
             * @brief Remaps P-NET callback to the Expansion Module Indication method.
             */
            friend int pnetds_exp_module_ind(pnet_t * net, void * arg, uint32_t api, uint16_t slot, uint32_t module_ident); 

            /**
             * @brief Remaps P-NET callback to the Expansion sub-module Indication method.
             */
            friend int pnetds_exp_submodule_ind(pnet_t * net, void * arg, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t * p_exp_data); 

            /**
             * @brief Remaps P-NET callback to the New Data Status indication method.
             */
            friend int pnetds_new_data_status_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status);

            /**
             * @brief Remaps P-NET callback to the Alarm Indication Method.
             */
            friend int pnetds_alarm_ind(pnet_t * net, void * arg, uint32_t arep, const pnet_alarm_argument_t * p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t * p_data);

            /**
             * @brief Remaps P-NET callback to the Alarm Confirmation method.
             */
            friend int pnetds_alarm_cnf(pnet_t * net, void * arg, uint32_t arep, const pnet_pnio_status_t * p_pnio_status);

            /**
             * @brief Remaps P-NET callback to the Alarm ACK Confirmation method.
             */
            friend int pnetds_alarm_ack_cnf(pnet_t * net, void * arg, uint32_t arep, int res);

            /**
             * @brief Remaps P-NET callback to the Reset Indication method.
             */
            friend int pnetds_reset_ind(pnet_t * net, void * arg, bool should_reset_application, uint16_t reset_mode);

            /**
             * @brief Remaps P-NET callback to the Signal LED indication method.
             */
            friend int pnetds_signal_led_ind(pnet_t * net, void * arg, bool led_state);
            //Endregion - Callback mappers

            //Region - Helpers
            /**
             * @brief Helper to set the data and the IO Producer/Consumer status.
             */
            int SetDataAndIOXS();

            /**
             * @brief Helper to set the cyclic data exchange.
             */
            int SetCyclicData();

            /**
             * @brief Checks if a file exists for a number of attempts, one every interval
             */
            bool FileExists(const char* pathName, int attempts, int intervalMs);

            /**
             * @brief Validates the configuration data handle, for basic features (as taken from pf_fspm_validate_configuration in pf_fspm.c)
             * @return true if base configuration data is valid
             */
            bool ValidateProfinetConfigurationHandle();
            //Endregion - Helpers

        public:
        
	    /**
 	    * @brief Destructor 
 	    */ 	
            ~ProfinetDataSourceAdapter();

            /**
             * @brief Returns a COPY of the Profinet device configuration handle.
             * Usage intended for testing purposes.
             * @return A copy of the configuration handle, in order to avoid interfering with the private data.
             */
            pnet_cfg_t GetProfinetConfigurationHandle();

            /**
             * @brief Checks if the callbacks are connected (explicitly)
             * implicitly indicates whether the constructor has managed to instantiate base
             * networking data.
             * @return true if callbacks (and main network data) are valid
             */
            bool CheckBaseNetworkingData();

            /**
             * @brief Constructs a ProfinetDataSourceAdapter instance with specified base configuration parameters.
             * @param[in] ethInterface The name of the ethernet interface reserver for Profinet communication
             * @param[in] periodicInterval The periodic interval expressed in microseconds for cyclic data exchange
             * @param[in] stationName The name of the station as appearing on the GSDML description file
             * @param[in] gearRatio The reduction between cycles and update cycles
             * @param[in] loggerAdapter The Instance of the logger, to redirect all internal messages in an unified outputter
             */
            ProfinetDataSourceAdapter(
                std::string     ethInterface, 
                uint32_t        periodicInterval,
                std::string     stationName,
                uint32_t        gearRatio,
                ILoggerAdapter  *loggerAdapter
            );
            //Region - Initialization methods

            /**
             * @brief Sets base configuration data for the Profinet stack and slave
             * @param[in] vendorIdHigh High part of the vendor identifier (see GSDML)
             * @param[in] vendorIdLow Low part of the vendor identifier (see GSDML)
             * @param[in] deviceIdHigh High part of the device identifier (see GSDML)
             * @param[in] deviceIdLow Low part of the vendor identifier (see GSDML)
             * @param[in] oemDeviceIdHigh High part of the OEM device identifier.
             * @param[in] oemDeviceIdLow Low part of the OEM device identifier.
             * @param[in] oemVendorIdLow Low part of the OEM vendor identifier.
             * @param[in] oemVendorIdHigh High part of the OEM vendor identifier.
	     * @param[in] productName The Profinet product name string.
             * @param[in] minimumDeviceInterval Minimum supported update interval for the Profinet slave
            */
            bool SetBaseData(
                MARTe::uint8           vendorIdHigh,
                MARTe::uint8           vendorIdLow,
                MARTe::uint8           deviceIdHigh,
                MARTe::uint8           deviceIdLow,
                MARTe::uint8           oemVendorIdHigh,
                MARTe::uint8           oemVendorIdLow,
                MARTe::uint8           oemDeviceIdHigh,
                MARTe::uint8           oemDeviceIdLow,
                MARTe::StreamString    productName,
                MARTe::uint16          minimumDeviceInterval
            );

            /**
             * @brief Sets the identification and Maintainance data for the Profinet stack and slave (I&M Data)
             * @param[in] vendorIdDataHigh High part of the vendor data identifier
             * @param[in] vendorIdDataLow Low part of the vendor data identifier
             * @param[in] hardwareRevision Hardware revision
             * @param[in] softwareRevision Software revision
             * @param[in] functionalEnhancement Functional enhancement
             * @param[in] bugFix Bug fix
             * @param[in] internalChange Internal change
             * @param[in] profileIdentifier Profile identifier
             * @param[in] profileSpecificType Profile specific type
             * @param[in] versionMajor Major software version
             * @param[in] versionMinor Minor software version
             * @param[in] orderIdentification Device order id
             * @param[in] serialNumber Device serial number
             * @param[in] tagFunction Tag attribute for function
             * @param[in] tagLocation Tag attribute for location
             * @param[in] date Tag attribute for data
             * @param[in] descriptor Tag attribute for device descriptor
             * @param[in] signature Tag attribute for device signature
             */
            bool SetIdentificationAndMaintainanceData(
                MARTe::uint8           vendorIdDataHigh,
                MARTe::uint8           vendorIdDataLow ,
                MARTe::uint16          hardwareRevision,
                softwarerevision_t     softwareRevision,
                MARTe::uint8           functionalEnhancement,
                MARTe::uint8           bugFix,
                MARTe::uint8           internalChange,
                MARTe::uint16          profileIdentifier,
                MARTe::uint16          profileSpecificType,
                MARTe::uint8           versionMajor,
                MARTe::uint8           versionMinor,
                MARTe::StreamString    orderIdentification,
                MARTe::StreamString    serialNumber,
                MARTe::StreamString    tagFunction,
                MARTe::StreamString    tagLocation,
                MARTe::StreamString    date,
                MARTe::StreamString    descriptor,
                MARTe::StreamString    signature
            );

            /**
             * @brief Sets the LLDP (Link Layer Discovery Protocol) Configuration data
             * @param[in] rtClass2Status RT Class 2 status (0 only supported)
             * @param[in] rtClass3Status RT Class 3 status (0 only supported)
             */
            void SetLLDPData(
                MARTe::uint16           rtClass2Status,
                MARTe::uint16           rtClass3Status
            );

            /**
             * @brief Adds a slot to the Profinet slave
             * @param[in] slotNumber The Profinet slot number to be added
             */
            void AddSlot(
                MARTe::uint16 slotNumber);
            /**
             * @brief Adds a sub-slot to the Profinet slave.
             * @param[in] slotNumber The Profinet slot number where the slot resides.
             * @param[in] subslotNumber The Profinet sub-slot number to be added.
             * @param[in] expectedDataDirection Direction of the data from the slave point of view
             * @param[in] isDeviceAccessPoint Whether the slot is part of the device DAP.
             * @param[in] expectedInputSize Expected size in bytes for the input image.
             * @param[in] expectedOutputSize Expected size in bytes for the output image.
             */
            void AddSubslot(
                MARTe::uint16               slotNumber, 
                MARTe::uint16               subslotNumber,
                bool                        isDeviceAccessPoint,
                pnet_submodule_direction_t  expectedDataDirection,
                MARTe::uint16               expectedInputSize,
                MARTe::uint16               expectedOutputSize);

            /**
             * @brief Gets the subslot data, given its identificator
             * @param[in] slotNumber the slot number where the sub-slot resides
             * @param[in] subslotNumber the sub-slot number to get
             * @return The requested sub-slot or null if not exists.
             */
            pnet_subslot_t* GetSubslot(
                MARTe::uint16 slotNumber,
                MARTe::uint16 subslotNumber);

            /**
             * @brief Gets an iterator to traverse the slot/subslot data structure
             */
            SimpleLinkedListIterator<pnet_slot_t*>* GetAPIStructureIterator() { return apiStructure.slots.GetIterator(); }

            /**
             * @brief Initializes the adapter.
             */
            bool Initialize();
            //Endregion - Initialization methods

            //Region - Thread and task timer 
            //void MainThread();
            
            /**
             * @brief The main thread entry-point implementation
             * @param[in] inputFlagMask The input flag mask which contains the to-be-served events
             */
            MARTe::uint16 MainThread(MARTe::uint16 inputFlagMask);

            /**
             * @brief Cleanly aborts the Profinet session
             * @return true if Abort succeeded, false otherwise
             */
            bool AbortConnection();

            /**
             * @brief The Task timer tick event entry-point implementation
             */
            void TaskTimerTick();
            //Endregion - Thread and task timer

            //Region - Handling
            /**
             * @brief Gets the current LED status
             * @return A boolean representing the LED status in positive boolean logic (true ON - false OFF)
             */
            bool GetLedStatus();
            //Endregion - Handling

            /**
             * @brief Pointer to the cyclic event notification listener
             */
            ICyclicNotifiable *cyclicNotificationListener;
            /**
             * @brief Pointer to the event notification listener
             */
            MARTe::IProfinetEventNotifiable *profinetEventNotificationListener;

            /**
             * @brief Pointer to the Operational Signals (ready and LED) entry point
             */
            MARTe::IOperationalSignalsEntryPoint *opSignalsEntryPoint;

            //Region - Static utilities
            /**
             * @brief Returns the default RT Class 2 status
             * @return 0 as currently is not managed
             */
            static MARTe::uint16 GetDefaultLLDPRTClass2Status() { return 0; }

            /**
             * @brief Returns the default RT Class 3 status
             * @return 0 as currently not managed
             */
            static MARTe::uint16 GetDefaultLLDPRTClass3Status() { return 0; }

            /**
             * @brief Conveniently translates an os_ipaddr_t into a quad uint8 representation
             * @param[in] inputIpAddress The whole IP Address expressed as 32 bit integer
             * @param[out] a First octet of the IP Address
             * @param[out] b Second octet of the IP Address
             * @param[out] c Third octet of the IP Address
             * @param[out] d Fourth octet of the IP Address
             */
            static void IPv4U32toQuadU8(
                pnal_ipaddr_t     inputIpAddress, 
                uint8_t         *a, 
                uint8_t         *b, 
                uint8_t         *c, 
                uint8_t         *d
            );
            //Endregion - Static utilities
    };

    //Region - Callback mappers
    /**
     * @brief Mapper for the Connect Indication
     */
    int pnetds_connect_ind(pnet_t *net, void *arg, uint32_t arep, pnet_result_t *p_result);

    /**
     * @brief Mapper for the Release Indication
     */
    int pnetds_release_ind(pnet_t *net, void *arg, uint32_t arep, pnet_result_t *p_result);

    /**
     * @brief Mapper for the DControl indication
     */
    int pnetds_dcontrol_ind(pnet_t *net, void *arg, uint32_t arep, pnet_control_command_t control_command, pnet_result_t *p_result);

    /**
     * @brief Mapper for the CControl indication
     */
    int pnetds_ccontrol_cnf(pnet_t *net, void *arg, uint32_t arep, pnet_result_t *p_result);

    /**
     * @brief Mapper for the State indication
     */
    int pnetds_state_ind(pnet_t *net, void *arg, uint32_t arep, pnet_event_values_t state);

    /**
     * @brief Mapper for the Reset indication
     */
    int pnetds_reset_ind(pnet_t *net, void *arg, bool should_reset_application, uint16_t reset_mode);
    
    /**
     * @brief Mapper for the Signal LED indication
     */
    int pnetds_signal_led_ind(pnet_t *net, void *arg, bool led_state);

    /**
     * @brief Mapper for the Read indication
     */
    int pnetds_read_ind(pnet_t *net, void *arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t **pp_read_data, uint16_t *p_read_length, pnet_result_t *p_result);

    /**
     * @brief Mapper for the Write indication
     */
    int pnetds_write_ind(pnet_t *net, void *arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t *p_write_data, pnet_result_t *p_result);

    /**
     * @brief Mapper for the Expansion module indication
     */
    int pnetds_exp_module_ind(pnet_t *net, void *arg, uint32_t api, uint16_t slot, uint32_t module_ident);

    /**
     * @brief Mapper for the Expansion sub-module indication
     */
    int pnetds_exp_submodule_ind(pnet_t *net, void *arg, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t *p_exp_data);

    /**
     * @brief Mapper for the New data status indication
     */
    int pnetds_new_data_status_ind(pnet_t *net, void *arg, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status);

    /**
     * @brief Mapper for the alarm indication
     */
    int pnetds_alarm_ind(pnet_t *net, void *arg, uint32_t arep, const pnet_alarm_argument_t *p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t *p_data);

    /**
     * @brief Mapper for the Alarm confirmation
     */
    int pnetds_alarm_cnf(pnet_t *net, void *arg, uint32_t arep, const pnet_pnio_status_t *p_pnio_status);

    /**
     * @brief Mapper for the Alarm acknowledgement confirmation
     */
    int pnetds_alarm_ack_cnf(pnet_t *net, void *arg, uint32_t arep, int res);
    //Endregion - Callback mappers

    //Region - Timer
    //void mainThreadFunction(void *arg);
    //void taskTimer_tick(os_timer_t *timer, void *arg);
    //void ledTimer_tick(os_timer_t *timer, void *arg);
    //Endregion - Timer
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_ADAPTER_H */

