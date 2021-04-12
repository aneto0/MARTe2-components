/**
 * @file RealTimeThreadSynchronisationTest.h
 * @brief Header file for class RealTimeThreadSynchronisationTest
 * @date 21/04/2017
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

 * @details This header file contains the declaration of the class RealTimeThreadSynchronisationTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMETHREADSYNCHRONISATIONTEST_H_
#define REALTIMETHREADSYNCHRONISATIONTEST_H_

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
 * @brief Tests the RealTimeThreadSynchronisation public methods.
 */
class RealTimeThreadSynchronisationTest {
public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the AllocateMemory method
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers method
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the GetSignalMemoryBuffer method
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests that the GetBrokerName method correctly returns a MemoryMapInputBroker.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the GetInputBrokers method
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the GetOutputBrokers method
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the PrepareNextState method
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with a timeout different from the default timeout
     */
    bool TestInitialise_Timeout();

    /**
     * @brief Tests the GetSynchroniseTimeout method.
     */
    bool TestGetSynchroniseTimeout();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the SetConfiguredDatabase method with WaitForNext=1.
     */
    bool TestSetConfiguredDatabase_WaitForNext();

    /**
     * @brief Tests the SetConfiguredDatabase method without specifying any consumer or producer GAM.
     */
    bool TestSetConfiguredDatabase_NoInteraction();

    /**
     * @brief Tests the SetConfiguredDatabase method with only a producer but no consumer GAMs.
     */
    bool TestSetConfiguredDatabase_ProducerOnly();

    /**
     * @brief Tests the SetConfiguredDatabase method with no producer.
     */
    bool TestSetConfiguredDatabase_False_NoProducer();

    /**
     * @brief Tests the SetConfiguredDatabase method with more than one producer.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneProducer();

    /**
     * @brief Tests the SetConfiguredDatabase method without writing all the signals.
     */
    bool TestSetConfiguredDatabase_Warning_NotAllSignals();

    /**
     * @brief Tests the SetConfiguredDatabase method writing more than one sample.
     */
    bool TestSetConfiguredDatabase_False_MoreThanOneOutputSample();

    /**
     * @brief Tests the SetConfiguredDatabase reading a different number of samples from the same GAM.
     */
    bool TestSetConfiguredDatabase_False_DifferentInputSamplesFromSameFunction();

    /**
     * @brief Tests that the RealTimeThreads values are correctly synchronised by the DataSourceI
     */
    bool TestSynchronise();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMETHREADSYNCHRONISATIONTEST_H_ */

