/**
 * @file pnetdsadapter.h
 * @brief   Adaptation layer between MARTe DataSource and Profinet underlying library (p-net).
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

 * @details     Implements an intermediate adapter between the MARTe DataSource and the Profinet library.
 *              As the p-net library is implemented in C, the library is first 1:1 mapped into C++.
 *              Convenient function for library and bus management are provided to upper layers, with
 *              the MARTe DataSource approach in mind.
 */

#ifndef __PROFINET_DATASOURCE_ADAPTER_H__
#define __PROFINET_DATASOURCE_ADAPTER_H__

#define PNET_MAX_DIRECTORYPATH_LENGTH 4095

#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <osal.h>
#include <pnal.h>
#include <pnet_api.h>

#include "pnetdstypes.h"
#include "ILoggerAdapter.h"
#include "CompilerTypes.h"
#include "StreamString.h"
#include "MutexSem.h"
#include "IProfinetEventNotifiable.h"
#include "ProfinetEventType.h"
#include "ICyclicNotifiable.h"
#include "IOperationalSignalsEntryPoint.h"

#define PNDS_MAX_INTERFACE_NAME_SIZE    32
#define PNDS_MAX_STATION_NAME_SIZE      241
#define PNDS_MAX_SUPPORTED_SUBSLOTS     5

#define PNDS_RETURN_VALUE_ERROR         -1
#define PNDS_RETURN_VALUE_SUCCESS       0


namespace ProfinetDataSourceDriver
{
    class ProfinetDataSourceAdapter {

        private:
            ProfinetDataSourceAdapter() {;}

            ILoggerAdapter  *loggerAdapter;
            pnet_t          *profinetHandle;
            pnet_cfg_t      *profinetConfigurationHandle;
            pnet_api_t      apiStructure;

            uint32_t        periodicInterval;
            uint32_t        appRelationshipEndPoint;

            os_ethaddr_t    macAddress;
            os_ipaddr_t     ipAddress;
            os_ipaddr_t     netmask;
            os_ipaddr_t     gateway;

            bool            threadUp;
            os_thread_t     *mainThread;
            os_timer_t      *taskTimer;
            os_timer_t      *ledTimer;
            os_event_t      *events;
            uint32_t        eventMask;
            uint32_t        eventFlags;

            uint32_t        gearRatio;

            bool            callbacksUp;
            bool            baseDataUp;
            bool            idMaintDataUp;
            bool            lldpDataUp;
            bool            profinetUp;

            char            ethInterface[PNDS_MAX_INTERFACE_NAME_SIZE];
            char            stationName[PNDS_MAX_STATION_NAME_SIZE];

            bool            signalLEDStatus;
            
            uint32_t cycles;

            //Region - Callbacks            
            void BaseInitializeCallbacks();

            int ConnectIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result);
            int ReleaseIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result);
            int DControlIndication(pnet_t * net, uint32_t arep, pnet_control_command_t control_command, pnet_result_t * p_result);
            int CControlIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result);
            int StateIndication(pnet_t * net, uint32_t arep, pnet_event_values_t state); 
            int ReadIndication(pnet_t * net, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t ** pp_read_data, uint16_t * p_read_length, pnet_result_t * p_result);
            int WriteIndication(pnet_t * net, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t * p_write_data, pnet_result_t * p_result);
            int ExpModuleIndication(pnet_t * net, uint32_t api, uint16_t slot, uint32_t module_ident); 
            int ExpSubmoduleIndication(pnet_t * net, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t * p_exp_data); 
            int NewDataStatusIndication(pnet_t * net, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status);
            int AlarmIndication(pnet_t * net, uint32_t arep, const pnet_alarm_argument_t * p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t * p_data);
            int AlarmConfirmation(pnet_t * net, uint32_t arep, const pnet_pnio_status_t * p_pnio_status);
            int AlarmAcknowledgementConfirmation(pnet_t * net, uint32_t arep, int res);
            int ResetIndication(pnet_t * net, bool should_reset_application, uint16_t reset_mode);
            int SignalLedIndication(pnet_t * net, bool led_state);
            
            //Endregion - Callbacks



            //Region - Signal Led management
            void SetLedStatus(bool ledStatus);
            //Endregion - Signal Led management

            //Region - Callback mappers
            friend int pnetds_connect_ind(pnet_t * net, void * arg, uint32_t arep, pnet_result_t * p_result);
            friend int pnetds_release_ind(pnet_t * net, void * arg, uint32_t arep, pnet_result_t * p_result);
            friend int pnetds_dcontrol_ind(pnet_t * net, void * arg, uint32_t arep, pnet_control_command_t control_command, pnet_result_t * p_result);          
            friend int pnetds_ccontrol_cnf(pnet_t * net, void * arg, uint32_t arep, pnet_result_t * p_result);
            friend int pnetds_state_ind(pnet_t * net, void * arg, uint32_t arep, pnet_event_values_t state); 
            friend int pnetds_read_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t ** pp_read_data, uint16_t * p_read_length, pnet_result_t * p_result);
            friend int pnetds_write_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t * p_write_data, pnet_result_t * p_result);
            friend int pnetds_exp_module_ind(pnet_t * net, void * arg, uint32_t api, uint16_t slot, uint32_t module_ident); 
            friend int pnetds_exp_submodule_ind(pnet_t * net, void * arg, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t * p_exp_data); 
            friend int pnetds_new_data_status_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status);
            friend int pnetds_alarm_ind(pnet_t * net, void * arg, uint32_t arep, const pnet_alarm_argument_t * p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t * p_data);
            friend int pnetds_alarm_cnf(pnet_t * net, void * arg, uint32_t arep, const pnet_pnio_status_t * p_pnio_status);
            friend int pnetds_alarm_ack_cnf(pnet_t * net, void * arg, uint32_t arep, int res);
            friend int pnetds_reset_ind(pnet_t * net, void * arg, bool should_reset_application, uint16_t reset_mode);
            friend int pnetds_signal_led_ind(pnet_t * net, void * arg, bool led_state);
            //Endregion - Callback mappers

            //Region - Threading
            friend void mainThreadFunction(void * arg);
            friend void pnetds_main_thread(void * arg);
            friend void taskTimer_tick(os_timer_t *timer, void *arg);
            friend void ledTimer_tick(os_timer_t *timer, void *arg);
            //Endregion - Threading

            //Region - Helpers
            int SetDataAndIOXS();
            int SetCyclicData();
            //Endregion - Helpers

        public:
            ProfinetDataSourceAdapter(
                std::string     ethInterface, 
                uint32_t        periodicInterval,
                std::string     stationName,
                uint32_t        gearRatio,
                ILoggerAdapter  *loggerAdapter
            );
            //Region - Initialization methods
            
            bool SetBaseData(
                MARTe::uint8           vendorIdHigh,
                MARTe::uint8           vendorIdLow,
                MARTe::uint8           deviceIdHigh,
                MARTe::uint8           deviceIdLow,
                MARTe::uint8           oemVendorIdHigh,
                MARTe::uint8           oemVendorIdLow,
                MARTe::uint8           oemDeviceIdHigh,
                MARTe::uint8           oemDeviceIdLow,
                MARTe::StreamString    deviceVendor,
                MARTe::StreamString    manufacturerSpecificString,
                MARTe::uint16          minimumDeviceInterval
            );

            void SetIdentificationAndMaintainanceData(
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
                MARTe::StreamString    tagFunction,
                MARTe::StreamString    tagLocation,
                MARTe::StreamString    date,
                MARTe::StreamString    descriptor,
                MARTe::StreamString    signature
            );

            void SetLLDPData(
                MARTe::StreamString     lldpPortIdentifier,
                MARTe::uint16           rtClass2Status,
                MARTe::uint16           rtClass3Status,
                MARTe::uint8            autoNegotiationCapability,
                MARTe::uint16           autoNegotiationSpeed,
                MARTe::uint16           mauType
            );

            void AddSlot(
                MARTe::uint16 slotNumber);

            void AddSubslot(
                MARTe::uint16               slotNumber, 
                MARTe::uint16               subslotNumber,
                bool                        isDeviceAccessPoint,
                pnet_submodule_direction_t  expectedDataDirection,
                MARTe::uint16               expectedInputSize,
                MARTe::uint16               expectedOutputSize);

            pnet_subslot_t* GetSubslot(
                MARTe::uint16 slotNumber,
                MARTe::uint16 subslotNumber);

            SimpleLinkedListIterator<pnet_slot_t*>* GetAPIStructureIterator() { return apiStructure.slots.GetIterator(); }

            bool Initialize();
            //Endregion - Initialization methods

            //Region - Thread and task timer 
            void MainThread();
            MARTe::uint16 MainThread(MARTe::uint16 inputFlagMask);
            void TaskTimerTick();
            //Endregion - Thread and task timer

            //Region - Handling
            bool GetLedStatus();
            //Endregion - Handling

            ICyclicNotifiable *cyclicNotificationListener;
            MARTe::IProfinetEventNotifiable *profinetEventNotificationListener;
            MARTe::IOperationalSignalsEntryPoint *opSignalsEntryPoint;

            //Region - Static utilities
            static MARTe::uint16 GetDefaultLLDPRTClass2Status() { return 0; }
            static MARTe::uint16 GetDefaultLLDPRTClass3Status() { return 0; }
            static MARTe::uint8 GetDefaultLLDPAutonegotiationCapability() { return PNET_LLDP_AUTONEG_SUPPORTED | PNET_LLDP_AUTONEG_ENABLED; }
            static MARTe::uint16 GetDefaultLLDPAutonegotiationSpeed() { return PNET_LLDP_AUTONEG_CAP_100BaseTX_HALF_DUPLEX | PNET_LLDP_AUTONEG_CAP_100BaseTX_FULL_DUPLEX; }
            static MARTe::uint16 GetDefaultLLDPMAUType() { return PNET_MAU_COPPER_100BaseTX_FULL_DUPLEX; }

            static void IPv4U32toQuadU8(
                os_ipaddr_t     inputIpAddress, 
                uint8_t         *a, 
                uint8_t         *b, 
                uint8_t         *c, 
                uint8_t         *d
            );
            //Endregion - Static utilities
    };

    //Region - Callback mappers
    int pnetds_connect_ind(pnet_t *net, void *arg, uint32_t arep, pnet_result_t *p_result);
    int pnetds_release_ind(pnet_t *net, void *arg, uint32_t arep, pnet_result_t *p_result);
    int pnetds_dcontrol_ind(pnet_t *net, void *arg, uint32_t arep, pnet_control_command_t control_command, pnet_result_t *p_result);
    int pnetds_ccontrol_cnf(pnet_t *net, void *arg, uint32_t arep, pnet_result_t *p_result);
    int pnetds_state_ind(pnet_t *net, void *arg, uint32_t arep, pnet_event_values_t state);
    int pnetds_reset_ind(pnet_t *net, void *arg, bool should_reset_application, uint16_t reset_mode);
    int pnetds_signal_led_ind(pnet_t *net, void *arg, bool led_state);
    int pnetds_read_ind(pnet_t *net, void *arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t **pp_read_data, uint16_t *p_read_length, pnet_result_t *p_result);
    int pnetds_write_ind(pnet_t *net, void *arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t *p_write_data, pnet_result_t *p_result);
    int pnetds_exp_module_ind(pnet_t *net, void *arg, uint32_t api, uint16_t slot, uint32_t module_ident);
    int pnetds_exp_submodule_ind(pnet_t *net, void *arg, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t *p_exp_data);
    int pnetds_new_data_status_ind(pnet_t *net, void *arg, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status);
    int pnetds_alarm_ind(pnet_t *net, void *arg, uint32_t arep, const pnet_alarm_argument_t *p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t *p_data);
    int pnetds_alarm_cnf(pnet_t *net, void *arg, uint32_t arep, const pnet_pnio_status_t *p_pnio_status);
    int pnetds_alarm_ack_cnf(pnet_t *net, void *arg, uint32_t arep, int res);
    //Endregion - Callback mappers

    //Region - Timer
    void mainThreadFunction(void *arg);
    void taskTimer_tick(os_timer_t *timer, void *arg);
    void ledTimer_tick(os_timer_t *timer, void *arg);
    //Endregion - Timer
}

#endif
