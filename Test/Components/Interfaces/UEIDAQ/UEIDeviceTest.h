/**
 * @file UEIDeviceTest.h
 * @brief Header file for class UEIDeviceTest
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

 * @details This header file contains the declaration of the class UEIDeviceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIDEVICE_UEIDEVICETEST_H_
#define UEIDEVICE_UEIDEVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEIDevice.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests all the UEIDevice methods
 */
class UEIDeviceTest : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEIDeviceTest();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEIDeviceTest();

        /**
        * @brief Tests the UEIDevice constructor
        */
        bool TestConstructor();

        /**
        * @brief Tests the UEIDevice methods which must be redefined by child classes, checks default returns.
        */
        bool TestBaseMethods();

        /**
        * @brief Tests the HardwareCorrespondence flag setting procedure.
        */
        bool TestHardwareCorrespondence();

        /**
        * @brief Tests the MapAssignment flag setting procedure.
        */
        bool TestMapAssignment();
        
        /**
        * @brief Tests a successful initialisation procedure for UEIDevice.
        */
        bool TestInitialise();
        
        /**
        * @brief Tests a fail in Initialisation method for UEIDevice due to no Devn parameter provided
        */
        bool TestInitialise_NoDevn();

        /**
        * @brief Tests a fail in Initialisation method for UEIDevice due to Devn parameter with invalid value (>MAX_IO_SLOTS)
        */
        bool TestInitialise_InvalidDevn();
        
        /**
        * @brief Tests a fail in Initialisation method for UEIDevice due to no SamplingFrequency parameter provided
        */
        bool TestInitialise_NoFrequency();

        /**
        * @brief Tests CheckChannelAndDirection method expecting a list of channels
        */
        bool TestCheckChannelAndDirection_List(); 

        bool TestSetChannelList();       
        bool TestAnyTypeToBoolean();
        bool TestFindChannelIndex();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIDEVICE_UEIDEVICETEST_H_ */

