/**
 * @file
 * @brief
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

 * @details 
 */


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ProfinetDataSource.h"
#include "ConfigurationDatabase.h"

#include "ILoggerAdapter.h"

#include <iostream>
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    class BasicConsoleLogAdapter : public ProfinetDataSourceDriver::ILoggerAdapter {

        public:
        virtual void Log(ProfinetDataSourceDriver::log_adapter_level_t logLevel, std::string message) {
            
            REPORT_ERROR_STATIC(ErrorManagement::Information, message.c_str());
        }
    };

    ProfinetDataSource::ProfinetDataSource() :
                        DataSourceI() {

    }

    ProfinetDataSource::~ProfinetDataSource() {
        timerHelper->Stop();
        mainHelper->Stop();
        
        timerHelper->SetEntryPoint(NULL_PTR(ITimerEntryPoint*));
        mainHelper->SetEntryPoint(NULL_PTR(IMainThreadEntryPoint*));
    }

    bool 
        ProfinetDataSource::Initialise(
            StructuredDataI& data) {
        
        bool returnValue = DataSourceI::Initialise(data);
        
        uint16 imResultFlag = 0u;

        StreamString tempNetworkInterface;
        StreamString tempStationName;
        uint32 tempPeriodicIntervalus;
        uint32 tempReductionRatio;

        //Reading base configuration parameters
        (data.Read("NetworkInterface", tempNetworkInterface))?(imResultFlag |= PNETDS_MASK_BASENIC):(imResultFlag &= ~PNETDS_MASK_BASENIC);
        (data.Read("StationName", tempStationName))?(imResultFlag |= PNETDS_MASK_BASESTATIONNAME):(imResultFlag &= ~PNETDS_MASK_BASESTATIONNAME);
        (data.Read("PeriodicInterval", tempPeriodicIntervalus))?(imResultFlag |= PNETDS_MASK_BASEPERIODICINTERVAL):(imResultFlag &= ~PNETDS_MASK_BASEPERIODICINTERVAL);
        (data.Read("ReductionRatio", tempReductionRatio))?(imResultFlag |= PNETDS_MASK_BASEREDUCTIONRATIO):(imResultFlag &= ~PNETDS_MASK_BASEREDUCTIONRATIO);

        if((imResultFlag & PNETDS_MASK_BASE_MANDATORIES) == PNETDS_MASK_BASE_MANDATORIES) {
            REPORT_ERROR(ErrorManagement::Information, "Profinet will attach on %s network interface", tempNetworkInterface.Buffer());
            REPORT_ERROR(ErrorManagement::Information, "Station name %s", tempStationName.Buffer());
            REPORT_ERROR(ErrorManagement::Information, "Periodic interval cycling every %d", tempPeriodicIntervalus);
            REPORT_ERROR(ErrorManagement::Information, "Cyclic data sent every %d Profinet scan cycles", tempReductionRatio);
            
            networkInterface = tempNetworkInterface;
            stationName = tempStationName;
            periodicIntervalus = tempPeriodicIntervalus;
            reductionRatio = tempReductionRatio;

            ProfinetDataSourceDriver::ILoggerAdapter *log = new BasicConsoleLogAdapter();
            adapter = new ProfinetDataSourceDriver::ProfinetDataSourceAdapter(networkInterface.Buffer(), periodicIntervalus, stationName.Buffer(), reductionRatio, log);

            if(adapter != NULL_PTR(ProfinetDataSourceDriver::ProfinetDataSourceAdapter *)) {
                returnValue = true;
                REPORT_ERROR(ErrorManagement::Information, "Initialization completed");
            }
            else {
                returnValue = false;
                REPORT_ERROR(ErrorManagement::FatalError, "Cannot initialize Profinet adapter");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Missing mandatory Profinet Base configuration keys, listed below");
            REPORT_ERROR(ErrorManagement::ParametersError, "%s %s %s %s",
                                                           ((imResultFlag & PNETDS_MASK_BASENIC) == 0)?"NetworkInterface":"",
                                                           ((imResultFlag & PNETDS_MASK_BASESTATIONNAME) == 0)?"StationName":"",
                                                           ((imResultFlag & PNETDS_MASK_BASEPERIODICINTERVAL) == 0)?"PeriodicInterval":"",
                                                           ((imResultFlag & PNETDS_MASK_BASEREDUCTIONRATIO) == 0)?"ReductionRatio":"");

            returnValue = false;
        }

        //Reading identification values
        if(returnValue) {            
            imResultFlag = 0u;

            uint16 tempVendorIdentifier;
            uint16 tempDeviceIdentifier;
            uint16 tempOEMVendorIdentifier;
            uint16 tempOEMDeviceIdentifier;
            StreamString tempDeviceVendor;
            StreamString tempManufacturerSpecificString;

            (data.Read("VendorIdentifier", tempVendorIdentifier))?(imResultFlag |= PNETDS_MASK_BASEIDVENDORID):(imResultFlag &= ~PNETDS_MASK_BASEIDVENDORID);
            (data.Read("DeviceIdentifier", tempDeviceIdentifier))?(imResultFlag |= PNETDS_MASK_BASEIDDEVICEID):(imResultFlag &= ~PNETDS_MASK_BASEIDDEVICEID);
            (data.Read("OEMVendorIdentifier", tempOEMVendorIdentifier))?(imResultFlag |= PNETDS_MASK_BASEIDOEMVENDORID):(imResultFlag &= ~PNETDS_MASK_BASEIDOEMVENDORID);
            (data.Read("OEMDeviceIdentifier", tempOEMDeviceIdentifier))?(imResultFlag |= PNETDS_MASK_BASEIDOEMDEVICEID):(imResultFlag &= ~PNETDS_MASK_BASEIDOEMDEVICEID);
            (data.Read("DeviceVendor", tempDeviceVendor))?(imResultFlag |= PNETDS_MASK_BASEIDDEVICEVENDOR):(imResultFlag &= ~PNETDS_MASK_BASEIDDEVICEVENDOR);
            (data.Read("ManufacturerSpecificString", tempManufacturerSpecificString))?(imResultFlag |= PNETDS_MASK_BASEIDMANUFACTURERSTRING):(imResultFlag &= ~PNETDS_MASK_BASEIDMANUFACTURERSTRING);

            if((imResultFlag & PNETDS_MASK_BASEID_MANDATORIES) == PNETDS_MASK_BASEID_MANDATORIES) {
                REPORT_ERROR(ErrorManagement::Information, "Profinet Vendor IDentifier 0x%04x", tempVendorIdentifier);
                REPORT_ERROR(ErrorManagement::Information, "Profinet Device IDentifier 0x%04x", tempDeviceIdentifier);
                REPORT_ERROR(ErrorManagement::Information, "Profinet OEM Vendor IDentifier 0x%04x", tempOEMVendorIdentifier);
                REPORT_ERROR(ErrorManagement::Information, "Profinet OEM Device IDentifier 0x%04x", tempOEMDeviceIdentifier);
                REPORT_ERROR(ErrorManagement::Information, "Device Vendor %s", tempDeviceVendor.Buffer());
                REPORT_ERROR(ErrorManagement::Information, "Manufacturer Specific String %s", tempManufacturerSpecificString.Buffer());

                vendorIdentifier = tempVendorIdentifier;
                deviceIdentifier = tempDeviceIdentifier;
                oemVendorIdentifier = tempOEMVendorIdentifier;
                oemDeviceIdentifier = tempOEMDeviceIdentifier;
                deviceVendor = tempDeviceVendor;
                manufacturerSpecificString = tempManufacturerSpecificString;

                returnValue = true;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Missing mandatory Profinet Base ID configuration keys, listed below");
                REPORT_ERROR(ErrorManagement::ParametersError,  "%s %s %s %s",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDVENDORID) == 0)?"VendorIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDDEVICEID) == 0)?"DeviceIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDOEMVENDORID) == 0)?"OEMVendorIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDOEMDEVICEID) == 0)?"OEMDeviceIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDDEVICEVENDOR) == 0)?"DeviceVendor":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDMANUFACTURERSTRING) == 0)?"ManufacturerSpecificString":"");

                returnValue = false;
            }
        }

        //Reading vendor identification values
        if(returnValue) {
            //TODO Check about endianess issues that may arise in these assumptions
            uint8 vendorIdHigh = static_cast<uint8>((vendorIdentifier >> 8) & 0x00FF);
            uint8 vendorIdLow = static_cast<uint8>((vendorIdentifier) & 0x00FF);

            uint8 deviceIdHigh = static_cast<uint8>((deviceIdentifier >> 8) & 0x00FF);
            uint8 deviceIdLow = static_cast<uint8>((deviceIdentifier) & 0x00FF);
            
            uint8 oemVendorIdHigh = static_cast<uint8>((oemVendorIdentifier >> 8) & 0x00FF);
            uint8 oemVendorIdLow = static_cast<uint8>((oemVendorIdentifier) & 0x00FF);

            uint8 oemDeviceIdHigh = static_cast<uint8>((oemDeviceIdentifier >> 8) & 0x00FF);
            uint8 oemDeviceIdLow = static_cast<uint8>((oemDeviceIdentifier) & 0x00FF);

            returnValue = adapter->SetBaseData(vendorIdHigh, vendorIdLow, deviceIdHigh, deviceIdLow, oemVendorIdHigh, oemVendorIdLow, oemDeviceIdHigh, oemDeviceIdLow, deviceVendor, manufacturerSpecificString, 32);
            if(returnValue) {
                REPORT_ERROR(ErrorManagement::Information, "Profinet Base Configuration data accepted");
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Profinet Base Configuration data rejected");
            }
        }

        imResultFlag = 0u;

        //Reading I&M configuration data
        if(returnValue) {

            uint16 tempIMVendor;
            uint16 tempIMHardwareRevision;
            char8 tempIMSoftwareRevision;
            uint8 tempIMFunctionalEnhancement;
            uint8 tempIMBugFix;
            uint8 tempIMInternalChange;
            uint16 tempIMProfileIdentifier;
            uint16 tempIMProfileSpecificType;
            uint8 tempIMVersionMajor;
            uint8 tempIMVersionMinor;
            StreamString tempIMFunction;
            StreamString tempIMLocation;
            StreamString tempIMDate;
            StreamString tempIMDescriptor;
            StreamString tempIMSignature;

            (data.Read("IMVendor", tempIMVendor))?(imResultFlag |= PNETDS_MASK_IMVENDOR):(imResultFlag &= ~PNETDS_MASK_IMVENDOR);
            (data.Read("IMHardwareRevision", tempIMHardwareRevision))?(imResultFlag |= PNETDS_MASK_IMHARDWAREREVISION):(imResultFlag &= ~PNETDS_MASK_IMHARDWAREREVISION);
            (data.Read("IMSoftwareRevision", tempIMSoftwareRevision))?(imResultFlag |= PNETDS_MASK_IMSOFTWAREREVISION):(imResultFlag &= ~PNETDS_MASK_IMSOFTWAREREVISION);

            (data.Read("IMFunctionalEnhancement", tempIMFunctionalEnhancement))?(imResultFlag |= PNETDS_MASK_IMFUNCTIONALENHANCEMENT):(imResultFlag &= ~PNETDS_MASK_IMFUNCTIONALENHANCEMENT);
            (data.Read("IMBugFix", tempIMBugFix))?(imResultFlag |= PNETDS_MASK_IMBUGFIX):(imResultFlag &= ~PNETDS_MASK_IMBUGFIX);
            (data.Read("IMInternalChange", tempIMInternalChange))?(imResultFlag |= PNETDS_MASK_IMINTERNALCHANGE):(imResultFlag &= ~PNETDS_MASK_IMINTERNALCHANGE);

            (data.Read("IMProfileIdentifier", tempIMProfileIdentifier))?(imResultFlag |= PNETDS_MASK_IMPROFILEIDENTIFIER):(imResultFlag &= ~PNETDS_MASK_IMPROFILEIDENTIFIER);
            (data.Read("IMProfileSpecificType", tempIMProfileSpecificType))?(imResultFlag |= PNETDS_MASK_IMPROFILESPECIFICTYPE):(imResultFlag &= ~PNETDS_MASK_IMPROFILESPECIFICTYPE);

            (data.Read("IMVersionMajor", tempIMVersionMajor))?(imResultFlag |= PNETDS_MASK_IMVERSIONMAJOR):(imResultFlag &= ~PNETDS_MASK_IMVERSIONMAJOR);
            (data.Read("IMVersionMinor", tempIMVersionMinor))?(imResultFlag |= PNETDS_MASK_IMVERSIONMINOR):(imResultFlag &= ~PNETDS_MASK_IMVERSIONMINOR);

            (data.Read("IMFunction", tempIMFunction))?(imResultFlag |= PNETDS_MASK_IMFUNCTION):(imResultFlag &= ~PNETDS_MASK_IMFUNCTION);
            (data.Read("IMLocation", tempIMLocation))?(imResultFlag |= PNETDS_MASK_IMLOCATION):(imResultFlag &= ~PNETDS_MASK_IMLOCATION);
            (data.Read("IMDate", tempIMDate))?(imResultFlag |= PNETDS_MASK_IMDATE):(imResultFlag &= ~PNETDS_MASK_IMDATE);
            (data.Read("IMDescriptor", tempIMDescriptor))?(imResultFlag |= PNETDS_MASK_IMDESCRIPTOR):(imResultFlag &= ~PNETDS_MASK_IMDESCRIPTOR);
            (data.Read("IMSignature", tempIMSignature))?(imResultFlag |= PNETDS_MASK_IMSIGNATURE):(imResultFlag &= ~PNETDS_MASK_IMSIGNATURE);

            if((imResultFlag & PNETDS_MASK_IM_MANDATORIES) == PNETDS_MASK_IM_MANDATORIES) {
                REPORT_ERROR(ErrorManagement::Information, "Mandatory I&M Data configured");
                imVendor = tempIMVendor;
                imHardwareRevision = tempIMHardwareRevision;
                
                switch(tempIMSoftwareRevision) {
                    case 'V':
                        imSoftwareRevision = ProfinetDataSourceDriver::SwRev_V;
                    break;
                    case 'R':
                        imSoftwareRevision = ProfinetDataSourceDriver::SwRev_R;
                    break;
                    case 'P':
                        imSoftwareRevision = ProfinetDataSourceDriver::SwRev_P;
                    break;
                    case 'U':
                        imSoftwareRevision = ProfinetDataSourceDriver::SwRev_U;
                    break;
                    case 'T':
                        imSoftwareRevision = ProfinetDataSourceDriver::SwRev_T;
                    break;
                    default:
                        REPORT_ERROR(ErrorManagement::ParametersError, "Software Revision possible values can only be [V,R,P,U,T], falling back to V");
                        imSoftwareRevision = ProfinetDataSourceDriver::SwRev_V;
                    break;
                }

                imFunctionalEnhancement = tempIMFunctionalEnhancement;
                imBugFix = tempIMBugFix;
                imInternalChange = tempIMInternalChange;
                imProfileIdentifier = tempIMProfileIdentifier;
                imProfileSpecificType = tempIMProfileSpecificType;
                imVersionMajor = tempIMVersionMajor;
                imVersionMinor = tempIMVersionMinor;

                imFunction = ((imResultFlag & PNETDS_MASK_IMFUNCTION) == PNETDS_MASK_IMFUNCTION)?tempIMFunction:"";
                imLocation = ((imResultFlag & PNETDS_MASK_IMLOCATION) == PNETDS_MASK_IMLOCATION)?tempIMLocation:"";
                imDate = ((imResultFlag & PNETDS_MASK_IMDATE) == PNETDS_MASK_IMDATE)?tempIMDate:"";
                imDescriptor = ((imResultFlag & PNETDS_MASK_IMDESCRIPTOR) == PNETDS_MASK_IMDESCRIPTOR)?tempIMDescriptor:"";
                imSignature = ((imResultFlag & PNETDS_MASK_IMSIGNATURE) == PNETDS_MASK_IMSIGNATURE)?tempIMSignature:"";
                uint8 imVendorHigh = static_cast<uint8>((imVendor >> 8) & 0x00FF);
                uint8 imVendorLow = static_cast<uint8>(imVendor & 0x00FF);

                adapter->SetIdentificationAndMaintainanceData(imVendorHigh, imVendorLow, imHardwareRevision, imSoftwareRevision, imFunctionalEnhancement, imBugFix, imInternalChange, imProfileIdentifier, imProfileSpecificType, imVersionMajor, imVersionMinor, imFunction, imLocation, imDate, imDescriptor, imSignature);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Missing I&M Mandatory configuration keys, listed below");
                REPORT_ERROR(ErrorManagement::ParametersError,  "%s %s %s %s %s %s %s %s %s %s", 
                                                                ((imResultFlag & PNETDS_MASK_IMVENDOR) == 0)?"IMVendor":"",
                                                                ((imResultFlag & PNETDS_MASK_IMHARDWAREREVISION) == 0)?"IMHardwareRevision":"",
                                                                ((imResultFlag & PNETDS_MASK_IMSOFTWAREREVISION) == 0)?"IMSoftwareRevision":"",
                                                                ((imResultFlag & PNETDS_MASK_IMFUNCTIONALENHANCEMENT) == 0)?"IMFunctionalEnhancement":"",
                                                                ((imResultFlag & PNETDS_MASK_IMBUGFIX) == 0)?"IMBugFix":"",
                                                                ((imResultFlag & PNETDS_MASK_IMINTERNALCHANGE) == 0)?"IMInternalChange":"",
                                                                ((imResultFlag & PNETDS_MASK_IMPROFILEIDENTIFIER) == 0)?"IMProfileIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_IMPROFILESPECIFICTYPE) == 0)?"IMProfileSpecificType":"",
                                                                ((imResultFlag & PNETDS_MASK_IMVERSIONMAJOR) == 0)?"IMVersionMajor":"",
                                                                ((imResultFlag & PNETDS_MASK_IMVERSIONMINOR) == 0)?"IMVersionMinor":"");

                returnValue = false;
            }
        }

        //Reading LLDP / RTClass / Network configuration data
        if(returnValue) {
            imResultFlag = 0u;

            StreamString tempLLDPPortIdentifier;
            uint16 tempRTClass2Status;
            uint16 tempRTClass3Status;
            uint8 tempAutoNegotiationCapability;
            uint16 tempAutoNegotiationSpeed;
            uint16 tempMAUType;

            (data.Read("LLDPPortIdentifier", tempLLDPPortIdentifier))?(imResultFlag |= PNETDS_MASK_LLDP_PORTID):(imResultFlag &= ~PNETDS_MASK_LLDP_PORTID);
            (data.Read("RTClass2Status", tempRTClass2Status))?(imResultFlag |= PNETDS_MASK_LLDP_RTCLASS2STATUS):(imResultFlag &= ~PNETDS_MASK_LLDP_RTCLASS2STATUS);
            (data.Read("RTClass3Status", tempRTClass3Status))?(imResultFlag |= PNETDS_MASK_LLDP_RTCLASS3STATUS):(imResultFlag &= ~PNETDS_MASK_LLDP_RTCLASS3STATUS);
            (data.Read("AutoNegotiationCapability", tempAutoNegotiationCapability))?(imResultFlag |= PNETDS_MASK_LLDP_AUTONEGCAPAB):(imResultFlag &= ~PNETDS_MASK_LLDP_AUTONEGCAPAB);
            (data.Read("AutoNegotiationSpeed", tempAutoNegotiationSpeed))?(imResultFlag |= PNETDS_MASK_LLDP_AUTONEGSPEED):(imResultFlag &= ~PNETDS_MASK_LLDP_AUTONEGSPEED);
            (data.Read("MAUType", tempMAUType))?(imResultFlag |= PNETDS_MASK_LLDP_MAUTYPE):(imResultFlag &= ~PNETDS_MASK_LLDP_MAUTYPE);

            lldpPortIdentifier = ((imResultFlag & PNETDS_MASK_LLDP_PORTID) == PNETDS_MASK_LLDP_PORTID)?tempLLDPPortIdentifier:"port-001";
            rtClass2Status = ((imResultFlag & PNETDS_MASK_LLDP_RTCLASS2STATUS) == PNETDS_MASK_LLDP_RTCLASS2STATUS)?tempRTClass2Status:adapter->GetDefaultLLDPRTClass2Status();
            rtClass3Status = ((imResultFlag & PNETDS_MASK_LLDP_RTCLASS3STATUS) == PNETDS_MASK_LLDP_RTCLASS3STATUS)?tempRTClass3Status:adapter->GetDefaultLLDPRTClass3Status();
            autoNegotiationCapability = ((imResultFlag & PNETDS_MASK_LLDP_AUTONEGCAPAB) == PNETDS_MASK_LLDP_AUTONEGCAPAB)?tempAutoNegotiationCapability:adapter->GetDefaultLLDPAutonegotiationCapability();
            autoNegotiationSpeed = ((imResultFlag & PNETDS_MASK_LLDP_MAUTYPE) == PNETDS_MASK_LLDP_MAUTYPE)?tempMAUType:adapter->GetDefaultLLDPMAUType();

            REPORT_ERROR(ErrorManagement::Information, "LLDP Configuration");
            REPORT_ERROR(ErrorManagement::Information, "Port Identifier %s", lldpPortIdentifier.Buffer());
            REPORT_ERROR(ErrorManagement::Information, "RT Class2/Class3 %d/%d", rtClass2Status, rtClass3Status);
            REPORT_ERROR(ErrorManagement::Information, "AutoNegotiation caps %02x", autoNegotiationCapability);
            REPORT_ERROR(ErrorManagement::Information, "AutoNegotiation speed %04x", autoNegotiationSpeed);
            REPORT_ERROR(ErrorManagement::Information, "MAU Type %04x", mauType);

            adapter->SetLLDPData(lldpPortIdentifier, rtClass2Status, rtClass3Status, autoNegotiationCapability, autoNegotiationSpeed, mauType);
            returnValue = true;
        }

        uint32 heapSizeToAllocateInputs = 0u;
        uint32 heapSizeToAllocateOutputs = 0u;
        inputHeap = NULL_PTR(uint8*);
        outputHeap = NULL_PTR(uint8*);

        //Scan the MARTe2 configuration file to assess the Slot/Subslot configuration for the Profinet peripheral
        if(returnValue) {
            returnValue = data.MoveRelative("Slots");

            if(!returnValue) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Cannot find [Slots] configuration key");
            }

            uint32 slotsCount = data.GetNumberOfChildren();
            if(slotsCount < 1u) {
                REPORT_ERROR(ErrorManagement::ParametersError, "[Slots] configuration key must contain at least one slot definition");
            }
            else {
                REPORT_ERROR(ErrorManagement::Information, "There will be %d slots available in the Profinet slave", slotsCount);

                for(uint32 slotIndex = 0u; slotIndex < slotsCount; slotIndex++) {
                    REPORT_ERROR(ErrorManagement::Information, "Slot idx %d", slotIndex);
                    uint16 tempSlotNumber = 0u;
                    if(data.MoveToChild(slotIndex)) {
                        if(data.Read("SlotNumber", tempSlotNumber)) {
                            REPORT_ERROR(ErrorManagement::Information, "Found SlotNumber %d, adding it to the driver structure", tempSlotNumber);
                            adapter->AddSlot(tempSlotNumber);

                            if(data.MoveRelative("Subslots")) {
                                uint32 subslotsCount = data.GetNumberOfChildren();
                                REPORT_ERROR(ErrorManagement::Information, "Slot %d will have %d subslots", tempSlotNumber, subslotsCount);
                                for(uint32 subslotIndex = 0u; subslotIndex < subslotsCount; subslotIndex++) {
                                    REPORT_ERROR(ErrorManagement::Information, "Working on subslot %s", data.GetChildName(subslotIndex));
                                    if(data.MoveToChild(subslotIndex)) {
                                        
                                        uint8 tempResultFlag = 0u;
                                        uint16 tempSubslotNumber = 0u;
                                        uint8 tempIsDAP = 0u;
                                        uint8 tempExpectedDataDirection = 0u;
                                        uint16 tempExpectedInputSize = 0u;
                                        uint16 tempExpectedOutputSize = 0u;
                                        
                                        (data.Read("SubslotNumber", tempSubslotNumber))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_NUMBER):(tempResultFlag &= ~PNETDS_MASK_SUBSLOT_NUMBER);
                                        (data.Read("DeviceAccessPoint", tempIsDAP))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_ISDAP):(tempResultFlag &= ~PNETDS_MASK_SUBSLOT_ISDAP);
                                        (data.Read("ExpectedDataDirection", tempExpectedDataDirection))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_EXPDIR):(tempResultFlag &= ~PNETDS_MASK_SUBSLOT_EXPDIR);
                                        (data.Read("ExpectedInputSize", tempExpectedInputSize))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_EXPINSIZE):(tempResultFlag &= ~PNETDS_MASK_SUBSLOT_EXPINSIZE);
                                        (data.Read("ExpectedOutputSize", tempExpectedOutputSize))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_EXPOUTSIZE):(tempResultFlag &= ~PNETDS_MASK_SUBSLOT_EXPOUTSIZE);

                                        if((tempResultFlag & PNETDS_MASK_SUBSLOT_MANDATORIES) == PNETDS_MASK_SUBSLOT_MANDATORIES) {
                                            ProfinetDataSourceDriver::pnet_submodule_direction_t tempDirection;
                                            switch(tempExpectedDataDirection) {
                                                case 0:
                                                    tempDirection = ProfinetDataSourceDriver::NoInputOutput;
                                                    break;
                                                case 1:
                                                    tempDirection = ProfinetDataSourceDriver::Input;
                                                    break;
                                                case 2:
                                                    tempDirection = ProfinetDataSourceDriver::Output;
                                                    break;
                                                case 3:
                                                    tempDirection = ProfinetDataSourceDriver::InputOutput;
                                                    break;
                                            }
                                            
                                            REPORT_ERROR(ErrorManagement::Information, "Found subslot %d [DAP:%d DIR:%d IN:%d OUT: %d]", tempSubslotNumber, tempIsDAP, tempDirection, tempExpectedInputSize, tempExpectedOutputSize);

                                            if(tempExpectedInputSize > 0u) {
                                                heapSizeToAllocateInputs += tempExpectedInputSize;
                                            }

                                            if(tempExpectedOutputSize > 0u) {
                                                heapSizeToAllocateOutputs += tempExpectedOutputSize;
                                            }                                           
                                            if(returnValue) {
                                                adapter->AddSubslot(tempSlotNumber, tempSubslotNumber, (tempIsDAP == 1), tempDirection, tempExpectedInputSize, tempExpectedOutputSize);
                                            }
                                            else {
                                                REPORT_ERROR(ErrorManagement::FatalError, "Cannot add submodule to the Profinet data structure");
                                            }
                                            
                                        }
                                        else {
                                            REPORT_ERROR(ErrorManagement::ParametersError, "Missing subslot mandatory configuration keys, listed below");
                                            REPORT_ERROR(ErrorManagement::ParametersError,  "%s %s %s %s %s", 
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_NUMBER) == 0)?"SubslotNumber":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_ISDAP) == 0)?"DeviceAccessPoint":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_EXPDIR) == 0)?"ExpectedDataDirection":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_EXPINSIZE) == 0)?"ExpectedInputSize":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_EXPOUTSIZE) == 0)?"ExpectedOutputSize":"");
                                            returnValue = false;
                                        }
                                        if(!data.MoveToAncestor(1u)) {
                                            returnValue = false;
                                            REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to ancestor node");
                                            break;
                                        }
                                    }
                                    else {
                                        REPORT_ERROR(ErrorManagement::FatalError, "Cannot move to child %s", data.GetChildName(subslotIndex));
                                        returnValue = false;
                                        break;
                                    }
                                }//LOOP in SUBSLOTS
                                if(!data.MoveToAncestor(1u)) {
                                    returnValue = false;
                                    REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to ancestor node");
                                    break;
                                }
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::ParametersError, "Missing Subslots information configuration block");
                                returnValue = false;
                                break;
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Missing SlotNumber configuration key");
                            returnValue = false;
                            break;
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to specified subslot");
                        returnValue = false;
                        break;
                    }
                    if(!data.MoveToAncestor(1u)) {
                        returnValue = false;
                        REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to ancestor node");
                        break;
                    }
                }//LOOP in SLOTS
            }

            if(!data.MoveToAncestor(1u)) {
                returnValue = false;
                REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to ancestor node");
            }
        }
    
        //Allocate double the memory of the inputs, one half for Profinet, the other for MARTe
        if(returnValue) {
            if(heapSizeToAllocateInputs > 0u) {
                REPORT_ERROR(ErrorManagement::Information, "Allocating %d * 2 bytes for inputs", heapSizeToAllocateInputs);
                //Allocating the size needed for inputs + 1, which will be reserved to the Alarm Acknowledgement signal
                inputHeap = reinterpret_cast<uint8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc((heapSizeToAllocateInputs * 2u) + 1));
                //Underlying malloc leaves undetermined memory. Cleaning it.
                MemoryOperationsHelper::Set(inputHeap, 0u, (heapSizeToAllocateInputs * 2u) + 1u);
                inputHeapHalfSize = heapSizeToAllocateInputs;
                returnValue = (inputHeap != NULL_PTR(uint8*));
            }

            if(!returnValue) {
                REPORT_ERROR(ErrorManagement::FatalError, "Cannot allocate heap for input bank");
            }
        }

        //Allocate double the memory of the outputs, one half for Profinet, the other for MARTe
        if(returnValue) {
            if(heapSizeToAllocateOutputs > 0u) {
                REPORT_ERROR(ErrorManagement::Information, "Allocating %d * 2 bytes for outputs", heapSizeToAllocateOutputs);
                //Allocating the size needed for the outputs + 3, which will be reserved to LED, Ready and Alarm special signals
                outputHeap = reinterpret_cast<uint8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc((heapSizeToAllocateOutputs * 2u) + 3u));
                //Underlying malloc leaves undetermined memory. Cleaning it.
                MemoryOperationsHelper::Set(outputHeap, 0u, (heapSizeToAllocateOutputs * 2u) + 3u);
                outputHeapHalfSize = heapSizeToAllocateOutputs;
                returnValue = (outputHeap != NULL_PTR(uint8*));
            }

            if(!returnValue) {
                REPORT_ERROR(ErrorManagement::FatalError, "Cannot allocate heap for output bank");
            }
        }

        //Scan the Profinet Slot/Subslot structure and assign the previously allocated memory, on the first half of each buffer (I/O)
        if(returnValue) {
            uint8* inputHeapIndex = inputHeap;
            uint8* outputHeapIndex = outputHeap;
            
            ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_slot_t*>* slotsIterator = adapter->GetAPIStructureIterator();
            if(slotsIterator != NULL_PTR(ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_slot_t*>*)) {
                slotsIterator->First();
                ProfinetDataSourceDriver::pnet_slot_t* tempSlot = NULL_PTR(ProfinetDataSourceDriver::pnet_slot_t*);
                ProfinetDataSourceDriver::pnet_subslot_t* tempSubslot = NULL_PTR(ProfinetDataSourceDriver::pnet_subslot_t*);
                
                while((tempSlot = slotsIterator->Next()) != NULL_PTR(ProfinetDataSourceDriver::pnet_slot_t*)) {
                    REPORT_ERROR(ErrorManagement::Information, "Scanning slot %d", tempSlot->slotNumber);
                    ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_subslot_t*>* subslotsIterator = tempSlot->subslots.GetIterator();
                    subslotsIterator->First();
                    if(subslotsIterator != NULL_PTR(ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_subslot_t*>*)) {
                        while((tempSubslot = subslotsIterator->Next()) != NULL_PTR(ProfinetDataSourceDriver::pnet_subslot_t*)) {
                            if(tempSubslot->expectedConfigurationData.insize > 0u) {
                                if(((inputHeapIndex + tempSubslot->expectedConfigurationData.insize) - inputHeap) <= heapSizeToAllocateInputs) {
                                    tempSubslot->inputData = inputHeapIndex;
                                    inputHeapIndex += tempSubslot->expectedConfigurationData.insize;
                                    REPORT_ERROR(   ErrorManagement::Information, "Assigning %p from input heap with size %d to Slot/Subslot %d/%d", 
                                                    tempSubslot->inputData,
                                                    tempSubslot->expectedConfigurationData.insize,
                                                    tempSlot->slotNumber,
                                                    tempSubslot->subslotNumber);
                                }
                                else {
                                    REPORT_ERROR(ErrorManagement::FatalError, "Input heap assignment for Slot/Subslot %d/%d would exceed allocated heap", tempSlot->slotNumber, tempSubslot->subslotNumber);
                                    returnValue = false;
                                }
                            }
                            if(tempSubslot->expectedConfigurationData.outsize > 0u) {
                                if(((outputHeapIndex + tempSubslot->expectedConfigurationData.outsize) - outputHeap) <= heapSizeToAllocateOutputs) {
                                    tempSubslot->outputData = outputHeapIndex;
                                    outputHeapIndex += tempSubslot->expectedConfigurationData.outsize;
                                    REPORT_ERROR(   ErrorManagement::Information, "Assigning %p from output heap with size %d to Slot/Subslot %d/%d", 
                                                    tempSubslot->outputData,
                                                    tempSubslot->expectedConfigurationData.outsize,
                                                    tempSlot->slotNumber,
                                                    tempSubslot->subslotNumber);
                                }
                                else {
                                    REPORT_ERROR(ErrorManagement::FatalError, "Output heap assignment for Slot/Subslot %d/%d would exceed allocated heap", tempSlot->slotNumber, tempSubslot->subslotNumber);
                                    returnValue = false;
                                }
                            }
                            if(!returnValue) {
                                break;
                            }
                        }
                        delete subslotsIterator;
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::FatalError, "Unable to iterate inside slot %d subslots", tempSlot->slotNumber);
                        returnValue = false;
                    }
                }

                delete slotsIterator;
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Unable to iterate inside API structure Slots/Subslots collection");
                returnValue = false;
            }
        }

        if(returnValue) {
            if(data.MoveRelative("Signals")) {
                //Locking Signals section
                if(signalsDatabase.MoveRelative("Signals") && signalsDatabase.Write("Locked", 1u) && signalsDatabase.MoveToAncestor(1u)) {
                    REPORT_ERROR(ErrorManagement::Information, "Successfully locked Signals section");
                    returnValue = true;
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "Cannot lock Signals sections");
                    returnValue = false;
                }

                //Scan the Signals sub-tree to read additional attributes for the Profinet-to-MARTe2 signals mapping (e.g. heap placement/offset)
                if(returnValue) {
                    signalIndexerCount = data.GetNumberOfChildren();
                    REPORT_ERROR(ErrorManagement::Information, "%d Signals found in the configuration file", signalIndexerCount);

                    signalIndexer[0] = reinterpret_cast<profinet_marte_signal_t*>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(sizeof(profinet_marte_signal_t) * signalIndexerCount));

                    //TODO Eventual multiple buffers should be initialised here, like this
                    //signalIndexer[1] = reinterpret_cast<profinet_marte_signal_t*>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(sizeof(profinet_marte_signal_t) * signalIndexerCount));

                    if((signalIndexer[0] == NULL_PTR(profinet_marte_signal_t*))) { // || (signalIndexer[1] == NULL_PTR(profinet_marte_signal_t*))) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Cannot allocate the signal indexer structure");
                        returnValue = false;
                    }

                    for(uint32 signalIndex = 0u; (signalIndex < signalIndexerCount) && (returnValue); signalIndex++) {
                        REPORT_ERROR(ErrorManagement::Information, "Moving to child %s", data.GetChildName(signalIndex));
                        StreamString tempSignalName = data.GetChildName(signalIndex);
                        if(data.MoveToChild(signalIndex)) {
                            //First check if we have one of the Profinet special signals, else it will be a standard signal
                            if(tempSignalName == "ProfinetDeviceLed") {
                                REPORT_ERROR(ErrorManagement::Information, "Found LED signal");
                                //First byte of LED signal is located after the output bank half
                                signalIndexer[0][signalIndex].firstByteOfSignal = outputHeap +  (2u * outputHeapHalfSize);
                                signalIndexer[0][signalIndex].direction = 1u;
                                signalIndexer[0][signalIndex].needsSwapping = 0u;
                                profinetLedSignalIndex = signalIndex;
                                MemoryOperationsHelper::Copy(signalIndexer[0][signalIndex].marteName, tempSignalName.Buffer(), tempSignalName.Size() + 1u);
                                profinetLedSignalEnabled = true;
                            }
                            else if(tempSignalName == "ProfinetDeviceReady") {
                                REPORT_ERROR(ErrorManagement::Information, "Found Ready signal");
                                //First byte of LED signal is located after the output bank half + 1
                                signalIndexer[0][signalIndex].firstByteOfSignal = outputHeap +  (2u * outputHeapHalfSize) + 1u;
                                signalIndexer[0][signalIndex].direction = 1u;
                                signalIndexer[0][signalIndex].needsSwapping = 0u;
                                profinetReadySignalIndex = signalIndex;
                                MemoryOperationsHelper::Copy(signalIndexer[0][signalIndex].marteName, tempSignalName.Buffer(), tempSignalName.Size() + 1u);
                                profinetReadySignalEnabled = true;
                            }
                            else {
                                uint16 tempSlotNumber = 0u;
                                uint16 tempSubslotNumber = 0u;
                                uint8 tempDirectionValue = 0u;
                                uint32 tempOffsetValue = 0u;
                                uint8 tempNeedsSwapping = false;

                                if(data.Read("Slot", tempSlotNumber) && data.Read("Subslot", tempSubslotNumber) && data.Read("Direction", tempDirectionValue) && data.Read("Offset", tempOffsetValue)) {
                                    data.Read("NeedsSwapping", tempNeedsSwapping);

                                    ProfinetDataSourceDriver::pnet_subslot_t* tempSubslot = adapter->GetSubslot(tempSlotNumber, tempSubslotNumber);
                                    if(tempSubslot != NULL_PTR(ProfinetDataSourceDriver::pnet_subslot_t*)) {
                                        if(tempDirectionValue == 0u) {
                                            //MARTe signals are located in the other half of the input heap, this is why we need to shift forward of one inputHeapHalfSize
                                            signalIndexer[0][signalIndex].firstByteOfSignal = tempSubslot->inputData + tempOffsetValue + inputHeapHalfSize;
                                            REPORT_ERROR(ErrorManagement::Information, "Input signal %d will be located at Slot/Subslot + Offset [%d/%d + %d]", signalIndex, tempSlotNumber, tempSubslotNumber, tempOffsetValue);
                                        }
                                        else if(tempDirectionValue == 1u) {
                                            //MARTe signals are located in the other half of the output heap, this is why we need to shift forward of one outputHeapHalfSize
                                            signalIndexer[0][signalIndex].firstByteOfSignal = tempSubslot->outputData + tempOffsetValue + outputHeapHalfSize;
                                            
                                            REPORT_ERROR(ErrorManagement::Information, "Output signal %d will be located at Slot/Subslot + Offset [%d/%d + %d]", signalIndex, tempSlotNumber, tempSubslotNumber, tempOffsetValue);
                                        }
                                        else {
                                            REPORT_ERROR(ErrorManagement::FatalError, "A wrong direction value was specified (%d), only [0 (IN), 1 (OUT)] are allowed");
                                            returnValue = false;
                                        }
                                        if(returnValue) {
                                            signalIndexer[0][signalIndex].direction = tempDirectionValue;
                                            signalIndexer[0][signalIndex].needsSwapping = (tempNeedsSwapping != 0u);
                                            REPORT_ERROR(ErrorManagement::Information, "Signal %d endianess %s be adjusted", signalIndex, (signalIndexer[0][signalIndex].needsSwapping?"will":"will not"));
                                            //TODO verify what is the best way (size) to copy a signal name
                                            MemoryOperationsHelper::Copy(signalIndexer[0][signalIndex].marteName, tempSignalName.Buffer(), tempSignalName.Size() + 1u);
                                        }
                                    }
                                    else {
                                        REPORT_ERROR(ErrorManagement::FatalError, "Cannot retrieve specified Slot/Subslot keys [%d/%d]", tempSlotNumber, tempSubslotNumber);
                                        returnValue = false;
                                    }
                                }
                                else {
                                    REPORT_ERROR(ErrorManagement::FatalError, "Missing Slot/Subslot coordinates for signal [%s] node", data.GetChildName(signalIndex));
                                    returnValue = false;
                                }
                            }                    

                            if(!data.MoveToAncestor(1u)) {
                                REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to top-level Signal node");
                                returnValue = false;
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failure moving to child %d", signalIndex);
                            returnValue = false;
                        }
                    }
                }

                if(!data.MoveToAncestor(1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Cannot leave the Signals section");
                    returnValue = false;
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "Cannot move to Signals section");
                returnValue = false;
            }
        }

        //Find the helper services
        if(returnValue) {
            uint32 size = Size();
            bool foundTimer = false;
            bool foundMain = false;

            for(uint32 i = 0u; i < size && !(foundMain && foundTimer); i++) {
                ReferenceT<ProfinetTimerHelper> refT = Get(i);
                ReferenceT<ProfinetMainThreadHelper> refM = Get(i);
                if((!foundTimer) && refT.IsValid()) {
                    timerHelper = refT;
                    foundTimer = true;
                    timerHelper->SetEntryPoint(this);
                    REPORT_ERROR(ErrorManagement::Information, "Found Timer helper");
                }
                else if((!foundMain) && refM.IsValid()) {
                    mainHelper = refM;
                    foundMain = true;
                    mainHelper->SetEntryPoint(this);
                    REPORT_ERROR(ErrorManagement::Information, "Found Main helper");
                }
            }

            returnValue = foundMain && foundTimer;
            if(!returnValue) {
                REPORT_ERROR(ErrorManagement::Information, "Missing Timer and/or Main helper services");
            }
        }

        //Last connections to helpers and cycles startup
        if(returnValue) {
            inputSignalsSemaphore.Create();
            outputSignalsSemaphore.Create();

            adapter->cyclicNotificationListener = this;
            adapter->profinetEventNotificationListener = this;
            adapter->opSignalsEntryPoint = this;
            returnValue = adapter->Initialize();
            //Main helper times out after 10 missed cycles
            mainHelper->SetPeriodicInterval(periodicIntervalus * 10e-9);
            mainHelper->SetTimeout(periodicIntervalus * 10e-9 * 10);

            timerHelper->SetTimerInterval(periodicIntervalus * 10e-9);
            mainHelper->Start();
            timerHelper->Start();
        }

        return returnValue;
    }

    uint16 ProfinetDataSource::MainThread(uint16 inputFlag) {
        return adapter->MainThread(inputFlag);
    }

    void ProfinetDataSource::TimerTick() {
        adapter->TaskTimerTick();
    }

    void ProfinetDataSource::NotifyCycle() {
        if(inputSignalsSemaphore.FastLock()) {
            //MARTe inputs must be copied into Profinet inputs (e.g. from the MARTe half to the Profinet half of the heap buffer)
            MemoryOperationsHelper::Copy(inputHeap, inputHeap + inputHeapHalfSize, inputHeapHalfSize);
            inputSignalsSemaphore.FastUnLock();
            
            //Swapping occurs outside the critical section, as it is not affecting the MARTe side
            //There is no reason to also lock MARTe while swapping data on the Profinet side
            //NOTE firstByteOfSignal is on the MARTe side, this is why we have to move pointer backwards to the Profinet section
            for(uint32 signalIndex = 0; signalIndex < signalIndexerCount; signalIndex++) {
                if((signalIndexer[0][signalIndex].direction == 0u) && (signalIndexer[0][signalIndex].needsSwapping)) {
                    switch(signalIndexer[0][signalIndex].signalSize) {
                        case 2:
                        Endianity::ToBigEndian(*reinterpret_cast<int16*>(signalIndexer[0][signalIndex].firstByteOfSignal - inputHeapHalfSize));
                        break;
                        case 4:
                        Endianity::ToBigEndian(*reinterpret_cast<int32*>(signalIndexer[0][signalIndex].firstByteOfSignal - inputHeapHalfSize));
                        break;
                        case 8:
                        Endianity::ToBigEndian(*reinterpret_cast<int64*>(signalIndexer[0][signalIndex].firstByteOfSignal - inputHeapHalfSize));
                        break;
                        default:
                        //TODO Check if really you can do nothing or cases exists where bigger swaps may occur
                        break;
                    }
                }
            }
        }

        if(outputSignalsSemaphore.FastLock()) {
            //Profinet outputs must be copied into MARTe outputs (e.g. from the Profinet half to the MARTe half of the heap buffer)
            MemoryOperationsHelper::Copy(outputHeap + outputHeapHalfSize, outputHeap, outputHeapHalfSize);

            //Swapping occurs inside the critical section, as we are swapping bytes on the MARTe side
            //MARTe must still wait before working on the signal buffer
            //NOTE firstByteOfSignal is on the MARTe side, this time no need to move pointers
            for(uint32 signalIndex = 0; signalIndex < signalIndexerCount; signalIndex++) {
                if((signalIndexer[0][signalIndex].direction == 1u) && (signalIndexer[0][signalIndex].needsSwapping)) {
                    switch(signalIndexer[0][signalIndex].signalSize) {
                        case 2:
                        Endianity::FromBigEndian(*reinterpret_cast<int16*>(signalIndexer[0][signalIndex].firstByteOfSignal));
                        break;
                        case 4:
                        Endianity::FromBigEndian(*reinterpret_cast<int32*>(signalIndexer[0][signalIndex].firstByteOfSignal));
                        break;
                        case 8:
                        Endianity::FromBigEndian(*reinterpret_cast<int64*>(signalIndexer[0][signalIndex].firstByteOfSignal));
                        break;
                        default:
                        //TODO Check if really you can do nothing or cases exists where bigger swaps may occur
                        break;
                    }
                }
            }
            outputSignalsSemaphore.FastUnLock();
        }
    }

    void ProfinetDataSource::NotifyEvent(ProfinetDataSourceEventType eventType) {
        mainHelper->NotifyEvent(eventType);
    }

    bool 
        ProfinetDataSource::SetConfiguredDatabase(
            StructuredDataI& data) {

        bool returnValue = DataSourceI::SetConfiguredDatabase(data);
        uint32 tempSignalMARTeIndex = 0u;

        for(uint32 signalIndex = 0u; signalIndex < signalIndexerCount; signalIndex++) {
            returnValue = GetSignalIndex(tempSignalMARTeIndex, signalIndexer[0][signalIndex].marteName);
            if(returnValue) {
                returnValue = GetSignalByteSize(tempSignalMARTeIndex, signalIndexer[0][signalIndex].signalSize);
                signalIndexer[0][signalIndex].marteIndex = tempSignalMARTeIndex;

                REPORT_ERROR(ErrorManagement::Information, "Indexer acquired signal MARTe Idx/Internal Idx %d/%d, with name %s which is %d bytes", 
                    signalIndexer[0][signalIndex].marteIndex,
                    signalIndex,
                    signalIndexer[0][signalIndex].marteName, 
                    signalIndexer[0][signalIndex].signalSize);
            }
            else {
                REPORT_ERROR(ErrorManagement::Information, "Failure dealing with signal %d", signalIndex);
            }
        }

        return returnValue;
    }

    bool 
        ProfinetDataSource::Synchronise() {
        //This method shall not be used
        return false;
    }

    bool 
        ProfinetDataSource::PrepareNextState(
            const char8* const currentStateName,
            const char8* const nextStateName) {
        
        return true;
    }

    bool 
        ProfinetDataSource::AllocateMemory() {
        
        return true;
    }

    uint32 
        ProfinetDataSource::GetNumberOfMemoryBuffers() {
        
        return 1u;
    }

    bool 
        ProfinetDataSource::GetSignalMemoryBuffer(
            const uint32 signalIdx,
            const uint32 bufferIdx,
            void*& signalAddress) {

            //TODO When upgrading to multiple buffer, pay attention to the bufferIdx assignment
            signalAddress = static_cast<void*>(signalIndexer[bufferIdx][signalIdx].firstByteOfSignal);

            return true;
    }

    const char8* 
        ProfinetDataSource::GetBrokerName(
            StructuredDataI& data,
            const SignalDirection direction) {
        
        const char8* brokerName = "";
        
        if(direction == InputSignals) {
            brokerName = "MemoryMapSynchNMutexInputBroker";
        }
        if(direction == OutputSignals) {
            brokerName = "MemoryMapSynchNMutexOutputBroker";
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Requested an invalid signal direction");
        }

        return brokerName;
    }

    bool 
        ProfinetDataSource::GetInputBrokers(
            ReferenceContainer& inputBrokers,
            const char8* const functionName,
            void* const gamMemPtr) {
        
        bool ok;
        ReferenceT<MemoryMapSynchNMutexInputBroker> broker("MemoryMapSynchNMutexInputBroker");
        ok = broker->Init(InputSignals, *this, functionName, gamMemPtr);
        if (ok) {
            ok = inputBrokers.Insert(broker);
        }
        return ok;
    }

    bool 
        ProfinetDataSource::GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void* const gamMemPtr) {
        
        bool ok;
        ReferenceT<MemoryMapSynchNMutexOutputBroker> broker("MemoryMapSynchNMutexOutputBroker");
        ok = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
        if (ok) {
            ok = outputBrokers.Insert(broker);
        }
        return ok;
    }

    bool ProfinetDataSource::SynchroniseInput() {
        return inputSignalsSemaphore.FastTryLock();
    }
    
    bool ProfinetDataSource::SynchroniseOutput() {
        return outputSignalsSemaphore.FastTryLock();
    }
    
    bool ProfinetDataSource::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
        bool returnValue = false;
        if(inputSignalsSemaphore.Locked()) {
            inputSignalsSemaphore.FastUnLock();
            returnValue = true;
        }
        return returnValue;
    }

    bool ProfinetDataSource::TerminateOutputCopy (const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
        bool returnValue = false;
        if(outputSignalsSemaphore.Locked()) {
            outputSignalsSemaphore.FastUnLock();
            returnValue = true;
        }
        return returnValue;
    }

    void ProfinetDataSource::SetLED(bool ledStatus) {
        if(profinetLedSignalEnabled) {
            *reinterpret_cast<uint8*>(signalIndexer[0][profinetLedSignalIndex].firstByteOfSignal) = ledStatus?1:0;
        }
    }

    void ProfinetDataSource::SetReady(bool readyStatus) {
        if(profinetReadySignalEnabled) {
            *reinterpret_cast<uint8*>(signalIndexer[0][profinetReadySignalIndex].firstByteOfSignal) = readyStatus?1:0;
        }
    }

    CLASS_REGISTER(ProfinetDataSource, "1.0")
}

