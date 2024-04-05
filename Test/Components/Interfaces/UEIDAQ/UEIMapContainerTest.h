/**
 * @file UEIMapContainerTest.h
 * @brief Header file for class UEIMapContainerTest
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

 * @details This header file contains the declaration of the class UEIMapContainerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UEIMAPCONTAINER_UEIMAPCONTAINERTEST_H_
#define UEIMAPCONTAINER_UEIMAPCONTAINERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "UEIDevice.h"
#include "UEIMapContainer.h"
#include "SafeMath.h"
#include "GlobalObjectsDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the UEIMapContainer methods
 */
class UEIMapContainerTest : public Object {
    public:
        /**
        * @brief Constructs the Test Unit environment basics
        */
        UEIMapContainerTest();

        /**
        * @brief Destructs the Test Unit environment
        */
        ~UEIMapContainerTest();

        /**
        * @brief Tests the UEIMapContainer constructor
        */
        bool TestConstructor();
        
        /**
        * @brief Tests the base class methods which must be implemented by
        * child classes to return safe values
        */
        bool TestBaseImplementations_Default();
        bool TestConfigurationSetter();
        /**
        * @brief Tests a simple configuration for initialisation success
        */
        bool TestInitialise();
        bool TestInitialise_General();
        bool TestInitialise_NoOutputs();
        bool TestInitialise_NoInputs();
        bool TestInitialise_NoDevices();
        bool TestInitialise_InvalidDevn();
        bool TestInitialise_NoDevn();
        bool TestInitialise_NegativeDevn();
        bool TestInitialise_RepeatedDevn();
        bool TestInitialise_NoChannels();
        bool TestInitialise_NegativeChannels();
        bool TestInitialise_DescendingChannels();
        bool TestInitialise_InvalidChannelType();
        bool TestInitialise_EmptyBlock();
        bool TestInitialise_TooManyOutputDevices();
        bool TestInitialise_TooManyInputDevices();
        bool TestInitialise_IndependentThread();
        bool TestInitialise_NoExecutionMode();
        bool TestInitialise_InvalidExecutionMode();
        bool TestDAQHandleSetter();
        bool TestTimestampMethods();
        bool TestDSRegistration();
        bool TestSetDevice();
        bool TestSetDevices();
        bool TestSignalAllowed();
        bool TestMapCoherency();
        bool TestMapCoherency_InvalidInputChannels();
        bool TestMapCoherency_InvalidOutputChannels();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UEIMAPCONTAINER_UEIMAPCONTAINERTEST_H_ */

