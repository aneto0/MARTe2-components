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
        bool EnvironmentSetup();

    public:
        bool TestConstructor();

         bool TestSetBaseData();

        bool TestSetIdentificationAndMaintainanceData();
        
        // bool TestSetLLDPData();

        // bool TestAddSlot();

        // bool TestAddSubslot();

        // bool TestGetSubslot();

        // bool TestInitialize();

        // bool TestMainThread();

        // bool TestTaskTimerTick();

        // bool TestGetLedStatus();

        // bool TestGetDefaultLLDPRTClass2Status();

        // bool TestGetDefaultLLDPRTClass3Status();

        // bool TestGetDefaultLLDPAutonegotiationCapability();

        // bool TestGetDefaultLLDPAutonegotiationSpeed();

        // bool TestGetDefaultLLDPMAUType();

        // bool TestIPv4U32toQuadU8();

};
#endif /* __PROFINET_DATASOURCE_ADAPTER_TEST_H__ */ 