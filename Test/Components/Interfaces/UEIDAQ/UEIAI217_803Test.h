/**
 * @file UEIAI217_803Test.h
 * @brief Header file for class UEIAI217_803Test
 * @date 22/03/2023
 * @author Xavier Ruche
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

 * @details This header file contains the declaration of the class UEIAI217_803Test
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIAI217_803_UEIAI217_803TEST_H_
#define UEIAI217_803_UEIAI217_803TEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEIAI217_803.h"
#include "UEIAI217_803Test.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
#include "UEIDAQMockup.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests all the UEIAI217_803 methods
 */
class UEIAI217_803Test : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEIAI217_803Test();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEIAI217_803Test();

        /**
        * @brief Tests the UEIAI217_803 constructor
        */
        bool TestConstructor();
        bool TestFixedParameters();
        bool TestAcceptedSignalType();
        bool TestInitialise();
        bool TestInitialise_class();
        bool TestInitialise_NoGains();
        bool TestInitialise_InvalidGainNumber();
        bool TestInitialise_InvalidGains();
        bool TestInitialise_ADCModeEnhanced();
        bool TestInitialise_ADCModeDefault();
        bool TestInitialise_InvalidADCMode();
        bool TestInitialise_NoADCMode();
        bool TestInitialise_NoFIRBanks();
        bool TestInitialise_NoFIRBankDefined();
        bool TestInitialise_ManyFIRTaps();
        bool TestInitialise_InvalidDefaultSetting();
        bool TestInitialise_InvalidFIRtSetting_CustomAndDefault();
        bool TestCheckChannelAndDirection();
        bool TestConfigureChannel();
        bool TestSignalScaling();
        bool TestConfigureDevice();
        bool TestConfigureDevice_SetCfgLayerError();
        bool TestConfigureDevice_SetDefaultError();
        bool TestConfigureDevice_SetBank1Error();
        bool TestConfigureDevice_FIRTapsWarning();
        
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIAI217_803_UEIAI217_803TEST_H_ */

