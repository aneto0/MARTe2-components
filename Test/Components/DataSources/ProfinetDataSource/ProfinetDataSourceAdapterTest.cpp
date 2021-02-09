/**
 * @file ProfinetDataSourceAdapterTest.cpp
 * @brief Source file for the ProfinetDataSourceAdapterTest
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

 * @details This header file contains the declaration of the class ProfinetDataSourceAdapterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#define PNET_MAX_DIRECTORYPATH_LENGTH 4095
#define TEST_NUMBER_OF_SLOTS 10

#include "ProfinetDataSourceConstants.h"
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string>
#include <ostream>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include <pnet_api.h>
#include "ProfinetDataSourceAdapterTest.h"
#include "StreamString.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ProfinetDataSourceAdapterTest::ProfinetDataSourceAdapterTest() {
    this->adapter = NULL;
    this->log = NULL;
}

bool ProfinetDataSourceAdapterTest::EnvironmentSetup(
        bool emptyIfName,
        bool getIfNameFromEnv,
        std::string stationName,
        uint32_t periodicInterval,
        uint32_t gearRatio,
        bool nullLogAdapter) {

    std::string ethInterface = emptyIfName?"":"dummyeth";

    //We try to maximize executed tests by running them even if user forgets setting the env
    //Moreover, some of them do not necessarily need a real eth interface to bind on
    //This is why they can stay with a dummy name
    //However if you explicitly ask for env interface name, you have to specify it
    bool envSetupResult = false;

    if(getIfNameFromEnv) {
        char* envEth = getenv("PROFINET_ETH_INTERFACE");

        if(envEth != NULL) {
            envSetupResult = true;
            ethInterface = envEth;
            std::cout << "env PROFINET_ETH_INTERFACE is " << ethInterface << std::endl;
        }
        else {
            std::cout << "env PROFINET_ETH_INTERFACE was null" << std::endl;
            envSetupResult = false;
        }
    }
    else {
        envSetupResult = true;
    }

    if(envSetupResult) {
        std::cout << "Using \"" << ethInterface << "\" as Profinet interface" << std::endl;
        
        if(!nullLogAdapter) {
            this->log = new MARTe::ProfinetToMARTeLogAdapter();
        }
        
        this->adapter = new ProfinetDataSourceDriver::ProfinetDataSourceAdapter(ethInterface.c_str(), periodicInterval, stationName, gearRatio, log);
        bool envSetupResult = (log != NULL) && (adapter != NULL);
    }

    return envSetupResult;
}

ProfinetDataSourceAdapterTest::~ProfinetDataSourceAdapterTest() {
    if(this->adapter != NULL) {
        delete this->adapter;
    }

    if(this->log != NULL) {
        delete this->log;
    }
}

bool ProfinetDataSourceAdapterTest::TestConstructor() {

    bool validTest = EnvironmentSetup(
        false,
        true,
        "constructortest",
        10000,
        10,
        false
    );

    if(validTest) {
        validTest = adapter->CheckBaseNetworkingData();
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestConstructorFailEmptyLoggerAdapter() {
    bool validTest = EnvironmentSetup(
        false,
        true,
        "failnullogger",
        10000,
        10,
        true
    );
    
    if(validTest) {
        validTest = adapter->CheckBaseNetworkingData();
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestConstructorFailZeroPeriodicInterval() {
    bool validTest = EnvironmentSetup(
        false,
        true,
        "failzeroperiodic",
        0,
        10,
        false
    );

    if(validTest) {
        validTest = adapter->CheckBaseNetworkingData();
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestConstructorFailEmptyStationName() {
    bool validTest = EnvironmentSetup(
        false,
        true,
        "",
        10000,
        10,
        false
    );

    if(validTest) {
        validTest = adapter->CheckBaseNetworkingData();
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestConstructorFailEmptyInterface() {
    bool validTest = EnvironmentSetup(
        true,
        false,
        "failemptyif",
        10000,
        10,
        false
    );

    if(validTest) {
        validTest = adapter->CheckBaseNetworkingData();
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestConstructorFailZeroGearRatio() {
    bool validTest = EnvironmentSetup(
        false,
        true,
        "failzerogearratio",
        10000,
        0,
        false
    );

    if(validTest) {
        validTest = adapter->CheckBaseNetworkingData();
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestConstructorFailNetworkDataSetup() {
    bool validTest = EnvironmentSetup(
        false,
        false,
        "dummyethintfc",
        10000,
        10,
        false
    );

    if(validTest) {
        validTest = adapter->CheckBaseNetworkingData();
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestSetBaseData() {
    bool validTest = EnvironmentSetup(
        false,
        true,
        "testsetbasedata",
        10000,
        10,
        false
    );

    if(validTest) {
        //Try and emulate what the real user of the adapter (ProfinetDataSource)
        //manupulates values to fill the handle

        MARTe::uint16 vendorId = 0x0102;
        MARTe::uint8 vendorIdHigh = 0x01;
        MARTe::uint8 vendorIdLow = 0x02;

        MARTe::uint16 deviceId = 0x0304;
        MARTe::uint8 deviceIdHigh = 0x03;
        MARTe::uint8 deviceIdLow = 0x04;

        MARTe::uint16 oemVendorId = 0x0506;
        MARTe::uint8 oemVendorIdHigh = 0x05;
        MARTe::uint8 oemVendorIdLow = 0x06;

        MARTe::uint16 oemDeviceId = 0x0708;
        MARTe::uint8 oemDeviceIdHigh = 0x07;
        MARTe::uint8 oemDeviceIdLow = 0x08;
        MARTe::StreamString productName = "productname";

        adapter->SetBaseData(
            static_cast<MARTe::uint8>((vendorId >> 8) & 0x00FF), 
            static_cast<MARTe::uint8>((vendorId) & 0x00FF), 
            static_cast<MARTe::uint8>((deviceId >> 8) & 0x00FF), 
            static_cast<MARTe::uint8>((deviceId) & 0x00FF), 
            static_cast<MARTe::uint8>((oemVendorId >> 8) & 0x00FF), 
            static_cast<MARTe::uint8>((oemVendorId) & 0x00FF), 
            static_cast<MARTe::uint8>((oemDeviceId >> 8) & 0x00FF), 
            static_cast<MARTe::uint8>((oemDeviceId) & 0x00FF), 
            productName,
            32);

        pnet_cfg_t pnetCfg = adapter->GetProfinetConfigurationHandle();

        validTest = (pnetCfg.device_id.vendor_id_hi == vendorIdHigh) &&
                    (pnetCfg.device_id.vendor_id_lo == vendorIdLow) &&
                    (pnetCfg.device_id.device_id_hi == deviceIdHigh) &&
                    (pnetCfg.device_id.device_id_lo == deviceIdLow) &&
                    (pnetCfg.oem_device_id.vendor_id_hi == oemVendorIdHigh) &&
                    (pnetCfg.oem_device_id.vendor_id_lo == oemVendorIdLow) &&
                    (pnetCfg.oem_device_id.device_id_hi == oemDeviceIdHigh) &&
                    (pnetCfg.oem_device_id.device_id_lo == oemDeviceIdLow);
                    (pnetCfg.min_device_interval == 32);
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestSetIdentificationAndMaintainanceData() {
    
    bool validTest = EnvironmentSetup(
        false,
        true,
        "testsetiandm",
        10000,
        10,
        false
    );

    if(validTest) {
        MARTe::uint8 vendorIdDataHigh = 0x01;
        MARTe::uint8 vendorIdDataLow = 0x02;
        MARTe::uint16 hardwareRevision = 0x01;
        ProfinetDataSourceDriver::softwarerevision_t softwareRevision = ProfinetDataSourceDriver::SwRev_V;
        MARTe::uint8 functionalEnhancement = 1;
        MARTe::uint8 bugFix = 2;
        MARTe::uint8 internalChange = 3;
        MARTe::uint16 profileIdentifier = 0x1234;
        MARTe::uint16 profileSpecificType = 0x5678;
        MARTe::uint8 versionMajor = 1;
        MARTe::uint8 versionMinor = 2;
        MARTe::StreamString orderId = "orderId";
        MARTe::StreamString serialNumber = "0001";
        MARTe::StreamString tagFunction = "tagFunction";
        MARTe::StreamString tagLocation = "tagLocation";;
        MARTe::StreamString date = "19-01-2020";
        MARTe::StreamString descriptor = "tagDescriptor";
        MARTe::StreamString signature = "tagSignature";

        adapter->SetIdentificationAndMaintainanceData(
            vendorIdDataHigh, 
            vendorIdDataLow, 
            hardwareRevision, 
            softwareRevision, 
            functionalEnhancement, 
            bugFix, 
            internalChange, 
            profileIdentifier, 
            profileSpecificType, 
            versionMajor, 
            versionMinor,
            orderId,
            serialNumber,
            tagFunction,
            tagLocation,
            date,
            descriptor,
            signature);

        pnet_cfg_t pnetCfg = adapter->GetProfinetConfigurationHandle();

        MARTe::StreamString orderIdFromHandle = pnetCfg.im_0_data.im_order_id;
        MARTe::StreamString serialNumFromHandle = pnetCfg.im_0_data.im_serial_number;
        MARTe::StreamString tagFunctionFromHandle = pnetCfg.im_1_data.im_tag_function;
        MARTe::StreamString tagLocationFromHandle = pnetCfg.im_1_data.im_tag_location;
        MARTe::StreamString tagDateFromHandle = pnetCfg.im_2_data.im_date;
        MARTe::StreamString tagDescriptorFromHandle = pnetCfg.im_3_data.im_descriptor;
        MARTe::StreamString tagSignatureFromHandle = pnetCfg.im_4_data.im_signature;
        
        validTest = validTest &&
                    (vendorIdDataHigh == pnetCfg.im_0_data.im_vendor_id_hi) &&
                    (vendorIdDataLow == pnetCfg.im_0_data.im_vendor_id_lo) &&
                    (hardwareRevision == pnetCfg.im_0_data.im_hardware_revision) &&
                    (softwareRevision == pnetCfg.im_0_data.im_sw_revision_prefix) &&
                    (functionalEnhancement == pnetCfg.im_0_data.im_sw_revision_functional_enhancement) &&
                    (bugFix == pnetCfg.im_0_data.im_sw_revision_bug_fix) &&
                    (internalChange == pnetCfg.im_0_data.im_sw_revision_internal_change) &&
                    (pnetCfg.im_0_data.im_revision_counter == 0) &&
                    (profileIdentifier == pnetCfg.im_0_data.im_profile_id) &&
                    (profileSpecificType == pnetCfg.im_0_data.im_profile_specific_type) &&
                    (versionMajor == pnetCfg.im_0_data.im_version_major) &&
                    (versionMinor == pnetCfg.im_0_data.im_version_minor) &&
                    (pnetCfg.im_0_data.im_supported == 0x001e) &&
                    (serialNumber == serialNumFromHandle) &&
                    (orderId == orderIdFromHandle) &&
                    (tagFunction == tagFunctionFromHandle) &&
                    (tagLocation == tagLocationFromHandle) &&
                    (date == tagDateFromHandle) &&
                    (descriptor == tagDescriptorFromHandle) &&
                    (signature == tagSignatureFromHandle);
    }
    
    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestSetLLDPData() {

    bool validTest = EnvironmentSetup(
        false,
        true,
        "testsetlldp",
        10000,
        10,
        false
    );

    if(validTest) {
        MARTe::uint16 rtClass2Status = 0;
        MARTe::uint16 rtClass3Status = 0;

        adapter->SetLLDPData(rtClass2Status, rtClass3Status);
        pnet_cfg_t pnetCfg = adapter->GetProfinetConfigurationHandle();

        validTest = validTest &&
                    (rtClass2Status == pnetCfg.if_cfg.ports[0].rtclass_2_status) &&
                    (rtClass3Status == pnetCfg.if_cfg.ports[0].rtclass_3_status);
    }

    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestSlotAndSubslotManagement() {

    bool validTest = EnvironmentSetup(
        false,
        true,
        "testsetslotsubslot",
        10000,
        10,
        false
    );

    if(validTest) {
        for(int i = 0; i < TEST_NUMBER_OF_SLOTS; i++) {
            adapter->AddSlot(i);
            adapter->AddSubslot(i, 1, true, ProfinetDataSourceDriver::NoInputOutput, 0, 0);
            adapter->AddSubslot(i, 2, false, ProfinetDataSourceDriver::Input, 10, 0);
            adapter->AddSubslot(i, 3, false, ProfinetDataSourceDriver::Output, 0, 10);
            adapter->AddSubslot(i, 4, false, ProfinetDataSourceDriver::InputOutput, 10, 10);
        }

        ProfinetDataSourceDriver::pnet_subslot_t* tempSubslot = NULL;

        bool testResult = true;

        for(int i = 0; i < TEST_NUMBER_OF_SLOTS; i++) {
            tempSubslot = adapter->GetSubslot(i, 1);
            validTest = validTest && tempSubslot->expectedConfigurationData.insize == 0;
            validTest = validTest && tempSubslot->expectedConfigurationData.outsize == 0;
            validTest = validTest && tempSubslot->expectedConfigurationData.data_dir == PNET_DIR_NO_IO;

            tempSubslot = adapter->GetSubslot(i, 2);
            validTest = validTest && tempSubslot->expectedConfigurationData.insize == 10;
            validTest = validTest && tempSubslot->expectedConfigurationData.outsize == 0;
            validTest = validTest && tempSubslot->expectedConfigurationData.data_dir == PNET_DIR_INPUT;

            tempSubslot = adapter->GetSubslot(i, 3);
            validTest = validTest && tempSubslot->expectedConfigurationData.insize == 0;
            validTest = validTest && tempSubslot->expectedConfigurationData.outsize == 10;
            validTest = validTest && tempSubslot->expectedConfigurationData.data_dir == PNET_DIR_OUTPUT;

            tempSubslot = adapter->GetSubslot(i, 4);
            validTest = validTest && tempSubslot->expectedConfigurationData.insize == 10;
            validTest = validTest && tempSubslot->expectedConfigurationData.outsize == 10;
            validTest = validTest && tempSubslot->expectedConfigurationData.data_dir == PNET_DIR_IO;
        }
    }
    
    return validTest;
}

bool ProfinetDataSourceAdapterTest::TestInitialize() {
    
    bool validTest = EnvironmentSetup(
        false,
        true,
        "testinitialize",
        10000,
        10,
        false
    );

    if(validTest) {
        adapter->SetBaseData(0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, "NAME", 32);
        adapter->SetIdentificationAndMaintainanceData(
            0x01,
            0x02, 
            0x0003, 
            ProfinetDataSourceDriver::SwRev_V, 
            0x05, 
            0x06, 
            0x07, 
            0x0008, 
            0x0009, 
            0x0A, 
            0x0B,
            "orderid",
            "0001",
            "Func",
            "Loc",
            "19-01-2021",
            "Desc",
            "Sig");

        adapter->SetLLDPData(0, 0);

        validTest = adapter->Initialize();
    }
    
    return validTest;
}


