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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>

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

bool ProfinetDataSourceAdapterTest::EnvironmentSetup() {
    char* ethInterface = getenv("PROFINET_ETH_INTERFACE");
    if(ethInterface == NULL) {
        return false;
    }
    else {
        return true;
    }
}

bool ProfinetDataSourceAdapterTest::TestConstructor() {
    ProfinetDataSourceDriver::ILoggerAdapter *log = new MARTe::ProfinetToMARTeLogAdapter();
    ProfinetDataSourceDriver::ProfinetDataSourceAdapter *adapter = new ProfinetDataSourceDriver::ProfinetDataSourceAdapter("dummyeth", 10000, "dummy-dev", 10, log);

    delete log;
    delete adapter;

    return true;
}

bool ProfinetDataSourceAdapterTest::TestSetBaseData() {
    ProfinetDataSourceDriver::ILoggerAdapter *log = new MARTe::ProfinetToMARTeLogAdapter();
    ProfinetDataSourceDriver::ProfinetDataSourceAdapter *adapter = new ProfinetDataSourceDriver::ProfinetDataSourceAdapter("dummyeth", 10000, "dummy-dev", 10, log);

    MARTe::uint8 vendorIdHigh = 0x01;
    MARTe::uint8 vendorIdLow = 0x02;
    MARTe::uint8 deviceIdHigh = 0x03;
    MARTe::uint8 deviceIdLow = 0x04;
    MARTe::uint8 oemVendorIdHigh = 0x05;
    MARTe::uint8 oemVendorIdLow = 0x06;
    MARTe::uint8 oemDeviceIdHigh = 0x07;
    MARTe::uint8 oemDeviceIdLow = 0x08;
    MARTe::StreamString vendor = "dummy-device-vendor";
    MARTe::StreamString manufacturer = "dummy-device-manufacturer";
    MARTe::uint16 minimumDeviceInterval = 1;

    adapter->SetBaseData(vendorIdHigh, vendorIdLow, deviceIdHigh, deviceIdLow, oemVendorIdHigh, oemVendorIdLow, oemDeviceIdHigh, oemDeviceIdLow, vendor, manufacturer, 1);

    pnet_cfg_t pnetCfg = adapter->GetProfinetConfigurationHandle();

    MARTe::StreamString vendorFromHandle = pnetCfg.device_vendor;
    MARTe::StreamString manufacturerFromHandle = pnetCfg.manufacturer_specific_string;

    bool validBaseData =    (pnetCfg.device_id.vendor_id_hi == vendorIdHigh) &&
                            (pnetCfg.device_id.vendor_id_lo == vendorIdLow) &&
                            (pnetCfg.device_id.device_id_hi == deviceIdHigh) &&
                            (pnetCfg.device_id.device_id_lo == deviceIdLow) &&
                            (pnetCfg.oem_device_id.vendor_id_hi == oemVendorIdHigh) &&
                            (pnetCfg.oem_device_id.vendor_id_lo == oemVendorIdLow) &&
                            (pnetCfg.oem_device_id.device_id_hi == oemDeviceIdHigh) &&
                            (pnetCfg.oem_device_id.device_id_lo == oemDeviceIdLow) &&
                            (vendorFromHandle == vendor) &&
                            (manufacturerFromHandle == manufacturer) &&
                            (pnetCfg.min_device_interval == minimumDeviceInterval);
    
    delete log;
    delete adapter;

    return validBaseData;
}

bool ProfinetDataSourceAdapterTest::TestSetIdentificationAndMaintainanceData() {
    ProfinetDataSourceDriver::ILoggerAdapter *log = new MARTe::ProfinetToMARTeLogAdapter();
    ProfinetDataSourceDriver::ProfinetDataSourceAdapter *adapter = new ProfinetDataSourceDriver::ProfinetDataSourceAdapter("dummyeth", 10000, "dummy-dev", 10, log);

    MARTe::uint8 vendorIdDataHigh = 0x01;
    MARTe::uint8 vendorIdDataLow = 0x01;
    MARTe::uint16 hardwareRevision = 0x01;
    ProfinetDataSourceDriver::softwarerevision_t softwareRevision = ProfinetDataSourceDriver::SwRev_V;
    MARTe::uint8 functionalEnhancement = 1;
    MARTe::uint8 bugFix = 2;
    MARTe::uint8 internalChange = 3;
    MARTe::uint16 profileIdentifier = 0x1234;
    MARTe::uint16 profileSpecificType = 0x5678;
    MARTe::uint8 versionMajor = 1;
    MARTe::uint8 versionMinor = 2;
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
        tagFunction,
        tagLocation,
        date,
        descriptor,
        signature);

    bool validIMData = true;

    delete log;
    delete adapter;

    return validIMData;
}