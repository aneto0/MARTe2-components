/**
 * @file ProfinetDataSourceAdapterTest.h
 * @brief Header file for class ProfinetDataSourceAdapterTest
 * @date 18/01/2021
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

 * @details This header file contains the declaration of the class ProfinetDataSourceAdapterTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef __PROFINET_DATASOURCE_ADAPTER_TEST_H__
#define __PROFINET_DATASOURCE_ADAPTER_TEST_H__


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ProfinetToMARTeLogAdapter.h"
#include "ILoggerAdapter.h"
#include "ProfinetDataSourceAdapter.h"
#include "ProfinetDataSourceAdapterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
class ProfinetDataSourceAdapterTest {
    private:
        ProfinetDataSourceDriver::ILoggerAdapter *log;
        ProfinetDataSourceDriver::ProfinetDataSourceAdapter *adapter;

    public:
        /**
         * @brief Constructs a test instance, initializing bases to null
         */
        ProfinetDataSourceAdapterTest();

        /**
         * @brief Destructs the test instance, removing also bases
         */
        ~ProfinetDataSourceAdapterTest();

        /**
         * @brief Prepares the environment adapter and log, eventually getting
         * from the environment the network interface which will be used for the 
         * testing.
         * @param[in] getIfNameFromEnv true if the environment variable PROFINET_ETH_INTERFACE
         * has to be used for the network interface. Otherwise a dummy if will be used.
         */
        bool EnvironmentSetup(
            bool emptyIfName = false,
            bool getIfNameFromEnv = false,
            std::string stationName = "dummystation",
            uint32_t periodicInterval = 10000,
            uint32_t gearRatio = 10,
            bool nullLogAdapter = false);

        /**
         * @brief Tests the constructor
         */
        bool TestConstructor();

        /**
         * @brief Tests if the constructor handles well a provided NULL network adapter
         */
        bool TestConstructorFailEmptyLoggerAdapter();

        /**
         * @brief Tests if the constructor handles well a zero periodic interval
         */
        bool TestConstructorFailZeroPeriodicInterval();

        /**
         * @brief Tests if the constructor handles well an empty station name
         */
        bool TestConstructorFailEmptyStationName();

        /**
         *@brief Tests if the constructor handles well an empty network interface name
        */
        bool TestConstructorFailEmptyInterface();

        /**
         * @brief Tests if the constructor handles well a zero reduction ratio
         */
        bool TestConstructorFailZeroGearRatio();

        /**
         * @brief Tests if the constructor handles well a failure in network setup
         */
        bool TestConstructorFailNetworkDataSetup();

        /**
         * @brief Feeds base configuration data to the adapter, checking if they are 
         * effectively copied inside configuration handles
         */
        bool TestSetBaseData();

        /**
         * @brief Feeds I&M data to the adapter, checking if they are 
         * effectively copied inside configuration handles
         */
        bool TestSetIdentificationAndMaintainanceData();

        /**
         * @brief Feeds LLDP data to the adapter, checking if they are
         * effectively copied inside configuration handles
         */
        bool TestSetLLDPData();

        /**
         * @brief Tests the Slots and Subslot system management
         */
        bool TestSlotAndSubslotManagement();

        /**
         * @brief Initializes a real Profinet instance.
         * NOTE: Needs a real network interface and permissions to bind a raw socket.
         * Needs also the PROFINET_ETH_INTERFACE environment variable to be set.
         */
        bool TestInitialize();
        
        /**
         * @brief Assesses default RT Class 2 status
         */
        bool TestGetDefaultLLDPRTClass2Status();

        /**
         * @brief Assesses default RT Class 3 status
         */
        bool TestGetDefaultLLDPRTClass3Status();
};
#endif /* __PROFINET_DATASOURCE_ADAPTER_TEST_H__ */ 