/**
 * @file UEIRtDMaprTest.h
 * @brief Header file for class UEIRtDMapTest
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

 * @details This header file contains the declaration of the class UEIRtDMaprest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIRtDMap_UEIRtDMapTEST_H_
#define UEIRtDMap_UEIRtDMapTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEIDevice.h"
#include "UEIAI217_803.h"
#include "UEIRtDMap.h"
#include "SafeMath.h"
#include "UEIDAQMockup.h"
#include "GlobalObjectsDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the UEIRtDMap methods
 */
class UEIRtDMapTest : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEIRtDMapTest();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEIRtDMapTest();

        /**
        * @brief Tests the UEIRtDMap constructor
        */
        bool TestConstructor();
        bool TestInitialise();
        bool TestInitialise_TrueClass();
        bool TestInitialise_NoScanRate();
        bool TestInitialise_IndependentThread();
        bool TestConfigureInputsForDataSource();
        bool TestStartMap();
        bool TestCoherencyCheck();
        bool TestCoherencyCheckFail();
        bool TestCoherencyCheckHighScanRate();
        bool TestPollForNewPacket();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIRtDMap_UEIRtDMapTEST_H_ */

