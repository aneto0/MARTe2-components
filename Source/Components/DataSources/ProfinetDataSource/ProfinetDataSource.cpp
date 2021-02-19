/**
 * @file ProfinetDataSource.cpp
 * @brief Source file for class ProfinetDataSource
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

  * @details This header file contains the declaration of the class ProfinetDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifndef LINT
#include <iostream>
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "ILoggerAdapter.h"
#include "ProfinetDataSource.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    ProfinetDataSource::ProfinetDataSource() :
                        DataSourceI() {

        	adapter = NULL_PTR(ProfinetDataSourceDriver::ProfinetDataSourceAdapter*);
		loggerAdapter = NULL_PTR(ProfinetDataSourceDriver::ILoggerAdapter*);
		periodicIntervalus = 0u;
		reductionRatio = 0u;
		vendorIdentifier = 0u;
		deviceIdentifier = 0u;
		oemVendorIdentifier = 0u;
		oemDeviceIdentifier = 0u;
		imVendor = 0u;
		imHardwareRevision = 0u;
		imFunctionalEnhancement = 0u;
		imBugFix = 0u;
		imInternalChange = 0u;
		imProfileIdentifier = 0u;
		imProfileSpecificType = 0u;
		imVersionMajor = 0u;
		imVersionMinor = 0u;
		imSoftwareRevision = ProfinetDataSourceDriver::SwRev_V;
		rtClass2Status = 0u;
		rtClass3Status = 0u;
		signalIndexerCount = 0u;
		inputHeap = NULL_PTR(uint8*);
		outputHeap = NULL_PTR(uint8*);
		inputHeapHalfSize = 0u;
		outputHeapHalfSize = 0u;
		profinetLedSignalEnabled = false;
		profinetLedSignalIndex = 0u;
		profinetReadySignalEnabled = false;
		profinetReadySignalIndex = 0u;
		signalIndexer[0] = NULL_PTR(profinet_marte_signal_t*);
		signalIndexer[1] = NULL_PTR(profinet_marte_signal_t*);

    }

    void ProfinetDataSource::SafeShutDown() {
        if(adapter != (NULL_PTR(ProfinetDataSourceDriver::ProfinetDataSourceAdapter*))) {
            bool abortStatus = adapter->AbortConnection();
			REPORT_ERROR(ErrorManagement::Information, "Connection %s", abortStatus?"aborted":"not aborted");
        }

        if(timerHelper.IsValid()) {
            timerHelper->Stop();
        }

        if(mainHelper.IsValid()) {
            mainHelper->Stop();
        }
        
        if(adapter != (NULL_PTR(ProfinetDataSourceDriver::ProfinetDataSourceAdapter*))) {
            delete adapter;
        }

	if(loggerAdapter != (NULL_PTR(ProfinetDataSourceDriver::ILoggerAdapter *))) {
	    delete loggerAdapter;
	}

	if(inputHeap != (NULL_PTR(uint8*))) {
	    void* inputHeapTmp = static_cast<void*>(inputHeap);
	    GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(inputHeapTmp);
	    inputHeap = NULL_PTR(uint8*);
        }

	if(outputHeap != (NULL_PTR(uint8*))) {
	    void* outputHeapTmp = static_cast<void*>(outputHeap);
	    GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(outputHeapTmp);
	    outputHeap = NULL_PTR(uint8*);
	}
    }

    ProfinetDataSource::~ProfinetDataSource() {
	//lint -esym(1579, MARTe::ProfinetDataSource::*)
	SafeShutDown();
    }

    bool 
        ProfinetDataSource::Initialise(StructuredDataI& data) {

        bool returnValue = DataSourceI::Initialise(data);
        
        uint32 imResultFlag = 0u;

        StreamString tempNetworkInterface;
        StreamString tempStationName;
        uint32 tempPeriodicIntervalus;
        uint32 tempReductionRatio;

        //Reading base configuration parameters
	//First flag is simply assigned, others are computed
        (data.Read("NetworkInterface", tempNetworkInterface))?(imResultFlag = PNETDS_MASK_BASENIC):(imResultFlag = 0u);

        (data.Read("StationName", tempStationName))?(imResultFlag |= PNETDS_MASK_BASESTATIONNAME):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_BASESTATIONNAME));
        (data.Read("PeriodicInterval", tempPeriodicIntervalus))?(imResultFlag |= PNETDS_MASK_BASEPERIODICINTERVAL):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_BASEPERIODICINTERVAL));
        (data.Read("ReductionRatio", tempReductionRatio))?(imResultFlag |= PNETDS_MASK_BASEREDUCTIONRATIO):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_BASEREDUCTIONRATIO));

        if((imResultFlag & PNETDS_MASK_BASE_MANDATORIES) == PNETDS_MASK_BASE_MANDATORIES) {
            REPORT_ERROR(ErrorManagement::Information, "Profinet will attach on %s network interface", tempNetworkInterface.Buffer());
            REPORT_ERROR(ErrorManagement::Information, "Station name %s", tempStationName.Buffer());
            REPORT_ERROR(ErrorManagement::Information, "Periodic interval cycling every %d", tempPeriodicIntervalus);
            REPORT_ERROR(ErrorManagement::Information, "Cyclic data sent every %d Profinet scan cycles", tempReductionRatio);
            
            networkInterface = tempNetworkInterface;
            stationName = tempStationName;
            periodicIntervalus = tempPeriodicIntervalus;
            reductionRatio = tempReductionRatio;

            loggerAdapter = static_cast<ProfinetDataSourceDriver::ILoggerAdapter *>(new ProfinetToMARTeLogAdapter());
            adapter = static_cast<ProfinetDataSourceDriver::ProfinetDataSourceAdapter*>(new ProfinetDataSourceDriver::ProfinetDataSourceAdapter(networkInterface.Buffer(), periodicIntervalus, stationName.Buffer(), reductionRatio, loggerAdapter));
		
	    //lint -e{774,948} operator new can return NULL when exceptions are disabled and allocations fails, altough very uncommon
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
                                                           ((imResultFlag & PNETDS_MASK_BASENIC) == 0u)?"NetworkInterface":"",
                                                           ((imResultFlag & PNETDS_MASK_BASESTATIONNAME) == 0u)?"StationName":"",
                                                           ((imResultFlag & PNETDS_MASK_BASEPERIODICINTERVAL) == 0u)?"PeriodicInterval":"",
                                                           ((imResultFlag & PNETDS_MASK_BASEREDUCTIONRATIO) == 0u)?"ReductionRatio":"");

            returnValue = false;
        }

        //Reading identification values
        if(returnValue) {            
            imResultFlag = 0u;

            uint16 tempVendorIdentifier;
            uint16 tempDeviceIdentifier;
            uint16 tempOEMVendorIdentifier;
            uint16 tempOEMDeviceIdentifier;
            //StreamString tempDeviceVendor;
            //StreamString tempManufacturerSpecificString;

            (data.Read("VendorIdentifier", tempVendorIdentifier))?(imResultFlag = PNETDS_MASK_BASEIDVENDORID):(imResultFlag = 0u);
            (data.Read("DeviceIdentifier", tempDeviceIdentifier))?(imResultFlag |= PNETDS_MASK_BASEIDDEVICEID):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_BASEIDDEVICEID));
            (data.Read("OEMVendorIdentifier", tempOEMVendorIdentifier))?(imResultFlag |= PNETDS_MASK_BASEIDOEMVENDORID):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_BASEIDOEMVENDORID));
            (data.Read("OEMDeviceIdentifier", tempOEMDeviceIdentifier))?(imResultFlag |= PNETDS_MASK_BASEIDOEMDEVICEID):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_BASEIDOEMDEVICEID));

            if((imResultFlag & PNETDS_MASK_BASEID_MANDATORIES) == PNETDS_MASK_BASEID_MANDATORIES) {
                REPORT_ERROR(ErrorManagement::Information, "Profinet Vendor IDentifier 0x%04x", tempVendorIdentifier);
                REPORT_ERROR(ErrorManagement::Information, "Profinet Device IDentifier 0x%04x", tempDeviceIdentifier);
                REPORT_ERROR(ErrorManagement::Information, "Profinet OEM Vendor IDentifier 0x%04x", tempOEMVendorIdentifier);
                REPORT_ERROR(ErrorManagement::Information, "Profinet OEM Device IDentifier 0x%04x", tempOEMDeviceIdentifier);

                vendorIdentifier = tempVendorIdentifier;
                deviceIdentifier = tempDeviceIdentifier;
                oemVendorIdentifier = tempOEMVendorIdentifier;
                oemDeviceIdentifier = tempOEMDeviceIdentifier;

                returnValue = true;
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Missing mandatory Profinet Base ID configuration keys, listed below");
                REPORT_ERROR(ErrorManagement::ParametersError,  "%s %s %s %s",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDVENDORID) == 0u)?"VendorIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDDEVICEID) == 0u)?"DeviceIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDOEMVENDORID) == 0u)?"OEMVendorIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_BASEIDOEMDEVICEID) == 0u)?"OEMDeviceIdentifier":"");

                returnValue = false;
            }
        }

        //Reading vendor identification values
        if(returnValue) {
            //TODO Check about endianess issues that may arise in these assumptions
            uint8 vendorIdHigh = static_cast<uint8>((vendorIdentifier >> 8u) & 0x00FFu);
            uint8 vendorIdLow = static_cast<uint8>((vendorIdentifier) & 0x00FFu);

            uint8 deviceIdHigh = static_cast<uint8>((deviceIdentifier >> 8u) & 0x00FFu);
            uint8 deviceIdLow = static_cast<uint8>((deviceIdentifier) & 0x00FFu);
            
            uint8 oemVendorIdHigh = static_cast<uint8>((oemVendorIdentifier >> 8u) & 0x00FFu);
            uint8 oemVendorIdLow = static_cast<uint8>((oemVendorIdentifier) & 0x00FFu);

            uint8 oemDeviceIdHigh = static_cast<uint8>((oemDeviceIdentifier >> 8u) & 0x00FFu);
            uint8 oemDeviceIdLow = static_cast<uint8>((oemDeviceIdentifier) & 0x00FFu);

            //lint -e{613} Null check was already done before and this point would have been unreachable
	    returnValue = adapter->SetBaseData(vendorIdHigh, vendorIdLow, deviceIdHigh, deviceIdLow, oemVendorIdHigh, oemVendorIdLow, oemDeviceIdHigh, oemDeviceIdLow, "name", 32u);
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
            StreamString tempIMSoftwareRevision;
            uint8 tempIMFunctionalEnhancement;
            uint8 tempIMBugFix;
            uint8 tempIMInternalChange;
            uint16 tempIMProfileIdentifier;
            uint16 tempIMProfileSpecificType;
            uint8 tempIMVersionMajor;
            uint8 tempIMVersionMinor;
            StreamString tempIMOrderId;
            StreamString tempIMSerialNumber;
            StreamString tempIMFunction;
            StreamString tempIMLocation;
            StreamString tempIMDate;
            StreamString tempIMDescriptor;
            StreamString tempIMSignature;

            (data.Read("IMVendor", tempIMVendor))?(imResultFlag = PNETDS_MASK_IMVENDOR):(imResultFlag = 0u);
            (data.Read("IMHardwareRevision", tempIMHardwareRevision))?(imResultFlag |= PNETDS_MASK_IMHARDWAREREVISION):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMHARDWAREREVISION));
            (data.Read("IMSoftwareRevision", tempIMSoftwareRevision))?(imResultFlag |= PNETDS_MASK_IMSOFTWAREREVISION):(imResultFlag &= ~PNETDS_MASK_IMSOFTWAREREVISION);

            (data.Read("IMFunctionalEnhancement", tempIMFunctionalEnhancement))?(imResultFlag |= PNETDS_MASK_IMFUNCTIONALENHANCEMENT):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMFUNCTIONALENHANCEMENT));
            (data.Read("IMBugFix", tempIMBugFix))?(imResultFlag |= PNETDS_MASK_IMBUGFIX):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMBUGFIX));
            (data.Read("IMInternalChange", tempIMInternalChange))?(imResultFlag |= PNETDS_MASK_IMINTERNALCHANGE):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMINTERNALCHANGE));

            (data.Read("IMProfileIdentifier", tempIMProfileIdentifier))?(imResultFlag |= PNETDS_MASK_IMPROFILEIDENTIFIER):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMPROFILEIDENTIFIER));
            (data.Read("IMProfileSpecificType", tempIMProfileSpecificType))?(imResultFlag |= PNETDS_MASK_IMPROFILESPECIFICTYPE):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMPROFILESPECIFICTYPE));

            (data.Read("IMVersionMajor", tempIMVersionMajor))?(imResultFlag |= PNETDS_MASK_IMVERSIONMAJOR):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMVERSIONMAJOR));
            (data.Read("IMVersionMinor", tempIMVersionMinor))?(imResultFlag |= PNETDS_MASK_IMVERSIONMINOR):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMVERSIONMINOR));

            (data.Read("IMOrderId", tempIMOrderId))?(imResultFlag |= PNETDS_MASK_IMORDERID):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMORDERID));
            (data.Read("IMSerialNumber", tempIMSerialNumber))?(imResultFlag |= PNETDS_MASK_IMSERIALNUMBER):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMSERIALNUMBER));

            (data.Read("IMFunction", tempIMFunction))?(imResultFlag |= PNETDS_MASK_IMFUNCTION):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMFUNCTION));
            (data.Read("IMLocation", tempIMLocation))?(imResultFlag |= PNETDS_MASK_IMLOCATION):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMLOCATION));
            (data.Read("IMDate", tempIMDate))?(imResultFlag |= PNETDS_MASK_IMDATE):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMDATE));
            (data.Read("IMDescriptor", tempIMDescriptor))?(imResultFlag |= PNETDS_MASK_IMDESCRIPTOR):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMDESCRIPTOR));
            (data.Read("IMSignature", tempIMSignature))?(imResultFlag |= PNETDS_MASK_IMSIGNATURE):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_IMSIGNATURE));

            if((imResultFlag & PNETDS_MASK_IM_MANDATORIES) == PNETDS_MASK_IM_MANDATORIES) {
                REPORT_ERROR(ErrorManagement::Information, "Mandatory I&M Data configured");
                imVendor = tempIMVendor;
                imHardwareRevision = tempIMHardwareRevision;
                
                switch(tempIMSoftwareRevision.Buffer()[0]) {
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

                imOrderId = ((imResultFlag & PNETDS_MASK_IMORDERID) == PNETDS_MASK_IMORDERID)?tempIMOrderId:"";
                imSerialNumber = ((imResultFlag & PNETDS_MASK_IMSERIALNUMBER) == PNETDS_MASK_IMSERIALNUMBER)?tempIMSerialNumber:"";
                imFunction = ((imResultFlag & PNETDS_MASK_IMFUNCTION) == PNETDS_MASK_IMFUNCTION)?tempIMFunction:"";
                imLocation = ((imResultFlag & PNETDS_MASK_IMLOCATION) == PNETDS_MASK_IMLOCATION)?tempIMLocation:"";
                imDate = ((imResultFlag & PNETDS_MASK_IMDATE) == PNETDS_MASK_IMDATE)?tempIMDate:"";
                imDescriptor = ((imResultFlag & PNETDS_MASK_IMDESCRIPTOR) == PNETDS_MASK_IMDESCRIPTOR)?tempIMDescriptor:"";
                imSignature = ((imResultFlag & PNETDS_MASK_IMSIGNATURE) == PNETDS_MASK_IMSIGNATURE)?tempIMSignature:"";
                uint8 imVendorHigh = static_cast<uint8>(static_cast<uint8>(imVendor >> 8u) & 0x00FFu);
                uint8 imVendorLow = static_cast<uint8>(imVendor & 0x00FFu);

	        //lint -e{613} Adapter cannot be null here, null check was done at startup 
                returnValue = adapter->SetIdentificationAndMaintainanceData(imVendorHigh, imVendorLow, imHardwareRevision, imSoftwareRevision, imFunctionalEnhancement, imBugFix, imInternalChange, imProfileIdentifier, imProfileSpecificType, imVersionMajor, imVersionMinor, imFunction, imOrderId, imSerialNumber, imLocation, imDate, imDescriptor, imSignature);
            }
            else {
                REPORT_ERROR(ErrorManagement::ParametersError, "Missing I&M Mandatory configuration keys, listed below");
                REPORT_ERROR(ErrorManagement::ParametersError,  "%s %s %s %s %s %s %s %s %s", 
                                                                ((imResultFlag & PNETDS_MASK_IMVENDOR) == 0u)?"IMVendor":"",
                                                                ((imResultFlag & PNETDS_MASK_IMHARDWAREREVISION) == 0u)?"IMHardwareRevision":"",
                                                                ((imResultFlag & PNETDS_MASK_IMSOFTWAREREVISION) == 0u)?"IMSoftwareRevision":"",
                                                                ((imResultFlag & PNETDS_MASK_IMFUNCTIONALENHANCEMENT) == 0u)?"IMFunctionalEnhancement":"",
                                                                ((imResultFlag & PNETDS_MASK_IMBUGFIX) == 0u)?"IMBugFix":"",
                                                                ((imResultFlag & PNETDS_MASK_IMINTERNALCHANGE) == 0u)?"IMInternalChange":"",
                                                                ((imResultFlag & PNETDS_MASK_IMPROFILEIDENTIFIER) == 0u)?"IMProfileIdentifier":"",
                                                                ((imResultFlag & PNETDS_MASK_IMPROFILESPECIFICTYPE) == 0u)?"IMProfileSpecificType":"",
                                                                ((imResultFlag & PNETDS_MASK_IMVERSIONMAJOR) == 0u)?"IMVersionMajor":"",
                                                                ((imResultFlag & PNETDS_MASK_IMVERSIONMINOR) == 0u)?"IMVersionMinor":"");
                REPORT_ERROR(ErrorManagement::ParametersError, "%s %s %s",
                    ((imResultFlag & PNETDS_MASK_IMSIGNATURE) == 0u)?"IMSignature":"",
                    ((imResultFlag & PNETDS_MASK_IMORDERID) == 0u)?"IMOrderId":"",
                    ((imResultFlag & PNETDS_MASK_IMSERIALNUMBER) == 0u)?"IMSerialNumber":"");

                returnValue = false;
            }
        }

        //Reading RTClass configuration data
        if(returnValue) {
            imResultFlag = 0u;

            uint16 tempRTClass2Status;
            uint16 tempRTClass3Status;

            (data.Read("RTClass2Status", tempRTClass2Status))?(imResultFlag = PNETDS_MASK_LLDP_RTCLASS2STATUS):(imResultFlag = 0u);
            (data.Read("RTClass3Status", tempRTClass3Status))?(imResultFlag |= PNETDS_MASK_LLDP_RTCLASS3STATUS):(imResultFlag &= static_cast<uint32>(~PNETDS_MASK_LLDP_RTCLASS3STATUS));

            rtClass2Status = ((imResultFlag & PNETDS_MASK_LLDP_RTCLASS2STATUS) == PNETDS_MASK_LLDP_RTCLASS2STATUS)?
				tempRTClass2Status:
				ProfinetDataSourceDriver::ProfinetDataSourceAdapter::GetDefaultLLDPRTClass2Status();
            rtClass3Status = ((imResultFlag & PNETDS_MASK_LLDP_RTCLASS3STATUS) == PNETDS_MASK_LLDP_RTCLASS3STATUS)?
				tempRTClass3Status:ProfinetDataSourceDriver::ProfinetDataSourceAdapter::GetDefaultLLDPRTClass3Status();

            REPORT_ERROR(ErrorManagement::Information, "RT Class2/Class3 %d/%d", rtClass2Status, rtClass3Status);
	    
 	    //lint -e{613} Adapter cannot be null here, null check was done at startup
            adapter->SetLLDPData(rtClass2Status, rtClass3Status);
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

                for(uint32 slotIndexLoop = 0u; (slotIndexLoop < slotsCount) && returnValue; slotIndexLoop++) {
					//This line just avoids linter complaining about					
					uint32 slotIndex = slotIndexLoop;
                    REPORT_ERROR(ErrorManagement::Information, "Slot idx %d", slotIndex);
                    uint16 tempSlotNumber = 0u;
                    if(data.MoveToChild(slotIndex)) {
                        if(data.Read("SlotNumber", tempSlotNumber)) {
                            REPORT_ERROR(ErrorManagement::Information, "Found SlotNumber %d, adding it to the driver structure", tempSlotNumber);
                            //lint -e{613} Adapter cannot be null here as nullity was before checked
			    adapter->AddSlot(tempSlotNumber);

                            if(data.MoveRelative("Subslots")) {
                                uint32 subslotsCount = data.GetNumberOfChildren();
                                REPORT_ERROR(ErrorManagement::Information, "Slot %d will have %d subslots", tempSlotNumber, subslotsCount);
                                for(uint32 subslotIndex = 0u; (subslotIndex < subslotsCount) && returnValue; subslotIndex++) {
                                    REPORT_ERROR(ErrorManagement::Information, "Working on subslot %s", data.GetChildName(subslotIndex));
                                    if(data.MoveToChild(subslotIndex)) {
                                        
                                        uint32 tempResultFlag = 0u;

                                        uint16 tempSubslotNumber = 0u;
                                        uint8 tempIsDAP = 0u;
                                        uint8 tempExpectedDataDirection = 0u;
                                        uint16 tempExpectedInputSize = 0u;
                                        uint16 tempExpectedOutputSize = 0u;
                                        
                                        (data.Read("SubslotNumber", tempSubslotNumber))?(tempResultFlag = PNETDS_MASK_SUBSLOT_NUMBER):(tempResultFlag = 0u);
                                        (data.Read("DeviceAccessPoint", tempIsDAP))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_ISDAP):(tempResultFlag &= static_cast<uint32>(~PNETDS_MASK_SUBSLOT_ISDAP));
                                        (data.Read("ExpectedDataDirection", tempExpectedDataDirection))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_EXPDIR):(tempResultFlag &= static_cast<uint32>(~PNETDS_MASK_SUBSLOT_EXPDIR));
                                        (data.Read("ExpectedInputSize", tempExpectedInputSize))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_EXPINSIZE):(tempResultFlag &= static_cast<uint32>(~PNETDS_MASK_SUBSLOT_EXPINSIZE));
                                        (data.Read("ExpectedOutputSize", tempExpectedOutputSize))?(tempResultFlag |= PNETDS_MASK_SUBSLOT_EXPOUTSIZE):(tempResultFlag &= static_cast<uint32>(~PNETDS_MASK_SUBSLOT_EXPOUTSIZE));

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
						default:
						    //It is safe to fallback to a "default" direction as eventually the Profinet master will refuse the Module/Submodule pluggin
						    tempDirection = ProfinetDataSourceDriver::NoInputOutput;
						    REPORT_ERROR(ErrorManagement::FatalError, "Invalid direction given, falling back no NoInputOutput");
						    break;
                                            }
                                            //lint -e{641} Integer conversion of enum is done for printing purposes only 
                                            REPORT_ERROR(ErrorManagement::Information, "Found subslot %d [DAP:%d DIR:%d IN:%d OUT: %d]", tempSubslotNumber, tempIsDAP, tempDirection, tempExpectedInputSize, tempExpectedOutputSize);

                                            if(tempExpectedInputSize > 0u) {
                                                heapSizeToAllocateInputs += tempExpectedInputSize;
                                            }

                                            if(tempExpectedOutputSize > 0u) {
                                                heapSizeToAllocateOutputs += tempExpectedOutputSize;
                                            }                                           
                                            
					    //lint -e{613} Adapter cannot be null here, nullity was checked before
                                            adapter->AddSubslot(tempSlotNumber, tempSubslotNumber, (tempIsDAP == 1u), tempDirection, tempExpectedInputSize, tempExpectedOutputSize);
                                        }
                                        else {
                                            REPORT_ERROR(ErrorManagement::ParametersError, "Missing subslot mandatory configuration keys, listed below");
                                            REPORT_ERROR(ErrorManagement::ParametersError,  "%s %s %s %s %s", 
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_NUMBER) == 0u)?"SubslotNumber":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_ISDAP) == 0u)?"DeviceAccessPoint":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_EXPDIR) == 0u)?"ExpectedDataDirection":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_EXPINSIZE) == 0u)?"ExpectedInputSize":"",
                                                                                            ((tempResultFlag & PNETDS_MASK_SUBSLOT_EXPOUTSIZE) == 0u)?"ExpectedOutputSize":"");
                                            returnValue = false;
                                        }
                                        if(!data.MoveToAncestor(1u)) {
                                            returnValue = false;
                                            REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to ancestor node");
                                            //break;
                                        }
                                    }
                                    else {
                                        REPORT_ERROR(ErrorManagement::FatalError, "Cannot move to child %s", data.GetChildName(subslotIndex));
                                        returnValue = false;
                                        //break;
                                    }
                                }//LOOP in SUBSLOTS
                                if(!data.MoveToAncestor(1u)) {
                                    returnValue = false;
                                    REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to ancestor node");
                                    //break;
                                }
                            }
                            else {
                                REPORT_ERROR(ErrorManagement::ParametersError, "Missing Subslots information configuration block");
                                returnValue = false;
                                //break;
                            }
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::ParametersError, "Missing SlotNumber configuration key");
                            returnValue = false;
                            //break;
                        }
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot move to specified subslot");
                        returnValue = false;
                        //break;
                    }
                    if(!data.MoveToAncestor(1u)) {
                        returnValue = false;
                        REPORT_ERROR(ErrorManagement::FatalError, "Cannot return to ancestor node");
                        //break;
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
                inputHeap = reinterpret_cast<uint8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc((heapSizeToAllocateInputs * 2u) + 1u));
                //Underlying malloc leaves undetermined memory. Cleaning it.
                returnValue = MemoryOperationsHelper::Set(inputHeap, static_cast<char8>(0x00u), (heapSizeToAllocateInputs * 2u) + 1u);
		if(!returnValue) {
		    REPORT_ERROR(ErrorManagement::FatalError, "Cannot initially clear heap for inputs");
		}
	    }

	    if(returnValue) {
                inputHeapHalfSize = heapSizeToAllocateInputs;
                returnValue = (inputHeap != NULL_PTR(uint8*));
		if(!returnValue) {
	            REPORT_ERROR(ErrorManagement::FatalError, "Cannot allocate heap for input bank");
		}
            }
        }

        //Allocate double the memory of the outputs, one half for Profinet, the other for MARTe
        if(returnValue) {
            if(heapSizeToAllocateOutputs > 0u) {
                REPORT_ERROR(ErrorManagement::Information, "Allocating %d * 2 bytes for outputs", heapSizeToAllocateOutputs);
                //Allocating the size needed for the outputs + 3, which will be reserved to LED, Ready and Alarm special signals
                outputHeap = reinterpret_cast<uint8 *>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc((heapSizeToAllocateOutputs * 2u) + 3u));
                //Underlying malloc leaves undetermined memory. Cleaning it.

                returnValue = MemoryOperationsHelper::Set(outputHeap, static_cast<char8>(0x00u), (heapSizeToAllocateOutputs * 2u) + 3u);
		if(!returnValue) {
		    REPORT_ERROR(ErrorManagement::FatalError, "Cannot initially clear heap for outputs");
		}
	    }
	    if(returnValue) {
                outputHeapHalfSize = heapSizeToAllocateOutputs;
                returnValue = (outputHeap != NULL_PTR(uint8*));
	        if(!returnValue) {
	            REPORT_ERROR(ErrorManagement::FatalError, "Cannot allocate heap for output bank");
	        }
            }
        }

        //Scan the Profinet Slot/Subslot structure and assign the previously allocated memory, on the first half of each buffer (I/O)
        if(returnValue) {
            uint8* inputHeapIndex = inputHeap;
            uint8* outputHeapIndex = outputHeap;
            
            //lint -e{613} Adapter cannot be null as nullity was checked before
     	    ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_slot_t*>* slotsIterator = adapter->GetAPIStructureIterator();
	    //lint -e{665} Parentheses are checked and good
            if(slotsIterator != (NULL_PTR(ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_slot_t*>*))) {
                //lint -e{831,1712} Default constructor is not intended to be called as the Iterator is obtained through factory method 
		slotsIterator->First();
                ProfinetDataSourceDriver::pnet_slot_t* tempSlot = NULL_PTR(ProfinetDataSourceDriver::pnet_slot_t*);
                ProfinetDataSourceDriver::pnet_subslot_t* tempSubslot = NULL_PTR(ProfinetDataSourceDriver::pnet_subslot_t*);
		//lint -e{838} tempSlot variable is used to recall the GetIterator
                while((tempSlot = slotsIterator->Next()) != NULL_PTR(ProfinetDataSourceDriver::pnet_slot_t*)) {
                    REPORT_ERROR(ErrorManagement::Information, "Scanning slot %d", tempSlot->slotNumber);
                    ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_subslot_t*>* subslotsIterator = tempSlot->subslots.GetIterator();
                    subslotsIterator->First();
                    if(subslotsIterator != NULL_PTR(ProfinetDataSourceDriver::SimpleLinkedListIterator<ProfinetDataSourceDriver::pnet_subslot_t*>*)) {
                        while((tempSubslot = subslotsIterator->Next()) != NULL_PTR(ProfinetDataSourceDriver::pnet_subslot_t*)) {
                            if(tempSubslot->expectedConfigurationData.insize > 0u) {
				//lint -e{613,946,947,9016} Pointer arithmetics is used to do a double-check on heap boundaries. Null checks are done before.
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
                                //lint -e{613,946,947,9016} Pointer arithmetics is used to do a double-check on heap boundaries. Null checks are done before.
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
                //lint -e{9007} Purpotedly using directly functions as if conditions, order also takes into account if short-circuit (no write if no MoveRelative)
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
		    //lint -e{712,747,9114,9119} Operations are done using standard sizeof pointer while Malloc takes a uint32 for the size specification
                    signalIndexer[0] = reinterpret_cast<profinet_marte_signal_t*>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(sizeof(profinet_marte_signal_t) * signalIndexerCount));

                    //TODO Eventual multiple buffers should be initialised here, like this
                    //signalIndexer[1] = reinterpret_cast<profinet_marte_signal_t*>(GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(sizeof(profinet_marte_signal_t) * signalIndexerCount));

                    if((signalIndexer[0] == NULL_PTR(profinet_marte_signal_t*))) { // || (signalIndexer[1] == NULL_PTR(profinet_marte_signal_t*))) {
                        REPORT_ERROR(ErrorManagement::FatalError, "Cannot allocate the signal indexer structure");
                        returnValue = false;
                    }

                    for(uint32 signalIndexLoop = 0u; (signalIndexLoop < signalIndexerCount) && (returnValue); signalIndexLoop++) {
                        uint32 signalIndex = signalIndexLoop;
			REPORT_ERROR(ErrorManagement::Information, "Moving to child %s", data.GetChildName(signalIndex));
                        StreamString tempSignalName = data.GetChildName(signalIndex);
                        if(data.MoveToChild(signalIndex)) {
                            //First check if we have one of the Profinet special signals, else it will be a standard signal
                            if(tempSignalName == "ProfinetDeviceLed") {
                                REPORT_ERROR(ErrorManagement::Information, "Found LED signal");
                                //First byte of LED signal is located after the output bank half
				//lint -e{613,9016} Memory map is built manually and outputHeap nullity is checked before arriving here
				signalIndexer[0][signalIndex].firstByteOfSignal = static_cast<uint8*>(outputHeap +  (2u * outputHeapHalfSize));
                                signalIndexer[0][signalIndex].direction = 1u;
                                signalIndexer[0][signalIndex].needsSwapping = false; 
                                profinetLedSignalIndex = signalIndex;
                                returnValue = MemoryOperationsHelper::Copy(&signalIndexer[0][signalIndex].marteName[0], &tempSignalName.Buffer()[0], static_cast<uint32>(tempSignalName.Size() + 1u));
                                profinetLedSignalEnabled = true;
                            }
                            else if(tempSignalName == "ProfinetDeviceReady") {
                                REPORT_ERROR(ErrorManagement::Information, "Found Ready signal");
                                //First byte of LED signal is located after the output bank half + 1
                                //lint -e{613,9016} Memory map is build manually and outputHeap nullity is checked before arriving here
                                signalIndexer[0][signalIndex].firstByteOfSignal = static_cast<uint8*>(outputHeap +  (2u * outputHeapHalfSize) + 1u);
                                signalIndexer[0][signalIndex].direction = 1u;
                                signalIndexer[0][signalIndex].needsSwapping = false;
                                profinetReadySignalIndex = signalIndex;
                                returnValue = MemoryOperationsHelper::Copy(&signalIndexer[0][signalIndex].marteName[0], &tempSignalName.Buffer()[0], static_cast<uint32>(tempSignalName.Size() + 1u));
                                profinetReadySignalEnabled = true;
                            }
                            else {
                                uint16 tempSlotNumber = 0u;
                                uint16 tempSubslotNumber = 0u;
                                uint8 tempDirectionValue = 0u;
                                uint32 tempOffsetValue = 0u;
                                uint8 tempNeedsSwapping = false;
				//lint -e{9007} Use of functions directly in conditions is done on purpose
                                if(data.Read("Slot", tempSlotNumber) && data.Read("Subslot", tempSubslotNumber) && data.Read("Direction", tempDirectionValue) && data.Read("Offset", tempOffsetValue)) {
                                    //lint -e{534} Return value is ignored as the parameter is not mandatory
				    data.Read("NeedsSwapping", tempNeedsSwapping);
				    //lint -e{613} adapter cannot be null here, nullity was checked before
                                    ProfinetDataSourceDriver::pnet_subslot_t* tempSubslot = adapter->GetSubslot(tempSlotNumber, tempSubslotNumber);
                                    if(tempSubslot != NULL_PTR(ProfinetDataSourceDriver::pnet_subslot_t*)) {
                                        if(tempDirectionValue == 0u) {
                                            //MARTe signals are located in the other half of the input heap, this is why we need to shift forward of one inputHeapHalfSize
                                            //lint -e{9016} Pointer arithmetic is extensively used to manually build process image memory map
                                            signalIndexer[0][signalIndex].firstByteOfSignal = tempSubslot->inputData + tempOffsetValue + inputHeapHalfSize;
                                            REPORT_ERROR(ErrorManagement::Information, "Input signal %d will be located at Slot/Subslot + Offset [%d/%d + %d]", signalIndex, tempSlotNumber, tempSubslotNumber, tempOffsetValue);
                                        }
                                        else if(tempDirectionValue == 1u) {
                                            //MARTe signals are located in the other half of the output heap, this is why we need to shift forward of one outputHeapHalfSize
                                            //lint -e{9016} Pointer arithmetic is extensively used to manually build process image memory map
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
                                            returnValue = MemoryOperationsHelper::Copy(&signalIndexer[0][signalIndex].marteName[0], &tempSignalName.Buffer()[0], static_cast<uint32>(tempSignalName.Size() + 1u));
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

            for(uint32 i = 0u; (i < size) && (!(foundMain && foundTimer)); i++) {
                ReferenceT<ProfinetTimerHelper> refT = Get(i);
                ReferenceT<ProfinetMainThreadHelper> refM = Get(i);
		//lint -e{9007} IsValid call has no side effects and is used purpotedly, also respecting if short-circuit ordering
                if((!foundTimer) && refT.IsValid()) {
                    timerHelper = refT;
                    foundTimer = true;
                    //lint -e{534} SetEntryPoint return value cannot be null as "this" is never null
		    timerHelper->SetEntryPoint(this);
                    REPORT_ERROR(ErrorManagement::Information, "Found Timer helper");
                }
		//lint -e{9007} IsValid call has no side effects and is used purpotedly, also respecting if short-circuit ordering
                if((!foundMain) && refM.IsValid()) {
                    mainHelper = refM;
                    foundMain = true;
                    //lint -e{534} SetEntryPoint return value cannot be null as "this" is never null
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
	    //lint -e{613} Adapter cannot be null here. Nullity was checked before
            adapter->cyclicNotificationListener = this;
           
	    //lint -e{613} Adapter cannot be null here. Nullity was checked before
	    adapter->profinetEventNotificationListener = this;
  
	    //lint -e{613} Adapter cannot be null here. Nullity was checked before
	    adapter->opSignalsEntryPoint = this;
	    
	    //lint -e{613} Adapter cannot be null here. Nullity was checked before
            returnValue = adapter->Initialize();
        }

        if(returnValue) {
            //Main helper times out after 10 missed cycles
            mainHelper->SetPeriodicInterval(static_cast<float32>(periodicIntervalus) * 10.0e-6F);
            mainHelper->SetTimeout(static_cast<float32>(periodicIntervalus) * 10.0e-6F * 10.0F);

            timerHelper->SetTimerInterval(static_cast<float64>(periodicIntervalus) * 10.0e-6F);
	    
            mainHelper->Start();
            timerHelper->Start();
        }
	if(!returnValue) {
	    REPORT_ERROR(ErrorManagement::FatalError, "Failure in DataSource initialization");
	}

        return returnValue;
    }

    uint16 ProfinetDataSource::MainThread(const uint16 inputFlag) {
	//lint -e{613} Adapter cannot be null here. Nullity was checked before
        return adapter->MainThread(inputFlag);
    }

    void ProfinetDataSource::TimerTick() {
	//lint -e{613} Adapter cannot be null here. Nullity was checked before
        adapter->TaskTimerTick();
    }

    void ProfinetDataSource::NotifyCycle() {
        if(inputSignalsSemaphore.FastLock() == ErrorManagement::NoError) {
            //MARTe inputs must be copied into Profinet inputs (e.g. from the MARTe half to the Profinet half of the heap buffer)
            //lint -e{613,9016} Null checks on inputHeap variable are done before
	    bool inputCopy = MemoryOperationsHelper::Copy(inputHeap, inputHeap + inputHeapHalfSize, static_cast<uint32>(inputHeapHalfSize));
	    if(inputCopy) {
	        inputSignalsSemaphore.FastUnLock();
		        
	        //Swapping occurs outside the critical section, as it is not affecting the MARTe side
	        //There is no reason to also lock MARTe while swapping data on the Profinet side
	        //NOTE firstByteOfSignal is on the MARTe side, this is why we have to move pointer backwards to the Profinet section
	        for(uint32 signalIndex = 0u; signalIndex < signalIndexerCount; signalIndex++) {
	            if((signalIndexer[0u][signalIndex].direction == 0u) && (signalIndexer[0u][signalIndex].needsSwapping)) {
	                switch(signalIndexer[0u][signalIndex].signalSize) {
	                    case 2u:
			    //lint -e{826,927,9016} Pointer arithmetics should be reasonably safe here
	                    Endianity::ToBigEndian(*reinterpret_cast<int16*>(signalIndexer[0u][signalIndex].firstByteOfSignal - inputHeapHalfSize));
	                    break;
	                    case 4u:
			    //lint -e{826,927,9016} Pointer arithmetics should be reasonably safe here
	                    Endianity::ToBigEndian(*reinterpret_cast<int32*>(signalIndexer[0u][signalIndex].firstByteOfSignal - inputHeapHalfSize));
	                    break;
	                    case 8u:
			    //lint -e{826,927,9016} Pointer arithmetics should be reasonably safe here
	                    Endianity::ToBigEndian(*reinterpret_cast<int64*>(signalIndexer[0u][signalIndex].firstByteOfSignal - inputHeapHalfSize));
	                    break;
	                    default:
	                    //TODO Check if really you can do nothing or cases exists where bigger swaps may occur
	                    break;
	                }
	            }
	        }
	    }
        }

        if(outputSignalsSemaphore.FastLock() == ErrorManagement::NoError) {
            //Profinet outputs must be copied into MARTe outputs (e.g. from the Profinet half to the MARTe half of the heap buffer)
            //lint -e{613,9016} Null checks on outputHeap variable are done before, pointer arithmetics is reasonably safe here
	    bool outputCopy = MemoryOperationsHelper::Copy(outputHeap + outputHeapHalfSize, outputHeap, static_cast<uint32>(outputHeapHalfSize));

	    if(outputCopy) {
            	//Swapping occurs inside the critical section, as we are swapping bytes on the MARTe side
            	//MARTe must still wait before working on the signal buffer
            	//NOTE firstByteOfSignal is on the MARTe side, this time no need to move pointers
            	for(uint32 signalIndex = 0u; signalIndex < signalIndexerCount; signalIndex++) {
                    if((signalIndexer[0u][signalIndex].direction == 1u) && (signalIndexer[0u][signalIndex].needsSwapping)) {
                    	switch(signalIndexer[0u][signalIndex].signalSize) {
                            case 2u:
			    //lint -e{826,927,9016} Pointer arithmetics should be reasonably safe here
                            Endianity::FromBigEndian(*reinterpret_cast<int16*>(signalIndexer[0u][signalIndex].firstByteOfSignal));
                            break;
                            case 4u:
			    //lint -e{826,927,9016} Pointer arithmetics should be reasonably safe here
                            Endianity::FromBigEndian(*reinterpret_cast<int32*>(signalIndexer[0u][signalIndex].firstByteOfSignal));
                            break;
                            case 8u:
			    //lint -e{826,927,9016} Pointer arithmetics should be reasonably safe here
                            Endianity::FromBigEndian(*reinterpret_cast<int64*>(signalIndexer[0u][signalIndex].firstByteOfSignal));
                            break;
                            default:
                            //TODO Check if really you can do nothing or cases exists where bigger swaps may occur
                            break;
                        }
                    }
                }
	    }

            outputSignalsSemaphore.FastUnLock();
        }
    }

    void ProfinetDataSource::NotifyEvent(const ProfinetDataSourceEventType eventType) {
        mainHelper->NotifyEvent(eventType);
    }

    bool 
        ProfinetDataSource::SetConfiguredDatabase(
            StructuredDataI& data) {

        bool returnValue = DataSourceI::SetConfiguredDatabase(data);
        uint32 tempSignalMARTeIndex = 0u;

        for(uint32 sigIndex = 0u; sigIndex < signalIndexerCount; sigIndex++) {
	    //Avoid Linter complaining about changing index in the loop
	    uint32 signalIndex = sigIndex;
            returnValue = GetSignalIndex(tempSignalMARTeIndex, &signalIndexer[0u][signalIndex].marteName[0u]);
            if(returnValue) {
                returnValue = GetSignalByteSize(tempSignalMARTeIndex, signalIndexer[0u][signalIndex].signalSize);
            }

	    if(returnValue) {
                signalIndexer[0u][signalIndex].marteIndex = tempSignalMARTeIndex;

                StreamString tempSignalStateName;
                uint32 tempNumberOfProducers = 0u;
                uint32 tempNumberOfConsumers = 0u;

                returnValue = GetSignalStateName(tempSignalMARTeIndex, 0u, tempSignalStateName);
                if(signalIndexer[0u][signalIndex].direction == 0u) {
                    returnValue = GetSignalNumberOfProducers(tempSignalMARTeIndex, &tempSignalStateName.Buffer()[0u], tempNumberOfProducers);
                    if(!returnValue) {
			StreamString marteName = &signalIndexer[0u][signalIndex].marteName[0];
			//lint -e{923,1786} Unusual conversions are used for printing purposes
                        REPORT_ERROR("Cannot configure signal MARTe Idx/Internal Idx %d/%d, with name %s. No producers connected to input",
                        signalIndexer[0u][signalIndex].marteIndex,
                        signalIndex,
                        marteName.Buffer());
                    }
                    else if(tempNumberOfProducers > 1u) {
			StreamString marteName = &signalIndexer[0u][signalIndex].marteName[0u];
			//lint -e{923,1786} Unusual conversions are used for printing purposes
                        REPORT_ERROR("Cannot configure signal MARTe Idx/Internal Idx %d/%d, with name %s. Number of producers must be exactly 0, %d found instead.",
                        signalIndexer[0u][signalIndex].marteIndex,
                        signalIndex,
                        marteName.Buffer(),
                        tempNumberOfProducers);                        

                        returnValue = false;
                    }
		    else {
			/* Portion added to satisfy linting but will never be reached as invalid signal direction cause an early discard of the DataSource */
		    }

                    if(returnValue) {
                        returnValue = !GetSignalNumberOfConsumers(tempSignalMARTeIndex, &tempSignalStateName.Buffer()[0u], tempNumberOfConsumers);
                        if(!returnValue) {
			    StreamString marteName = &signalIndexer[0u][signalIndex].marteName[0u];
                            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot configure signal MARTe Idx/Internal Idx %d/%d, with name %s. It is produced and consumed at the same time",
                            signalIndexer[0u][signalIndex].marteIndex,
                            signalIndex,
                            marteName.Buffer());

                            returnValue = false;
                        }
                    }
                }
                else if(signalIndexer[0u][signalIndex].direction == 1u) {
                    returnValue = GetSignalNumberOfConsumers(tempSignalMARTeIndex, &tempSignalStateName.Buffer()[0u], tempNumberOfConsumers);
                    if(!returnValue) {
			StreamString marteName = &signalIndexer[0u][signalIndex].marteName[0u];
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot configure signal MARTe Idx/Internal Idx %d/%d, with name %s. No consumers connected to output.",
                        signalIndexer[0u][signalIndex].marteIndex,
                        signalIndex,
                        marteName.Buffer());
                    }
                    else if(tempNumberOfConsumers > 1u) {
			StreamString marteName = &signalIndexer[0u][signalIndex].marteName[0u];
                        REPORT_ERROR(ErrorManagement::ParametersError, "Cannot configure signal MARTe Idx/Internal Idx %d/%d, with name %s. Number of consumers must be exactly 0, %d found instead.",
                        signalIndexer[0u][signalIndex].marteIndex,
                        signalIndex,
                        marteName.Buffer(),
                        tempNumberOfConsumers);

                        returnValue = false;
                    }
		    else {
			/* Portion added to satisfy linting but will never be reached as invalid signal direction cause an early discard of the DataSource */
		    }

                    if(returnValue) {
                        returnValue = !GetSignalNumberOfProducers(tempSignalMARTeIndex, &tempSignalStateName.Buffer()[0u], tempNumberOfProducers);
                        if(!returnValue) {
			    StreamString marteName = &signalIndexer[0u][signalIndex].marteName[0u];
                            REPORT_ERROR(ErrorManagement::ParametersError, "Cannot configure signal MARTe Idx/Internal Idx %d/%d, with name %s. It is produced and consumed at the same time",
                            signalIndexer[0u][signalIndex].marteIndex,
                            signalIndex,
                            marteName.Buffer());

                            returnValue = false;
                        }
                    }
                }
		else {
			/* Portion added to satisfy linting but will never be reached as invalid signal direction cause an early discard of the DataSource */
		}

                REPORT_ERROR(ErrorManagement::Information, "Indexer acquired signal MARTe Idx/Internal Idx %d/%d, with name %s which is %d bytes (NOP/NOC = %d/%d)", 
                    signalIndexer[0u][signalIndex].marteIndex,
                    signalIndex,
                    signalIndexer[0u][signalIndex].marteName, 
                    signalIndexer[0u][signalIndex].signalSize,
                    tempNumberOfProducers,
                    tempNumberOfConsumers);
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

    //lint -e{715} currentStateName and nextStateName are unreferenced because the DataSource uses single state
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
    
    //lint -e{715} data variable is not used as the DataSource only supports two brokers 1I and 1O
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
    
    //lint -e{715,830} signalIdx, offset and numberOfSamples are not used in this context as the termination is seen as a whole block signaling
    bool ProfinetDataSource::TerminateInputCopy(const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
        bool returnValue = false;
        if(inputSignalsSemaphore.Locked()) {
            inputSignalsSemaphore.FastUnLock();
            returnValue = true;
        }
        return returnValue;
    }

    //lint -e{715,830} signalIdx, offset and numberOfSamples are not used in this context as the termination is seen as a whole block signaling
    bool ProfinetDataSource::TerminateOutputCopy (const uint32 signalIdx, const uint32 offset, const uint32 numberOfSamples) {
        bool returnValue = false;
        if(outputSignalsSemaphore.Locked()) {
            outputSignalsSemaphore.FastUnLock();
            returnValue = true;
        }
        return returnValue;
    }

    void ProfinetDataSource::SetLED(const bool ledStatus) {
        if(profinetLedSignalEnabled) {
            *reinterpret_cast<uint8*>(signalIndexer[0][profinetLedSignalIndex].firstByteOfSignal) = ledStatus?1u:0u;
        }
    }

    void ProfinetDataSource::SetReady(const bool readyStatus) {
        if(profinetReadySignalEnabled) {
            *reinterpret_cast<uint8*>(signalIndexer[0][profinetReadySignalIndex].firstByteOfSignal) = readyStatus?1u:0u;
        }
    }

    void ProfinetDataSource::Abort() {
        if(timerHelper.IsValid()) {
            timerHelper->Stop();
            //timerHelper->SetEntryPoint(NULL_PTR(ITimerEntryPoint*));
        }
        
        if(mainHelper.IsValid()) {
            mainHelper->Stop();
            //mainHelper->SetEntryPoint(NULL_PTR(IMainThreadEntryPoint*));
        }
    }

    CLASS_REGISTER(ProfinetDataSource, "1.0")
}

