#include "ProfinetDataSourceAdapter.h"

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

        loggerAdapter->Log(LogLevel_Debug, "ProfinetDataSource Callbacks connected");
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
        MARTe::StreamString    deviceVendor,
        MARTe::StreamString    manufacturerSpecificString,
        MARTe::uint16          minimumDeviceInterval
    ) {
        std::ostringstream outStr;

        //Region - Device configuration
        profinetConfigurationHandle->device_id.vendor_id_hi = static_cast<MARTe::uint8>(vendorIdHigh);
        profinetConfigurationHandle->device_id.vendor_id_lo = static_cast<MARTe::uint8>(vendorIdLow);
        profinetConfigurationHandle->device_id.device_id_hi = static_cast<MARTe::uint8>(deviceIdHigh);
        profinetConfigurationHandle->device_id.device_id_lo = static_cast<MARTe::uint8>(deviceIdLow);
        outStr << "Profinet device identifiers: VID_0x" << std::hex << (int)vendorIdHigh << std::hex << (int)vendorIdLow << " DEVID_0x" << std::hex << (int)deviceIdHigh << std::hex << (int)deviceIdLow << std::endl;

        profinetConfigurationHandle->oem_device_id.vendor_id_hi = static_cast<MARTe::uint8>(oemVendorIdHigh);
        profinetConfigurationHandle->oem_device_id.vendor_id_lo = static_cast<MARTe::uint8>(oemVendorIdLow);
        profinetConfigurationHandle->oem_device_id.device_id_hi = static_cast<MARTe::uint8>(oemDeviceIdHigh);
        profinetConfigurationHandle->oem_device_id.device_id_lo = static_cast<MARTe::uint8>(oemDeviceIdLow);
        outStr << "Profinet OEM device identifiers: VID_0x" << std::hex << (int)oemVendorIdHigh << std::hex << (int)oemVendorIdLow << " DEVID_0x" << std::hex << (int)oemDeviceIdHigh << std::hex << (int)oemDeviceIdLow << std::endl;
        
        strncpy(profinetConfigurationHandle->device_vendor, deviceVendor.Buffer(), 21);
        strncpy(profinetConfigurationHandle->manufacturer_specific_string, manufacturerSpecificString.Buffer(), 241);
        
        /* Safety string truncation */
        profinetConfigurationHandle->device_vendor[20] = '\0';
        profinetConfigurationHandle->manufacturer_specific_string[240] = '\0';

        outStr << "Vendor: " << deviceVendor.Buffer() << std::endl;
        outStr << "Manufacturer specific string: " << manufacturerSpecificString.Buffer() << std::endl;

        //Endregion - Device configuration

        //Region - Timing
        profinetConfigurationHandle->min_device_interval = minimumDeviceInterval;  /* Corresponds to 1 ms */
        outStr << "Minimum device interval: " << minimumDeviceInterval << std::endl;
        //Endregion - Timing

        //Region - Network configuration
        profinetConfigurationHandle->send_hello = 1;
        profinetConfigurationHandle->dhcp_enable = 0;
        outStr << (profinetConfigurationHandle->send_hello?"Sending":"Not sending") << " HELLO, DHCP " << (profinetConfigurationHandle->dhcp_enable?"enabled":"disabled") << std::endl;
        //Endregion - Network configuration

        baseDataUp = true;
        outStr << "Profinet base data up" << std::endl;

        loggerAdapter->Log(LogLevel_Debug, outStr.str());

        return baseDataUp;
    }

    void ProfinetDataSourceAdapter::SetIdentificationAndMaintainanceData(
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
    ) {
        std::ostringstream outStr;

        profinetConfigurationHandle->im_0_data.im_vendor_id_hi = static_cast<uint8_t>(vendorIdDataHigh);
        profinetConfigurationHandle->im_0_data.im_vendor_id_lo = static_cast<uint8_t>(vendorIdDataLow);
        outStr << "I&M device identifiers: VID_0x" << std::hex << (int)profinetConfigurationHandle->im_0_data.im_vendor_id_hi << std::hex << (int)profinetConfigurationHandle->im_0_data.im_vendor_id_lo << std::endl;
        loggerAdapter->Log(LogLevel_Debug, outStr.str());
        outStr.str("");
        outStr.clear();

        profinetConfigurationHandle->im_0_data.im_hardware_revision = static_cast<uint16_t>(hardwareRevision);

        profinetConfigurationHandle->im_0_data.im_sw_revision_prefix = softwareRevision;
        profinetConfigurationHandle->im_0_data.im_sw_revision_functional_enhancement = static_cast<uint8_t>(functionalEnhancement);
        profinetConfigurationHandle->im_0_data.im_sw_revision_bug_fix = static_cast<uint8_t>(bugFix);
        profinetConfigurationHandle->im_0_data.im_sw_revision_internal_change = static_cast<uint8_t>(internalChange);
        outStr  << "REV HW: " << profinetConfigurationHandle->im_0_data.im_hardware_revision << 
                " SW: " << profinetConfigurationHandle->im_0_data.im_sw_revision_prefix << "." << 
                profinetConfigurationHandle->im_0_data.im_sw_revision_functional_enhancement << "." << 
                profinetConfigurationHandle->im_0_data.im_sw_revision_bug_fix << "." << 
                profinetConfigurationHandle->im_0_data.im_sw_revision_internal_change << std::endl;
        loggerAdapter->Log(LogLevel_Debug, outStr.str());
        outStr.str("");
        outStr.clear();

        profinetConfigurationHandle->im_0_data.im_revision_counter = 0;  /* Only 0 allowed according to standard */

        profinetConfigurationHandle->im_0_data.im_profile_id = static_cast<uint16_t>(profileIdentifier);
        profinetConfigurationHandle->im_0_data.im_profile_specific_type = static_cast<uint16_t>(profileSpecificType);
        outStr << "Profile ID:" << profinetConfigurationHandle->im_0_data.im_profile_id << " Type: " << profinetConfigurationHandle->im_0_data.im_profile_specific_type << std::endl;
        loggerAdapter->Log(LogLevel_Debug, outStr.str());
        outStr.str("");
        outStr.clear();

        profinetConfigurationHandle->im_0_data.im_version_major = static_cast<uint8_t>(versionMajor);
        profinetConfigurationHandle->im_0_data.im_version_minor = static_cast<uint8_t>(versionMinor);
        outStr << "Version: " << profinetConfigurationHandle->im_0_data.im_version_major << "." << profinetConfigurationHandle->im_0_data.im_version_minor << std::endl;
        loggerAdapter->Log(LogLevel_Debug, outStr.str());
        outStr.str("");
        outStr.clear();

        profinetConfigurationHandle->im_0_data.im_supported = 0x001e;        /* Only I&M0..I&M4 supported */

        strncpy(profinetConfigurationHandle->im_1_data.im_tag_function, tagFunction.Buffer(), 33);
        strncpy(profinetConfigurationHandle->im_1_data.im_tag_location, tagLocation.Buffer(), 23);
        strncpy(profinetConfigurationHandle->im_2_data.im_date, date.Buffer(), 17);
        strncpy(profinetConfigurationHandle->im_3_data.im_descriptor, descriptor.Buffer(), 55);
        strncpy(profinetConfigurationHandle->im_4_data.im_signature, signature.Buffer(), 54);  /* For functional safety only */

        /* Safety automatic string truncation */
        profinetConfigurationHandle->im_1_data.im_tag_function[32] = '\0';
        profinetConfigurationHandle->im_1_data.im_tag_location[22]  = '\0';
        profinetConfigurationHandle->im_2_data.im_date[16]  = '\0';
        profinetConfigurationHandle->im_3_data.im_descriptor[54]  = '\0';
        profinetConfigurationHandle->im_4_data.im_signature[53]  = '\0';

        outStr << "---- TAG ----" << std::endl;
        outStr  << profinetConfigurationHandle->im_1_data.im_tag_function << std::endl <<
                profinetConfigurationHandle->im_1_data.im_tag_location << std::endl <<
                profinetConfigurationHandle->im_2_data.im_date << std::endl <<
                profinetConfigurationHandle->im_3_data.im_descriptor << std::endl <<
                profinetConfigurationHandle->im_4_data.im_signature << std::endl;
        outStr << "---- END TAG ----" << std::endl;
        outStr << "Id and Maintainance data up" << std::endl;
        loggerAdapter->Log(LogLevel_Debug, outStr.str());

        idMaintDataUp = true;

    }

    void ProfinetDataSourceAdapter::SetLLDPData(
        MARTe::StreamString     lldpPortIdentifier,
        MARTe::uint16           rtClass2Status,
        MARTe::uint16           rtClass3Status,
        MARTe::uint8            autoNegotiationCapability,
        MARTe::uint16           autoNegotiationSpeed,
        MARTe::uint16           mauType
    ) {
        strcpy(profinetConfigurationHandle->lldp_cfg.port_id, lldpPortIdentifier.Buffer());
        profinetConfigurationHandle->lldp_cfg.rtclass_2_status = rtClass2Status;
        profinetConfigurationHandle->lldp_cfg.rtclass_3_status = rtClass3Status;
        profinetConfigurationHandle->lldp_cfg.cap_aneg = autoNegotiationCapability;
        profinetConfigurationHandle->lldp_cfg.cap_phy = autoNegotiationSpeed;
        profinetConfigurationHandle->lldp_cfg.mau_type = mauType;

        lldpDataUp = true;

        loggerAdapter->Log(LogLevel_Debug, "I&M data set");
    }

    ProfinetDataSourceAdapter::ProfinetDataSourceAdapter(
        std::string     ethInterface, 
        uint32_t        periodicInterval,
        std::string     stationName,
        uint32_t        gearRatio,
        ILoggerAdapter  *loggerAdapter
    ) {
        this->loggerAdapter = loggerAdapter;
        std::ostringstream outStr;

        profinetConfigurationHandle = new pnet_cfg_t;
        profinetConfigurationHandle->cb_arg = (void *)this;
        
        this->periodicInterval = periodicInterval;
        this->gearRatio = gearRatio;

        outStr << "Running every " << periodicInterval << "us and cycling every " << gearRatio << " intervals" << std::endl;
        loggerAdapter->Log(LogLevel_Info, outStr.str());

        appRelationshipEndPoint = UINT32_MAX;
        
        int macRes = os_get_macaddress(ethInterface.c_str(), &macAddress);
        outStr << "Getting MAC address " << ((macRes == 0)?"success":"failure");

        ipAddress = os_get_ip_address(ethInterface.c_str());
        netmask = os_get_netmask(ethInterface.c_str());
        gateway = os_get_gateway(ethInterface.c_str());
        
        BaseInitializeCallbacks();

        strncpy(this->stationName, stationName.c_str(), PNDS_MAX_STATION_NAME_SIZE);
        this->stationName[PNDS_MAX_STATION_NAME_SIZE - 1] = '\0';

        strncpy(this->ethInterface, ethInterface.c_str(), PNDS_MAX_INTERFACE_NAME_SIZE);
        this->ethInterface[PNDS_MAX_INTERFACE_NAME_SIZE - 1] = '\0';

        eventMask = ReadyForData | Timer | Alarm | Abort;
        eventFlags = 0;
    }

    bool ProfinetDataSourceAdapter::Initialize() {
        bool valid = callbacksUp && baseDataUp && idMaintDataUp && lldpDataUp;
        
        if(valid) {
            
            memcpy(&profinetConfigurationHandle->eth_addr, macAddress.addr, sizeof(os_ethaddr_t));
            strncpy(profinetConfigurationHandle->station_name, stationName, PNDS_MAX_STATION_NAME_SIZE);
            
            ProfinetDataSourceAdapter::IPv4U32toQuadU8(
                ipAddress, 
                &profinetConfigurationHandle->ip_addr.a,
                &profinetConfigurationHandle->ip_addr.b,
                &profinetConfigurationHandle->ip_addr.c,
                &profinetConfigurationHandle->ip_addr.d
            );

            ProfinetDataSourceAdapter::IPv4U32toQuadU8(
                netmask,
                &profinetConfigurationHandle->ip_mask.a,
                &profinetConfigurationHandle->ip_mask.b,
                &profinetConfigurationHandle->ip_mask.c,
                &profinetConfigurationHandle->ip_mask.d
            );

            ProfinetDataSourceAdapter::IPv4U32toQuadU8(
                gateway,
                &profinetConfigurationHandle->ip_gateway.a,
                &profinetConfigurationHandle->ip_gateway.b,
                &profinetConfigurationHandle->ip_gateway.c,
                &profinetConfigurationHandle->ip_gateway.d
            );

            profinetHandle = pnet_init(ethInterface, periodicInterval, profinetConfigurationHandle);
            
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

                threadUp = true;
                cycles = 0;
                events = os_event_create();

                //mainThread = os_thread_create("mainThread", 15, 4096, mainThreadFunction, (void*)this);
                //taskTimer = os_timer_create(periodicInterval, taskTimer_tick, (void*)this, false);
                //os_timer_start(taskTimer);
            }
        }
        else {
            loggerAdapter->Log(LogLevel_Error, "Configuration preconditions not met. Cannot proceed.");
        }

        return profinetUp;
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
        // std::ostringstream outStr;
        // outStr << "Signal led going " << (ledStatus?"ON":"OFF");
        // loggerAdapter->Log(LogLevel_Info, outStr.str());
    }

    bool ProfinetDataSourceAdapter::GetLedStatus() {
        return signalLEDStatus;
    }
    //Endregion - Signal Led Management

    //Region - Thread and task timer mapper
    void mainThreadFunction(void *arg) {
        static_cast<ProfinetDataSourceAdapter*>(arg)->MainThread();
    }


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
        }

        return outputFlagMask;
    }

    void ProfinetDataSourceAdapter::MainThread() {
        

        // os_event_wait(events, eventMask, &eventFlags, OS_WAIT_FOREVER);

        // if(eventFlags & ReadyForData) {
        //     os_event_clr(events, ReadyForData);
        //     loggerAdapter->Log(LogLevel_Debug, "Application Ready.");
        //     pnet_application_ready(profinetHandle, appRelationshipEndPoint);
            
        // }
        // else if(eventFlags & Alarm) {
        //     os_event_clr(events, Alarm);
        //     loggerAdapter->Log(LogLevel_Info, "Alarm detected.");

        // }
        // else if(eventFlags & Timer) {
        //     os_event_clr(events, Timer);
            
        //     if((appRelationshipEndPoint != UINT32_MAX) && ((cycles % gearRatio) ==  0)) {
        //         SetCyclicData();
        //     }

        //     cycles++;
        //     pnet_handle_periodic(profinetHandle);
        // }
        // else if(eventFlags & Abort) {
        //     loggerAdapter->Log(LogLevel_Debug, "Abort.");
        //     os_event_clr(events, Abort);
        // }

    }

    void taskTimer_tick(os_timer_t *timer, void *arg) {
        static_cast<ProfinetDataSourceAdapter*>(arg)->TaskTimerTick();
    }

    void ledTimer_tick(os_timer_t *timer, void *arg) {
        static_cast<ProfinetDataSourceAdapter*>(arg)->SetLedStatus(false);
    }

    void ProfinetDataSourceAdapter::TaskTimerTick() {
        profinetEventNotificationListener->NotifyEvent(MARTe::ProfinetEventTimer);
        //os_event_set(events, Timer);
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
        os_ipaddr_t     inputIpAddress, 
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