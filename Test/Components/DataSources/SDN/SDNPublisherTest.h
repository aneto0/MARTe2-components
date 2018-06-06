/**
 * @file SDNPublisherTest.h
 * @brief Header file for class SDNPublisherTest
 * @date 12/12/2016
 * @author Bertrand Bauvir
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

 * @details This header file contains the declaration of the class LinuxTimerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SDNPUBLISHERTEST_H_
#define SDNPUBLISHERTEST_H_

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
 * @brief Tests the SDNPublisher public methods.
 */
class SDNPublisherTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method with an empty StructuredDataI.
     */
    bool TestInitialise_Empty();

    /**
     * @brief Tests the Initialise method with Default parameters.
     */
    bool TestInitialise_Default();

    /**
     * @brief Tests the Initialise method  with .
     */
    bool TestInitialise_Address();

    /**
     * @brief Tests the Initialise method  with .
     */
    bool TestInitialise_Address_mcast();

    /**
     * @brief Tests the Initialise method  with .
     */
    bool TestInitialise_Address_ucast();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_ShortForm();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_Missing_Topic();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_Missing_Interface();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_False_Interface_1();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_False_Interface_2();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_False_Address_1();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_False_Address_2();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_False_Address_3();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_False_Address_4();

    /**
     * @brief Tests the Initialise method with .
     */
    bool TestInitialise_False_Address_5();
#ifdef FEATURE_10840
    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise_SourcePort();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise_NetworkByteOrder();
#endif
    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the AllocateMemory method with an header.
     */
    bool TestAllocateMemory_Header();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory_False();

    /**
     * @brief Tests the AllocateMemory method with a wrong header size.
     */
    bool TestAllocateMemory_False_Header_Size();

#ifdef FEATURE_10840
    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory_SourcePort();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory_False_SourcePort();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory_NetworkByteOrder();
#endif
    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method without signals.
     */
    bool TestSetConfiguredDatabase_False_NOfSignals();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the GetSignalMemoryBuffer method with an invalid signal index.
     */
    bool TestGetSignalMemoryBuffer_False();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName_InputSignals();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName_OutputSignals();

    /**
     * @brief Tests the GetInputBrokers method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetInputBrokers method with an header.
     */
    bool TestGetInputBrokers_Header();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_1();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_2();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_3();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_4();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_5();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_6();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_7();

    /**
     * @brief Tests the GetOutputBrokers method.
     */
    bool TestGetOutputBrokers_8();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_False();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_MCAST_Topic_1();

    /**
     * @brief Tests the Synchronise method with an header.
     */
    bool TestSynchronise_MCAST_Topic_1_Header();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_MCAST_Topic_2();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_MCAST_Topic_3();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_MCAST_Topic_4();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_UCAST_Topic_1();
#ifdef FEATURE_10840
    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_NetworkByteOrder_Topic_1();

    /**
     * @brief Tests the Synchronise method with the header.
     */
    bool TestSynchronise_NetworkByteOrder_Topic_1_Header();


    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise_NetworkByteOrder_Topic_2();
#endif
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SDNPUBLISHERTEST_H_ */

