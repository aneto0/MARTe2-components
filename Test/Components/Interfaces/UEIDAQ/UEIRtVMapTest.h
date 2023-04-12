/**
 * @file UEIRtVMapTest.h
 * @brief Header file for class UEIRtVMapTest
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

 * @details This header file contains the declaration of the class UEIRtVMapest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIRtVMap_UEIRtVMapTEST_H_
#define UEIRtVMap_UEIRtVMapTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEIDevice.h"
#include "UEIAI217_803.h"
#include "UEIRtVMap.h"
#include "SafeMath.h"
#include "UEIDAQMockup.h"
#include "GlobalObjectsDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the UEIRtVMap methods
 */
class UEIRtVMapTest : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEIRtVMapTest();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEIRtVMapTest();

        /**
        * @brief Tests the UEIRtVMap constructor
        */
        bool TestConstructor();
        bool TestInitialise();
        bool TestInitialiseNoSamples();
        bool TestInitialiseNumberOfBuffers();
        bool TestConfigureInputsForDataSource();
        bool TestInitialise_TrueClass();
        bool TestEnableMap();
        bool TestResetMap();
        bool TestStartMap();
        bool TestPollForNewPacket();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIRtVMap_UEIRtVMapTEST_H_ */

