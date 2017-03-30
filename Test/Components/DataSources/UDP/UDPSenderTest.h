/**
 * @file UDPSenderTest.h
 * @brief Header file for class UDPSenderTest
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

 * @details This header file contains the declaration of the class UDPSenderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UDPSenderTEST_H_
#define UDPSenderTEST_H_

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
 * @brief Tests the UDPSenderTest public methods.
 */
class UDPSenderTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();
    
    /**
     * @brief Tests the data configuration when no input is made
     */
    bool TestInitialise_Empty();

    /**
     * @brief Tests the data configuration when a valid port is input
     */
    bool TestInitialise_Port();
    
    /**
     * @brief Tests the data configuration when a valid port is input, however required administator privileges to use
     */
    bool TestInitialise_Restricted_Port();

    /**
     * @brief Tests the data configuration when an empty port is entered
     */
    bool TestInitialise_False_Port_1();

    /**
     * @brief Tests the data configuration when a port number that is too large is entered
     */
    bool TestInitialise_False_Port_2();

    /**
     * @brief Tests the data configuration when a port number that is too small (negative number) is entered
     */
    bool TestInitialise_False_Port_3();

    /**
     * @brief Tests the data configuration when text is inputted as a port number
     */
    bool TestInitialise_False_Port_4();

    /**
     * @brief Tests the data configuration when a correct (not infinite) timeout has be inputted
     */
    bool TestInitialise_TargetAddress();

    
    bool TestInitialise_False_TargetAddress_1();

    
    bool TestInitialise_False_TargetAddress_2();

    
    bool TestInitialise_False_TargetAddress_3();

    
    bool TestInitialise_False_TargetAddress_4();

    
    bool TestInitialise_False_TargetAddress_5();

    /**
     * @brief Tests the database configures correctly with a correct standard, minimum configuration
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the database configures correctly with a correct standard configuration, with more than minimum number of signals
     */
    bool TestSetConfiguredDatabase_More_Minimum_Signals();

    /**
     * @brief Tests the database configuration with no data signals defined
     */
    bool TestSetConfiguredDatabase_False_NOfSignals();


    /**
     * @brief Tests the database configuration with the first signal being defined at an incorrect size (16 bit)
     */
    bool TestSetConfiguredDatabase_Incorrect_Signal_Size1();

    /**
     * @brief Tests the database configuration with the second signal being defined at an incorrect size (16 bit)
     */
    bool TestSetConfiguredDatabase_Incorrect_Signal_Size2();

    /**
     * @brief Tests the database configuration with the first signal being defined as a incorrect type (float)
     */
    bool TestSetConfiguredDatabase_Incorrect_Signal_Type1();

    /**
     * @brief Tests the database configuration with the second signal being defined as a incorrect type (float)
     */
    bool TestSetConfiguredDatabase_Incorrect_Signal_Type2();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the AllocateMemory method, when incorrect data is passed through
     */
    bool TestAllocateMemory_False();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the GetBrokerName when passed input signals.
     */
    bool TestGetBrokerName_InputSignals();

    /**
     * @brief Tests the GetBrokerName when passed output signals.
     */
    bool TestGetBrokerName_OutputSignals();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

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

#endif /* UDPSenderTEST_H_ */