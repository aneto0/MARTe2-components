/**
 * @file UEIMasterObjectTest.h
 * @brief Header file for class UEIMasterObjectTest
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

 * @details This header file contains the declaration of the class UEIMasterObjectTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIMasterObject_UEIMasterObjectTEST_H_
#define UEIMasterObject_UEIMasterObjectTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEIMasterObject.h"
#include "UEIMasterObjectTest.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
#include "UEIDAQMockup.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests all the UEIMasterObject methods
 */
class UEIMasterObjectTest : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEIMasterObjectTest();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEIMasterObjectTest();

        /**
        * @brief Tests the UEIMasterObject constructor
        */
        bool TestConstructor();    
        bool TestInitialise();
        bool TestInitialise_TrueClass();
        bool TestInitialise_NoConnectionTimeout();
        bool TestInitialise_NoIp();
        bool TestInitialise_BadIp();
        bool TestInitialise_RemoteIpNoPort();
        bool TestInitialise_RemoteIp();
        bool TestInitialise_NoDevices();
        bool TestInitialise_ZeroDevices();
        bool TestInitialise_TooManyDevices();
        bool TestInitialise_RepeatedDevices();
        bool TestInitialise_DaqLibError();
        bool TestInitialise_UnableToConnectToIOM();
        bool TestInitialise_BadHandle();
        bool TestInitialise_BadDevnHandle();
        bool TestInitialise_DevnModelMismatch();
        bool TestInitialise_DeviceConfiguredNotInstalled();
        bool TestInitialise_DeviceUnableToConfigure();
        bool TestInitialise_MapsDefined();
        bool TestInitialise_InconsistentMaps();
        bool TestInitialise_InvalidSetDevices();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIMasterObject_UEIMasterObjectTEST_H_ */

