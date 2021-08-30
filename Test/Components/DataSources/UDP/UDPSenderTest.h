/**
 * @file UDPSenderTest.h
 * @brief Header file for class UDPSenderTest
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

 * @details This header file contains the declaration of the class UDPSenderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_COMPONENTS_DATASOURCES_UDP_UDPSENDERTEST_H_
#define TEST_COMPONENTS_DATASOURCES_UDP_UDPSENDERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class UDPSenderTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method without NumberOfPreTriggers.
     */
    bool TestInitialise_No_NumberOfPreTriggers();

    /**
     * @brief Tests the Initialise method without NumberOfPostTriggers.
     */
    bool TestInitialise_No_NumberOfPostTriggers();

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
     * @brief Tests the Initialise method without CpuMask.
     */
    bool TestInitialise_No_CpuMaks();

    /**
     * @brief Tests the Initialise method without StackSize.
     */
    bool TestInitialise_No_StackSize();

    /**
     * @brief Tests the Initialise method with zero StackSize.
     */
    bool TestInitialise_StackSize_Zero();

    /**
     * @brief Tests the Initialise method without Trigger Signal.
     */
    bool TestInitialise_No_TriggerSignal();

    /**
     * @brief Tests the Initialise method without Counter Signal.
     */
    bool TestInitialise_No_CounterSignal();

    /**
     * @brief Tests the Initialise method without Time Signal.
     */
    bool TestInitialise_No_TimeSignal();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise_Correct();

    /**
     * @brief Tests the SetConfiguredDatabase method with wrong Counter Type.
     */
    bool TestSetConfiguredDatabase_Wrong_CounterType();

    /**
     * @brief Tests the GetInputBrokers method
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the GetBrokerName method
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
     * @brief Tests the SetConfiguredDatabase method with wrong Time Type.
     */
    bool TestSetConfiguredDatabase_Wrong_TimeType();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase_Correct();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_COMPONENTS_DATASOURCES_UDP_UDPSENDERTEST_H_ */

