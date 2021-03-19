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

#ifndef DATASOURCES_PROFINET_ADAPTER_LINT_H
#define DATASOURCES_PROFINET_ADAPTER_LINT_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "ProfinetDataSourceConstants.h"

#ifndef LINT
#include <cstring>
#include <iomanip>
#include <iostream>
#endif

#include <pnal.h>
#include <pnet_api.h>

#ifndef LINT
#include <sstream>
#include <unistd.h>
#endif

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "ICyclicNotifiable.h"
#include "ILoggerAdapter.h"
#include "IOperationalSignalsEntryPoint.h"
#include "IProfinetEventNotifiable.h"
#include "MutexSem.h"
//#include "ProfinetDataSourceTypes.h"
#include "ProfinetEventType.h"
#include "StreamString.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace ProfinetDataSourceDriver
{
	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_INTERFACE_NAME_SIZE    = 16;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_STATION_NAME_SIZE      = 241;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_SUPPORTED_SUBSLOTS     = 5;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_PRODUCTNAME_SIZE       = 26;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_IM0_ORDERID_SIZE       = 21;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_IM0_SERNUM_SIZE        = 17;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_IM1_TAGFUNC_SIZE       = 33;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_IM1_TAGLOC_SIZE        = 23;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_IM2_DATE_SIZE          = 17;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_IM3_DESC_SIZE          = 55;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_MAX_IM4_SIGN_SIZE          = 54;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_RETURN_VALUE_ERROR 		  = -1;

	/**
 	* @brief Mock for linting purposes, please refer to the specific class 
 	*/ 	
	const int32_t PNDS_RETURN_VALUE_SUCCESS 	  = 0;
    /**
     * @brief   Implements an intermediate adapter between the MARTe DataSource and the Profinet library.
     *          As the p-net library is implemented in C, the library is first 1:1 mapped into C++.
     *          Convenient function for library and bus management are provided to upper layers, with
     *          the MARTe DataSource approach in mind.
     */
    class ProfinetDataSourceAdapter {

        private:

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            ProfinetDataSourceAdapter();

    	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            pnet_api_t      apiStructure;

        public:

    	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            ~ProfinetDataSourceAdapter();

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            bool CheckBaseNetworkingData();

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            ProfinetDataSourceAdapter(
                std::string     ethInterface, 
                uint32_t        periodicInterval,
                std::string     stationName,
                uint32_t        gearRatio,
                ILoggerAdapter  *loggerAdapter
            );

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
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
 	    * @brief Mock for linting purposes, please refer to the specific class 
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
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            void SetLLDPData(
                MARTe::uint16           rtClass2Status,
                MARTe::uint16           rtClass3Status
            );

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            void AddSlot(
                MARTe::uint16 slotNumber);

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            void AddSubslot(
                MARTe::uint16               slotNumber, 
                MARTe::uint16               subslotNumber,
                bool                        isDeviceAccessPoint,
                pnet_submodule_direction_t  expectedDataDirection,
                MARTe::uint16               expectedInputSize,
                MARTe::uint16               expectedOutputSize);

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            pnet_subslot_t* GetSubslot(
                MARTe::uint16 slotNumber,
                MARTe::uint16 subslotNumber);

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            SimpleLinkedListIterator<pnet_slot_t*>* GetAPIStructureIterator() { return apiStructure.slots.GetIterator(); }

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            bool Initialize();

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            MARTe::uint16 MainThread(MARTe::uint16 inputFlagMask);

	    /**
 	     * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            bool AbortConnection();

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            void TaskTimerTick();

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            bool GetLedStatus();

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            ICyclicNotifiable *cyclicNotificationListener;

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            MARTe::IProfinetEventNotifiable *profinetEventNotificationListener;

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            MARTe::IOperationalSignalsEntryPoint *opSignalsEntryPoint;

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            static MARTe::uint16 GetDefaultLLDPRTClass2Status() const { return 0u; }

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            static MARTe::uint16 GetDefaultLLDPRTClass3Status() const { return 0u; }

	    /**
 	    * @brief Mock for linting purposes, please refer to the specific class 
 	    */ 	
            static void IPv4U32toQuadU8(
                pnal_ipaddr_t     inputIpAddress, 
                uint8_t         *a, 
                uint8_t         *b, 
                uint8_t         *c, 
                uint8_t         *d
            );

    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCES_PROFINET_ADAPTER_LINT_H */

