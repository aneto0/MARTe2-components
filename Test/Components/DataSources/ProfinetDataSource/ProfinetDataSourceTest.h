/**
 * @file ProfinetDataSourceTest.h
 * @brief Header file for class ProfinetDataSourceTest
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

 * @details This header file contains the declaration of the class ProfinetDataSourceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_PROFINETDATASOURCETEST_H_
#define DATASOURCES_PROFINET_PROFINETDATASOURCETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "ConfigurationDatabase.h"

#include "ProfinetDataSourceConstants.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class ProfinetDataSourceTest {
    private:
        /**
         * @brief Enables changes in the configuration database, including removal of an element
         * @param[in] inputCdb The input configuration database
         * @param[in] absoluteParameterPath Absolute path of the element to find and patch
         * @param[in] parameterToPatch Name of the element to patch
         * @param[in] replacementValue Value to replace with or NULL to remove
         */
        bool PatchConfigurationDatabase(
            MARTe::ConfigurationDatabase& inputCdb,
            const MARTe::char8 * const absoluteParameterPath,
            const MARTe::char8 * const parameterToPatch,
            const MARTe::char8 * const replacementValue);
        
        /**
         * @brief Gets a plausible interface name, either from an hardcoded
         * or from the environment variable PROFINET_ETH_INTERFACE
         * @param[in] getIfNameFromEnv true if the value should be read from the PROFINET_ETH_INTERFACE env variable,
         * false if the interface name can be a dummy one.
         * @return A StreamString which contains the interface name
         */
        MARTe::StreamString GetEthInterfaceName(bool getIfNameFromEnv);


        bool RunIntegrated(MARTe::ConfigurationDatabase& inputCdb, bool withSignalValueCheck);

        /**
         * @brief Prepares the environment adapter and log, eventually getting
         * from the environment the network interface which will be used for the 
         * testing.
         * @param[in] getIfNameFromEnv true if the environment variable PROFINET_ETH_INTERFACE
         * has to be used for the network interface. Otherwise a dummy if will be used.
         */
        bool EnvironmentSetup(bool getIfNameFromEnv = false);

    public:
        /**
         * @brief Empty default cosntructor
         */
        ProfinetDataSourceTest();

        /**
         * @brief
         */
        bool TestStandaloneInitialise();

        /**
         * @brief Tries to initialize a ProfinetDataSource with wrong network parameters
         */
        bool TestStandaloneInitialiseFailBaseNet();

        /**
         * @brief Tries to initialize a ProfinetDataSource with wrong base identification parameters
         */
        bool TestStandaloneInitialiseFailBaseId();

        /**
         * @brief Tries to initialize a ProfinetDataSource with wrong Identification and Maintainance Data
         */
        bool TestStandaloneInitialiseFailInM();

        /**
         * @brief Tries to initialize a ProfinetDataSource with wrong LLDP parameters
         */
        bool TestStandaloneInitialiseFailLLDP();

        /**
         * @brief Tries to initialize a ProfinetDataSource where the whole Slots description section is missing 
         * from the MARTe2 configuration file
         */
        bool TestStandaloneInitialiseNoSlots();

        /**
         * @brief Tries to initialize a ProfinetDataSource where one of the Slots contains wrong initialization parameters
         */
        bool TestStandaloneInitialiseWrongSlots();

        /**
         * @brief Tries to initialize a ProfinetDataSource where one of the SLots has missing Subslots informations
         */
        bool TestStandaloneInitialiseNoSubslots();

        /**
         * @brief Tries to initialize a ProfinetDataSource with wrong signal direction specification
         */
        bool TestStandaloneInitialiseWrongDirection();

        /**
         * @brief Tries to initialize a ProfinetDataSource where one of the signals is missing the Slot/Subslot memory mapping
         */
        bool TestStandaloneInitialiseWrongCoordinates();

        /**
         * @brief Tries to initialize a ProfinetDataSource where the Slots section is totally empty
         */
        bool TestStandaloneInitialiseFailSlotsSection();

        /**
         * @brief Tries to initialize a ProfinetDataSource where the Signals section is totally empty
         */
        bool TestStandaloneInitialiseFailSignalsSection();

        /**
         * @brief Tries to initialize a ProfinetDataSource where the Timer helper service is not specified/configured
         */
        bool TestStandaloneInitialiseFailTimerHelperSection();

        /**
         * @brief Tries to initialize N ProfinetDataSources, sweeping over possible Software Revisions in configuration
         */
        bool TestStandaloneSweepSoftwareRevision();

        /**
         * @brief Tries to initialize a ProfinetDataSource where the Main helper service is not specified/configured
         */
        bool TestStandaloneInitialiseFailMainHelperSection();

        /**
         * @brief Initializes and runs a Profinet Slave without checking for data integrity
         */
        bool TestIntegratedRunProfinetSlave();

        /**
         * @brief Initializes and runs a Profinet Slave where module plugging is not matching the Master GSDML advertised
         */
        bool TestIntegratedRunFailModulePlugging();

        /**
         * @brief Initializes, runs and checks data integrity of the Profinet Slave by running a loopback on the PLC
         * NOTE: Requires a I->O copy of all the signals on the PLC (Master) side
         */
        bool TestIntegratedRunPLCLoopback();
};

#endif /* DATASOURCES_PROFINET_PROFINETDATASOURCETEST_H_ */