/**
 * @file UDPReceiverTest.h
 * @brief Header file for class UDPReceiverTest
 * @date 25/10/2016
 * @author Andre Neto
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

#ifndef UDPReceiverTEST_H_
#define UDPReceiverTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the UDPReceiverTest public methods.
 */
class UDPReceiverTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    bool TestInitialise_Empty();

    bool TestInitialise_Port();

    bool TestInitialise_False_Port_1();

    bool TestInitialise_False_Port_2();

    bool TestInitialise_False_Port_3();

    bool TestInitialise_False_Port_4();

    bool TestSetConfiguredDatabase();
    
    bool TestSetConfiguredDatabase_More_Minimum_Signals();

    bool TestSetConfiguredDatabase_False_NOfSignals();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    bool TestAllocateMemory_False();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    bool TestGetBrokerName_InputSignals();

    bool TestGetBrokerName_OutputSignals();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UDPReceiverTEST_H_ */

