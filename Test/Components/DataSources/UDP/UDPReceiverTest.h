/**
 * @file UDPReceiverTest.h
 * @brief Header file for class UDPReceiverTest
 * @date 10/06/2021
 * @author Luca Porzio
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

 * @details This header file contains the declaration of the class UDPReceiverTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_UDP_UDPRECEIVERTEST_H_
#define TEST_COMPONENTS_DATASOURCES_UDP_UDPRECEIVERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class UDPReceiverTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method without Address.
     */
    bool TestInitialise_No_Address();

    /**
     * @brief Tests the Initialise method without Port.
     */
    bool TestInitialise_No_Port();

    /**
     * @brief Tests the Initialise method with forbidden port number.
     */
    bool TestInitialise_Forbidden_Port();

    /**
     * @brief Tests the Initialise method without Timeout.
     */
    bool TestInitialise_No_Timeout();

    /**
     * @brief Tests the Initialise method without ExecutionMode set.
     */
    bool TestInitialise_No_ExecutionMode();

    /**
     * @brief Tests the Initialise method without ExecutionMode set and No CPUMask set.
     */
    bool TestInitialise_No_ExecutionMode_No_Cpu();

    /**
     * @brief Tests the Initialise method without ExecutionMode set and No StackSize set.
     */
    bool TestInitialise_No_ExecutionMode_No_StackSize();

    /**
     * @brief Tests the Initialise method with InterfaceAddress.
     */
    bool TestInitialise_InterfaceAddress();

    /**
     * @brief Tests the Initialise method with ExecutionMode == IndependentThread.
     */
    bool TestInitialise_IndependentThread();

    /**
     * @brief Tests the Initialise method with a wrong ExecutionMode
     */
    bool TestInitialise_Wrong_ExecutionMode();

    /**
     * @brief Tests the SetConfiguredDatabase method with Address for multicast.
     */
    bool TestSetConfiguredDatabase_ValidAddress();

    /**
     * @brief Tests the SetConfiguredDatabase method with invalid Address for multicast.
     */
    bool TestSetConfiguredDatabase_InvalidAddress();

    /**
     * @brief Tests the SetConfiguredDatabase method with an InterfaceAddress for multicast binding.
     */
    bool TestSetConfiguredDatabase_MulticastAddress_Interface();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the PrepareNextState method
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Synchronise method
     */
    bool TestSynchronise();

    /**
     * @brief Tests the Execute method
     */
    bool TestExecute_Timeout();

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_UDP_UDPRECEIVERTEST_H_ */
	
