/**
 * @file ProfinetDataSourceAdapter.cpp
 * @brief Source file for class ProfinetDataSourceAdapter
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ProfinetDataSourceAdapter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace ProfinetDataSourceDriver {

    void ProfinetDataSourceAdapter::BaseInitializeCallbacks() {

        profinetConfigurationHandle->connect_cb = pnetds_connect_ind;
        profinetConfigurationHandle->release_cb = pnetds_release_ind;
        profinetConfigurationHandle->state_cb = pnetds_state_ind;
        profinetConfigurationHandle->dcontrol_cb = pnetds_dcontrol_ind;
        profinetConfigurationHandle->ccontrol_cb = pnetds_ccontrol_cnf;
        profinetConfigurationHandle->read_cb = pnetds_read_ind;
        profinetConfigurationHandle->write_cb = pnetds_write_ind;
        profinetConfigurationHandle->exp_module_cb = pnetds_exp_module_ind;
        profinetConfigurationHandle->exp_submodule_cb = pnetds_exp_submodule_ind;
        profinetConfigurationHandle->new_data_status_cb = pnetds_new_data_status_ind;
        profinetConfigurationHandle->alarm_ind_cb = pnetds_alarm_ind;
        profinetConfigurationHandle->alarm_cnf_cb = pnetds_alarm_cnf;
        profinetConfigurationHandle->alarm_ack_cnf_cb = pnetds_alarm_ack_cnf;
        profinetConfigurationHandle->reset_cb = pnetds_reset_ind;
        profinetConfigurationHandle->signal_led_cb = pnetds_signal_led_ind;
        
        callbacksUp = true;
    }

    bool ProfinetDataSourceAdapter::SetBaseData(
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
    ) {
        std::ostringstream outStr;
        bool returnValue = true;

        returnValue = (profinetConfigurationHandle != NULL);

        if(returnValue) {
            strncpy(profinetConfigurationHandle->product_name, productName.Buffer(), PNDS_MAX_PRODUCTNAME_SIZE);
            if(profinetConfigurationHandle->product_name[PNDS_MAX_PRODUCTNAME_SIZE - 1] != '\0') {
                profinetConfigurationHandle->product_name[PNDS_MAX_PRODUCTNAME_SIZE - 1] = '\0';
                loggerAdapter->Log(LogLevel_Error, "Product name is too long");
                returnValue = false;
            }
        }

        if(returnValue) {
            profinetConfigurationHandle->device_id.vendor_id_hi = static_cast<MARTe::uint8>(vendorIdHigh);
            profinetConfigurationHandle->device_id.vendor_id_lo = static_cast<MARTe::uint8>(vendorIdLow);
            profinetConfigurationHandle->device_id.device_id_hi = static_cast<MARTe::uint8>(deviceIdHigh);
            profinetConfigurationHandle->device_id.device_id_lo = static_cast<MARTe::uint8>(deviceIdLow);
            outStr << "Profinet device identifiers: VID_0x" << 
                std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(vendorIdHigh) << 
                std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(vendorIdLow) << 
                " DEVID_0x" << 
                std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(deviceIdHigh) << 
                std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(deviceIdLow);

            loggerAdapter->Log(LogLevel_Debug, outStr.str());
            outStr.str("");
            outStr.clear();

            profinetConfigurationHandle->oem_device_id.vendor_id_hi = static_cast<MARTe::uint8>(oemVendorIdHigh);
            profinetConfigurationHandle->oem_device_id.vendor_id_lo = static_cast<MARTe::uint8>(oemVendorIdLow);
            profinetConfigurationHandle->oem_device_id.device_id_hi = static_cast<MARTe::uint8>(oemDeviceIdHigh);
            profinetConfigurationHandle->oem_device_id.device_id_lo = static_cast<MARTe::uint8>(oemDeviceIdLow);
            outStr << "Profinet OEM device identifiers: VID_0x" << 
                std::setfill('0') << std::setw(2) << std::hex << (int)oemVendorIdHigh << 
                std::setfill('0') << std::setw(2) << std::hex << (int)oemVendorIdLow << 
                " DEVID_0x" << 
                std::setfill('0') << std::setw(2) << std::hex << (int)oemDeviceIdHigh << 
                std::setfill('0') << std::setw(2) << std::hex << (int)oemDeviceIdLow;

            loggerAdapter->Log(LogLevel_Debug, outStr.str());
            outStr.str("");
            outStr.clear();

            profinetConfigurationHandle->send_hello = true;
            profinetConfigurationHandle->if_cfg.ip_cfg.dhcp_enable = false;
            outStr << (profinetConfigurationHandle->send_hello?"Sending":"Not sending") << " HELLO, DHCP " << (profinetConfigurationHandle->if_cfg.ip_cfg.dhcp_enable?"enabled":"disabled");
            profinetConfigurationHandle->use_qualified_diagnosis = false; /* False means Extended = better supported by wireshark */
            profinetConfigurationHandle->min_device_interval = minimumDeviceInterval;
            outStr << "Profinet base data up";

            loggerAdapter->Log(LogLevel_Debug, outStr.str());

            returnValue = baseDataUp = true;
        }

        return returnValue;
    }

    pnet_cfg_t ProfinetDataSourceAdapter::GetProfinetConfigurationHandle() {
        pnet_cfg_t returnCfg = *profinetConfigurationHandle;

        return returnCfg;
    }

    bool ProfinetDataSourceAdapter::ValidateProfinetConfigurationHandle() {
        bool returnValue = false;
        const uint16_t imMask = ~(PNET_SUPPORTED_IM1 | PNET_SUPPORTED_IM2 | PNET_SUPPORTED_IM3 | PNET_SUPPORTED_IM4);

        if(profinetConfigurationHandle == NULL) {
            loggerAdapter->Log(LogLevel_Error, "Provided configuration is NULL");
        }
        else {
            returnValue = true;
        }

        if(returnValue) {
            if(profinetConfigurationHandle->tick_us == 0) {
                loggerAdapter->Log(LogLevel_Error, "Configured tick interval must not be 0");
                returnValue = false;
            }
            else {
                returnValue = true;
            }
        }

        if(returnValue) {
            if(profinetConfigurationHandle->min_device_interval == 0) {
                loggerAdapter->Log(LogLevel_Error, "Minimum device interval must be 0 < x <= 4096");
                returnValue = false;
            }
            else {
                returnValue = true;
            }
        }

        if(returnValue) {
            if(profinetConfigurationHandle->min_device_interval > 4096) {
                loggerAdapter->Log(LogLevel_Error, "Minimum device interval must be 0 < x <= 4096");
                returnValue = false;
            }
            else {
                returnValue = true;
            }
        }

        if(returnValue) {
            if((profinetConfigurationHandle->im_0_data.im_supported & imMask) > 0) {
                loggerAdapter->Log(LogLevel_Error, "The supported I&M setting is wrong");
                returnValue = false;
            }
            else {
                returnValue = true;
            }
        }

        return returnValue;
    }

    bool ProfinetDataSourceAdapter::SetIdentificationAndMaintainanceData(
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
    ) {
        std::ostringstream outStr;

        bool returnValue = (profinetConfigurationHandle != NULL);

        if(returnValue) {
            profinetConfigurationHandle->im_0_data.im_vendor_id_hi = static_cast<uint8_t>(vendorIdDataHigh);
            profinetConfigurationHandle->im_0_data.im_vendor_id_lo = static_cast<uint8_t>(vendorIdDataLow);
            profinetConfigurationHandle->im_0_data.im_hardware_revision = static_cast<uint16_t>(hardwareRevision);
            profinetConfigurationHandle->im_0_data.im_sw_revision_prefix = softwareRevision;
            
            outStr << "I&M device identifiers: VID_0x" << std::hex << 
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_vendor_id_hi) <<
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_vendor_id_lo) << std::endl;

            outStr << "Hardware revision " << 
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_hardware_revision) << std::endl;

            outStr << "Software revision prefix " << 
                static_cast<char>(profinetConfigurationHandle->im_0_data.im_sw_revision_prefix) << std::endl;

            loggerAdapter->Log(LogLevel_Debug, outStr.str());
            outStr.str("");
            outStr.clear();

            profinetConfigurationHandle->im_0_data.im_sw_revision_functional_enhancement = static_cast<uint8_t>(functionalEnhancement);
            profinetConfigurationHandle->im_0_data.im_sw_revision_bug_fix = static_cast<uint8_t>(bugFix);
            profinetConfigurationHandle->im_0_data.im_sw_revision_internal_change = static_cast<uint8_t>(internalChange);
            
            outStr  << "Software revision " <<
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_sw_revision_functional_enhancement) << "." << 
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_sw_revision_bug_fix) << "." << 
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_sw_revision_internal_change) << std::endl;
            loggerAdapter->Log(LogLevel_Debug, outStr.str());
            outStr.str("");
            outStr.clear();

            profinetConfigurationHandle->im_0_data.im_revision_counter = 0;  /* Only 0 allowed according to standard */

            profinetConfigurationHandle->im_0_data.im_profile_id = static_cast<uint16_t>(profileIdentifier);
            profinetConfigurationHandle->im_0_data.im_profile_specific_type = static_cast<uint16_t>(profileSpecificType);
            outStr << "Profile ID: " << static_cast<int>(profinetConfigurationHandle->im_0_data.im_profile_id) << 
                " Specific Type: " << static_cast<int>(profinetConfigurationHandle->im_0_data.im_profile_specific_type) << std::endl;
            loggerAdapter->Log(LogLevel_Debug, outStr.str());
            outStr.str("");
            outStr.clear();

            profinetConfigurationHandle->im_0_data.im_version_major = static_cast<uint8_t>(versionMajor);
            profinetConfigurationHandle->im_0_data.im_version_minor = static_cast<uint8_t>(versionMinor);
            outStr << "Version: " << 
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_version_major) << "." << 
                static_cast<int>(profinetConfigurationHandle->im_0_data.im_version_minor) << std::endl;
            loggerAdapter->Log(LogLevel_Debug, outStr.str());
            outStr.str("");
            outStr.clear();

            profinetConfigurationHandle->im_0_data.im_supported = PNET_SUPPORTED_IM1 | PNET_SUPPORTED_IM2 | PNET_SUPPORTED_IM3 | PNET_SUPPORTED_IM4;
            strncpy(profinetConfigurationHandle->im_0_data.im_order_id, orderIdentification.Buffer(), PNDS_MAX_IM0_ORDERID_SIZE);
            strncpy(profinetConfigurationHandle->im_0_data.im_serial_number, serialNumber.Buffer(), PNDS_MAX_IM0_SERNUM_SIZE);
            strncpy(profinetConfigurationHandle->im_1_data.im_tag_function, tagFunction.Buffer(), PNDS_MAX_IM1_TAGFUNC_SIZE);
            strncpy(profinetConfigurationHandle->im_1_data.im_tag_location, tagLocation.Buffer(), PNDS_MAX_IM1_TAGLOC_SIZE);
            strncpy(profinetConfigurationHandle->im_2_data.im_date, date.Buffer(), PNDS_MAX_IM2_DATE_SIZE);
            strncpy(profinetConfigurationHandle->im_3_data.im_descriptor, descriptor.Buffer(), PNDS_MAX_IM3_DESC_SIZE);
            strncpy(profinetConfigurationHandle->im_4_data.im_signature, signature.Buffer(), PNDS_MAX_IM4_SIGN_SIZE);  /* For functional safety only */

            /* Safety automatic string truncation */
            profinetConfigurationHandle->im_0_data.im_order_id[PNDS_MAX_IM0_ORDERID_SIZE - 1] = '\0';
            profinetConfigurationHandle->im_0_data.im_serial_number[PNDS_MAX_IM0_SERNUM_SIZE - 1] = '\0';
            profinetConfigurationHandle->im_1_data.im_tag_function[PNDS_MAX_IM1_TAGFUNC_SIZE - 1] = '\0';
            profinetConfigurationHandle->im_1_data.im_tag_location[PNDS_MAX_IM1_TAGLOC_SIZE - 1]  = '\0';
            profinetConfigurationHandle->im_2_data.im_date[PNDS_MAX_IM2_DATE_SIZE - 1]  = '\0';
            profinetConfigurationHandle->im_3_data.im_descriptor[PNDS_MAX_IM3_DESC_SIZE - 1]  = '\0';
            profinetConfigurationHandle->im_4_data.im_signature[PNDS_MAX_IM4_SIGN_SIZE - 1]  = '\0';

            outStr << std::endl <<
                "---- I&M TAG ----" << std::endl <<
                profinetConfigurationHandle->im_0_data.im_order_id << std::endl <<
                profinetConfigurationHandle->im_0_data.im_serial_number << std::endl <<
                profinetConfigurationHandle->im_1_data.im_tag_function << std::endl <<
                profinetConfigurationHandle->im_1_data.im_tag_location << std::endl <<
                profinetConfigurationHandle->im_2_data.im_date << std::endl <<
                profinetConfigurationHandle->im_3_data.im_descriptor << std::endl <<
                profinetConfigurationHandle->im_4_data.im_signature << std::endl <<
                "---- END TAG ----" << std::endl;

            outStr << "Id and Maintainance data up" << std::endl;
            loggerAdapter->Log(LogLevel_Debug, outStr.str());

            returnValue = idMaintDataUp = true;        
        }        
        else {
            loggerAdapter->Log(LogLevel_Error, "Cannot set I&M data on invalid Profinet configuration handle");
            returnValue = idMaintDataUp = false;
        }

        return returnValue;
    }

    void ProfinetDataSourceAdapter::SetLLDPData(
        MARTe::uint16           rtClass2Status,
        MARTe::uint16           rtClass3Status
    ) {
        //We force the LLDP port name accordingly, there's no need to ask user
        //things we can sort out by ourselves
        strcpy(profinetConfigurationHandle->if_cfg.ports[0].port_name, "port-001");
        profinetConfigurationHandle->if_cfg.ports[0].rtclass_2_status = rtClass2Status;
        profinetConfigurationHandle->if_cfg.ports[0].rtclass_3_status = rtClass3Status;

        lldpDataUp = true;

        loggerAdapter->Log(LogLevel_Debug, "LLDP data set");
    }

    bool ProfinetDataSourceAdapter::ConfigureNetworkData() {
        bool returnValue = false;
        std::ostringstream outStr;

        returnValue = (pnal_get_macaddress(ethInterface, &macAddress) == 0);
        outStr << "Getting MAC address "<<  ((returnValue)?"success":"failure") << " for interface " << ethInterface;
        loggerAdapter->Log((returnValue)?LogLevel_Info:LogLevel_Error, outStr.str());
        outStr.str("");
        outStr.clear();

        if(returnValue) {
            ipAddress = pnal_get_ip_address(ethInterface);
            returnValue = (ipAddress != 0);

            outStr << "Getting IP address "<<  ((returnValue)?"success":"failure");
            loggerAdapter->Log((returnValue)?LogLevel_Info:LogLevel_Error, outStr.str());
            outStr.str("");
            outStr.clear();
        }

        if(returnValue) {
            //As of b276cf7 commit for the p-net library, this function is a fake
            //Citing their comment "it is difficult to get gateway address"
            //and so they "wisely" assume the gateway as xxx.yyy.zzz.1 always.
            //No impact on the Profinet operations, however
            gateway = pnal_get_gateway(ethInterface);

            if(gateway == 0) {
                //Until p-net guys will fix  pnal_get_gateway 
                //We will never reach this point
                loggerAdapter->Log(LogLevel_Error, "Invalid gateway address for the network interface");
                returnValue = false;
            }
        }

        if(returnValue) {
            //Unfortunately we have no way telling if NM is correctly taken
            //However the call should fail way before, during the get ip address 
            //if things are messed with the network interface
            netmask = pnal_get_netmask(ethInterface);
        }

        if(returnValue) {
            IPv4U32toQuadU8(ipAddress, 
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.a,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.b,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.c,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.d
            );

            IPv4U32toQuadU8(gateway, 
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.a,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.b,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.c,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.d
            );

            IPv4U32toQuadU8(netmask, 
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.a,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.b,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.c,
                &profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.d
            );

            outStr << std::endl << "IP address "<<  
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.a << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.b << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.c << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_addr.d << std::endl;

            outStr << "Subnet Mask "<<  
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.a << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.b << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.c << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_mask.d  << std::endl;

            outStr << "Gateway "<<  
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.a << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.b << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.c << "." << 
                (int)profinetConfigurationHandle->if_cfg.ip_cfg.ip_gateway.d  << std::endl;

            //Here we can go smoothly as stationName was sanitized before
            strncpy(profinetConfigurationHandle->station_name, stationName, PNDS_MAX_STATION_NAME_SIZE);

            loggerAdapter->Log((returnValue)?LogLevel_Info:LogLevel_Error, outStr.str());
        }

        return returnValue;
    }

    bool ProfinetDataSourceAdapter::ConfigureProfinetPort() {
        //We have to configure both the main port (management) and the first port (physical)
        std::ostringstream outStr;

        bool returnValue = true;

        outStr << "ETH Interface: " << ethInterface << " MAC ";
        outStr << 
            std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(macAddress.addr[0]) << ":" <<
            std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(macAddress.addr[1]) << ":" <<
            std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(macAddress.addr[2]) << ":" <<
            std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(macAddress.addr[3]) << ":" <<
            std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(macAddress.addr[4]) << ":" <<
            std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(macAddress.addr[5]) << std::endl;

        loggerAdapter->Log(LogLevel_Info, outStr.str());

        memcpy(profinetConfigurationHandle->if_cfg.main_port.eth_addr.addr, macAddress.addr, sizeof(pnal_ethaddr_t));
        memcpy(profinetConfigurationHandle->if_cfg.ports[0].phy_port.eth_addr.addr, macAddress.addr, sizeof(pnal_ethaddr_t));
        
        strncpy(profinetConfigurationHandle->if_cfg.main_port.if_name, ethInterface, PNET_INTERFACE_NAME_MAX_SIZE);
        
        if(profinetConfigurationHandle->if_cfg.main_port.if_name[PNET_INTERFACE_NAME_MAX_SIZE - 1] != '\0') {
            //Something wrong is going on if the interface name is so long and non \0 terminated
            //However we truncate it but avoid things going on
            //We should never arrive here, as an invalid interface name should fail way before
            profinetConfigurationHandle->if_cfg.main_port.if_name[PNET_INTERFACE_NAME_MAX_SIZE - 1] = '\0';
            returnValue = false;
            //If there's no terminating null byte in the first n bytes of the src string, strncpy produces an unterminated string in dest
            //see man(3) strncpy
        }

        if(returnValue) {
            //Like before, things are not going to go this bad but we still keep it up
            strncpy(profinetConfigurationHandle->if_cfg.ports[0].phy_port.if_name, ethInterface, PNET_INTERFACE_NAME_MAX_SIZE);
            if(profinetConfigurationHandle->if_cfg.ports[0].phy_port.if_name[PNET_INTERFACE_NAME_MAX_SIZE - 1] != '\0') {
                profinetConfigurationHandle->if_cfg.ports[0].phy_port.if_name[PNET_INTERFACE_NAME_MAX_SIZE - 1] = '\0';
                returnValue = false;
            }          
        }
        
        return returnValue;
    }

    bool ProfinetDataSourceAdapter::FileExists(
        const char* pathName, 
        int attempts, 
        int intervalMs) {
        
        int nOfAttempts = 0;
        bool found = false;
        int sleepInterval = 1000 * intervalMs; //us to ms

        while(!found && (nOfAttempts++ < attempts)) {
            if(access(pathName, R_OK | W_OK) == 0 ) {
                //File is still there, let's wait
                usleep(sleepInterval);
            } else {
                //File has finally disappeared
                found = true;
                break;
            }
        }
        return found;
    }

    ProfinetDataSourceAdapter::ProfinetDataSourceAdapter(
        std::string     ethInterface, 
        uint32_t        periodicInterval,
        std::string     stationName,
        uint32_t        gearRatio,
        ILoggerAdapter  *loggerAdapter
    ) {
        bool integrityCheck = true;
        callbacksUp = baseDataUp = idMaintDataUp = lldpDataUp = profinetUp = false;
               
        //We have them disabled on startup
        cyclicNotificationListener = NULL;
        profinetEventNotificationListener = NULL;
        opSignalsEntryPoint = NULL;

        //We do not want stray pointers, especially when cleaning up after premature disasters
        profinetHandle = NULL;
        profinetConfigurationHandle = NULL;
        this->loggerAdapter = NULL;

        std::ostringstream outStr;

        //Region integrity checks

        //We must remove these files, unless something changes on the lib side
        bool fileA = (remove("pnet_data_im.bin") == 0);
        bool fileB = (remove("pnet_data_ip.bin") == 0);
        bool fileC = (remove("pnet_data_pdport_1.bin") == 0);

        //Remove function may not act immediately. Let's wait the removal.
        //Conversely, if remove fails, we can safely say the file was not there.
        int checksCount = 0;

        if(fileA) {
            fileA = FileExists("pnet_data_im.bin", 8, 250);
        }
        else {
            fileA = true;
        }

        if(fileB) {
            fileB = FileExists("pnet_data_ip.bin", 8, 250);
        }
        else {
            fileB = true;
        }

        if(fileC) {
            fileC = FileExists("pnet_data_pdport_1.bin", 8, 250);
        }
        else {
            fileC = true;
        }

        if(integrityCheck) {
            if(loggerAdapter == NULL) {
                //We have to say something to the user and unfortunately
                //cout is the only way, as the logger is NULL
                std::cout << "Error in ProfinetDataSourceAdapter constructor: the loggerAdapter points to NULL" << std::endl;
                integrityCheck = false;
            }
            else {
                this->loggerAdapter = loggerAdapter;
            }
        }

        if(integrityCheck) {
            integrityCheck = fileA && fileB && fileC;
            outStr << "File cleanup " << ((integrityCheck)?"succeeded":"failed");
            loggerAdapter->Log(((integrityCheck)?LogLevel_Info:LogLevel_Error), outStr.str());
            outStr.str("");
            outStr.clear();
        }

        if(integrityCheck) {
            if(periodicInterval == 0) {
                loggerAdapter->Log(LogLevel_Error, "The periodic interval must be > 0");
                integrityCheck = false;
            }
            else {
                this->periodicInterval = periodicInterval;
            }
        }

        if(integrityCheck) {
            if(stationName.empty()) {
                loggerAdapter->Log(LogLevel_Error, "The station name cannot be empty");
                integrityCheck = false;
            }
        }

        if(integrityCheck) {
            if(ethInterface.empty()) {
                loggerAdapter->Log(LogLevel_Error, "The ETH interface name cannot be empty");
                integrityCheck = false;
            }
        }

        if(integrityCheck) {
            if(gearRatio == 0) {
                loggerAdapter->Log(LogLevel_Error, "The gear ratio must be > 0");
                integrityCheck = false;
            }
            else {
                this->gearRatio = gearRatio;
            }
        }
        //EndRegion
        
        if(integrityCheck) {
            profinetConfigurationHandle = new pnet_cfg_t;
            if(profinetConfigurationHandle == NULL) {
                //These lines are really hard to reach for the coverage
                loggerAdapter->Log(LogLevel_Info, "Unable to allocate memory for the configuration handle structure");
                integrityCheck = false;
            }
            else {
                memset(profinetConfigurationHandle, 0, sizeof(pnet_cfg_t));
                profinetConfigurationHandle->cb_arg = (void *)this;
                strncpy(profinetConfigurationHandle->file_directory, "./", 240);
            }
        }
       
        if(integrityCheck) {
            //We use this magic number to say no AREP
            appRelationshipEndPoint = UINT32_MAX;
            outStr << "Running every " << periodicInterval << "us and cycling every " << gearRatio << " intervals";
            loggerAdapter->Log(LogLevel_Info, outStr.str());
            outStr.str("");
            outStr.clear();

            strncpy(this->stationName, stationName.c_str(), PNDS_MAX_STATION_NAME_SIZE);
            this->stationName[PNDS_MAX_STATION_NAME_SIZE - 1] = '\0';

            strncpy(this->ethInterface, ethInterface.c_str(), PNET_INTERFACE_NAME_MAX_SIZE);
            this->ethInterface[PNET_INTERFACE_NAME_MAX_SIZE - 1] = '\0';

            integrityCheck = ConfigureNetworkData();
            if(!integrityCheck) {
                loggerAdapter->Log(LogLevel_Error, "Network data configuration failure");
            }
        }
        
        if(integrityCheck) {
            loggerAdapter->Log(LogLevel_Info, "Network data configured");

            integrityCheck = ConfigureProfinetPort();
            if(!integrityCheck) {
                loggerAdapter->Log(LogLevel_Error, "Profinet port data configuration failure");
            }
        }

        if(integrityCheck) {
            loggerAdapter->Log(LogLevel_Info, "Profinet port data configured");
            
            //And finally we use the BaseInitializeCallbacks as a virtual return value
            //to avoid subsequent calls going up due to something missing here
            //as BaseInitializeCallbacks is a pure assigning function which bases on
            //things that, if failing, should fail way before here.
            //Further calls are infact blocked by the here-set callbacksUp class member
            BaseInitializeCallbacks();
        }
    }

    bool ProfinetDataSourceAdapter::CheckBaseNetworkingData() {
        return callbacksUp;
    }

    ProfinetDataSourceAdapter::~ProfinetDataSourceAdapter() {
        if(profinetConfigurationHandle != NULL) {
                        
            delete profinetConfigurationHandle;
            profinetHandle = NULL;
        }

        sleep(5);
    }

    bool ProfinetDataSourceAdapter::Initialize() {
        bool valid = callbacksUp && baseDataUp && idMaintDataUp && lldpDataUp;
        
        if(valid) {
            std::ostringstream outStr;
            loggerAdapter->Log(LogLevel_Info, "Configuration parameters up, going on with initialization");                 
            profinetConfigurationHandle->tick_us = periodicInterval;

            valid = ValidateProfinetConfigurationHandle();

            if(valid) {
                profinetHandle = pnet_init(profinetConfigurationHandle);
                
                if(profinetHandle == NULL) {
                    loggerAdapter->Log(LogLevel_Error, "Profinet initialization failure.");
                    profinetUp = false;
                }
                else {
                    loggerAdapter->Log(LogLevel_Info, "Profinet successfully initialized.");
                    profinetUp = true;
                }

                if(profinetUp) {
                    pnet_show(profinetHandle, 0x2010);
                    cycles = 0;
                }
            }
            else {
                loggerAdapter->Log(LogLevel_Error, "Configuration handle contains invalid values");
                profinetUp = false;
            }
            
        }
        else {
            std::cout << "CB: " << callbacksUp << " BD: " << baseDataUp << " IM: " << idMaintDataUp << " LLDP: " << lldpDataUp << std::endl;
            loggerAdapter->Log(LogLevel_Error, "Configuration preconditions not met. Cannot proceed.");
        }

        return profinetUp;
    }

    bool ProfinetDataSourceAdapter::AbortConnection() {
        bool returnValue = false;
        std::ostringstream outStr;

        if(profinetHandle != NULL) {
            if(appRelationshipEndPoint != UINT32_MAX) {
                //returnValue = (pnet_ar_abort(profinetHandle, appRelationshipEndPoint) == 0);
                //pnet_close(profinetHandle);
                sleep(2);
                
            }

            //returnValue = (pnet_factory_reset(profinetHandle));
            outStr << "ABORT " << returnValue << " on AREP " << appRelationshipEndPoint << std::endl;
            loggerAdapter->Log(LogLevel_Warning, outStr.str());
        }
        else {
            returnValue = true;
        }

        return returnValue;
    }

    void ProfinetDataSourceAdapter::AddSlot(
        MARTe::uint16 slotNumber) {
        
        pnet_slot_t *newSlot = new pnet_slot_t;
        std::ostringstream outStr;

        newSlot->slotNumber = static_cast<uint16_t>(slotNumber);
        newSlot->configAdded = true;

        apiStructure.slots.InsertTail(newSlot);
        
        outStr << "Adding slot: " << static_cast<uint16_t>(slotNumber);
        loggerAdapter->Log(LogLevel_Info, outStr.str());
    }

    void ProfinetDataSourceAdapter::AddSubslot(
        MARTe::uint16               slotNumber, 
        MARTe::uint16               subslotNumber,
        bool                        isDeviceAccessPoint,
        pnet_submodule_direction_t  expectedDataDirection,
        MARTe::uint16               expectedInputSize,
        MARTe::uint16               expectedOutputSize) {
        
        std::ostringstream outStr;

        uint16_t slotNumberParam = static_cast<uint16_t>(slotNumber);
        uint16_t subslotNumberParam = static_cast<uint16_t>(subslotNumber);
        uint16_t expectedInputSizeParam = static_cast<uint16_t>(expectedInputSize);
        uint16_t expectedOutputSizeParam = static_cast<uint16_t>(expectedOutputSize);

        SimpleLinkedListIterator<pnet_slot_t*> *slotsIterator = apiStructure.slots.GetIterator();
        pnet_slot_t *tempSlot = NULL;
        slotsIterator->First();
        bool found = false;

        while((tempSlot = slotsIterator->Next()) != NULL) {
            if(tempSlot->slotNumber == slotNumberParam) {
                pnet_subslot_t *newSubslot = new pnet_subslot_t;

                newSubslot->subslotNumber = subslotNumberParam;
                newSubslot->configAdded = true;
                newSubslot->isDeviceAccessPoint = isDeviceAccessPoint;
                
                switch(expectedDataDirection) {
                    case NoInputOutput:
                        newSubslot->expectedConfigurationData.data_dir = PNET_DIR_NO_IO;
                        break;
                    case Input:
                        newSubslot->expectedConfigurationData.data_dir = PNET_DIR_INPUT;
                        break;
                    case Output:
                        newSubslot->expectedConfigurationData.data_dir = PNET_DIR_OUTPUT;
                        break;
                    case InputOutput:
                        newSubslot->expectedConfigurationData.data_dir = PNET_DIR_IO;
                        break;
                    default:
                        newSubslot->expectedConfigurationData.data_dir = PNET_DIR_NO_IO;
                        break;
                }

                newSubslot->expectedConfigurationData.insize = expectedInputSizeParam;
                newSubslot->expectedConfigurationData.outsize = expectedOutputSizeParam;

                found = true;
                tempSlot->subslots.InsertTail(newSubslot);
                break;
            }
        }
        delete slotsIterator;
        outStr << "Slot " << slotNumberParam << " to insert subslot " << subslotNumberParam << " was " << (found?"found":"not found");
        loggerAdapter->Log(LogLevel_Info, outStr.str());
    }

    pnet_subslot_t* ProfinetDataSourceAdapter::GetSubslot(
        MARTe::uint16 slotNumber,
        MARTe::uint16 subslotNumber) {
        
        pnet_slot_t *tempSlot = NULL;
        pnet_subslot_t *tempSubslot = NULL;
        
        uint16_t convSlotNumber = static_cast<uint16_t>(slotNumber);
        uint16_t convSubslotNumber = static_cast<uint16_t>(subslotNumber);

        SimpleLinkedListIterator<pnet_slot_t*> *slotsIterator = apiStructure.slots.GetIterator();
        
        slotsIterator->First();
        bool found = false;

        while((tempSlot = slotsIterator->Next()) != NULL) {
            if(tempSlot->slotNumber == convSlotNumber) {
                SimpleLinkedListIterator<pnet_subslot_t*> *subslotsIterator = tempSlot->subslots.GetIterator();
                subslotsIterator->First();

                while((tempSubslot = subslotsIterator->Next()) != NULL) {
                    if(tempSubslot->subslotNumber == convSubslotNumber) {
                        found = true;
                        break;
                    }
                }
                delete subslotsIterator;
                if(found) {
                    break;
                }
            }
        }
        delete slotsIterator;

        if(!found) {
            tempSubslot = NULL;
        }

        return tempSubslot;
    }

    //Region - Helpers
    int ProfinetDataSourceAdapter::SetDataAndIOXS() {
        std::ostringstream outStr;
        pnet_slot_t *tempSlot = NULL;
        pnet_subslot_t *tempSubslot = NULL;
        uint8_t ioxs = PNET_IOXS_GOOD;
        int returnValue;

        SimpleLinkedListIterator<pnet_slot_t*> *slotsIterator = apiStructure.slots.GetIterator();
        slotsIterator->First();
        while((tempSlot = slotsIterator->Next()) != NULL) {
            if(tempSlot->IsUsed()) {
                SimpleLinkedListIterator<pnet_subslot_t*> *subslotsIterator = tempSlot->subslots.GetIterator();
                subslotsIterator->First();

                while((tempSubslot = subslotsIterator->Next()) != NULL) {
                    if(tempSubslot->IsUsed()) {
                        if(tempSubslot->IsInput() || tempSubslot->IsNoIO()) {
                            if(tempSubslot->IsInput() && (tempSubslot->inputData == NULL)) {
                                ioxs = PNET_IOXS_BAD;
                            }
                            returnValue = pnet_input_set_data_and_iops(profinetHandle, 0, tempSlot->slotNumber, tempSubslot->subslotNumber, tempSubslot->inputData, tempSubslot->masterConfigurationData.insize, ioxs);
                            outStr << ((returnValue == 0)?"Success":"Failure") << " setting data and IOPS for Slot: " << tempSlot-> slotNumber << "/Subslot: " << tempSubslot->subslotNumber << " [Size: " << tempSubslot->masterConfigurationData.insize << "]" << std::endl;
                        }

                        if(tempSubslot->IsOutput()) {
                            returnValue = pnet_output_set_iocs(profinetHandle, 0, tempSlot->slotNumber, tempSubslot->subslotNumber, PNET_IOXS_GOOD);
                            outStr << ((returnValue == 0)?"Success":"Failure") << " setting IOCS for Slot: " << tempSlot-> slotNumber << "/Subslot: " << tempSubslot->subslotNumber << std::endl;
                        }
                    }
                }
                delete subslotsIterator;
            }
        }
        delete slotsIterator;
        loggerAdapter->Log(LogLevel_Info, outStr.str());

        return returnValue;
    }

    int ProfinetDataSourceAdapter::SetCyclicData() {
        pnet_slot_t *tempSlot = NULL;
        pnet_subslot_t *tempSubslot = NULL;
        uint8_t ioxs = PNET_IOXS_GOOD;
        int returnValue;

        SimpleLinkedListIterator<pnet_slot_t*> *slotsIterator = apiStructure.slots.GetIterator();
        slotsIterator->First();
    
        while((tempSlot = slotsIterator->Next()) != NULL) {
            if(tempSlot->IsUsed()) {
                SimpleLinkedListIterator<pnet_subslot_t*> *subslotsIterator = tempSlot->subslots.GetIterator();
                subslotsIterator->First();

                while((tempSubslot = subslotsIterator->Next()) != NULL) {
                    if(tempSubslot->IsUsed()) {
                        if(tempSubslot->IsInput()) {
                            if(tempSubslot->inputData == NULL) {
                                ioxs = PNET_IOXS_BAD;
                            }
                            
                            returnValue = pnet_input_set_data_and_iops(profinetHandle, 0, tempSlot->slotNumber, tempSubslot->subslotNumber, tempSubslot->inputData, tempSubslot->masterConfigurationData.insize, ioxs);                            
                            returnValue += pnet_input_get_iocs(profinetHandle, 0, tempSlot->slotNumber, tempSubslot->subslotNumber, &ioxs);
                        }

                        if(tempSubslot->IsOutput()) {
                            bool newData = false;
                            uint16_t dataLength = tempSubslot->masterConfigurationData.outsize;
                            returnValue += pnet_output_get_data_and_iops(profinetHandle, 0, tempSlot->slotNumber, tempSubslot->subslotNumber, &newData, tempSubslot->outputData, &dataLength, &ioxs);
                        }
                    }
                }
                delete subslotsIterator;
            }
        }
        delete slotsIterator;

        if(returnValue == 0 && cyclicNotificationListener != NULL) {
            cyclicNotificationListener->NotifyCycle();
        }

        if(returnValue != 0) {
            returnValue = -1;
        }
        return returnValue;
    }
    //Endregion - Helpers

    //Region - Callbacks region
    int ProfinetDataSourceAdapter::ConnectIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Connect Indication (CI) Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::ReleaseIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Release Indication (RI) Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::DControlIndication(pnet_t * net, uint32_t arep, pnet_control_command_t control_command, pnet_result_t * p_result) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "DControl Indication (DCI) Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::CControlIndication(pnet_t * net, uint32_t arep, pnet_result_t * p_result) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "CControl Indication (CCI) Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::StateIndication(pnet_t * net, uint32_t arep, pnet_event_values_t state) { 
        profinetHandle = net;

        switch(state)
        {
            case PNET_EVENT_ABORT: 
            {
                std::ostringstream outStr;

                uint16_t errorClass = 0;
                uint16_t errorCode = 0;

                loggerAdapter->Log(LogLevel_Debug, "State Indication (SI) Event Abort.");
                if(pnet_get_ar_error_codes(profinetHandle, appRelationshipEndPoint, &errorClass, &errorCode) == 0) {
                    outStr << "Abort error, class " << errorClass << " code " << errorCode;
                }
                else {
                    outStr << "Abort error, cannot retrieve error class and code";
                }
                loggerAdapter->Log(LogLevel_Error, outStr.str());

                profinetEventNotificationListener->NotifyEvent(MARTe::ProfinetEventAbort);
                //appRelationshipEndPoint = UINT32_MAX;
                if(opSignalsEntryPoint != NULL) {
                    opSignalsEntryPoint->SetReady(false);
                }
                //os_event_set(events, Abort);
                break;
            }
            case PNET_EVENT_STARTUP:
            {
                loggerAdapter->Log(LogLevel_Debug, "State Indication (SI) Event Startup.");
                if(opSignalsEntryPoint != NULL) {
                    opSignalsEntryPoint->SetReady(false);
                }

                break;
            }
            case PNET_EVENT_PRMEND:
            {
                loggerAdapter->Log(LogLevel_Debug, "State Indication (SI) Event PRMEND.");
                profinetEventNotificationListener->NotifyEvent(MARTe::ProfinetEventReadyForData);
                //os_event_set(events, ReadyForData);
                appRelationshipEndPoint = arep;

                if(opSignalsEntryPoint != NULL) {
                    opSignalsEntryPoint->SetReady(true);
                }

                SetDataAndIOXS();

                pnet_set_provider_state(profinetHandle, true);
                break;
            }
            case PNET_EVENT_APPLRDY: 
            {
                loggerAdapter->Log(LogLevel_Debug, "State Indication (SI) Event Application Ready.");
                break;
            }
            case PNET_EVENT_DATA: 
            {
                loggerAdapter->Log(LogLevel_Debug, "State Indication (SI) Event Data.");
                break;
            }
        }  

        return 0;
    }

    int ProfinetDataSourceAdapter::ReadIndication(pnet_t * net, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t ** pp_read_data, uint16_t * p_read_length, pnet_result_t * p_result) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Read Indication Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::WriteIndication(pnet_t * net, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t * p_write_data, pnet_result_t * p_result) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Write Indication Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::ExpModuleIndication(pnet_t * net, uint32_t api, uint16_t slot, uint32_t module_ident) {
        
        std::ostringstream outStr;

        int returnValue = PNDS_RETURN_VALUE_ERROR;
        profinetHandle = net;
        outStr << "Expansion module indication callback for slot: " << slot << " with module identifier: " << module_ident << std::endl;

        SimpleLinkedListIterator<pnet_slot_t*> *slotsIterator = apiStructure.slots.GetIterator();
        slotsIterator->First();
        pnet_slot_t* tempSlot = NULL;

        while((tempSlot = slotsIterator->Next()) != NULL) {
            if(tempSlot->slotNumber == slot) {
                tempSlot->masterAdded = true;
                tempSlot->moduleIdentifier = module_ident;
                outStr << "Found, plugging slot: " << slot << " ident: " << module_ident << std::endl;
                returnValue = pnet_plug_module(profinetHandle, api, slot, module_ident);
                break;
            }
        }

        delete slotsIterator;
        outStr << "Slot plug " << ((returnValue == 0)?"succeeded":"failed") << std::endl;
        loggerAdapter->Log(LogLevel_Info, outStr.str());
        return returnValue;
    }

    int ProfinetDataSourceAdapter::ExpSubmoduleIndication(pnet_t * net, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t * p_exp_data) {
        
        std::ostringstream outStr;
        
        
        int returnValue = PNDS_RETURN_VALUE_ERROR;
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Expansion submodule indication Callback.");
        outStr << "Expansion submodule indication callback for slot " << slot << " subslot " << subslot << " with module ident " << module_ident << " and submodule ident " << submodule_ident << std::endl;

        SimpleLinkedListIterator<pnet_slot_t*> *slotsIterator = apiStructure.slots.GetIterator();
        slotsIterator->First();
        pnet_slot_t* tempSlot = NULL;

        while((tempSlot = slotsIterator->Next()) != NULL) {
            if((tempSlot->slotNumber == slot) && (tempSlot->moduleIdentifier == module_ident)) {
                SimpleLinkedListIterator<pnet_subslot_t*> *subslotsIterator = tempSlot->subslots.GetIterator();
                subslotsIterator->First();
                pnet_subslot_t *tempSubslot = NULL;

                while((tempSubslot = subslotsIterator->Next()) != NULL) {
                    if(tempSubslot->subslotNumber == subslot) {
                        tempSubslot->masterConfigurationData = *p_exp_data;
                        if(tempSubslot->IsMatchingExpectedConfiguration()) {
                            outStr << "Found, plugging slot: " << slot << " ident: " << module_ident << " subslot: " << subslot << " sub ident: " << submodule_ident << std::endl;
                            tempSubslot->masterAdded = true;
                            tempSubslot->masterConfigurationData = *p_exp_data;
                            returnValue = pnet_plug_submodule(profinetHandle, api, slot, subslot, module_ident, submodule_ident, tempSubslot->masterConfigurationData.data_dir, tempSubslot->masterConfigurationData.insize, tempSubslot->masterConfigurationData.outsize);
                        }
                        else {
                            outStr << "Found but configuration is not matching expected parameters. Plugging aborted." << std::endl;
                            outStr << "MARTe says " << tempSubslot->expectedConfigurationData.data_dir << " " << tempSubslot->expectedConfigurationData.insize << " " << tempSubslot->expectedConfigurationData.outsize << std::endl;
                            outStr << "PNET says " << p_exp_data->data_dir << " " << p_exp_data->insize << " " << p_exp_data->outsize << std::endl;
                        }

                        break;
                    }
                }
                delete subslotsIterator;
            }
        }
        delete slotsIterator;
        outStr << "Subslot plug " << ((returnValue == 0)?"succeeded":"failed") << std::endl;
        loggerAdapter->Log(LogLevel_Info, outStr.str());
        return returnValue;
    }

    int ProfinetDataSourceAdapter::NewDataStatusIndication(pnet_t * net, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status) {
        profinetHandle = net;

        loggerAdapter->Log(LogLevel_Debug, "New Data Status Indication Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::AlarmIndication(pnet_t * net, uint32_t arep, const pnet_alarm_argument_t * p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t * p_data) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Alarm Indication Callback.");

        //os_event_set(events, Alarm);
        profinetEventNotificationListener->NotifyEvent(MARTe::ProfinetEventAlarm);
        return 0;
    }

    int ProfinetDataSourceAdapter::AlarmConfirmation(pnet_t * net, uint32_t arep, const pnet_pnio_status_t * p_pnio_status) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Alarm Confirmation Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::AlarmAcknowledgementConfirmation(pnet_t * net, uint32_t arep, int res) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Alarm Acknowledge Confirmation Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::ResetIndication(pnet_t * net, bool should_reset_application, uint16_t reset_mode) {
        profinetHandle = net;
        loggerAdapter->Log(LogLevel_Debug, "Reset Indication Callback.");

        return 0;
    }

    int ProfinetDataSourceAdapter::SignalLedIndication(pnet_t * net, bool led_state) {
        profinetHandle = net;
        SetLedStatus(led_state);        
        return 0;
    }
    //Endregion - Callbacks region

    //Region - Signal Led Management
    void ProfinetDataSourceAdapter::SetLedStatus(bool ledStatus) {
        this->signalLEDStatus = ledStatus;
        if(opSignalsEntryPoint != NULL) {
            opSignalsEntryPoint->SetLED(ledStatus);
        }
    }

    bool ProfinetDataSourceAdapter::GetLedStatus() {
        return signalLEDStatus;
    }
    //Endregion - Signal Led Management

    //Region - Thread and task timer mapper

    MARTe::uint16 ProfinetDataSourceAdapter::MainThread(MARTe::uint16 inputFlagMask) {
        MARTe::uint16 outputFlagMask = inputFlagMask;

        if(inputFlagMask & ReadyForData) {
            loggerAdapter->Log(LogLevel_Debug, "Application Ready.");
            pnet_application_ready(profinetHandle, appRelationshipEndPoint);
            outputFlagMask = outputFlagMask & (~ReadyForData);
        }
        else if(inputFlagMask & Alarm) {
            loggerAdapter->Log(LogLevel_Info, "Alarm detected.");
            outputFlagMask = outputFlagMask & (~Alarm);
        }
        else if(inputFlagMask & Timer) {           
            if((appRelationshipEndPoint != UINT32_MAX) && ((cycles % gearRatio) ==  0)) {
                SetCyclicData();
            }
            cycles++;
            pnet_handle_periodic(profinetHandle);

            outputFlagMask = outputFlagMask & (~Timer);
        }
        else if(inputFlagMask & Abort) {
            loggerAdapter->Log(LogLevel_Debug, "Abort.");
            outputFlagMask = outputFlagMask & (~Abort);
            if(opSignalsEntryPoint != NULL) {
                opSignalsEntryPoint->Abort();
            }
        }

        return outputFlagMask;
    }

    void ProfinetDataSourceAdapter::TaskTimerTick() {
        profinetEventNotificationListener->NotifyEvent(MARTe::ProfinetEventTimer);
    }

    //Endregion - Thread and task timer mapper

    //Region - Callback mappers
    int pnetds_connect_ind(pnet_t *net, void *arg, uint32_t arep, pnet_result_t * p_result) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->ConnectIndication(net, arep, p_result);
    }

    int pnetds_release_ind(pnet_t *net, void *arg, uint32_t arep, pnet_result_t * p_result) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->ReleaseIndication(net, arep, p_result);
    }

    int pnetds_dcontrol_ind(pnet_t * net, void * arg, uint32_t arep, pnet_control_command_t control_command, pnet_result_t * p_result) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->DControlIndication(net, arep, control_command, p_result);
    }

    int pnetds_ccontrol_cnf(pnet_t * net, void * arg, uint32_t arep, pnet_result_t * p_result) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->CControlIndication(net, arep, p_result);
    }

    int pnetds_state_ind(pnet_t * net, void * arg, uint32_t arep, pnet_event_values_t state) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->StateIndication(net, arep, state);
    }

    int pnetds_read_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint8_t ** pp_read_data, uint16_t * p_read_length, pnet_result_t * p_result) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->ReadIndication(net, arep, api, slot, subslot, idx, sequence_number, pp_read_data, p_read_length, p_result);
    }

    int pnetds_write_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t api, uint16_t slot, uint16_t subslot, uint16_t idx, uint16_t sequence_number, uint16_t write_length, const uint8_t * p_write_data, pnet_result_t * p_result) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->WriteIndication(net, arep, api, slot, subslot, idx, sequence_number, write_length, p_write_data, p_result);
    }

    int pnetds_exp_module_ind(pnet_t * net, void * arg, uint32_t api, uint16_t slot, uint32_t module_ident) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->ExpModuleIndication(net, api, slot, module_ident);
    }

    int pnetds_exp_submodule_ind(pnet_t * net, void * arg, uint32_t api, uint16_t slot, uint16_t subslot, uint32_t module_ident, uint32_t submodule_ident, const pnet_data_cfg_t * p_exp_data) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->ExpSubmoduleIndication(net, api, slot, subslot, module_ident, submodule_ident, p_exp_data);
    }

    int pnetds_new_data_status_ind(pnet_t * net, void * arg, uint32_t arep, uint32_t crep, uint8_t changes, uint8_t data_status) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->NewDataStatusIndication(net, arep, crep, changes, data_status);
    }

    int pnetds_alarm_ind(pnet_t * net, void * arg, uint32_t arep, const pnet_alarm_argument_t * p_alarm_argument, uint16_t data_len, uint16_t data_usi, const uint8_t * p_data) {
         return static_cast<ProfinetDataSourceAdapter*>(arg)->AlarmIndication(net, arep, p_alarm_argument, data_len, data_usi, p_data);
    }

    int pnetds_alarm_cnf(pnet_t * net, void * arg, uint32_t arep, const pnet_pnio_status_t * p_pnio_status) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->AlarmConfirmation(net, arep, p_pnio_status);
    }

    int pnetds_alarm_ack_cnf(pnet_t * net, void * arg, uint32_t arep, int res) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->AlarmAcknowledgementConfirmation(net, arep, res);
    }

    int pnetds_reset_ind(pnet_t * net, void * arg, bool should_reset_application, uint16_t reset_mode) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->ResetIndication(net, should_reset_application, reset_mode);
    }

    int pnetds_signal_led_ind(pnet_t * net, void * arg, bool led_state) {
        return static_cast<ProfinetDataSourceAdapter*>(arg)->SignalLedIndication(net, led_state);
    }
    //Endregion - Callback mappers

    //Region - Static utilities
    void ProfinetDataSourceAdapter::IPv4U32toQuadU8(
        pnal_ipaddr_t     inputIpAddress, 
        uint8_t         *a, 
        uint8_t         *b, 
        uint8_t         *c, 
        uint8_t         *d
    ) {
        *a = ((inputIpAddress >> 24) & 0xFF);
        *b = ((inputIpAddress >> 16) & 0xFF);
        *c = ((inputIpAddress >> 8) & 0xFF);
        *d = (inputIpAddress & 0xFF);
    }
    //Endregion - Static utilities

}
